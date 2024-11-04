
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DISPLAY_H
#define _DISPLAY_H


//***************** OLED COM DEFINE ********************* 
#define OLED_HEAD0 0x5A
#define OLED_HEAD1 0xA5   
#define Fill_OLED 0x90  
#define DrawStringXY 0x91
#define DrawFillRectXY 0x92
#define DrawRectXY 0x93
#define DisplayBattery 0x94
#define DisplayAntena 0x95
#define ControlLED 0x99


#define BLACK 1
#define WHITE 2
#define RED   3	
#define GREEN 4	
#define BLUE  5	
#define YELLOW   6	


#define FONT_8PT 	0
#define FONT_10PT	1
#define FONT_12PT	2
#define FONT_16PT	3

#define GREEN_LED   1
#define RED_LED     2
#define GREEN_RED_LED   3


#define DISPLAY_SHIFT_INT           0
#define DISPLAY_SHIFT_FLOAT         1

#include "stm32f7xx_hal.h"


void display_process(void);
int  ErrorNo_Conversion(int err);
void key_process(void);

void fillOLED(uint8_t color);
void drawStringXY(uint8_t x, uint8_t y, uint8_t size, uint8_t fontCol, uint8_t bgCol, uint8_t length, char* str);
void drawFillRectXY(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey, uint8_t thick, uint8_t lineCol, uint8_t fillCol);
void drawRectXY(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey, uint8_t thick, uint8_t color);
void displayBattery(uint8_t level, uint8_t mode);
void displayAntena(uint8_t level, uint8_t mode);
void Control_LED(void);

void Display_SetTorque(uint8_t disp);
void DisPlay_CurrentTorque(uint8_t disp, float value);
void Display_SetSpeed(uint8_t disp);
void DisPlay_CurrentSpeed(uint8_t disp, float value);
void Display_PresetNo(uint8_t disp);
void Display_CurrentRPM(uint8_t disp, float rpm, float ratio);
void Display_CurrentShift(uint8_t disp, float shift, uint8_t type);
void Display_Angle(uint8_t disp);
void Display_Count(uint8_t disp);
void Send_Oled_Data(void);

#endif 