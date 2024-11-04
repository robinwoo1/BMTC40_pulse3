/**
  ******************************************************************************
  * @file    errorParameter.c
  * @author  jun
  * @brief   Source file of errorParameter.
  * @note    Error 발생 시 저장 및 표시
  ******************************************************************************
  */


/* Include ---------------------------------------------------------------------*/
#include "errorcodeParameter.h"
#include "parameter.h"
#include "backup.h"
#include "RTC.h"
#include "memoryQspiControl.h"

/* Variable ---------------------------------------------------------------------*/
errorCodeStruct errorCodeList[ERROR_CODE_INDEX_SIZE]; 



/* Preivate Funcions ----------------------------------------------------------------------*/
static errorCodeProcessStruct errorCode = { 0 , 0 };
 


/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief Error code list 저장 및 읽기 명령 실행 process
 */
void errorCodeProcess(void)
{
    // Error code 리스트 초기화
    initErrorCodeList();

    // Error code 읽기 실행
    if(errorCode.readCommand)
    {
        readErrorCodeMemory();

        errorCode.readCommand = 0;
    }

    // Error code 저장 실행
    if(errorCode.saveCommand)
    {
        saveErrorCodeMemory();

        errorCode.saveCommand = 0;
    }
}


/**
 * @brief 에러 코드 읽기 및 초기화 이전에 발생한 에러 저장
 */
void initErrorCodeList(void)
{
    static uint8_t init;

    if(init == 0)
    {
        // 초기화 이전에 발생했던 에러 copy
        errorCodeStruct errorCodeListTemp[ERROR_CODE_INDEX_SIZE];
        memcpy(errorCodeListTemp, errorCodeList, sizeof(errorCodeList));


        // Error code 읽기 실행
        readErrorCodeMemory();


        // 초기화 이전에 발생한 에러 적용
        for(uint8_t index = 0; index < ERROR_CODE_INDEX_SIZE; index++)
        {
            if(errorCodeListTemp[index].code != 0)
            {
                addErrorCodeList(errorCodeListTemp[index].code);
            }
        }

        init = ON;
    }
}


/**
 * @brief Error code를 리스트에 추가
 * 
 * @param code : 추가하고자 하는 Error code
 */
void addErrorCodeList(uint16_t code)
{
    errorCodeStruct buf[ERROR_CODE_INDEX_SIZE] = {0, };

    // Arrange error code list (이전 데이터 리스트를 하나 뒤로 미룬다)
    memcpy(&buf[0], &errorCodeList[0], sizeof(errorCodeStruct) * (ERROR_CODE_INDEX_SIZE - 1));
    memcpy(&errorCodeList[1], &buf[0], sizeof(errorCodeStruct) * (ERROR_CODE_INDEX_SIZE - 1));


    // Make new error code list
    errorCodeList[0].code = code;


    #if RTC_ACTIVE
        getRtc(&rtcCalendar);
        errorCodeList[0].time[3] = rtcCalendar.year;
        errorCodeList[0].time[2] = (rtcCalendar.month << 8) + rtcCalendar.date;
        errorCodeList[0].time[1] = (rtcCalendar.hour << 8) + rtcCalendar.minute;
        errorCodeList[0].time[0] = (rtcCalendar.second << 8);
    #endif

    errorCode.saveCommand = 1;
}


/**
 * @brief Error code list 중 해당하는 index를 초기화
 * 
 * @param index : 선택하고자하는 index
 */
void removeErrorCodeList(uint8_t index)
{
    memset(&errorCodeList[index].code, 0, sizeof(errorCodeStruct));

    errorCode.saveCommand = 1;
}


/**
 * @brief Error code에 관한 값을 memory에 저장한다.
 */
void saveErrorCodeMemory(void)
{
    // Save Error code list
    saveMemoryData(ERROR_CODE_MEMORY_START_ADDRESS, 0, 0, (uint8_t*)&errorCodeList, sizeof(errorCodeStruct) * ERROR_CODE_INDEX_SIZE);
}


/**
 * @brief Error code에 관한 값을 Memory에 저장한다.
 */
void readErrorCodeMemory(void)
{
    uint8_t buf[128] = {0, };
    uint16_t size = sizeof(errorCodeStruct);


    // Error code의 리스트 개수 만큼 반복
    for(uint8_t index = 0; index < ERROR_CODE_INDEX_SIZE; index++)
    {
        // Memory에서 데이터 읽기
        readMemoryData(ERROR_CODE_MEMORY_START_ADDRESS, index, size, buf, size);

        memcpy(&errorCodeList[index].code, buf, sizeof(errorCodeStruct));
    }

    // Check error code data
    if(errorCodeList[0].time[0] == 0xFFFF || errorCodeList[0].time[1] == 0xFFFF)
    {
        memset(&errorCodeList[0], 0, size * ERROR_CODE_INDEX_SIZE);
    }
}

