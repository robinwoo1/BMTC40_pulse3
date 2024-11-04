/**
  ******************************************************************************
  * @file    timer.c
  * @brief   timer Service Routines.
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "main.h"
#include "common.h"
#include <stdlib.h>
#include <math.h>
#include "variable.h"
#include "multicastLudp.h"
#include "velocity.h"
#include "main_bg.h"
#include "display.h"
#include "backup.h"
#include "advancedPlus.h"
#include "wifiTask.h"
#include "socketControl.h"
#include "BldcControl.h"
#include "bissSpiCommunicate.h"
#include "svpwm.h"
#include "realtimeMonitoring.h"

/* Extern Variables ---------------------------------------------------------*/
//IWDG
extern IWDG_HandleTypeDef hiwdg;



/* Functions ---------------------------------------------------------------*/
void SysTick_loop(void)         // 1ms
{
    static uint16_t gCnt_timer10;
    static uint16_t gCnt_timer200;
    static uint16_t gCnt_once=0;
    static uint16_t CntTimer500ms;

    gCnt_timer200++;
    gCnt_once++;


    // Clear IWDG (Reset over 512ms)
	if(hiwdg.Instance == IWDG)
        HAL_IWDG_Refresh(&hiwdg);
	 	
    // LED
    if (gCnt_timer200 > 200)     // 200ms
    {
        gCnt_timer200 = 0;
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }


    // After 2sec , Monitoring timeout
    if (gCnt_once > 2000)
    {
        gCnt_once = 3001;
        Flag_sec_delay_after_power_on = ON;
    }

    // 2sec 이후에 실행..
    if (Flag_sec_delay_after_power_on)
    {
        gCnt_timer10++;
        if (gCnt_timer10 > 9) // 10ms
        {
            Process_10ms();   // 10ms process execution
            gCnt_timer10 = 0;
        }

        // if modbus receive packet start이구 30ms가 지나면.. 다시 초기화 한다.
        if (Cnt_ComRead_Buffer_a > 0)   // mobbus packet start
        {
            Cnt_OverTimeCheckModBus++;
            if (Cnt_OverTimeCheckModBus > 30)
            {
                Cnt_ComRead_Buffer_a = 0;           // 다시 받는다.
                Cnt_OverTimeCheckModBus = 0;
            }
        }
        else
            Cnt_OverTimeCheckModBus = 0;


        // 레버, L/F 스위치 제어
        FastenLoosenControl();

	    // Encoder 값 오류 검출
        checkEncoderError();
    }

	if (Flag.OneTimeExecute)
	{
		CntTimer500ms++;
		if (CntTimer500ms >= 3000)	// 3 sec   초기 booting시에 한번 실행한다.
		{
			CheckDriverModel();

			Flag.OneTimeExecute = OFF;
		}		
		else
		{
			// 초기화 중 시작 방지
			if(Flag.Run)
				Flag.Stopping = ON;
		}
	}
}

void Process_10ms(void)
{
    static uint8_t Cnt_100ms;

    ms_process();
    Error_process();

    // 100ms
    if(Cnt_100ms >= 10)
    {
        DriverLED_Control();

        Cnt_100ms = 0;
    }
    else 
        Cnt_100ms++;


    // Screw 개수 카운트 및 시작 제어
    screwCountReset();
    Handling_FlagExtOutput();
    get_tq_sensor_value();

}

void Light_Control(void)	// 100ms
{
    static uint32_t driverRunTime;
    uint8_t Flag_LightOnOff;

    // if used LED for Lamp
    if (Para[LAMP_ON_TIME].val != 0)  
    {
        // Driver 동작 중에는 ON
        if(Flag.Run)
            driverRunTime = HAL_GetTick();

        // 드라이버 정지 후 설정된 시간 만큼 만 ON
        if (HAL_GetTick() - driverRunTime > (Para[LAMP_ON_TIME].val * 1000))     // 1s 단위
            Flag_LightOnOff = OFF;	    // off
        else
            Flag_LightOnOff = ON;       // on
    }
    else    
        Flag_LightOnOff = OFF;

    if (Flag_LightOnOff)
        HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_SET);    // on
    else
        HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_RESET);  // off

}

void DriverLED_Control(void)	// 100ms
{
	static int cnt500     = 0;
	static int flagToggle = 0;

	cnt500++;
	if (cnt500 > 5)    // 500ms
	{
		flagToggle = !flagToggle;
		cnt500 = 0;
	}

    if (Flag.Lock)      // if lock, Yellow LED.
    {
        if (flagToggle)
        {
            Flag_RedLED   = ON;     // red
            Flag_GreenLED = ON;		// green
        }		
        else
        {
            Flag_RedLED   = OFF;	// red
            Flag_GreenLED = OFF;	// green
        }         
    }
	else if (system_err == 0)        // ok condition
	{
		if (Flag.ControlFL == FASTENING_CONTROL)
		{
			if (!FlagFasten.FasteningErrorForLED)
			{
				Flag_GreenLED = ON;		// green
				Flag_RedLED   = OFF;	// red

				if (!Flag.Run)
				{
                    Flag_RedLED = OFF;	// red					
				}
				else
				{
					Flag_RedLED = ON;	// red => yellow display
				}
			}
			else
			{
				Flag_RedLED   = ON;		// red 
				Flag_GreenLED = OFF;	// green
			}		
		}
		else 		// loosening
		{
			if (flagToggle)
			{
				Flag_RedLED   = OFF;	// red
				Flag_GreenLED = OFF;	// green
			}		
			else
			{
				Flag_RedLED   = OFF;	// red
				Flag_GreenLED = ON;		// green
			} 
		}
	}
	else        // error condition
	{
        // Fastening error 확인
		if(checkFasteningNgError(system_err))
			FlagFasten.FasteningErrorForLED = ON;
		else
			FlagFasten.FasteningErrorForLED = OFF;

        // fastening error시 다음 동작시 까지 red on
		if (FlagFasten.FasteningErrorForLED) 		
		{
			Flag_RedLED   = ON;		// red
			Flag_GreenLED = OFF;	// green
		}
		else 		// error toggle yellow/red
		{
			if (flagToggle)
			{
				Flag_RedLED   = ON;		// red
				Flag_GreenLED = OFF;	// green
			}		
			else
			{
				Flag_RedLED   = ON;		// red
				Flag_GreenLED = ON;		// green
			} 	
		}
	}
}


/**
 * @brief 정역 스위치 및 Start 레버 동작 제어
 * 
 */
void FastenLoosenControl(void)  // 1ms
{
    static int str_edge[2]       = {0,0};
    static int loosen_edge[2]    = {0,0};
    static int flagrun_edge[2]   = {0,0};
    static int Flag_TriggerStart = 0;
    static uint8_t startSignalBuf[52] = {0, };
	static uint8_t looseningSignalBuf[52] = {0, };
    static uint16_t Cnt_TriggerStart = 0;
    uint16_t paraTriggerDelay;

    // Check 드라이버 동작
    flagrun_edge[0] = flagrun_edge[1];
    flagrun_edge[1] = Flag.Run;


    // 드라이버 최소 재동작 Delay 시간  (MD같이 즉시 동작해야 하는 것은 시간 조정 필요)
    paraTriggerDelay = Para[TRIGGER_START_DELAY_TIME].val;

    // 최소 재동작 시간 100ms
    if(paraTriggerDelay == 0)
        paraTriggerDelay = 100;

    // 스위치 트리거 Delay
    if (paraTriggerDelay != 0)
    {
        if (flagrun_edge[0] > flagrun_edge[1])  // if off
        {
            Cnt_TriggerStart  = 0;
            Flag_TriggerStart = ON;
        }

        if (Flag_TriggerStart)
        {
            Cnt_TriggerStart++;
            if (Cnt_TriggerStart > paraTriggerDelay)
            {
                Flag_TriggerStart = OFF;
            }
        }
    }
    else
        Flag_TriggerStart = OFF;

    // 트리거 Delay Return
    if (Flag_TriggerStart)      // if time setting, skip start switch.. 
        return;

    // LCD Setting 모드에서는 드라이버 동작 중지
    if (current_mode < _OPERATION)      // operation과 monitoring 이 아니면 반영을 하지 않는다.
    {
        if(Flag.Run)
            Flag.Stopping = ON;
            
        return;
    }


    // stopping 중에는 data를 반영 안한다. 가감속으로 인해 정/역이 변환이 되지 않는 현상이 있다. (Start, F/L 스위치 변화에 Delay 5ms)
    if (!Flag.Stopping)
    {
        // 레버 스위치 반영
        Input_StartInternal = !HAL_GPIO_ReadPin(DR_START_IN_GPIO_Port, DR_START_IN_Pin);

        str_edge[0] = str_edge[1];
        str_edge[1] = driverFilter(startSignalBuf, Input_StartInternal, str_edge[0], 50);				// Filter 50 횟수

        // L/F 스위치 반영
        if (Para[LF_SWITCH_TYPE].val == 0)
            Input_LoosnFasten = !HAL_GPIO_ReadPin(DR_LF_IN_GPIO_Port, DR_LF_IN_Pin);
        else
            Input_LoosnFasten = HAL_GPIO_ReadPin(DR_LF_IN_GPIO_Port, DR_LF_IN_Pin);
            
        loosen_edge[0] = loosen_edge[1];
        loosen_edge[1] = driverFilter(looseningSignalBuf, Input_LoosnFasten, loosen_edge[0], 50);		// Filter 50 횟수
    }

    // 레버 스위치 동작
    if (str_edge[0] < str_edge[1])      // on starting
    {
        // Lock 확인
        if(Flag.Lock == OFF)
        {
            if (Flag.Multi_Mode && (Flag.ControlFL == FASTENING_CONTROL))   // mult mode는 fastening에서만 동작..
                Flag.Multi_Start = ON;
            else
                Flag.InternalRun = ON;
        }

        // OLED 화면 전환
        if (current_mode == _MONITORING_MODE)
            current_mode = _OPERATION;
    }
    else if ((str_edge[0] > str_edge[1]) && (Flag.InternalRun || Flag.Multi_Start)) // off stopping
    {
        // off 시에는 현재 정/역 상태를 반영한다.
        if (Input_LoosnFasten)  Flag.ControlFL = LOOSENING_CONTROL;
        else                    Flag.ControlFL = FASTENING_CONTROL;

        Flag.Multi_Start = OFF;
        Flag.Stopping    = ON; 
    }

    // L/F 스위치 동작
    if (Para[REVERSE_LOCK].val == 0 || Para[REVERSE_LOCK].val == 2)
    {
        if ((loosen_edge[0] < loosen_edge[1]) && !Flag.InternalRun)     // loosen
        {
            Flag.ControlFL = LOOSENING_CONTROL;

            StatusOutput = rF_L;
            ClearSendData_Init(ON);
            putRealtimeData(&realPollStruct);
        }
        else if ((loosen_edge[0] > loosen_edge[1]) && !Flag.InternalRun)
        {
            Flag.ControlFL = FASTENING_CONTROL;

            StatusOutput = rF_L;
            ClearSendData_Init(ON);
            putRealtimeData(&realPollStruct);
        }           
    }
    else if(Para[REVERSE_LOCK].val == 1)
    {
        // Always fastening mode
        Flag.ControlFL = FASTENING_CONTROL;
    }

    if(Para[REVERSE_LOCK].val == 2)
    {
        // Loosening Lock 해제
        if(Flag.LockCommand == 1)
            Flag.LockCommand = 3;       // Both lock -> Fastening lock 으로 변경
        else if(Flag.LockCommand == 2)
            Flag.LockCommand = 0;       // Loosening lock -> Unlock 으로 변경
    }
}


/**
 * @brief 드라이버의 모델과 설정한 모델이 같은지 비교
 */
void CheckDriverModel(void)
{
	if (dset[4] != Para[DRIVER_MODEL].val)
	{
		if(system_err == 0)
		{
            system_err = NOT_COMPATABLE_DRIVER;
            
            Flag.LockCommand = 1;
        }	
	}
}


/**
 * @brief Screw count가 0이 되었을 때 초기화 하는 함수
 */
void screwCountReset(void)
{
	// Auto mode에서 SCREW COUNT 복구
	if(ScrewCountValue == 0)
	{
		// Check screw count number
		if(Para[COUNT_SCREW].val == 0)
			return;

		// Screw count reset
		if(Para[COUNT_MODE].val == 0 && Para[MODEL_SELECT].val == 0 && Para[AUTO_LOCK].val == 0 && Flag.Run == OFF)
		{
			ScrewCountValue = Para[COUNT_SCREW].val;

			StatusOutput = rScrewCountReset;
			ClearSendData_Init(ON);
			putRealtimeData(&realPollStruct);	
		}
	}
}


/**
 * @brief 체결에 관한 데이터를 초기화 한다.
 * 
 * @param flagResetTorque : 1 = 체결 토크값을 초기화, 0 = 초기화 X
 */
void ClearSendData_Init(uint8_t flagResetTorque)
{
    Fastening_time      = 0;
    Disp_Loosening_time = 0;

    mPulse.After_Fastening_Start_ForA1 = 0;
    mPulse.After_Fastening_Start_ForA2 = 0;
    mPulse.DisplayLoosening = 0;
    mPulse.RealSnugTq = 0;

    // 토크 초기화
    if(flagResetTorque)
    {
        Torque_cnv.fastenComplete = 0;

        // Advanced value
        Torque_cnv.seating      = 0;
        Torque_cnv.clamp 	    = 0;
        Torque_cnv.prevailing   = 0;
        Torque_cnv.snug = 0;
    }
}


// Error의 리셋 및 저장 관리
void Error_process(void)
{
    static uint16_t Cnt_PowerOnDelay_500ms=0;
    static uint16_t Cnt_ErrorCntTime=0;

    // Change display OLED at error
    if(system_err != 0)
    {
    	current_mode = _ALARM;
    }


	// Error Auto Reset process
	if (system_err != 0)
	{
		Flag.Run = OFF;
        Flag.InternalRun = OFF;
        Flag.ExternalRun = OFF;
        Flag.RunByMult   = OFF;
        Flag.Multi_Start = OFF;

        // Under voltage 에러는 화면에 일정 시간 표시
		if (system_err == UNDER_VOLTAGE)
		{
			Cnt_PowerOnDelay_500ms++;

			if (Cnt_PowerOnDelay_500ms < 100)
				return;
		}

		Cnt_ErrorCntTime++;

		if (getParaVal(ERROR_CLEAR_TIME_H) == 0)		// 자동으로 error를 clear하지 않는다
		{
			Cnt_ErrorCntTime = 0;
		}
		else
		{
            // Error Auto reset 불가능한 에러코드
			if ( (system_err == UNDER_VOLTAGE)        || (system_err == OVER_TEMPERATURE_MOTOR) || system_err == TOTAL_SCREW_OVER     || (system_err == ENCODER_OPEN_ERROR) ||
			 	//  (system_err == OVER_TEMPERATURE_BAT) || (system_err == PARA_CHECKSUM_ERROR)    || (system_err == HALL_SENSOR_OPEN)   || (system_err == MONITORING_DATA_READ_WRITE))
        (system_err == OVER_TEMPERATURE_BAT) || (system_err == PARA_CHECKSUM_ERROR)    || (system_err == MONITORING_DATA_READ_WRITE))
			{
				Cnt_ErrorCntTime = 0;
			}
			else 	// 나머지는 모두 setting 된 시간 후에 자동 복구
			{
				if (Cnt_ErrorCntTime > (getParaVal(ERROR_CLEAR_TIME_H) * 100))   //  10(주기) * 100ms
					Flag.Reset = ON;
			}
		}
	}
	else
	{
		Cnt_ErrorCntTime = 0;
		Cnt_PowerOnDelay_500ms = 0;
	}

    // Memory에 에러코드 저장
	Save_errorCode();

    // controller and driver data read 에러는 반드시 전원을 다시 켜야 한다.
	if ((system_err == NOT_COMPATABLE_DRIVER) || (system_err == NOT_RECOGNIZE_CONTROLLER) || (system_err == DRIVER_PARAMETER_ERROR))
		Flag.Reset = OFF;

}


/**
 * @brief Error code를 Memory에 저장하는 것 처리
 */
void Save_errorCode(void)
{
	int errorOccur;
	static int      edg_Flag_Reset[2] = {0,0};
    static uint16_t Flag_save60sec    = 0;
    static uint16_t Cnt_save60sec     = 0;
    static uint16_t MonitoringCheck[2];
    static uint32_t MonitoringTime;

	// 상승에서 한번만 저장한다.
	if(system_err == 0) errorOccur = 0;
	else                errorOccur = 1;

	edg_Flag_Reset[1] = edg_Flag_Reset[0];
	edg_Flag_Reset[0] = errorOccur;

	if (edg_Flag_Reset[1] < edg_Flag_Reset[0])		// error event 를 발생 시켜 해당 값을 상위에 올려 준다. 모든 error에 대하여 다 올려 준다.
	{
		if (!Flag.Reached_LITTLE_REWIND) 	// off면 도착을 하지 않은 것으로 기존 pick값을 보여 준다.
		{
			if(Flag.ControlFL == FASTENING_CONTROL) 	// loosening 일 경우에는 loosening 값을 보여 준다.
			{
				Torque_cnv.fastenComplete = Fasten_Real_Torque;
				Torque_cnv.fastenComplete_dsp_comp = Torque_cnv.fastenComplete * inv_Torque_ConvertValue;
			}
		}

		Flag.Stopping = ON;

		if (checkFasteningNgError(system_err))
		{
			StatusOutput = rFASTENING_NG;
		}
		else
			StatusOutput = rSYSTEM_ERR;	

        MonitoringCheck[0] = MonitoringCheck[1];
        MonitoringCheck[1] = system_err;

        // 2s안에 같은 System error일 시 Memory에 저장 안함
        if(MonitoringCheck[0] == MonitoringCheck[1] && (system_err < 300 || system_err > 342) && HAL_GetTick() - MonitoringTime < 2000)
        {
            // Pass
        }
        else
            putRealtimeData(&realPollStruct);
        
        MonitoringTime = HAL_GetTick();
	}

	// error 발생시 동일한 에러에 대하여 60초 이내에 또 발생을 하면 저장을 하지 않는다.
	// 이유는 자동 Reset을 하기 때문에 계속 저장을 할 경우 동일한 에러로 모두 저장이 된다.
	// 단, 60초이내에도 다른 에러가 발생을 할 경우에는 저장을 한다.
	if (Flag_save60sec)		++Cnt_save60sec;
	else					Cnt_save60sec = 0;

	if (Cnt_save60sec > 6000)	
	{
		Cnt_save60sec  = 0;
		Flag_save60sec = OFF;
	}

    // System err only save
	if ((system_err < 300 || system_err >= 400) && (system_err != 0) && \
        (system_err != UNDER_VOLTAGE) && (system_err != OVER_TEMPERATURE_MOTOR) && (system_err != OVER_TEMPERATURE_BAT) && (system_err != OVER_TEMPERATURE_PCB) && \
        (edg_Flag_Reset[0] > edg_Flag_Reset[1]))		
	{
		if (!Flag_save60sec || (system_err != errorCodeList[0].code))
		{
			Flag_save60sec   = ON;
			Cnt_save60sec    = 0;

            // Error code list 추가
			addErrorCodeList(system_err);
		}
	}
}


/**
 * @brief Battery의 voltage 확인  및 battery 잔량 계산
 */
void battery_voltage_check(void)
{
    static float Battery_voltage = 0;
    static uint8_t batteryInit = 0;
    float temp = 0;

    // 초기 잘못된 값 제거
    if(HAL_GetTick() < 3000 && adc_value_buf[0] < 500)
        return;

    // 초기 평균값 계산
    if(Battery_voltage == 0)
    {
        for(uint8_t i=0; i < 10; i++)
            adcVoltageAverage(adc_conv_value * (float)adc_value_buf[0]);
    }

    Battery_voltage = adcVoltageAverage(adc_conv_value * (float)adc_value_buf[0]);
    real_battery_voltage = Battery_voltage * 20.98765;

    if (batteryInit == 0)    // 초기 3sec만 실행..
    {
        BatteryType = BATTERY_24V;

        batteryInit = ON;
    }
   
    // cell 당 Low voltage : 3.4V,,  max voltage : 4.2V
    if (BatteryType == BATTERY_18V)
    {
        if (real_battery_voltage < 17)     // 17v 이하면..
        {
            system_err = UNDER_VOLTAGE;
        }

        // 18V 기준
        // cal level
        temp = real_battery_voltage - 17;     // 20V = 100%,  17V = 0% (실제 10% 정도 남음)
        battery_level =  (uint8_t)(temp * 36.3);
        if (battery_level > 100)
        {
            battery_level = 100;
        }
    }
    else if (BatteryType == BATTERY_24V)
    {
        if (real_battery_voltage < 23.8)
        {
            system_err = UNDER_VOLTAGE;
        }

        // 24V 기준
        // cal level
        temp = real_battery_voltage - 23.8; // 29V = 100%, 23.8V = 0% check
		battery_level = (uint8_t)(temp * 22.22223);
        if (battery_level > 100)
        {
            battery_level = 100;
        }
    }
    else if (BatteryType == BATTERY_36V)
    {
        if (real_battery_voltage < 34.0)
        {
            system_err = UNDER_VOLTAGE;
        }

        // 40V 기준
        // cal level
        temp = real_battery_voltage - 34.0;     // 42V = 100%,  34.0V = 0%      check
        battery_level =  (uint8_t)(temp * 15.5);
        if (battery_level > 100)
        {
            battery_level = 100;
        }
    }
    else
        system_err = UNDER_VOLTAGE;

    // - 값 확인
    if(temp < 0)
        system_err = UNDER_VOLTAGE;

    // Calculate display battery level
    calcDisplayBattery();

}


// Battery의 전압 레벨을 측정한다
void calcDisplayBattery(void)
{
    static float battery_level_filter;

    // 초기 부팅시 전력 감지를 위해 3s 후 Under voltage 에러 반영
    if (Flag.OneTimeExecute)
    {
        // Voltage error 초기화
        if(system_err == UNDER_VOLTAGE)
            system_err = 0;
    }

    // 초기 배터리 값 반영
    if(battery_level_filter == 0)
        battery_level_filter = battery_level;

    // 2% 배터리 변화 반영
    battery_level_filter = battery_level * 0.02 + battery_level_filter * 0.98;
    battery_level = battery_level_filter;
}


/**
 * @brief 드라이버가 동작 중 전류를 많이 사용하여 전압이 떨어졌을 때 드라이버 정지 후 Error를 표시하는 함수
 */
void battery_voltage_check_with_run(void)
{
    static float Battery_voltage = 0;
    static uint32_t errorDisplayTime;
    static uint16_t batteryError;
    static uint8_t batteryInit = 0;
    float calcVoltage;
    float temp;

    // 초기화 시간
    if(HAL_GetTick() > 12000)
        batteryInit = ON;

    // 초기화 완료 후
    if(batteryInit)
    {
        // 배터리 전원 값 초기화
        if(Battery_voltage == 0)
            Battery_voltage = adc_conv_value * (float)adc_value_buf[0];

        // Voltage 평균값 계산
        temp = adc_conv_value * (float)adc_value_buf[0];
        Battery_voltage = Battery_voltage * 0.8 + temp * 0.2;
        calcVoltage = Battery_voltage * 20.98765;

        // cell 당 Low voltage : 3.3V,,  max voltage : 4.2V
        if (BatteryType == BATTERY_18V)
        {
            // 18V (Cell:5  5x3.3 = 16.5)
            if (calcVoltage < 16.5)
            {
                batteryError = UNDER_VOLTAGE;
            }
        }
        else if (BatteryType == BATTERY_24V)
        {
            // 25.2V (Cell:7  7x3.3 = 23.1)
            if (calcVoltage < 23.1)
            {
                batteryError = UNDER_VOLTAGE;
            }
        }
        else if (BatteryType == BATTERY_36V)
        {
            // 40V (Cell:10  10x3.3 = 33)
            if (calcVoltage < 33.0)
            {
                batteryError = UNDER_VOLTAGE;
            }
        }

        // 드라이버가 정지되었을 때 에러 표시
        if(Flag.Run == OFF && batteryError && errorDisplayTime == 0)
            errorDisplayTime = HAL_GetTick();

        // 정지 후 100ms 뒤에 에러 표시
        if(HAL_GetTick() - errorDisplayTime > 100 && errorDisplayTime != 0 && batteryInit == 1)
        {
            system_err = UNDER_VOLTAGE;     // Battery 전원 에러
            batteryInit = 2;                // 1회만 발생되도록 제한
        }
    }
}


// Motor의 온도 확인
void temperature_check_motor(void)
{
    static uint32_t timeout;
    float rdVoltage;
    float R2R;
    float Ktmp;
    float Temperature_real_motor;
	
    if(HAL_GetTick() - timeout > 1000)
    {
        timeout = HAL_GetTick();

        rdVoltage = adc_value_buf[4] * 3.3 / 4096;     // 전압으로 바꾸고..
        R2R = rdVoltage / (5.0 - rdVoltage) * 10000.0;  // 저항값을 찾고..
        //Ktmp = 1.0 / ((1.0 / (273.15 + 25.0)) + (1.0 / 3435.0) * log(R2R / 10000.0));       // 3435 = 25도 ~ 85도 온도 상수.. 온도 계산.
        // R2R = rdVoltage / (6.85 - rdVoltage) * 22000.0;  // 저항값을 찾고..
        Ktmp = 1.0 / ((1.0 / (273.15 + 25.0)) + (1.0 / 3435.4) * log(R2R / 10000.0));       // 3435 = 25도 ~ 85도 온도 상수.. 온도 계산.
        Temperature_real_motor = Ktmp - 273.15;   // 절대 온도를 뺀다.
        
        Temperature_real_Flt_motor = (Temperature_real_motor * 0.3) + (Temperature_real_Flt_motor * 0.7);

        if (Temperature_real_Flt_motor > 65.0)    // 65도 TODO: 온도 임시로 일단 70 -> 65도로 내림
        {
			if(system_err == 0)
            	system_err = OVER_TEMPERATURE_MOTOR;
        }
    }
}


// PCB의 온도 확인
void temperature_check_pcb(void)
{
    static uint32_t timeout;
    float rdVoltage;
    float R2R;
    float Ktmp;
    float Temperature_real_pcb;

    if(HAL_GetTick() - timeout > 1000)
    {
        timeout = HAL_GetTick();

        rdVoltage = adc_value_buf[1] * 3.3 / 4096;     // 전압으로 바꾸고..
        R2R = rdVoltage / (5.0 - rdVoltage) * 10000.0;  // 저항값을 찾고..
        //Ktmp = 1.0 / ((1.0 / (273.15 + 25.0)) + (1.0 / 3435.0) * log(R2R / 10000.0));       // 3435 = 25도 ~ 85도 온도 상수.. 온도 계산.
        // R2R = rdVoltage / (6.85 - rdVoltage) * 22000.0;  // 저항값을 찾고..
        Ktmp = 1.0 / ((1.0 / (273.15 + 25.0)) + (1.0 / 3435.4) * log(R2R / 10000.0));       // 3435 = 25도 ~ 85도 온도 상수.. 온도 계산.
        Temperature_real_pcb = Ktmp - 273.15;   // 절대 온도를 뺀다.
        
        Temperature_real_Flt_pcb = (Temperature_real_pcb * 0.1) + (Temperature_real_Flt_pcb * 0.9);

        if (Temperature_real_Flt_pcb > 70.0)    // 70도
        {
            if(system_err == 0)
                system_err = OVER_TEMPERATURE_PCB;
        }
    }
}


// Battery의 온도 확인
void temperature_check_battery(void)
{
    static uint32_t timeout;
    float rdVoltage;
    float R2R;
    float Ktmp;
    float Temperature_real_bat;

    if(HAL_GetTick() - timeout > 1000)
    {
        timeout = HAL_GetTick();

        rdVoltage = adc_value_buf[3] * 3.3 / 4096;     // 전압으로 바꾸고..
        R2R = rdVoltage / (5.0 - rdVoltage) * 10000.0;  // 저항값을 찾고..
        //Ktmp = 1.0 / ((1.0 / (273.15 + 25.0)) + (1.0 / 3435.0) * log(R2R / 10000.0));       // 3435 = 25도 ~ 85도 온도 상수.. 온도 계산.
        // R2R = rdVoltage / (6.85 - rdVoltage) * 22000.0;  // 저항값을 찾고..
        Ktmp = 1.0 / ((1.0 / (273.15 + 25.0)) + (1.0 / 3435.4) * log(R2R / 10000.0));       // 3435 = 25도 ~ 85도 온도 상수.. 온도 계산.
        Temperature_real_bat = Ktmp - 273.15;   // 절대 온도를 뺀다.
        
        Temperature_real_Flt_bat = (Temperature_real_bat * 0.2) + (Temperature_real_Flt_bat * 0.8);

        if (Temperature_real_Flt_bat > 50.0)    // 50도
        {
			if(system_err == 0)
            	system_err = OVER_TEMPERATURE_BAT;
        }
    }
}


/**
 * @brief 드라이버 Lock 관리
 * @note 0: No Lock,  1: All Lock,  2: Loosening Lock,  3: Fastening Lock
 */
void Lock_Command_process(void)
{
	if (Flag.LockCommand == 1) 	    // all lock
		Flag.Lock = ON;
	else if (Flag.LockCommand == 2)	// loosening lock
	{
		if (Flag.ControlFL == FASTENING_CONTROL)
			Flag.Lock = OFF;
		else
			Flag.Lock = ON;
	}
	else if (Flag.LockCommand == 3)	// fastening  lock
	{
		if (Flag.ControlFL == FASTENING_CONTROL)
			Flag.Lock = ON;
		else
			Flag.Lock = OFF;
	}
	else 	// 0이면 해제..
	{		
	}
}


/**
 * @brief Torque up 200ms 동안 ON 유지 후 -> OFF 시퀀스
 */
void Handling_FlagExtOutput(void)		// 10ms rootin
{
	static uint16_t Cnt_OutputSignalDelay   = 0;
	static uint16_t Cnt_TorqueUpcompleteOut = 0;

    // 부팅 후 1초 이후부터 신호 제어
	if (Cnt_OutputSignalDelay < 100)	
	{
		Cnt_OutputSignalDelay++;

		Flag.FasteningCompleteOut = OFF;
		Cnt_FasteningCompleteOut  = 0;
		Cnt_TorqueUpcompleteOut   = 0;
	}
	else
	{
		if (Flag.FasteningCompleteOut)   // fastening  Complete 
		{
            Cnt_FasteningCompleteOut++;
			if(Cnt_FasteningCompleteOut > 20)        // 200ms
				Flag.FasteningCompleteOut = OFF;
		}
		else
		{
			Cnt_FasteningCompleteOut = 0;
		}

		// torque up 신호로 다른곳에서 사용중..단지 output 출력만 막음..
		if (Flag.TorqueUpCompleteOut)      // Torque up Complete 
		{
			Cnt_TorqueUpcompleteOut++;
			if(Cnt_TorqueUpcompleteOut > 20)   //200ms 유지..
			{
				Flag.TorqueUpCompleteOut = OFF;
			}
		}
		else
		{
			Cnt_TorqueUpcompleteOut = 0;
		}
    }
}


void get_tq_sensor_value(void)
{
	static uint16_t CntGetSensor  = 0;
	static uint32_t add_read_sign_value = 0;
	int32_t tmp_value;

	if (Flag_GetSensorOffset)		// 10ms
	{
		CntGetSensor++;
		add_read_sign_value += Tq_SensorValue_Avg;

		if (CntGetSensor >= 30)		// about 300ms
		{
			tmp_value = add_read_sign_value / 30;
			dset[10] = (tmp_value >> 8) & 0xff;
			dset[11] = tmp_value & 0xff;
			SensorOffset = tmp_value - AD_OFFSET_VALUE;

            if (labs(SensorOffset) > 1500)
            {
                if(system_err == 0)
                    system_err = CHECK_TRANSDUCER;
				SensorOffset = 0;
            }			
			Flag_GetSensorOffset = OFF;
		}
	}
	else
	{
		CntGetSensor = 0;
		add_read_sign_value = 0;
	}
}


/**
 * @brief 전원 값에 필터를 적용한다.
 * 		- 입력 값을 계속해서 더한 후 예전 값을 1개씩 제거하여 평균을 구한다 
 * 
 * @param  input 	: 전원 AD 값 
 * @return average	: 필터적용 후 AD 값
 */
float adcVoltageAverage(float input)
{
    static float  data[15];	                    // 필터 값 보다 커야함 -> 새로운 data 저장하는 index와 예전 data 빼려는 index가 겹치면 안된다.
	static float sumdata;					    // Data의 합산	
    static uint16_t sumCount;					// 몇개의 Data가 더해졌는지 판단 (필터 개수까지 카운트)

	static uint16_t bufIndex;					// Buffer에 저장할 index
	static uint16_t subtractBufIndex;			// 뺄셈할 값의 index

	float average = 0;						    // 평균값
    
	// Data 반영
	data[bufIndex] = input;
	sumdata += data[bufIndex++];

	// 저장 Buffer index 관리
	if(bufIndex >= 15)
		bufIndex = 0;

	// 더한 개수 판단
	if(sumCount < 10)
		sumCount++;								// 개수 미달 -> count 증가
	else
		sumdata -= data[subtractBufIndex++];	// 개수 충족 -> 이전 값 1개 제거

	// 빼는 buffer Index 관리
	if(subtractBufIndex >= 15)
		subtractBufIndex = 0;

	// 평균 계산
	average = sumdata / 10.0;

	return average;		
}


/**
 * @brief 드라이버에서 오는 입력 신호 Filter
 */
uint8_t driverFilter(uint8_t* buf, uint8_t newSignal, uint8_t oldSignal, uint16_t limitCnt)
{
	uint16_t sameSingalCnt = 0;
	uint8_t retval;

	// 이전 신호
	retval = oldSignal;
	
	// 최소 limit 개수
	if(limitCnt <= 2)
		limitCnt = 2;

	// Buf 재정렬
	for(int32_t i = limitCnt - 2; i >= 0; i--)
	{
		buf[i + 1] = buf[i];	

		// Check 현재 신호와 같은 값
		if(buf[i + 1] == newSignal)
			sameSingalCnt++;
	}

	// Apply 현재 signal
	buf[0] = newSignal;

	// Check 같은 신호 값이였던 개수
	if(sameSingalCnt == limitCnt - 1)
		retval = newSignal;

	return retval;
}


/**
 * @brief 모니터링 realtime, graph의 시작 10초 후 종료 timeout을 계산
 */
void monitoringTimeout(void)
{
    // Realtime monitoring (RS232)
    if (TRIG_REAL.MonitoringRS232)
    {
        TRIG_REAL.TimeOutCntRS232 += 10;
        if (TRIG_REAL.TimeOutCntRS232 >= 10000)        // 10sec 
        {
            TRIG_REAL.MonitoringRS232 = OFF;
            TRIG_REAL.TimeOutCntRS232 = 0;
        }
    }

    // Realtime monitoring (Ethernet)
    if (TRIG_REAL.MointoringEthernet)
    {
        TRIG_REAL.TimeOutCntEthernet += 10;
        if (TRIG_REAL.TimeOutCntEthernet >= 10000)     // 10sec 
        {
            TRIG_REAL.MointoringEthernet = OFF;
            TRIG_REAL.TimeOutCntEthernet = 0;

            // Monitoring flag OFF
            for(uint8_t i=0; i < RSI_NUMBER_OF_SOCKETS; i++)
                setSocketMonitoringControl(SOCKET_DESCRIPTION, i, MONITORING_REALTIME, OFF);
        }
    }

    // Graph monitoinrg (RS232)
    if (TRIG.MonitoringRS232)     
    {
        TRIG.TimeOutCntRS232 += 10;
        if (TRIG.TimeOutCntRS232 >= 10000)        // 10sec 
        {
            TRIG.MonitoringRS232 = OFF;
            TRIG.TimeOutCntRS232 = 0;
        }
    }

    // Graph monitoinrg (Ethernet)
    if (TRIG.MointoringEthernet)
    {
        TRIG.TimeOutCntEthernet += 10;
        if (TRIG.TimeOutCntEthernet >= 10000)     // 10sec 
        {
            TRIG.MointoringEthernet = OFF;
            TRIG.TimeOutCntEthernet = 0;

            // Monitoring flag OFF
            for(uint8_t i=0; i < RSI_NUMBER_OF_SOCKETS; i++)
                setSocketMonitoringControl(SOCKET_DESCRIPTION, i, MONITORING_GRAPH, OFF);
        }
    }
}


//=============================================================================

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
