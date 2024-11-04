
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MULTICAST_LUDP_H
#define MULTICAST_LUDP_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "stm32f7xx_hal.h"


/* Define ------------------------------------------------------------------*/
#define MUTLICAST_PORT          53256                          // Multicast Dst Port
#define RSI_MULTICAST_GROUP_IPV4_ADDRESS   "239.66.77.43"      //@ Multicast IPv4 address

// Multicast Message Type
#define MULTICAST_SCAN          0x01
#define MULTICAST_SEARCH        0x02
#define MULTICAST_ADVERTISE     0x81
#define MULTICAST_SCAN_ACK      0x82
#define MULTICAST_SEARCH_ACK    0x83


/* Struct ------------------------------------------------------------------*/
// Message
typedef struct {
    uint8_t transcationId[2];       // Send ID
    uint8_t messageType[2];         // Message Type 0x81: Adevertise, 0x82: Scan ack, 0x83: Search ack

}MultiHeaderTypdef;

typedef struct {
    uint8_t  cause;                 // 1: AP Connect, 2: IP Change, 3: Enter sleep mode, 4: AP Re Connect
    char     serialNum[16];         // "19010101"(8자리)
    uint8_t  modelNumber[4];        // BM3201 -> 3201
    uint8_t  ipAddress[4];          // IP Address of BM
    uint8_t  TcpPort[2];            // BM TCP port
    uint8_t  macAddress[6];
    uint8_t  TcpConnectionNum;      // TCP Connection count number

}MultiAdvertiseTypdef;

typedef struct {
    char     serialNum[16];
    uint8_t  modelNumber[4];
    uint8_t  ipAddress[4];
    uint8_t  TcpPort[2];            // Module TCP Server port
    uint8_t  macAddress[6];
    uint8_t  TcpConnectionNum;      // TCP (Modbus) Connect check
    uint8_t  firmwareVersion[2];
    uint8_t  ModelType;             // Model (MDC: 1, BM: 2, BMT: 3...)
}MultiAckTypdef;

/* Enum ------------------------------------------------------------------*/
// Multicast cause type
typedef enum {
    AP_CONNECT = 1,     // Connect with AP
    IP_CHANGE,          // IP Change
    ENTER_SLEEP_MODE,   // BM Sleep Mode
    AP_RECONNECT,       // ReConnect with AP
    DONT_RECV_ACK,      // Event Ack don't recv
    RE_JOIN_MULTICAST   // Multicast rejoin (Send 0x00 1byte)
}MultiCauseEnum;


/* Extern ------------------------------------------------------------------*/
extern uint8_t MulticastCause;                  // Multicast cause event value
extern uint8_t MultiStep;                       // Multicast init process step
extern MultiHeaderTypdef    multiRecvHeader;    // Multicast header data


/* Function ------------------------------------------------------------------*/
int8_t  initMulticast(void);
uint8_t MulticastProcess(void);                                     // Multicast process (Receive data analaysis, Data send)

void setMultiPacket(void);                                          // Previous set Multicast message
void setAdvertiseMsg(uint8_t messageType, uint8_t advertiseCause);  // Set Advertise message type, cause type


int multicastJoin(uint8_t* ipAddress);





#ifdef __cplusplus
    }
#endif

#endif
