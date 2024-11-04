/**
  ******************************************************************************
  * @file    eventRealtimeTask.h
  * @author  jun
  * @brief   Header file of event task with QSPI Memory.
  ******************************************************************************
  * @version
  * 2022.01.21 - event 저장 및 전송 처리
  * 
  * 
  * @note 사용법
  * - .c 파일
  * 1. initRealtimeMemSetting()
  *    초기 1회 동작
  * 
  * 2. realtimeEventSendTo232() , realtimeEventSendSeleNumTo232(), realtimeEventSendToWifi()
  *    주기적으로 동작
  * 
  * 3. CheckMaxValue()
  *    index가 증가 할 경우 추가
  * 
  * 
  * 5. setRealtimeSelect()
  *    통신 A:2902, Func: 03 - Serial back up 모드 선택
  * 
  * 6. initRealtimeEventAckWait()
  *    통신 A:4016, Func: 06 , AP 끊어졌을 때 - Ack 값 초기화
  * 
  * 7. returnMemorySavedValue()
  *    통신 A:2903, Func: 03 - 메모리 1 turn 사용 여부
  * 
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EVENT_REALTIME_TASK_H
#define EVENT_REALTIME_TASK_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Include --------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"


/* Defines ---------------------------------------------------------------------*/
// Realtime 주소 (Flash memory)
#define REALTIME_EVENT_BASE_ADDRESS             0                   // Realtime 저장 메모리 시작 주소
#define REALTIME_EVENT_DATA_SIZE                256                 // 메모리에 저장하는 크기

#define REALTIME_EVENT_SAVE_1TURN_OVER_ADDRESS  0x1F44              // Realtime index를 1회 모두 사용 여부
#define REALTIME_EVENT_SAVE_INDEX_ADDRESS       0x1F49              // 마지막으로 저장한 위치            
#define REALTIME_EVENT_SEND_INDEX_ADDRESS       0x1F4B              // 마지막으로 전송한 위치            


// Realtime Event 데이터 총 개수
#define REALTIME_EVENT_MAX_INDEX                32768 - 1           // 최대 저장 개수 (128 Block x 16 sector x sector당 16개   - 인덱스 겹치는것 1)

// 횟수 제한
#define MAX_RETRY_FAIL                          3



/* Struct ---------------------------------------------------------------------*/

// Event 관련 Struct
typedef struct
{
	uint32_t save_memory_data_no;                           // 마지막으로 저장한 index 위치
	uint32_t last_send_data_no;                             // 마지막으로 전송한 index 위치
} monitoringRealtimeEventStruct;

typedef struct 
{
    uint8_t  memorySaved1TurnOver;                          // 메모리에 데이터 저장 index가 1회전 이상 돌았을 때

    uint8_t  Flag_SendBackupDataAll;                        // Serial로 전체 데이터를 받을 때
    uint8_t  Flag_SendBackupSelecNum;                       // Serial로 일부 데이터를 선택해 받을 때

    uint16_t eventCount;                                    // 전송할 때 Event count 숫자
}eventRealtimeSettingStruct;

typedef struct 
{
    uint8_t  eventAckWait;                                  // Event 전송 후 ACK 기다리는 중
    uint8_t  eventAckFailCount;                             // Ack 실패 횟수
}eventRealtimeAckStruct;


typedef struct 
{
    monitoringRealtimeEventStruct   indexSelect;             // 선택한 일정 개수만 데이터 전송 index 관리

    eventRealtimeSettingStruct      setting;                 // 데이터 설정
    eventRealtimeAckStruct          ack;                     // 전송 후 Ack 관리
}RealtimeMemStruct;



/* Global vriables --------------------------------------------------------------------*/
extern RealtimeMemStruct realtimeMem;



/* Funcions --------------------------------------------------------------------*/
// Init
void initRealtimeMemSetting(void);


// Send
void realtimeEventSendTo232(void);
void realtimeEventSendSeleNumTo232(void);
void realtimeEventSendToWifi(void);

// Index 관리
void realtimeMemIndexManage(void);
void CheckMaxValue(void);


// 전송 모드 선택 (일정 개수 전송할 때)
uint8_t setRealtimeSelect(uint16_t data);

// Ack
void initRealtimeEventAckWait(void);

// 값 반환
uint8_t returnMemorySavedValue(void);






#ifdef __cplusplus
    }
#endif

#endif
