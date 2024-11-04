/**
  ***********************************************************************************
  * @file    svpwm.h
  * @version V1.0
  * @date    May-2021
  * @brief   This file contains the type definition of data structure and function
  *	     prototypes for implementation the SVPWM - Space-Vector Pulse Width 
  *	     Modulation
  */

/* Define to prevent recursive inclusion ------------------------------------------*/
#ifndef __SVPWM_H__
#define __SVPWM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>
#include "stm32f7xx_hal.h"

/* Exported types -----------------------------------------------------------------*/

/* Exported constants -------------------------------------------------------------*/
extern int zeroTotalAngle, zeroTotalAngleInit;    // Motor의 위치 각도

/* Defines ------------------------------------------------------------------------*/
// Motor 및 PWM 최대 값 설정
#define MOTOR_SETP_NUM          7.0       // 14pole / 2: 1회 회전당 6step의 발생횟수 (Motor.stepNum_1turn)
#define MAX_PWM_INDEX           599
#define MAX_PWM_SVPWM           5300      // SVPWM에 반영하는 최대 PWM 값

#define PWM_MAX                 10000     // 실제 출력으로 내보내는 PWM 최대치
#define PWM_HALF                5400      // PWM 최대치의 절반 (SVPWM 계산 시 사용)

#define SVPWM_LOOP_TIME         0.0001    // 100us


/* Exported macro -----------------------------------------------------------------*/

/* Exported functions -------------------------------------------------------------*/
void svpwmCalc(float angle_el, float pwm);
void svpwmPositionApply(void);
void svpwmInitPositionApply(int32_t angle, uint16_t pwm);


// PWM Table
void getPwmTable(float* angle, float* pwm);

// PWM reverse Table 
void getReversePwmTable(float* angle, float* pwm);


/* SVPWM Duty Cycles calculation function prototype ********************************/


#ifdef __cplusplus
}
#endif

#endif /* __SVPWM_H__ */
