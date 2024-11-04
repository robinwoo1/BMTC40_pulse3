/**
  ******************************************************************************
  * @file    bldcControl.c
  * @brief   BLDC Control by HAL library
  * @note    SVPWM
  *          설명 -> Header file
  ******************************************************************************
  */

/* Includes ----------------------------------------------------------------*/
#include "BldcControl.h"
#include "velocity.h"
#include "variable.h"
#include "common.h"
#include "svpwm.h"
#include "main.h"
#include "bissSpiCommunicate.h"

/* Extern ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
// extern TIM_HandleTypeDef htim3;

/* Variable ----------------------------------------------------------------*/
uint32_t PWM_U;
uint32_t PWM_V;
uint32_t PWM_W;
BldcControl bldcControl;

/* Function ----------------------------------------------------------------*/
void BldcMotorInit(void)
{
    float angle;

    // Stop TR
    TIM1->CCR1 = 0;         // Duty cycle = 0%
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;         

    // Speed 보상 값 설정
    if(Motor.num_pulse)
    {
        bldcControl.stepNum_1turn = MOTOR_SETP_NUM;                                     // FIXME: 속도 보상에 따른 온도 상승 확인
        angle = 360.0 / 60.0 * SVPWM_LOOP_TIME;                                         // 스피드 1에서 100us 마다 회전 각도
        // bldcControl.speedCompensation = angle * bldcControl.stepNum_1turn * 1.6;        // 모터 스탭 각도 보상을 반영 (1속도 당 보상각도)
        bldcControl.speedCompensation = angle * bldcControl.stepNum_1turn * 1.3;        // 모터 스탭 각도 보상을 반영 (1속도 당 보상각도)
        bldcControl.motorPulse_motorStep = Motor.num_pulse / bldcControl.stepNum_1turn; // 1회전 모터 6step 당 모터 pulse 수
        bldcControl.invMotorPulse = (float)(1.0 / Motor.num_pulse);                     // Pulse의 역수
    }     
    else
    {
        // BLDC 설정 초기화 실패
        bldcControl.svpwmMaxPwm = 0;   
        bldcControl.motorPulse_motorStep = 0;    

        // Lock
        Flag.LockCommand = 1; 
    }   
}



/* 각도, Pulse 관련 -----------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  Z Pulse를 감지하는 함수 
 * 
 * @note   계속 실행되서 감지 실행, 정회전시 6번 Hall에 존재
 * @retval 1: 감지 o
 *         0: 감지 x
 */
uint8_t findZPulse(void)
{
    uint8_t retval = 0;

    // Z Pulse 감지 시 ON - LIMIT_SW_GPIO_Port 신호 제거 - BISS로 대체
    // if(HAL_GPIO_ReadPin(LIMIT_SW_GPIO_Port, LIMIT_SW_Pin) == GPIO_PIN_SET)
    // {
    //     retval = ON;
    // }

    return retval;
}


/**
 * @brief 초기 Hall sensor 변화를 통해 정확한 각도 위치를 찾는 함수 (현재는 초기 BISS 통신 여부 확인으로 사용 중)
 * 
 * @note  1. 찾은 경우: zeroTotalAngle에 반영된다.
 *        2. 못찾은 경우: Error(126) 발생, PWM 정지
 * 
 * */
void findZeroAngle(void)
{
    // Encoder 초기화가 안 되었을 때
    if(bldcControl.flagZeroAngleOn == OFF)
    {
        // // PWM 전송 종료 (드라이버가 고정되어 있는 것 해제)
        // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_1);
        // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_1);
        // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_2);
        // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_2);
        // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_3);
        // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_3);

        // Zero 위치 찾은지 확인 하는 Flag
        bldcControl.flagZeroAngleOn = OFF;

        // Absolute encoder 0 위치 값 반영 
        zeroTotalAngle = ABSOLUTE_ENCODER_DEFAULT;


        // Check 절대치 Encoder의 값 및 통신 (BISS 통신 정상 확인)
        if(bldcControl.absoluteAngle <= 4096 && bissSpi.commState)
            bldcControl.flagZeroAngleOn = ON;

        // Check BLDC setting
        if(Motor.num_pulse == 0 || bldcControl.motorPulse_motorStep == 0)
            bldcControl.flagZeroAngleOn = OFF;


        // // PWM Init
        // TIM1->CCR1 = 0;         // Duty cycle = 0%
        // TIM1->CCR2 = 0;
        // TIM1->CCR3 = 0;


        // // Check hall find error
        // if(bldcControl.flagZeroAngleOn == OFF && HAL_GetTick() > 5000)
        //     system_err = HALL_SENSOR_OPEN;


        // PWM 시작 여부 (zero 지점 찾았다면)
        if(bldcControl.flagZeroAngleOn)
        {
            // PWM START
            // HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_1);       
            // HAL_TIMEx_PWMN_Start(&htim1,  TIM_CHANNEL_1);
            // HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_2);
            // HAL_TIMEx_PWMN_Start(&htim1,  TIM_CHANNEL_2);
            // HAL_TIM_PWM_Start(&htim1,  TIM_CHANNEL_3);
            // HAL_TIMEx_PWMN_Start(&htim1,  TIM_CHANNEL_3);
        }
        else
        {
            // // PWM STOP
            // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_1);
            // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_1);
            // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_2);
            // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_2);
            // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_3);
            // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_3);

            // Clock 초기화
            // TIM1->CNT = 0;
        }
    }
}


/**
 * @brief Hall sensor 변화에 맞쳐 zero offset을 계산한다.
 * @note  1. hallsensor 변경 interrupt문에 있어야 정확하다.
 *        2. zeroTotalAngle 변수에 offset 값을 적용한다.
 * 
 * */
void zeroOffsetCalc(uint8_t hall)
{
    float zeroAngleOffset;                   // Zero 각도 위치 offset pulse

    // Init encoder count value
    // __HAL_TIM_SET_COUNTER(&htim4, 0);

    // Angle calc
    switch(hall)
    {
        case 1:
            zeroAngleOffset = 0;
        break;
        case 2:
            // (6STEP의 각도 / 1회전당 6step의 횟수) * (1회전 pulse / 360도) => 실제 각도 * 1도 당 pulse -> zero 각도 (pulse 단위)
            zeroAngleOffset = (120.0 / 360.0) * bldcControl.motorPulse_motorStep;   // 기존식: 120.0 / MOTOR_SETP_NUM * Motor.num_pulse / 360.0
        break;
        case 3:
            zeroAngleOffset = (60.0  / 360.0) * bldcControl.motorPulse_motorStep;   // 341.333
        break;
        case 4:
            zeroAngleOffset = (240.0 / 360.0) * bldcControl.motorPulse_motorStep;   // 1365.33
        break;
        case 5:
            zeroAngleOffset = (320.0 / 360.0) * bldcControl.motorPulse_motorStep;
        break;
        case 6:
            // zeroAngleOffset = (180.0 / 360.0) * bldcControl.motorPulse_motorStep;   // 1023.99 
            zeroAngleOffset = (180.0 / 360.0) * bldcControl.motorPulse_motorStep - 1;  // 1023.99  Z Pulse 오차
        break;
        default:
            zeroAngleOffset = 0;
        break;
    }

    // Init angle apply
    zeroTotalAngle = (int)zeroAngleOffset;
    
}


/**
 * @brief 에러 종류에 따라 Z Pulse값을 다시 찾아 초기화 해야할 경우 (현재 사용 안함 - BISS에서는 사용 X)
 * 
 * @param errorCode : 에러 코드
 * @param timA      : 인터럽트 발생 Timer
 * @param timB      : 인터럽트 발생 Timer
 */
void bldcResetZeroAngle(uint16_t errorCode, TIM_HandleTypeDef* timA, TIM_HandleTypeDef* timB)
{
    static uint16_t error[2];

    // Error 상태 저장
    error[0] = error[1];
    error[1] = errorCode;

    // Check reset해야할 에러 발생 여부
    if(error[0] != error[1] && error[1] == ENCODER_OPEN_ERROR)
    {
        // Motor 동작 정지
        TIM1->CCR1 = 0;
        TIM1->CCR2 = 0;
        TIM1->CCR3 = 0;
        // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_1);
        // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_1);
        // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_2);
        // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_2);
        // HAL_TIM_PWM_Stop(&htim1,  TIM_CHANNEL_3);
        // HAL_TIMEx_PWMN_Stop(&htim1,  TIM_CHANNEL_3);

        // // Interrupt 초기화
        // HAL_TIM_Base_Stop_IT(timA);
        // HAL_TIM_Base_Stop_IT(timB);

        // Z pulse 찾는 동작 다시 실행
        // findZeroAngle();

        // // Interrupt 재동작
        // HAL_TIM_Base_Start_IT(timA);
        // HAL_TIM_Base_Start_IT(timB);
    }
}




/* 모터 PWM -----------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief Motor의 채널 별로 PWM 반영
 * @note  Zero 각도를 찾았을 경우 반영.
 * 
 * */
void MotorSpeed(void)
{
    // Run  (Zero position ON)
    if (Flag.Run && bldcControl.flagZeroAngleOn)   
    {
        TIM1->CCR1 = PWM_U;
        TIM1->CCR2 = PWM_V;
        TIM1->CCR3 = PWM_W;
    }
    // Stop
    else if(bldcControl.flagZeroAngleOn)
    {
        TIM1->CCR1 = 0;
        TIM1->CCR2 = 0;                
        TIM1->CCR3 = 0;            
    }
}

