
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _COMMON_H
#define _COMMON_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stdio.h"
#include "string.h"

#include "parameter.h"
#include "errorcodeParameter.h"
#include "controllerParameter.h"



/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE       128

// 232 ASCII
#define STX			0x02	/* Start of Text */
#define ETX			0x03	/* End of Text */
#define ACK			0x06	/* Acknowlege */
#define	BEL			0x07	/* Acknowlege alarm */
#define NAK			0x15	/* Not Acknowlege */
#define ETB			0x17	/* End of Block */
#define	RS			0x1E	/* Range Over */
#define	US			0x1F	/* Undefine */

//#define OK 	1
//#define NG	0
#define     CHK_STATUS_ADDR        ((uint32_t)0x08018000)

/* Exported macro ------------------------------------------------------------*/
/* Common routines */
#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))

#define ON      1
#define OFF     0

#define nON      0
#define nOFF     1

#define TRUE	1
#define FALSE	0

#define POSITIVE	1
#define NEGATIVE    -1

#define PEAK		1
#define TRACE 		2

#define fABS(x)		  ((x>=0.) ? x :(-x))
//#define labs(x)     ((x>0)  ? x :(-x))
//#define Labs(x)     ((x>0)  ? x :(-x))

#define BATTERY_18V     1
#define BATTERY_24V     2
#define BATTERY_36V     3




// define for MCP23S17 IO chip
#define IO_DEVICE_ADDRESS_WRITE   0x40 
#define IO_DEVICE_ADDRESS_READ    0x41

#define IODIRA        0x00      //Data Direction Register for PORTA    
#define IODIRB        0x01      //Data Direction Register for PORTB 
#define IPOLA         0x02      //Input Polarity Register for PORTA 
#define IPOLB         0x03      //Input Polarity Register for PORTB 
#define GPINTENA      0x04      //Interrupt-on-change enable Register for PORTA 
#define GPINTENB      0x05      //Interrupt-on-change enable Register for PORTB 
#define DEFVALA       0x06      //Default Value Register for PORTA    
#define DEFVALB       0x07      //Default Value Register for PORTB      
#define INTCONA       0x08      //Interrupt-on-change control Register for PORTA  
#define INTCONB       0x09      //Interrupt-on-change control Register for PORTB      
#define IOCON         0x0A      //Configuration register for device                      
#define GPPUA         0x0C      //100kOhm pullup resistor register for PORTA (sets pin to input when set)    
#define GPPUB         0x0D      //100kOhm pullup resistor register for PORTB (sets pin to input when set)      
#define INTFA         0x0E      //Interrupt flag Register for PORTA        
#define INTFB         0x0F      //Interrupt flag Register for PORTB    
#define INTCAPA       0x10      //Interrupt captured value Register for PORTA  
#define INTCAPB       0x11      //Interrupt captured value Register for PORTB    
#define GPIOA_IO      0x12      //General purpose I/O Register for PORTA
#define GPIOB_IO      0x13      //General purpose I/O Register for PORTB
#define OLATA         0x14      //Output latch Register for PORTA 
#define OLATB         0x15      //Output latch Register for PORTB

// TFT LCD display mode
#define _YES_NO                 101
#define _SET_TORQUE             102
#define _SET_SPEED              103
#define _MAIN                   104     // Not use
#define _SETTING                105
#define _SETTING_TIME           106
#define _SETTING_BACK           107
#define _ALARM                  108
#define _OPERATION              109
#define _SLEEP                  110
#define _MONITORING_NET         111
#define _MONITORING_SER         112
#define _MONITORING_SSID        113
#define _MONITORING_DRIVER      114
#define _MONITORING_VER         115
#define _MONITORING_MODE        116
#define _JOG_REMOTE             117
#define _JOG_AUTO_SET           118

// advance
#define	ADV_CLAMP_TQ_CONTROL		1
#define ADV_PREVAIL_TQ_CONTROL		2
#define ADV_THREAD_TAPING			3
#define ADV_FING_ENGAGING_TORQUE	4




// system error define
#define TRANSDUCER_NOT_CONNECTED	101         // not use
#define NOT_CALIBRATION				102         // not use
#define OVER_RANGE_TORQUE			103         // not use
#define UNDER_VOLTAGE				104         // battery under voltage
#define CHECK_TRANSDUCER			105         // sensor offset error
#define ENCODER_OPEN_ERROR          106         // Encoder value error
#define PARA_READ_ERROR             107         // not use
#define MONITORING_DATA_READ_WRITE  108         // monitoring data read/write error, SPI memory. (체결Data)
#define OVER_CURRENT                109         // over current

// error define
#define AD_OFFSET_ERR               110         // current offset error
#define BAT_UNDER_VOLTAGE           111         // Battery under voltage gpio (셀의 이상 등 문제가 있을 때)
#define OVER_SPEED                  112         // over speed
#define DRIVER_PARAMETER_ERROR      113         // driver parameter 이상.
#define NOT_COMPATABLE_DRIVER       114         // setting 과 driver data 가 맞지 않는 경우.
#define NOT_RECOGNIZE_CONTROLLER    115         // dip switch에서 선택된 모델과 driver data가 일치하지 않는 경우.
#define IO_READ_ERROR_I2C           116         // not use
#define NO_SPEED_ERROR              118         // 일정 시간동안 모터 회전이 없는 경우.
#define BARCODE_READ_WRITE_ERROR    119			// barcode read/write error. memory access error.

#define NO_CONNECTION_WIFI          120         // wifi 통신이 끊어진 경우.
#define SDCARD_FAIL                 121         // not use
#define USB_COMMUNICATION_FAIL      122         // usb comm fail
#define INIT_FAIL_WIFI              123         // wifi module 초기화가 되지 않는 경우.
#define NO_RAMPUP_TORQUE			125			// driver이상으로 torque를 끌어 올리지 못할 경우 time limit에 걸림. (1000ms)
#define HALL_SENSOR_OPEN            126			// check hall sensor
#define CHECK_BATTERY               127         // Check battery type 18v or 25v

#define PARAMETER_READ_WRITE        200         // parameter read/write error, SPI memory.
#define PARA_CHECKSUM_ERROR         201         // parameter checksum error
#define MS_PGM_ERROR                220         // multi pgm을 잘못 한경우..

#define FASTENING_TIMEOUT           300         // fastening time out
#define LOOSENING_TIMEOUT           301         // loosening time out
#define MODEL_SETTING_ERROR         302         // not use
#define MODEL_CANCEL_ERROR          303         // not use
#define OVER_TIME_DURING_LOOSENING  304         // 초기 풀림 시간 over
#define OVER_TORQUE_BEFORE_RAMPUP   305         // over torque display before ramp up..

#define COUNT_TIME_OVER_ERROR       310 		// P242 setting time over when use count mode 2.
#define COUNT_MISSING_ERROR         311			// screw count missing error when use count mode 1-3.
#define CROWFOOT_REVERSE_TORQUE_ERROR   329     // CROW FOOT에서 Reverse 토크를 설정하지 않았을 때 (역회전 동작하지 않음)
#define FASTENING_TURN_MIN_ERROR    330			// fastening min angle error.
#define TARGET_ANGLE_PARA_SET_ERROR 331			// when use AC/TM, We have to set the Tartget Angle.
#define FASTENING_TURN_MAX_ERROR        332		// fastening max angle error.
#define FASTENING_STOP_ERROR            333		// when use fastening stop error, not finshed fastening process.
#define FIND_ENGAGING_TORQUE_ERROR      334		// Can not dectet engaging torque when use engaging torque.
#define C_TORQUE_ERROR                  335		// converted torque range over error when use check converted torque parameter.
#define FASTEN_OVER_TORQUE_ERROR        336		// when use angle after torque up process, reach to the dirver max torque.
#define TORQUEUP_DURING_FREE_SPEED_ERROR        337		// when use free-speed, freed 구간에서 target torque 이상이 발생됨.
#define THREADTAP_MAX_TORQUE_ERROR		338 	// when reach to the max torque.
#define THREADTAP_MIN_MAX_RANGE_ERROR	339		// after ThreadTap, out of range error.

#define SEATING_TQ_ERROR				350		// seating tq error.
#define SEATING_ANGLE_ERROR				351		// seating angle error.
#define COMPENSATION_OVER_MAX_TQ		352		// compensation torque를 계산 후 max torque 보다 높을 경우
#define CLAMP_TQ_ERROR					353		// clamp torque error.
#define A2_ANGLE_ERROR					354		// A2 min/max error in clamp torque.
#define NO_FINSH_PREVAILING				355		// not finshed prevailing
#define PREVAILING_TQ_ERROR				356		// min/max prevailing torque error..
#define ADVACNED_PARA_ERROR				358		// Advanced plus 기능 파라미터에 설정이 부족할 때 발생하는 에러 코드
#define RUNDOWN_TORQUE_ERROR			359		// 

#define ETHNET_CHIP_INIT_FAIL           400     // not use
#define SOCKET_ERROR                    401     // not use

// system_warning
#define OVER_TEMPERATURE_MOTOR        500
#define OVER_TEMPERATURE_BAT          501
#define OVER_TEMPERATURE_PCB          503
#define TOTAL_SCREW_OVER              505

// change Nm to others
#define CONVERTER_cNm_Nm      	100.         // cN.m/N.m
#define CONVERTER_Kgfcm_Nm      10.19716
#define CONVERTER_lbfin_Nm      8.850746
#define CONVERTER_ozfin_Nm      141.6119
#define CONVERTER_lbfft_Nm      0.7375622
#define CONVERTER_Kgfm_Nm      	0.1019716

// N.m 기준 각 단위를 Nm로 변경하는 상수
#define Nm_CONVERTER_cNm        0.01         // cN.m/N.m
#define Nm_CONVERTER_Kgfcm      0.0981
#define Nm_CONVERTER_lbfin      0.113
#define Nm_CONVERTER_ozfin      0.00706
#define Nm_CONVERTER_lbfft      1.36
#define Nm_CONVERTER_Kgfm       9.81

// 각 단위를 kgfcm 단위로 변경 하는 상수.
#define Kgfcm_CONVERTER_Nm      0.0981          // N.m/Kgf.cm
#define Kgfcm_CONVERTER_cNm     9.81            // cN.m/Kgf.cm
#define Kgfcm_CONVERTER_Ibfin   0.868           // Ibf.in/Kgf.cm
#define Kgfcm_CONVERTER_ozfin   13.9            // ozf.in/Kgf.cm
#define Kgfcm_CONVERTER_lbfft   0.0723          // lbfft/Kgf.cm
#define Kgfcm_CONVERTER_Kgfm    0.01            // kgf.m/Kgf.cm
#define Kgfcm_CONVERTER_Kgfcm   1               // kgf.cm/Kgf.cm 

// real time running status 
#define rNONE               0
#define rFASTENING_OK       1
#define rFASTENING_NG       2
#define rF_L                3
#define rPRESET_CHANGE      4       // or model change
#define rALARM_RESET        5
#define rSYSTEM_ERR         6
#define rBarcode            7
#define rScrewCount_D1      8
#define rScrewCountReset    9

//key define
#define MODE_KEY		0x01
#define UNIT_KEY		0x02
#define UP_KEY			0x04
#define DOWN_KEY		0x08

#define KEY_TIME 		3 		// 30ms

#define   TS_VEL  0.000100  // 100us
#define   TS_CUR  0.000050  // 50 usec  
#define   INV_TS_VEL  1 / TS_VEL

// OP Code define FM25L512
#define WREN    0x06
#define WRDI    0x04
#define RDSR    0x05
#define WRSR    0x01
#define READ    0x03
#define WRITE   0x02

// Set scale 
#define PI              3.141592654
#define TWO_PI          6.28318530718

#define RPM_TO_RAD_SCALE    0.1047197551197 // (2 PI)/60 = 0.1047197551197
#define RAD_TO_RPM_SCALE    9.549296585514  // 60/2PI


#define I2C_ADDRESS_IO     0x40


#define FASTENING_CONTROL   0
#define LOOSENING_CONTROL   1

/* fastening_step */
#define REVOLUTION_START        1
#define STOP_REVERSE_ANGLE      2
#define THREADTAP				3
#define FIND_ENGAGING_TORQUE    4
#define START_FASTENING         5
#define FIND_SEATING_POINT      6
#define REDUCE_SPEED            7
#define RAMPUP_TARGET_TORQUE    8
#define MAINTAIN_TARGET_TORQUE  9
#define LITTLE_REWIND           10




#define PRODUCT_CODE        19      // TODO: 제품 코드 설정 필요 (BMT: 19, BMT40: 23)

// display unit
#define Kgfcm           0
#define Kgfm            1
#define Nm              2               
#define cNm             3               
#define lbfin           4
#define Ozfin           5
#define lbfft           6

// TRANSDUCER_CALIB_UNIT
#define UNIT_Kgfcm      0
#define UNIT_Kgfm       1
#define UNIT_Nm         2
#define UNIT_cNm        3
#define UNIT_Lbfin      4
#define UNIT_ozfin      5
#define UNIT_Lbfft      6

// count..
#define		PROGRESS	0
#define		COMPLETE	1
#define		NG			2


// TODO: 펌웨어 타입 설정
#define HANTAS_FIRMWARE             TRUE

#define HANTAS_FIRMWARE_NUMBER      1
#define MOUNTZ_FIRMWARE_NUMBER      2


typedef enum 
{
    CAL_UNIT,
    MAX_TQ,
    OFFSET,
    CW_MIN,     // 10%
    CW_MID,     // 50%
    CW_MAX,     // 100%
    CCW_MIN,
    CCW_MID,
    CCW_MAX
}SENSOR_DATA;

// #define INPUT_SIZE       21
// #define OUTPUT_SIZE      17

// #define IN_PORT_LENGTH      8
// #define OUT_PORT_LENGTH     8



// system reset level
#define 	RESET_ALL_PARA	1
#define 	RESET_UNIT		2
#define 	RESET_MODEL		3
#define 	RESET_CROWFOOT	4
// #define 	RESET_SYSTEM_ONLY 4

// Controller model
#define BMT40               1

// Motor
#define MT40PI_24V          1        // real 18V motor (실제는 18V 모터지만 24V 사용)

// Driver model
#define BMT4001              1       // maxon ECi 18V 40d 1:1           not use
#define BMT4002              2       // maxon ECi 18V 40d 1:1.96        not use
#define BMT4004              3       // maxon ECi 18V 40d 1:4
#define BMT4006              4       // maxon ECi 18V 40d 1:6
#define BMT4011              5       // maxon ECi 18V 40d 1:11.3569
#define BMT4016              6       // maxon ECi 18V 40d 1:16.0
#define BMT4020              7       // maxon ECi 18V 40d 1:20.0

#define BMT4024              8       // maxon ECi 18V 40d 1:24.0
#define BMT4036              9       // maxon ECi 18V 40d 1:36.0
#define BMT4045              10      // maxon ECi 18V 40d 1:45.4276
#define BMT4064              11      // maxon ECi 18V 40d 1:64.0
#define BMT4080              12      // maxon ECi 18V 40d 1:80.0


#define DRV_MODEL_SIZE      20

#define RIGHT_SCREW 0
#define LEFT_SCREW  1


typedef  struct
{
    uint64_t Run                          :1;   // #00.
    uint64_t Reset                        :1;   // #01.
    uint64_t ControlFL                    :1;   // #02.     정/역 구분.
    uint64_t Lock                         :2;   // #03.     driver lock 샐행.
    uint64_t Stopping                     :1;   // #04.     stop process start
    uint64_t Multi_Mode                   :1;   // #05.     select mult mode
    uint64_t Multi_Start                  :1;   // #06.     start mult sequence by IO or start switch
    uint64_t TorqueUpCompleteOut          :1;   // #07.
    uint64_t FasteningCompleteOut         :1;   // #08.
    uint64_t LockCommand                  :2;   // #09. #10 driver Lock 유형
    uint64_t Buzzer                       :1;   // #11.     buzzer control
    uint64_t ReceiveModBusData            :1;   // #12.
    uint64_t InternalRun                  :1;   // #13.     driver start switch
    uint64_t ExternalRun                  :1;   // #14.     IO start
    uint64_t RunByMult                    :1;   // #15.     Multisequence start 내부에서 실행..
    uint64_t JabCompliteIoOut             :1;   // #16.     Flag_JabCompliteIOOut io 출력..
    uint64_t FirmwareUpdate               :1;   // #17. 
    uint64_t CountStartSensorSignalResult :1;   // #18. sensor 입력을 지연 시간을 감안한 신호..
    uint64_t ParaInitialize               :1;   // #19.
    uint64_t SaveParameter                :1;   // #20.
    uint64_t SearchCommand                :1;   // #21.
    uint64_t OneTimeExecute               :1;   // #22. 초기 bootint시 한번만 실행..
    uint64_t OneTimeDisplayTFT            :1;   // #23. LCD 관련 mode change시 한번 실행.
    uint64_t SendHostCTqNotComplete       :1;   // #24. c tq값을 올려 주는 step 정의..
    uint64_t FasteningStopAlarm           :1;   // #25. after start 체결전 정지..
    uint64_t FoundEngagingTorque          :1;   // #26.
    uint64_t Reached_LITTLE_REWIND        :1;   // #27. if error appier display torque.
    uint64_t DriverParaInit               :1;   // #28. driver parameter init request
    uint64_t DriverSaveParaData           :1;   // #29.
    uint64_t RunningMultisequence         :1;   // #30. 실제로 step을 실행하고 있는 단계..
    uint64_t ReadMSData                   :1;   // #31  step을 실행하기 전에 기초 data를 가져왔음을 알리는 flag..
    uint64_t ScrewDecrLoosening           :1;   // #32. Loosening 동작 중 일정 토크 이상 발생하면 Screw의 개수를 -1 시키는 Flag
}flag;

typedef  struct
{
    uint64_t WithinPrevalingTorque           :1;   // #00.
    uint64_t FinshedPrevaling                :1;   // #01.
    uint64_t FindedSeationPointStep          :1;   // #02.      
    uint64_t ReduceSpeedStep                 :1;   // #03.
    uint64_t OverTorqueErrorCheck            :1;   // #04.   
    uint64_t FactorySetting                  :1;   // #05.       
    uint64_t System_reset                    :1;   // #06.   
    uint64_t FindedTargetTorqueStep          :1;   // #07.
    uint64_t ReverseAngleCompleteStep        :1;   // #08.
    uint64_t ReachedMaxAngle                 :1;   // #09. max angle에 도착..
    uint64_t ReachedInitialSpeed             :1;   // #10. 
    uint64_t FasteningErrorForLED            :1;   // #11.  fastening error시 red led 표기.
    uint64_t SnugTorque                      :1;   // #12.
    uint64_t OutputTestEnable                :1;   // #13.      
    uint64_t SendOptionCardData              :1;   // #14.
    uint64_t ReturnOptionCardData            :1;   // #15
    uint64_t ReloadModelData                 :1;   // #16
    uint64_t ScrewCountProcessStatus         :2;   // #17,  #18 0:체결중 1:완료 2:NG;
    //uint64_t revsed19  :1;   // #18
    uint64_t ReachedTargetTorqueForCount     :1;   // #19 체결 완료 후 count 조건 확인을 위한 count 전달 자..
    uint64_t OneTimeCountForFastenProcess    :1;   // #20 mult나 일반 체결중에 한번만 count 하기 위한 flag
    uint64_t BackCountForTouch               :1;   // #21 touch에서 back key를 누를 경우..
    uint64_t etReceiveModBusData             :1;   // #22 ethernet data received
    uint64_t ComPort232Enable                :1;   // #23 RS232 
    uint64_t ComPortEthernetEnable           :1;   // #24 Ethernet
    uint64_t CompleteMultisequence           :1;   // #25
    uint64_t SleepMode                       :1;   // #26
    uint64_t NoSavePara                      :1;   // #27
    uint64_t CntCheckEthernet                :1;   // #28  ethernet connection check main에서 100ms 마다 check하기 위한 flag
    uint64_t EthernetPHYInit                 :1;   // #29      PHY Init 여부..
    uint64_t WithinThreadTapTorque           :1;   // #30 
    uint64_t FinishedThreadTap               :1;   // #31
}flagfasten;

typedef struct 
{
    long After_Fastening_Start_abs;     // Display되는 Angle
    long After_Fastening_Start_ForA1;
    long After_Fastening_Start_ForA2;
    long ThreadTap_Angle;
    long DisplayLoosening;
    long StartSnugTq;
    long RealSnugTq;                    // end - start
    long ContinuSnugAngle;              // 연속적인 값..
    long After_Fastening_Start;         // 역회전 제외된 값.
}mpulse;

typedef struct
{
    int MonitoringRS232;
    int MointoringEthernet;         
    int Capture;                // triger가 on 이면..
    uint16_t TimeOutCntRS232;
    uint16_t TimeOutCntEthernet;
} trig_real;

typedef struct
{
    int MonitoringRS232;
    int MointoringEthernet;
    int Channel[2];             // 1:torque, 2:current, 3:speed, 4:angle, 5:tq_angle, 6:current command, 7: snug angle, 8: torqur/angle
    int SamplingRate;           // 1:5ms, 2:10ms, 3:15ms, 4:30ms
    int Option;                 // 1 : fasteing 2: loosening 3 : both
    int DataCount;              // data 개수.
    int Capture;
    uint16_t TimeOutCntRS232;
    uint16_t TimeOutCntEthernet;    
    uint8_t graphStop;
    uint8_t Flag_MonitoringData_Overflow;
} trig;

typedef struct
{
    int PGNumber;           
    int StepNumber;             
    long LooseningAngle;
    int LoopCount;
} multSq;

typedef struct 
{
    float Speed;
    long TimeLimit;
    long StallTimeLimit;
    float SpeedAcc;
    // Crow foot
    float Crowfoot_speed;
	float Crowfoot_speedAcc;
	float Crowfoot_torque_Nm;
}loosen;


typedef struct 
{
    int   TcamActm;         // torque control and angle monitoring
    float TargetTorque_Nm;     // N.m 단위..
    float MaxTorque_Nm;
    float TargetSpeed;      // radian   real motor speed
    float MaxSpeed;
    int   RpmOfLoad;        // bit rpm
    float SeatingTQ_Nm;
    float SnugTQ_Nm;
    float TorqueRisingValue;
    float RampUpSpeed;      // torque up시 control speed 해당 값은 torque 상승 시간과도 관계가 있다..
    float RampUpSpeedToControl;      // 제어 최소 speed
    //float RampUpSpeed_forSoftJoint;   // if softjoint, i wil change the this value.
    //float P1;
    int SeatingValue;
    float P2;
    float P3;
    long HoldingTime;
    long TargetAngle;       // degree
    long MinAngle;          // degree
    long MaxAngle;          // degree
    long FasteningTimeLimit;
    float SpeedAcc;
    float SpeedDcc;
    float SpeedAccForRampUp;
    long  JudgeMinAngle;
    float AutoSpeed;    // auto speed
}fastensq;

typedef struct
{
	uint16_t ReadCw_min;
	uint16_t ReadCw_max;
	uint16_t ReadCcw_min;
	uint16_t ReadCcw_max;
    uint16_t cw_max_min_interpol; 		// 100% - 0% = x,  
    uint16_t cw_min_0_interpol;
    uint16_t ccw_max_min_interpol;
    uint16_t ccw_min_0_interpol;
    uint16_t cw_max_min;					// 100% - 50%
    uint16_t cw_min_0;
    uint16_t ccw_max_min;
    uint16_t ccw_min_0;
    double 	cUNIT[8]; 					// 초기 transducer에서 cal한 값을 each unit으로 변경한 것..
    double  ConvTqValueCW_max_min;
    double  ConvTqValueCW_min_0;
    double  ConvTqValueCCW_max_min;
    double  ConvTqValueCCW_min_0;
    uint16_t MaxTorque;
    uint8_t  CalUnit;
}adconstantvalue;

typedef struct 
{
    float i_kp;
    float i_ki;
    float i_Ka;
    float w_kp;
    float w_ki;
    float w_Ka;
    float hss_gain;    
}gain;

typedef struct 
{
    long Angle;
    float Speed;
    // uint16_t UsingPreset;
    float SpeedAcc;
}freereverse;

typedef struct 
{
    float Speed;
    long  Angle;
	float Torque_Nm;
    float SpeedAcc;
    float SpeedDcc;
    int   FlagInStep;
    int   FlagOutStep;
    int   FlagInOut;      // for dinamic skip
}freespeed;


typedef struct
{
	float minTorqueUnitConvert;		// Max Torque를 Unit으로 계산한 값 
	float maxTorqueUnitConvert;

}settingRange;



//========================================
typedef struct 
{
    unsigned int    con_model;      // controller model no. 1:26, 2:32
    unsigned int    motor;          // used motor no.       1:26, 2:32
    float           tq_min;         // default Nm
    float           tq_max;         // default Nm
    unsigned int    speed_min;

    unsigned int    speed_max;
    float           def_tq;         // default torque value Nm 기준..
    float           tq_conver;      // driver 의 공장 보상값. 1.0 = 100
    unsigned int    acc_dcc;        // default 가감속.. ms
    unsigned int    seating_point;  // default seating point %

    unsigned int    sc_type;        // default screw type Right: 0, left:1 
    char            name[32];       // driver name display for TFT name[7]
    float           gear_ratio;
} dr_model;

typedef struct 
{
    float Ra;            /* Resistance of Motor */
    float La;            /* Inductance of Motor */
    float Kt;            /* = Te_rated / I_rated */
    float inv_Kt;         /* Used in Current Loop */
    float Ke;
    float Jm;            
    float I_rated;      
    float Te_rated;      
    float Nominal_voltage;
    float max_motor_speed;
    float OverCurrentLimit;
    float torque_limit;
    int   num_pulse;      // Number of 1 Mech_Rev. pulse 
    float wn_cc;
    float wn_sc;
    float gradient;     // rpm/torque
    float mech_t;
    int   Poles;
    float Ratio;
}motor;

typedef struct 
{
    float TqConv_kfgcm;
    float TqConv_kgfm;
    float TqConv_Nm;
    float TqConv_cNm;
    float TqConv_Ibfin;
    float TqConv_ozfin;
    float TqConv_Ibfft;
    float AngleConv_mPulseToDeg;
    float LoadConv;
    float SamplingPulsConv;
    float ConvSpeed_screwtype;
    float ConvSpeed;
}math_conversion;




/* Exported functions ------------------------------------------------------- */


#endif  /* _COMMON_H */

/*******************(C)COPYRIGHT 2009 STMicroelectronics *****END OF FILE******/
