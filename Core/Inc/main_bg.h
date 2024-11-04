
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MAIN_BG_H
#define _MAIN_BG_H

/* Include -------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Struct --------------------------------------------------------------------*/


/* Functions ------------------------------------------------------------------*/
void main_init(void);
void background(void);

void Update_flash(void);
uint8_t flashWrite(uint32_t Addr, uint32_t* Buf, uint32_t count);

void ReceiveComData(void);
void checkUartState(void);
void SerialPutChar(uint16_t len, uint8_t* data);
void SerialPutChar_Oled(uint16_t len, uint8_t* data);
void WiFi_DataReceive(void);


void Init_ViriableFirst(void);
void DriverDefaultPara(void);           // Driver parameter default value


void Set_Torque_variable(void);
void Gain_Setting(void);
int Converting_Torque_Speed(int setno, float paravalue, int flag);
void Set_MotorEncoderType(void);
void Fine_i_offset(void);


uint8_t checkFasteningNgError(uint16_t errorCode);
void bitsocketLockControl(void);



void ReadWrite_DriverData();

void Power_down(void);
void PowerWakeUpJudge(void);



void torqueReadState(void);

void sleepTimeCheck(void);
void setOledMessageOn(void);


#endif 