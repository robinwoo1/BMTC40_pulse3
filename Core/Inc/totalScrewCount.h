/**
  ******************************************************************************
  * @file    totalScrewCount.h
  * @author  jun
  * @brief   Header file of Total screw count.
  ******************************************************************************
  * @version
  * 2022.01.18 - Total screw count 정의
  * 
  * 
  * @note 동작
  * 1. Total screw값이 Limit 값을 넘을 때 에러 발생.
  * 2. Limit 값을 설정한 이후부터 에러 발생.
  * 3. 초기 설정값 읽기 실패할 경우 Total screw 증가 및 에러 발생 X.
  * 
  * 
  * @note 사용버
  * - .h 파일
  * TOTAL_SCREW_ADDRESS - ADDRESS 주소 변경시 재설정
  * 
  * - .c 파일
  * 1. initTotalScrewCount()
  *    초기 1회 실행
  * 
  * 2. totalScrewProcess()
  *    주기적으로 반복 실행
  * 
  * 3. getTotalScrewCount()
  *    모드버스 통신에서 screw 값 요청 시 실행
  * 
  * 4. setTotalScrewCount()
  *    모드버스 통신에서 screw 값 설정 시 실행
  * 
  * + display에 에러코드 추가, Auto reset에 error 초기화 예외 설정 필요.
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TOTAL_SCREW_COUNT_H
#define TOTAL_SCREW_COUNT_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Include --------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"


/* Defines ---------------------------------------------------------------------*/
// Address
#define TOTAL_SCREW_ALARM_ACTIVE_ADDRESS    0x1F4F
#define TOTAL_SCREW_ADDRESS                 0x1F50
#define RESET_SCREW_ADDRESS                 0x1F54
#define LIMIT_SCREW_ADDRESS                 0x1F58



#define TOTAL_SCREW_ADDR_REVISION_0         992
#define RESET_SCREW_ADDR_REVISION_0         994
#define LIMIT_SCREW_ADDR_REVISION_0         996



/* Struct ---------------------------------------------------------------------*/
typedef struct 
{
    uint8_t  enable;
    uint32_t totalScrew;                // 총 드라이버 동작 횟수
    uint32_t resetScrew;                // Reset한 드라이버 동작 횟수
    uint32_t limitScrew;                // 드라이버 동작 횟수 제한

    uint32_t totalScrewOld;             // 마지막으로 저장된 드라이버 동작 횟수

    uint16_t errorState;                // 에러 발생 여부 (드라이버 동작 횟수가 limit 초과)
}totalScrewCount;



/* Global vriables --------------------------------------------------------------------*/
extern totalScrewCount screwCount;



/* Funcions --------------------------------------------------------------------*/
// 초기화
void initTotalScrewCount(void);


// Screw 제어
void totalScrewProcess(uint16_t* error, uint16_t errCode, uint8_t run);
void totalScrewIncrease(uint8_t run);
void totalScrewError(uint16_t* error, uint16_t errCode);


// 통신으로 Screw 설정
uint16_t getTotalScrewCount(uint16_t address);
void setTotalScrewCount(uint16_t address, uint16_t Data);





#ifdef __cplusplus
    }
#endif

#endif
