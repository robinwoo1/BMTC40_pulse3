/* Includes ------------------------------------------------------------------*/
#include "variable.h"
// #include "common.h"
// FIXME: SRAM에 DMA buffer 영역 선언
#define __ATTR_RAM_DMA	__attribute__ ((section(".RAM_DMA"))) __attribute__ ((aligned (4)))


/* Variables ------------------------------------------------------------------*/
float i_real, i_ref, tq_cmd, i_ref_flt, i_err, i_pi_out;
uint8_t Flag_L_F_control_current;
float tq_real_Nm;

float   w_cmd, w_cmd_flt, w_err, w_err_1, w_rpm, w_rpm_abs, w_rpm_display;
float   w_pi_out;
int32_t m_cnt_m, m_cnt_m_old, m_pulse_diff, m_pulse_diff_abs;

// float tq_real;
// float tq_ref_flt;
// float tq_ref;
// float tq_pi_out;
// float tq_err;
uint32_t v_loop_cnt;

uint16_t system_err;
uint16_t current_mode;
float i_offset;

uint8_t WifiInitOn;          // Wifi communication fail init
uint16_t GraphTimeout;           // Graph Timeout count
uint32_t multiWaitCount;         // Multicast message wait time for send message  (JUN)
float real_battery_voltage;
uint8_t BatteryType;

int32_t adc_current_value[2];
int32_t ADCRecBuff[3];

int   TargetSign;
float Delta_acc, Delta_dcc;
float TargetSpeed;
float vin_out;
float adc_conv_value;

float    w_Ka;
float    Iref_It, Iref_fb;
// uint8_t Flag_End_process;


// int Flag_DRIVER_PARAMETER_ReadERROR; 	// 해당 에러가 발생을 하면 flag를 on 하고, 이 에러가 발생을 하면 전원을 다시 켜야 한다.

uint8_t Model_Number;
uint8_t Processing_number;
uint8_t Fastening_number;
uint16_t ScrewCountValue;
uint8_t flagSleepCheck;     // Sleep mode에서 wake up 체크
uint16_t pCount_TimeLimit;
uint8_t Flag_CountEnd;
uint16_t Cnt_FasteningCompleteOut;
float Fasten_Complete_Torque_with_Conv;
float Fasten_Real_Torque;

uint16_t Disp_Loosening_time;
uint16_t Fastening_time;

uint8_t Input_StartInternal;
uint8_t Input_LoosnFasten;
uint16_t Cnt_LooseningTime;
uint32_t TotalLooseningTimeReal, TotalFasteningTime;
uint16_t Cnt_EngagingTQTimeLimit;
float tq_cmd_increase;
uint8_t fastening_step;
uint8_t Cnt_JumpModeChangeToP2;

uint8_t OledRx_data[2] __ATTR_RAM_DMA;
uint8_t ComReadBufferMODBUS[512];
uint16_t Cnt_ComRead_Buffer_a;
uint8_t Cnt_OverTimeCheckModBus;

uint8_t dset[14] __ATTR_RAM_DMA;
uint8_t dParameterValue[14];
uint16_t DriverSaveParaNumber;
uint16_t mbMData[128];
float   deceleration_ratio, inv_deceleration_ratio;

float gMonitoringData[2][1000];

uint16_t backup_para_no, Para_value_temp;

float   tq_real_avg, tq_real_avg_old;
uint8_t Flag_JumpCompleteAngleControl;
long   Cnt_RewindAngle;
int  screw_type;
float  Torque_ConvertValue;
float  inv_Torque_ConvertValue;
float   Delta_acc_loosen, Delta_dcc_loosen;
float    P_ConvertedTorqueMaxLimit, P_ConvertedTorqueMinLimit;
float Temperature_real_Flt_motor;
float Temperature_real_Flt_bat;
float Temperature_real_Flt_pcb;
// float Temperature_real_pcb;
// float Temperature_real_motor;
// float Temperature_real_bat;

uint16_t Flag_power_down;
float key_input_value;
float cursor_position;

uint16_t Tq_SensorValue_Avg_tmp;

uint8_t MakePacket_T[4096];
uint8_t gSENDDATABUF[4096];
int16_t mbTData[430];
uint8_t etReadBuffer[512];
uint8_t ethRxdummyBuf[1600] __ATTR_RAM_DMA;
uint8_t ethTxdummyBuf[1600] __ATTR_RAM_DMA;

uint8_t wifiModule_buf[6400] __ATTR_RAM_DMA;        // WIFI_MODULE_BUF_LEN 크기


uint8_t MakePacket_Oled[300] __ATTR_RAM_DMA;
uint16_t Cnt_Oled_Packet;
uint16_t ReadADValue;
int etCntReadBuffer;
int etCntOverTimeCheckModBus;
uint16_t gSendDataLen;
uint16_t ReceiveKeyData[4];
uint8_t  spiRecvBuf[10] __ATTR_RAM_DMA;		// BISS 통신 DMA 데이터 받는 버퍼


uint16_t TorqueRead_ToADC;

uint8_t battery_level, antena_level;
uint8_t Flag_GreenLED, Flag_RedLED;
uint8_t Flag_bitsocket_mode;
uint8_t Flag_sleep_mode;
uint16_t Cnt_SleepTime;
uint8_t Flag_msStepFastenStepIN;
float  v_e_real_compensation_constant;
float   vel_time;
long  PWM_Maxvalue;
float w_rad, w_rad_tmp;
float loosening_PickTorque;
uint16_t DisplayAngle_forLCD;
uint16_t Cnt_LCD_Off;
uint8_t Flag_LCD_off;

uint16_t StatusOutput;
long Cnt_HoldingTime;
float tq_real_avg_for_holding_Nm, tq_real_avg_for_holding_old_Nm;
long cnt_250us;
uint8_t Flag_sec_delay_after_power_on;
uint8_t Flag_SendOledData;
uint8_t KeyOldData;

float w_rpm, w_rpm_tmp;
uint8_t make_backup_data[256];
uint16_t make_backupRead_data[128];
float rampup_tq_value;


uint32_t adc_value_buf[7] __ATTR_RAM_DMA;
int16_t AD_OFFSET_VALUE;
uint8_t Tq_Sensor_read_data[5] __ATTR_RAM_DMA;
uint16_t SystemResetValue;
int16_t SensorAD_OffsetValue;

int16_t SensorOffset;
uint16_t Tq_SensorValue;
uint16_t Tq_SensorValue_Avg;
uint16_t Tq_SensorValue_Avg_max;
float RealTq_fromSensor_reflectedSettingUnit;
uint8_t Flag_GetSensorOffset;
uint8_t  torqueReadOn;

// user test
float ThreadTapTorque_real;
uint8_t  paraLastPresetNum; // LAST_PRESET_NUM


// for BLDC motor
uint16_t paraUnitOld;
uint8_t Hall_position;
uint16_t PWM_Out;

gain Gain;
motor Motor;
flag Flag;
flagfasten FlagFasten;
trig_real  TRIG_REAL;
loosen LooseningSeq;
multSq  ms;
freereverse FreeReverse;

freespeed FreeSpeed;
math_conversion MathConv;
// auto_setting AutoSetting;
settingRange setRange;

mpulse mPulse;
fastensq FastenSeq;
motor Motor;
adconstantvalue AdConstantValue;

trig TRIG = { 0, 0, {1,0}, 20, 1, 0, 0 };






// torque 기준 : Nm
// model 추가 방법..
// 1. const.c에서 모델명 추가..
// 2. const.c에서 제어기가 추가 경우 제어기명 추가.
// 3. main.c motor가 추가 될 경우 motor별 ki/kp 값 및 motor paramter 추가.
// 4. main.c에서 모델별 speed 공식 추가.
// 5. 아래 내용 추가. 모델 size 확인..
dr_model  Info_DrvModel[DRV_MODEL_SIZE]
= 
{   //con_model, motor, tq_min, tq_max, speed_min,    speed_max, def_tq, tq_conver, acc_dcc, seating_point,   screw_type, name, gear_ratio
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ""}, //dummy 0
    {BMT40, MT40PI_24V, 0.1,  1.60,  100, 2000,   0.4,   1.00, 100, 40, 0, "NONE", 1.00},       //1 BMT4001 no use.
    {BMT40, MT40PI_24V, 0.2,  3.20,  100, 2000,   0.8,   1.00, 100, 40, 0, "NONE", 1.96},       //2 BMT4002 no use

    {BMT40, MT40PI_24V, 0.4,  6.40,  100, 1400,   1.0,   0.98, 100, 40, 0, "BMT4004", 4.00},    //3 BMT4004 // 모터 모델 변경, 최대 5700rpm까지 가능         - 테스트 완료
    {BMT40, MT40PI_24V, 0.6,  9.50,  100,  950,   1.0,   1.00, 150, 40, 0, "BMT4006", 6.00},    //4 BMT4006  (Stall TQ: 2.86Nm * 0.7 * Gear * 0.8  : 0.8은 효율)         - 테스트 완료
    
    {BMT40, MT40PI_24V, 1.5, 17.50,   50,  500,   3.0,   1.00, 150, 40, 0, "BMT4011", 11.34342},//5 BMT4011 
    {BMT40, MT40PI_24V, 2.0, 25.00,   30,  350,   5.0,   1.00, 150, 40, 1, "BMT4016", 16.00},   //6 BMT4016
    // {BMT40, MT40PI_24V, 2.0, 25.00,   50,  350,   5.0,   1.00, 150, 40, 0, "BMT4016", 16.00},   //6 BMT4016
    {BMT40, MT40PI_24V, 3.0, 32.00,   50,  285,   5.0,   1.00, 150, 40, 0, "BMT4020", 20.00},   //7 BMT4020 

    {BMT40, MT40PI_24V, 0.5, 38.40,   50,  235,  10.0,   1.00, 150, 40, 1, "BMT4024", 24.0},    //8 BMT4024         - 테스트 완료
    // {BMT40, MT40PI_24V, 4.0, 38.40,   50,  235,  10.0,   1.00, 150, 40, 0, "BMT4024", 24.0},    //8 BMT4024         - 테스트 완료 - 이게 진짜
    {BMT40, MT40PI_24V, 5.0, 57.60,   50,  155,  10.0,   1.00, 150, 40, 0, "BMT4036", 36.0},    //9 BMTH3236
    {BMT40, MT40PI_24V, 6.0, 72.50,   50,  125,  10.0,   1.00, 200, 40, 0, "BMT4045", 45.3737}, //10 BMTH3245
    {BMT40, MT40PI_24V, 8.0, 102.0,   50,  85,   10.0,   1.00, 200, 40, 0, "BMT4064", 64.0},    //11 BMTH3264    
};

