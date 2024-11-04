
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _VARIABLE_H
#define _VARIABLE_H

#define TQ_EXT extern //volatile


/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "common.h"


/* Variabless ------------------------------------------------------------------*/
TQ_EXT float i_real, i_ref, tq_cmd, i_ref_flt, i_err, i_pi_out;
TQ_EXT uint8_t Flag_L_F_control_current;
TQ_EXT float tq_real_Nm;

TQ_EXT float   w_cmd, w_cmd_flt, w_err, w_err_1, w_rpm, w_rpm_abs, w_rpm_display;
TQ_EXT float   w_pi_out;
TQ_EXT int32_t m_cnt_m, m_cnt_m_old, m_pulse_diff, m_pulse_diff_abs;

// TQ_EXT float tq_real;
// TQ_EXT float tq_ref_flt;
// TQ_EXT float tq_ref;
// TQ_EXT float tq_pi_out;
// TQ_EXT float tq_err;
TQ_EXT uint32_t v_loop_cnt;

TQ_EXT uint16_t system_err;
TQ_EXT uint16_t current_mode;
TQ_EXT float i_offset;


TQ_EXT uint8_t WifiInitOn;              // Wifi communication fail init
TQ_EXT uint16_t GraphTimeout;           // Graph Timeout count
TQ_EXT uint32_t multiWaitCount;         // Multicast message wait time for send message  (JUN)
TQ_EXT float real_battery_voltage;
TQ_EXT uint8_t BatteryType;

TQ_EXT int32_t adc_current_value[2];
TQ_EXT int32_t ADCRecBuff[3];

TQ_EXT int   TargetSign;
TQ_EXT float Delta_acc, Delta_dcc;
TQ_EXT float TargetSpeed;
TQ_EXT float vin_out;
TQ_EXT float adc_conv_value;

TQ_EXT float    w_Ka;
TQ_EXT float    Iref_It, Iref_fb;
// TQ_EXT uint8_t Flag_End_process;


// TQ_EXT int Flag_DRIVER_PARAMETER_ReadERROR; 	// 해당 에러가 발생을 하면 flag를 on 하고, 이 에러가 발생을 하면 전원을 다시 켜야 한다.

TQ_EXT uint8_t Model_Number;
TQ_EXT uint8_t Processing_number;
TQ_EXT uint8_t Fastening_number;
TQ_EXT uint16_t ScrewCountValue;
TQ_EXT uint8_t flagSleepCheck;
TQ_EXT uint16_t pCount_TimeLimit;
TQ_EXT uint8_t Flag_CountEnd;
TQ_EXT uint16_t Cnt_FasteningCompleteOut;
TQ_EXT float Fasten_Complete_Torque_with_Conv;
TQ_EXT float Fasten_Real_Torque;

TQ_EXT uint16_t Tq_SensorValue_Avg_tmp;

TQ_EXT uint16_t Disp_Loosening_time;
TQ_EXT uint16_t Fastening_time;

TQ_EXT uint8_t Input_StartInternal;
TQ_EXT uint8_t Input_LoosnFasten;
TQ_EXT uint16_t Cnt_LooseningTime;
TQ_EXT uint32_t TotalLooseningTimeReal, TotalFasteningTime;
TQ_EXT uint16_t Cnt_EngagingTQTimeLimit;
TQ_EXT float tq_cmd_increase;
TQ_EXT uint8_t fastening_step;
TQ_EXT uint8_t Cnt_JumpModeChangeToP2;

TQ_EXT uint8_t OledRx_data[2];
TQ_EXT uint8_t ComReadBufferMODBUS[512];
TQ_EXT uint16_t Cnt_ComRead_Buffer_a;
TQ_EXT uint8_t Cnt_OverTimeCheckModBus;
TQ_EXT uint8_t etReadBuffer[512];
TQ_EXT uint8_t dset[14], dParameterValue[14];
TQ_EXT uint16_t DriverSaveParaNumber;
TQ_EXT uint16_t mbMData[128];
TQ_EXT float   deceleration_ratio, inv_deceleration_ratio;
TQ_EXT int16_t mbTData[430];
TQ_EXT float gMonitoringData[2][1000];

TQ_EXT uint16_t backup_para_no, Para_value_temp;

TQ_EXT float   tq_real_avg, tq_real_avg_old;
TQ_EXT uint8_t Flag_JumpCompleteAngleControl;
TQ_EXT long   Cnt_RewindAngle;
TQ_EXT int  screw_type;
TQ_EXT float  Torque_ConvertValue;
TQ_EXT float  inv_Torque_ConvertValue;
TQ_EXT float   Delta_acc_loosen, Delta_dcc_loosen;
TQ_EXT float    P_ConvertedTorqueMaxLimit, P_ConvertedTorqueMinLimit;
TQ_EXT float Temperature_real_Flt_motor;
TQ_EXT float Temperature_real_Flt_bat;
TQ_EXT float Temperature_real_Flt_pcb;
// TQ_EXT float Temperature_real_pcb;
// TQ_EXT float Temperature_real_motor;
// TQ_EXT float Temperature_real_bat;

TQ_EXT uint16_t Flag_power_down;
TQ_EXT float key_input_value;
TQ_EXT float cursor_position;

TQ_EXT uint8_t MakePacket_T[4096];
TQ_EXT uint8_t gSENDDATABUF[4096];

TQ_EXT uint8_t ethRxdummyBuf[1600];
TQ_EXT uint8_t ethTxdummyBuf[1600];

TQ_EXT uint8_t wifiModule_buf[6400];

TQ_EXT uint8_t MakePacket_Oled[300];
TQ_EXT uint16_t Cnt_Oled_Packet;
TQ_EXT uint16_t ReadADValue;
TQ_EXT int etCntReadBuffer;
TQ_EXT int etCntOverTimeCheckModBus;
TQ_EXT uint16_t gSendDataLen;
TQ_EXT uint16_t ReceiveKeyData[4];
TQ_EXT uint8_t  spiRecvBuf[10];                // BISS 통신 DMA 데이터 받는 버퍼

TQ_EXT uint16_t TorqueRead_ToADC;
TQ_EXT uint16_t paraUnitOld;

TQ_EXT uint8_t battery_level, antena_level;
TQ_EXT uint8_t Flag_GreenLED, Flag_RedLED;
TQ_EXT uint8_t Flag_bitsocket_mode;
TQ_EXT uint8_t Flag_sleep_mode;
TQ_EXT uint16_t Cnt_SleepTime;
TQ_EXT uint8_t Flag_msStepFastenStepIN;
TQ_EXT float  v_e_real_compensation_constant;
TQ_EXT float   vel_time;
TQ_EXT long  PWM_Maxvalue;
TQ_EXT float w_rad, w_rad_tmp;
TQ_EXT float loosening_PickTorque;
TQ_EXT uint16_t DisplayAngle_forLCD;
TQ_EXT uint16_t Cnt_LCD_Off;
TQ_EXT uint8_t Flag_LCD_off;

TQ_EXT uint16_t StatusOutput;
TQ_EXT long Cnt_HoldingTime;
TQ_EXT float tq_real_avg_for_holding_Nm, tq_real_avg_for_holding_old_Nm;
TQ_EXT long cnt_250us;
TQ_EXT uint8_t Flag_sec_delay_after_power_on;
TQ_EXT uint8_t Flag_SendOledData;
TQ_EXT uint8_t KeyOldData;

TQ_EXT float w_rpm, w_rpm_tmp;
TQ_EXT uint8_t make_backup_data[256];
TQ_EXT uint16_t make_backupRead_data[128];
TQ_EXT float rampup_tq_value;


TQ_EXT uint32_t adc_value_buf[7];
TQ_EXT int16_t AD_OFFSET_VALUE;
TQ_EXT uint8_t Tq_Sensor_read_data[5];
TQ_EXT uint16_t SystemResetValue;
TQ_EXT int16_t SensorAD_OffsetValue;
TQ_EXT int16_t SensorOffset;
TQ_EXT uint16_t Tq_SensorValue;
TQ_EXT uint16_t Tq_SensorValue_Avg;
TQ_EXT uint16_t Tq_SensorValue_Avg_max;
TQ_EXT float RealTq_fromSensor_reflectedSettingUnit;
TQ_EXT uint8_t Flag_GetSensorOffset;
TQ_EXT uint8_t  torqueReadOn;

// user test
TQ_EXT float ThreadTapTorque_real;
TQ_EXT uint8_t  paraLastPresetNum;               // LAST_PRESET_NUM

// for BLDC motor
TQ_EXT uint8_t 	Hall_position;
TQ_EXT uint16_t PWM_Out;

TQ_EXT gain Gain;
TQ_EXT motor Motor;
TQ_EXT flag Flag;
TQ_EXT flagfasten FlagFasten;
TQ_EXT trig_real  TRIG_REAL;
TQ_EXT loosen LooseningSeq;
TQ_EXT multSq  ms;
TQ_EXT freereverse FreeReverse;
TQ_EXT freespeed FreeSpeed;
TQ_EXT math_conversion MathConv;
TQ_EXT mpulse mPulse;
TQ_EXT fastensq FastenSeq;
TQ_EXT motor Motor;
TQ_EXT adconstantvalue AdConstantValue;
TQ_EXT settingRange setRange;


TQ_EXT trig  TRIG;





TQ_EXT dr_model  Info_DrvModel[DRV_MODEL_SIZE];

#endif
