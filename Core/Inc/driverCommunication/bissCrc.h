/**
  ******************************************************************************
  * @file    bissCrc.h
  * @author  jun
  * @brief   Header file of CRC Calculation.
  ******************************************************************************
  * @version
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BISS_CRC_H
#define BISS_CRC_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Include --------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"


/* Defines --------------------------------------------------------------------*/
#define BISS_MESSAGE_LENGTH               17        // Maxon motor BISS 엔코더 메시지 길이


/* Struct ---------------------------------------------------------------------*/


/* Global vriables ------------------------------------------------------------*/




/* Funcions --------------------------------------------------------------------*/
void makeCrc6Table(void);
void makeCrc8Table(void);
uint8_t makeCrc6FromTable(uint8_t* data, uint8_t length);

uint8_t makeCrcDirect(uint8_t* msgBuf);



#ifdef __cplusplus
    }
#endif

#endif
