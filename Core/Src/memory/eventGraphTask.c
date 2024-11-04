/**
  ******************************************************************************
  * @file    eventGraphTask.c
  * @author  jun
  * @brief   Source file of monitoring event task.
  * @note    Monitoring event 메모리 저장 및 데이터 전송
  ******************************************************************************
  */

/* Include ---------------------------------------------------------------------*/
#include "eventGraphTask.h"
#include "common.h"
#include "variable.h"
#include "realtimeMonitoring.h"
#include "backup.h"
#include "modbus.h"
#include "wifiTask.h"
#include "socketControl.h"
#include "memoryQspiControl.h"
#include "eventRealtimeTask.h"
#include "graphMonitoring.h"
#include "multicastLudp.h"



/* Variable ---------------------------------------------------------------------*/
GraphMemStruct GraphMem;                // Graph save struct

graphSetResultStruct graphEventStepInfo;
uint8_t graphEventMonStepQueue[GRAPH_STEP_INDEX_SIZE];
uint8_t graphEventMonStepUsed[GRAPH_STEP_INDEX_SIZE];

uint8_t graphNoneMemoryBuf[GRAPH_EVENT_NON_MEM_MAX_INDEX + 1][MAX_EVENT_SEND_SIZE];




/* Extern -----------------------------------------------------------------------*/


/* Static Functions --------------------------------------------------------------*/
static void ackWaitManage(void);



/* Funcions ----------------------------------------------------------------------*/
/**
 * @brief Graph event를 저장 및 전송
 */
void GraphMemProcess(void)
{
    static uint32_t graphSendTimeout;

    uint8_t  errorState;
    uint16_t headerLength, sendLength;
    uint8_t  socketNum;

    // Graph event socket 상태 확인
    checkEventSocketConnect();

    // Flash memory 없을 때 setting 설정
    memoryNoneExistSet();

    // Graph index 관리
    realtimeMemIndexManage();
    graphMemIndexManage();


    // Make 고해상도 그래프 데이터 - Queue에 쌓기
    makeGraphMemDataProcess();

    // Save 고해상도 그래프 to memory
    saveGraphMemDataProcess();


    /* Send process --------------------------------------------------------------- */

    // Check send socket number
    socketNum = getEventSocketActive(MONITORING_EVENT_GRAPH);


    // Send Graph data (Event 전송 모드, 소켓 연결 상태 확인)
    if(socketControl.socketState[socketNum].monitoring.flagEventGraph && socketControl.socketState[socketNum].connect)
    {
        // Ack 관리
        ackWaitManage();

        // Ack 대기중 확인
        if(GraphMem.ack.eventAckWait)
            errorState = ON;
        else
            errorState = OFF;

        // Error 확인
        if(errorState != ON)
        {
            // 전송할 것이 있는지 Index 확인
            if(GraphMem.index.save_memory_data_no != GraphMem.index.last_send_data_no)
            {
                if(GraphMem.saveStep == GRAPH_STEP_CHECK_SAVE_DATA)
                {
                    // 전송 시간 Delay (20ms)
                    if(HAL_GetTick() - graphSendTimeout > 20 && GraphMem.queue.step == GRAPH_STEP_CHECK_QUEUE)
                    {
                        // Read Realtime data
                        if(memoryQspi.flashOperation == ON)
                        {
                            // Realtime
                            readMemoryData(REALTIME_EVENT_BASE_ADDRESS, GraphMem.index.last_send_data_no, REALTIME_EVENT_DATA_SIZE, GraphMem.sendBuf + MODBUS_TCP_HEADER_SIZE, REALTIME_EVENT_DATA_SIZE);

                            // Check graph data size
                            if(checkGraphDataActive(GraphMem.sendBuf))
                            {
                                // Graph
                                readMemoryData(GRAPH_EVENT_BASE_ADDRESS, getGraphMemCycleIndex(GraphMem.sendBuf), GRAPH_EVENT_GRAPH_SIZE, GraphMem.sendBuf + MODBUS_TCP_HEADER_SIZE + GRAPH_EVENT_HEADER_SIZE + getGraphRealimeSize(), GRAPH_EVENT_GRAPH_SIZE);
                            }
                        }
                        else
                            readNoneMemory(GraphMem.index.last_send_data_no, graphNoneMemoryBuf[GraphMem.index.last_send_data_no], GraphMem.sendBuf + MODBUS_TCP_HEADER_SIZE, MAX_EVENT_SEND_SIZE);

                        // Check data
                        if(memoryQspi.flashOperation == ON && GraphMem.sendBuf[MODBUS_TCP_HEADER_SIZE] == 0xFF && GraphMem.sendBuf[MODBUS_TCP_HEADER_SIZE + 1] == 0xFF)
                        {
                            memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
                            return;
                        }

                        // Modbus Header Transaction ID, Protocol ID
                        memset(GraphMem.sendBuf, 0, 4);

                        // Modbus Header Length (Modbus unit id(1byte) + function code(1byte) + data header length(2byte)+ data (n byte))
                        headerLength = 1 + 1 + 2 + (GraphMem.sendBuf[MODBUS_TCP_HEADER_SIZE] << 8) + GraphMem.sendBuf[MODBUS_TCP_HEADER_SIZE + 1];  
                        GraphMem.sendBuf[4] = (uint8_t)(headerLength >> 8);
                        GraphMem.sendBuf[5] = (uint8_t)(headerLength & 0xFF);

                        // Modbus Header Unit id, Function code
                        GraphMem.sendBuf[6] = 0;
                        GraphMem.sendBuf[7] = 0x66;

                        // Send length
                        sendLength = headerLength + 6;          // other (n byte) + Transaction ID(2byte) + Protocol ID(2byte) + header length(2byte)

                        // Make big endian
                        eventConvertEndian(&GraphMem.sendBuf[MODBUS_TCP_HEADER_SIZE], sendLength - MODBUS_TCP_HEADER_SIZE);
                    
                        // 데이터 전송
                        tcpSendData(socketNum, GraphMem.sendBuf, sendLength);

                        // Ack 세팅
                        GraphMem.ack.eventAckWait = ON;
                        graphSendTimeout = HAL_GetTick();
                    }
                }
            }
        }
    }
}





/* Graph memory 관리 프로세스 -------------------------------------------------------------------------------*/

/**
 * @brief 고해상도 그래프 데이터를 생성 + Queue에 쌓는다
 */
void makeGraphMemDataProcess(void)
{
    uint8_t saveRealtimehBuf[REALTIME_EVENT_DATA_SIZE + 1];
    uint8_t saveGraphBuf[MAX_EVENT_SEND_SIZE + 1];
    uint16_t saveLength;
    int8_t   result;

    // Graph data save process
    switch (GraphMem.saveStep)
    {
        case GRAPH_STEP_CHECK_SAVE_DATA:
            // 이벤트 저장할 지 판단
            if(GraphMem.makeData.flagRealtimeOn)
            {
                // Graph data를 받을 지 판단
                result = graphMonitoringNeed((pollingRealtimeDataStruct*)mbMData);

                // Graph 데이터 필요한지 판단
                if(result == ON)
                {
                    // Graph 데이터 필요
                    if(GraphMem.makeData.flagSaveGraph == ON)
                        GraphMem.saveStep = GRAPH_STEP_MAKE_DATA;
                } 
                else
                {
                    // Graph 데이터 X
                    GraphMem.saveStep = GRAPH_STEP_MAKE_DATA;

                    GraphMem.makeData.dataPoint = 0;
                    GraphMem.makeData.flagOverflow = 0;
                }
            }
        break;

        case GRAPH_STEP_MAKE_DATA:
            // Init buf
            memset(saveRealtimehBuf, 0, sizeof(saveRealtimehBuf));
            memset(saveGraphBuf, 0, sizeof(saveGraphBuf));

            // Make graph Header
            makeGraphHeader((graphEventHeaderStruct*)saveRealtimehBuf);

            // Make fastening result
            result = makeFasteningResult(saveRealtimehBuf, mbMData);

            // Arrange graph data
            arrangeGraphData(saveGraphBuf);

            // printf("G1 R1: %x, %x, %x, %x\n", saveRealtimehBuf[2 + 4], saveRealtimehBuf[3 + 4], saveRealtimehBuf[4 + 4], saveRealtimehBuf[5 + 4]);
            // printf("G1 D1: %x, %x, %x, %x\n", saveGraphBuf[0], saveGraphBuf[1], saveGraphBuf[2], saveGraphBuf[3]);


            // Graph data index를 Realtime 영역에 저장
            if(GraphMem.makeData.flagSaveGraph)
            {
                saveRealtimehBuf[GRAPH_EVENT_G_DATA_ACT_INDEX] = ON;
                saveRealtimehBuf[GRAPH_EVENT_G_CYCLE_H_INDEX] = (GraphMem.graphIndex.cycleIndex >> 8) & 0xFF;
                saveRealtimehBuf[GRAPH_EVENT_G_CYCLE_L_INDEX] = (GraphMem.graphIndex.cycleIndex >> 0) & 0xFF;
                saveRealtimehBuf[GRAPH_EVENT_G_MEM_H_INDEX] = (GraphMem.graphIndex.saveGraphIndex >> 8) & 0xFF;
                saveRealtimehBuf[GRAPH_EVENT_G_MEM_L_INDEX] = (GraphMem.graphIndex.saveGraphIndex >> 0) & 0xFF;
            }

            // Check result
            if(result == -1)
            {
                GraphMem.saveStep = GRAPH_STEP_CHECK_SAVE_DATA;         // Error
            }
            else
            {
                GraphMem.saveStep = GRAPH_STEP_CHECK_SAVE_DATA;         // OK

                if(GraphMem.makeData.flagOverflow)
                    saveLength = GRAPH_EVENT_CHANNEL_SIZE * sizeof(float) * 2;
                else
                    saveLength = GraphMem.makeData.dataPoint * sizeof(float) * 2;

                // Enqueue
                enQueueGraphMem(saveRealtimehBuf, saveGraphBuf, saveLength);

                // Graph 저장 flag OFF
                GraphMem.makeData.flagSaveGraph = OFF;
                GraphMem.makeData.flagRealtimeOn = OFF;
            }
        break;
    }

}


/**
 * @brief 고해상도 그래프 데이터를 Memory에 저장 + Queue에 쌓았던 데이터를 사용
 */
void saveGraphMemDataProcess(void)
{
    static uint8_t  queueGraphBuf[MAX_EVENT_SEND_SIZE + 1];
	static uint8_t  queueRealtimeBuf[256];
    static uint16_t queueLength;
    static uint32_t eraseTimeout;


    // Queue에 저장된 이벤트 Memory에 저장
    switch(GraphMem.queue.step)
    {
        case GRAPH_STEP_CHECK_QUEUE:
            if(deQueueGraphMem(queueRealtimeBuf, queueGraphBuf, &queueLength))
            {
                GraphMem.queue.step = GRAPH_STEP_SAVE_REALTIME_DATA;
                eraseTimeout = 0;
            }
        break;

        case GRAPH_STEP_SAVE_REALTIME_DATA:
            // Save Graph data
            if(memoryQspi.flashOperation == ON)
            {
                // Save realtime data
                saveMemoryData(REALTIME_EVENT_BASE_ADDRESS, GraphMem.index.save_memory_data_no, REALTIME_EVENT_DATA_SIZE, (uint8_t*)queueRealtimeBuf, REALTIME_EVENT_DATA_SIZE);
            }
            else
            {
                saveNoneMemory(GraphMem.index.save_memory_data_no, graphNoneMemoryBuf[GraphMem.index.save_memory_data_no], queueRealtimeBuf, REALTIME_EVENT_DATA_SIZE);
            }
            
            GraphMem.queue.step = GRAPH_STEP_SAVE_GRAPH_CH1_DATA_ERASE;
        break;


        case GRAPH_STEP_SAVE_GRAPH_CH1_DATA_ERASE:
            // Check QSPI 메모리 상태, Graph 데이터 사용 여부
            if(memoryQspi.flashOperation == ON && queueRealtimeBuf[GRAPH_EVENT_G_DATA_ACT_INDEX] == ON)
            {
                if(eraseTimeout == 0)
                {
                    eraseMemoryNoneWait(GRAPH_EVENT_BASE_ADDRESS, GraphMem.graphIndex.saveGraphIndex, GRAPH_EVENT_GRAPH_SIZE, GRAPH_EVENT_GRAPH_SIZE / 2);
                    eraseTimeout = HAL_GetTick();
                }               
            }
            else
                eraseTimeout = 0;

            // Memory 지워지는 시간 대기 (40ms)
            if(HAL_GetTick() - eraseTimeout > 40 && HAL_GetTick() > 40)
            {
                GraphMem.queue.step = GRAPH_STEP_SAVE_GRAPH_CH1_DATA;
                eraseTimeout = 0;
            }
        break;

        case GRAPH_STEP_SAVE_GRAPH_CH1_DATA:
            // Save Graph channel 1 data
            if(memoryQspi.flashOperation == ON)
            {
                // Check Graph data
                if(queueRealtimeBuf[GRAPH_EVENT_G_DATA_ACT_INDEX] == ON)
                {
                    saveMemoryDataNoneWait(GRAPH_EVENT_BASE_ADDRESS, GraphMem.graphIndex.saveGraphIndex, GRAPH_EVENT_GRAPH_SIZE, queueGraphBuf, GRAPH_EVENT_GRAPH_SIZE / 2);
                }
            }
            else
            {
                saveNoneMemory(GraphMem.index.save_memory_data_no, &graphNoneMemoryBuf[GraphMem.index.save_memory_data_no][GRAPH_EVENT_HEADER_SIZE + getGraphRealimeSize()], queueGraphBuf, GRAPH_EVENT_GRAPH_SIZE / 2);
            }
            
            GraphMem.queue.step = GRAPH_STEP_SAVE_GRAPH_CH2_DATA_ERASE;
        break;

        case GRAPH_STEP_SAVE_GRAPH_CH2_DATA_ERASE:
            // Check QSPI 메모리 상태, Graph 데이터 사용 여부
            if(memoryQspi.flashOperation == ON && queueRealtimeBuf[251] == ON)
            {
                if(eraseTimeout == 0)
                {
                    eraseMemoryNoneWait(GRAPH_EVENT_BASE_ADDRESS + GRAPH_EVENT_GRAPH_SIZE / 2, GraphMem.graphIndex.saveGraphIndex, GRAPH_EVENT_GRAPH_SIZE, GRAPH_EVENT_GRAPH_SIZE / 2);
                    eraseTimeout = HAL_GetTick();
                }               
            }
            else
                eraseTimeout = 0;

            // Memory 지워지는 시간 대기 (40ms)
            if(HAL_GetTick() - eraseTimeout > 40 && HAL_GetTick() > 40)
            {
                GraphMem.queue.step = GRAPH_STEP_SAVE_GRAPH_CH2_DATA;
                eraseTimeout = 0;
            }
        break;

        case GRAPH_STEP_SAVE_GRAPH_CH2_DATA:
            if(eraseTimeout == 0)
            {
                // Save Graph channel 2 data
                if(memoryQspi.flashOperation == ON)
                {
                    // Check Graph data
                    if(queueRealtimeBuf[251] == ON)
                    {
                        saveMemoryDataNoneWait(GRAPH_EVENT_BASE_ADDRESS + GRAPH_EVENT_GRAPH_SIZE/2, GraphMem.graphIndex.saveGraphIndex, GRAPH_EVENT_GRAPH_SIZE, &queueGraphBuf[GRAPH_EVENT_GRAPH_SIZE / 2], GRAPH_EVENT_GRAPH_SIZE / 2);
                    }

                    if(memoryQspi.error == 0)
                    {
                        // Index 증가
                        GraphMem.index.save_memory_data_no++;

                        if(queueRealtimeBuf[GRAPH_EVENT_G_DATA_ACT_INDEX] == ON)
                        {
                            GraphMem.graphIndex.saveGraphIndex++;
                        }
                    }
                }
                else
                {
                    saveNoneMemory(GraphMem.index.save_memory_data_no, &graphNoneMemoryBuf[GraphMem.index.save_memory_data_no][GRAPH_EVENT_HEADER_SIZE + getGraphRealimeSize()], queueGraphBuf, GRAPH_EVENT_GRAPH_SIZE);
                    
                    GraphMem.index.save_memory_data_no++;
                }

                // Index 관리
                graphMemIndexManage();
                eraseTimeout = HAL_GetTick();
            }
            
            // Memory 쓰는 시간 대기 (10ms)
            if(HAL_GetTick() - eraseTimeout > 10 && HAL_GetTick() > 10)
            {
                GraphMem.queue.step = GRAPH_STEP_CHECK_QUEUE;
                eraseTimeout = 0;
            }
        break;
        
        default:
            GraphMem.queue.step = GRAPH_STEP_CHECK_QUEUE;
        break;
    }
}




/* Graph 데이터 생성 -------------------------------------------------------------------------------*/

/**
 * @brief graph 데이터에 Header를 만든다.
 * 
 * @param buf : 데이터를 저장할 buffer
 */
void makeGraphHeader(graphEventHeaderStruct * buf)
{
    uint16_t length1;
    uint16_t length2;

    // Graph - channel 1
    if(GraphMem.setting.channel1Type == 0)
        length1 = 0;                                // Channel 1 없을 때
    else if(GraphMem.makeData.flagOverflow)
        length1 = GRAPH_EVENT_CHANNEL_SIZE;         // Channel 1 overflow
    else
        length1 = GraphMem.makeData.dataPoint;      // Channel 1 일정 개수


    // Graph - channel 2
    if(GraphMem.setting.channel2Type == 0)
        length2 = 0;                                // Channel 2 없을 때
    else if(GraphMem.makeData.flagOverflow)
        length2 = GRAPH_EVENT_CHANNEL_SIZE;         // Channel 2 overflow
    else
        length2 = GraphMem.makeData.dataPoint;      // Channel 2 일정 개수


    // Message length (Revision에 따라 변경)
    switch (GraphMem.setting.eventRevision)
    {
        case 0:
            buf->length[0] = (uint8_t)((sizeof(eventFasteningRevision0Struct) + (length1 + length2) * sizeof(float)) >> 8);
            buf->length[1] = (uint8_t)((sizeof(eventFasteningRevision0Struct) + (length1 + length2) * sizeof(float)) >> 0);
        break;
        
        default:
            buf->length[0] = (uint8_t)((sizeof(eventFasteningRevision0Struct) + (length1 + length2) * sizeof(float)) >> 8);
            buf->length[1] = (uint8_t)((sizeof(eventFasteningRevision0Struct) + (length1 + length2) * sizeof(float)) >> 0);
        break;
    }

    // printf("G Len: %x, %x\n", buf->length[0], buf->length[1]);
    // printf("CH Len: %x, %x\n", length1 * 4, length2 * 4);
}


/**
 * @brief Graph의 데이터를 순서대로 재배열한다.
 * 
 * @param buf : 데이터를 저장할 buffer
 */
void arrangeGraphData(uint8_t* buf)
{
    uint32_t length;
    uint8_t*   bufPoint;
    uint8_t  channelNum;
    uint8_t  startChannel;
    uint16_t channelOffset;
    float (*bufPointer)[GRAPH_EVENT_CHANNEL_SIZE];


    // 채널 개수 설정
    if(GraphMem.setting.channel1Type != 0 && GraphMem.setting.channel2Type != 0)
    {
        channelNum = 2;         // Chaneel 1, 2
        startChannel = 0;
    }
    else if(GraphMem.setting.channel1Type != 0)
    {
        channelNum = 1;         // Chaneel 1
        startChannel = 0;
    }
    else if(GraphMem.setting.channel2Type != 0)
    {
        channelNum = 1;         // Chaneel 2
        startChannel = 1;
    }
    else
        channelNum = 0;         // Chaneel X

    // 그래프 값 넣을 pointer 위치 계산
    bufPoint = buf;

    // Data 개수 limit
    if(GraphMem.makeData.dataPoint <= GRAPH_EVENT_CHANNEL_SIZE)
    {
        // Copy할 graph 버퍼 선택
        if(GraphMem.makeData.graphBufType == 0)
            bufPointer = GraphMem.makeData.graphData1;
        else
            bufPointer = GraphMem.makeData.graphData2;

        // 그래프 데이터 순서 재정렬
        if(GraphMem.makeData.flagOverflow)
        {
            // Overflow 되었을 때
            for(uint8_t i=0; i < channelNum; i++)
            {
                // Channel별 메모리 위치
                channelOffset = i * GRAPH_EVENT_CHANNEL_SIZE * sizeof(float);

                // Overflow 위치 계산
                length = GRAPH_EVENT_CHANNEL_SIZE - GraphMem.makeData.dataPoint;

                // overflow된 위치 확인 후 나누어 저장
                memcpy(bufPoint + channelOffset, &bufPointer[startChannel][GraphMem.makeData.dataPoint], length * sizeof(float));
                memcpy(bufPoint + channelOffset + length * sizeof(float), &bufPointer[startChannel][0], GraphMem.makeData.dataPoint * sizeof(float));

                startChannel++;
            }
        }
        else
        {
            // Overflow X
            for(uint8_t i=0; i < channelNum; i++)
            {
                // Channel별 메모리 위치
                channelOffset = i * GraphMem.makeData.dataPoint * sizeof(float);

                // Overflow 없을 때
                memcpy(bufPoint + channelOffset, &bufPointer[startChannel][0], GraphMem.makeData.dataPoint * sizeof(float));
                
                startChannel++;
            }
        }
    }
}



/* Fastening 데이터 생성 -------------------------------------------------------------------------------*/

/**
 * @brief Fastening result의 Data를 revision에 맞게 만든다.
 * 
 * @param buf : 데이터 저장하는 Buffer
 * @param fasteningBuf : 체결 결과 값
 */
int8_t makeFasteningResult(uint8_t* buf, uint16_t* fasteningBuf)
{
    int8_t  retval = 1;
    uint8_t headerOffset;
    
    // 헤더 offset 길이 계산
    headerOffset = sizeof(graphEventHeaderStruct);

    // Revision 확인
    switch (GraphMem.setting.eventRevision)
    {
        case 0:
            // Revision 0
            makeFasteningRevision0((eventFasteningRevision0Struct*)(buf + headerOffset), (pollingRealtimeDataStruct*)fasteningBuf);
        break;
        
        default:
            // Error
            retval = -1;
        break;
    }
    
    return retval;
}


/**
 * @brief Revision 0에 맞게 데이터 구성
 * 
 * @param buf : 데이터 저장하는 Buffer
 * @param fasteningBuf : 체결 결과 값
 */
void makeFasteningRevision0(eventFasteningRevision0Struct* buf, pollingRealtimeDataStruct* fasteningBuf)
{
    // Copy realtiem 데이터
    memcpy(&buf->revision, &fasteningBuf->revision, sizeof(eventFasteningRevision0Struct));

    // Set revison
    buf->revision = GraphMem.setting.eventRevision;

    // Set graph data
    buf->graphInfo.channel1Type = GraphMem.setting.channel1Type;
    buf->graphInfo.channel2Type = GraphMem.setting.channel2Type;

    if(GraphMem.setting.channel1Type)
    {
        if(GraphMem.makeData.flagOverflow)
            buf->graphInfo.channel1Count = GRAPH_EVENT_CHANNEL_SIZE;
        else
            buf->graphInfo.channel1Count = GraphMem.makeData.dataPoint;
    }
    else
        buf->graphInfo.channel1Count = 0;

    if(GraphMem.setting.channel2Type)
    {
        if(GraphMem.makeData.flagOverflow)
            buf->graphInfo.channel2Count = GRAPH_EVENT_CHANNEL_SIZE;
        else
            buf->graphInfo.channel2Count = GraphMem.makeData.dataPoint;
    }
    else
        buf->graphInfo.channel2Count = 0;
        
    buf->graphInfo.samplingRate = GraphMem.setting.samplingRate;
    memcpy(buf->graphInfo.stepInfo, GraphMem.makeData.stepInfo, sizeof(buf->graphInfo.stepInfo));
}






/* Memory 관리 -------------------------------------------------------------------------------*/

/**
 * @brief 고해상도 Graph event - Graph가 저장되는 index, Graph 전송 시작 index 관리
 */
void graphMemIndexManage(void)
{
    static uint32_t lastSendIndex;
    static uint32_t lastSaveIndex;
    static uint8_t  initFlag;


    // 초기 Set send, save index
    if(initFlag == OFF)
    {
        lastSendIndex = GraphMem.graphIndex.cycleIndex;
        lastSaveIndex = GraphMem.graphIndex.saveGraphIndex;

        initFlag = ON;
    }

    // Index 최대 값 확인
    if(memoryQspi.flashOperation == ON)
    {
        // 저장 Index는 Graph 저장 개수와 연동
        if (GraphMem.graphIndex.saveGraphIndex > GRAPH_EVENT_MAX_INDEX)
        {
            GraphMem.graphIndex.saveGraphIndex = 0;
            GraphMem.graphIndex.cycleIndex++;
        }
    }
    else
    {
        if (GraphMem.graphIndex.saveGraphIndex > GRAPH_EVENT_NON_MEM_MAX_INDEX)
            GraphMem.graphIndex.saveGraphIndex = 0;
    }

    // Index overflow 관리
    if(memoryQspi.flashOperation == ON)
    {
    	// index 변경되었으면 저장
        if(lastSaveIndex != GraphMem.graphIndex.saveGraphIndex)
        	SPI_WriteMemoryData(GRAPH_EVENT_GRAPH_SAVE_INDEX_ADDR, (uint8_t*)&GraphMem.graphIndex.saveGraphIndex, sizeof(GraphMem.graphIndex.saveGraphIndex));

        if(lastSendIndex != GraphMem.graphIndex.cycleIndex)
        	SPI_WriteMemoryData(GRAPH_EVENT_CYCLE_INDEX_ADDR, (uint8_t*)&GraphMem.graphIndex.cycleIndex, sizeof(GraphMem.graphIndex.cycleIndex));
    }

    // Save index old
    lastSendIndex = GraphMem.graphIndex.cycleIndex;
    lastSaveIndex = GraphMem.graphIndex.saveGraphIndex;
}





/* Graph 설정 관리 -------------------------------------------------------------------------------*/

/**
 * @brief 메모리 index를 0으로 초기화 한다.
 */
void initGraphMemIndex(void)
{
    // Realtime 관련 index
    GraphMem.index.save_memory_data_no = 0;
    GraphMem.index.last_send_data_no = 0;
    SPI_WriteMemoryData(REALTIME_EVENT_SAVE_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.save_memory_data_no, sizeof(GraphMem.index.save_memory_data_no)); 	// save memory data no
    SPI_WriteMemoryData(REALTIME_EVENT_SEND_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.last_send_data_no, sizeof(GraphMem.index.last_send_data_no)); 	    // send memory data no

    // Graph 관련 index
    GraphMem.graphIndex.saveGraphIndex = 0;
    GraphMem.graphIndex.cycleIndex = 0;
    SPI_WriteMemoryData(GRAPH_EVENT_GRAPH_SAVE_INDEX_ADDR, (uint8_t*)&GraphMem.graphIndex.saveGraphIndex, sizeof(GraphMem.graphIndex.saveGraphIndex));
    SPI_WriteMemoryData(GRAPH_EVENT_CYCLE_INDEX_ADDR, (uint8_t*)&GraphMem.graphIndex.cycleIndex, sizeof(GraphMem.graphIndex.cycleIndex));
}


/**
 * @brief Set the Graph Setting
 * 
 * @param data      : Graph setting 값
 * @param socketNum : 마지막으로 메시지 전송한 소켓 (Modbus socket)
 * @return 0: 성공, -1: 에러
 */
int8_t setGraphSetting(uint32_t data, uint8_t socketNum)
{
    int8_t retval = 0;
    int8_t eventSocket;
    uint8_t socketType;
    graphEventSettingUnion setUnion;
    graphEventSetting setTemp;

    // Find socket type by socket number
    socketType = findSocketType(socketNum);

	// 메시지 전송한 모드버스 socket에 따라 event socket 번호 추출 
    switch (socketType)
    {
        case TCP_SERVER_MODBUS_SOCKET:
        case TCP_SERVER_MODBUS_SOCKET_2:
            eventSocket = findSocketDescription(TCP_SERVER_EVENT_SOCKET);
            break;
        
        case TCP_CLIENT_MODBUS_SOCKET:
            eventSocket = findSocketDescription(TCP_CLIENT_EVENT_SOCKET);
            break;

        default:
            eventSocket = -1;
            break;
    }

    // 데이터 임시 적용
    setUnion.totalVal = data;

    setTemp.eventRevision = (setUnion.raw.eventRevision4 << 3) + (setUnion.raw.eventRevision3 << 2) + (setUnion.raw.eventRevision2 << 1) + (setUnion.raw.eventRevision1 << 0);
    setTemp.reseved = 0;
    setTemp.eventSendActive = (setUnion.raw.eventSendActive2 << 1) + (setUnion.raw.eventSendActive1 << 0);
    setTemp.noAck = (setUnion.raw.noAck << 0);
    setTemp.channel1Type = (setUnion.raw.channel1Type3 << 2) + (setUnion.raw.channel1Type2 << 1) + (setUnion.raw.channel1Type1 << 0);
    setTemp.channel2Type = (setUnion.raw.channel2Type3 << 2) + (setUnion.raw.channel2Type2 << 1) + (setUnion.raw.channel2Type1 << 0);
    setTemp.samplingRate = (setUnion.raw.samplingRate7 << 6) + (setUnion.raw.samplingRate6 << 5) + (setUnion.raw.samplingRate5 << 4) + (setUnion.raw.samplingRate4 << 3) + (setUnion.raw.samplingRate3 << 2) + (setUnion.raw.samplingRate2 << 1) + (setUnion.raw.samplingRate1 << 0);


    // 에러 확인
    if(setTemp.channel1Type > 4 || setTemp.channel2Type > 4  || \
       setTemp.samplingRate < 2 || setTemp.samplingRate > 30 || \
       setTemp.eventRevision > 5)
        retval = -1;

    if(eventSocket <= 0)
        retval = -1;

    // 에러 없을 경우
    if(retval == 0)
    {
        // Read start
        socketControl.socketState[eventSocket].monitoring.flagEventGraph = setTemp.eventSendActive;   

        // Graph Torque/Angle check
        if(setTemp.channel1Type == 4)
            setTemp.channel2Type = 0;  // Torque/Angle 사용시 channel2 미사용

        // Index 값 초기화
        if(socketControl.socketState[eventSocket].monitoring.flagEventGraph == 2)
        {
            GraphMem.index.last_send_data_no = GraphMem.index.save_memory_data_no;
            SPI_WriteMemoryData(REALTIME_EVENT_SAVE_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.save_memory_data_no, sizeof(GraphMem.index.save_memory_data_no));
            SPI_WriteMemoryData(REALTIME_EVENT_SEND_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.last_send_data_no, sizeof(GraphMem.index.last_send_data_no));
        }

        // 통신 설정된 값 적용
        GraphMem.setting.eventRevision = setTemp.eventRevision;
        GraphMem.setting.reseved = setTemp.reseved;
        GraphMem.setting.eventSendActive = setTemp.eventSendActive;
        GraphMem.setting.noAck = setTemp.noAck;
        GraphMem.setting.channel1Type = setTemp.channel1Type;
        GraphMem.setting.channel2Type = setTemp.channel2Type;
        GraphMem.setting.samplingRate = setTemp.samplingRate;

        // Graph event setting값 Memory에 저장
        SPI_WriteMemoryData(GRAPH_EVENT_EEPROM_SETTING_ADDR, (uint8_t*)&setUnion.totalVal, 4);
    }
    
    return retval;
}


/**
 * @brief Get the Graph Setting object
 * 
 * @return Graph setting
 */
uint32_t getGraphSetting(void)
{
    uint16_t retval = 0;
    graphEventSettingUnion setUnion;

    setUnion.totalVal = 0;
    setUnion.raw.eventRevision1 = (GraphMem.setting.eventRevision >> 0) & 0x01;
    setUnion.raw.eventRevision2 = (GraphMem.setting.eventRevision >> 1) & 0x01;
    setUnion.raw.eventRevision3 = (GraphMem.setting.eventRevision >> 2) & 0x01;
    setUnion.raw.eventRevision4 = (GraphMem.setting.eventRevision >> 3) & 0x01;
    
    setUnion.raw.eventSendActive1 = (GraphMem.setting.eventSendActive >> 0) & 0x01;
    setUnion.raw.eventSendActive2 = (GraphMem.setting.eventSendActive >> 1) & 0x01;

    setUnion.raw.noAck = (GraphMem.setting.noAck >> 0) & 0x01;

    setUnion.raw.channel1Type1 = (GraphMem.setting.channel1Type >> 0) & 0x01;
    setUnion.raw.channel1Type2 = (GraphMem.setting.channel1Type >> 1) & 0x01;
    setUnion.raw.channel1Type3 = (GraphMem.setting.channel1Type >> 2) & 0x01;

    setUnion.raw.channel2Type1 = (GraphMem.setting.channel2Type >> 0) & 0x01;
    setUnion.raw.channel2Type2 = (GraphMem.setting.channel2Type >> 1) & 0x01;
    setUnion.raw.channel2Type3 = (GraphMem.setting.channel2Type >> 2) & 0x01;

    setUnion.raw.samplingRate1 = (GraphMem.setting.samplingRate >> 0) & 0x01;
    setUnion.raw.samplingRate2 = (GraphMem.setting.samplingRate >> 1) & 0x01;
    setUnion.raw.samplingRate3 = (GraphMem.setting.samplingRate >> 2) & 0x01;
    setUnion.raw.samplingRate4 = (GraphMem.setting.samplingRate >> 3) & 0x01;
    setUnion.raw.samplingRate5 = (GraphMem.setting.samplingRate >> 4) & 0x01;
    setUnion.raw.samplingRate6 = (GraphMem.setting.samplingRate >> 5) & 0x01;
    setUnion.raw.samplingRate7 = (GraphMem.setting.samplingRate >> 6) & 0x01;


    retval = setUnion.totalVal;
        
    return retval;
}


/**
 * @brief 초기 부팅중 메모리에서 Graph memroy event 설정을 읽어 설정
 */
void initGraphMemSetting(void)
{
    graphEventSettingUnion setUnion;

    // Index
    SPI_ReadMemoryData(REALTIME_EVENT_SAVE_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.save_memory_data_no, 2);        // 저장할 index
    SPI_ReadMemoryData(REALTIME_EVENT_SEND_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.last_send_data_no, 2);          // 전송할 index

    // Graph index
    SPI_ReadMemoryData(GRAPH_EVENT_GRAPH_SAVE_INDEX_ADDR, (uint8_t*)&GraphMem.graphIndex.saveGraphIndex, 2);        // Graph 저장할 index
    SPI_ReadMemoryData(GRAPH_EVENT_CYCLE_INDEX_ADDR, (uint8_t*)&GraphMem.graphIndex.cycleIndex, 2);                 // Graph까지 전송이 가능한 index 시작 주소

    // 데이터 분석
    SPI_ReadMemoryData(GRAPH_EVENT_EEPROM_SETTING_ADDR, (uint8_t*)&setUnion.totalVal, 4);

    GraphMem.setting.eventRevision = (setUnion.raw.eventRevision4 << 3) + (setUnion.raw.eventRevision3 << 2) + (setUnion.raw.eventRevision2 << 1) + (setUnion.raw.eventRevision1 << 0);
    GraphMem.setting.reseved = 0;
    GraphMem.setting.eventSendActive = (setUnion.raw.eventSendActive2 << 1) + (setUnion.raw.eventSendActive1 << 0);
    GraphMem.setting.noAck = (setUnion.raw.noAck << 0);
    GraphMem.setting.channel1Type = (setUnion.raw.channel1Type3 << 2) + (setUnion.raw.channel1Type2 << 1) + (setUnion.raw.channel1Type1 << 0);
    GraphMem.setting.channel2Type = (setUnion.raw.channel2Type3 << 2) + (setUnion.raw.channel2Type2 << 1) + (setUnion.raw.channel2Type1 << 0);
    GraphMem.setting.samplingRate = (setUnion.raw.samplingRate7 << 6) + (setUnion.raw.samplingRate6 << 5) + (setUnion.raw.samplingRate5 << 4) + (setUnion.raw.samplingRate4 << 3) + (setUnion.raw.samplingRate3 << 2) + (setUnion.raw.samplingRate2 << 1) + (setUnion.raw.samplingRate1 << 0);

    // Default setting
    if(GraphMem.setting.samplingRate == 0)
        GraphMem.setting.samplingRate = 5;
}


/**
 * @brief 현재 메모리에 전송하지 않고 남아있는 Graph 개수를 구한다.
 * 
 * @return uint16_t retval: 남은 Graph 리스트의 개수
 */
uint16_t graphRemainData(void)
{
    uint16_t retval;

    // 저장 위치가 전송 위치보다 이전일 때
    if(GraphMem.index.save_memory_data_no >= GraphMem.index.last_send_data_no)
    {
        retval = GraphMem.index.save_memory_data_no - GraphMem.index.last_send_data_no;
    }
    else
    {
        // 전송 위치가 저장위치 보다 클 때 (저장위치가 Memroy를 1바퀴 회전하였다)
        if(memoryQspi.flashOperation == ON)
            retval = GRAPH_EVENT_MAX_INDEX - GraphMem.index.last_send_data_no + GraphMem.index.save_memory_data_no + 1;             // 메모리 동작 O
        else
            retval = GRAPH_EVENT_NON_MEM_MAX_INDEX - GraphMem.index.last_send_data_no + GraphMem.index.save_memory_data_no + 1;     // 메모리 동작 X
    }

    return retval;
}


/**
 * @brief Graph 전송 후 Ack 관리
 */
void ackWaitManage(void)
{
    static uint32_t ackWaitTime;

    // No ack 관리
    if(GraphMem.setting.noAck == ON && GraphMem.ack.eventAckWait)
    {
        // 200ms 대기 후 초기화 (다음 전송을 위해)
        if(HAL_GetTick() - ackWaitTime > 200 && HAL_GetTick() > 200)
        {
            initGraphEventAckWait();

            // Index 증가
            ++GraphMem.index.last_send_data_no;
            graphMemIndexManage();
        }
    }

    // Check ack wait time
    if(GraphMem.ack.eventAckWait && ackWaitTime == 0)
        ackWaitTime = HAL_GetTick();
    else if(GraphMem.ack.eventAckWait == OFF)
        ackWaitTime = 0;

    // Wait timeout 1초 (Ack 실패 확인)
    if(HAL_GetTick() - ackWaitTime > 1000 && GraphMem.ack.eventAckWait && HAL_GetTick() > 1000)
    {
        GraphMem.ack.eventAckFailCount++;
        GraphMem.ack.eventAckWait = OFF;
        ackWaitTime = 0;

        // Multicast message on
        setAdvertiseMsg(MULTICAST_ADVERTISE, DONT_RECV_ACK);
    }

    // 3회 실패
    if(GraphMem.ack.eventAckFailCount > MAX_RETRY_FAIL)
    {
        // Init ack value
        initGraphEventAckWait();
        ackWaitTime = 0;

        // Graph 모니터링 종료
        for(uint8_t i=0; i <= RSI_NUMBER_OF_SOCKETS; i++)
            socketControl.socketState[i].monitoring.flagEventGraph = OFF;

        // Event TCP socket 종료 명령 실행
        socketClose(TCP_SERVER_EVENT_SOCKET);
        socketClose(TCP_CLIENT_EVENT_SOCKET);
    }
}


/**
 * @brief Ack 응답 받은 후 Ack 관련 값 초기화
 */
void initGraphEventAckWait(void)
{
    GraphMem.ack.eventAckWait = OFF;
    GraphMem.ack.eventAckFailCount = 0;
}


/**
 * @brief 메모리에 저장된 Realtime 데이터에서 graph data index 위치를 구한다
 * 
 * @param buf : Reatlime data pointer
 * @return uint16_t : Graph 저장 index 번호
 */
uint16_t getGraphMemCycleIndex(uint8_t* buf)
{
    uint16_t retval = 0;
    uint16_t graphIndex, graphCycle;
    eventFasteningRevision0Struct* bufPointer0;
    uint8_t active = 0;


    // Get graph의 cycle, index 번호
    graphCycle = (buf[GRAPH_EVENT_G_CYCLE_H_INDEX + MODBUS_TCP_HEADER_SIZE] << 8) + buf[GRAPH_EVENT_G_CYCLE_L_INDEX + MODBUS_TCP_HEADER_SIZE];
    graphIndex = (buf[GRAPH_EVENT_G_MEM_H_INDEX + MODBUS_TCP_HEADER_SIZE] << 8) + buf[GRAPH_EVENT_G_MEM_L_INDEX + MODBUS_TCP_HEADER_SIZE];


    // Check graph data 함유
    bufPointer0 = (eventFasteningRevision0Struct*)(buf + MODBUS_TCP_HEADER_SIZE + sizeof(graphEventHeaderStruct));

    switch (bufPointer0->revision)
    {
        case 0:
            if(bufPointer0->graphInfo.channel1Count || bufPointer0->graphInfo.channel2Count)
                active = ON;
        break;
    }

    // 그래프 데이터가 있을 경우
    if(active)
    {
        // Check graph cycle
        if(GraphMem.graphIndex.cycleIndex == graphCycle)
        {
            retval = graphIndex;
        }
        else
        {
            // 현재 cycle 번호와 저장되어 있는 cycle 번호가 일치하지 않는 경우
            if(GraphMem.graphIndex.cycleIndex == 0 && graphCycle == 0xFFFF)
            {
                // Cycle이 overflow 되었을 때
                if(graphIndex >= GraphMem.graphIndex.saveGraphIndex)
                    retval = graphIndex;
            }
            else if(GraphMem.graphIndex.cycleIndex - 1 == graphCycle)
            {
                // 이전 Cycle일 때
                if(graphIndex >= GraphMem.graphIndex.saveGraphIndex)
                    retval = graphIndex;
            }
        }
    }
    else
    {
        retval = 0;
    }


    // Check limit
    if(retval >= GRAPH_EVENT_MAX_INDEX)
        retval = 0;

    return retval;
}


/**
 * @brief 고해상도 Graph - Revision 별 Realtime 데이터의 크기를 구한다
 * 
 * @return uint16_t :realtime data 크기
 */
uint16_t getGraphRealimeSize(void)
{
    uint16_t retval = 0;

    switch (GraphMem.setting.eventRevision)
    {
        case 0:
            retval = sizeof(eventFasteningRevision0Struct);
        break;
    }

    return retval;
}


uint16_t checkGraphDataActive(uint8_t* buf)
{
    uint16_t retval = 0;
    eventFasteningRevision0Struct* bufPointer0;

    // Check graph data 함유
    bufPointer0 = (eventFasteningRevision0Struct*)(buf + sizeof(graphEventHeaderStruct) + MODBUS_TCP_HEADER_SIZE);

    switch (bufPointer0->revision)
    {
        case 0:
            if(bufPointer0->graphInfo.channel1Count || bufPointer0->graphInfo.channel2Count)
                retval = ON;
        break;
    }

    return retval;
}







/* Graph 모니터링 및 설정 -------------------------------------------------------------------------------*/

/**
 * @brief 메모리에 저장하기 위한 그래프 모니터링
 */
void graphMonitoringEvent(void)
{
    uint16_t        i=0;
    uint16_t        tmpData0  = 0, tmpData1  = 0;

    float           SampingPulseValue;
    float           SampingPulseValueAdd; 
    static int      Cnt_monitoringDataForTqAngle;

    static int      run_edge[2] = {0,0};
    static uint8_t  targetTorqueOn;
    static uint8_t  samplingRate = 8;
    static uint8_t  SamplingAngle;                          // Torque/Angle 변수
    static uint8_t  channelType[2];
    static float    (*buf)[GRAPH_EVENT_CHANNEL_SIZE];     // 실제 데이터가 저장될 버퍼 포인터
    static uint16_t  bufIndex;
    static uint8_t   overFlow;
    static uint8_t   bufType;

    // Graph 저장 Mode 확인
    if(GraphMem.setting.channel1Type == 0 && GraphMem.setting.channel2Type == 0)
    {
        GraphMem.makeData.dataPoint = 0;
        GraphMem.makeData.flagOverflow = 0;

        run_edge[1] = OFF;
        return;
    }
    
    // Save Run state 
    run_edge[0] = run_edge[1];
    run_edge[1] = Flag.Run | Flag.RunningMultisequence;

    // Check driver run
    if (run_edge[0] < run_edge[1])
    {
        // Graph의 버퍼 index 설정
        if(bufType == 0)
            buf = GraphMem.makeData.graphData1;
        else
            buf = GraphMem.makeData.graphData2;

        bufIndex = 0;
        overFlow = 0;
        targetTorqueOn = 0;
        Cnt_monitoringDataForTqAngle = 1;

        // Set sampling rate
        samplingRate = GraphMem.setting.samplingRate * GRAPH_EVENT_SAMPLING_1MS;
        SamplingAngle = GraphMem.setting.samplingRate;

        channelType[0] = graphChannelMapping(GraphMem.setting.channel1Type);
        channelType[1] = graphChannelMapping(GraphMem.setting.channel2Type);
    }
    else if (run_edge[0] > run_edge[1]) // stop
    {
        // Save memory flag ON
        GraphMem.makeData.flagSaveGraph = ON;

        // Graph step
        if(overFlow)
        {
            // step change
            saveGraphStepChange(&graphEventStepInfo, GRAPH_EVENT_CHANNEL_SIZE, graphEventMonStepQueue);

            // Step increase
            increaseGraphStepIndex(&graphEventStepInfo);
        }
        else
            saveGraphStepChange(&graphEventStepInfo, bufIndex, graphEventMonStepQueue);


        // 최종 토크 표시
        if(targetTorqueOn == ON)
        {
            for (i=0; i < 2; i++) 
            {
                buf[i][bufIndex] = SelectDisplayData(channelType[i]);

                // 토크 모니터링 중일 때 최종 토크 표시
                if(channelType[i] == 1)
                {
                    buf[i][bufIndex] = SelectDisplayData(20);
                    targetTorqueOn = 2;
                }
            }

            bufIndex++;

            if (bufIndex > GRAPH_EVENT_CHANNEL_SIZE - 1)
            {
                bufIndex = 0;
                overFlow = ON;
            }   
        }

        // GRAPH_EVENT_CHANNEL_SIZE - 1 보다 작으면 마지막에 0을 넣는다.
        if (overFlow == OFF && bufIndex < GRAPH_EVENT_CHANNEL_SIZE - 1)		
        {
            tmpData0 = tmpData1 = 0;

            // Angle 은 기존 값 유지
            if (channelType[0] == 4)
                tmpData0  = buf[0][bufIndex - 1];

            if (channelType[1] == 4)
                tmpData1  = buf[1][bufIndex - 1];

            buf[0][bufIndex] = tmpData0;
            buf[1][bufIndex] = tmpData1;

            bufIndex++;				
        }		

        // Graph 설정 값 저장
        GraphMem.makeData.dataPoint = bufIndex;
        GraphMem.makeData.flagOverflow = overFlow;
        memcpy(GraphMem.makeData.stepInfo, graphEventStepInfo.stepInfo, sizeof(graphStepStruct) * GRAPH_STEP_INDEX_SIZE);

        if(bufType == 0)
        {
            bufType = 1;
            GraphMem.makeData.graphBufType = 0;
        }
        else
        {
            bufType = 0;
            GraphMem.makeData.graphBufType = 1;
        }
    }

    // 동작 중 Graph data 저장
    if (Flag.Run || Flag.RunningMultisequence)
    {
        // Torque / Angle 모니터링 확인
        if(channelType[0] == 8)
        {
            // Clear channel 2
            channelType[1] = 0;                     
            

            SampingPulseValue = MathConv.SamplingPulsConv * (float)SamplingAngle;       // samping pulse count
            SampingPulseValueAdd = SampingPulseValue * (float)Cnt_monitoringDataForTqAngle;
            if (SampingPulseValueAdd <= mPulse.After_Fastening_Start_abs)
            {
                // Graph step
                if(overFlow)
                {
                    // step change
                    saveGraphStepChange(&graphEventStepInfo, GRAPH_EVENT_CHANNEL_SIZE, graphEventMonStepQueue);

                    // Step increase
                    increaseGraphStepIndex(&graphEventStepInfo);
                }
                else
                    saveGraphStepChange(&graphEventStepInfo, bufIndex, graphEventMonStepQueue);


                buf[0][bufIndex] = SelectDisplayData(1);   // 토크 값
                
                Cnt_monitoringDataForTqAngle++;
                bufIndex++;
            }
        }
        else
        {
            // 최종 토크 표시
            if (FlagFasten.FindedTargetTorqueStep == ON && targetTorqueOn == OFF)
                targetTorqueOn = ON;

            if(!(cnt_250us % samplingRate) && (bufIndex < GRAPH_EVENT_CHANNEL_SIZE))
            {
                // Graph step
                if(overFlow)
                {
                    // step change
                    saveGraphStepChange(&graphEventStepInfo, GRAPH_EVENT_CHANNEL_SIZE, graphEventMonStepQueue);

                    // Step increase
                    increaseGraphStepIndex(&graphEventStepInfo);
                }
                else
                    saveGraphStepChange(&graphEventStepInfo, bufIndex, graphEventMonStepQueue);


                for (i=0; i < 2; i++) 
                {
                    buf[i][bufIndex] = SelectDisplayData(channelType[i]);
                } 

                // 토크 모니터링 중일 때 최종 토크 표시
                for (i=0; i < 2; i++) 
                {
                    if(targetTorqueOn == ON)
                    {
                        if(channelType[i] == 1)
                            buf[i][bufIndex] = SelectDisplayData(20);
                        else
                            buf[i][bufIndex] = SelectDisplayData(channelType[i]);

                        targetTorqueOn = 2;
                    }
                }
                
                bufIndex++;
            }
        }
        
        // Overflow 감지
        if (bufIndex > GRAPH_EVENT_CHANNEL_SIZE - 1)
        {
            bufIndex = 0;
            overFlow = ON;
        }   
    }
}


/**
 * @brief 현재 발생된 이벤트가 그래프가 필요한 이벤트인지 판단
 * 
 * @param fasteningTime : 체결 시간
 * @return int8_t : 0 = 필요 없음, 1 = 그래프 필요 
 */
int8_t graphMonitoringNeed(pollingRealtimeDataStruct* realtime)
{
    int8_t retval = OFF;

    // 채널 활성화 여부 판단
    if(GraphMem.setting.channel1Type || GraphMem.setting.channel2Type)
    {
        // 그래프 결과가 존재한다면 받는다
        if(realtime->fasteningTime)
            retval = ON;
        else
            retval = OFF;
    }

    return retval;
}


/**
 * @brief 설정된 그래프 채널 타입을 모니터링 값에 맞도록 변환
 * 
 * @param channelType : 0 ~ 4 그래프 타입
 * @return uint8_t retval: 변환된 그래프 타입 - 0: Disable, 1: Torque, 3: speed, 4: angle, 8: torque/angle
 */
uint8_t graphChannelMapping(uint8_t channelType)
{
    uint8_t retval;

    // 모니터링 설정 Type에 따라 변환
    switch (channelType)
    {
        case 0:
            retval = 0;         // Disable
            break;
        case 1:
            retval = 1;         // Torque 선택할 수 있도록 변환
            break;
        case 2:
            retval = 3;         // Speed
            break;
        case 3:
            retval = 4;         // Angle
            break;
        case 4:
            retval = 8;         // Torque/Angle
            break;

        default:
            retval = 0;
            break;
    }

    return retval;
}


/**
 * @brief Realtime 결과 생성 완료 되었을 때 result 상태를 보고 realtime을 ON 시킨다.
 */
void fasteningResultOn(void)
{
    uint16_t check_data;

    check_data = (Para[SELECT_DATA_TO_SAVE].val >> mbMData[REALTIME_STATUS_INDEX]) & 0x01;

    // 저장 유형 확인
    if(check_data)
    {
        // 저장 활성화
        GraphMem.makeData.flagRealtimeOn = ON; 
    }
}






/* Flash memory가 없을 때 데이터 처리 ------------------------------------------------------------------------------------------------*/

/**
 * @brief Flash 메모리가 없을 때 Setting 값 변경
 * 
 */
void memoryNoneExistSet(void)
{
    static uint8_t init = 0;

    // QSPI flash 메모리가 없을 때 처리
    if(memoryQspi.flashOperation == OFF)
    {
        // 초기 index 설정
        if(init == 0)
        {
            // Realtime Index 초기화
            GraphMem.index.save_memory_data_no = 0;
            GraphMem.index.last_send_data_no = 0;
            
            SPI_WriteMemoryData(REALTIME_EVENT_SAVE_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.save_memory_data_no, sizeof(GraphMem.index.save_memory_data_no));
            SPI_WriteMemoryData(REALTIME_EVENT_SEND_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.last_send_data_no, sizeof(GraphMem.index.last_send_data_no));


            // Graph Index 초기화
            GraphMem.graphIndex.saveGraphIndex = 0;
            GraphMem.graphIndex.cycleIndex = 0;
            
            SPI_WriteMemoryData(GRAPH_EVENT_GRAPH_SAVE_INDEX_ADDR, (uint8_t*)&GraphMem.graphIndex.saveGraphIndex, sizeof(GraphMem.graphIndex.saveGraphIndex));
            SPI_WriteMemoryData(GRAPH_EVENT_CYCLE_INDEX_ADDR, (uint8_t*)&GraphMem.graphIndex.cycleIndex, sizeof(GraphMem.graphIndex.cycleIndex));

            init = ON;
        }
    }  
}


/**
 * @brief Flash 메모리가 없을 때 Graph event를 저장하는 함수
 * 
 * @param index     : 저장하는 위치 index
 * @param eventBuf  : 이벤트 저장 buffer
 * @param dataBuf   : 저장할 데이터
 * @param length    : 저장할 개수
 */
void saveNoneMemory(uint16_t index, uint8_t* eventBuf, uint8_t* dataBuf, uint16_t length)
{
    // Limit index, length
    if(index > GRAPH_EVENT_NON_MEM_MAX_INDEX || length > MAX_EVENT_SEND_SIZE)
        return;

    // Copy
    memcpy(eventBuf, dataBuf, length);
}


/**
 * @brief Flash 메모리가 없을 때 저장한 Graph event를 읽는 함수
 * 
 * @param index     : 읽을 위치 index
 * @param eventBuf  : 이벤트 저장된 buffer
 * @param saveBuf   : 읽은 데이터 저장할 buffer
 * @param length    : 읽을 데이터 개수
 */
void readNoneMemory(uint16_t index, uint8_t* eventBuf, uint8_t* saveBuf, uint16_t length)
{
    // Limit index, length
    if(index > GRAPH_EVENT_NON_MEM_MAX_INDEX || length > MAX_EVENT_SEND_SIZE)
        return;

    // Copy
    memcpy(saveBuf, eventBuf, length);
}







/* Graph data Queue ------------------------------------------------------------------------------------------------*/

void enQueueGraphMem(uint8_t* reatlimeData, uint8_t* graphData, uint16_t length)
{
    uint8_t index;

    // Check queue limit
    if(GraphMem.queue.remainSize >= GRAPH_MEM_QUEUE_SIZE)
        return;

    // Enqueue 데이터
    index = GraphMem.queue.startIndex + GraphMem.queue.remainSize;

    memcpy(GraphMem.queue.realtimeBuf[index], reatlimeData, sizeof(GraphMem.queue.realtimeBuf[index]));   // Realtime data
    memcpy(GraphMem.queue.graphBuf[index], graphData, length);                                            // Graph data
    GraphMem.queue.queueLegthBuf[index] = length;                                                         // Graph data length

    // Remain 증가
    GraphMem.queue.remainSize++;
}


uint8_t deQueueGraphMem(uint8_t* saveRealtimeBuf, uint8_t* saveGraphBuf, uint16_t* length)
{
    uint8_t index;
    uint8_t retval = 0;

    // Check queue remain data
    if(GraphMem.queue.remainSize)
    {
        // Dequeue 데이터
        index = GraphMem.queue.startIndex;

        memcpy(saveRealtimeBuf, GraphMem.queue.realtimeBuf[index], sizeof(GraphMem.queue.realtimeBuf[index]));   // Realtime data
        memcpy(saveGraphBuf, GraphMem.queue.graphBuf[index], GraphMem.queue.queueLegthBuf[index]);                // Graph data
        *length = GraphMem.queue.queueLegthBuf[index];                                                            // Graph data length

        // Index 증가
        GraphMem.queue.startIndex++;
        GraphMem.queue.remainSize--;

        // Check index limit
        if(GraphMem.queue.startIndex >= GRAPH_MEM_QUEUE_SIZE)
            GraphMem.queue.startIndex = 0;

        retval = ON;
    }

    return retval;
}



/* Graph data Endian change ------------------------------------------------------------------------------------------------*/

void eventConvertEndian(uint8_t* buf, uint16_t length)
{
    uint8_t tempBuf[4];

    // uint16_t offset;
    // offset = sizeof(graphEventHeaderStruct) + sizeof(eventFasteningRevision0Struct);
    // printf("\nG2 Len: %x, %x\n", buf[0], buf[1]);
    // printf("G2 R1: %x, %x, %x, %x\n", buf[0 + 6], buf[1 + 6], buf[2 + 6], buf[3 + 6]);
    // printf("G2 D1: %x, %x, %x, %x\n\n", buf[0 + offset], buf[1 + offset], buf[2 + offset], buf[3 + offset]);


    // Make big endian
    for(uint16_t index = sizeof(graphEventHeaderStruct); index < length; index += sizeof(uint16_t))
    {
        tempBuf[0] = buf[index + 1];
        tempBuf[1] = buf[index];

        memcpy(&buf[index], tempBuf, sizeof(uint16_t));
    }

    // Make big endian: graph data
    for(uint16_t index = sizeof(graphEventHeaderStruct) + sizeof(eventFasteningRevision0Struct); index < length; index += sizeof(float))
    {
        tempBuf[0] = buf[index + 2];
        tempBuf[1] = buf[index + 3];
        tempBuf[2] = buf[index + 0];
        tempBuf[3] = buf[index + 1];

        memcpy(&buf[index], tempBuf, sizeof(float));
    }

    // printf("T2 R1: %x, %x, %x, %x\n", buf[0 + 6], buf[1 + 6], buf[2 + 6], buf[3 + 6]);
    // printf("T2 D1: %x, %x, %x, %x\n\n", buf[0 + offset], buf[1 + offset], buf[2 + offset], buf[3 + offset]);
}

