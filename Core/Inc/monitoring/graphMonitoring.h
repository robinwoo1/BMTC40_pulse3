/**
  ******************************************************************************
  * @file    graphMonitoring.h
  * @author  jun
  * @brief   Header file of graph monitoring.
  ******************************************************************************
  * @version
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 
  * 
  * - .C 파일
  * 1. graphMonitoringProcess()
  *    주기적으로 실행
  * 
  * 2. Collect_GraphData()
  *    그래프를 모으는 곳에서 주기적으로 실행
  * 
  * 3. clearGraphStepQueue()
  *    드라이버 동작이 시작될 때 같이 초기화
  * 
  * 4. setGraphStepQueue()
  *    그래프의 step 정보를 넣어야할 때 실행
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GRAPH_MONITORING_H
#define __GRAPH_MONITORING_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/

#define GRAPH_DATA_SIZE                         1000                        // 그래프 point 데이터 값 개수
#define GRAPH_STEP_INDEX_SIZE                   16                          // 그래프에서 step 별 체결 진행사항 개수


// 그래프 샘플링 시간
#define GRAPH_EVENT_SAMPLING_1MS                INV_TS_VEL / 1000           // 1ms 당 Sampling 하기 위한 상수


// 그래프 전송 데이터 정보
#define GRAPH_TRIGGER_HEADER_ADDR_SIZE          2                           // 그래프 Header 주소 크기 (byte)
#define GRAPH_TRIGGER_CHANNEL1                  0                           // 그래프 채널 정보 CH1
#define GRAPH_TRIGGER_CHANNEL2                  1                           // 그래프 채널 정보 CH2


// 그래프 Queue 정보
#define GRAPH_QUEUE_INDEX_SIZE                  6                           // 그래프 point 데이터를 저장하는 Queue 개수




/* Enums ---------------------------------------------------------------------*/

typedef enum {
    GRAPH_STEP_NONE = 0,
    GRAPH_STEP_FREE_REVERSE,
    GRAPH_STEP_THREAD_TAP,
    GRAPH_STEP_ENGAGING,
    GRAPH_STEP_FREE_SPEED,
    GRAPH_STEP_FASTENING_SPEED_START,
    GRAPH_STEP_SNUG_TQ,
    GRAPH_STEP_PREVAILING_START,
    GRAPH_STEP_SEATING,
    GRAPH_STEP_CLAMP,
    GRAPH_STEP_TQ_COMPLETE,
    GRAPH_STEP_ANGLE_AFTER,

} GRAPH_STEP_INORMATION_ENUM;



/* Struct ---------------------------------------------------------------------*/

typedef struct {
    uint16_t stepId;                    // Step 정보 ID
    uint16_t index;                     // Step 정보가 몇 번째 Graph point에 해당하는 지

} graphStepStruct;


typedef struct {
    uint16_t channel1Type;
    uint16_t channel2Type;

    uint16_t channel1Count;
    uint16_t channel2Count;

    uint16_t samplingRate;

    graphStepStruct stepInfo[GRAPH_STEP_INDEX_SIZE];

} graphSetResultStruct;


typedef struct {
    uint16_t    data[2][GRAPH_DATA_SIZE * 2];           // Float 형 이여야 하는데 데이터 전송을 위해 uint16_t로 정의
    uint16_t    realtimeData[128];

    uint8_t     realtimeOn;
    uint8_t     overflow;

    graphSetResultStruct setting;

} graphMonitoringReusltStruct;


typedef struct {
    uint16_t channel1[GRAPH_QUEUE_INDEX_SIZE][GRAPH_DATA_SIZE * 2];
    uint16_t channel2[GRAPH_QUEUE_INDEX_SIZE][GRAPH_DATA_SIZE * 2];

    uint16_t realtime[GRAPH_QUEUE_INDEX_SIZE][128];

    uint8_t startIndex;
    uint8_t endIndex;
    
} graphQueueStruct;


typedef struct {
    uint8_t complete;

    uint16_t realtime[128];
} graphMultiStruct;



/* Extern -----------------------------------------------------------------------*/
extern graphMonitoringReusltStruct graphMonitoringResult;
extern graphQueueStruct graphQueue;
extern graphMonitoringReusltStruct graphMonLastData;
extern graphMultiStruct graphMultiSequence;


/* Functions --------------------------------------------------------------------*/

void graphMonitoringProcess(void);

void Collect_GraphData(void);
void getGraphMonitoringRealtime(void);

void makeGraphResultData(void);
void sendGraphResultData(void);

void setGraphStepQueue(uint8_t step);
uint8_t getGraphStepQueue(uint8_t* queue);
void clearGraphStepQueue(void);

void saveGraphStepChange(graphSetResultStruct* graph, uint16_t index, uint8_t* stepQueue);
void increaseGraphStepIndex(graphSetResultStruct* graph);


void getGraphResultSetting(uint8_t* buf);

void addGraphQueue(graphMonitoringReusltStruct* dataBuf);
void deGraphQueue(graphMonitoringReusltStruct* saveBuf);

void graphExternalSave(void);
void graphInternalSave(void);




#ifdef __cplusplus
    }
#endif

#endif





