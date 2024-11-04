/**
  ******************************************************************************
  * @file    eventRealtimeTask.c
  * @author  jun
  * @brief   Source file of monitoring event task.
  * @note    Monitoring event 메모리 저장 및 데이터 전송
  ******************************************************************************
  */

/* Include ---------------------------------------------------------------------*/
#include "eventRealtimeTask.h"
#include "common.h"
#include "variable.h"
#include "realtimeMonitoring.h"
#include "backup.h"
#include "modbus.h"
#include "wifiTask.h"
#include "socketControl.h"
#include "memoryQspiControl.h"
#include "eventGraphTask.h"
#include "multicastludp.h"


/* Variable ---------------------------------------------------------------------*/
RealtimeMemStruct realtimeMem;


/* Extern -----------------------------------------------------------------------*/


/* Static Functions -----------------------------------------------------------------------*/
static void ackWaitManage(void);


/* Funcions ----------------------------------------------------------------------*/

/* Init --------------------------------------------------------------------*/

/**
 * @brief 초기 부팅중 메모리에서 Realtime memory event 설정을 읽어 설정
 */
void initRealtimeMemSetting(void)
{
    // Setting
    SPI_ReadMemoryData(REALTIME_EVENT_SAVE_1TURN_OVER_ADDRESS, &realtimeMem.setting.memorySaved1TurnOver, 1);      // Memory 1turn over
}


/**
 * @brief 고해상도 Graph event - Realtime memory의 index를 관리.
 */
void realtimeMemIndexManage(void)
{
    static uint32_t lastSendIndex;
    static uint32_t lastSaveIndex;
    static uint8_t  initFlag;


    // 초기 Set send, save index
    if(initFlag == OFF)
    {
        lastSendIndex = GraphMem.index.last_send_data_no;
        lastSaveIndex = GraphMem.index.save_memory_data_no;

        initFlag = ON;
    }

    // Index가 Max size 초과 시 초기화
    if(memoryQspi.flashOperation == ON)
    {
        // 저장 메모리 있을 경우 (32768)
        if (GraphMem.index.save_memory_data_no > REALTIME_EVENT_MAX_INDEX)
            GraphMem.index.save_memory_data_no = 0;

        if (GraphMem.index.last_send_data_no > REALTIME_EVENT_MAX_INDEX)
            GraphMem.index.last_send_data_no = 0;
    }
    else
    {
        // 저장 메모리 없을 경우 - 최대 저장 개수 변경 (20)
        if (GraphMem.index.save_memory_data_no > GRAPH_EVENT_NON_MEM_MAX_INDEX)
            GraphMem.index.save_memory_data_no = 0;

        if (GraphMem.index.last_send_data_no > GRAPH_EVENT_NON_MEM_MAX_INDEX)
            GraphMem.index.last_send_data_no = 0;
    }


    // 그래프 Overflow 관리 - 그래프 Index가 같아졌을 때
    if(GraphMem.index.last_send_data_no == GraphMem.index.save_memory_data_no)
    {
        // Index 초기화 확인
        if(GraphMem.index.last_send_data_no == 0 && GraphMem.index.save_memory_data_no == 0)
        {
            // 초기화가 되지 않았을 경우 (0, 0으로 되었을 때 overflow)
            if(lastSendIndex == 0 && (lastSaveIndex == REALTIME_EVENT_MAX_INDEX || lastSaveIndex == GRAPH_EVENT_NON_MEM_MAX_INDEX))
                GraphMem.index.last_send_data_no++;
        }
        // Send index는 안 바뀌고, Save index만 바뀐 경우 (Save index가 증가로 Overflow 되서 index가 같아진다)
        else if(lastSendIndex == GraphMem.index.last_send_data_no && lastSaveIndex != GraphMem.index.save_memory_data_no && lastSaveIndex == lastSendIndex - 1)
        {
            GraphMem.index.last_send_data_no++;
        }
    }

    // Index overflow 관리
    if(memoryQspi.flashOperation == ON)
    {
        if (GraphMem.index.last_send_data_no > REALTIME_EVENT_MAX_INDEX)
            GraphMem.index.last_send_data_no = 0;

        if (GraphMem.index.save_memory_data_no > REALTIME_EVENT_MAX_INDEX)
            GraphMem.index.save_memory_data_no = 0;
    }
    else
    {
        if (GraphMem.index.last_send_data_no > GRAPH_EVENT_NON_MEM_MAX_INDEX)
            GraphMem.index.last_send_data_no = 0;

        if (GraphMem.index.save_memory_data_no > GRAPH_EVENT_NON_MEM_MAX_INDEX)
            GraphMem.index.save_memory_data_no = 0;
    }

    // Index 저장
    if(memoryQspi.flashOperation == ON)
    {
        // Index 변경되었으면 저장
        if(lastSaveIndex != GraphMem.index.save_memory_data_no)
        	SPI_WriteMemoryData(REALTIME_EVENT_SAVE_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.save_memory_data_no, sizeof(GraphMem.index.save_memory_data_no));

        if(lastSendIndex != GraphMem.index.last_send_data_no)
        	SPI_WriteMemoryData(REALTIME_EVENT_SEND_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.last_send_data_no, sizeof(GraphMem.index.last_send_data_no));
    }

    // Save index old
    lastSendIndex = GraphMem.index.last_send_data_no;
    lastSaveIndex = GraphMem.index.save_memory_data_no;
}



/* Send --------------------------------------------------------------------*/

/**
 * @brief USB serial로 Realtime event 데이터를 모두 전송
 */
void realtimeEventSendTo232(void) 
{
    uint32_t result;
    uint16_t sendLength;
    static uint32_t delayTime;

    // Check send flag
    if(realtimeMem.setting.Flag_SendBackupDataAll == OFF)
        return;

    // 10ms Delay 없이 할 경우 Recv측에서 에러
    if(HAL_GetTick() - delayTime > 10 && HAL_GetTick() > 10)
    {
        // Index 같을 때까지
        if (GraphMem.index.save_memory_data_no != GraphMem.index.last_send_data_no)
        {
            result = 1;

            // Read realtime data from memory
            readMemoryData(REALTIME_EVENT_BASE_ADDRESS, GraphMem.index.last_send_data_no, REALTIME_EVENT_DATA_SIZE, (uint8_t*)make_backupRead_data, REALTIME_EVENT_DATA_SIZE);     
            
            // Check data
            if(make_backupRead_data[0] == 0xFFFF && make_backupRead_data[0 + 1] == 0xFFFF)
            {
                memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
                return;
            }

            // 전송 data 길이 구하기
            sendLength = sizeof(pollingRealtimeDataStruct) / sizeof(uint16_t);                 

            // Send
            result = mbMakeandSend_eventMonitoring(GRAPH_REALTIME_START_ADDRESS, sendLength, MODBUS_RS232_TYPE);

            // if send ok, incress the send count no.
            // if (result == RSI_SUCCESS)      
            {
                ++GraphMem.index.last_send_data_no;
                SPI_WriteMemoryData(REALTIME_EVENT_SAVE_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.last_send_data_no, sizeof(GraphMem.index.last_send_data_no)); 	// send memory data no
                CheckMaxValue();
            }            

            delayTime = HAL_GetTick();
        }
        else
        {
            realtimeMem.setting.Flag_SendBackupDataAll = OFF;   // send complete
            delayTime = 0;
        }
    }    
}


/**
 * @brief USB serial로 Realtime 데이터의 개수 선택하여 전송
 */
void realtimeEventSendSeleNumTo232(void)
{
    uint32_t result;
    uint16_t sendLength;
    static uint32_t delayTime;

    // Check send flag
    if(realtimeMem.setting.Flag_SendBackupSelecNum == OFF)
        return;

    // 10ms Delay 없이 할 경우 Recv측에서 에러
    if(HAL_GetTick() - delayTime > 10 && HAL_GetTick() > 10)
    {
        // Index 같을 때까지
        if (realtimeMem.indexSelect.save_memory_data_no != realtimeMem.indexSelect.last_send_data_no)
        {
            result = 1;

            // Read realtime data from memory
            readMemoryData(REALTIME_EVENT_BASE_ADDRESS, realtimeMem.indexSelect.last_send_data_no, REALTIME_EVENT_DATA_SIZE, (uint8_t*)make_backupRead_data, REALTIME_EVENT_DATA_SIZE);     
            
            // Check data
            if(make_backupRead_data[0] == 0xFFFF && make_backupRead_data[0 + 1] == 0xFFFF)
            {
                memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
                return;
            }

            // 전송 data 길이 구하기
            sendLength = sizeof(pollingRealtimeDataStruct) / sizeof(uint16_t);

            // Send
            result = mbMakeandSend_eventMonitoring(GRAPH_REALTIME_START_ADDRESS, sendLength, MODBUS_RS232_TYPE);

            // if send ok, incress the send count no.
            // if (result == RSI_SUCCESS)      
            {
                // Index 관리
                ++realtimeMem.indexSelect.last_send_data_no;
                if (realtimeMem.indexSelect.last_send_data_no > REALTIME_EVENT_MAX_INDEX)		//  다시 0 부터 저장한다.
                {
                    realtimeMem.indexSelect.last_send_data_no = 0;
                }   
            }        

            delayTime = HAL_GetTick();
        }
        else
        {
            realtimeMem.setting.Flag_SendBackupSelecNum = OFF;   // send complete
            delayTime = 0;
        }
    }
}


/**
 * @brief Realtime event send to Wi-Fi
 */
void realtimeEventSendToWifi(void)
{
    static uint32_t sendTimeout;
    uint32_t result;
    uint8_t  socketNum;
    uint16_t sendLength;
    
    // Ack 관리
    ackWaitManage();

    // Send timeout (전송 후 10ms 이상 지나면 재전송)
    if(HAL_GetTick() - sendTimeout > 10 && HAL_GetTick() > 10)
        sendTimeout = HAL_GetTick();
    else
        return;

    // Find event realtime socket number
    socketNum = getEventSocketActive(MONITORING_EVENT_REALTIME);

    // data를 하나씩 보낸다. main loop 동작 유지..
    if ((GraphMem.index.save_memory_data_no != GraphMem.index.last_send_data_no) && socketControl.socketState[socketNum].connect)
    {
        // Ack 기다리고 있지 않다면
        if (GraphMem.ack.eventAckWait == OFF)
        {
            result = 1;
            
            // Read realtime data from memory
            readMemoryData(REALTIME_EVENT_BASE_ADDRESS, GraphMem.index.last_send_data_no, REALTIME_EVENT_DATA_SIZE, (uint8_t*)make_backupRead_data, REALTIME_EVENT_DATA_SIZE);
            
            // Check data
            if(make_backupRead_data[0] == 0xFFFF && make_backupRead_data[0 + 1] == 0xFFFF)
            {
                memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
                return;
            }
            
            // 전송 data 길이 구하기
            sendLength = sizeof(pollingRealtimeDataStruct) / sizeof(uint16_t);                 // 전송 data 길이 구하기

            // Send
            changeLastSocket(socketNum, ON);
            result = mbMakeandSend_eventMonitoring(GRAPH_REALTIME_START_ADDRESS, sendLength, MODBUS_ETHERNET_TYPE);
            changeLastSocket(socketNum, OFF);

            // if (result == RSI_SUCCESS)      // if send ok, incress the send count no.
            {
                realtimeMem.ack.eventAckWait = ON;            // data send 이후, ack check 동안 기다리기 위해서..
                sendTimeout = HAL_GetTick();
            }  
        }
    }
}



/* index --------------------------------------------------------------------*/
/**
 * @brief Realtime event의 index overflow 관리
 */
void CheckMaxValue(void)
{
    uint16_t data;

    // Index 확인
    if (GraphMem.index.last_send_data_no > REALTIME_EVENT_MAX_INDEX)
    {
        GraphMem.index.last_send_data_no = 0;
        SPI_WriteMemoryData(REALTIME_EVENT_SAVE_INDEX_ADDRESS, (uint8_t*)&GraphMem.index.last_send_data_no, sizeof(GraphMem.index.last_send_data_no)); 	// send memory data no

        // Save memory 1 turn 
        data = ON;
        SPI_WriteMemoryData(REALTIME_EVENT_SAVE_1TURN_OVER_ADDRESS, (uint8_t*)&data, 1);
    }   
}




/* 전송 모드 선택 --------------------------------------------------------------------*/

/**
 * @brief Set the Realtime Select object
 * 
 * @param data    : 모드 선택     0 = 모든 데이터 받기, 이외 = 일정 개수 선택 받기, 범위초과 - 에러
 * @return retval : 0 = 에러, 1 = 정상
 */
uint8_t setRealtimeSelect(uint16_t data)
{
    uint8_t retval = 1;

    // Data 값에 따라 동작 모드 선택
    if (data == 0)
    {
        // 남은 모든 realtime data 받기
        realtimeMem.setting.Flag_SendBackupDataAll = ON;
    }
    // 조건은 모든 데이타를 보낸 상태에서만 다시 받을 수 있다.
    else if (GraphMem.index.save_memory_data_no == GraphMem.index.last_send_data_no && data <= REALTIME_EVENT_MAX_INDEX)
    {
        // 개수 선택해서 받기
        realtimeMem.setting.Flag_SendBackupSelecNum = ON;

        if((int)GraphMem.index.last_send_data_no - (int)data < 0)
            realtimeMem.indexSelect.last_send_data_no = REALTIME_EVENT_MAX_INDEX - (data - GraphMem.index.last_send_data_no);
        else
            realtimeMem.indexSelect.last_send_data_no = GraphMem.index.last_send_data_no - data;

        realtimeMem.indexSelect.save_memory_data_no = GraphMem.index.save_memory_data_no;
    }
    else
    {
        // Error
        retval = 0;
    }

    return retval;
}



/* Ack --------------------------------------------------------------------*/

/**
 * @brief Realtime 전송 후 Ack 관리
 */
void ackWaitManage(void)
{
    static uint32_t ackWaitTime;

    // Check ack wait time
    if(realtimeMem.ack.eventAckWait && ackWaitTime == 0)
        ackWaitTime = HAL_GetTick();
    else if(realtimeMem.ack.eventAckWait == OFF)
        ackWaitTime = 0;

    // Wait timeout 1초 (Ack 실패 확인)
    if(HAL_GetTick() - ackWaitTime > 1000 && realtimeMem.ack.eventAckWait && HAL_GetTick() > 1000)
    {
        realtimeMem.ack.eventAckFailCount++;
        realtimeMem.ack.eventAckWait = OFF;
        ackWaitTime = 0;

        // Multicast message on
        setAdvertiseMsg(MULTICAST_ADVERTISE, DONT_RECV_ACK);
    }

    // 3회 실패
    if(realtimeMem.ack.eventAckFailCount > MAX_RETRY_FAIL)
    {
        // Init ack value
        initRealtimeEventAckWait();
        ackWaitTime = 0;

        // Realtime 모니터링 종료
        for(uint8_t i=0; i <= TCP_CLIENT_EVENT_SOCKET; i++)
            socketControl.socketState[i].monitoring.flagEventRealtime = OFF;

        // Event TCP socket 종료 명령 실행
        socketClose(TCP_SERVER_EVENT_SOCKET);
        socketClose(TCP_CLIENT_EVENT_SOCKET);
    }
}


/**
 * @brief Ack 응답 받은 후 Ack 관련 값 초기화
 */
void initRealtimeEventAckWait(void)
{
    realtimeMem.ack.eventAckWait = OFF;
    realtimeMem.ack.eventAckFailCount = 0;
}



/* 값 반환 --------------------------------------------------------------------*/

/**
 * @brief Realtime event의 index가 1회전 되었는지 확인 값 반환
 * 
 * @return 0: 1회전 X,  1: 1회전 O
 */
uint8_t returnMemorySavedValue(void)
{
    return realtimeMem.setting.memorySaved1TurnOver;
}
