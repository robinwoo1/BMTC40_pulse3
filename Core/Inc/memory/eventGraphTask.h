/**
  ******************************************************************************
  * @file    eventGraphTask.h
  * @author  jun
  * @brief   Header file of event task with QSPI Memory.
  ******************************************************************************
  * @version
  * 2021.11.01 - event 저장 및 전송 처리
  * 2023.07.21 - 파라미터 주소 정리 함에 따라 재정의
  * 
  * 
  * @note 사용법
  * 
  * - .c 파일
  * 1. initGraphMemSetting()
  *    초기 1회 실행.
  * 
  * 2. GraphMemProcess()
  *    주기적으로 실행
  * 
  * 3. graphMemIndexManage()
  *    통신 A: 6322, Func: 6 (event 응답 오는 곳에서 실행 - index 관리)
  * 
  * 4. graphRemainData()
  *    통신 A: 6323, Func: 3 에서 실행
  * 
  * 5. setGraphSetting()
  *    통신 A:6320, Func: 6 에서 실행
  * 
  * 6. getGraphSetting()
  *    통신 A:6320, Func: 3 에서 실행
  * 
  * 7. initGraphEventAckWait()
  *    통신 A: 6322, Func: 6 에서 실행 , TCP 연결 끊어졌을 때 실행
  * 
  * 8. graphMonitoringEvent()
  *    기존 Graph 모니터링 하는 곳에 추가 (Graph 완료 확인 함수)
  * 
  * 9. fasteningResultOn()
  *    기존 Realtime 모니터링 완료 후 동작 하는 곳 (Realtime 완료 확인 함수) 
  * 
  * 10. setEventSocketActivate()
  *     통신 A: 6320 ~ 6321, Func: 6, Graph 모니터링 활성화 설정
  * 
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EVENT_TASK_H
#define EVENT_TASK_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Include --------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "realtimeMonitoring.h"
#include "graphMonitoring.h"
#include "eventRealtimeTask.h"


/* Defines ---------------------------------------------------------------------*/
// 저장 주소 및 개수
#define GRAPH_EVENT_BASE_ADDRESS                0x800000                            // 4096 x 16 x 128 (1 sector 크기 x 16 sector x 128 block)
#define GRAPH_EVENT_GRAPH_SIZE                  4096 * 2                            // 그래프 1개당 차지하는 주소 공간
#define GRAPH_EVENT_MAX_INDEX                   512 - 1                             // 그래프 최대 저장 개수 (실제 저장은 0 ~ 511까지 512개를 저장 - index 겹치는 것 때문에 1 뺀다)
#define GRAPH_EVENT_NON_MEM_MAX_INDEX           10                                  // 메모리 없을 경우 그래프 저장 개수

// Index 주소 (eeprom 메모리 주소)
#define GRAPH_EVENT_CYCLE_INDEX_ADDR            0x1F40                               // Graph 전송이 가능한 realtime index 시작 주소        
#define GRAPH_EVENT_GRAPH_SAVE_INDEX_ADDR       0x1F42                               // Graph를 저장할 index 주소

// Setting 값 주소 (eeprom 메모리 주소)
#define GRAPH_EVENT_EEPROM_SETTING_ADDR         0x1F45     

// Data 크기
#define GRAPH_EVENT_HEADER_SIZE                 sizeof(graphEventHeaderStruct)                          // 그래프 Header 크기: 2    
#define GRAPH_EVENT_CHANNEL_SIZE                1000                                                    // 그래프 채널 당 데이터 개수

#define MAX_EVENT_SEND_SIZE                     GRAPH_EVENT_HEADER_SIZE + GRAPH_EVENT_GRAPH_SIZE + REALTIME_EVENT_DATA_SIZE     // Event에 대한 총 길이 (2 + 4096 * 2 + 256)
#define MODBUS_TCP_HEADER_SIZE                  8                                                       // Modbus TCP에 붙는 Header 크기

// 횟수 제한
#define MAX_RETRY_FAIL                          3

// REVISION
#define MAX_REVISION_NUM                        0                  // FIXME: 현재 툴에서 최대 지원하는 REVISION


#define GRAPH_MEM_QUEUE_SIZE                    5                   // Graph 데이터를 저장하기 위한 Queue


// 모니터링 종류
#define MONITORING_EVENT_REALTIME               0
#define MONITORING_EVENT_GRAPH                  1


// 고해상도 Realtime 데이터에서 Graph data가 저장된 정보에 대한 버퍼 위치
#define GRAPH_EVENT_G_DATA_ACT_INDEX            251
#define GRAPH_EVENT_G_CYCLE_H_INDEX             252
#define GRAPH_EVENT_G_CYCLE_L_INDEX             253
#define GRAPH_EVENT_G_MEM_H_INDEX               254
#define GRAPH_EVENT_G_MEM_L_INDEX               255




/* Struct ---------------------------------------------------------------------*/
// Graph 만들기 관련 Struct
typedef struct
{
    uint8_t  flagRealtimeOn;                                // Realtime 생성 완료
    uint8_t  flagSaveGraph;                                 // Graph 저장
    uint8_t  flagOverflow;                                  // Graph 데이터가 overflow 되었는지 확인
    uint16_t dataPoint;                                     // Graph 데이터 저장하던 위치
    uint8_t  graphBufType;                                  // Graph 버퍼의 종류 (0: graphData1,   1: graphData2)
    uint16_t realtimeSize;                                  // Revision 별로 다른 realtime 길이를 구한다.

    graphStepStruct stepInfo[GRAPH_STEP_INDEX_SIZE];

    float   graphData1[2][GRAPH_EVENT_CHANNEL_SIZE];        // Graph 데이터 저장
    float   graphData2[2][GRAPH_EVENT_CHANNEL_SIZE];        // Graph 데이터 저장
}graphMakeStruct;



/**
 * @brief 데이터 1byte 씩 정렬
 */
#pragma pack(push,1)

typedef struct
{
    uint8_t length[2];

} graphEventHeaderStruct;


typedef struct {
    uint16_t stepId;
    uint16_t index;

} graphEventStepStruct;


typedef struct {
    uint16_t  channel1Type;
    uint16_t  channel2Type;

    uint16_t  channel1Count;
    uint16_t  channel2Count;

    uint16_t  samplingRate;

    graphEventStepStruct stepInfo[GRAPH_STEP_INDEX_SIZE];

} graphEventResultStruct;


typedef struct 
{
    uint16_t  revision;
    uint16_t  eventId;
    uint16_t  date[2];
    uint16_t  time[2];

    uint16_t  fasteningTime;
    uint16_t  preset;
    uint16_t  unit;
    uint16_t  screwCount;
    uint16_t  fl;
    uint16_t  errorCode;
    uint16_t  status;

    uint16_t  targetTQ[2];
    uint16_t  convetTQ[2];
    uint16_t  seatingTQ[2];
    uint16_t  clampTQ[2];
    uint16_t  prevailingTQ[2];
    uint16_t  snugTQ[2];

    uint16_t  speed;
    uint16_t  angle1;
    uint16_t  angle2;
    uint16_t  angle3;
    uint16_t  snugAngle;

    uint16_t  reserve[8];
    uint16_t  barcode[REALTIME_BARCODE_SIZE];

    graphEventResultStruct graphInfo;
    
}eventFasteningRevision0Struct;

#pragma pack(pop)



// Event 관련 Struct
typedef struct
{
	uint16_t save_memory_data_no;                   // 마지막으로 저장한 index 위치
	uint16_t last_send_data_no;                     // 마지막으로 전송한 index 위치
} monitoringEventStruct;

typedef struct
{
	uint16_t saveGraphIndex;                        // 마지막으로 Graph 저장한 index 위치
	uint16_t cycleIndex;                            // Graph의 cycle 번호
} graphIndexStruct;


typedef struct 
{
    uint8_t  eventAckWait;                          // Event 전송 후 ACK 기다리는 중
    uint8_t  eventAckFailCount;                     // Ack 실패 횟수
}eventAckStruct;


typedef union {
 
    uint32_t totalVal;
    struct
    {
        // Low bit
        uint8_t  eventSendActive1 : 1;                      // Event 전송 활성화  ( 0: 비활성화, 1: 활성화, 2: index 초기화 후 활성화 )
        uint8_t  eventSendActive2 : 1;                       

        uint8_t  noAck : 1;                                 // Ack 전송 받을 지 결정

        uint8_t  channel1Type1 : 1;                         // 0: None, 1: Torque, 2: Speed, 3: Angle, 4: Torque/Angle
        uint8_t  channel1Type2 : 1;                          
        uint8_t  channel1Type3 : 1;                          

        uint8_t  channel2Type1 : 1;                          // 0: None, 1: Torque, 2: Speed, 3: Angle, 4: Torque/Angle
        uint8_t  channel2Type2 : 1;                          
        uint8_t  channel2Type3 : 1;                          

        uint8_t  samplingRate1 : 1;                          // Sampling rate: 2 ~ 30
        uint8_t  samplingRate2 : 1;                          
        uint8_t  samplingRate3 : 1;                          
        uint8_t  samplingRate4 : 1;                          
        uint8_t  samplingRate5 : 1;                          
        uint8_t  samplingRate6 : 1;                          
        uint8_t  samplingRate7 : 1;    


        uint8_t  eventRevision1 : 1;                        // Event 결과 값의 버전
        uint8_t  eventRevision2 : 1;                         
        uint8_t  eventRevision3 : 1;                         
        uint8_t  eventRevision4 : 1;                         

        uint8_t  reseved1 : 1;
        uint8_t  reseved2 : 1;
        uint8_t  reseved3 : 1;
        uint8_t  reseved4 : 1;
        uint8_t  reseved5 : 1;
        uint8_t  reseved6 : 1;
        uint8_t  reseved7 : 1;
        uint8_t  reseved8 : 1;
        uint8_t  reseved9 : 1;
        uint8_t  reseved10 : 1;
        uint8_t  reseved11 : 1;
        uint8_t  reseved12 : 1;
        // High bit
    } raw;
} graphEventSettingUnion;




typedef struct {
    uint8_t  eventRevision;                         // Event 결과 값의 버전
    uint16_t reseved;
    uint8_t  eventSendActive;                       // Event 전송 활성화  ( 0: 비활성화, 1: 활성화, 2: index 초기화 후 활성화 )
    uint8_t  noAck;                                 // Ack 전송 받을 지 결정
    uint8_t  channel1Type;                          // 0: None, 1: Torque, 2: Speed, 3: Angle, 4: Torque/Angle
    uint8_t  channel2Type;                          // 0: None, 1: Torque, 2: Speed, 3: Angle, 4: Torque/Angle
    uint8_t  samplingRate;                          // Sampling rate: 2 ~ 30

} graphEventSetting;


typedef struct
{
    uint8_t                 step;
    uint16_t                queueLegthBuf[GRAPH_MEM_QUEUE_SIZE];
    uint8_t                 graphBuf[GRAPH_MEM_QUEUE_SIZE][MAX_EVENT_SEND_SIZE + MODBUS_TCP_HEADER_SIZE + 1];
    uint8_t                 realtimeBuf[GRAPH_MEM_QUEUE_SIZE][256];

    uint8_t                 startIndex;
    uint8_t                 remainSize;

}eventQueueStruct;



typedef struct 
{
    uint8_t  sendBuf[MAX_EVENT_SEND_SIZE + MODBUS_TCP_HEADER_SIZE + 1];    // Graph 전송 버퍼

    graphMakeStruct         makeData;                       // Graph 데이터 저장 값
    graphEventSetting       setting;                        // 데이터 설정
    monitoringEventStruct   index;                          // 데이터 전송 index 관리
    graphIndexStruct        graphIndex;                     // save index: 마지막으로 저장한 Graph memory address, send index: cycle 번호
    eventAckStruct          ack;                            // 전송 후 Ack 관리

    eventQueueStruct        queue;
    uint8_t                 saveStep;

}GraphMemStruct;





/* Enum --------------------------------------------------------------------*/

typedef enum {
    GRAPH_STEP_CHECK_SAVE_DATA = 0,                         // 저장할 데이터 생성 여부 확인 STEP
    GRAPH_STEP_MAKE_DATA,                                   // 저장할 데이터 형식에 맞게 생성 STEP

    GRAPH_STEP_CHECK_QUEUE,
    GRAPH_STEP_SAVE_REALTIME_DATA,                          // Realtime 데이터 저장 STEP
    GRAPH_STEP_SAVE_GRAPH_CH1_DATA_ERASE,                   // Graph channel 1 영역 제거 STEP
    GRAPH_STEP_SAVE_GRAPH_CH1_DATA,                         // Graph channel 1 저장 STEP
    GRAPH_STEP_SAVE_GRAPH_CH2_DATA_ERASE,                   // Graph channel 2 영역 제거 STEP
    GRAPH_STEP_SAVE_GRAPH_CH2_DATA,                         // Graph channel 2 저장 STEP

} GRAPH_EVENT_STEP_ENUM;





/* Global vriables --------------------------------------------------------------------*/
extern GraphMemStruct GraphMem;                // Graph save struct

extern graphSetResultStruct graphEventStepInfo;
extern uint8_t graphEventMonStepQueue[GRAPH_STEP_INDEX_SIZE];
extern uint8_t graphEventMonStepUsed[GRAPH_STEP_INDEX_SIZE];



/* Funcions --------------------------------------------------------------------*/
// Graph event 설정 및 전송
void initGraphMemSetting(void);
void initGraphMemIndex(void);
void GraphMemProcess(void);

void makeGraphMemDataProcess(void);
void saveGraphMemDataProcess(void);



// Graph 데이터 생성
void makeGraphHeader(graphEventHeaderStruct * buf);
void arrangeGraphData(uint8_t* buf);

// Fastening 데이터 생성
int8_t makeFasteningResult(uint8_t* buf, uint16_t* fasteningBuf);
void makeFasteningRevision0(eventFasteningRevision0Struct* buf, pollingRealtimeDataStruct* fasteningBuf);

// Memory index 관리
void graphMemIndexManage(void);
uint16_t graphRemainData(void);

uint16_t getGraphRealimeSize(void);
uint16_t checkGraphDataActive(uint8_t* buf);
uint16_t getGraphMemCycleIndex(uint8_t* buf);



// Graph 설정 관리
int8_t setGraphSetting(uint32_t data, uint8_t socketNum);
uint32_t getGraphSetting(void);

// Ack 관리
void initGraphEventAckWait(void);

// Graph 모니터링 설정
void graphMonitoringEvent(void);
uint8_t graphChannelMapping(uint8_t channelType);
int8_t graphMonitoringNeed(pollingRealtimeDataStruct* realtime);

// Graph event 상태
void fasteningResultOn(void);

// Endian 변환
void eventConvertEndian(uint8_t* buf, uint16_t length);

// None memory 관리
void memoryNoneExistSet(void);
void saveNoneMemory(uint16_t index, uint8_t* eventBuf, uint8_t* dataBuf, uint16_t length);
void readNoneMemory(uint16_t index, uint8_t* eventBuf, uint8_t* saveBuf, uint16_t length);

// Queue - Graph
void enQueueGraphMem(uint8_t* reatlimeData, uint8_t* graphData, uint16_t length);
uint8_t deQueueGraphMem(uint8_t* saveRealtimeBuf, uint8_t* saveGraphBuf, uint16_t* length);




#ifdef __cplusplus
    }
#endif

#endif
