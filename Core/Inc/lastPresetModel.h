/**
  ******************************************************************************
  * @file    lastPresetModel.h
  * @author  jun
  * @brief   Header file of last preset, model operation process.
  ******************************************************************************
  * @version
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 
  * 
  * - .C 파일
  * 1. lastPresetModelProcess()
  *    주기적으로 실행
  * 
  * 2. getLastPresetModel()
  *    처음 초기화 과정에서 실행
  * 
  * 3. setBootModelStep()
  *    모델 설정 불러오는 곳에서 실행
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LAST_PRESET_MODEL_H
#define __LAST_PRESET_MODEL_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/

#define LAST_MODEL_ACTIVE                       0               // Model 기능 사용하는지 체크




/* Enums ---------------------------------------------------------------------*/


/* Struct ---------------------------------------------------------------------*/
typedef struct {
  uint8_t   preset;
  uint8_t   modelStep;
  uint16_t  modelScrew;

} lastPresetModelStruct;
 


/* Extern -----------------------------------------------------------------------*/
extern lastPresetModelStruct lastPreset;




/* Functions --------------------------------------------------------------------*/

void lastPresetModelProcess(void);

void getLastPresetModel(void);

void checkChangePreset(void);
void checkChangeModelStep(void);
void setBootModelStep(uint16_t* function, uint16_t* data1, uint16_t* data2, uint16_t* screwCount);







#ifdef __cplusplus
    }
#endif

#endif





