/**
  ******************************************************************************
  * @file    RTC.h
  * @author  jun
  * @brief   Header file of RTC.
  ******************************************************************************
  * @version
  * 2022.06.19 - RTC 기능 생성
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 1. setRtc()
  *    RTC 시간 설정 필요할 때 실행
  * 
  * 2. getRtc()
  *    주기적으로 실행 - RTC 시간 반영
  * 
  * 3. static void MX_RTC_Init(void) 
  *    CubeMx에서 자동으로 생성해주는 함수 안에 밑에 코드 추가 (초기 부팅했을 때 RTC가 설정되어 있으면 재설정을 안하게 막는 코드)
  * 
  *     //USER CODE BEGIN Check_RTC_BKUP

        // RTC TIME BACK UP
        if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) == 0x5A5B)
            return;

        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x5A5B);

        //USER CODE END Check_RTC_BKUP
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_TIME_H
#define __RTC_TIME_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/
#define RTC_ACTIVE                  ON

/* Enums ---------------------------------------------------------------------*/


/* Struct ---------------------------------------------------------------------*/

typedef struct
{
    uint16_t    year;
    uint8_t     month;
    uint8_t     date;

    uint8_t     hour;
    uint8_t     minute;
    uint8_t     second;

    uint32_t    setTime;
}rtcTime;
 



/* Extern -----------------------------------------------------------------------*/

extern rtcTime rtcCalendar;




/* Functions --------------------------------------------------------------------*/

void setRtc(rtcTime* rtc);
void getRtc(rtcTime* rtc);
void checkRtcWork(void);




#ifdef __cplusplus
    }
#endif

#endif





