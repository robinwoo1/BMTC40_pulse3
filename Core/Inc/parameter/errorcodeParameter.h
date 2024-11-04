/**
  ******************************************************************************
  * @file    errorCodeSave.h
  * @author  jun
  * @brief   Header file of error code save.
  ******************************************************************************
  * @version
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 
  * - .c 파일
  * errorCodeProcess()
  * 주기적으로 실행
  * 
  * addErrorCodeList()
  * 에러 코드를 추가적으로 저장하고자할 때 실행
  * 
  * removeErrorCodeList()
  * 현재 저장되어있는 에러 코드를 제거할 때 사용
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ERROR_CODE_PARAMETER_H
#define __ERROR_CODE_PARAMETER_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/

#define ERROR_CODE_INDEX_SIZE                   32           // 에러 리스트 저장 개수

#define ERROR_CODE_MEMORY_START_ADDRESS         0x0C02000



/* Enums ---------------------------------------------------------------------*/


/* Struct ---------------------------------------------------------------------*/

// 메모리에 저장될 에러 코드 구격
typedef struct 
{
    uint32_t    code;
    uint16_t    dummy[2];
    uint16_t    time[4];                // High: Year, Month, day / Low: Hour, minute, second

} errorCodeStruct;


typedef struct
{
    uint8_t     saveCommand;
    uint8_t     readCommand;
} errorCodeProcessStruct;


 



/* Extern -----------------------------------------------------------------------*/

extern errorCodeStruct errorCodeList[ERROR_CODE_INDEX_SIZE]; 




/* Functions --------------------------------------------------------------------*/

void initErrorCodeList(void);
void errorCodeProcess(void);

void addErrorCodeList(uint16_t code);
void removeErrorCodeList(uint8_t index);

void saveErrorCodeMemory(void);
void readErrorCodeMemory(void);





#ifdef __cplusplus
    }
#endif

#endif





