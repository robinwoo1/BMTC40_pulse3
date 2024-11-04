/* Includes ------------------------------------------------------------------*/
#include "socketControl.h"
#include "variable.h"
#include "wifiTask.h"
#include "multicastLudp.h"
#include "stm32f7xx_hal.h"
#include "eventGraphTask.h"
#include "eventRealtimeTask.h"

/* Private variables ---------------------------------------------------------*/


/* Global variables -----------------------------------------------------------*/
extern NetworkTypeDef       WifiEthSet;          // Ethernet Config: IP, Gateway, Netmask ...

socketControlTypedef        socketControl;


/* Functions ------------------------------------------------------------------*/

/* Socket 만들기 관련 ----------------------------------------------------------------------*/
/**
 * @brief Socket의 타입별로 Port, connection 개수 등을 설정.
 * 
 * @param socketType : Socket의 타입
 */
void makeSocket(uint8_t socketType)
{
    socketInfoTypedef socketInfo;
    int socketNum = 0;

    switch (socketType)
    {
        case UDP_SOCKET:
            setSocketInfo(UDP_SOCKET, (socketInfoTypedef*)socketInfo.ip);
            socketNum = createUdpClient(socketInfo.ip, socketInfo.port);                            // Multicast LUDP
            if(socketNum >= 0)
                socketControl.socketState[socketNum].type = UDP_SOCKET;
            break;

        case TCP_SERVER_MODBUS_LISTEN:
            setSocketInfo(TCP_SERVER_MODBUS_LISTEN, (socketInfoTypedef*)socketInfo.ip);
            socketNum = createTcpServer(socketInfo.port, socketInfo.maxConnectNumber);              // TCP server Modbus
            if(socketNum >= 0)
                socketControl.socketState[socketNum].type = TCP_SERVER_MODBUS_LISTEN;
            break;

        case TCP_SERVER_EVENT_LISTEN:
            setSocketInfo(TCP_SERVER_EVENT_LISTEN, (socketInfoTypedef*)socketInfo.ip);
            socketNum = createTcpServer(socketInfo.port, socketInfo.maxConnectNumber);              // TCP server Event 
            if(socketNum >= 0)
                socketControl.socketState[socketNum].type = TCP_SERVER_EVENT_LISTEN;
            break;

        case TCP_CLIENT_MODBUS_SOCKET:
            setSocketInfo(TCP_CLIENT_MODBUS_SOCKET, (socketInfoTypedef*)socketInfo.ip);
            socketNum = createTcpClient(socketInfo.port);                                           // TCP client Modbus
            if(socketNum >= 0)
                socketControl.socketState[socketNum].type = TCP_CLIENT_MODBUS_SOCKET;
            break;

        case TCP_CLIENT_EVENT_SOCKET:
            setSocketInfo(TCP_CLIENT_EVENT_SOCKET, (socketInfoTypedef*)socketInfo.ip);
            socketNum = createTcpClient(socketInfo.port);                                           // TCP client event
            if(socketNum >= 0)
                socketControl.socketState[socketNum].type = TCP_CLIENT_EVENT_SOCKET;
            break;

        default:
            break;
    }
}


/**
 * @brief Set the Socket Info object
 * 
 * @param socketType : Socket의 타입
 * @param socketInfo : 생성하는 Socket의 ip, port...
 */
void setSocketInfo(uint8_t socketType, socketInfoTypedef* socketInfo)
{
    switch (socketType)
    {
        case UDP_SOCKET:
            rsi_ascii_dot_address_to_4bytes(socketInfo->ip, (int8_t*)RSI_MULTICAST_GROUP_IPV4_ADDRESS);
            socketInfo->port = MUTLICAST_PORT;
            socketInfo->maxConnectNumber = 1;
            break;

        case TCP_SERVER_MODBUS_LISTEN:
            memcpy(socketInfo->ip, WifiEthSet.DEVICE_IP, 4);
            socketInfo->port = WifiEthSet.DEVICE_PORT;
            socketInfo->maxConnectNumber = 1;                   // FIXME: 서버 소켓 접속 가능한 숫자 설정 (TCP_SERVER_MODBUS_LISTEN_2는 제거하고 이거 숫자를 늘려야한다)
            break;

        case TCP_SERVER_EVENT_LISTEN:
            memcpy(socketInfo->ip, WifiEthSet.DEVICE_IP, 4);
            socketInfo->port = WifiEthSet.DEVICE_PORT + 1;
            socketInfo->maxConnectNumber = 1;
            break;

        case TCP_CLIENT_MODBUS_SOCKET:
            memcpy(socketInfo->ip, WifiEthSet.TARGET_IP, 4);
            socketInfo->port = WifiEthSet.TARGET_PORT;
            socketInfo->maxConnectNumber = 1;
            break;

        case TCP_CLIENT_EVENT_SOCKET:
            memcpy(socketInfo->ip, WifiEthSet.TARGET_IP, 4);
            socketInfo->port = WifiEthSet.TARGET_PORT + 1;
            socketInfo->maxConnectNumber = 1;

            // Port error 확인
            if(WifiEthSet.TARGET_PORT == 0)
                socketInfo->port = 0;
            break;
        
        default:
            break;
    }
}




/* Socket 상태 관리 관련 ----------------------------------------------------------------------*/
/**
 * @brief Socket이 외부 연결되어 있는지 설정
 * 
 * @param socketType : 설정하고자 하는 socket의 Type
 */
void setSocketConnectState(uint8_t socketType)
{
    // Find socket description
    uint8_t socketDescription = findSocketDescription(socketType);
    uint8_t socketNum = 0;

    // Check socket number limit
    if(socketDescription > RSI_NUMBER_OF_SOCKETS)
        socketDescription = 0;
        
    // Check socket connect
    if(rsi_socket_pool[socketDescription].sock_state == RSI_SOCKET_STATE_CONNECTED)
    {
        socketControl.socketState[socketDescription].connect = ON;

        // UDP는 연결되었다고 표시 X
        if(socketDescription == 0)
            socketControl.socketState[socketDescription].connect = OFF; 
    }
    else
    {
        // 소켓이 접속 해제가 된 것이면 초기화
        if(socketControl.socketState[socketDescription].connect == ON)
        {
            // TCP Server의 연결이 끊어진 경우 waitConnect 해제
            switch (socketType)
            {
                case TCP_SERVER_MODBUS_SOCKET:
                    socketNum = findSocketDescription(TCP_SERVER_MODBUS_LISTEN);
                    break;

                case TCP_SERVER_EVENT_SOCKET:
                    socketNum = findSocketDescription(TCP_SERVER_EVENT_LISTEN);
                    break;
            }

            socketControl.socketState[socketNum].waitConnect = OFF;
            memset(&socketControl.socketState[socketDescription].connect, 0, sizeof(socketTcpStateTypedef));
        }

        socketControl.socketState[socketDescription].connect = OFF; 
    }
}


/**
 * @brief Socket의 Data 받은 데이터를 할당한다.
 * 
 * @param socketDescription : Socket description 번호
 * @param buffer : 데이터 버퍼 포인터
 * @param length : 데이터 길이
 */
void setSocketDataRecv(uint8_t socketDescription, uint8_t* buffer, uint16_t length)
{
    // Socket 번호 확인
    if(socketDescription <= RSI_NUMBER_OF_SOCKETS)
    {
        socketControl.socketState[socketDescription].recvCount++;
        socketControl.socketState[socketDescription].recvLength = length;

        // Length 제한
        if(socketControl.socketState[socketDescription].recvLength > 512)
            socketControl.socketState[socketDescription].recvLength = 512;

        // Data copy (직접 데이터 받아서 처리)
        // memcpy(socketControl.socketState[socketDescription].recvBuf, buffer, socketControl.socketState[socketDescription].recvLength);
    }    
}


/**
 * @brief Set the Socket Type object
 *        RS9113 - 툴의 서버 소켓을 처음 생성했을 때, Client 소켓이 접속을 성공했을 때 실행
 * 
 * @param socketDescription : 설정된 description number
 * @param portBuf : 설정된 Port 번호
 */
void setSocketType(uint16_t socketDescription, uint8_t* srcIp, uint16_t srcPort, uint8_t* destIp, uint16_t destPort)
{
    uint8_t  multiIp[4];
    uint8_t  socketType;
    uint8_t  socketState;

    // Limit 확인
    if(socketDescription > RSI_NUMBER_OF_SOCKETS)
        return;

    rsi_ascii_dot_address_to_4bytes(multiIp, (int8_t*)RSI_MULTICAST_GROUP_IPV4_ADDRESS);        // Multicast ip
    socketState = rsi_socket_pool[socketDescription].sock_state;                                // Socket state


    // 설정된 IP, Port를 비교하여 Socket type을 찾는다
    if(srcPort == MUTLICAST_PORT)
    {
        socketType = UDP_SOCKET;
    }
    else if(srcPort == WifiEthSet.DEVICE_PORT && memcmp(srcIp, WifiEthSet.DEVICE_IP, 4) == 0 && socketState == RSI_SOCKET_STATE_LISTEN)
    {
        socketType = TCP_SERVER_MODBUS_LISTEN;
    }
    else if(srcPort == WifiEthSet.DEVICE_PORT && memcmp(srcIp, WifiEthSet.DEVICE_IP, 4) == 0 && socketState == RSI_SOCKET_STATE_CONNECTED)
    {
        socketType = TCP_SERVER_MODBUS_SOCKET;

        // TCP 소켓이 연결되어 있으면 소켓2로 설정
        if(socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET)].connect)
            socketType = TCP_SERVER_MODBUS_SOCKET_2;
    }
    else if(srcPort == WifiEthSet.DEVICE_PORT + 1 && memcmp(srcIp, WifiEthSet.DEVICE_IP, 4) == 0 && socketState == RSI_SOCKET_STATE_LISTEN)
    {
        socketType = TCP_SERVER_EVENT_LISTEN;
    }
    else if(srcPort == WifiEthSet.DEVICE_PORT + 1 && memcmp(srcIp, WifiEthSet.DEVICE_IP, 4) == 0 && socketState == RSI_SOCKET_STATE_CONNECTED)
    {
        socketType = TCP_SERVER_EVENT_SOCKET;
    }
    else if(destPort == WifiEthSet.TARGET_PORT && memcmp(destIp, WifiEthSet.TARGET_IP, 4) == 0)
    {
        socketType = TCP_CLIENT_MODBUS_SOCKET;
    }
    else if(destPort == WifiEthSet.TARGET_PORT + 1 && memcmp(destIp, WifiEthSet.TARGET_IP, 4) == 0)
    {
        socketType = TCP_CLIENT_EVENT_SOCKET;
    }
    else
    {
        socketType = 0;
    }

    // Set Info
    socketControl.socketState[socketDescription].type = socketType;                     // Socket 종류
    memcpy(socketControl.socketState[socketDescription].netInfo.srcIp, srcIp, 4);       // Tool ip
    memcpy(socketControl.socketState[socketDescription].netInfo.destIp, destIp, 4);     // Dst ip
    socketControl.socketState[socketDescription].netInfo.srcPort = srcPort;             // Tool port
    socketControl.socketState[socketDescription].netInfo.destPort = destPort;           // Dst port
}


/**
 * @brief 서버 소켓 생성 시 소켓 타입 결정 및 ip, port 저장
 */
void saveServerSocket(uint8_t socketNum)
{
    uint8_t  srcIp[4];
    uint8_t  destIp[4];
    uint16_t srcPort;
    uint16_t destPort;
    uint16_t socketDescription;


    // Network 값 구조 변경 (RS9113)
    socketDescription = socketNum;                                                  // Socket number
    memcpy((void*)srcIp, WifiEthSet.DEVICE_IP, 4);                                  // Tool IP
    memcpy((void*)destIp, rsi_socket_pool[socketNum].destination_ip_addr.ipv4, 4);  // Dst IP
    srcPort = rsi_socket_pool[socketNum].source_port;                               // Tool Port
    destPort = rsi_socket_pool[socketNum].destination_port;                         // Dst Port  

    // Socket 타입 결정 및 network 데이터 저장
    setSocketType(socketDescription, srcIp, srcPort, destIp, destPort);
}


/**
 * @brief 마지막 통신 socket을 저장하여 모니터링 통신 때 특정 소켓으로 데이터를 전송하고 Socket 번호를 복원한다.
 * 
 * @param socketNum : 변경하고자 하는 Socket
 * @param save : 1 = socket 저장, 0 = socket 복원
 */
void changeLastSocket(uint8_t socketNum, uint8_t save)
{
    static uint8_t saveSocketNum;

    // Check socketNum
    if(socketNum > RSI_NUMBER_OF_SOCKETS)
        return;

    // Save
    if(save)
    {
        saveSocketNum = socketControl.lastCommSocket;
        socketControl.lastCommSocket = socketNum;
    }
    // Rollback
    else
        socketControl.lastCommSocket = saveSocketNum;
}



/* Socket Type 찾기  ------------------------------------------------------------------------------------*/

/**
 * @brief 소켓 타입에 해당되는 Description number를 구한다
 * 
 * @param socketType : 찾고자 하는 Socket type (1: TCP_SERVER_MODBUS_SOCKET ...)
 * @return 0: 없음, 이외: 해당 description number
 */
uint8_t findSocketDescription(uint8_t socketType)
{
    uint8_t retval = 0;

    // Limit 확인
    if(socketType > RSI_NUMBER_OF_SOCKETS)
        return 0;

    // Find 타입과 같은 Description number
    for(uint8_t i=0; i <= RSI_NUMBER_OF_SOCKETS; i++)
    {
        if(socketControl.socketState[i].type == socketType)
            retval = i;
    }

    return retval;
}


/**
 * @brief Socket description으로 socket의 Type을 찾는다
 * 
 * @param socketDesciption : Socket의 description 번호 (버퍼 index 번호)
 * @return retval: UDP_SOCKET (1) ~ TCP_CLIENT_EVENT_SOCKET (5), 0: 없음
 */
uint8_t findSocketType(uint8_t socketDesciption)
{
    uint8_t retval;

    if(socketDesciption > RSI_NUMBER_OF_SOCKETS)
        return 0;

    retval = socketControl.socketState[socketDesciption].type;
    return retval;
}



/* Socket 종료  ------------------------------------------------------------------------------------*/

/**
 * @brief Close TCP socket
 * 
 * @param socketType : 종료하고자 하는 Socket의 Type
 */
int8_t socketClose(uint8_t socketType)
{
    int8_t retval = 0;
    uint8_t socketDescription;
    
    // Socket type에 맞는 desciption 번호 찾기
    socketDescription = findSocketDescription(socketType);
    
    // Check max socket 
    if(socketDescription > RSI_NUMBER_OF_SOCKETS)
        return -1;

    // Socket close
    if(rsi_socket_pool[socketDescription].sock_state)
        retval = rsi_shutdown(socketDescription, 0);

    return retval;
}


/**
 * @brief Ap와 연결이 끊어진 후 Socket 구조체 초기화
 */
void soketInitWhenDisconnect(void)
{
    // Init socket control struct
    memset(&socketControl, 0, sizeof(socketControlTypedef));
}



/* Socket 모니터링  ------------------------------------------------------------------------------------*/

/**
 * @brief Modbus 소켓에서 Event 소켓의 연결 상태를 keep alive할 때 event 상태 반환.
 * 
 * @param socketNum : Modbus socket (TCP_SERVER_MODBUS_SOCKET, TCP_CLIENT_MODBUS_SOCKET)
 * @return retval: 0 - 연결 중, 1 - 연결 필요
 */
uint16_t returnEventSocketKeepalive(uint8_t socketNum)
{
    uint16_t retval = 0;
    uint8_t  socketType;

    // Socket type 찾기
    socketType = findSocketType(socketNum);

    // Modbus 소켓에서 keep alive 확인 했을 때 server modbus - server client, client modbus - client event 상태 전달
    switch (socketType)
    {
        case TCP_SERVER_MODBUS_SOCKET:
        case TCP_SERVER_MODBUS_SOCKET_2:
            // TCP Server 연결 확인
            if(socketControl.socketState[findSocketDescription(TCP_SERVER_EVENT_SOCKET)].connect == OFF)
                retval = ON;
            break;

        case TCP_CLIENT_MODBUS_SOCKET:
            // TCP Client 연결 확인
            if(socketControl.socketState[findSocketDescription(TCP_CLIENT_EVENT_SOCKET)].connect == OFF)
                retval = ON;
            break;
        
        default:
            retval = ON;
            break;
    }

    return retval;
}


/**
 * @brief Event 소켓의 연결이 끊어졌을 경우 초기화 실행
 */
void checkEventSocketConnect(void)
{
    // 모든 소켓 번호 확인
    for(uint8_t i = 0; i <= RSI_NUMBER_OF_SOCKETS; i++)
    {
        // Graph event 활성화 중
        if(socketControl.socketState[i].monitoring.flagEventGraph)
        {
            // Socket 연결이 끊어지면 Ack 상태 초기화
            if(socketControl.socketState[i].connect == OFF)
            {
                // Init Ack 
                initGraphEventAckWait();
                socketControl.socketState[i].monitoring.flagEventGraph = OFF;
            }
        }

        // Realtime event 활성화 중
        if(socketControl.socketState[i].monitoring.flagEventRealtime)
        {
            // Socket 연결이 끊어지면 Ack 상태 초기화
            if(socketControl.socketState[i].connect == OFF)
            {
                // Init Ack
                initRealtimeEventAckWait();
                socketControl.socketState[i].monitoring.flagEventRealtime = OFF;
            }
        }
    }
}


/**
 * @brief Event monitoring을 해당하는 Modbus socket에 따라 활성화.
 * 
 * @param flagSocket : 매개변수 socketType의 값 의미 (0: Socket Type, 1: Socket number)
 * @param socketType : 통신한 Socket의 type
 * @param eventType  : 활성화하려는 Event 종류
 * @param set        : ON, OFF
 */
void setEventSocketActivate(uint8_t flagSocket, uint8_t socketType, uint8_t eventType, uint8_t set)
{
    uint8_t socketNum = 0;

    // SocketType의 종류 확인
    if(flagSocket == SOCKET_TYPE)
    {
        // Socket의 타입
        socketType = socketType;
    }
    else
    {
        // Socket 번호 -> 타입 변환
        socketType = findSocketType(socketType);
    }

    // Find socket description
    switch (socketType)
    {
        // Modbus 통신으로 온 명령을 -> Event 소켓으로 변경
        case TCP_SERVER_MODBUS_SOCKET:
        case TCP_SERVER_MODBUS_SOCKET_2:
            socketNum = findSocketDescription(TCP_SERVER_EVENT_SOCKET);
            break;
        
        case TCP_CLIENT_MODBUS_SOCKET:
            socketNum = findSocketDescription(TCP_CLIENT_EVENT_SOCKET);
            break;

        // Event socket
        case TCP_SERVER_EVENT_SOCKET:
            socketNum = findSocketDescription(TCP_SERVER_EVENT_SOCKET);
            break;
        
        case TCP_CLIENT_EVENT_SOCKET:
            socketNum = findSocketDescription(TCP_CLIENT_EVENT_SOCKET);
            break;

        default:
            break;
    }

    // Check socket limit
    if(socketNum > RSI_NUMBER_OF_SOCKETS || socketNum == 0)
        return;

    // Set
    switch (eventType)
    {
        case MONITORING_EVENT_REALTIME:
            socketControl.socketState[socketNum].monitoring.flagEventRealtime = set;
            break;

        case MONITORING_EVENT_GRAPH:
            socketControl.socketState[socketNum].monitoring.flagEventGraph = set;
            break;

        default:
            break;
    }
}


/**
 * @brief Get the Event Socket Active object
 * 
 * @param  eventType - 0: Realtime, 1: Graph
 * @return int8_t retval - 0: 없음, 1 ~ 5: 해당 소켓 활성화되어 있음 (Type에 대한 Socket의 index 번호 return -> RS9113에서는 Socket 번호 O)
 */
uint8_t getEventSocketActive(uint8_t eventType)
{
    uint8_t retval = 0;

    // Event의 타입에 따라
    switch (eventType)
    {
        case MONITORING_EVENT_REALTIME:
            // Check event monitoring flag
            for(uint8_t i=0; i <= RSI_NUMBER_OF_SOCKETS; i++)
            {
                if(socketControl.socketState[findSocketDescription(i)].monitoring.flagEventRealtime && socketControl.socketState[findSocketDescription(i)].connect)
                {
                    retval = findSocketDescription(i);
                    break;
                }
            }
            break;
        
        case MONITORING_EVENT_GRAPH:
            // Check event monitoring flag
            for(uint8_t i=0; i <= RSI_NUMBER_OF_SOCKETS; i++)
            {
                if(socketControl.socketState[findSocketDescription(i)].monitoring.flagEventGraph && socketControl.socketState[findSocketDescription(i)].connect)
                {
                    retval = findSocketDescription(i);
                    break;
                }
            }
            break;
        
        default:
            break;
    }

    return retval;
}


/**
 * @brief 비트 소켓 트레이 - 이더넷 연결 활성화
 * 
 * @param socketDescription : 소켓 Number
 */
void setBitsocketConntion(uint16_t socketDescription)
{
    if(socketDescription > RSI_NUMBER_OF_SOCKETS)
        return;

    // 비트 소켓 모드 활성화
    if(socketControl.socketState[socketDescription].connect)
        socketControl.socketState[socketDescription].bitsocket = ON;
}
