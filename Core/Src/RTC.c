/**
  ******************************************************************************
  * @file    RTC.c
  * @author  jun
  * @brief   Source file of RTC.
  * @note    RTC 시간 설정 및 확인
  ******************************************************************************
  */


/* Include ---------------------------------------------------------------------*/
#include "RTC.h"
#include <string.h>

 

/* Variable ---------------------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
rtcTime rtcCalendar;
uint8_t rtcReadOn = 1;
uint8_t rtcCheckOn = 0;
 


/* Preivate Funcions ----------------------------------------------------------------------*/
static void setRtcDate(uint8_t year, uint8_t month, uint8_t date);
static void setRtcTime(uint8_t hour, uint8_t minute, uint8_t second);
static void setRtcDefaultVal(rtcTime* rtc);

 

/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief Set the RTC Time
 * 
 * @param rtc : 날짜 및 시간 구조체
 */
void setRtc(rtcTime* rtc)
{
    // RTC의 year는 99까지 저장가능 - 분리하여 따로 저장한다 (라이브러리의 한계가 년도는 99까지만 설정이 가능하다)
    if(rtc->year >= 2000)
    {
        rtc->year = rtc->year % 2000;
    }

    if(rtcReadOn)
    {
        // Set 
        setRtcTime(rtc->hour, rtc->minute, rtc->second);
        setRtcDate(rtc->year, rtc->month, rtc->date);
    }
    else
    {
        // RTC에 문제가 있을 때 강제로 설정
        rtcCalendar.year = rtc->year + 2000;
        rtcCalendar.month = rtc->month;
        rtcCalendar.date = rtc->date;
        rtcCalendar.hour = rtc->hour;
        rtcCalendar.minute = rtc->minute;
        rtcCalendar.second = rtc->second;
    }
}


/**
 * @brief Get the RTC Time
 * 
 * @param rtc : 년도 및 시간 구조체
 */
void getRtc(rtcTime* rtc)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    if(rtcReadOn)
    {
        // Get RTC
        HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN);

        // Apply
        rtc->year = sDate.Year + 2000;      // Year은 2000 + 저장된 년도 로 계산
        rtc->month = sDate.Month;
        rtc->date = sDate.Date;

        rtc->hour = sTime.Hours;
        rtc->minute = sTime.Minutes;
        rtc->second = sTime.Seconds;

        // Check RTC not work
        if(rtcCheckOn == 0)
            checkRtcWork();
    }
    else 
    {
        // 기본 시간 반영
        if(rtc != &rtcCalendar)
        {
            memcpy(rtc, &rtcCalendar, sizeof(rtcTime));
        }

        // 초 단위 시간 흐름 반영
        if(HAL_GetTick() - rtc->setTime >= 1000)
        {
            // RTC의 시간은 2자리 숫자로 설정이 가능하기 때문에 2000을 더해서 년도를 만든다
            if(rtc->year < 2000)
                rtc->year += 2000;
                
            // HAL Tick overflow 확인
            if(HAL_GetTick() < rtc->setTime)
            {
                rtc->setTime = 0;
                return;
            }


            // 1초 시간 흐른 것 판단
            rtc->second++;

            if(rtc->second >= 60)
            {
                rtc->second = 0;
                rtc->minute++;
            }

            if(rtc->minute >= 60)
            {
                rtc->minute = 0;
                rtc->hour++;
            }

            if(rtc->hour >= 24)
            {
                rtc->hour = 1;
                rtc->date++;
            }

            if(rtc->month == 2)
            {
                // Check 윤년
                if(rtc->year % 4 == 0)
                {
                    if(rtc->date > 29)
                    {
                        // 2월달 29일 초과시
                        rtc->date = 1;
                        rtc->month++;
                    }
                }
                else
                {
                    if(rtc->date > 28)
                    {
                        // 2월달 28일 초과시
                        rtc->date = 1;
                        rtc->month++;
                    }
                }
            }
            else if(rtc->month % 2 == 0 && rtc->date > 30)
            {
                // 짝수달 30일 초과시
                rtc->date = 1;
                rtc->month++;
            }
            else if(rtc->month % 2 == 1 && rtc->date > 31)
            {
                // 홀수달 31일 초과시
                rtc->date = 1;
                rtc->month++;
            }

            if(rtc->month > 12)
            {
                rtc->month = 1;
                rtc->year++;
            }

            // 설정 시간에 반영
            rtc->setTime += 1000;
        }
    }

    // Set RTC의 기본 시간
    setRtcDefaultVal(&rtcCalendar);
}


/**
 * @brief Set the RTC Time
 * 
 * @param year : 년도 (2000이 감소된 99까지의 숫자)
 * @param month : 달
 * @param date : 일
 */
static void setRtcDate(uint8_t year, uint8_t month, uint8_t date)
{
    RTC_DateTypeDef sDate;

    // Check RTC 동작 유무
    if(rtcReadOn)
    {
        // RTC의 year는 99까지 저장가능 - 분리하여 따로 저장한다

        // Check value limit
        if(year > 99)
            year = 0;

        if(month > 12)
            month = 1;
        
        if(date > 31)
            date = 1;

        // Write Date
        sDate.Year = year;
        sDate.Month = month;
        sDate.Date = date;

        if(HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BIN) != HAL_OK)
        {
            rtcReadOn = 0;      // RTC Error state
            rtcCalendar.setTime = HAL_GetTick();
        }
    }
    else
    {
        // Check value limit
        if(year > 99)
            year = 0;

        if(month > 12)
            month = 1;
        
        if(date > 31)
            date = 1;

        rtcCalendar.year = year + 2000;
        rtcCalendar.month = month;
        rtcCalendar.date = date;
    }
}


/**
 * @brief Set the RTC Time
 * 
 * @param hour : 시간
 * @param minute : 분
 * @param second : 초
 */
static void setRtcTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    RTC_TimeTypeDef sTime;

    // Check value limit
    if(hour > 24)
        hour = 0;

    if(minute > 60)
        minute = 0;
    
    if(second > 60)
        second = 0;

    // Check RTC 동작 유무
    if(rtcReadOn)
    {
        // Write time
        sTime.Hours = hour;
        sTime.Minutes = minute;
        sTime.Seconds = second;
        
        if(HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BIN) != HAL_OK)
        {
            rtcReadOn = 0;
            rtcCalendar.setTime = HAL_GetTick();
        }
    }
    else
    {
        rtcCalendar.hour = hour;
        rtcCalendar.minute = minute;
        rtcCalendar.second = second;
    }
}


/**
 * @brief Set the Rtc Default Val object
 */
static void setRtcDefaultVal(rtcTime* rtc)
{
    if(rtc->year == 0)
        rtc->year = 2000;

    if(rtc->month == 0)
        rtc->month = 1;

    if(rtc->date == 0)
        rtc->date = 1;


    if(rtc->hour == 0)
        rtc->hour = 0;

    if(rtc->minute == 0)
        rtc->minute = 0;

    if(rtc->second == 0)
        rtc->second = 0;
}


/**
 * @brief RTC의 변화가 있는지 확인하여 일정 시간 동안 동일하게 유지될 경우 RTC를 비활성화한다. -> 내부 Clock으로 RTC를 증가 시킨다
 */
void checkRtcWork(void)
{
    static uint32_t timeout;
    static int8_t checkCnt = 5;
    static rtcTime rtcTemp;

    if(HAL_GetTick() - timeout > 2500)
    {
        // RTC 초의 시간이 변화가 없는지 확인
        if(rtcTemp.setTime == rtcCalendar.setTime)
            checkCnt++;
        else
            checkCnt--;

        rtcTemp.setTime = rtcCalendar.setTime;
        timeout = HAL_GetTick();
    }

    // RTC 상태 최종 결정
    if(checkCnt == 10)
    {
        rtcReadOn = 0;
        rtcCalendar.setTime = HAL_GetTick();
    }
    else if(checkCnt == 0)
        rtcCheckOn = 1;
}
