/**
  ******************************************************************************
  * File Name          : tdc.c
  * Description        : tdc program body
  ******************************************************************************
*/

/* Include Functions ---------------------------------------------------------*/
#include "main_bg.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "variable.h"
#include "modbus.h"
#include "ringBuffer.h"
#include "backup.h"
#include "display.h"
#include "wifiTask.h"
#include "multicastLudp.h"
#include "httpServer.h"
#include "backup.h"
#include "timer.h"
#include "modbusMultiWrite.h"
#include "torque.h"
#include "BldcControl.h"

#include "advancedPlus.h"
#include "eventGraphTask.h"
#include "eventRealtimeTask.h"
#include "socketControl.h"
#include "memoryQspiControl.h"
#include "autoCustomizing.h"
#include "virtualPreset.h"
#include "svpwm.h"
#include "modbusMultiWrite.h"
#include "bissSpiCommunicate.h"
#include "totalScrewCount.h"
#include "RTC.h"
#include "barcode.h"
#include "realtimeMonitoring.h"
#include "lastPresetModel.h"
#include "current.h"

/* Extern variables ---------------------------------------------------------*/
// ADC
extern ADC_HandleTypeDef hadc1;
// I2C
extern I2C_HandleTypeDef hi2c1;
// TIM
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;
// UART
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
//IWDG
extern IWDG_HandleTypeDef hiwdg;

extern NetworkTypeDef        WifiEthSet;        // IP, Port, Mac address
extern WifiAPTypeDef         WifiAPSet;         // AP Config: SSID, Password, Security of AP




/* Functions --------------------------------------------------------------*/
void main_init(void)
{
    int i;

    Init_ViriableFirst();

    // EEPROM init
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_SET);
    HAL_Delay(50);

    //parameter read Error 
    for(i=0; i < 3; i++)
    {
        if(ParameterReadFromMemory(-1) == 1)
        {
            //system_err = 0
            break;
        }
    }

    // Apply Firmware version
    ParameterInitial_hh();

    // Check Parameter min max 
    paraMinMaxCheck();

    // Get Default RTC
    getRtc(&rtcCalendar);


    // Read Saved preset number (드라이버 종료 전 preset 번호)
	getLastPresetModel();


	// event 설정 값 초기화
    initGraphMemSetting();
    initRealtimeMemSetting();


    // Flash memory 초기화 (Event memory)
    initQspiMemory();


    // Total screw 초기화
    initTotalScrewCount();



    // encoder start
    HAL_Delay(10);



    // Read driver parameter
    ReadWrite_DriverData();

    // Set driver motor type
    Set_MotorEncoderType();

    // Init parameter min max dft
    Init_Para_limit();

    // Set torque variable value
    Set_Torque_variable();

    // Start UART communication
    HAL_UART_Receive_DMA(&huart1, (uint8_t*) OledRx_data, 1);        // rx data를 받을 준비를 한다.
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_value_buf, 5);            // Read ADC (온도, 배터리...)


    // Read Hallsensor
    BldcMotorInit();
    makeCrc6Table();                // Make CRC Table
    findZeroAngle();                // BISS 통신 연결 확인 및 드라이버 Lock

  MakeTable();

  // timer1 start 100us
  // PWM START
  // HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_1);       
  // HAL_TIMEx_PWMN_Start(&htim1,  TIM_CHANNEL_1);
  // HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_2);
  // HAL_TIMEx_PWMN_Start(&htim1,  TIM_CHANNEL_2);
  // HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_3);
  // HAL_TIMEx_PWMN_Start(&htim1,  TIM_CHANNEL_3);
  Pwm_L_On(TIM_CHANNEL_1);
  Pwm_Off(TIM_CHANNEL_2);
  Pwm_Off(TIM_CHANNEL_3);
  HAL_TIM_Base_Start_IT(&htim1);

    // timer13 start 100us
    HAL_TIM_Base_Start_IT(&htim13);
    HAL_TIM_Base_Start_IT(&htim14);

    // Transducer offset calibration
    Fine_i_offset();

    // Init wifi parameter (display를 위해 무조건 실행)
    initWifiPara();

    // Barcode read off
    HAL_GPIO_WritePin(BAR_START_GPIO_Port, BAR_START_Pin, ON);


    // Sleep mode에서 screw count back up
    uint16_t sleepScrewCount;
    SPI_ReadMemoryData(SLEEP_SCREW_COUNT, (uint8_t*)&sleepScrewCount, 2);

    if(flagSleepCheck == ON && sleepScrewCount != 0)
    {
        // 저장되어 있던 screw 값 반영 및 저장된 screw 초기화
        ScrewCountValue = sleepScrewCount;
        sleepScrewCount = 0;
        SPI_WriteMemoryData(SLEEP_SCREW_COUNT, (uint8_t*)&ScrewCountValue, sizeof(ScrewCountValue));
    }
}


void background(void)
{
	static uint32_t loop10msTime = 0, loop100Ms;

    // TODO: 시간 소요 측정
    static uint32_t timeout;
    if(HAL_GetTick() - timeout > 2)
        printf("T: %lu\n", HAL_GetTick() - timeout);

    timeout = HAL_GetTick();


    

    if (Flag.Reset)
    {
        Flag.Reset = OFF;
        system_err = 0;
        current_mode = _OPERATION;

        StatusOutput = rALARM_RESET;
        ClearSendData_Init(OFF);
        putRealtimeData(&realPollStruct);
    }

    if (Flag.FirmwareUpdate)
    {
        HAL_IWDG_Refresh(&hiwdg);   // Clear IWDG Count
        Update_flash();             // Clear Flash firmware check bit
        HAL_IWDG_Refresh(&hiwdg);
    	HAL_DeInit();
        HAL_Delay(1000);
        NVIC_SystemReset();         // reset system 
    }



    /* Toatal screw ----------------------------------------------------------------------*/
    
    // 체결 횟수 저장.
    totalScrewProcess(&system_err, TOTAL_SCREW_OVER, Flag.Run);


    


    /* Communication ----------------------------------------------------------------------*/
    // com data read
    ReceiveComData();

    // com receive data processing
    if (Flag.ReceiveModBusData)     // receive data process
    {
        Flag.ReceiveModBusData = OFF;
        Modbus_RTU_Process(1);      // RS232
    }
    if (FlagFasten.etReceiveModBusData)     // receive data process
    {
        FlagFasten.etReceiveModBusData = OFF;
        Modbus_RTU_Process(2);      // Ethernet
    }

    // Wifi process
    if (WifiEthSet.WifiOperate)  
        WifiProcess();
		
	
    // BISS 와 통신 상태 확인
    checkBissEncoderComm();





    /* Realtime -----------------------------------------------------------------------------*/

    // Virtual preset 사용 중 preset 변경 확인
    checkVirtualPresetChange();

    // Realtime monitoring send process
    realtimeMonitoringProcess();

    // Monitoring data send to Rs232
    realtimeEventSendTo232();    // Back up fastening data ALL (Use RS232)

    // Realtime event data 중 개수 선택하여 Backup
    realtimeEventSendSeleNumTo232();

    // Send saved fastening data (Event socket)
    if(WifiAPSet.Status)
    {
        realtimeEventSendToWifi();      // WiFi only
    }




    /* Graph ---------------------------------------------------------------------------------*/

    // Graph data process
    graphMonitoringProcess();

    // Save, Send graph data in memeory
    GraphMemProcess();




    /* Parameter ------------------------------------------------------------------------------*/

    // 파라미터 저장
    if (Flag.SaveParameter == ON)
    {
        // 저장할 주소 및 값 가져오기
        parameterBackUpStruct tempBackup;
        parameterBackup.setAddress = parameterBackup.list[0].address;
        parameterBackup.setData = parameterBackup.list[0].data;

        // List 재정렬
        memcpy(&tempBackup.list[1], &parameterBackup.list[1], sizeof(parameterBackup.list[0]) * (PARAMETER_BACKUP_LIST_SIZE - 1));
        memset(&parameterBackup.list, 0, sizeof(parameterBackup.list));
        memcpy(&parameterBackup.list[0], &tempBackup.list[1], sizeof(parameterBackup.list[0]) * (PARAMETER_BACKUP_LIST_SIZE - 1));

        if(parameterBackup.setAddress == 0)
        {
            // 파라미터 저장 및 설정
            Flag.SaveParameter = OFF;
        }
        else
        {
            // Save 파라미터
            Save_Converting_Torque_Speed();

            // Para 변경으로 Setting 재설정
            ParaChangeResetting();
			Init_Para_limit();
        }
    }


    // Parameter multi save
    paraMultiSaveSequential();
    

    // Save Auto customizing value
    if (AutoSetting.FlagSavePara)
    {
        AutoSetting.FlagSavePara = OFF;
        SaveAutoSettingParaValue();
    }

    // all init or one parameter save..
    if (Flag.DriverParaInit || Flag.DriverSaveParaData)		// driver parameter init
        ReadWrite_DriverData();

	// 파라메타 초기화
    if (Flag.ParaInitialize)
    {
        Init_Para_limit();              // torque/speed min/max value change
        ParameterWriteToMemory(-1, 0);  // init default value
        Flag.ParaInitialize = OFF;

        // System reboot
    	HAL_DeInit();
        HAL_Delay(1000);
        NVIC_SystemReset();     // reset system    
    }


    // Set, Read controller parameter
    controllerParaProcess();




    /* Barcode -----------------------------------------------------------------------------*/

    // Barcode process
    barcodeProcess();



    /* LCD ---------------------------------------------------------------------------------*/

    if (Flag_sec_delay_after_power_on)
    {
        key_process();      // OLED Key button
        if (Flag_SendOledData)
        {
            Flag_SendOledData = OFF;    // 100ms에 한번 실행.
            display_process();
        }
    }

	// Clear IWDG (Reset over 512ms)
	if(hiwdg.Instance == IWDG)
	 	HAL_IWDG_Refresh(&hiwdg);



    /* Power --------------------------------------------------------------------------------*/

    // Power saving mode
    Power_down();    




    // Loop 10ms (Systick 시간이 부족할 경우 발생 - main loop로 이동)
    if(HAL_GetTick() - loop10msTime >= 10 && HAL_GetTick() > 10)
    {
        // Read 온도 ADC (주기적 읽는 모드로 변경)
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_value_buf, 5);

        // Battery voltage check
        if (!Flag.Run)
            battery_voltage_check();      // if motor run, don't check voltage.

        // 드라이버 회전 중 배터리 전압 확인
        battery_voltage_check_with_run();

        // Motor temperature check
        temperature_check_motor();   
        // Battery temperature check
        temperature_check_battery();
        // PCB temperature check
        temperature_check_pcb();


        // Lock command
        Lock_Command_process();

        // Preset number 확인
        checkChangePreset();

        // Torque 읽는 상태 정상인지 확인
        torqueReadState();


        // Wifi systick loop (count increase)
        WifiSysLoop();    

        // Lamp LED Control
        Light_Control();

        // Sleep mode check
        sleepTimeCheck();


        // LCD 메시지 전송 Flag 설정
        setOledMessageOn();

        // UART의 상태를 확인
        checkUartState();

        // Check Flash memory 에러
        returnMemoryErrorState(&system_err, MONITORING_DATA_READ_WRITE);

        // Monitoring 종료 timeout
        monitoringTimeout();

        // Auto customizing 설정 저장
        SaveAutoSettingParaValue();

        // Bit socket tray 접속에 따른 Lock 제어
        bitsocketLockControl();

        // Check driver biss complete
        findZeroAngle();

        // 바코드 스캐너 동작
        barcodeProcess();


        // Set time
        loop10msTime = HAL_GetTick();
    }


    // Loop 주기 100ms
    if(HAL_GetTick() - loop100Ms > 100 && HAL_GetTick() > 100)
    {
        loop100Ms = HAL_GetTick();


        // Get RTC
        getRtc(&rtcCalendar);

        // Error code 저장 및 읽기 실행
        errorCodeProcess();
    }
}


// Flash에서 Firmware update check하는 것을 지운다.
void Update_flash(void)
{
    uint8_t  flash_values[20] = {0, };

    HAL_FLASH_Unlock();
    FLASH_Erase_Sector(3, VOLTAGE_RANGE_3);
    HAL_FLASH_Lock();    

    flashWrite(CHK_STATUS_ADDR, (uint32_t*)flash_values, 4);
}


// Flash 해당 주소에 값을 쓴다.
uint8_t flashWrite(uint32_t Addr, uint32_t* Buf, uint32_t count)
{
    // Unlock
    uint8_t state = FALSE;
    HAL_FLASH_Unlock();

    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Addr, Buf[0]) == HAL_OK)
        state = TRUE;

    HAL_FLASH_Lock();

    return state;
}



// Sleep Mode
void Power_down(void)
{
    static uint32_t UnderVoltageTime = 0;

    if (Flag_power_down)
    {
        // Check AP Connection for sleep Multicast
        if(WifiAPSet.Status == ON)
        {
            // Send sleep mode message to contorller
            setAdvertiseMsg(MULTICAST_ADVERTISE, ENTER_SLEEP_MODE);
            MulticastProcess();

            HAL_Delay(100);
        }
        
        HAL_GPIO_WritePin(POWER_ENABLE_GPIO_Port, POWER_ENABLE_Pin, GPIO_PIN_RESET);    // sleep
        HAL_Delay(500);

        // Sleep mode
        HAL_IWDG_Refresh(&hiwdg);
        __HAL_PWR_CLEAR_WAKEUP_FLAG(PWR_WAKEUP_PIN_FLAG1);
        HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);
        HAL_PWR_EnterSTANDBYMode();         // 대기상태 중 8%의 전력소모 있음
    }

    // Under voltage power sleep
    if(UnderVoltageTime != 0)
    {
        // 에러 발생 이후 6초 후 시스템 종료
        if(HAL_GetTick() - UnderVoltageTime > 6000)
            Flag_power_down = ON;
    }
    else if(system_err == UNDER_VOLTAGE)
        UnderVoltageTime = HAL_GetTick();
}

void Fine_i_offset(void)
{
	long i;
	long AdValue;


    // sensor의 offset값을 기본으로 가지고 있으나, 이곳에서 한번더 확인을 해서 offset을 반영하고,
    // 일정값이상으로 틀어진 경우에는 에러를 발생한다.
	AdValue = 0;
	for(i=0; i < 32; i++)
	{
		HAL_Delay(5);
		//store results
		AdValue += Tq_SensorValue;
	}

	AdValue = AdValue >> 5; // 나누기 32
    SensorAD_OffsetValue = AdValue - AD_OFFSET_VALUE;

    // Sensor 값의 틀어짐 비교
    if (labs(SensorAD_OffsetValue) > 1500)      // about 5%
    {
		if(system_err == 0)
			system_err = CHECK_TRANSDUCER;

        SensorAD_OffsetValue = 0;	    // error 발생시에는    
        SensorOffset = 0;               
    }
    else
    {
        SensorAD_OffsetValue = 0;       // sensor offset은 이상이 없을 것으로 판단하고, 0으로 설정.
    }   
}

void ReceiveComData(void)
{
    uint8_t  err = FALSE;
    uint8_t  data;
    uint16_t cnt;
	int retval;

    long     Chksum_a;
    uint8_t  crc_data[2];
    
    // external comm port process
    cnt = bcRecvBuf.head - bcRecvBuf.tail;
    for (int i=0; i < cnt; i++)
    {
        data = rb_get(&bcRecvBuf, &err);

        if(err == TRUE)     // error
            break;

        ComReadBufferMODBUS[Cnt_ComRead_Buffer_a++] = data;
        Cnt_OverTimeCheckModBus = 0;    // for time check. check the timer0 rootin

        // usb port로 driver ID는 1로 고정.
        if (((ComReadBufferMODBUS[1] == 0x03) || (ComReadBufferMODBUS[1] == 0x04) || (ComReadBufferMODBUS[1] == 0x06)) && (Cnt_ComRead_Buffer_a >= 8))  //command
        {
            Chksum_a = mbGetCRC(ComReadBufferMODBUS, Cnt_ComRead_Buffer_a);
            crc_data[1] = (uint8_t)((Chksum_a >> 8) & 0xFF);
            crc_data[0] = (uint8_t)(Chksum_a & 0xFF);
            if ((crc_data[0] == ComReadBufferMODBUS[Cnt_ComRead_Buffer_a - 2]) && (crc_data[1]==ComReadBufferMODBUS[Cnt_ComRead_Buffer_a - 1]))
            {
                Cnt_ComRead_Buffer_a = 0;
                Flag.ReceiveModBusData = ON;
                break;
            }
            else        // check sum error
            {
                Cnt_ComRead_Buffer_a = 0;
                mbSendErrorCode(0x07, 1);       // crc error
                break;
            }
        }
        else if ((ComReadBufferMODBUS[1] == 0x10) && (Cnt_ComRead_Buffer_a >= 6))
        {
            retval = checkFunctionCode16RTU(ComReadBufferMODBUS, &Cnt_ComRead_Buffer_a);
            if(retval == ON)
                break;
        }
        else if ((ComReadBufferMODBUS[1] == 0x11) && (Cnt_ComRead_Buffer_a >= 4))
        {
            Cnt_ComRead_Buffer_a = 0;
            Flag.ReceiveModBusData = ON;
            break;
        }
        else if ((Cnt_ComRead_Buffer_a > 1) && ((ComReadBufferMODBUS[1] != 0x03) && (ComReadBufferMODBUS[1] != 0x04) && (ComReadBufferMODBUS[1] != 0x06) && (ComReadBufferMODBUS[1] != 0x11 && (ComReadBufferMODBUS[1] != 0x10))))  // error
        {
            Cnt_ComRead_Buffer_a = 0;
            mbSendErrorCode(0x01, 1);       // function error
            break;
        }
    }

    // OLED comm process
    cnt = OledRecvBuf.head - OledRecvBuf.tail;
    for (int i=0; i < cnt; i++)
    {
        data = rb_get(&OledRecvBuf, &err);

        if(err == TRUE)     // error
            break;
        
        if ((KeyOldData != 0) && (data == 0))       // off
        {
            /*if (KeyOldData == 0x01)
                ReceiveKeyData[0] = 0x0;
            else if (KeyOldData == 0x02)
                ReceiveKeyData[1] = 0x0;
            else if (KeyOldData == 0x04)
                ReceiveKeyData[2] = 0x0;
            else if (KeyOldData == 0x08)
                ReceiveKeyData[3] = 0x0;*/
        }
        else        // on
        {
            if (data == 0x01)
                ReceiveKeyData[0] = 0x01;
            else if (data == 0x02)
                ReceiveKeyData[1] = 0x01;
            else if (data == 0x04)
                ReceiveKeyData[2] = 0x01;
            else if (data == 0x08)
                ReceiveKeyData[3] = 0x01;
        }

        KeyOldData = data;
        break;
    }
}


/**
 * @brief WIFI 모듈에서 받은 Data를 통신 프로토콜에 맞게 처리한다.
 */
void WiFi_DataReceive(void)
{
    static uint32_t TimeoutPacket;
    uint8_t err = FALSE;
    uint8_t data;
    uint16_t cnt;
    int retval;

    int i;

    // Timeout 70ms
    if(etCntReadBuffer > 0)
    {
        if(TimeoutPacket == 0)
            TimeoutPacket = HAL_GetTick();

        if(HAL_GetTick() - TimeoutPacket > 70)
        {
            etCntReadBuffer = 0;
            TimeoutPacket = 0;
        }    
    }
    else
        TimeoutPacket = 0;


    // Count receive data length
    cnt = WifiRecvBuf.head - WifiRecvBuf.tail;

    if(cnt > 0)
    {
        for (i=0; i < cnt; i++)
        {
            data = rb_get(&WifiRecvBuf, &err);

            if(err == TRUE)
                break;

            etReadBuffer[etCntReadBuffer++] = data;
            TimeoutPacket = 0;   // for time check. check the timer0 rootin

            // Check modbus Function code
            if (((etReadBuffer[7] == 0x03) || (etReadBuffer[7] == 0x04) || (etReadBuffer[7] == 0x06)) && (etCntReadBuffer >= 12))
            {
                etCntReadBuffer = 0;
                FlagFasten.etReceiveModBusData = ON;
                break;
            }
            else if ((etReadBuffer[7] == 0x10) && (etCntReadBuffer >= 12))
            {
                retval = checkFunctionCode16TCP(etReadBuffer, (uint16_t*)&etCntReadBuffer);
                if(retval == ON)
                    break;
            }
            else if ((etReadBuffer[7] == 0x11) && (etCntReadBuffer > 7))
            {
                etCntReadBuffer = 0;
                FlagFasten.etReceiveModBusData = ON;
                break;
            }
            else if ((etCntReadBuffer > 7) && ((etReadBuffer[7] != 0x03) && (etReadBuffer[7] != 0x04) && (etReadBuffer[7] != 0x06) && (etReadBuffer[7] != 0x11) && (etReadBuffer[7] != 0x10)))   // error
            {
                etCntReadBuffer = 0;
                mbSendErrorCode(0x01, 2);       // function error
                break;
            }
        }
    }
}

int Converting_Torque_Speed(int setno, float paravalue, int flag)     // flag:0:para no, value.  flag:1: x, value
{
    int tSpeed=0;
    float tTorque=0.0;

    if (flag == 0)
    {
        // Save parameter
        setParaVal(setno, paravalue);

        // set값은 이미 0.01 단위이다. 실제 torque 값으로 만든다 (N.m) .. torque의 방향은 적용 안함.
        tTorque = getParaVal(setno) * TorqueMetric[Para[UNIT].val][UNIT_Nm];
    }
    else
    {
        // change unit to N.m
        tTorque = paravalue * TorqueMetric[Para[UNIT].val][UNIT_Nm];
    }

    switch(Para[DRIVER_MODEL].val)     // driver
    {
        // case BMT4001:    //
        //     tSpeed = (int)(((tTorque * 10.2) + 0.9238) / 0.004 * 0.80);      // 80%
        // break;
        // case BMT4002:    //
        //     tSpeed = (int)(((tTorque * 10.2) + 1.0914) / 0.0075 * 0.80);     // 80%
        // break;
        case BMT4004:    // o
            tSpeed = (int)(((tTorque * 10.2) + 4.9593) / 0.0359 * 0.80);        // 80%
        break;
        case BMT4006: // o
            tSpeed = (int)(((tTorque * 10.2) + 5.65) / 0.0558 * 0.80);          // 80%      // FIXME: 최저 토크일 때 Hard joint에서 오버토크가 발생된다. (Auto speed 전부 수정 필요)
        break;
		case BMT4011:	//
			tSpeed = (int)(((tTorque * 10.2) + 0.8333) / 0.2017 * 0.85);        // 85%
		break;   
        case BMT4016:    // 0.4422x - 37.089
            tSpeed = (int)(((tTorque * 10.2) + 37.089) / 0.4422 * 0.90);        // 90%
        break;
        case BMT4020:    // 0.6386x - 39.661
            tSpeed = (int)(((tTorque * 10.2) + 39.661) / 0.6386 * 0.85);        // 90%
        break;
        case BMT4024:    // o
            tSpeed = (int)(((tTorque * 10.2) + 108.33) / 0.8333 * 0.87);        // 87%
        break;
//        case BMT4016:    // o y = 1.28x - 55.667
//            tSpeed = (int)(((tTorque * 10.2) + 55.667) / 1.28 * 0.97);          // 97%
//        break;
        case BMT4045:    // 0
            tSpeed = (int)(((tTorque * 10.2) + 75) / 2.5 * 1.0);                // 100%
        break;
        case BMT4064:    // o y = 6x - 390
            tSpeed = (int)(((tTorque * 10.2) + 390) / 6.0 * 1.00);              // 100%
        break;
        default: 
            tSpeed = (int)(((tTorque * 10.2) + 0.0) / 0.0008 * 0.95);           // 95% 
        break;
    }
    
    // Max RPM 확인 (24V 일 경우 기준)
    if (tSpeed > Info_DrvModel[Para[DRIVER_MODEL].val].speed_max / getCrowFootRatio())  
        tSpeed = Info_DrvModel[Para[DRIVER_MODEL].val].speed_max / getCrowFootRatio();

    // Min RPM 확인
    if (tSpeed < Info_DrvModel[Para[DRIVER_MODEL].val].speed_min / getCrowFootRatio())
        tSpeed = Info_DrvModel[Para[DRIVER_MODEL].val].speed_min / getCrowFootRatio();

    return tSpeed;  
}



void SerialPutChar_Oled(uint16_t len, uint8_t* data)    // for OLED
{
    if (HAL_UART_GetState(&huart1) != HAL_UART_STATE_BUSY_TX)
    {
        HAL_UART_Transmit_DMA(&huart1, data, len);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)     // OLED commnication
    {
        rb_put(&OledRecvBuf, OledRx_data[0]);
    }
    else if (huart->Instance == USART3)
    {
        barcodeRxCallback();
    }
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) 
{
    if(huart->Instance == USART1) 
    {
        if (huart1.ErrorCode != 0)
        {
            huart1.ErrorCode = 0;
            HAL_UART_Receive_DMA(&huart1, (uint8_t*) OledRx_data, 1);
        }
    }
    else if (huart->Instance == USART3)
    {
        // Over run Error
        if(huart3.ErrorCode != 0)
        {
            huart3.ErrorCode = 0;
            barcodeRxStart();
        }
    }
}

void Set_MotorEncoderType(void)
{
    uint8_t lastPresetNo = 0;   // 저장된 Preset number

    // Load last preset number (Preset 저장기능 활성화되어 있을 때 - LAST_PRESET_NUM)
    if(Para[FASTEN_NO_SAVE].val == 0 && lastPreset.preset <= PRESET_INDEX_SIZE + 2 && lastPreset.preset >= 1)
        lastPresetNo = lastPreset.preset;

    // Booting 중 preset 결정
    if(lastPresetNo != 0)
        Fastening_number = Processing_number = lastPreset.preset;               // 마지막 저장된 Preset
    else if(Para[FASTEN_NO_SAVE].val == 0)
        Fastening_number = Processing_number = 1;                               // 저장된 preset 없을 때
    else
        Fastening_number = Processing_number = Para[FASTEN_NO_SAVE].val;        // 설정된 Preset

    
    // Multi sequence check
    if(Fastening_number >= PRESET_INDEX_SIZE + 1)
        Fastening_number = 1;

    switch (Info_DrvModel[Para[DRIVER_MODEL].val].motor)
    {
        case MT40PI_24V:            // 18V 1024 maxon 40
                Motor.Ra                    = 0.236;              // maxson motor 100W:  18V
                Motor.La                    = 0.000169; 
                Motor.Kt                    = 0.0375 / 1.414;             // = 37.5 * 0.001;  단위 : Nm/A
                Motor.inv_Kt                = 1./Motor.Kt;//26.6666666666666;  // 1.0/Kt; 
                Motor.Jm                    = 44.0 * 0.001 * 0.01 * 0.01;
                Motor.I_rated               = 5.46;// * 1.414;
                Motor.Te_rated              = 0.207;
                Motor.Nominal_voltage       = 24;
                Motor.max_motor_speed       = 5700.0;
                Motor.OverCurrentLimit      = Motor.I_rated * 11.17;     // Stall torque: 2.86N.m, kt: 0.375 -> 2.86/0.375 = 76.26A (최대 토크) -> 76.26*0.8 = 61A
                Motor.torque_limit          = Motor.OverCurrentLimit * Motor.Kt;     //Te_rated * 3.0;
                Motor.num_pulse             = 4096;
                Motor.wn_cc                 = 50;//0;//500;//5000;     // 응답 주파수
                Motor.gradient              = 4240;     // rpm/Nm : 기울기
                Motor.wn_sc                 = 90;//70;
                Motor.Poles                 = 7*2;
                Motor.mech_t                = 0.739e-3;
                Motor.Ratio                 = 90;//70;
                Motor_mech_t_back = Motor.mech_t;
        break;
        default:
                Motor.Ra                    = 0.236;              // maxson motor 100W:  18V
                Motor.La                    = 0.000169; 
                Motor.Kt                    = 0.0375 / 1.414;             // = 37.5 * 0.001;  단위 : Nm/A
                Motor.inv_Kt                = 26.6666666666666;  // 1.0/Kt; 
                Motor.Jm                    = 44.0 * 0.001 * 0.01 * 0.01;
                Motor.I_rated               = 5.46;
                Motor.Te_rated              = 0.207;
                Motor.Nominal_voltage       = 24;
                Motor.max_motor_speed       = 5700.0;
                Motor.OverCurrentLimit      = Motor.I_rated * 11.17;     // Stall torque: 2.86N.m, kt: 0.375 -> 2.86/0.375 = 76.26A (최대 토크) -> 76.26*0.8 = 61A
                Motor.torque_limit          = Motor.OverCurrentLimit * Motor.Kt;     //Te_rated * 3.0;
                Motor.num_pulse             = 4096;
                Motor.wn_cc                 = 5000;     // 응답 주파수
                Motor.gradient              = 4240;     // rpm/Nm : 기울기
                Motor.wn_sc                 = 70;
                Motor.Poles                 = 7*2;
                Motor.mech_t                = 0.739e-3;
                Motor.Ratio                 = 70;
        break;
    }

    // 기어비 설정을 상위에서 했으나, 내부에서 모델별로 가지고 가는 것으로 수정함..
    deceleration_ratio  = Info_DrvModel[Para[DRIVER_MODEL].val].gear_ratio * getCrowFootRatio();
    inv_deceleration_ratio  = 1.0 / deceleration_ratio;
	
    vel_time = (float)(TWO_PI) / ((float)Motor.num_pulse * (float)(TS_VEL));

    if (Info_DrvModel[Para[DRIVER_MODEL].val].con_model == BMT40)
		Gain.hss_gain =	(0.32 / 32767.) * 200.;   // 0.32mV / 분해능.. 200 = 1A / 0.005mV (5mohm)

    // max pwm으로 가동시 하단 충전 시간이 필요함.
    PWM_Maxvalue = PWM_MAX;    // 10000

    // Set PID gain
    Gain_Setting();

    adc_conv_value = 3.3 / 4096;    // use adc conversion..

    AD_OFFSET_VALUE = 32767;         // if 1.6V

}

void Gain_Setting(void)
{
    Gain.i_kp = 8.0;
    Gain.i_ki = 0.1;
    Gain.w_kp = 0.5;
    Gain.w_ki = 0.005;

    switch(Para[DRIVER_MODEL].val)     // driver
    {
        case BMT4004:
            if ((fastening_step == RAMPUP_TARGET_TORQUE) || (fastening_step == THREADTAP))
            {
                Gain.i_kp = 8.5;   Gain.i_ki = 0.35; Gain.w_kp = 0.5;   Gain.w_ki = 0.005;
            }
            else
            {
                Gain.i_kp = 8.0;   Gain.i_ki = 0.1;  Gain.w_kp = 0.5;   Gain.w_ki = 0.005;    
            }
        break;
        case BMT4006:
            // Gain.i_kp = 20.5;   Gain.i_ki = 2.00;  Gain.w_kp = 0.01;   Gain.w_ki = 0.00001;      // Ramp up 때
            // Gain.i_kp = 25.5;   Gain.i_ki = 2.83;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;       // Ramp up 이외
            // Gain.i_kp = 25.5;   Gain.i_ki = 1.73;  Gain.w_kp = 0.03;   Gain.w_ki = 0.0001;
            // Gain.i_kp = 25.5;   Gain.i_ki = 1.73;  Gain.w_kp = 0.03;   Gain.w_ki = 0.0001;
            if ((fastening_step == RAMPUP_TARGET_TORQUE) || (fastening_step == THREADTAP))
            {
                Gain.i_kp = 8.5;   Gain.i_ki = 0.35; Gain.w_kp = 0.5;   Gain.w_ki = 0.005;
            }
            else
            {
                Gain.i_kp = 8.0;   Gain.i_ki = 0.1;  Gain.w_kp = 0.5;   Gain.w_ki = 0.005;    
            }     
        break;
		case BMT4011:
            if (((fastening_step == RAMPUP_TARGET_TORQUE) || (fastening_step == THREADTAP)) \
                && getParaVal(FASTEN_TORQUE1_H + ((Fastening_number-1)*PRESET_SIZE)) < getParaMax(FASTEN_TORQUE1_H) * 0.22)
            {
                Gain.i_kp = 20.5;   Gain.i_ki = 02.00;  Gain.w_kp = 0.005;   Gain.w_ki = 0.00001;
            }
            else
            {
                Gain.i_kp = 25.5;   Gain.i_ki = 02.83;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
            }
		break;   
        case BMT4016:
        case BMT4020:
            // if (((fastening_step == RAMPUP_TARGET_TORQUE) || (fastening_step == THREADTAP)) 
            //     && getParaVal(FASTEN_TORQUE1_H + ((Fastening_number-1)*PRESET_SIZE)) < getParaMax(FASTEN_TORQUE1_H) * 0.22)
            // {
            //     Gain.i_kp = 6.5;   Gain.i_ki = 0.35;  Gain.w_kp = 0.4;   Gain.w_ki = 0.004;
            // }
            // else
            // {
            //     Gain.i_kp = 8.0;   Gain.i_ki = 0.1;  Gain.w_kp = 0.5;   Gain.w_ki = 0.005;
            // }
            
            Gain.w_kp = Motor.Jm * Motor.Ratio * Motor.wn_sc;
            Gain.w_Ka = 1. / Gain.w_kp;
            Gain.w_ki = Motor.Jm / Motor.mech_t * Motor.wn_sc * TS_VEL;

            Gain.i_kp = Motor.wn_cc * 1. * Motor.La;
            Gain.i_Ka = 1. / Gain.i_kp;
            Gain.i_ki = Motor.wn_cc * Motor.Ra * 1e-4;//TS_CUR;
            
        break;
        case BMT4024:
            // if ((fastening_step == RAMPUP_TARGET_TORQUE) || (fastening_step == THREADTAP))
            // {
            //     // Gain.i_kp = 16.0;   Gain.i_ki = 01.60;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
            //     Gain.i_kp = 8.5;   Gain.i_ki = 0.35; Gain.w_kp = 0.5;   Gain.w_ki = 0.005;
            // }
            // else
            // {
            //     // Gain.i_kp = 28.5;   Gain.i_ki = 02.83;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
            //     Gain.i_kp = 8.0;   Gain.i_ki = 0.1;  Gain.w_kp = 0.5;   Gain.w_ki = 0.005;    
            // }
            
            Gain.w_kp = Motor.Jm * Motor.Ratio * Motor.wn_sc;
            Gain.w_Ka = 1. / Gain.w_kp;
            Gain.w_ki = Motor.Jm / Motor.mech_t * Motor.wn_sc * TS_VEL;

            Gain.i_kp = Motor.wn_cc * 1. * Motor.La;
            Gain.i_Ka = 1. / Gain.i_kp;
            Gain.i_ki = Motor.wn_cc * Motor.Ra * 1e-4;//TS_CUR;
        break;
//        case BMT4016:
        case BMT4045:
            if ((fastening_step == RAMPUP_TARGET_TORQUE) || (fastening_step == THREADTAP))
            {
                Gain.i_kp = 15.5;   Gain.i_ki = 01.53;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
            }
            else
            {
                Gain.i_kp = 28.5;   Gain.i_ki = 02.83;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
            }
        break;
        case BMT4064:
            if ((fastening_step == RAMPUP_TARGET_TORQUE) || (fastening_step == THREADTAP))
            {
                Gain.i_kp = 15.5;   Gain.i_ki = 01.53;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
            }
            else
            {
                // Gain.i_kp = 25.5;   Gain.i_ki = 02.83;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
                // Gain.i_kp = 28.5;   Gain.i_ki = 02.83;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
                Gain.i_kp = 20.5;   Gain.i_ki = 2.0;  Gain.w_kp = 0.01;   Gain.w_ki = 0.0001;
            }
        break;
        default: 
        break;
    }

    // if ((fastening_step == RAMPUP_TARGET_TORQUE) || (fastening_step == THREADTAP))
    // {
    //     Gain.i_kp = Para[5+15].val / 10.0;   Gain.i_ki = Para[6+15].val / 1000.0;  Gain.w_kp = Para[9+15].val / 100.0;   Gain.w_ki = Para[10+15].val / 1000.0; 
    // }

    if(Gain.w_kp == 0.0)
        Gain.w_kp = 0.01;

    Gain.w_kp = Motor.Jm * Motor.Ratio * Motor.wn_sc;
    Gain.w_Ka = 1. / Gain.w_kp;
    Gain.w_ki = Motor.Jm / Motor.mech_t * Motor.wn_sc * TS_VEL;

    Gain.i_kp = Motor.wn_cc * 1. * Motor.La;
    Gain.i_Ka = 1. / Gain.i_kp;
    Gain.i_ki = Motor.wn_cc * Motor.Ra * 1e-4;//TS_CUR;
}

void Set_Torque_variable(void)
{
    float maintain_period;
    float ctorque;
    float FasteningTargetTorque_Nm=0.0, FasteningTargetTorque_org=0.0;
    float SnugTorqueTmp_Nm=0.0;
    float SumAcc;
    float DefaultAcc;
    int temp;
    int preset;
    float speedAccTimeInv;
    float paraTargetTQ;
    uint32_t paraU32Val;


    /***
     * @note BMT40 - 트랜듀서 타입의 토크 값 설정 방법
     *      1. velocity에서 계산하는 토크 값의 단위: N.m
     *      2. velocity에서 토크 비교할 때 N.m로만 변환하여 비교 
     *      3. velocity에서 사용한 토크 값을 외부로 보여줄 때는 토크 보상값만 적용하여여 표시
     * 
     *      4. w_pi_out_screw_type 는 기어비가 보상되기 전 토크 값 (실제 토크 값은 w_pi_out_screw_type * deceleration_ratio 하면 나온다)
    */
	
	
    // 기어비 계산
    deceleration_ratio = Info_DrvModel[Para[DRIVER_MODEL].val].gear_ratio * getCrowFootRatio();
    inv_deceleration_ratio  = 1.0 / deceleration_ratio;


    // Preset - Multi mode 선택
    if (Processing_number < PRESET_INDEX_SIZE + 1)     
    {
        // Preset mode
        Fastening_number = Processing_number;
        Flag.Multi_Mode = OFF;
    }
    else     
    {
        // Multi mode torque 
        Flag.Multi_Mode = ON;

        if (Processing_number == PRESET_INDEX_SIZE + 1)        // Multi A mode
            ms.PGNumber = 1;
        else if (Processing_number == PRESET_INDEX_SIZE + 2)   // Multi B mode
            ms.PGNumber = 2;
    }

    // Check Multi sequence - Preset 
    if (Flag.Multi_Mode)
    {
        // 초기 Multi 부팅시 무조건 Preset 1의 CW를 체크
        if (Fastening_number > PRESET_INDEX_SIZE)      
            preset = 1;                     // 초기 Multi 부팅시 무조건 Preset 1의 CW를 체크
        else
            preset = Fastening_number;
    }
    else
    {
        // Preset 모드
        preset = Fastening_number;
    }
        

    // Get Preset 별 screw type
    paraU32Val = (getParaValSingle(SCREW_TYPE_H) << 16) + getParaValSingle(SCREW_TYPE_L);
    temp = (paraU32Val >> (preset - 1)) & 0x01;

    // Screw type 설정
    if (Info_DrvModel[Para[DRIVER_MODEL].val].sc_type == 0)
    {
        // 0 = right, CW
        if      ( temp == RIGHT_SCREW ) screw_type =  1;
        else if ( temp == LEFT_SCREW  ) screw_type = -1;
    }
    else    
    {
        // 1 = left
        if      ( temp == RIGHT_SCREW ) screw_type = -1;
        else if ( temp == LEFT_SCREW  ) screw_type =  1;
    }

    // 속도의 가감속 시간 상수 계산 (1ms 당 가감속 시간 배율 -> Velocity 주기에서 1ms를 만들기 위해 곱해야하는 값)
    speedAccTimeInv = (1.0 / TS_VEL) / 1000.0;

    // TCAM, ACTM Check
    FastenSeq.TcamActm = Para[TCAM_ACTM1 + ((Fastening_number-1)*PRESET_SIZE)].val;

    // Target 토크 설정 (ACTM, Max torque: 0 일때 목표 최대 토크 사용)
    if(FastenSeq.TcamActm && getParaVal(FASTEN_TORQUE1_H + ((Fastening_number-1)*PRESET_SIZE)) == 0)
        paraTargetTQ = getParaMax(FASTEN_TORQUE1_H + ((Fastening_number-1)*PRESET_SIZE));       // Max torque
    else
        paraTargetTQ = getParaVal(FASTEN_TORQUE1_H + ((Fastening_number-1)*PRESET_SIZE));       // Target torque


    // 토크값 계산
    // set값은 이미 0.1/0.01 단위이다. 실제 torque 값으로 만든다 (change N.m) .. torque의 방향은 적용 안함. torque를 바로 읽으므로 감속비는 적용하지 않는다.
    FasteningTargetTorque_Nm = paraTargetTQ * TorqueMetric[Para[UNIT].val][UNIT_Nm] / getCrowFootRatio();
    SnugTorqueTmp_Nm = getParaVal(SNUG_TORQUE1_H + ((Fastening_number-1)*PRESET_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / getCrowFootRatio();
    FastenSeq.MaxTorque_Nm = getParaMax(FASTEN_TORQUE1_H) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / getCrowFootRatio();


    // 가감속 값
    DefaultAcc = Info_DrvModel[Para[DRIVER_MODEL].val].acc_dcc;                                         // Default 가감속
    SumAcc = Para[ACC_DEA_TIME].val + Para[SOFT_START1 + ((Fastening_number-1)*PRESET_SIZE)].val;       // 전체 가감속 값


    // 효율 보상값 계산 (효율 보상은 내부 계산시에는 계속 보상 값이 적용, 외부 출력 시에만 보상 값을 제거한다.)
    Torque_ConvertValue = Info_DrvModel[Para[DRIVER_MODEL].val].tq_conver * 100;                    // driver의 보상값을 여기서 100맞춰서 정한다. factory
    Torque_ConvertValue += dset[3];                                                                 // 1 단위 %임. user
    Torque_ConvertValue += Para[TORQUE_COMPENSATION_MAIN].val;                                      // dset[9] 값임. 단위 %임. user main
    Torque_ConvertValue += Para[TORQUE_COMPENSATION1 + ((Fastening_number-1)*PRESET_SIZE)].val;     // parameter preset 개별 단위
    
    if (Para[CROWFOOT_ENABLE].val)
    {
        Torque_ConvertValue += Para[CROWFOOT_EFFICIENCY].val;
        Torque_ConvertValue = Torque_ConvertValue - 500;
    }
    else
    {
        Torque_ConvertValue = Torque_ConvertValue - 400;
    }

    // 최종 토크 효율 보상값 (100 단위 -> 100%)
    Torque_ConvertValue = 100 + Torque_ConvertValue;


    // Torque 효율 보상값 반영
    FasteningTargetTorque_org = FasteningTargetTorque_Nm;
    FasteningTargetTorque_Nm = FasteningTargetTorque_Nm * Torque_ConvertValue * 0.01;
    SnugTorqueTmp_Nm = SnugTorqueTmp_Nm * Torque_ConvertValue * 0.01;
	FastenSeq.MaxTorque_Nm = FastenSeq.MaxTorque_Nm * Torque_ConvertValue * 0.01;


    if (BatteryType == BATTERY_18V)     // 18v 일 경우 max torque 제한.
    {
        if (FasteningTargetTorque_Nm > FastenSeq.MaxTorque_Nm)
            FasteningTargetTorque_Nm = FastenSeq.MaxTorque_Nm;
    }

    // torque 반전을 위한 상수를 계산한다.
    inv_Torque_ConvertValue = FasteningTargetTorque_org / FasteningTargetTorque_Nm;

    // Free reverse rotation
    FreeReverse.Angle = (long)(getParaVal(FREE_REVERSE_ROTATION_ANGLE1_H + ((Fastening_number-1)*ADV_PARA_SIZE)) * (float)Motor.num_pulse * (float)deceleration_ratio);    //turn
    FreeReverse.Speed = - (float)Para[FREE_REVERSE_ROTATION_SPEED1 + ((Fastening_number-1)*ADV_PARA_SIZE)].val * RPM_TO_RAD_SCALE * deceleration_ratio * screw_type;
    FreeReverse.SpeedAcc = fABS(FreeReverse.Speed) / (DefaultAcc * speedAccTimeInv);      // 가감속은 양수로..


    // Free speed control
    FreeSpeed.Speed = (float)Para[FREE_FASTEN_SPEED1 + ((Fastening_number-1)*PRESET_SIZE)].val * deceleration_ratio * RPM_TO_RAD_SCALE * screw_type;
    FreeSpeed.Angle = (long)((float)Para[FREE_FASTEN_ANGLE1 + ((Fastening_number-1)*PRESET_SIZE)].val * (float)Motor.num_pulse * (float)deceleration_ratio / 360);  //degree
    FreeSpeed.SpeedAcc = fABS(FreeSpeed.Speed) / (SumAcc * speedAccTimeInv);
    FreeSpeed.SpeedDcc = fABS(FreeSpeed.Speed) / (DefaultAcc * speedAccTimeInv);

    if(getParaVal(FREE_SPEED_MAX_TORQUE_H) == 0)
        FreeSpeed.Torque_Nm = FasteningTargetTorque_Nm;
    else
        FreeSpeed.Torque_Nm = getParaVal(FREE_SPEED_MAX_TORQUE_H) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / getCrowFootRatio() * Torque_ConvertValue * 0.01;


    // angle after torque up
    AngleAfterTqUp.Speed = (float)Para[REVERS_ANGLE_SETTING_SPEED1 + ((Fastening_number-1)*ADV_PARA_SIZE)].val * RPM_TO_RAD_SCALE * deceleration_ratio * screw_type;
    AngleAfterTqUp.Angle = (long)((float)Para[REVERS_ANGLE_SETTING_ANGLE1 + ((Fastening_number-1)*ADV_PARA_SIZE)].val * (float)Motor.num_pulse * (float)deceleration_ratio / 360);
    AngleAfterTqUp.Direction = Para[REVERS_ANGLE_SETTING_FW_REV1 + ((Fastening_number-1)*ADV_PARA_SIZE)].val;
    AngleAfterTqUp.SpeedAcc = fABS(AngleAfterTqUp.Speed) / (DefaultAcc * speedAccTimeInv);

    // fasteing sequence
    FastenSeq.TargetSpeed  = (float)Para[FASTEN_SPEED1 + ((Fastening_number-1)*PRESET_SIZE)].val * deceleration_ratio * RPM_TO_RAD_SCALE * screw_type;
    AutoSetting.OldSpeed   = (uint16_t)FastenSeq.TargetSpeed;
    AutoSetting.OldSumAcc  = SumAcc;
    FastenSeq.MaxSpeed     = (float)Para[FASTEN_SPEED1].max * deceleration_ratio * RPM_TO_RAD_SCALE * screw_type;
    FastenSeq.RpmOfLoad    = Para[FASTEN_SPEED1 + ((Fastening_number-1)*PRESET_SIZE)].val; // bit speed
    FastenSeq.SeatingValue = Para[FASTEN_SEATTING_POINT_RATE1 + ((Fastening_number-1)*PRESET_SIZE)].val;
    FastenSeq.SeatingTQ_Nm = FasteningTargetTorque_Nm * FastenSeq.SeatingValue * 0.01;
    FastenSeq.SnugTQ_Nm    = SnugTorqueTmp_Nm;

    // Auto customizing
    AutoSetting.OldSpeed   = (uint16_t)FastenSeq.TargetSpeed;
    AutoSetting.OldSumAcc  = SumAcc;

    // FastenSeq.TorqueRisingValue = FasteningTargetTorque_Nm / ((float)Para[FASTEN_TQ_RISING_TIME1 + ((Fastening_number-1)*PRESET_SIZE)].val * 4.0);
    FastenSeq.TorqueRisingValue = (float)Para[FASTEN_TQ_RISING_TIME1 + ((Fastening_number-1)*PRESET_SIZE)].val * speedAccTimeInv;
    maintain_period = (float)Para[FASTEN_TORQUE_MAINTAIN_TIME].val * 0.001001;
    FastenSeq.HoldingTime = (long)(maintain_period / TS_VEL);
    FastenSeq.TargetTorque_Nm = FasteningTargetTorque_Nm;
    FastenSeq.JudgeMinAngle = (long)(getParaVal(JUDGE_FASTEN_MIN_TURNS_H) * (float)Motor.num_pulse * (float)deceleration_ratio);   //turn
    FastenSeq.RampUpSpeed  = (float)Para[RAMP_UP_SPEED1 + ((Fastening_number-1)*PRESET_SIZE)].val * deceleration_ratio * RPM_TO_RAD_SCALE * screw_type;
    // FastenSeq.RampUpSpeedToControl = ramp_up_speed_setting_default() * deceleration_ratio * RPM_TO_RAD_SCALE * screw_type;
    // FastenSeq.SpeedAccForRampUp = fABS(FastenSeq.RampUpSpeedToControl) / (50 * 4);    // 50ms ramp up은 fix
    // FastenSeq.SpeedAccForRampUp = fABS(FastenSeq.RampUpSpeedToControl) / (35 * 4);    // 35ms ramp up은 fix
    FastenSeq.SpeedAccForRampUp = fABS(FastenSeq.RampUpSpeed) / FastenSeq.TorqueRisingValue;

    FastenSeq.TargetAngle = (long)((float)Para[TARGET_ANGLE1 + ((Fastening_number-1)*PRESET_SIZE)].val * (float)Motor.num_pulse * (float)deceleration_ratio / 360);     //degree
    FastenSeq.MinAngle = (long)((float)Para[FASTEN_MIN_TURNS1 + ((Fastening_number-1)*PRESET_SIZE)].val * (float)Motor.num_pulse * (float)deceleration_ratio / 360);    //degree
    FastenSeq.MaxAngle = (long)((float)Para[FASTEN_MAX_TURNS1 + ((Fastening_number-1)*PRESET_SIZE)].val * (float)Motor.num_pulse * (float)deceleration_ratio / 360);    //degree
    FastenSeq.FasteningTimeLimit = (long)(getParaVal(TOTAL_FASTENING_TIME_H) * INV_TS_VEL);
    FastenSeq.SpeedAcc = fABS(FastenSeq.TargetSpeed) / (SumAcc * speedAccTimeInv);
    FastenSeq.SpeedDcc = fABS(FastenSeq.TargetSpeed) / (DefaultAcc * speedAccTimeInv);


    // torque에 맞는 speed를 구한다 - Auto speed
    temp = Converting_Torque_Speed(0, paraTargetTQ, 1);    
    FastenSeq.AutoSpeed = (float)temp * deceleration_ratio * RPM_TO_RAD_SCALE * screw_type;


    // Advanced setting
    setAdvancedParameter(Fastening_number, FasteningTargetTorque_Nm, SumAcc * speedAccTimeInv);

    // Advanced compensation apply
    setAdvancedCompensation(Para[ADVANCED_MODE1 + ((Fastening_number-1)*ADV_PARA_SIZE)].val, Torque_ConvertValue);


    //  - 체결 속도에 대해서, 실제로 감속기를 거쳐서 모터가 회전해야 할 속도 : 단위 rad/sec
    //   screw_type 을 곱한 것은 좌회전 조임 나사의 경우 반대 방향으로 회전하기 위함이다.
    LooseningSeq.Speed = - (float)Para[LOOSENING_SPEED].val * RPM_TO_RAD_SCALE * deceleration_ratio * screw_type;   // loosening_rad_of_motor
    LooseningSeq.Crowfoot_speed = - (float)Para[CROWFOOT_REVERSE_SPEED].val * RPM_TO_RAD_SCALE * deceleration_ratio * screw_type;   // loosening_rad_of_motor
    LooseningSeq.TimeLimit = (long)(getParaVal(TOTAL_LOOSENING_TIME_H) * INV_TS_VEL);  
    LooseningSeq.StallTimeLimit = (long)(getParaVal(INIT_LOOSENING_TIME_LIMIT_H) * INV_TS_VEL);

    LooseningSeq.SpeedAcc = fABS(LooseningSeq.Speed) / (float)(DefaultAcc * speedAccTimeInv);         // loosening
    LooseningSeq.Crowfoot_speedAcc = fABS(LooseningSeq.Crowfoot_speed) / (float)(DefaultAcc * speedAccTimeInv);         // loosening
    LooseningSeq.Crowfoot_torque_Nm = getParaVal(CROWFOOT_REVERSE_TORQUE_H) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / getCrowFootRatio() * (Torque_ConvertValue * 0.01);
    Delta_acc_loosen = Delta_dcc_loosen = LooseningSeq.SpeedAcc;

    if (Delta_acc == 0)     // 초기에 초기화가 되지 않는 현상이 잇음..
        Delta_dcc = Delta_acc = Delta_acc_loosen;


    // 토크 체결시 Min, Max 범위
    if (FastenSeq.TcamActm == 0)
    {
        // torque control
        ctorque = (float)paraTargetTQ * getParaVal(TORQUE_MIN_MAX1_H + ((Fastening_number-1)*PRESET_SIZE)) * 0.01;
        P_ConvertedTorqueMaxLimit = (getParaVal(FASTEN_TORQUE1_H + ((Fastening_number-1)*PRESET_SIZE)) + ctorque) / getCrowFootRatio() * (Torque_ConvertValue * 0.01);      // Max limit (외부 표시를 위해 토크 보상을 제거하기 전 판단을 하기 때문에 토크 보상 적용)
        P_ConvertedTorqueMinLimit = (getParaVal(FASTEN_TORQUE1_H + ((Fastening_number-1)*PRESET_SIZE)) - ctorque) / getCrowFootRatio() * (Torque_ConvertValue * 0.01);
    }
    else
    {
        // angle control
        P_ConvertedTorqueMaxLimit = (paraTargetTQ) / getCrowFootRatio() * (Torque_ConvertValue * 0.01);      // FASTEN_TORQUE1_H
        P_ConvertedTorqueMinLimit = (getParaVal(TORQUE_MIN_MAX1_H + ((Fastening_number-1)*PRESET_SIZE))) / getCrowFootRatio() * (Torque_ConvertValue * 0.01); 
    }

    // 계산식 값
    MathConv.AngleConv_mPulseToDeg = 360.0005 / (float)Motor.num_pulse * (float)inv_deceleration_ratio;
    MathConv.LoadConv = Motor.gradient * inv_deceleration_ratio * 3.0 ;  // Nm unit..
    MathConv.SamplingPulsConv = ((float)Motor.num_pulse * deceleration_ratio) / 360.0;      // 1도당 pulse 수.
    MathConv.ConvSpeed_screwtype = RAD_TO_RPM_SCALE * inv_deceleration_ratio * screw_type;
    MathConv.ConvSpeed = RAD_TO_RPM_SCALE * inv_deceleration_ratio * 1.00005;
}


void Init_ViriableFirst(void)
{
	int i;

	// 500 ~ 513
	DriverDefaultPara();    // Driver parameter default value

	for ( i=0; i < 13; i++)
	{
		dset[i] = dParameterValue[i];
	}

	Cnt_ComRead_Buffer_a = 0;
	system_err = 0;
	current_mode = _OPERATION;

	Fastening_number = Processing_number = 1;
	Flag.Stopping = OFF;
	v_e_real_compensation_constant = 1.0;
	Flag.Run = Flag.InternalRun = Flag.RunByMult = OFF;
	Flag.ExternalRun = Flag.Multi_Start = OFF;

	Flag.ControlFL = FASTENING_CONTROL;
	mbMData[0] = 0;

	Flag.Buzzer = OFF;
	Flag_L_F_control_current = OFF;
	Flag.FasteningCompleteOut = OFF;

	Cnt_FasteningCompleteOut = 0;
	fastening_step = REVOLUTION_START;
	Flag.TorqueUpCompleteOut = OFF;
	Flag.DriverParaInit = OFF;
	Flag.DriverSaveParaData = OFF;
	i_ref_flt = 0.0;

	ScrewCountValue = 0;
	TRIG_REAL.Capture = OFF;
	Fastening_time = 0;
    w_rpm_abs = 0.0;
	Flag_CountEnd = OFF;
	FlagFasten.ScrewCountProcessStatus = 0;
	Flag.OneTimeExecute = ON;
	Flag.Multi_Mode = OFF;
	Cnt_OverTimeCheckModBus = 0;

	m_pulse_diff_abs = m_pulse_diff = 0;
    w_rad = 0.0;
	TRIG.MonitoringRS232 = OFF;
	TRIG.MointoringEthernet = OFF;
	Disp_Loosening_time = 0;
	Flag.JabCompliteIoOut = OFF;
	Torque_ConvertValue = 0.0;
	w_rpm_display = 0.0;
	Flag.FasteningStopAlarm = OFF;
	
	Torque_cnv.fastenComplete = 0.0;
	mPulse.After_Fastening_Start_abs = 0;
	TotalFasteningTime = 0;
	Flag.Lock = OFF;
	// Flag_AutoModeCountComplete = OFF;
	Flag.OneTimeDisplayTFT = OFF;
	Flag.ParaInitialize = OFF;
	//system_warning = 0;
	Model_Number = 1;

    Flag_GetSensorOffset = OFF;
    Flag_power_down = OFF;
    Flag.FirmwareUpdate = OFF;
	
	TRIG_REAL.Capture       = OFF;
	TRIG.Channel[0] = 1;            // Torque Graph monitoring
    TRIG.Channel[1] = 3;            // Speed
    TRIG.SamplingRate = 5;          // Sampling 5ms

    AD_OFFSET_VALUE = 32767;         // if 1.6V
}

void ReadWrite_DriverData()
{
	int i;
	//int  stat_i2c;
    int TmpResult = 0;
    uint16_t _eepromAddr = 0xA0;
    int32_t tmp_value;
    uint8_t save_data[5];
    int16_t tempI16Val = 0;

    TmpResult = 1;

	if (Flag.DriverSaveParaData)
	{
        save_data[0] = dParameterValue[DriverSaveParaNumber];
        // when use i2c for DMA, you have set the I2C event interrupt.
        // read/write address를 1부터 start하므로..한개씩 write할때도 +1을 한다.
        HAL_I2C_Mem_Write(&hi2c1, _eepromAddr, DriverSaveParaNumber+1, I2C_MEMADD_SIZE_8BIT, (uint8_t*)save_data, 1, 100);
		Flag.DriverSaveParaData = OFF;
        HAL_Delay(10);
	}
	else if (Flag.DriverParaInit)	// save the driver
	{
        DriverDefaultPara();    // Set default parameter (dParameterValue)

		for (i=1; i < 13; i++)
		{
			dset[i] = dParameterValue[i];
		}

        HAL_I2C_Mem_Write(&hi2c1, _eepromAddr, 1, I2C_MEMADD_SIZE_8BIT, (uint8_t*)dset, 13, 100);
		Flag.DriverParaInit = OFF;
	}
	else	// read driver data
	{
        HAL_I2C_Mem_Read(&hi2c1, _eepromAddr, 1, I2C_MEMADD_SIZE_8BIT, (uint8_t*)dset, 13, 100);
        HAL_Delay(200);
        dset[0] = 10;       // version으로 사용..사실 필요 없음.

        // GEAR_EFFICIENCY 20% for factory
        if ((dset[3] < 80) || (dset[3] > 120))	
            dset[3] = 100;
        
        // driver model
        if ((dset[4] > 100) || (dset[4] < 1))	
            TmpResult = 0;

        // Serial number
        for(i=0; i < 4; i++)
        {
            if(dset[5 + i] >= 100)
                dset[5 + i] = 0;
        }
        
        // GEAR_EFFICIENCY 20% for user
        if ((dset[9] < 80) || (dset[9] > 120))	
            dset[9] = 100;

        // Driver torque offset (Kgf.cm 단위, 값 10 = 1.0Kgf.cm)
        tempI16Val |= ((uint16_t)dset[1] << 8) + dset[2];
        tqSlopeOffset = tempI16Val / 10.0;
        
        // 최저 토크의 10 ~ 30%
        if(Info_DrvModel[dset[4]].tq_min < 1.0)
            tqOffsetRange = 0.3;
        else
            tqOffsetRange = 0.3;

        // Check Offset limit (N.m에서 Kgf.cm로 변환)
        if ((tqSlopeOffset < -(Info_DrvModel[dset[4]].tq_min * tqOffsetRange * 10.2)) || (tqSlopeOffset > (Info_DrvModel[dset[4]].tq_min * tqOffsetRange * 10.2)))	
        {
            dset[1] = 0;
            dset[2] = 0;
            tqSlopeOffset = 0;
        }

        // user setting value
        Para[TORQUE_COMPENSATION_MAIN].val = dset[9];

        // Tranducer offset apply
        tmp_value = 0;
        tmp_value = ((uint16_t)dset[10] << 8) & 0xff00;
        tmp_value += dset[11];
        SensorOffset = tmp_value - AD_OFFSET_VALUE;

        if (labs(SensorOffset) > 1500)
        {
            if(system_err == 0)
                system_err = CHECK_TRANSDUCER;
            SensorOffset = 0;
        }

		if (TmpResult == 0)
		{
            // Driver data read fail
			dset[4] = 0;
            system_err = DRIVER_PARAMETER_ERROR;
            
            Flag.LockCommand = 1;
            // Flag_DRIVER_PARAMETER_ReadERROR = ON;       // if error appare, can't start motor.
            HAL_Delay(100);
		}
	}
}


// Default driver parameter initialize
void DriverDefaultPara(void)
{
    dParameterValue[0] = 10;	// VERSION 0.1
    dParameterValue[1] = 0;		// 기어비: 1단위.. 정수 실제 사용 안함.
    dParameterValue[2] = 0;		// 기어비: 0.1단위.. 소숫점.  value * 100 실제 사용 안함.
    dParameterValue[3] = 100;	// GEAR_EFFICIENCY
    dParameterValue[4] = 0; 	// DRIVER_TYPE

    dParameterValue[5] = 0; 	// SERIAL_NUMBER1
    dParameterValue[6] = 0;	    // SERIAL_NUMBER2
    dParameterValue[7] = 0;	    // SERIAL_NUMBER3
    dParameterValue[8] = 0;	    // SERIAL_NUMBER4

    dParameterValue[9] = 100;	// Gear efficiency .. user가 변경 가능..

    dParameterValue[10] = 127; 	// MSB sensor offset  0x7f
    dParameterValue[11] = 255; 	// LSB sensor offset  0xff
    dParameterValue[12] = 0; 	//
    dParameterValue[13] = 0; 	//
}


/**
 * @brief IWDG로 Wake up 판단 및 최대 절전 모드 실행.
 * 
 * @note  IWDG 초기화 문 다음에 실행하면 더욱 금방 실행 될 수 있다.
 *        1. IWDG 초기화 다음 23ms.
 *        2. 초기화가 다된 경우 55ms.
 *        3. IWDG 초기화 실행 안하고 그전에 실행하면 IWDG 재시작 안되는 현상.
 * 
 * @note  판단 절차
 *        1.IWDG 확인 - 2.voltage 확인 - 3.드라이버 레버 확인 - 3.1.레버 동작 및 배터리 양호 (재부팅) - 4.voltage 문제시 최대 절전 모드
 * 
 */
void PowerWakeUpJudge(void)
{
    float   Battery_voltage;
    uint8_t batType = 0;
    uint8_t maxPowerSaving = OFF;
    uint32_t adcAverage = 0;

    // IWDG로 부팅 확인
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB))
	{
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU | PWR_FLAG_SB);    // Clear flag
        HAL_IWDG_Refresh(&hiwdg);                           // Reset IWDG

        // Board power ON
        HAL_GPIO_WritePin(POWER_ENABLE_GPIO_Port, POWER_ENABLE_Pin, GPIO_PIN_SET);

        // Battery 상태 체크
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_value_buf, 5);
        HAL_Delay(20);

        // ADC값 Filter
        for(uint8_t i=0; i < 5; i++)
        {
            adcAverage += adc_value_buf[0];

            HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_value_buf, 5);
            HAL_Delay(1);
        }
        adcAverage = adcAverage / 5.0;

        // Voltage 계산
        Battery_voltage = (3.3 / 4096 * adcAverage) * 20.98765;

        if (Battery_voltage < 21)                                           // 18V battery
            batType = BATTERY_18V;
        else if ((Battery_voltage > 22.0) && (Battery_voltage <= 30.0))     // 24V battery
            batType = BATTERY_24V;
        else if ((Battery_voltage > 31.0) && (Battery_voltage <= 43.0))     // 40V battery
            batType = BATTERY_36V;


        // Check Voltage
        switch(batType)
        {
            case BATTERY_18V:
                // 절전 모드
                if (Battery_voltage < 17.5)
                    maxPowerSaving = 2;         // UNDER_VOLTAGE 이후 낮은 배터리 상태 - 이 상태에서는 레버 동작해도 재부팅 X
                
                // 최대 절전
                if (Battery_voltage < 16.5)
                    maxPowerSaving = ON;        // 완전 종료
            break;
            case BATTERY_24V:
                // 절전 모드
                if (Battery_voltage < 24.5)
                    maxPowerSaving = 2;
                
                // 최대 절전
                if (Battery_voltage < 23.1)
                    maxPowerSaving = ON;
            break;
            case BATTERY_36V:
                // 절전 모드
                if (Battery_voltage < 35.5)
                    maxPowerSaving = 2;
                
                // 최대 절전
                if (Battery_voltage < 33.0)
                    maxPowerSaving = ON;
            break;
            default:
                maxPowerSaving = ON;
            break;
        }

        // 전압 level OK, 드라이버 레버 동작 확인 - 재부팅
        if(HAL_GPIO_ReadPin(DR_START_IN_GPIO_Port, DR_START_IN_Pin) == 0 && maxPowerSaving == OFF)
        {
            return;
        }

        // Power OFF 또는 Sleep mode 재진입
        if(maxPowerSaving == ON)
        {
            // Power OFF
            HAL_GPIO_WritePin(POWER_ENABLE_GPIO_Port, POWER_ENABLE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(POWER_OFF_GPIO_Port, POWER_OFF_Pin, GPIO_PIN_RESET);
            HAL_Delay(10);
        }
        else
        {
            // Sleep mode ( IWDG 32s 부팅 설정 )
            hiwdg.Instance = IWDG;
            hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
            hiwdg.Init.Window = 4095;
            hiwdg.Init.Reload = 4095;
            if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
                Error_Handler();
        }

        __HAL_PWR_CLEAR_WAKEUP_FLAG(PWR_WAKEUP_PIN_FLAG1);
        HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);
        HAL_PWR_EnterSTANDBYMode();     // Sleep mode로 다시 진입
    }
}






/**
 * @brief Sleep mode 진입할지 결정 
 */
void sleepTimeCheck(void)
{
    static uint32_t timeout;

    // Sleep mode 파라미터 설정 시
    if (Para[SLEEP_IN_TIME].val != 0)
    {
        // 드라이버 동작시 시간 초기화
        if(Flag.Run)
            timeout = HAL_GetTick();

        // Tick time이 overflow되서 잘못 Sleep mode로 가는 것 방지)
        if(timeout > HAL_GetTick())
            timeout = HAL_GetTick();

        // Timeout 초기화
        if(timeout == 0)
            timeout = HAL_GetTick();

        // Time over 확인
        if (HAL_GetTick() - timeout > (Para[SLEEP_IN_TIME].val * 60000))     // (1 min) 해당 시간이 지나면 sleep mode in..
            Flag_power_down = ON;
    }
}


/**
 * @brief Set the OLED Message를 100ms 마다 전송하도록 설정 
 */
void setOledMessageOn(void)
{
    static uint32_t loop100ms;

    // Message 전송이 ON 되어있을 경우 시간을 좀 더 Delay
    if(Flag_SendOledData)
        loop100ms = HAL_GetTick();

    // Tick overflow 체크
    if(abs(loop100ms - HAL_GetTick()) > 10000)
        loop100ms = HAL_GetTick();

    // 100ms 주기
    if(HAL_GetTick() - loop100ms > 100)
    {
        Flag_SendOledData = ON;

        loop100ms = HAL_GetTick();
    }
}

// SPI 통신 Receive 콜백
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    uint16_t tqTemp;

    // Torque ADC value
    if (hspi->Instance == SPI2)
    {
        // make real value
        tqTemp = ((uint16_t)Tq_Sensor_read_data[0] & 0x03) << 14;
        tqTemp += ((uint16_t)Tq_Sensor_read_data[1] & 0xff) << 6;
        tqTemp += (Tq_Sensor_read_data[2] >> 2) & 0x3F;

        Tq_SensorValue = tqTemp;
        torqueReadOn = 0;
    }
    else if (hspi->Instance == SPI1)
    {
        /* 여기 코드 실행되는데 2.2us*/
        
        // Receive 완료 후 데이터 copy
        memcpy(bissSpi.spiRxComplete, spiRecvBuf, 4);

        // 통신 데이터 처리
        maxonBissDataAnalyze();

        // SVPWM 실행 (받은 위치 데이터를 바로 적용)
        // svpwmPositionApply();
    }
}



/**
 * @brief Torque를 읽는 상태를 확인.
 * 
 * @note  DMA로 요청하여 토크를 읽는다 -> 요청 후 실패했을 때 재시작 못하는 경우를 방지하기 위해 실행
 */
void torqueReadState(void)
{
	static uint16_t torqueValue;
	static uint32_t timeout;
    static uint8_t  errorOn;

	// Check request change
	if(torqueReadOn == OFF)
		timeout = HAL_GetTick();

	// Check torque ADC value change
	if(torqueValue != Tq_SensorValue)
		timeout = HAL_GetTick();

	// 5초 이상 값의 변화가 없을 경우 (10s 이후 실행)
	if(HAL_GetTick() - timeout > 5000 && HAL_GetTick() > 10000 && errorOn == OFF)
	{
        // Error
        if(system_err == 0)
		    system_err = CHECK_TRANSDUCER;

        errorOn = ON;                       // Error 1회만 실행
		torqueReadOn = OFF;                 // DMA를 다시 시작하기 위해 초기화
        Flag.LockCommand = 1;               // Driver lock
	}

	// Save
	torqueValue = Tq_SensorValue;
}


/**
 * @brief UART의 상태를 확인 후 복구한다.
 * */
void checkUartState(void)
{
    // OLED UART state 확인
    if(huart1.RxState != HAL_UART_STATE_BUSY_RX)
        HAL_UART_Receive_DMA(&huart1, (uint8_t*) OledRx_data, 1);

}


/**
 * @brief Bit socket tray가 연결된 후 접속이 끊어졌을 때 드라이버의 Lock을 제어한다.
 */
void bitsocketLockControl(void)
{
    static uint32_t timeout;
    uint8_t socketNum = 0;
    int calc;

    // Check bit sockcet mode 활성화
    if(Flag_bitsocket_mode == 0)
        return;

    // Find bit socket이 접속한 socket number
    for(uint8_t i=0; i < RSI_NUMBER_OF_SOCKETS; i++)
    {
        if(socketControl.socketState[i].bitsocket)
        {
            socketNum = i;
            break;
        }
    }    

    // 접속 끊어졌을 경우 Lock
    if(socketControl.socketState[socketNum].connect == OFF)
    {
        calc = HAL_GetTick() - timeout;

        if(abs(calc) > 1000)
        {
            // Lock, Bit socket 모드 비활성화
            Flag.LockCommand = 1;
            Flag_bitsocket_mode = 0;
        }
    }
    else
        timeout = HAL_GetTick();
}


/**
 * @brief 현재 에러 코드가 Fastening NG 에러 코드인지 판단
 * 
 * @param errorCode : 에러코드
 * @return uint8_t 0: System error, 1: Fastening NG
 */
uint8_t checkFasteningNgError(uint16_t errorCode)
{
    uint8_t retval = 0;

    if ((errorCode == FASTENING_TIMEOUT) 			        ||
        (errorCode == LOOSENING_TIMEOUT) 			        ||
        (errorCode == FASTENING_TURN_MIN_ERROR) 			||
        (errorCode == FASTENING_TURN_MAX_ERROR) 			||
        (errorCode == OVER_TIME_DURING_LOOSENING) 			||
        (errorCode == OVER_TORQUE_BEFORE_RAMPUP) 			||
        (errorCode == FASTENING_STOP_ERROR) 				||
        (errorCode == FIND_ENGAGING_TORQUE_ERROR) 		    ||
        (errorCode == C_TORQUE_ERROR) 					    ||
        (errorCode == FASTEN_OVER_TORQUE_ERROR) 			||
        (errorCode == TORQUEUP_DURING_FREE_SPEED_ERROR) 	||
        (errorCode == THREADTAP_MAX_TORQUE_ERROR) 		    ||
        (errorCode == THREADTAP_MIN_MAX_RANGE_ERROR) 		||
        (errorCode == SEATING_TQ_ERROR) 					||
        (errorCode == SEATING_ANGLE_ERROR) 				    ||
        (errorCode == COMPENSATION_OVER_MAX_TQ) 			||
        (errorCode == CLAMP_TQ_ERROR) 					    ||
        (errorCode == A2_ANGLE_ERROR) 					    ||
        (errorCode == NO_FINSH_PREVAILING) 				    ||
        (errorCode == PREVAILING_TQ_ERROR) )
    {
        retval = 1;
    }

    return retval;
}
