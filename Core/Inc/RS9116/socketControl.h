
/**
  ******************************************************************************
  * @file    socketControl.h
  * @author  jun
  * @brief   Header file of ethernet socket.
  ******************************************************************************
  * @version
  * 2021.11.01 - event 저장 및 전송 처리
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 1. TCP_SERVER_MODBUS_SOCKET
  *    소켓의 종류별로 타입 지정
  * 
  * FIXME: RS9113 맞춤형
  * - Rs9113 wifi 모듈의 경우 Socket의 번호가 Socket을 새로 생성하면 변함
  * -> TCP Server의 경우 연결을 끊어도 유지, Client의 경우 소켓 번호 바뀜
  *    따라서 해당 socket의 Index가 어떠한 타입인지 판단 후 사용
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SOCKET_CONTROL_H
#define SOCKET_CONTROL_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "rsi_driver.h"

/* Enums ------------------------------------------------------------------*/

/* Defines ------------------------------------------------------------------*/
// Socket의 타입
#define UDP_SOCKET                          1
#define TCP_SERVER_MODBUS_LISTEN            2           // Client 접속을 기다리는 서버 소켓
#define TCP_SERVER_EVENT_LISTEN             3
#define TCP_CLIENT_MODBUS_SOCKET            4
#define TCP_CLIENT_EVENT_SOCKET             5       

#define TCP_SERVER_MODBUS_SOCKET            6           // 서버에 접속한 Client 소켓
#define TCP_SERVER_EVENT_SOCKET             7
#define TCP_SERVER_MODBUS_SOCKET_2          8           // 모드 버스 SERVER 소켓 2 (RSI_NUMBER_OF_SOCKETS)




/* Structs ------------------------------------------------------------------*/
typedef struct 
{
    uint8_t flagGraph;                  // Graph monitoring
    uint8_t flagRealtime;               // Realtime monitoring
    uint8_t flagEventGraph;             // Graph Event monitoring
    uint8_t flagEventRealtime;          // Realtime Event monitoring
}eventMonitoringTypedef;


typedef struct
{
    uint8_t     srcIp[4];               // Tool 의 IP
    uint16_t    srcPort;                // Tool 의 PORT
    uint8_t     destIp[4];              // 연결된 쪽의 IP
    uint16_t    destPort;               // 연결된 쪽의 PORT
}networkInfoTypedef;


typedef struct 
{
    uint8_t     connect;                // Socket 연결 상태
    uint8_t     type;                   // Socket의 타입
    uint8_t     waitConnect;            // TCP Server가 Listen 중일 때 생성된 소켓이 연결을 대기중인가 확인

    uint16_t    recvCount;              // Data receive할 때 마다 count 증가
    uint16_t    recvCountOld;           // 마지막 메시지 처리했을 때 count
    uint8_t     recvBuf[512];           // 받은 Data
    uint16_t    recvLength;             // 받은 Data 길이

    uint8_t     bitsocket;              // 비트 소켓이 연결되었을 때 활성화

    eventMonitoringTypedef monitoring;  // 모니터링 Flag
    networkInfoTypedef     netInfo;     // network 정보
}socketTcpStateTypedef;


typedef struct 
{
    socketTcpStateTypedef   socketState[RSI_NUMBER_OF_SOCKETS + 1];
    
    uint8_t lastCommSocket;
}socketControlTypedef;


typedef struct 
{
    uint8_t     ip[4];                  // IP 주소
    uint16_t    port;                   // 포트
    uint8_t     maxConnectNumber;       // 최대 접속 개수
}socketInfoTypedef;




/* Extern ------------------------------------------------------------------*/
extern socketControlTypedef    socketControl;


/* Function ------------------------------------------------------------------*/
// Socket 생성 관련
void setSocketInfo(uint8_t socketType, socketInfoTypedef* socketInfo);
void makeSocket(uint8_t socketType);

// Socket close 관련
int8_t socketClose(uint8_t socketType);
void soketInitWhenDisconnect(void);

// 소켓 상태 관련
void setSocketConnectState(uint8_t socketDescription);
void setSocketType(uint16_t socketDescription, uint8_t* srcIp, uint16_t srcPort, uint8_t* destIp, uint16_t destPort);
void saveServerSocket(uint8_t socketNum);
void changeLastSocket(uint8_t socketNum, uint8_t save);

// 소켓 타입 찾기
uint8_t findSocketDescription(uint8_t socketType);
uint8_t findSocketType(uint8_t socketDesciption);

// 소켓 데이터 관련
void setSocketDataRecv(uint8_t socketDescription, uint8_t* buffer, uint16_t length);


// 소켓 모니터링
uint16_t returnEventSocketKeepalive(uint8_t socketNum);
void checkEventSocketConnect(void);
void setEventSocketActivate(uint8_t flagSocket, uint8_t socketType, uint8_t eventType, uint8_t set);
uint8_t getEventSocketActive(uint8_t eventType);

void setBitsocketConntion(uint16_t socketDescription);






#ifdef __cplusplus
    }
#endif

#endif
