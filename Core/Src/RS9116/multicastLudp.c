/* Includes ------------------------------------------------------------------*/
#include "multicastLudp.h"
#include "variable.h"
#include "wifiTask.h"
#include "socketControl.h"
#include "rsi_multicast.h"


/* Private variables ---------------------------------------------------------*/
/* Global variables -----------------------------------------------------------*/
MultiHeaderTypdef    multiRecvHeader;        // Multicast recv data header
MultiAckTypdef       multiAck;               // Multicast Ack data struct 

uint8_t multicastSendBuf[70] = {0, };        // Multicast send data Buffer
uint8_t MulticastCause = 0;                  // Multicast advertise cause flag
uint8_t MultiStep;                           // Multicast init process step
uint8_t multicastIp[4];

extern NetworkTypeDef     WifiEthSet;        // Ethernet Config: IP, Gateway, Netmask, etc ...


/* Functions ------------------------------------------------------------------*/

/******************************************************************************************************************************
 * Set Multicast Join, Leave, Multi struct(IP, Port, Mac filter)
 * 
 * Sequence: Set Multi struct - JOIN - (Recv Message) - MultiStep = 1
 *                                   - (Don't recv)   - Over 100s Send Multi message - Over MULTICASAT_WAIT_TIME - Leave - JOIN
 * *****************************************************************************************************************************/
int8_t initMulticast(void)
{
    static uint8_t multiInit[2];
    int8_t retval = 1;

    // Multicast 초기화 상태 반영
    multiInit[0] = multiInit[1];
    multiInit[1] = MultiStep;

    // Multicast Join
    if(MultiStep == 0)
    {
        // Set Multicast packet about Serial num, Model, etc...
        if(multiAck.serialNum[0] == 0)
            setMultiPacket();

        rsi_ascii_dot_address_to_4bytes(multicastIp, (int8_t*)RSI_MULTICAST_GROUP_IPV4_ADDRESS);

        // Set multicast
        multicastJoin(multicastIp);

        // Multicast 초기화 완료
        MultiStep = ON;
    }

    // Multicast 초기화 상태에 따라 AP에 처음 접속한건지 메시지 전송
    if(multiInit[0] == 0 && multiInit[1] == 0)
        setAdvertiseMsg(MULTICAST_ADVERTISE, AP_CONNECT);    // First time
    else if(multiInit[0] == 1 && multiInit[1] == 0)
        setAdvertiseMsg(MULTICAST_ADVERTISE, AP_RECONNECT);  // After reset

        
    return retval;
}


/***********************************************************************
 * Multicast receive data 처리
 * 
 * Sequence: Check Receive data - Check Message Type - Ignore or Send
 *           Check Advertise msg- Make - Send
 * 
 * *********************************************************************/
uint8_t MulticastProcess(void)
{
    static uint16_t recvCountOld;
    uint32_t sendLength;
    uint16_t messageType;
    uint16_t socketDesriptor;
    uint8_t ip[4];
    uint16_t port;

    uint8_t socketNum;
    uint8_t* recvBuf;
    uint16_t recvCount;


    // Set UDP socket
    socketNum = findSocketDescription(UDP_SOCKET);
    recvBuf = socketControl.socketState[socketNum].recvBuf;
    recvCount = socketControl.socketState[socketNum].recvCount;

    // Receive UDP Data
    udpRecvData(socketNum, recvBuf, 128);

    // Multicast data received
    if(recvCountOld != recvCount)
    {
        // Init recv flag
        recvCountOld = recvCount;
        multiWaitCount = 0;     // Recv data then Multicast count init 
        MultiStep = 1;

        // Check message type
        messageType = (recvBuf[2] << 8) + recvBuf[3];

        if(messageType == MULTICAST_SCAN || messageType == MULTICAST_SEARCH)
        {
            memcpy(multiRecvHeader.transcationId, recvBuf, 2);     // Save Transcation id

            // Message type에 따라 ACK 결정
            switch (messageType)
            {
                case MULTICAST_SCAN:
                    multiRecvHeader.messageType[0] = 0;
                    multiRecvHeader.messageType[1] = MULTICAST_SCAN_ACK;   // Message Type: Scan ack

                    sendLength = sizeof(multiRecvHeader) + sizeof(multiAck);
                break;

                case MULTICAST_SEARCH:
                    // Check host name
                    if(memcmp(bmHostName, (char*)(recvBuf + sizeof(multiRecvHeader)), sizeof(bmHostName)) != 0)
                        return 0;

                    multiRecvHeader.messageType[0] = 0;
                    multiRecvHeader.messageType[1] = MULTICAST_SEARCH_ACK;   // Message Type: Search ack

                    sendLength = sizeof(multiRecvHeader) + sizeof(multiAck);
                break;
            }

            // Find TCP socket
            for(uint8_t i=0; i <= RSI_NUMBER_OF_SOCKETS; i++)
            {
                switch (i)
                {
                    case TCP_SERVER_MODBUS_SOCKET:
                        if(socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET)].connect && \
                           socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET)].bitsocket == 0)
                            socketDesriptor = i;
                        break;
                    case TCP_SERVER_MODBUS_SOCKET_2:
                        if(socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET_2)].connect && \
                           socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET_2)].bitsocket == 0)
                            socketDesriptor = i;
                        break;
                    default:
                        socketDesriptor = 0;
                        break;
                }

                if(socketDesriptor)
                    break;
            }

            // TCP Connection count
            if(socketControl.socketState[socketDesriptor].connect)
                multiAck.TcpConnectionNum = ON;
            else
                multiAck.TcpConnectionNum = OFF;


            // Make Multicast Send data
            memcpy(multicastSendBuf, &multiRecvHeader, sizeof(multiRecvHeader));                // Header (Length: 4)
            memcpy(multicastSendBuf + sizeof(multiRecvHeader), &multiAck, sizeof(multiAck));    // Multicast data (35)
        }
        // Don't Need Message , Error Message
        else
        {
            return 0;
        }
    }
    // Send Advertise message (event)
    else if(multiRecvHeader.messageType[1] == MULTICAST_ADVERTISE && MulticastCause != 0)
    {
        // IP Change
        if(MulticastCause == 0x02)
            memcpy(multiAck.ipAddress, WifiEthSet.DEVICE_IP, sizeof(multiAck.ipAddress));  // IP Address

        // Transcation ID
        multiRecvHeader.transcationId[0] = 0;
        multiRecvHeader.transcationId[1] = 1;

        // Find TCP socket
        for(uint8_t i=0; i <= RSI_NUMBER_OF_SOCKETS; i++)
        {
            switch (i)
            {
                case TCP_SERVER_MODBUS_SOCKET:
                    if(socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET)].connect && \
                        socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET)].bitsocket == 0)
                        socketDesriptor = i;
                    break;
                case TCP_SERVER_MODBUS_SOCKET_2:
                    if(socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET_2)].connect && \
                        socketControl.socketState[findSocketDescription(TCP_SERVER_MODBUS_SOCKET_2)].bitsocket == 0)
                        socketDesriptor = i;
                    break;
                default:
                    socketDesriptor = 0;
                    break;
            }

            if(socketDesriptor)
                break;
        }

        // TCP Connection count
        if(socketControl.socketState[socketDesriptor].connect)
            multiAck.TcpConnectionNum = ON;
        else
            multiAck.TcpConnectionNum = OFF;

        // Make Multicast Send data
        memcpy(multicastSendBuf, &multiRecvHeader, sizeof(multiRecvHeader));                            // Header (4)
        multicastSendBuf[ sizeof(multiRecvHeader) ] = MulticastCause;                                   // Cause  (1)
        memcpy((void*)(multicastSendBuf + sizeof(multiRecvHeader) + 1), &multiAck, sizeof(multiAck));   // Multicast data (35)

        sendLength = sizeof(multiRecvHeader) + sizeof(multiAck) + 1;    // (Total: 40)

        // Multicast Rejoin일 때는 1Byte Send
        if(MulticastCause == RE_JOIN_MULTICAST)
            sendLength = 1;

    }
    // No receive message
    else
    {
        return 0;
    }


    // Check send socket number
    // Check Message Type for IP Address change
    if(multiRecvHeader.messageType[1] == MULTICAST_ADVERTISE)   
    {
        // Advertise Message (Multicast)
        memcpy(ip, multicastIp, 4);                         // Multicast IP
        port = MUTLICAST_PORT;                              // Multicast (53256)

        // Init
        memset(multiRecvHeader.messageType, 0, sizeof(multiRecvHeader.messageType));
        MulticastCause = 0;
    }
    else    
    {
        // Ack Message (Unicast)
        memcpy(ip, socketControl.socketState[socketNum].netInfo.destIp, 4);     // Unicast IP
        port = socketControl.socketState[socketNum].netInfo.destPort;           // Unicast port
    }

    // Send Multicast data
    udpSendData(socketNum, multicastSendBuf, sendLength, ip, port);

    return 0;
}


// Make serial number, model, ETC... of Multicast send data
void setMultiPacket(void)
{
    char     serialNum[20] = "";
    char     tempNum[10]   = "";
    uint32_t modelNumber;
    uint8_t  modelNumBuf[4];
    uint8_t  portNum[2];
    uint8_t  firmwareBuf[2];

    // Make BM Serial number (8자리)
    for(uint8_t i = 0; i < 4; i++)
    {
        sprintf(tempNum, "%02d", dset[5 + i]);     // 2자리가 나오도록  (EX: 09 00 10)
        tempNum[2] = 0;
        strcat(serialNum, tempNum);

        if(i == 1)
        {
            sprintf(tempNum, "%02d", PRODUCT_CODE);
            tempNum[2] = 0;
            strcat(serialNum, tempNum);                 // BM 제품코드
        }
    }
    
    // BMT Model number (ex: 4001)
    modelNumber    = atoi(Info_DrvModel[dset[4]].name + 3);
    modelNumBuf[0] = (modelNumber >> 24) & 0xFF;
    modelNumBuf[1] = (modelNumber >> 16) & 0xFF;
    modelNumBuf[2] = (modelNumber >> 8) & 0xFF;
    modelNumBuf[3] = modelNumber & 0xFF;

    // Port number
    portNum[0] = (WifiEthSet.DEVICE_PORT >> 8) & 0xFF;        // TCP Port
    portNum[1] = WifiEthSet.DEVICE_PORT & 0xFF;

    // Firmware version
    firmwareBuf[0] = ((controllerPara.firmwareVersion[0] * 1000 + controllerPara.firmwareVersion[1] * 10 + controllerPara.firmwareVersion[2]) >> 8) & 0xFF;
    firmwareBuf[1] = (controllerPara.firmwareVersion[0] * 1000 + controllerPara.firmwareVersion[1] * 10 + controllerPara.firmwareVersion[2]) & 0xFF;

    // Make Multicast Ack send data    
    memcpy(multiAck.serialNum,   serialNum,            sizeof(multiAck.serialNum));         // Serial number (length: 16)
    memcpy(multiAck.modelNumber, modelNumBuf,          sizeof(multiAck.modelNumber));       // Model number (4)
    memcpy(multiAck.ipAddress,   WifiEthSet.DEVICE_IP, sizeof(multiAck.ipAddress));         // IP Address   (4)
    memcpy(multiAck.TcpPort,     portNum,              sizeof(multiAck.TcpPort));           // Port number  (2)
    memcpy(multiAck.macAddress,  WifiEthSet.MACADDRESS,sizeof(multiAck.macAddress));        // Mac Address  (6)
    memcpy(multiAck.firmwareVersion, firmwareBuf,      sizeof(multiAck.firmwareVersion));   // Firmware Version  (2)
    multiAck.ModelType = 0x03;      // Model type: BMT = 3   (1)
    
}

// Set advertise message send (MessageType: MULTICAST_ADVERTISE, advertiseCause: AP_CONNECT etc...)
void setAdvertiseMsg(uint8_t messageType, uint8_t advertiseCause)
{
    // Message Type
    multiRecvHeader.messageType[0] = 0;
    multiRecvHeader.messageType[1] = messageType;

    // Advertise Cause type
    MulticastCause = advertiseCause;

}


/*- Multicast -------------------------------------------------------*/
/**
 * @brief Multicast 그룹에 접속 
 * 
 * @param ipAddress : 접속하고자 하는 그룹 ip 주소
 * @return int (0: 성공, 그외: 실패)
 */
int multicastJoin(uint8_t* ipAddress)
{
    int32_t status;

    status = rsi_multicast_join(0, (int8_t*)ipAddress);

    return status;
}
