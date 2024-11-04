/**
  ******************************************************************************
  * File Name          : backup.c
  * Description        : tdc program body
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "main_bg.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "variable.h"
#include "stm32469i_discovery_qspi.h"
//#include "modbus.h"
#include "backup.h"
#include "torque.h"
#include "advancedPlus.h"
#include "eventGraphTask.h"
#include "eventRealtimeTask.h"
#include "totalScrewCount.h"
#include "memoryQspiControl.h"


/* Extern variables ------------------------------------------------------------*/
extern SPI_HandleTypeDef hspi3;

parameterBackUpStruct parameterBackup;


/*

Parameter EEPROM 정의 (8Kbyte)
--------------------------------------------------------
시작주소    총 크기  기능             개수
0x0000	    8000	Parameters	    2 byte * 4000ea
0x1F40	    192	    Status	
-------------------------------------------------------

1. Parameter: 
- 시작 주소: 0x0000
- 1주소 당 2byte 
- 총 4000 개의 주소
- 8000 byte 영역 할당


2. Status
- 현재 Event 모니터링의 index 상태, 총 체결 횟수 상태 등...
- 시작 주소: 0x1F40
- 192 byte 할당



QSPI - FLASH
--------------------------------------------------------
시작주소    총 크기  기능             개수
0x0C0 0000	4K	    Barcode	        80 byte * 20 ea
0x0C0 1000	4K	    Model Barcode	80 byte * 20 ea
0x0C0 2000	4K	    Error	        16 byte * 32 ea
-------------------------------------------------------

1. Barcode
- 바코드 입력에 따른 preset 호출
- 시작 주소: 0x0C0 0000
- 바코드 1개 당 80 byte
- 총 20개 바코드
- 1600 byte 할당


2. Model Barcode
- 바코드 입력에 따른 model step number
- 시작 주소: 0x0C0 1000
- 바코드 1개 당 80 byte
- 총 20개 바코드
- 1600 byte 할당

3. Error List
- 에러 발생 이력 저장
- 시작 주소: 0x0C0 2000
- 1개 에러 당 16 byte
- 총 32 개 에러 이력
- 512 byte 영역 할당

*/


void ParameterInitial_hh(void)  
{
    // Version setting

    // Firmware version
    controllerPara.firmwareVersion[0] = 4;
    controllerPara.firmwareVersion[1] = 0;
    controllerPara.firmwareVersion[2] = 4;

    // V4.00.4 - 2024.05    속도 보상 계산 변경, 
    // V4.00.3 - 2024.04.12 Multi sequence의 Realtime graph 출력 수정, Parameter min max 전체 설정
    // V4.00.2 - 2024.03.29 모델 별 전체적인 토크 속도 변경, Deadtime 수정, 속도에 따른 각도보상 수정  
    // V4.00.1 - 2024.02.   
    // V4.00.0 - 2024.02.07 [ Gen2 ] Multi write CRC 수정
    
    // V1.01.4 - 2023.06.09 (Advanced 기능 개편) Advanced plus 기능 개편
    // V1.01.3 - 2023.04.10 QSPI 메모리 동작 안정화, Modbus 통신 주소 수정, LCD Error 표시 추가, 웹페이지 로드 수정
    // V1.01.3 - 2023.03.31 (Advnaced Plus 기능 추가)
    // V1.01.3 - 2023.03.31 Crowfoot 기능 추가, 바코드 스캐너 동작 기능 추가, Crowfoot reverse 토크 단위 문제 수정
    // V1.01.2 - 2023.03.07 (BISS, RS9116) ADC 읽는 크기 ioc 파일에서 수정, Barcode 통신 가능하도록 수정, 고해상도 그래프 Memory 없이 동작 추가, PID 제어 제한 수정,
    //                      PWM의 최대치 증가 - 효율 상승, 그래프 샘플링 시간 버그 수정, Ethernet port 범위 증가
    // V1.01.1 - 2022.06.20 Gain 적용하는 법 수정, 속도 당 각도 보상률 수정, 95% 토크에서 Ramp up speed 감속하는 기능 추가, 최대 PWM Limit 수정, Webpage 수정, D Cache 사용 추가,
    //                      Virtual preset 모드 동작 수정, Auto speed 등 저장 방법 수정, Parameter 초기화 시 torque % 파라미터 수정, Virtual preset 기능 수정, PWM 기본 출력 Pull down
    // V1.01.0 - 2022.04.25 (Graph event 추가) Virtual advanced 주소 추가, Last preset number 동작 수정, Auto customizing 추가, QSPI 메모리 라이브러리 수정, Key lock 기능 추가,
    //                      Battery level 표시 수정, 토크 값 못 읽음 방지 추가, SVPWM 계산 방법 개선
    //                      (ADC IC 추가 - 토크 계산 방법 변경)
    // V1.00.3B- 2022.02.21 Realtime event 라이브러리 추가, QSPI 메모리 라이브러리 추가, 일반 모니터링 방법 수정, Encoder 에러 수정
    // V1.00.3A- 2022.02.07 Graph 최종 토크 수정, Gain 값 수정, 토크 AD Filter 수정, LCD에 Driver 상태 값 추가, TCP Client 추가, Wifi 초기화 오류 수정
    // V1.00.3 - 2021.11.26 LC3216, LC3220 모델 추가, 낮은 토크일 때 Auto speed 수정, 토크 필터 계산 수정, WIFI 인터럽트 순위 수정, Graph overflow 수정, Over temperature 제한 70도 변경
    // V1.00.2 - 2021.10.25 3211 토크 기울기 수정, WIFI 동작 중 delay 안정화, WIFI 속도 개선, 토크 Filter 보완, 절전모드 전원 계산 보완, PWM 출력 방식 변경
    // V1.00.1 - 2021.10.14 정,역 방향 PWM 수정, No Speed 에러 처리 수정, ADC값 Over 필터링 수정, LCD에 토크값 표시 수정, Screw count 기본값 1로 변경, Error 리셋시 토크 초기화 수정
    //                      토크 Offset값 버그 수정, PCB 온도 에러 번호 변경, Under voltage 및 최대 절전모드 수정, USB 포트 연결 수정, Lock 중 드라이버 동작시 Event 생성 막음,
    //                      Preset 번호 저장 기능 추가, WI-FI 안정화, REVERSE_LOCK 동작 수정
    // V1.00.0 - 2021.06.08 start program

    ScrewCountValue = Para[COUNT_SCREW].val;
}

void ParameterInitial(void)
{
    int speed_tmp;
    int i;
    int speed_low_limit, speed_upper_limit;

    if (SystemResetValue == RESET_ALL_PARA)
    {
        if (Para[DRIVER_MODEL].val > DRV_MODEL_SIZE)		// nothing driver model setting
        {
            Para[DRIVER_MODEL].val = 1;							
        }

		Para[DRIVER_MODEL].dft = Para[DRIVER_MODEL].val;


        
        if (Para[UNIT].val == Kgfcm)            // kgfcm 0.1
            Para[UNIT].dft = Kgfcm;
        else if (Para[UNIT].val == Kgfm)
            Para[UNIT].dft = Kgfm;
        else if (Para[UNIT].val == Nm)          // Nm
            Para[UNIT].dft = Nm;
        else if (Para[UNIT].val == lbfin)       // Ibf.in
            Para[UNIT].dft = lbfin;                     
        else if (Para[UNIT].val == Ozfin)       // Ozf.in
            Para[UNIT].dft = Ozfin;                     
        else if (Para[UNIT].val == cNm)         // Nm
            Para[UNIT].dft = cNm;
        else if (Para[UNIT].val == lbfft)       // lbfft
            Para[UNIT].dft = lbfft;
        else
            Para[UNIT].dft = Kgfcm;

        Para[ZERO_DUMMY].dft = 0;

        default_ParaValueCalculation(Info_DrvModel[Para[DRIVER_MODEL].val].def_tq);

        Para[FASTEN_NO_SAVE].dft = 1;
        
        // setting 
        for(uint8_t i=0; i < PRESET_INDEX_SIZE; i++)
        {
            Para[FASTEN_SPEED1 + i * PRESET_SIZE].dft  = Converting_Torque_Speed(FASTEN_TORQUE1_H + i * PRESET_SIZE,  getParaVal(FASTEN_TORQUE1_H + i * PRESET_SIZE), 0);  // Para[11] :/ 100-2000  / 1rpm
        }

        Para[ACC_DEA_TIME].dft     = Info_DrvModel[Para[DRIVER_MODEL].val].acc_dcc;
        Para[LOOSENING_SPEED].dft  = Para[LOOSENING_SPEED].max * 0.5;    // setting 50% of max
        Para[CROWFOOT_REVERSE_SPEED].dft = Para[LOOSENING_SPEED].max * 0.5;

        // network setting은 유지..
        for(i = WIFI_SELECT; i <= AP_COUNTRY; i++)
        {
            // Network value is 0 then init default value (dft에다 val(현재값)을 안 넣으면 초기화됨)
            if(Para[ETHERNET_PORT].val == 0)
                Para[ETHERNET_PORT].val = Para[ETHERNET_PORT].dft;

            // Check network 값 초기화 되어 있는지 (초기화 되었으면 값 변경 X)
            if(!Para[STATIC_DHCP].val && !Para[IP_ADDRESS1].val && !Para[SUBNETMASK1].val && !Para[GATEWAY1].val)
                break;

            // Default 값 적용
            Para[i].dft = Para[i].val;
        }

        // maintain of network setting.
        for (i=0; i <= 16; i++)   
        {
            Para[AP_SSID_CHAR1 + i].dft = Para[AP_SSID_CHAR1 + i].val;
        }

        for (i=0; i <= 32; i++)   
        {
            Para[AP_PW_CHAR1 + i].dft   = Para[AP_PW_CHAR1 + i].val;
        }

        Para[AP_COUNTRY].dft = Para[AP_COUNTRY].val;

        speed_tmp = ramp_up_speed_setting_default();
        for (i=0; i < PRESET_INDEX_SIZE; i++)
        {
            Para[RAMP_UP_SPEED1 + (i*PRESET_SIZE)].dft = speed_tmp;
            Para[RAMP_UP_SPEED1 + (i*PRESET_SIZE)].val = speed_tmp;

            // Torque limit
            if (Para[TCAM_ACTM1 + (i*PRESET_SIZE)].dft == 0)            // torque control
                setParaDef(TORQUE_MIN_MAX1_H + (i*PRESET_SIZE), 10.0);     // 10% default
            else    // angle control
                setParaDef(TORQUE_MIN_MAX1_H + (i*PRESET_SIZE), 0);
        }
		

		// 체결 정보 저장 index 초기화
        initGraphMemIndex();
    }
    else if (SystemResetValue == RESET_UNIT)
    {
        // Advanced value init
        for(uint8_t presetNum = 1; presetNum <= PRESET_INDEX_SIZE; presetNum++)
            initAdvancedParameter(presetNum);

        // 현재 파라미터 저장
        for (i=1; i < BACKUP_SIZE-1; i++)
            Para[i].dft = Para[i].val;

        default_ParaValueCalculation(Info_DrvModel[Para[DRIVER_MODEL].val].def_tq * getCrowFootRatio());		// 0.01 단위..

        // Crow foot
        setParaDef(CROWFOOT_REVERSE_TORQUE_H, convertUnit(paraUnitOld, Para[UNIT].val, getParaVal(CROWFOOT_REVERSE_TORQUE_H)));
    }
    else if (SystemResetValue == RESET_MODEL)
    {
        // Advanced value init
        for(uint8_t presetNum = 1; presetNum <= PRESET_INDEX_SIZE; presetNum++)
            initAdvancedParameter(presetNum);


        // 현재 파라미터 저장
        for (i=1; i < BACKUP_SIZE-1; i++)
            Para[i].dft = Para[i].val;
            
        default_ParaValueCalculation(Info_DrvModel[Para[DRIVER_MODEL].val].def_tq * getCrowFootRatio());		// 0.01 단위..

        // Crowfoot Speed 범위 계산
        speed_low_limit   = Info_DrvModel[Para[DRIVER_MODEL].val].speed_min / getCrowFootRatio();
        speed_upper_limit = Info_DrvModel[Para[DRIVER_MODEL].val].speed_max / getCrowFootRatio();

        // 목표 Speed calc
        speed_tmp = Converting_Torque_Speed(FASTEN_TORQUE1_H, getParaVal(FASTEN_TORQUE1_H) / getCrowFootRatio(), 0);
        if(speed_tmp > speed_upper_limit)
            speed_tmp = speed_upper_limit;
        else if(speed_tmp < speed_low_limit)
            speed_tmp = speed_low_limit;

        // 목표 속도 반영
        for(i = 0; i < PRESET_INDEX_SIZE; i++)
            Para[FASTEN_SPEED1 + i*PRESET_SIZE].dft = speed_tmp;       

        Para[ACC_DEA_TIME].dft     = Info_DrvModel[Para[DRIVER_MODEL].val].acc_dcc;
        Para[LOOSENING_SPEED].dft  = Para[LOOSENING_SPEED].max * 0.5;    // setting 50% of max

        // Crow foot
        setParaDef(CROWFOOT_REVERSE_TORQUE_H, 0);
        Para[CROWFOOT_REVERSE_SPEED].dft  = Para[LOOSENING_SPEED].dft * 0.5;

        speed_tmp = ramp_up_speed_setting_default();
        for (i=0; i < PRESET_INDEX_SIZE; i++)
        {
            Para[RAMP_UP_SPEED1 + (i*PRESET_SIZE)].dft = speed_tmp;
            Para[RAMP_UP_SPEED1 + (i*PRESET_SIZE)].val = speed_tmp; 

            if (Para[TCAM_ACTM1 + (i*PRESET_SIZE)].val == 0)            // torque control
                setParaDef(TORQUE_MIN_MAX1_H + (i*PRESET_SIZE), 10.0);     // 10% default
            else    // angle control
                setParaDef(TORQUE_MIN_MAX1_H + (i*PRESET_SIZE), 0);
        }
    }
    else if(SystemResetValue == RESET_CROWFOOT)
    {
        // Advanced 초기화
        for(uint8_t presetNum = 1; presetNum <= PRESET_INDEX_SIZE; presetNum++)
            initAdvancedParameter(presetNum);

        // Free speed 초기화
        for(i = FREE_FASTEN_SPEED1; i < FREE_FASTEN_SPEED1 + PRESET_INDEX_SIZE * PRESET_SIZE; i+= PRESET_SIZE)
            Para[i].val = 0;

        // 현재 파라미터 저장
        for (i=1; i < BACKUP_SIZE-1; i++)     // all copy
            Para[i].dft = Para[i].val;

        // 기어비 초기화
        deceleration_ratio = Info_DrvModel[Para[DRIVER_MODEL].val].gear_ratio * getCrowFootRatio();

        // Crowfoot Speed 범위 계산
        speed_low_limit   = Info_DrvModel[Para[DRIVER_MODEL].val].speed_min / getCrowFootRatio();
        speed_upper_limit = Info_DrvModel[Para[DRIVER_MODEL].val].speed_max / getCrowFootRatio();
        
        // 목표 Torque 초기화
        default_ParaValueCalculation(Info_DrvModel[Para[DRIVER_MODEL].val].def_tq * getCrowFootRatio());		// 0.01 단위..
        
        // 목표 Speed calc
        speed_tmp = Converting_Torque_Speed(FASTEN_TORQUE1_H, getParaVal(FASTEN_TORQUE1_H) / getCrowFootRatio(), 0);
        if(speed_tmp > speed_upper_limit)
            speed_tmp = speed_upper_limit;
        else if(speed_tmp < speed_low_limit)
            speed_tmp = speed_low_limit;

        // 목표 속도 반영
        for(i = 0; i < PRESET_INDEX_SIZE; i++)
            Para[FASTEN_SPEED1 + i*PRESET_SIZE].dft = speed_tmp;       

        // Loosening speed
        if(Para[LOOSENING_SPEED].max > speed_upper_limit || Para[LOOSENING_SPEED].max < speed_low_limit)
            Para[LOOSENING_SPEED].max = (uint16_t)speed_upper_limit;

        // Controller setting 
        Para[LOOSENING_SPEED].dft  = Para[LOOSENING_SPEED].max * 0.5;   // setting 50% of max
        Para[AUTO_SELECT_SPEED].dft = OFF;                              // Auto speed 종료
        Para[CROWFOOT_REVERSE_SPEED].dft = Para[LOOSENING_SPEED].dft * 0.5;

        // Ramp up speed
        speed_tmp = ramp_up_speed_setting_default();
        if(speed_tmp > speed_upper_limit)
            speed_tmp = (int)(speed_upper_limit * 0.4);
        else if(speed_tmp < speed_low_limit)
            speed_tmp = (int)(speed_low_limit * 1.2);

        for (i=0; i < PRESET_INDEX_SIZE; i++)
        {
            Para[RAMP_UP_SPEED1 + (i*PRESET_SIZE)].dft = speed_tmp;
            Para[RAMP_UP_SPEED1 + (i*PRESET_SIZE)].val = speed_tmp; 

            // Min, Max torque limit
            if (Para[TCAM_ACTM1 + (i*PRESET_SIZE)].val == 0)
                setParaDef(TORQUE_MIN_MAX1_H + (i*PRESET_SIZE), 10.0);     // torque control 10% default
            else
                setParaDef(TORQUE_MIN_MAX1_H + (i*PRESET_SIZE), 0);        // angle control
        }
    }


    // 파라미터의 Revision에 따라 메모리에 저장된 값을 mapping된 위치로 이동시켜준다
    paraRevisionMapping();
}

void Init_Para_limit(void)
{
    int i;
    float tq_low_limit_unit, tq_upper_limit_unit;
    int speed_low_limit, speed_upper_limit;
    uint16_t rampUpSpeed;

    // Max, Min Torque 설정
    tq_low_limit_unit   = Info_DrvModel[Para[DRIVER_MODEL].val].tq_min * TorqueMetric[UNIT_Nm][Para[UNIT].val] * getCrowFootRatio();    // N.m 단위
    tq_upper_limit_unit = Info_DrvModel[Para[DRIVER_MODEL].val].tq_max * TorqueMetric[UNIT_Nm][Para[UNIT].val] * getCrowFootRatio();    // N.m 단위

    // Max, Min speed 설정 (Crow foot일 때)
    speed_low_limit   = Info_DrvModel[Para[DRIVER_MODEL].val].speed_min / getCrowFootRatio();
    speed_upper_limit = Info_DrvModel[Para[DRIVER_MODEL].val].speed_max / getCrowFootRatio();
    rampUpSpeed = (uint16_t)(ramp_up_speed_setting_min() / getCrowFootRatio());


    // 18V 배터리 사용하면 토크, 스피드 3/4 으로 감소 시킨다.
    if (BatteryType == BATTERY_18V)     
    {
        tq_upper_limit_unit = tq_upper_limit_unit * 0.75;
        speed_upper_limit = speed_upper_limit * 0.73;

        // Max speed 5700rpm at 18v battery
        if(speed_upper_limit * deceleration_ratio > 5700)
            speed_upper_limit = 5700 * inv_deceleration_ratio;
    }


    // Preset 별로 값 반영
    for (i=1; i < PRESET_INDEX_SIZE + 1; i++)
    {
        // change to para unit
        setParaMin(FASTEN_TORQUE1_H + (i-1)*PRESET_SIZE, tq_low_limit_unit);            
        setParaMax(FASTEN_TORQUE1_H + (i-1)*PRESET_SIZE, tq_upper_limit_unit);
        setParaMin(SNUG_TORQUE1_H + (i-1)*PRESET_SIZE, 0);           
        setParaMax(SNUG_TORQUE1_H + (i-1)*PRESET_SIZE, tq_upper_limit_unit);

        setParaMin(FREE_SPEED_MAX_TORQUE_H, 0);           
        setParaMax(FREE_SPEED_MAX_TORQUE_H, tq_upper_limit_unit);

        Para[FASTEN_SPEED1 + ((i-1)*PRESET_SIZE)].min = speed_low_limit;            
        Para[FASTEN_SPEED1 + ((i-1)*PRESET_SIZE)].max = speed_upper_limit;

        Para[FREE_FASTEN_SPEED1 + ((i-1)*PRESET_SIZE)].min = 0;
        Para[FREE_FASTEN_SPEED1 + ((i-1)*PRESET_SIZE)].max = speed_upper_limit;

        Para[RAMP_UP_SPEED1 + ((i-1)*PRESET_SIZE)].min = rampUpSpeed;
        Para[RAMP_UP_SPEED1 + ((i-1)*PRESET_SIZE)].max = (uint16_t)(speed_upper_limit * 0.8);   // 80%


        // Back up torque min, max value
        setRange.minTorqueUnitConvert = getParaMin(FASTEN_TORQUE1_H + ((i-1)*PRESET_SIZE));
        setRange.maxTorqueUnitConvert = getParaMax(FASTEN_TORQUE1_H + ((i-1)*PRESET_SIZE));

        // TCAM , ACTM
        if (Para[TCAM_ACTM1 + ((i-1)*PRESET_SIZE)].val == 0)
        {
            // Torque control
            setParaMin(TORQUE_MIN_MAX1_H + ((i-1)*PRESET_SIZE), 0);
            setParaMax(TORQUE_MIN_MAX1_H + ((i-1)*PRESET_SIZE), 100.0);    // 100.0 %
        }
        else    
        {
            // Angle control
            setParaMin(FASTEN_TORQUE1_H + ((i-1)*PRESET_SIZE), 0);      // Max torque limit

            setParaMin(TORQUE_MIN_MAX1_H + ((i-1)*PRESET_SIZE), 0);
            setParaMax(TORQUE_MIN_MAX1_H + ((i-1)*PRESET_SIZE), getParaMax(FASTEN_TORQUE1_H + ((i-1)*PRESET_SIZE)));
        }

        // Advanced 파라미터 Min, Max 설정
        Init_Para_limit_forAdvence(i, tq_upper_limit_unit, speed_upper_limit);
    }

    setParaMax(CROWFOOT_REVERSE_TORQUE_H, getParaVal(FASTEN_TORQUE1_H));
    Para[CROWFOOT_REVERSE_SPEED].max = Para[FASTEN_SPEED1].max;

    Para[LOOSENING_SPEED].min = speed_low_limit;
    Para[LOOSENING_SPEED].max = speed_upper_limit;
}


uint16_t ramp_up_speed_setting_min(void)
{
    uint16_t speed_tmp;
    float ratio;

    ratio = Info_DrvModel[Para[DRIVER_MODEL].val].gear_ratio;

    // 각 모델별 ramp up speed setting
    if (ratio == 1.96)     speed_tmp = 100; // 1.96
    else if (ratio == 4.0) speed_tmp = 70;  // 4
    else if (ratio == 6.0)   speed_tmp = 60;    // 6
    else if ((ratio > 6.5) && (ratio <= 12.0)) speed_tmp = 50; // 11
    else if ((ratio > 12)  && (ratio <= 17.0)) speed_tmp = 40; // 16
    else if ((ratio > 17)  && (ratio <= 25.0)) speed_tmp = 40; // 24
    else if ((ratio > 25)  && (ratio <= 40.0)) speed_tmp = 30; // 36
    else if ((ratio > 40)  && (ratio < 55.0))  speed_tmp = 25; // 45
    else if ((ratio > 55)  && (ratio < 65.0))  speed_tmp = 20; // 64
    else if (ratio > 65.0)  speed_tmp = 20; // 80
    else speed_tmp = 200; // min value 200rpm

    return speed_tmp;
}

uint16_t ramp_up_speed_setting_default(void)
{
    uint16_t speed_tmp;
    float ratio;

    ratio = Info_DrvModel[Para[DRIVER_MODEL].val].gear_ratio * getCrowFootRatio();

    // 각 모델별 ramp up speed setting
    if (ratio == 1.96)       speed_tmp = 300;    // 1.96
    else if (ratio == 4.0)   speed_tmp = 300;    // 4
    else if (ratio == 6.0)   speed_tmp = 250;    // 6
    else if ((ratio > 6.5) && (ratio <= 12.0)) speed_tmp = 150; // 11
    else if ((ratio > 12)  && (ratio <= 17.0)) speed_tmp = 120; // 16
    else if ((ratio > 17)  && (ratio <= 25.0)) speed_tmp = 110; // 24
    else if ((ratio > 25)  && (ratio <= 40.0)) speed_tmp = 100; // 36
    else if ((ratio > 40)  && (ratio < 55.0))  speed_tmp = 80;  // 45
    else if ((ratio > 55)  && (ratio < 65.0))  speed_tmp = 60;  // 64
    else if (ratio > 65.0)  speed_tmp = 50; // 80
    else    speed_tmp = 500;        // etc about 500rpm을 기준으로 설정함.. 이후 setting 필요..

    return speed_tmp;
}

void default_ParaValueCalculation(float data)		// dd 는 Nm 기준으로 작성.. 0.01 단위..
{
	int i;

	switch(Para[UNIT].val)
	{
		case Kgfcm:			// kgfcm   0.01
			for (i=0; i < PRESET_INDEX_SIZE; i++)
			{
				setParaDef(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 10);	
				setParaVal(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 10);	
			}
		break;
		case Kgfm:			// Kgfm		 0.01 
			for (i=0; i < PRESET_INDEX_SIZE; i++)
			{
				setParaDef(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 0.1);	
				setParaVal(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 0.1);	
			}
		break;
		case Nm:			// Nm		 0.01 
			for (i=0; i < PRESET_INDEX_SIZE; i++)
			{
				setParaDef(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data);	
				setParaVal(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data);	
			}
		break;
		case cNm:			// cNm		 0.01 
			for (i=0; i < PRESET_INDEX_SIZE; i++)
			{
				setParaDef(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 100);	
				setParaVal(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 100);	
			}
		break;
		case lbfin:			// lbf.in 0.01
			for (i=0; i < PRESET_INDEX_SIZE; i++)
			{
				setParaDef(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 8.6);	
				setParaVal(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 8.6);	
			}
		break;
		case Ozfin:		    // ozf.in		 0.01
			for (i=0; i < PRESET_INDEX_SIZE; i++)
			{
				setParaDef(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 140);	
				setParaVal(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 140);	
			}
		break;
		case lbfft:		    // lbfft		0.01
			for (i=0; i < PRESET_INDEX_SIZE; i++)
			{
				setParaDef(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 0.7);	
				setParaVal(FASTEN_TORQUE1_H + (i*PRESET_SIZE), data * 0.7);	
			}
		break;
	}

	for (i=0; i < PRESET_INDEX_SIZE; i++)
	{
		Para[FASTEN_SEATTING_POINT_RATE1 + (i*PRESET_SIZE)].dft = Info_DrvModel[Para[DRIVER_MODEL].val].seating_point;
	}
}


void Save_Converting_Torque_Speed(void)
{
    floatFrom16 convert;
    uint16_t saveAddrBuf[16] = {0, };
    uint16_t saveValBuf[16] = {0, };
    uint8_t  saveLength = 0;
    uint16_t address;
    uint16_t data = 0;
    uint16_t dataOld;
    uint8_t  index = 0;


    // 파라미터 값 변경
    dataOld = Para[parameterBackup.setAddress].val;

    // 현재 UNIT 저장
    if (parameterBackup.setAddress == UNIT)
        paraUnitOld = Para[UNIT].val;

    // Torque change
    if((parameterBackup.setAddress % PRESET_SIZE) == FASTEN_TORQUE1_H && parameterBackup.setAddress <= FASTEN_TORQUE1_H + (PRESET_INDEX_SIZE - 1) * PRESET_SIZE)
    {
        // Auto speed
        if (Para[AUTO_SELECT_SPEED].val == 1)
        {
            index = parameterBackup.setAddress / PRESET_SIZE;
            Para[parameterBackup.setAddress].val = parameterBackup.setData;
            data = Converting_Torque_Speed(parameterBackup.setAddress, getParaVal(parameterBackup.setAddress), 0);

            // Torque, Speed (Auto speed)
            ParameterSaveWaitBuf(parameterBackup.setAddress, parameterBackup.setData, saveLength++, saveAddrBuf, saveValBuf);               // Torque
            ParameterSaveWaitBuf(FASTEN_SPEED1 + index * PRESET_SIZE, data, saveLength++, saveAddrBuf, saveValBuf);                         // Speed
        }
        else
        {
            ParameterSaveWaitBuf(parameterBackup.setAddress, parameterBackup.setData, saveLength++, saveAddrBuf, saveValBuf);
        }
    }

    // TCAM , ACTM 모드 변경
    if((parameterBackup.setAddress % PRESET_SIZE) == TCAM_ACTM1 && parameterBackup.setAddress <= TCAM_ACTM1 + (PRESET_INDEX_SIZE - 1) * PRESET_SIZE)
    {
        // Init min, max
        Init_Para_limit();  

        // 모드 변경됨
        if(dataOld != parameterBackup.setData)
        {
            index = parameterBackup.setAddress / PRESET_SIZE;
            
            // 모드 확인
            if (parameterBackup.setData == 0)
            {
                // TCAM - Torque limit 10%
                convert.fVal = 10.0;
                ParameterSaveWaitBuf(TORQUE_MIN_MAX1_H + index * PRESET_SIZE, convert.raw.integer_high, saveLength++, saveAddrBuf, saveValBuf); 
                ParameterSaveWaitBuf(TORQUE_MIN_MAX1_L + index * PRESET_SIZE, convert.raw.integer_low, saveLength++, saveAddrBuf, saveValBuf); 

                // Target torque 0일 때 default로 복구
                if(getParaVal(FASTEN_TORQUE1_H + index * PRESET_SIZE) < setRange.minTorqueUnitConvert)
                {
                    // Default torque
                    convert.fVal = convertUnit(Nm, Para[UNIT].val, Info_DrvModel[Para[DRIVER_MODEL].val].def_tq);
                    ParameterSaveWaitBuf(FASTEN_TORQUE1_H + index * PRESET_SIZE, convert.raw.integer_high, saveLength++, saveAddrBuf, saveValBuf);
                    ParameterSaveWaitBuf(FASTEN_TORQUE1_L + index * PRESET_SIZE, convert.raw.integer_low, saveLength++, saveAddrBuf, saveValBuf);
                }
            }
            else
            {
                // ACTM - Torque min limit
                ParameterSaveWaitBuf(TORQUE_MIN_MAX1_H + index * PRESET_SIZE, 0, saveLength++, saveAddrBuf, saveValBuf);
                ParameterSaveWaitBuf(TORQUE_MIN_MAX1_L + index * PRESET_SIZE, 0, saveLength++, saveAddrBuf, saveValBuf);
            }
        }            
    }


    // Check parameter don't save
    Para[parameterBackup.setAddress].val = parameterBackup.setData;
    ParameterSaveWaitBuf(parameterBackup.setAddress, parameterBackup.setData, saveLength++, saveAddrBuf, saveValBuf);


    // Apply parameter
    for(uint8_t i=0; i < saveLength; i++)
    {
        address = saveAddrBuf[i];
        Para[address].val = saveValBuf[i];
    }


    // Save parameter
    for(uint8_t i=0; i < saveLength; i++)
    {
        if(FlagFasten.NoSavePara == OFF)
            ParameterWriteToMemory(saveAddrBuf[i], saveValBuf[i]);
    }


    // Advanced의 mode 변경 시 파라미터 min, max, val 초기화 실행
    setAdvancedFromModeChange(parameterBackup.setAddress, parameterBackup.setData, dataOld);
    
    // Set fastening data
    Set_Torque_variable();

    // 저장관련 설정 초기화
    FlagFasten.NoSavePara = OFF;
}


void SPI_WritePara(uint16_t paraNo, uint16_t data)
{
    uint8_t wdata[5];

    // write enable
    wdata[0] = WREN;
    wdata[1] = 0;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi3, wdata, 1, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_SET);

    // write op code
    wdata[0] = WRITE;
    wdata[1] = 0;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi3, wdata, 1, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    // write address
    wdata[0] = (paraNo * 2) >> 8;
    wdata[1] = (paraNo * 2) & 0xff;
    if (HAL_SPI_Transmit(&hspi3, wdata, 2, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    // write data
    wdata[0] = data >> 8;
    wdata[1] = data & 0xff;
    if (HAL_SPI_Transmit(&hspi3, wdata, 2, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_SET);
}


uint16_t SPI_ReadPara(uint16_t paraNo)
{
    uint8_t read_data[2];
    uint8_t wdata[5];
    uint16_t data;
    uint16_t pNo;

    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_RESET);

    // write op code
    wdata[0] = READ;
    wdata[1] = 0;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi3, wdata, 1, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    // write address
    pNo = paraNo * 2;
    wdata[0] = pNo >> 8;
    wdata[1] = pNo & 0xff;
    if (HAL_SPI_Transmit(&hspi3, wdata, 2, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    if (HAL_SPI_Receive(&hspi3, read_data, 2, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_SET);

    data = (read_data[0] << 8) + read_data[1];
    return data;
}


void SPI_WriteMemoryData(uint16_t address, uint8_t* buf, uint16_t length)
{
    uint8_t wdata[5];

    // write enable
    wdata[0] = WREN;
    wdata[1] = 0;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi3, wdata, 1, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_SET);

    // write op code
    wdata[0] = WRITE;
    wdata[1] = 0;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi3, wdata, 1, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    // write address
    wdata[0] = address >> 8;
    wdata[1] = address & 0xff;
    if (HAL_SPI_Transmit(&hspi3, wdata, 2, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;
        
    // write data
    if (HAL_SPI_Transmit(&hspi3, buf, length, 500) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_SET);
}


void SPI_ReadMemoryData(uint16_t address, uint8_t* buf, uint16_t length)
{
    uint8_t wdata[5];

    // write op code
    wdata[0] = READ;
    wdata[1] = 0;
    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi3, wdata, 1, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    // write address
    wdata[0] = address >> 8;
    wdata[1] = address & 0xff;
    if (HAL_SPI_Transmit(&hspi3, wdata, 2, 200) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    if (HAL_SPI_Receive(&hspi3, buf, length, 500) != HAL_OK)
        system_err = PARAMETER_READ_WRITE;

    HAL_GPIO_WritePin(EEP_CS1_GPIO_Port, EEP_CS1_Pin, GPIO_PIN_SET);
}


int ParameterReadFromMemory(int parano)
{
    int i = 0;

    if(parano < 0)
    {
        // Read All parameter
        for(i=0; i < PARA_SIZE; i++)
        {
            // Parameter Read From memory
            Para[i].val = SPI_ReadPara(i);             
        }

        if(PARAMETER_REVISION != Para[ZERO_DUMMY].val)
        {
            if(system_err == 0)
                system_err = PARA_CHECKSUM_ERROR;

            return 0;
        }
    }
    else
    {
        // Read single parameter
        Para[parano].val = SPI_ReadPara(parano);
    }

    return 1;
}


void ParameterWriteToMemory(int setno, uint16_t data)
{
    int i=0;

    if((setno >= 0) && (setno <= PARA_SIZE))
        Para[setno].val = data;

    if(setno < 0)   // parameter all save..
    {
        ParameterInitial();
        
        Para[ZERO_DUMMY].val = PARAMETER_REVISION;
        SPI_WritePara(ZERO_DUMMY, Para[ZERO_DUMMY].val);

        for(i=1; i < PARA_SIZE; i++)
        {
            SPI_WritePara(i, Para[i].dft);
            Para[i].val = Para[i].dft;
        }
    }
    else
    {
        SPI_WritePara(setno, Para[setno].val);
    }
}


/**
 * @brief 파라미터를 저장할 때 buffer에 모았다가 한 번에 저장하고자 사용
 * 
 * @param address : 파라미터 주소
 * @param value   : 파라미터 값
 * @param index   : 저장되는 buffer의 인덱스
 * @param addrBuf : 주소 buffer
 * @param valBuf  : 값 buffer
 */
void ParameterSaveWaitBuf(uint16_t address, uint16_t value, uint8_t index, uint16_t* addrBuf, uint16_t* valBuf)
{
    addrBuf[index] = address;
    valBuf[index] = value;
}





/* Save parameter ---------------------------------------------------------------------------*/

void SendSaveParameter(int pnum, uint16_t pvalue)
{
    parameterBackUpStruct tempBackup = {0, };

	Flag.SaveParameter = ON; 

    // List 재정렬
    memcpy(&tempBackup.list[1], &parameterBackup.list[0], sizeof(parameterBackup.list[0]) * (PARAMETER_BACKUP_LIST_SIZE - 1));
    memcpy(&parameterBackup.list[1], &tempBackup.list[1], sizeof(parameterBackup.list[0]) * (PARAMETER_BACKUP_LIST_SIZE - 1));

    parameterBackup.list[0].address = pnum;
    parameterBackup.list[0].data = pvalue;
}



// 	Save Parameter data
void mbSavePara(uint16_t data, uint16_t addr)
{
	uint16_t p_num, p_value;

	p_value = data;
	p_num   = addr;

    if(p_num % PRESET_SIZE == FASTEN_SPEED1 && p_num >= FASTEN_SPEED1 && p_num <= FASTEN_SPEED1 + (PRESET_INDEX_SIZE - 1) * PRESET_SIZE)
    {
		if (!Para[AUTO_SELECT_SPEED].val)	// manual change. if auto no change
		{
			SendSaveParameter(p_num, p_value);
		}
	}
    else if(p_num % PRESET_SIZE == RAMP_UP_SPEED1 && p_num >= RAMP_UP_SPEED1 && p_num <= RAMP_UP_SPEED1 + (PRESET_INDEX_SIZE - 1) * PRESET_SIZE)
	{
		SendSaveParameter(p_num, p_value);
	}
	else if ((p_num == PARA_INIT) && (p_value == 77))	// parameter init
	{
		SystemResetValue    = RESET_ALL_PARA;
		Flag.ParaInitialize = ON;
	}
	else if ((p_num == PARA_INIT) && (p_value == 7799))	// Memory data index init
	{
		initGraphMemIndex();		// INIT Graph index
	}
	else if (p_num == TORQUE_COMPENSATION_MAIN)	// save driver and controller
	{
		dset[9] = p_value;
		dParameterValue[9] = p_value;
		DriverSaveParaNumber = 9;
		Flag.DriverSaveParaData = ON;

		SendSaveParameter(p_num, p_value);
	}				
	else
	{
		SendSaveParameter(p_num, p_value);
	}
}



/**
 * @brief  Function decide to make Real-time monitoring data (Auto data, SD-Card도 영향)
 * @retval Make ON or OFF
 * 
 * */
uint8_t selectEventType(void)
{
	uint8_t retval;

	// if select data to save..
	if(((Para[SELECT_DATA_TO_SAVE].val >> StatusOutput) & 0x01) == 1)
		retval = ON;
	else
		retval = OFF;

	return retval;
}


// Parameter 변경으로 기존 setting 재설정
void ParaChangeResetting(void)
{
    // Screw resetting
    if (parameterBackup.setAddress == COUNT_SCREW)
        ScrewCountValue = parameterBackup.setData;

    // unit/DRIVER_MODEL를 변경할 경우에는 모든 파라메타를 초기화 한다.
    if ((parameterBackup.setAddress == UNIT) || (parameterBackup.setAddress == DRIVER_MODEL))
    {
        if (parameterBackup.setAddress == UNIT)
        {
            SystemResetValue = RESET_UNIT;
            Para[UNIT].val = parameterBackup.setData;
        }
        if (parameterBackup.setAddress == DRIVER_MODEL)
        {
            SystemResetValue = RESET_MODEL;
            Para[DRIVER_MODEL].val = parameterBackup.setData;
        }

        Flag.ParaInitialize = ON;
    }

    // Crowfoot - 파라미터 초기화
    if(Para[CROWFOOT_ENABLE].val && (parameterBackup.setAddress == CROWFOOT_ENABLE || parameterBackup.setAddress == CROWFOOT_RATIO_L))
    {
        SystemResetValue = RESET_CROWFOOT;
        Flag.ParaInitialize = ON;
    }
    else if(Para[CROWFOOT_ENABLE].val == OFF && parameterBackup.setAddress == CROWFOOT_ENABLE)
    {
        // Crowfoot 해제되었을 때 초기화
        SystemResetValue = RESET_MODEL;
        Flag.ParaInitialize = ON;
    }

    if (parameterBackup.setAddress == LCD_KEY_LOCK)
        current_mode = _OPERATION;
}


/**
 * @brief Unit 단위에 맞게 값을 변환한다.
 * 
 * @param currentUnit : 현재 단위
 * @param changeUnit  : 변경할 단위
 * @param value       : 변경할 값
 * 
 * */
float convertUnit(uint8_t currentUnit, uint8_t changeUnit, float value)
{
    float tempData;

    // Change to N.m
    switch(currentUnit)
    {
        case Kgfcm:
            tempData = value *  Nm_CONVERTER_Kgfcm;
        break;
        case Kgfm:
            tempData = value *  Nm_CONVERTER_Kgfm;
        break;
        case Nm:
            tempData = value;
        break;
        case cNm:
            tempData = value *  Nm_CONVERTER_cNm;
        break;
        case lbfin:
            tempData = value *  Nm_CONVERTER_lbfin;
        break;
        case Ozfin:
            tempData = value *  Nm_CONVERTER_ozfin;
        break;
        case lbfft:
            tempData = value *  Nm_CONVERTER_lbfft;
        break;
        default:
            tempData = 0;
        break;
    }

    // N.m Change to changeUnit
    switch(changeUnit)
    {
        case Kgfcm:
            tempData = tempData *  CONVERTER_Kgfcm_Nm;
        break;
        case Kgfm:
            tempData = tempData *  CONVERTER_Kgfm_Nm;
        break;
        case Nm:
            tempData = tempData;
        break;
        case cNm:
            tempData = tempData *  CONVERTER_cNm_Nm;
        break;
        case lbfin:
            tempData = tempData *  CONVERTER_lbfin_Nm;
        break;
        case Ozfin:
            tempData = tempData *  CONVERTER_ozfin_Nm;
        break;
        case lbfft:
            tempData = tempData *  CONVERTER_lbfft_Nm;
        break;
        default:
            tempData = 0;
        break;
    }

    return tempData;
}


/**
 * @brief Parameter의 min, max를 체크하고 범위 밖에 값을 default로 변환한다.
 * @note  1. Fastening 관련 값은 제외한다.
 *        2. Hardfault 발생할 수 있는 상황을 제거한다.
 * */
void paraMinMaxCheck(void)
{
    // Check UNIT 
    if(Para[UNIT].val > Para[UNIT].max || Para[UNIT].val < Para[UNIT].min)
        Para[UNIT].val = Para[UNIT].dft;

    // Check DRIVER ID
    if(Para[DRIVER_ID].val == 0 || Para[DRIVER_ID].val > Para[DRIVER_ID].max)
        Para[DRIVER_ID].val = 1;

    // Check DRIVER MODEL
    if(Para[DRIVER_MODEL].val > DRV_MODEL_SIZE)
        Para[DRIVER_MODEL].val = 1;

    // Check Revision
    if(Para[ZERO_DUMMY].val == PARAMETER_REVISION_0 || Para[ZERO_DUMMY].val == 0 || Para[ZERO_DUMMY].val == 0xFFFF)
    {
        Para[DRIVER_ID].val = 1;
        Para[SELECT_BAUDRATE].val = Para[SELECT_BAUDRATE].dft;
        Para[DRIVER_MODEL].val = 0;
        Para[PROTOCOL].val = 0;
    }
}




/**
 * @brief 파라미터의 버전이 다를 때 기존에 메모리에 저장되어 있던 값을 다른 곳을 저장한다
 *        파라미터를 초기화 할 때 실행
 * 
 * @note
 * 1. 총 체결 횟수
 * 2. 고해상도 모니터링 데이터 index
 * 3. Controller 파라미터 (시리얼 번호, 생산 날짜)
 */
void paraRevisionMapping(void)
{
    uint8_t buf[256] = {0, };

    // 현재 파라미터가 overflow가 발생한 것을 강제로 수정해준다. (펌웨어 업데이트에 따라 주소가 달라져서 이상한 값이 설정되어 있을 수 있음)

    // Check parameter revision
    if(Para[ZERO_DUMMY].val == PARAMETER_REVISION_0 && PARAMETER_REVISION == PARAMETER_REVISION_2)
    {
        // Read 기존의 파라미터
        uint32_t totalScrew, resetScrew, limitScrew;

        // Read
        totalScrew = ((uint32_t)SPI_ReadPara(TOTAL_SCREW_ADDR_REVISION_0 + 1) << 16) + (uint32_t)SPI_ReadPara(TOTAL_SCREW_ADDR_REVISION_0);
        resetScrew = ((uint32_t)SPI_ReadPara(RESET_SCREW_ADDR_REVISION_0 + 1) << 16) + (uint32_t)SPI_ReadPara(RESET_SCREW_ADDR_REVISION_0);
        limitScrew = ((uint32_t)SPI_ReadPara(LIMIT_SCREW_ADDR_REVISION_0 + 1) << 16) + (uint32_t)SPI_ReadPara(LIMIT_SCREW_ADDR_REVISION_0);


        // 파라미터 전체를 한번 초기화 - 잘못된 값이 들어가 있어 이상한 출력이 발생하는 것 방지
        for(uint8_t i=0; i < 32; i++)
        {
            SPI_WriteMemoryData(256 * i, buf, 256);     // 8Kbyte
            HAL_Delay(30);
        }

        
        // Total screw
        if(resetScrew)
            setTotalScrewCount(TOTAL_SCREW_ALARM_ACTIVE_ADDRESS, ON);
        else
            setTotalScrewCount(TOTAL_SCREW_ALARM_ACTIVE_ADDRESS, OFF);

        SPI_WriteMemoryData(TOTAL_SCREW_ADDRESS, (uint8_t*)&totalScrew, 4);
        SPI_WriteMemoryData(RESET_SCREW_ADDRESS, (uint8_t*)&resetScrew, 4);
        SPI_WriteMemoryData(LIMIT_SCREW_ADDRESS, (uint8_t*)&limitScrew, 4);


        // 고해상도 모니터링
        initGraphMemIndex();

        // Controller parameter

        // Mac address (MDC)

        // Network 초기화
        for(uint8_t i=0; i <= 16; i++)
        {
            Para[AP_SSID_CHAR1 + i].dft = 0;
            Para[AP_PW_CHAR1 + i].dft   = 0;
        }

        for(uint8_t i=0; i < 4; i++)
        {
            Para[TARGET_IP1 + i].dft = 0;
            Para[IP_ADDRESS1 + i].dft = 0;
            Para[SUBNETMASK1 + i].dft = 0;
            Para[GATEWAY1 + i].dft = 0;
        }

        Para[WIFI_SELECT].dft = 0;
        Para[STATIC_DHCP].dft = 0;
        Para[ETHERNET_PORT].dft = 5000;
        Para[TARGET_PORT].dft = 5000;


        // Init QSPI Memory
        BSP_QSPI_Erase_Chip();
    }
}



/* Crowfoot -------------------------------------------------------------------------------------------------*/

/**
 * @brief Get Crow Foot의 Ratio
 * 
 * @return float: 사용 안 할때 = 1.0, 사용중 = Ratio(기어비)
 */
float getCrowFootRatio(void)
{
    float retval = 1.0;

    // Check crowfoot enable
    if(Para[CROWFOOT_ENABLE].val && getParaVal(CROWFOOT_RATIO_H))
        retval = getParaVal(CROWFOOT_RATIO_H);

    return retval;
}

