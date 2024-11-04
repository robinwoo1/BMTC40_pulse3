
/**
 * @file  SWPWM.c
 * @brief BLDC 제어 시 각도에 따라 벡터 힘으로 제어
 * 
 * @note  제어 조건
 *        1. 빠른 interrupt (고속 회전시 실시간으로 각도에 맞게 적용해줘야 하기 때문에 빠른 interrupt 필요 - 현재: 11.11us)
 *        2. 초기 각도 설정  (초기 각도 설정할 때 다른 것에 영향 받지 않도록 홀센서 인터럽트 우선순위 최상위)
 * 
 * @note  순서
 *        1.BLDC 초기각 찾기  2. 위치에 따른 초기화(Hall을 기준)   3. SVWPM 인터럽트 ON
 * 
 * @note  방법
 *        1. 현재 각도를 구한다.
 *        2. 360도 를 6구간으로 나누어 60도 마다 Table에서 값을 본다.
 *        3. Table의 인덱스는 0.1도 단위로 되어있고 현재각도 * 10 (0.1도까지) % 600 이 인덱스의 번호가 된다.
 * 
 * @note  소모 시간
 *        1. (이전 버전) svpwmCalc 전까지 0.6us  2. svpwmCalc 1.6us  3. 그외 0.9us   대략 총: 3.1us (X)
 *        2. svpwmCalc 계산 수정 버전: 총 소모시간 2.3us (X)
 *        3. D Cache 활성화: 소모시간 1.4us
 */

/* Includes -----------------------------------------------------------------------*/
#include "svpwm.h"
#include "variable.h"
#include "BldcControl.h"
#include "velocity.h"
#include "stdlib.h"
#include "main.h"
#include "svpwmTable.h"
#include "bissSpiCommunicate.h"

/* Variables ----------------------------------------------------------------------*/
static float Ua;
static float Ub;
static float Uc;


int zeroTotalAngle, zeroTotalAngleInit;         // Motor 위치 각도



/* Private typedef ----------------------------------------------------------------*/
// SVPWM U, V, W gain값 계산
void svpwmCalc(float angle_el, float pwm)
{
    int8_t tempCalc;

	angle_el = angle_el * bldcControl.stepNum_1turn;       // 6STEP이 한 회전에 7번

    // 토크 높을 때는 보상을 막기 위해 제한
    // if(w_rpm_abs > 1150)
    // {
    //     // 속도에 따른 위치 보상 반영
    //     if(Flag_L_F_control_current == 0)
    //         angle_el = angle_el - bldcControl.speedCompensation * w_rpm_abs;
    //     else
    //         angle_el = angle_el + bldcControl.speedCompensation * w_rpm_abs;
    // }
    // else
    // {
    //     // 속도에 따른 위치 보상 반영 (현재 PWM량에 따라 비례하여 90도 각도까지 조절 = 360/7/4 = 12.85)
    //     if(Flag_L_F_control_current == 0)
    //         angle_el = angle_el - 12.85 * (PWM_Out / MAX_PWM_SVPWM) * screw_type;
    //     else
    //         angle_el = angle_el + 12.85 * (PWM_Out / MAX_PWM_SVPWM) * screw_type;
    // }

    
        // 속도에 따른 위치 보상 반영 (현재 PWM량에 따라 비례하여 90도 각도까지 조절 = 360/7/4 = 12.85)
        if(Flag_L_F_control_current == 0)
            angle_el = angle_el - (12.85 + bldcControl.speedCompensation * w_rpm_abs);
        else
            angle_el = angle_el + (12.85 + bldcControl.speedCompensation * w_rpm_abs);
    

    // 360 초과 값 제거, (-)값 제거 (Degree단위)
    tempCalc = (int8_t)(angle_el / 360.0);
    angle_el -= tempCalc * 360.0;
    angle_el = angle_el >= 0 ? angle_el : (angle_el + 360.0);

    // 정방향
    if(Flag_L_F_control_current == 0)
    {
        getPwmTable(&angle_el, &pwm);
    }
    else    // 역방향 (정방향에서 180도 차이)
    {
        getReversePwmTable(&angle_el, &pwm);
    }
}


/**
 * @brief PWM Table을 이용하여 정방향 U, V, W 의 PWM 값을 계산한다.
 * 
 * @param angle: 360도 이하의 값
 * @param pwm  : PWM의 세기
 * 
 * */
void getPwmTable(float* angle, float* pwm)
{
    uint16_t index;

    // Angle change to table index
    index = (uint16_t)(*angle * 10);

    // Get U, V, W PWM 
    Ua = PWM_HALF + tablePwmU[index] * *pwm;
    Ub = PWM_HALF + tablePwmV[index] * *pwm;
    Uc = PWM_HALF + tablePwmW[index] * *pwm;
}


/**
 * @brief PWM Table을 이용하여 역방향 U, V, W 의 PWM 값을 계산한다.
 * 
 * @param angle: 360도 이하의 값
 * @param pwm  : PWM의 세기
 * 
 * */
void getReversePwmTable(float* angle, float* pwm)
{
    uint16_t index;

    // Angle change to table index
    index = (uint16_t)(*angle * 10);

    // Get U, V, W PWM 
    Ua = PWM_HALF + tablePwmReverseU[index] * *pwm;
    Ub = PWM_HALF + tablePwmReverseV[index] * *pwm;
    Uc = PWM_HALF + tablePwmReverseW[index] * *pwm;
}


/**
 * @brief SVPWM PWM 반영하는 function 
 * 
 * @note  1. 정밀하게 PWM을 반영하기 위해 별도로 빠른 LOOP에서 실행
 * 		  2. Speed등을 다른 곳에서 계산하고 PWM만 위치에 맞게 반영한다.
 * 		  3. 100us 주기 - 2.3us 소모
 */
void svpwmPositionApply(void)
{
	// SVPWM의 입력 PWM 제한
	if(PWM_Out > MAX_PWM_SVPWM)
		PWM_Out = MAX_PWM_SVPWM;

	// Clac SVPWM PWM
	svpwmCalc(((float)(bldcControl.absoluteAngle + zeroTotalAngle) / 4096.0) * 360.0, PWM_Out);
	
	// U V W Limit
	PWM_U = (uint32_t)Ua;
	if(PWM_U > PWM_Maxvalue)
    {
		PWM_U = PWM_Maxvalue;
    }

    if(Ua < 0)
    {
        PWM_U = 0;
    }

	PWM_V = (uint32_t)Ub;
	if(PWM_V > PWM_Maxvalue)
    {
		PWM_V = PWM_Maxvalue;
    }

    if(Ub < 0)
    {
        PWM_V = 0;
    }

	PWM_W = (uint32_t)Uc;
	if(PWM_W > PWM_Maxvalue)
    {
		PWM_W = PWM_Maxvalue;
    }

    if(Uc < 0)
    {
        PWM_W = 0;
    }

	MotorSpeed();		// running motor

    // 현재 반영했던 각도 저장
    bldcControl.absoluteAngleOld2 = bldcControl.absoluteAngleOld;
    bldcControl.absoluteAngleOld = bldcControl.absoluteAngle;
}


/**
 * @brief 초기 각을 잡기 위해 SVPWM pwm 반영하는 function 
 * 
 * @note  초기에는 임의에 angle을 넣다가 각도를 찾은 후 encoder에 맞게 증가된 angle을 반영한다.
 * @param angle: 현재 각도를 의미
 * @param pwm:   속도에 세기를 조절
 */
void svpwmInitPositionApply(int32_t angle, uint16_t pwm)
{
    // Angle 반영
    zeroTotalAngleInit = angle;

	// SVPWM의 입력 PWM 제한
	if(pwm > 500)
		pwm = 500;

	// SVPWM angle overflow 초기화
	if(Motor.num_pulse != 0)
	{
		if(zeroTotalAngleInit < -Motor.num_pulse)
			zeroTotalAngleInit += Motor.num_pulse;
		else if(zeroTotalAngleInit > Motor.num_pulse)
			zeroTotalAngleInit -= Motor.num_pulse;
	}
    else
    {
    	pwm = 0;
    }

	
	// Calcultaion 필요없을 때 return (모든 PWM이 0일 때)
	if(TIM1->CCR1 == 0 && TIM1->CCR2 == 0 && TIM1->CCR3 == 0 && pwm == 0)
		return;

	// Clac SVPWM PWM
	svpwmCalc(((float)zeroTotalAngleInit * bldcControl.invMotorPulse) * 360.0, pwm);
	
	// U V W Limit
	PWM_U = (uint32_t)Ua;
	if(PWM_U > PWM_Maxvalue)
		PWM_U = PWM_Maxvalue;

    if(Ua < 0)
    {
        PWM_U = 0;
    }

	PWM_V = (uint32_t)Ub;
	if(PWM_V > PWM_Maxvalue)
		PWM_V = PWM_Maxvalue;

    if(Ub < 0)
    {
        PWM_V = 0;
    }

	PWM_W = (uint32_t)Uc;
	if(PWM_W > PWM_Maxvalue)
		PWM_W = PWM_Maxvalue;
    if(Uc < 0)
    {
        PWM_W = 0;
    }

	// running motor
    TIM1->CCR1 = PWM_U;
    TIM1->CCR2 = PWM_V;
    TIM1->CCR3 = PWM_W;
	
}

