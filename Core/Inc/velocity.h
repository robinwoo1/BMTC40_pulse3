

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _VELOCITY_H
#define _VELOCITY_H



/* Includes ------------------------------------------------------------------*/

#include "stm32f7xx_hal.h"



/* Struct -----------------------------------------------------------------*/

typedef struct 
{
    uint8_t     flStatusOld;

}multiSequenceStruct;



/* variables -----------------------------------------------------------------*/

extern multiSequenceStruct multiStr;




/* functions -----------------------------------------------------------------*/

void Velocity_loop(void);
void velocity(float input);
void M_cnt(void);
long Check_counter(long cnt);
float square_acc(float Vin);

void Fastening(void);
void Loosening(void);
void fasten_loosen_control(void);
void Initial_Fastening_Parameter_After_ReverseTq(void);
void Initial_Fastening_Parameter(void);
void MinMax_Angle_process(void);
void ChangeSpeedAcc(void);
void After_fasten_init_process(void);
void ScrewCount_process(void);
void screwCountLooseningD1(void);

int  find_reach_initial_speed(void);
float SelectDisplayData(long data_num);
float Reverse_TorqueConversion(float cc, int flag);

void NoSpeedUp_ErrorCheck(void);
void Over_Speed_Check();

void ms_process(void);
void ms_countValueA(int count);
void ms_subIfA(void);
void ms_jump(int step);
void ms_loosening(int angle);
void ms_delay(int time);
void ms_start();
void ms_fastening(int no);
void ms_changeTorque(int no);
void ms_end();
void ms_MakeRealtimeAtStop(void);


#endif

