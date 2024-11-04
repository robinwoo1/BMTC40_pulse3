/**
  ******************************************************************************
  * File Name          : tdc.c
  * Description        : tdc program body
  ******************************************************************************
*/
#include "torque.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "variable.h"

/* Public variable -----------------------------------------------------------*/

STATE_T         state;
float tqSlopeOffset;
float tqOffsetRange;

const float  TorqueMetric[NO_TORQUE_UNIT][NO_TORQUE_UNIT] =
{
    /*             Kgf.cm       Kgf.m        N.m            cN.m        Lbf.in       Ozf.in      Lbf.ft     */
    /* Kgf.cm */ { 1.0,         0.01,        0.0980665,     9.80665,    0.8679617,   13.88739,   0.0723301 },
    /* Kgf.m  */ { 100.0,       1.0,         9.806650,      980.665,    86.79617,    1388.739,   7.23301 },
    /* N.m    */ { 10.19716,    0.1019716,   1.0,           100.0,      8.850746,    141.6119,   0.7375622 },
    /* cN.m   */ { 0.1019716,   0.0010197,   0.01,          1.0,        0.0885075,   1.416119,   0.007376 },
    /* Lbf.in */ { 1.152125,    0.0115213,   0.1129848,     11.29848,   1.0,         16.0,       0.0833333 },    
    /* Ozf.in */ { 0.0720078,   0.0007201,   0.00706,       0.7061552,  0.0625,      1.0,        0.0052083 },
    /* Lbf.ft */ { 13.82549,    0.1382549,   1.355818,      135.5818,   12.0,        192.0,      1.0 }
};



const float SensorData[DRV_MODEL_SIZE][6] =
{
	// 공식 : y = ax + b (직선)
	// 기울기의 값은 offset 32767를 기준으로 작성됨.

	// // unit		max_torque		a			b	
	// {UNIT_Kgfcm,	 0,			0,			0,					0,			0},									// no use
	// {UNIT_Kgfcm,	15,			0,			0,					0,			0},									//1  BMT4001		no use
	// {UNIT_Kgfcm,	25,			0,			0,					0,			0},									//2  BMT4002		no use

	// {UNIT_Kgfcm,	70,			0.00574,	-187.9048,			0,			0},							//3  BMT4004			 
	// {UNIT_Kgfcm,	100,		0.005173,	-169.5135,			0,			0},							//4  BMT4006		// TODO: 04, 06, 24 수정 완료

	// {UNIT_Kgfcm,	120,		0.0883,		-175.6807,			0,			0},							//5  BMT4011		
	// {UNIT_Kgfcm,	165,		0.08303,	-164.5532,			0,			0},							//6  BMT4016			
	// {UNIT_Kgfcm,	205,		0.12722,	-247.9470,			0,			0},							//7  BMT4020	

	// // {UNIT_Kgfcm,	390,		-0.0324344,	1044.85153,			0,			0},						//8  BMT4024		// AD값이 작아질 때 토크 발생 -> (-)기울기 발생
	// {UNIT_Kgfcm,	390,		-0.0324344,	1044.85153,			0,			0},						//8  BMT4024		// AD값이 작아질 때 토크 발생 -> (-)기울기 발생
	// {UNIT_Kgfcm,	330,		0.034535,	-1131.592,			0,			0},							//9  BMT4026  
	// {UNIT_Kgfcm,	390,		0.034535,	-1131.592,			0,			0},							//10 BMT4045		
	// {UNIT_Kgfcm,	510,		0.0348,		-1140.8,			0,			0},							//11 BMT4064		




	// 공식 : y = ax3 + bx2 + cx + d (다항식)
	// unit		max_torque		d				c					b			a	
	{UNIT_Kgfcm,	 0,			0,				0,					0,			0},						// no use
	{UNIT_Kgfcm,	15,			0,				0,					0,			0},						//1  BMT4001		no use
	{UNIT_Kgfcm,	25,			0,				0,					0,			0},						//2  BMT4002		no use

	{UNIT_Kgfcm,	70,			-187.9048,		0.00574,			0,			0},						//3  BMT4004			 
	{UNIT_Kgfcm,	100,		-169.5135,		0.005173,			0,			0},						//4  BMT4006		// TODO: 04, 06, 24 수정 완료

	{UNIT_Kgfcm,	120,		-175.6807,		0.0883,				0,			0},						//5  BMT4011		
	{UNIT_Kgfcm,	165,		344.4414,		-1.0521e-2,			0,			0},						//4  BMT4016		// TODO: 04, 06, 24 수정 완료
	// {UNIT_Kgfcm,	165,		-164.5532,		0.08303,			0,			0},						//6  BMT4016			
	{UNIT_Kgfcm,	205,		-247.9470,		0.12722,			0,			0},						//7  BMT4020	

	{UNIT_Kgfcm,	390,		1331.634129,	-0.05499662,		0.00000044,	0},						//8  BMT4024		// AD값이 작아질 때 토크 발생 -> (-)기울기 발생
	{UNIT_Kgfcm,	330,		-1131.592,		0.034535,			0,			0},						//9  BMT4026  
	{UNIT_Kgfcm,	390,		-1131.592,		0.034535,			0,			0},						//10 BMT4045		
	{UNIT_Kgfcm,	510,		-1140.8,		0.0348,				0,			0},						//11 BMT4064		
};

float convertToTorque(uint32_t adc)
{
    float torque = 0.0;

	// // Torque 값 계산
	// torque = (SensorData[Para[DRIVER_MODEL].val][2] * (float)adc) +  SensorData[Para[DRIVER_MODEL].val][3];



	// Torque 값 계산						bx2 + cx + d	
	torque = (SensorData[Para[DRIVER_MODEL].val][4] * adc * adc) + (SensorData[Para[DRIVER_MODEL].val][3] * adc) +  SensorData[Para[DRIVER_MODEL].val][2];



    // Convert unit
    torque = torque * TorqueMetric[(uint8_t)SensorData[Para[DRIVER_MODEL].val][CAL_UNIT]][Para[UNIT].val];

    return torque;
}
