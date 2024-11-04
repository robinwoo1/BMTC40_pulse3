/**
  ******************************************************************************
  * @file    totalScrewCount.c
  * @author  jun
  * @brief   Source file of Total screw count.
  * @note    드라이버의 총 동작 횟수를 저장 -> Limit 횟수 초과하면 에러 발생.
  ******************************************************************************
  */

/* Include ---------------------------------------------------------------------*/
#include "totalScrewCount.h"
#include "backup.h"
#include "variable.h"


/* Defines -----------------------------------------------------------------------*/
#define ON      1
#define OFF     0


/* Variable ---------------------------------------------------------------------*/
totalScrewCount screwCount;


/* Extern -----------------------------------------------------------------------*/


/* Funcions ----------------------------------------------------------------------*/
/**
 * @brief Total screw의 값 메모리에서 읽기
 */
void initTotalScrewCount(void)
{
    uint8_t enable;
    uint32_t totalScrew = 0, resetScrew = 0, limitScrew = 0;

    // Read
    SPI_ReadMemoryData(TOTAL_SCREW_ALARM_ACTIVE_ADDRESS, (uint8_t*)&enable, 1);
    SPI_ReadMemoryData(TOTAL_SCREW_ADDRESS, (uint8_t*)&totalScrew, 4);
    SPI_ReadMemoryData(RESET_SCREW_ADDRESS, (uint8_t*)&resetScrew, 4);
    SPI_ReadMemoryData(LIMIT_SCREW_ADDRESS, (uint8_t*)&limitScrew, 4);
     
    // Apply
    screwCount.enable = enable;
    screwCount.totalScrew = totalScrew;
    screwCount.resetScrew = resetScrew;
    screwCount.limitScrew = limitScrew;

    screwCount.totalScrewOld = screwCount.totalScrew;
}




/* Screw 제어 --------------------------------------------------------------------*/

/**
 * @brief Total screw 값 비교 및 에러 반영
 * 
 * @note
 * 1. Total screw가 limit 값 보다 overflow 됐는지
 * 2. Total Screw 증가 및 메모리에 값 저장
 * 
 * @param error     : 에러값 반영할 변수
 * @param errCode   : 에러 코드 (TOTAL_SCREW_OVER)
 * @param run       : 드라이버 동작 상태
 */
void totalScrewProcess(uint16_t* error, uint16_t errCode, uint8_t run)
{
    static uint32_t saveTimeout;
    // static uint8_t enableEdge[2] = {0, };
    static uint8_t errorCheck;

    // Check limit screw setting
    if(screwCount.enable)
    {
        // Check Total screw over limit screw
        if((int64_t)screwCount.totalScrew - (int64_t)screwCount.resetScrew > screwCount.limitScrew && screwCount.limitScrew != 0)
        {
            // Error 1회만 표시
            if(errorCheck == OFF)
            {
                screwCount.errorState = ON;
                errorCheck = ON;
            }
        }
        else
            errorCheck = OFF;
    }
    
    
    // 10초 주기로 screw 저장 실행
    if(HAL_GetTick() - saveTimeout > 10000)
    {
        // Total screw count 값 저장
        if(screwCount.totalScrewOld != screwCount.totalScrew)
        {
            SPI_WriteMemoryData(TOTAL_SCREW_ADDRESS, (uint8_t*)&screwCount.totalScrew, sizeof(screwCount.totalScrew));

            screwCount.totalScrewOld = screwCount.totalScrew;
        }

        saveTimeout = HAL_GetTick();
    }

    // 에러 값 반영
    totalScrewError(error, errCode);

    // 체결 횟수 증가 확인
    totalScrewIncrease(run);
}


/**
 * @brief 드라이버 700ms 이상 동작 했을 시 Total screw 증가
 */
void totalScrewIncrease(uint8_t run)
{
    static uint8_t runState[2];
    static uint8_t flagRunOk;
    static uint32_t startTime;

    // Run 상태 값 저장
    runState[0] = runState[1];
    runState[1] = run;

    // 드라이버 시간 동작 시간 
    if(runState[0] < runState[1])
        startTime = HAL_GetTick();          // ON
    else if(run == OFF)
        startTime = 0;                      // OFF


    // 700ms 이상 회전 시 - 스크류 동작 횟수 ON (실제적으로 체결 결과 값을 보면 500ms)
    if(HAL_GetTick() - startTime > 700 && run == ON && startTime != 0)
        flagRunOk = ON;
    // 또는 체결 결과가 Fastening OK, NG, Error 일 때
    else if(runState[0] > runState[1] && (StatusOutput == rFASTENING_OK || StatusOutput == rFASTENING_NG || StatusOutput == rSYSTEM_ERR))
        flagRunOk = ON;


    // 드라이버 동작 종료 후 - 스크류 개수 증가
    if(run == OFF && flagRunOk)
    {
        screwCount.totalScrew++;
        flagRunOk = OFF;
    }
}


/**
 * @brief Total screw의 에러 상태를 Error 변수에 반영
 * 
 * @param error     : 반영할 error 상태 변수
 * @param errCode   : 적용할 에러 코드 (TOTAL_SCREW_OVER: 505)
 */
void totalScrewError(uint16_t* error, uint16_t errCode)
{
    if(screwCount.errorState)
    {
        // Error 상태가 아닐 때 값 반영
        if(*error == 0)
        {
            *error = errCode;
        }

        screwCount.errorState = 0;
    }
}




/* 통신으로 Screw 설정 --------------------------------------------------------------------*/

/**
 * @brief Get the Total Screw Count object
 * 
 * @note  모드버스에서 읽어가는 것이기 때문에 리턴 값을 16bit로 제한
 * @param address   : Screw count 별 주소 (TOTAL_SCREW_ADDRESS, RESET_SCREW_ADDRESS, LIMIT_SCREW_ADDRESS)
 * @return retval   : 해당 screw 값 (2byte - 절반씩 반환)
 */
uint16_t getTotalScrewCount(uint16_t address)
{
    uint16_t retval;

    // Address에 맞는 screw 값 반환
    switch (address)
    {
        case TOTAL_SCREW_ALARM_ACTIVE_ADDRESS:
            retval = (uint16_t)screwCount.enable;
            break;

        case TOTAL_SCREW_ADDRESS:
            retval = (uint16_t)((screwCount.totalScrew >> 16) & 0xFFFF);
            break;

        case TOTAL_SCREW_ADDRESS + 1:
            retval = (uint16_t)(screwCount.totalScrew & 0xFFFF);
            break;

        case RESET_SCREW_ADDRESS:
            retval = (uint16_t)((screwCount.resetScrew >> 16) & 0xFFFF);
            break;

        case RESET_SCREW_ADDRESS + 1:
            retval = (uint16_t)(screwCount.resetScrew & 0xFFFF);
            break;

        case LIMIT_SCREW_ADDRESS:
            retval = (uint16_t)((screwCount.limitScrew >> 16) & 0xFFFF);
            break;

        case LIMIT_SCREW_ADDRESS + 1:
            retval = (uint16_t)(screwCount.limitScrew & 0xFFFF);
            break;

        default:
            retval = 0;
            break;
    }

    return retval;
}


/**
 * @brief Set the Total Screw Count object
 * 
 * @note  모드 버스에서 2byte의 값을 적용할지 결정 (Total screw 등은 4Byte로 각각 2개의 주소 소유)
 * @param address   : Screw count 주소 (TOTAL_SCREW_ADDRESS, RESET_SCREW_ADDRESS, LIMIT_SCREW_ADDRESS)
 * @param Data      : 데이터
 */
void setTotalScrewCount(uint16_t address, uint16_t Data)
{
    // Screw count 주소
    switch (address)
    {
        case TOTAL_SCREW_ALARM_ACTIVE_ADDRESS:
            screwCount.enable = Data;

            SPI_WriteMemoryData(TOTAL_SCREW_ALARM_ACTIVE_ADDRESS, (uint8_t*)&screwCount.enable, sizeof(screwCount.enable));
            break;

        case TOTAL_SCREW_ADDRESS:
            // screwCount.totalScrew = (screwCount.totalScrew & 0xFFFF0000) + Data;

            // SPI_WriteMemoryData(TOTAL_SCREW_ADDRESS, (uint8_t*)&screwCount.totalScrew, sizeof(screwCount.totalScrew));
            break;
        
        case TOTAL_SCREW_ADDRESS + 1:
            // screwCount.totalScrew = ((Data << 16) & 0xFFFF0000) + (screwCount.totalScrew & 0x0000FFFF);

            // SPI_WriteMemoryData(TOTAL_SCREW_ADDRESS, (uint8_t*)&screwCount.totalScrew, sizeof(screwCount.totalScrew));
            break;

        case RESET_SCREW_ADDRESS:
            screwCount.resetScrew = ((Data << 16) & 0xFFFF0000) + (screwCount.resetScrew & 0x0000FFFF);

            SPI_WriteMemoryData(RESET_SCREW_ADDRESS, (uint8_t*)&screwCount.resetScrew, sizeof(screwCount.resetScrew));
            break;
        
        case RESET_SCREW_ADDRESS + 1:
            screwCount.resetScrew = (screwCount.resetScrew & 0xFFFF0000) + Data;

            SPI_WriteMemoryData(RESET_SCREW_ADDRESS, (uint8_t*)&screwCount.resetScrew, sizeof(screwCount.resetScrew));
            break;
        
        case LIMIT_SCREW_ADDRESS:
            screwCount.limitScrew = ((Data << 16) & 0xFFFF0000) + (screwCount.limitScrew & 0x0000FFFF);

            SPI_WriteMemoryData(LIMIT_SCREW_ADDRESS, (uint8_t*)&screwCount.limitScrew, sizeof(screwCount.limitScrew));
            break;

        case LIMIT_SCREW_ADDRESS + 1:
            screwCount.limitScrew = (screwCount.limitScrew & 0xFFFF0000) + Data;
            
            SPI_WriteMemoryData(LIMIT_SCREW_ADDRESS, (uint8_t*)&screwCount.limitScrew, sizeof(screwCount.limitScrew));
            break;

        default:
            break;
    }
}



