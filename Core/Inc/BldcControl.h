
#ifndef __BLDCCONTROL_H
#define __BLDCCONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif 


/* Includes ---------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_tim.h"
#include "stm32f7xx_it.h"


/* Define ----------------------------------------------------------------*/
#define ABSOLUTE_ENCODER_DEFAULT    195     // 절대치 Encoder의 위치 offset     // (엔코더 해상도 / 6step / pole쌍 개수 = 4096 / 6 / 7 의 배수로 위치 찾음)

#define SVPWM_SLOPE_A               0.0002
#define SVPWM_SLOPE_B               -0.3333


/* Struct -------------------------------------------------------------------*/
typedef struct 
{
    uint8_t flagZeroAngleOn;                // Zero 위치 찾음 flag
    float   speedCompensation;              // Speed에 대한 보상 계수
    float   svpwmMaxPwm;                    // SVPWM 사용시 최대 입력 PWM
    float   motorPulse_motorStep;           // Motor.num_pulse / MOTOR_SETP_NUM  ( 1회전 모터 6스탭 당 모터 펄스 개수 )
    float   invMotorPulse;                  // 1 / Motor.num_pulse
    float   stepNum_1turn;                  // 1 회전에 6step 발생 횟수
    float   angleOffsetConst;
    float   angleOffsetVal;
    uint16_t absoluteAngle;                 // 절대치 Encoder
    uint16_t absoluteAngleOld;              // 이전 Angle 값
    uint16_t absoluteAngleOld2;             // 이전전 Angle 값

}BldcControl;


/* Function ---------------------------------------------------------------*/
/**
 * @note 함수 사용법
 *       1. BldcMotorInit
 *          BLDC 변수 초기화
 *          가장 먼저 실행, 초기 초기화할 때 1회 실행.
 * 
 *       2. findZeroAngle
 *          SVPWM 사용시 초기각 잡기 위해 사용, 1번 함수 다음에 사용
 *          초기 초기화할 때 1회 실행.
 * 
 *       3. zeroOffsetCalc
 *          초기각 계산, Hall sensor 변화 인터럽트에 설정
 * 
 *       4. MotorSpeed
 *          PWM 반영할 때 사용, SVPWM 계산 후 실제 반영을 위해 다음에 사용
 * 
 *       5. bldcResetZeroAngle (현재는 사용 X)
 *          에러 발생여부를 확인하고 Zero 각도를 다시 초기화 할 때 사용.
 *          주기적으로 실행.
 * 
 *       6. checkEncoderError
 *          엔코더 값을 보고 오류를 확인.
 *          1ms 단위로 함수 주지적 실행.
 * 
 *       7. pwmStartOnOff
 *          드라이버 동작에 따라 PWM 출력을 ON, OFF할지 결정
 *          velocity 에서 실행.
 * 
 * */

void BldcMotorInit(void);

// 각도, Pulse 관련
void findZeroAngle(void);
uint8_t findZPulse(void);
void zeroOffsetCalc(uint8_t hall);
void bldcResetZeroAngle(uint16_t errorCode, TIM_HandleTypeDef* timA, TIM_HandleTypeDef* timB);

// Motor
void MotorSpeed(void);
void Motor6Step(uint16_t pwm);

// PWM 제어
void pwmStartOnOff(uint8_t run);


/* Variables ---------------------------------------------------------------*/
extern uint32_t PWM_U;
extern uint32_t PWM_V;
extern uint32_t PWM_W;
extern BldcControl bldcControl;






#ifdef __cplusplus
}
#endif

#endif
