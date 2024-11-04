#ifndef __ADVANCED_PLUS_H
#define __ADVANCED_PLUS_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"


/* Defines ------------------------------------------------------------------*/



/* Struct -----------------------------------------------------------------*/
typedef struct 
{
	// Auto seating setting para
	float autoSeatDetectActive;			// Auto seating 활성화
	float set_gradient_Nm;				// Seating 감지를 위한 토크
	float min_seating_torque;
	float max_seating_torque;
	float min_seating_angle;
	float max_seating_angle;

	// cal para and monitoring data 
    float  	 adc[100];  	            // 100us .. 10ms  100개 필요
    float 	 mpulse[100];  	            // 100us .. 10ms  100개 필요
    uint16_t sdx;
	uint16_t sdxOld;
    float 	 finalGradient;				// 최종 Gradient 값
	float 	 currentGradient_Nm;		// 현재 계산된 Gradient 값
    float 	 finalGradientTq_Nm;		// 최종 Gradient 토크

	// Gradient의 평균값
	float	averageAdc;
	float	averageMpulse;

}atseating;

typedef struct 
{
	float clamp_torque_Nm;
	float max_clamp_torque;
	float min_clamp_torque;
	float replace_target_active;					// Clamp 토크로 목표 토크 대체 활성화

	float min_angle_after_seating;
	float max_angle_after_seating;

	float compensation_torque_Nm;					// seating tq + clamp tq = 최종 목표 토크
	
}clamping;

typedef struct 
{
	// setting para
	float snugTorque_Nm;							// Prevailing 시작 감지 토크
	float startAngle;								// Prevailing snug torque 이후 측정을 시작할 각도
	float targetAngle;								// Prevailing 측정할 구간 Angle

	float minTorqueInSection_Nm;					// Prevailing 구간 내에서 한계 토크
	float maxTorqueInSection_Nm;

	float min_prevailing_torque;					// 구해진 Prevailing 토크의 한계 토크
	float max_prevailing_torque;

	float compensationActive;						// Prevailing 토크 보상 활성화

	float result_of_prevailing_torque_Nm;			// 최종 Prevailing 평균 토크
	float compensation_torque_Nm;					// Prevailing + clamp tq = 최종 목표 토크

	uint8_t  finished_prevailing;
    float 	 sum_tq;
    uint32_t cnt_loop;
	uint32_t detectAngle;
		
}prevailingtq;

typedef struct 
{
	float    Min_Torque_Nm;
	float    Max_Torque_Nm;
    float    Speed;
    float    Finish_Torque_Nm;
    float  	 AngleStartFromThreadTap;
	float 	 AngleLimitFromMinTorque;
	uint32_t ThreadStartAngle;
    float 	 SpeedAcc;
}threadTap;

typedef struct 
{
    float 	Speed;
    float 	Torque;
    float 	AngleLimit;
    float 	TimeLimit;
	float 	EngagingAfterAngleClear;
    float 	SpeedAcc;
}engagingtq;


typedef struct 
{
    float Speed;
    long Angle;
    // uint16_t UsingPreset;
    uint8_t Direction;
    float SpeedAcc;
}angleAfterTq;


typedef struct 
{
    float seating;					// Seating TQ
    float clamp;					// Clamp TQ
    float snug;						// Snug 토크
	float prevailing;				// Prevailing TQ
    float fastenComplete;			// 체결 완료 토크 
    float fastenComplete_dsp_comp;	// 체결 완료 토크 - Display 용 - 토크 효율 보상이 반영되어 있다.
}torque_cnv;

typedef struct
{
	float runDown_min_torque_Nm;			// Rundown 중 min 토크
	float runDown_max_torque_Nm;			// Rundown 중 max 토크
	float runDown_inspect_start_angle;		// Rundown의 감지 시작 angle
	float runDown_inspect_end_angle;		// Rundown의 감지 종료 angle

}rundownTq;


/* Variable ------------------------------------------------------------------*/
extern atseating 	AtSeating;
extern clamping 	Clamping;
extern prevailingtq Prevailing;
extern threadTap 	ThreadTap;
extern engagingtq 	EngagingTQ;
extern angleAfterTq AngleAfterTqUp;
extern rundownTq 	RundownTq;

extern torque_cnv 	Torque_cnv;

extern uint16_t 	AdvanceMode;



/* functions -----------------------------------------------------------------*/

// Advanced 파라미터 수정 관련

void setAdvancedParameter(uint8_t presetNum, float FasteningTargetTorque, float DefaultAcc_time);
void setAdvancedCompensation(uint16_t advancedMode, float compensation);
void setAdvancedFromModeChange(uint16_t address, uint16_t newData, uint16_t oldData);



// Advanced 초기화 동작

void Init_Para_limit_forAdvence(uint8_t adv_Pset_number, float tq_upper_limit_unit, uint16_t speed_upper_limit);
void write_init_advanced_para_to_memory(uint8_t pno);
void initAdvancedParameter(uint8_t pNum);



// Advanced 동작 관련

void check_prevailing(uint8_t step, uint32_t currentAngle, float torqueNm);
void check_tq_during_prevailing(float torqueNm);
float advancedTargetTorque(uint8_t preset);



// Seating관련 min, max 확인

void check_seating_error_max(float tq, long angle);
void check_seating_error_min(float tq, long angle);
void check_afterSeating_error_max(long angle);
void check_afterSeating_error_min(long angle);
void runDownTorqueMinMaxLimit(float angle, float torque);


// Auto seating 감지를 위한 토크, 각도 평균 계산

void calcGradient(uint8_t step, float torqueNm, float angle);
void check_gradient_and_min(float torqueNm, float gradient, long angle);
float gradientAdcAverage(float input);
float gradientMpulseAverage(float input);





#ifdef __cplusplus
    }
#endif

#endif
