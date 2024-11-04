
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef WIFI_MAIN_H
#define WIFI_MAIN_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "rsi_data_types.h"         // include file to refer data types
#include "rsi_common_apis.h"        // COMMON include file to refer wlan APIs
#include "rsi_wlan_apis.h"          // WLAN include file to refer wlan APIs
#include "rsi_socket.h"             // socket include file to refer socket APIs
#include "rsi_error.h"              // Error include files
#include <stdint.h>
#include "httpServer.h"


/* Enums ------------------------------------------------------------------*/
typedef enum{
    WIFI_NOTUSE,
    WIFI_INIT,
    WIFI_START,
    WIFI_STOP,
    WIFI_NOINIT
} WifiCtrlTypeDef;

typedef enum
{
    WIFI_DONTUSE,      //  Not used
    WIFI_WORK
} WifiStatusTypeDef;

/* Defines ------------------------------------------------------------------*/
#define WIFF_PRINT                      0           // Print wifi error
#define WIFI_MAC_ADDRESS_ACTIVE         1

// Wifi Initialize 
#define WIFI_WAIT_MESSAGE               0x01
#define WIFI_INIT_SUCCESS               0xF0
#define WIFI_INIT_SYS_FAIL              -10
#define WIFI_AP_CONNECT_FAIL            -1

#define MULTICASAT_WAIT_TIME            110000      // Multiacast message Wait (Max: 110s)

#define WIFI_INIT_LOCK                  2           // Init 중일 때 Lock을 건다.


// wifi module
#define WIFI_MODULE_BUF_LEN             6400        // Memory length for driver
#define RSI_FIRMWARE_NAME_SIZE          20
#define CHANNEL_NO                      0           // Scan Channel number , 0 - to scan all channels


// 모니터링 종류
#define MONITORING_REALTIME             0
#define MONITORING_GRAPH                1

#define MONITORING_GRAPH_CH1            2
#define MONITORING_GRAPH_CH2            3
#define MONITORING_GRAPH_REALTIME       4


#define SOCKET_TYPE                     0
#define SOCKET_DESCRIPTION              1

#define RSI_STATIC_IP_MODE              0



/* Structs ------------------------------------------------------------------*/
// Network config
typedef struct {
    uint8_t  DEVICE_IP[4];
    uint8_t  TARGET_IP[4];
    uint8_t  NETMASK[4];
    uint8_t  GATEWAY[4];
    uint8_t  DHCP;              // 0:Static 1:DHCP
    uint8_t  MACADDRESS[6];
    uint16_t DEVICE_PORT;
    uint16_t TARGET_PORT;
    uint8_t  FirmVersion[6];    // Firmware version
    uint8_t  WifiOperate;       // 1: Wifi Enable, 0: disable
} NetworkTypeDef;

// AP config
typedef struct {
    char    SSID[33];           // Access point SSID to connect, use 32char
    char    PASSWORD[64];       // Password, use 32char
    uint8_t SECURITY_TYPE;      // Security type   (2 : RSI_WPA2)
    uint8_t AP_COUNTRY;         // 1: U.S 2: Europe 3: JAPAN
    uint8_t RSSI;               // AP signal strength
    uint8_t Status;             // AP Connection status
    uint8_t Channel;            // AP channel
    uint8_t PasswordMode;       // 0: PSK, 1: PMK - 비밀번호의 모드
} WifiAPTypeDef;

// Wifi Message receive check value
typedef struct {
    uint8_t MsgRSSI;        // 0: Wait request messge reply from RS9113, 1: Can send request message    
    uint8_t MsgSocket; 
    uint8_t MsgHttp;
    uint8_t MsgWebpageRSP;
    uint8_t MsgMulticast;
} WifiMsgState;

// Check timeout socket
typedef struct {
    uint32_t createSocketTime;      // Socket 생성 주기 시간 
    uint32_t discSocketTime;        // Socket disconnect 주기 시간
} WifiTimeoutTypdef;

typedef struct {
    char   enable;
    char   eapMethod[32];                    //@ EAP method
    char   innerMethod[32];                  //@ Inner method
    char   userIdentity[64];                 //@ user name
    char   password[128];                    //@ Password    
    char   okc_enable;                       //@ Opportunistic Key Caching enable
    char   private_key_passwd[82];           //@ Private key password for encrypted certificates
} WifiEapSetTypedef;


/* Extern ------------------------------------------------------------------*/
extern WifiEapSetTypedef wifiEapSet;
extern char    bmHostName[16];          // BM HOST Name  ("BM" + Mac Address)
extern uint8_t eapCertification[1024 * 8];


/* Function ------------------------------------------------------------------*/
// Wifi process
void initWifiPara(void);
int16_t InitRS9113(uint8_t InitRetryCount);

// WiFi 동작
void WifiProcess(void);
void WifiSysLoop(void);
void wifiErrorDisplay(void);
void RequestNetworkSetting(void);
int checkResponse(void);

// TCP, Web server run
int32_t TcpClientRun(void);
int32_t httpServerRun(void);

// Data 전송
int32_t tcpSendData(uint8_t socketDescription, uint8_t* data, uint16_t length);
int32_t udpSendData(uint8_t socketDescription, uint8_t* data, uint16_t length, uint8_t* ipAddress, uint16_t port);
int32_t tcpRecvData(uint8_t socketDescription, uint8_t* data, uint16_t length);
int32_t udpRecvData(uint8_t socketDescription, uint8_t* data, uint16_t length);

// AP 연결 상태
void CheckConnectAp(void);      // Connection check with AP
void WifiResetLock(void);       // Driver lock, module reset
void ResetWifiModule(void);
void getApState(void);
uint8_t getRssi(void);

// Socket 생성 및 관리
int createTcpServer(uint16_t port, uint8_t maxConnectNum);
int createUdpClient(uint8_t* ip, uint16_t port);
int createTcpClient(uint16_t port);
int acceptTcpServer(uint8_t socketNum);
int connectTcpClient(uint8_t socketNum, uint8_t* ipAddress, uint16_t port);
void trySocketConnect(uint8_t type);


// 모니터링 관리
void socketMonitoringSend(uint16_t length, uint8_t type);
void setSocketMonitoringControl(uint8_t flagSocket ,uint8_t socketType, uint8_t eventType, uint8_t set);

// Callback 함수
void wlanScanCallback(uint16_t status, const uint8_t* buffer, const uint16_t length);
void wlanConnectCallback(uint16_t status, const uint8_t* buffer, const uint16_t length);
void acceptCallback(int32_t sock_id, int16_t dest_port, uint8_t* ip_addr, int16_t ip_version);
void httpCallback(uint8_t type, uint8_t *url_name, uint8_t *post_content_buffer, uint32_t post_content_length, uint32_t status);
void socket_connect_response_handler(uint16_t status, uint8_t *buffer, const uint32_t length);



#ifdef __cplusplus
    }
#endif

#endif
