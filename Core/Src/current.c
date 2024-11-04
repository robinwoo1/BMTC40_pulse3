/**
  ******************************************************************************
  * @file    velocity.c
  * @brief   Velocity Service Routines.
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#define __CURRENT_H
#include "current.h"
#undef __CURRENT_H
#include "common.h"
#include <stdlib.h>
#include "variable.h"
#include "main_bg.h"
#include "velocity.h"
#include "torque.h"
#include "svpwm.h"
#include "BldcControl.h"
#include "bissSpiCommunicate.h"

float sinTBL[585];

float Vref_max;
float PWM_gain;
int16_t adc_current_u_value, adc_current_v_value;
float i_u_offset, i_v_offset, i_u_offset_back, i_v_offset_back;
long PWM_Offset, PWM_HalfMax;

float Vuss_ref, Vvss_ref, Vwss_ref, Vofs;
float Idse_ref, Iqse_ref, Idse, Iqse, Idse_flt, Iqse_flt, EIdse, EIqse;
float Iqse_sensor, Iqse_sensor_flt;
float Idse_ref_flt, Iqse_ref_flt;
float Vdse_ref_It, Vdse_ref_fb, Vdse_ref_ff, Vdse_ref, Vdse_ref_out;
float Vqse_ref_It, Vqse_ref_fb, Vqse_ref_ff, Vqse_ref, Vqse_ref_out;
float iu_real, iv_real, iw_real;

float Tref, Tref_p, Tref_i, Tref_ff;
float Tref_ramp, Tref_slop;
// float tq_cmd, tq_cmd1;

float sin_theta_e, cos_theta_e;
long Fu_ref, Fv_ref, Fw_ref, Pwm_ref;
unsigned int theta_e_TBL;

int32_t on_time_cnt, on_time=500;
int32_t off_time_cnt, off_time=400;//350;
int32_t on_off_state = 0, pulse_on = 0;
// float previous_Out_tq;
float previous_Tref;
float previous_Tref_i;
float previous_w_cmd;
float tq_cmd_pulse_on;
float preVqse_ref_It, preVqse_ref_fb, preVqse_ref_ff, preVqse_ref;

/* Defines ------------------------------------------------------------------*/
#define  MOV_FILTERSIZE			2						// 필터 크기
#define  MOV_BUF_FILTERSIZE		MOV_FILTERSIZE + 10		// 저장할 버퍼의 크기

#define     INV_SQRT3       0.57735
#define     SQRT_3_INV_2    0.866025404


/* Variables ------------------------------------------------------------------*/
// SPI
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim1;

float max_measure_tq;
int32_t max_measure_value;
/* Functions -------------------------------------------------------------------*/
void Pwm_H_On(uint32_t Channel, long Value)
{
  switch (Channel)
  {
    case TIM_CHANNEL_1:
      TIM1->CCR1 = Value;
      break;
    case TIM_CHANNEL_2:
      TIM1->CCR2 = Value;
      break;
    case TIM_CHANNEL_3:
      TIM1->CCR3 = Value;
      break;
    default:
      break;
  }
  HAL_TIM_PWM_Start(&htim1, Channel);
  HAL_TIMEx_PWMN_Start(&htim1, Channel);
}
void Pwm_L_On(uint32_t Channel)
{
  switch (Channel)
  {
    case TIM_CHANNEL_1:
      TIM1->CCR1 = 0;
      break;
    case TIM_CHANNEL_2:
      TIM1->CCR2 = 0;
      break;
    case TIM_CHANNEL_3:
      TIM1->CCR3 = 0;
      break;
    default:
      break;
  }
  HAL_TIM_PWM_Start(&htim1, Channel);
  HAL_TIMEx_PWMN_Start(&htim1, Channel);
}
void Pwm_Off(uint32_t Channel)
{
  switch (Channel)
  {
    case TIM_CHANNEL_1:
      TIM1->CCR1 = 0;
      break;
    case TIM_CHANNEL_2:
      TIM1->CCR2 = 0;
      break;
    case TIM_CHANNEL_3:
      TIM1->CCR3 = 0;
      break;
    default:
      break;
  }
  HAL_TIM_PWM_Stop(&htim1, Channel);
  HAL_TIMEx_PWMN_Stop(&htim1, Channel);
}


// 50us loop
void current_loop(void)
{
  float ftmp, min, max;
  
	static uint16_t Tq_SensorValueOld;
	int calcVal;
	float i_err_1;	
	int32_t read_sign_value = 0;
	float i_current;
  int32_t comp_th;

	// ADC의 변화가 갑작스럽게 커졌을 때 - 기존의 ADC 값으로 유지한다.
	calcVal = Tq_SensorValueOld - Tq_SensorValue;
	if ((abs(calcVal) > 10000))
	{
		Tq_SensorValueOld = Tq_SensorValue;
		Tq_SensorValue = Tq_SensorValue_Avg;
	}
	else
		Tq_SensorValueOld = Tq_SensorValue;

  Idse = (float)adc_value_buf[2]*3.3/4096/4e-3 - 4.8;
  if (tq_cmd<0)
    Idse *= -1.0;

  // // Run 신호에 따라 PWM 동작모드 제어 
  // pwmStartOnOff(Flag.Run);
	
	// ADC값 필터
	Tq_SensorValue_Avg = MovingAverage(Tq_SensorValue);
	// Offset 감소
	read_sign_value = (int32_t)Tq_SensorValue_Avg - (int32_t)SensorOffset;
	// read_sign_value = (int32_t)Tq_SensorValue - (int32_t)SensorOffset;
	// ADC 값 토크로 변환 (설정한 UNIT 토크 값)
	// RealTq_fromSensor_reflectedSettingUnit = convertToTorque(read_sign_value); TODO: 센서가 반대로 읽힘 - (-)로 움직일 때 토크가 커짐 (원래 반대)
	RealTq_fromSensor_reflectedSettingUnit = -convertToTorque(read_sign_value)*0.9;

	// FIXME:: 토크 측정을 위한 ad (토크 기울기 계산을 위해 사용 - 센서의 변화에 따라 <, > 부등호 변경이 필요할 수 있음)
	if(Tq_SensorValue_Avg_max > read_sign_value)
	{
		Tq_SensorValue_Avg_max = read_sign_value;
	}
	// // tq comd에 대한 전류 계산.
	// i_ref = tq_cmd * 1.005;

	// // current command filter
	// //default 1000, TS_CUR는 면적 계산을 위해서..쓰임.  1차 필터..
	// i_ref_flt = (20000.0 * (i_ref - i_ref_flt) * TS_CUR) + i_ref_flt;

	// tq_real_Nm 는 N.m
	tq_real_Nm = RealTq_fromSensor_reflectedSettingUnit * TorqueMetric[Para[UNIT].val][UNIT_Nm];

	// 100us velocity
	// if (v_loop_cnt >= 1) 
	{
    M_cnt();
    bissSpiRecvDma();

    Vref_max = (25.) / 1.732050807568877;
    PWM_gain = (float)(htim1.Init.Period / 2) / (Vref_max*1.05);

    // Vref_max = (25.) / 1.732050807568877;
    // PWM_gain = (float)(htim1.Init.Period / 2) / (Vref_max*1.0);

    comp_th = (int32_t)(w_cmd*(float)Motor.num_pulse/(2.*3.141592)*TS_VEL*1.45);

    theta_e_TBL = (m_cnt_m + 585 - 536 + comp_th) % (585-1);// offset + compensation
    sin_theta_e = sinTBL[theta_e_TBL];
    cos_theta_e = sinTBL[(theta_e_TBL + 146) %(585-1)];

		v_loop_cnt = 0;
		Velocity_loop();
	}
	// else
	// 	v_loop_cnt++;


  if (Flag.ControlFL==0)
  {
    if (abs(read_sign_value-32767) > abs(max_measure_value-32767))
    {
      max_measure_tq = tq_real_Nm;
      max_measure_value = read_sign_value;
    }
  }
  else
  {
    max_measure_tq = 0;
    max_measure_value = 32767;
  }

  if (Flag.Run || Flag.Stopping)
  {
    Iqse_ref = tq_cmd * Motor.inv_Kt * inv_deceleration_ratio;
    // Iqse = 0;//tq_real_Nm * Motor.inv_Kt * inv_deceleration_ratio;
    // if (pulse_on)
    //   Iqse = tq_real_Nm * Motor.inv_Kt;
    // else
      // Iqse = (1.-500.*1e-4)*Iqse + 500.*1e-4*Iqse_ref;
      Iqse = (1.-500.*1e-4)*Iqse + 500.*1e-4*Idse;//_ref;
    EIqse = Iqse_ref - Iqse;

    // q-axis
    Vqse_ref_It += Gain.i_ki * (EIqse - Gain.i_Ka * (Vqse_ref_fb + Vqse_ref_ff - Vqse_ref));
    Vqse_ref_fb = Vqse_ref_It + Gain.i_kp * EIqse;
    Vqse_ref_ff = 0;
    Vqse_ref = Vqse_ref_fb + Vqse_ref_ff;

    if (pulse_on)
    {
      Motor.mech_t = Motor_mech_t_back * 0.5;
      if (on_off_state)
      {
        Vqse_ref = Vqse_ref;
        Vdse_ref = -1.*fabs(Vqse_ref);
      }
      else
      {
        Vqse_ref = 0;//-0.5*Vqse_ref;;
        // Vqse_ref = -1.*Vqse_ref;
        Vdse_ref = 0;//-0.5*fabs(Vqse_ref);
      }
    }
    else
    {
      Motor.mech_t = Motor_mech_t_back * 1.;
      Vqse_ref = Vqse_ref;
      if (Flag.ControlFL == 0)
      {
        Vdse_ref = 0;//-1.*fabs(Vqse_ref);
      }
      else
        Vdse_ref = 0;//-1.*fabs(Vqse_ref);
    }
    
    // cos_theta_e = 0.7071;//1.;
    // sin_theta_e = 0.7071;//0.;
    // Vdse_ref = 0.;//2.5;
    // Vqse_ref = 3;//2.5;

    if (Vqse_ref > Vref_max)		Vqse_ref = Vref_max;
    else if	(Vqse_ref < -Vref_max)	Vqse_ref = -Vref_max;

    ftmp = Vqse_ref * PWM_gain;
    Pwm_ref = (ftmp > (float)PWM_HalfMax) ? PWM_HalfMax : (ftmp < (float)-PWM_HalfMax) ? -PWM_HalfMax : (long)ftmp;
    if (Pwm_ref >= 0)
    {
      if ((theta_e_TBL>=536) && (theta_e_TBL<49))
      {
        Pwm_Off(TIM_CHANNEL_1);
        Pwm_L_On(TIM_CHANNEL_2);
        Pwm_H_On(TIM_CHANNEL_3, abs(Pwm_ref));
      }
      else if ((theta_e_TBL>=49) && (theta_e_TBL<146))
      {
        Pwm_H_On(TIM_CHANNEL_1, abs(Pwm_ref));
        Pwm_L_On(TIM_CHANNEL_2);
        Pwm_Off(TIM_CHANNEL_3);
      }
      else if ((theta_e_TBL>=146) && (theta_e_TBL<244))
      {
        Pwm_H_On(TIM_CHANNEL_1, abs(Pwm_ref));
        Pwm_Off(TIM_CHANNEL_2);
        Pwm_L_On(TIM_CHANNEL_3);
      }
      else if ((theta_e_TBL>=244) && (theta_e_TBL<341))
      {
        Pwm_Off(TIM_CHANNEL_1);
        Pwm_H_On(TIM_CHANNEL_2, abs(Pwm_ref));
        Pwm_L_On(TIM_CHANNEL_3);
      }
      else if ((theta_e_TBL>=341) && (theta_e_TBL<439))
      {
        Pwm_L_On(TIM_CHANNEL_1);
        Pwm_H_On(TIM_CHANNEL_2, abs(Pwm_ref));
        Pwm_Off(TIM_CHANNEL_3);
      }
      else if ((theta_e_TBL>=439) && (theta_e_TBL<536))
      {
        Pwm_L_On(TIM_CHANNEL_1);
        Pwm_Off(TIM_CHANNEL_2);
        Pwm_H_On(TIM_CHANNEL_3, abs(Pwm_ref));
      }
    }
    else
    {
      if ((theta_e_TBL>=536) && (theta_e_TBL<49))
      {
        Pwm_Off(TIM_CHANNEL_1);
        Pwm_H_On(TIM_CHANNEL_2, abs(Pwm_ref));
        Pwm_L_On(TIM_CHANNEL_3);
      }
      else if ((theta_e_TBL>=49) && (theta_e_TBL<146))
      {
        Pwm_L_On(TIM_CHANNEL_1);
        Pwm_H_On(TIM_CHANNEL_2, abs(Pwm_ref));
        Pwm_Off(TIM_CHANNEL_3);
      }
      else if ((theta_e_TBL>=146) && (theta_e_TBL<244))
      {
        Pwm_L_On(TIM_CHANNEL_1);
        Pwm_Off(TIM_CHANNEL_2);
        Pwm_H_On(TIM_CHANNEL_3, abs(Pwm_ref));
      }
      else if ((theta_e_TBL>=244) && (theta_e_TBL<341))
      {
        Pwm_Off(TIM_CHANNEL_1);
        Pwm_L_On(TIM_CHANNEL_2);
        Pwm_H_On(TIM_CHANNEL_3, abs(Pwm_ref));
      }
      else if ((theta_e_TBL>=341) && (theta_e_TBL<439))
      {
        Pwm_H_On(TIM_CHANNEL_1, abs(Pwm_ref));
        Pwm_L_On(TIM_CHANNEL_2);
        Pwm_Off(TIM_CHANNEL_3);
      }
      else if ((theta_e_TBL>=439) && (theta_e_TBL<536))
      {
        Pwm_H_On(TIM_CHANNEL_1, abs(Pwm_ref));
        Pwm_Off(TIM_CHANNEL_2);
        Pwm_L_On(TIM_CHANNEL_3);
      }
    }
    // {
    //   TIM1->CCR1 = Fu_ref + PWM_Offset;
    //   TIM1->CCR2 = Fv_ref + PWM_Offset;
    //   TIM1->CCR3 = Fw_ref + PWM_Offset;
    // }
  }
  else
  {
    {
      pulse_on = 0;
      on_time_cnt = 0;
      off_time_cnt = 0;
      on_off_state = 0;

      Tref_slop = 0;
      Tref_ramp = 0;
    }
    PWM_Offset = htim1.Init.Period / 2;
    PWM_HalfMax = PWM_Offset-0;

    Vqse_ref_It  = 0;
    Vqse_ref_fb  = 0;
    Vqse_ref_ff  = 0;
    Vqse_ref     = 0;
    Vqse_ref_out = 0;
    
    Vdse_ref_It  = 0;
    Vdse_ref_fb  = 0;
    Vdse_ref_ff  = 0;
    Vdse_ref     = 0;
    Vdse_ref_out = 0;

    w_cmd = 0;
    w_rad = 0;
    Tref = 0;
    Tref_i = 0;
    w_pi_out = 0;
    tq_cmd = 0.0;

    // TIM1->CCR1 = 0;
    // TIM1->CCR2 = 0;
    // TIM1->CCR3 = 0;
    Pwm_L_On(TIM_CHANNEL_1);
    Pwm_Off(TIM_CHANNEL_2);
    Pwm_Off(TIM_CHANNEL_3);
  }

	// 전류 제한 (Max: 61A)
	if(HAL_GetTick() > 5000)
	{
		i_current = adc_value_buf[2] * adc_conv_value;
		i_real = i_current * 22.727272;		// 22.727272 = 50A / 2.2V

		if (fABS(i_real) > Motor.OverCurrentLimit)
		{
			system_err = OVER_CURRENT;
		}
	}


	// 토크 ADC 값 요청
	if(torqueReadOn == 0)
	{
		HAL_SPI_Receive_DMA(&hspi2, Tq_Sensor_read_data, 3);

		torqueReadOn = 1;
	}
}


/**
 * @brief 토크 값에 필터를 적용한다.
 * 		- 입력 값을 계속해서 더한 후 예전 값을 1개씩 제거하여 평균을 구한다 
 * 		- 값이 반영되는데 걸리는 시간: FILTER 개수 * 함수 실행 주기
 * 
 * @param  input 	: 토크 AD 값 
 * @return average	: 필터적용 후 AD 값
 */
int32_t MovingAverage(int32_t input)
{
  static int32_t  data[MOV_BUF_FILTERSIZE];	// 필터 값 보다 커야함 -> 새로운 data 저장하는 index와 예전 data 빼려는 index가 겹치면 안된다.
	static uint32_t sumdata;					// Data의 합산	
  static uint16_t sumCount;					// 몇개의 Data가 더해졌는지 판단 (필터 개수까지 카운트)
	static uint16_t bufIndex;					// Buffer에 저장할 index
	static uint16_t subtractBufIndex;			// 뺄셈할 값의 index
	int32_t average = 0;						// 평균값

	// Data 반영
	data[bufIndex] = input;
	sumdata += data[bufIndex++];

	// 저장 Buffer index 관리
	if(bufIndex >= MOV_BUF_FILTERSIZE)
		bufIndex = 0;

	// 더한 개수 판단
	if(sumCount < MOV_FILTERSIZE)
		sumCount++;								// 개수 미달 -> count 증가
	else
		sumdata -= data[subtractBufIndex++];	// 개수 충족 -> 이전 값 1개 제거

	// 빼는 buffer Index 관리
	if(subtractBufIndex >= MOV_BUF_FILTERSIZE)
		subtractBufIndex = 0;

	// 평균 계산
	average = sumdata / MOV_FILTERSIZE;

	return average;		
}

void MakeTable(void)
{
  int j;

  for(j=0; j<585; j++)
  {
    sinTBL[j] = sin(6.283185307 / 585. * (float)j);
  }
}

// // 50us loop (TEST용 - PWM을 강제로 설정)
// void current_loop(void)
// {
// 	int32_t read_adc;

// 	int16_t i_current;
// 	float i_err_1;
	
// 	static uint16_t ReadADValueOrg[10];
// 	long checkValue[2];
// 	int32_t read_sign_value = 0;

// 	// check value.. if value is big difference it is not real data.. we need to erase not real data
// 	// checkValue[0] = Tq_SensorValue_Avg - Tq_SensorValue; 		// If too much away from the existing value from readings. (Sensor 값 통신버전)
// 	checkValue[0] = Tq_SensorValue_Avg - adc_value_buf[3]; 			// If too much away from the existing value from readings. (AD로 직접 Read 12bit: Max 4095)
// 	checkValue[1] = Tq_SensorValue - AD_OFFSET_VALUE;				// If the read value deviates too much from the reference value.

// 	if ((labs(checkValue[0]) > 500) && (labs(checkValue[1]) > 500))
// 	{
// 		Tq_SensorValue = Tq_SensorValue_Avg;
// 	}
	
// 	// filtering ad value
// 	// ReadADValueOrg[0] = ReadADValueOrg[1];
// 	// ReadADValueOrg[1] = ReadADValueOrg[2];
// 	// ReadADValueOrg[2] = ReadADValueOrg[3];
// 	// ReadADValueOrg[3] = adc_value_buf[3];
// 	// ReadADValueOrg[0] + ReadADValueOrg[1] + ReadADValueOrg[2] + ReadADValueOrg[3]) * 0.25

// 	// Tq_SensorValue_Avg = MovingAverage(Tq_SensorValue);

// 	read_sign_value = (int32_t)Tq_SensorValue_Avg - (int32_t)SensorAD_OffsetValue;
// 	if (read_sign_value >= AD_OFFSET_VALUE)
// 		TargetSign = POSITIVE; // +
// 	else
// 		TargetSign = NEGATIVE; // -

// 	RealTq_fromSensor_reflectedSettingUnit = 0;

// 	//Read current for A/D
// 	// read_adc = ADCRecBuff[0] >> 8;
// 	// adc_current_value[0] = (int16_t)(SaturaLH(read_adc, -32768, 32767));

// 	// i_current = (int16_t)adc_current_value[0];
// 	// i_real = ((float)i_current * Gain.hss_gain) - i_offset;

// 	// check motor current to protect the motor..
// 	// if (fABS(i_real) > Motor.OverCurrentLimit)
// 	// {
// 	// 	system_err = OVER_CURRENT;
// 	// }

// 	// tq comd에 대한 전류 계산.
// 	i_ref = tq_cmd;

// 	// current command filter
// 	//default 1000, TS_CUR는 면적 계산을 위해서..쓰임.  1차 필터..
// 	i_ref_flt = (20000.0 * (i_ref - i_ref_flt) * TS_CUR) + i_ref_flt;

// 	// tq_real_Nm 는 N.m
// 	tq_real_Nm = RealTq_fromSensor_reflectedSettingUnit * TorqueMetric[Para[UNIT].val][UNIT_Nm];

// 	// Current control by PI control /
// 	i_err_1 = i_err;
// 	i_err = i_ref_flt - tq_real_Nm;													 //오차 계산..
// 	i_pi_out = i_pi_out + ((Gain.i_kp + Gain.i_ki) * i_err) - (Gain.i_kp * i_err_1); //출력 전압 계산..

// 	if (i_pi_out < 0.0) // pwm 방향 계산..
// 	{
// 		Flag_L_F_control_current = 1; // fasten		생산용
// 	}
// 	else if (i_pi_out >= 0.0)
// 	{
// 		Flag_L_F_control_current = 0; // loosen		생산용
// 	}

// 	static uint32_t timeOld;
// 	static uint16_t speedAccTemp;

// 	if(Flag.Run)
// 	{
// 		if(timeOld == 0)
// 			timeOld = HAL_GetTick();

		
// 		// 감속
// 		if(HAL_GetTick() - timeOld > 1000)
// 		{
// 			speedAccTemp = (timeOld + 1000 + 500 - HAL_GetTick()) / 10;

// 			if(speedAccTemp == 0)
// 				speedAccTemp = 1;
// 			else if(speedAccTemp > 50)
// 				speedAccTemp = 0;
// 		}
// 		// 가속
// 		else
// 		{
// 			speedAccTemp = (HAL_GetTick() - timeOld) / 10;

// 			if(speedAccTemp == 0)
// 				speedAccTemp = 1;
// 			else if(speedAccTemp > 50)
// 				speedAccTemp = 50;
// 		}		

// 		PWM_Out = Para[9].val * speedAccTemp / 50;	// 가감속

// 	}
// 	else
// 	{
// 		timeOld = 0;
// 		PWM_Out = 0;
// 		speedAccTemp = 0;
// 	}

// 	// PWM_Out = fABS(i_pi_out);

// 	// SVPWM Max input value
// 	if (PWM_Out > MAX_PWM_SVPWM)
// 		PWM_Out = MAX_PWM_SVPWM;


// 	if (v_loop_cnt >= 4) // 250us velocity
// 	{
// 		v_loop_cnt = 0;
// 		Velocity_loop();
// 	}
// 	else
// 		v_loop_cnt++;

// }
