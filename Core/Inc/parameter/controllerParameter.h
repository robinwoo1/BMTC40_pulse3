/**
  ******************************************************************************
  * @file    controllerParameter.h
  * @author  jun
  * @brief   Header file of controller에 관한 정보 관리.
  ******************************************************************************
  * @version
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 
  * - .c 파일
  * controllerParaProcess()
  * 1회 초기화 실행,
  * 주기적으로 실행
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROLLER_PARAMETER_H
#define __CONTROLLER_PARAMETER_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/

// MAC ADDRESS를 메모리에 저장하여 사용할 것인지 결정
#define CONTROLLER_PARA_MAC_ADDRESS_ACTIVE      0


// FIXME: 전용으로 만듬
#define CONTROLLER_MODEL_NAME_HANTAS            "BMT40"
#define CONTROLLER_MODEL_NAME_MOUNTZ            "EPT70"



/* Enums ---------------------------------------------------------------------*/


/* Struct ---------------------------------------------------------------------*/

// 메모리에 저장될 컨트롤러 관한 정보
typedef struct 
{
    uint16_t    model;
    uint16_t    modelName[16];
    uint16_t    serialNumber[5];
    uint16_t    productDate[2];
    uint16_t    macAddress[3];
    uint8_t     firmwareVersion[3];

} controllerParaStruct;


typedef struct
{
    uint8_t     saveCommand;
    uint8_t     readCommand;
} contorllerParaProcessStruct;


 



/* Extern -----------------------------------------------------------------------*/

extern controllerParaStruct controllerPara; 
extern contorllerParaProcessStruct controllerParaCommand;



/* Functions --------------------------------------------------------------------*/

void controllerParaProcess(void);

void saveControllerParaMemory(void);
void readControllerParaMemory(void);





#ifdef __cplusplus
    }
#endif

#endif





