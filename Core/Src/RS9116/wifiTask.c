/* Includes ------------------------------------------------------------------*/
#include "ringBuffer.h"
#include "backup.h"
#include "main_bg.h"
#include "variable.h"
#include "main.h"
#include "wifiTask.h"
#include "multicastLudp.h"
#include "stm32f7xx_hal.h"
#include "socketControl.h"
#include "eventGraphTask.h"
#include "eventRealtimeTask.h"
#include "rsi_driver.h"
#include "rsi_common_apis.h"        // COMMON include file to refer wlan APIs
#include "httpServer.h"
#include "graphMonitoring.h"
#include "realtimeMonitoring.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t WifiCtrl = WIFI_INIT, WifiState = WIFI_DONTUSE;
static uint8_t InitRetryCount = 0;
static uint8_t wifiError;

/* Global variables -----------------------------------------------------------*/
NetworkTypeDef     WifiEthSet;          // Ethernet Config: IP, Gateway, Netmask ...
WifiAPTypeDef      WifiAPSet;           // AP Config: SSID, Password, Security of AP
WifiMsgState       WifiMsg;             // Message receive check
WifiTimeoutTypdef  WifiTimeout;         // WI-FI의 Timtout을 체크하기 위한 Struct
WifiEapSetTypedef  wifiEapSet;


uint8_t  eapCertification[1024 * 8];
char     bmHostName[16]       = {0, };  // BM HOST Name

volatile uint8_t spiRxComplete = 1;     // When send data RX complete Flag
volatile uint8_t spiTxComplete = 1;     // When send data TX complete Flag

extern rsi_socket_info_non_rom_t *rsi_socket_pool_non_rom;

/* Functions ------------------------------------------------------------------*/
/*****************************************************************************************************************************
 *  Sequence: Wifi Module Init & AP Connect - (Success) - Recv module response - Multicast - TCP - HTTP
 *                                          - (Fail)    - Retry connection with AP
 *                                          +  Check WIFI Module Update 
 * ***************************************************************************************************************************/
void WifiProcess(void)
{
    int16_t status = 0;

    // Reset Wifi module when communication fail
    ResetWifiModule();    
    wifiErrorDisplay();

    switch(WifiCtrl)
    {
        case WIFI_INIT:
            status = InitRS9113(InitRetryCount);

            if(status == WIFI_INIT_SUCCESS)  // Init Success
            {
                if(web_content[0].content_len == 0)
                    httpWebpageLoad();      // Webpage load

                WifiCtrl = WIFI_START;
            }
            else
            {   
                // Can't connect with AP
                WifiCtrl = WIFI_INIT;      // 재시도

                if(InitRetryCount < 0xFC)  // Count increase
                    InitRetryCount++;

                // ERROR display
                if(system_err == 0 && HAL_GetTick() > 23000 && HAL_GetTick() < 24000)
                    system_err = INIT_FAIL_WIFI;
            }
            break;

        case WIFI_START:
            WifiCtrl  = WIFI_NOTUSE;
            WifiState = WIFI_WORK;

            WifiAPSet.Status = ON;
            break;

        case WIFI_STOP:
            WifiCtrl  = WIFI_NOTUSE;        // 종료
            WifiState = WIFI_DONTUSE;
            break;
            
        default:
            break;
    }

    if(WifiState == WIFI_WORK)
    {
        // Get message response
        checkResponse();

        // TCP 연결 상태 확인
        for(uint8_t i = UDP_SOCKET; i <= RSI_NUMBER_OF_SOCKETS; i++)
            setSocketConnectState(i);

        // Multicast process
        MulticastProcess();

        // TCP client 
        TcpClientRun();

        // HTTP Server Run
        httpServerRun();

        rsi_semaphore_wait(&rsi_driver_cb_non_rom->common_cmd_send_sem, 1);
    }
    // WIFI operation X
    else
        WifiAPSet.Status = 0;

    // Driver lock, module reset when wifi update
    WifiResetLock();
}


// Wifi TCP Socket RUN, Check receive data
int32_t TcpClientRun(void)
{
    static uint8_t socketType = TCP_CLIENT_MODBUS_SOCKET;
    uint8_t socketNum;    

    // Type 변경
    socketType++;

    if(socketType > RSI_NUMBER_OF_SOCKETS)
        socketType = TCP_CLIENT_MODBUS_SOCKET;

    // Find socket description
    socketNum = findSocketDescription(socketType);

    // Check socket number
    if(socketNum > RSI_NUMBER_OF_SOCKETS || socketNum == 0)
        return -1;

    // 소켓 데이터 받기
    tcpRecvData(socketNum, socketControl.socketState[socketNum].recvBuf, 512);

    // Check receive data
    if(socketControl.socketState[socketNum].recvCountOld != socketControl.socketState[socketNum].recvCount)
    {
        for(uint16_t i=0; i < socketControl.socketState[socketNum].recvLength; i++)
        {
            rb_put(&WifiRecvBuf, socketControl.socketState[socketNum].recvBuf[i]);
        }

        // Message receive complete
        socketControl.socketState[socketNum].recvCountOld = socketControl.socketState[socketNum].recvCount;
        socketControl.lastCommSocket = socketNum;
    }

    WiFi_DataReceive();

    return 0;
}


// HTTP Server check receive data
int32_t httpServerRun(void)
{
    if(WifiMsg.MsgHttp)
        httpServerProcess();

    return 0;
}


// Initialize wifi parameter (DHCP, IP, Port, Gateway, Netmask, SSID etc...)
void initWifiPara(void)
{
    // Network - DHCP
    WifiEthSet.DHCP = (uint8_t)Para[STATIC_DHCP].val;                   // 0: STATIC_MODE; 1: DHCP_MODE

    if (WifiEthSet.DHCP == RSI_STATIC_IP_MODE)
    {
        // Network - IP
        WifiEthSet.DEVICE_IP[0] = (uint8_t)Para[IP_ADDRESS1].val;       // 192.168.0.101 (Static)
        WifiEthSet.DEVICE_IP[1] = (uint8_t)Para[IP_ADDRESS2].val;
        WifiEthSet.DEVICE_IP[2] = (uint8_t)Para[IP_ADDRESS3].val;
        WifiEthSet.DEVICE_IP[3] = (uint8_t)Para[IP_ADDRESS4].val;
        // Network - Gateway
        WifiEthSet.GATEWAY[0]   = (uint8_t)Para[GATEWAY1].val;          // 192.168.0.1
        WifiEthSet.GATEWAY[1]   = (uint8_t)Para[GATEWAY2].val;
        WifiEthSet.GATEWAY[2]   = (uint8_t)Para[GATEWAY3].val;
        WifiEthSet.GATEWAY[3]   = (uint8_t)Para[GATEWAY4].val;
        // Network - NetMask
        WifiEthSet.NETMASK[0]   = (uint8_t)Para[SUBNETMASK1].val;       // 255.255.255.0
        WifiEthSet.NETMASK[1]   = (uint8_t)Para[SUBNETMASK2].val;
        WifiEthSet.NETMASK[2]   = (uint8_t)Para[SUBNETMASK3].val;
        WifiEthSet.NETMASK[3]   = (uint8_t)Para[SUBNETMASK4].val;
    }
    
    // Target Server IP
    WifiEthSet.TARGET_IP[0] = (uint8_t)Para[TARGET_IP1].val;
    WifiEthSet.TARGET_IP[1] = (uint8_t)Para[TARGET_IP2].val;
    WifiEthSet.TARGET_IP[2] = (uint8_t)Para[TARGET_IP3].val;
    WifiEthSet.TARGET_IP[3] = (uint8_t)Para[TARGET_IP4].val;
    WifiEthSet.TARGET_PORT  = Para[TARGET_PORT].val;


    // Access Point - SSID
    for (uint8_t i=0; i < 16; i++)
    {
        WifiAPSet.SSID[i * 2]       = (char)(Para[AP_SSID_CHAR1 + i].val >> 8);
        WifiAPSet.SSID[(i * 2) + 1] = (char)(Para[AP_SSID_CHAR1 + i].val & 0xff);
    }

    // Access Point - Password
    for(uint8_t i = 0; i < 32; i++)
    {
        WifiAPSet.PASSWORD[i * 2]       = (char)(Para[AP_PW_CHAR1 + i].val >> 8);
        WifiAPSet.PASSWORD[(i * 2) + 1] = (char)(Para[AP_PW_CHAR1 + i].val & 0xff);
    }
        

    // Network - Port
    WifiEthSet.DEVICE_PORT = Para[ETHERNET_PORT].val;  
    WifiEthSet.WifiOperate = Para[WIFI_SELECT].val;     // Wifi Operate On, Off
    WifiAPSet.RSSI = 0;                                 // Wifi Signal strength
    WifiAPSet.AP_COUNTRY   = Para[AP_COUNTRY].val;      // Country type
    WifiAPSet.Channel = 0;                              // Default: scan all

    // RS9113 Message struct init
    WifiMsg.MsgRSSI       = 1;
    WifiMsg.MsgMulticast  = 1;
    WifiMsg.MsgSocket     = 1;
    WifiMsg.MsgHttp       = 0;

    multiWaitCount = 6000;  // Multicast message wait time count
    WifiInitOn = OFF;

    // AP Country 설정 (Default 일 때 US로 설정)
    if(WifiAPSet.AP_COUNTRY == 0)
        WifiAPSet.AP_COUNTRY = 1;



    // EAP 설정
    wifiEapSet.enable = Para[WIFI_EAP_ENABLE].val;


    // eapMethod
    switch(Para[WIFI_EAP_METHOD].val)
    {
        case 0:
            sprintf(wifiEapSet.eapMethod, "%s", "TLS");
        break;

        case 1:
            sprintf(wifiEapSet.eapMethod, "%s", "TTLS");
        break;

        case 2:
            sprintf(wifiEapSet.eapMethod, "%s", "PEAP");
        break;

        case 3:
            sprintf(wifiEapSet.eapMethod, "%s", "FAST");
        break;

        case 4:
            sprintf(wifiEapSet.eapMethod, "%s", "LEAP");
        break;

        default:
            memset(wifiEapSet.eapMethod, 0, sizeof(wifiEapSet.eapMethod));
        break;
    }

    // userIdentity
    uint8_t indexCnt = 0;
    uint8_t charHigh;
    uint8_t charLow;

    for(uint8_t i=0; i < 16; i++)
    {
        if(Para[WIFI_EAP_USER_ID_1].val == 0)
        {
            wifiEapSet.password[indexCnt++] = 0x22;
        	wifiEapSet.password[indexCnt++] = 0x22;
            break;
        }
        else
        {
            // 처음에 "문자 적용
            if(i == 0)
                wifiEapSet.userIdentity[indexCnt++] = 0x22;          // " 를 의미

            // Make character
            charHigh = (uint8_t)(Para[WIFI_EAP_USER_ID_1 + i].val >> 8);
            charLow = (uint8_t)Para[WIFI_EAP_USER_ID_1 + i].val;

            // Apply character
            if(charHigh)
                wifiEapSet.userIdentity[indexCnt++] = charHigh;         // High
            else
            {
                wifiEapSet.userIdentity[indexCnt++] = 0x22;
                break;
            }

            if(charLow)
                wifiEapSet.userIdentity[indexCnt++] = charLow;         // Low
            else
            {
                wifiEapSet.userIdentity[indexCnt++] = 0x22;
                break;
            }

            // 끝에 "문자 적용
            if(i == 16 - 1)
                wifiEapSet.password[indexCnt++] = 0x22;          // " 를 의미
        }
    }

    // EAP password
    indexCnt = 0;

    for(uint8_t i=0; i < 16; i++)
    {
        if(Para[WIFI_EAP_USER_PASSWORD_1].val == 0)
        {
        	wifiEapSet.password[indexCnt++] = 0x22;
        	wifiEapSet.password[indexCnt++] = 0x22;
            break;
        }
        else
        {
            // 처음에 "문자 적용
            if(i == 0)
                wifiEapSet.password[indexCnt++] = 0x22;          // " 를 의미

            // Make character
            charHigh = (uint8_t)(Para[WIFI_EAP_USER_PASSWORD_1 + i].val >> 8);
            charLow = (uint8_t)Para[WIFI_EAP_USER_PASSWORD_1 + i].val;

            // Apply character
            if(charHigh)
                wifiEapSet.password[indexCnt++] = charHigh;         // High
            else
            {
                wifiEapSet.password[indexCnt++] = 0x22;
                break;
            }

            if(charLow)
                wifiEapSet.password[indexCnt++] = charLow;         // Low
            else
            {
                wifiEapSet.password[indexCnt++] = 0x22;
                break;
            }

            // 끝에 "문자 적용
            if(i == 16 - 1)
                wifiEapSet.password[indexCnt++] = 0x22;          // " 를 의미
        }
    }


    // Password - PSK, PMK
    uint8_t j = 0;
    uint8_t passwordLength = 64;
    uint8_t temp_psk[64];

    // Decide AP Password Mode - PSK, PMK
    for(uint8_t i=0; i < sizeof(WifiAPSet.PASSWORD); i++)
    {
        // 총 Password 개수
        if(WifiAPSet.PASSWORD[i] == 0)
        {
            passwordLength = i;
            break;
        }
    }

    if(passwordLength == 64)
        WifiAPSet.PasswordMode = 1;         // PMK (64글자가 있을 경우 PMK 모드로 판단)
    else
        WifiAPSet.PasswordMode = 0;         // PSK

    // ASCII -> HEX
    if(WifiAPSet.PasswordMode == 1)
    {
        memcpy(temp_psk, WifiAPSet.PASSWORD, 64);

        for(uint8_t i=0 , j=0 ; i < 64 && j < 32; i+=2,j++)
        {
            if(temp_psk[i] && temp_psk[i + 1])
            {
                WifiAPSet.PASSWORD[j]  = ((uint16_t)convert_lower_case_to_upper_case(temp_psk[i])) * 16;
                WifiAPSet.PASSWORD[j] += convert_lower_case_to_upper_case(temp_psk[i + 1]); 
            }
            else
            {
                WifiAPSet.PASSWORD[j] = 0;
            }
        }
    }


    // 초기화 중에 Lock
    if (WifiEthSet.WifiOperate)
    {
        if(Flag.Lock == OFF)
            Flag.Lock = WIFI_INIT_LOCK;

        HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port, WIFI_RESET_Pin, OFF);
        HAL_Delay(300);

        HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port, WIFI_RESET_Pin, ON);
        HAL_Delay(300);     // 모듈 초기화 대기 딜레이
    }
}


// Check message response from wifi module, request TCP Connection, RSSI, Multicast
int checkResponse(void)
{
    // Check Connection with AP
    CheckConnectAp();
    
    // AP 연결된 상태에서 Socket create, Multicast, RSSI 실행
    RequestNetworkSetting();


    // Save 소켓 타입 및 ip, port
    for(uint8_t i=0; i < RSI_NUMBER_OF_SOCKETS; i++)
    {
        if(socketControl.socketState[i].netInfo.srcPort == 0 && rsi_socket_pool[i].sock_state >= RSI_SOCKET_STATE_LISTEN)
            saveServerSocket(i);
    }

    return 0;
}

/**************************************************************************************************************************
 *  RS9113 WIFI Module initialize 
 * 
 *  Sys_init -> boardready 후
 *  Sequence: RSI_RSP_CARD_READY - OPERMODE - FWVERSION - BAND - INIT - SET_REGION - AP SCAN - JOIN - IP PARAMETER_CONFIG
 *                                 OPERMODE 후 - RSI_RSP_CLEAR_FILES - RSI_RSP_GET_WEBPAGE 추가실행 (Webpage Module에 저장)
 * 
 *  retval: WIFI_INIT_SUCCESS (0xF0): RS9113 Init Success
 *          WIFI_AP_CONNECT_FAIL(-1): Connect with AP Fail (Scan or Join fail)
 *          WIFI_INIT_SYS_FAIL (-10): Module System init Fail
 *          WIFI_WAIT_MESSAGE (0x01): Wait Module response message
 * 
 * *************************************************************************************************************************/
int16_t InitRS9113(uint8_t InitRetryCount)
{
    static uint32_t timeout;
    static uint8_t resp_buf[20];
    static uint8_t initState = 0;
    int32_t status;

    // 모듈 초기화 순서 처음부터 시작
    if(InitRetryCount == 0)
        initState = 0;

    // 초기화 실행 주기 500ms
    if(HAL_GetTick() - timeout > 500 && HAL_GetTick() > 500)
    {
        timeout = HAL_GetTick();
    }
    else
    {
        return -1;
    }


    // EAP의 ID, PW 설정
    rsi_eap_credentials_t eapScrect;
    rsi_strcpy(eapScrect.username, wifiEapSet.userIdentity);
    rsi_strcpy(eapScrect.password, wifiEapSet.password);


    // WIFI 모듈 초기화
    switch(initState)
    {
        case 0:
            // Driver initialization
            status = rsi_driver_init(wifiModule_buf, WIFI_MODULE_BUF_LEN);
            if((status < 0) || (status > WIFI_MODULE_BUF_LEN))
                return status;
            
            initState = 1;
        break;

        case 1:
            // RS9113 intialisation (650ms 정도 소요)
            status = rsi_device_init(LOAD_NWP_FW);
            if(status != RSI_SUCCESS)
                return status;

            initState = 2;
        break;
            
        case 2:
            status = rsi_wlan_get(RSI_FW_VERSION, resp_buf, RSI_FIRMWARE_NAME_SIZE);
            memcpy(WifiEthSet.FirmVersion, &resp_buf[5], sizeof(WifiEthSet.FirmVersion));
			printf("FW: %s\n", WifiEthSet.FirmVersion);


            // WC initialization
            if(wifiEapSet.enable)
                status = rsi_wireless_init(2, 0);       // EAP mode
            else
                status = rsi_wireless_init(0, 0);       // Normal mode

            if(status != RSI_SUCCESS)
                return status;
                                
            initState = 3;
        break;

        case 3:
            // Power save mode 해제
            status = rsi_wlan_power_save_profile(RSI_ACTIVE, RSI_MAX_PSP);

            // Send feature frame (RF 안테나 선택 - internal or external)
            status = rsi_send_feature_frame();
            if(status != RSI_SUCCESS)
                return status;

            initState = 4;
        break;

        case 4:
            // Scan for Access points
            if(rsi_driver_cb->wlan_cb->state < RSI_WLAN_STATE_INIT_DONE)
            {
                status = rsi_wlan_scan_async((int8_t *)WifiAPSet.SSID, CHANNEL_NO, wlanScanCallback);
                printf("scan\n");

                return -1;
            }
            else
            {
                //! wait for signal from main WLAN task (Wait async)
                rsi_wait_on_wlan_semaphore(&rsi_driver_cb_non_rom->wlan_cmd_sem, 1);

                // Scan이 실패 했을 때 재시도
                if(rsi_wlan_cb_non_rom->callback_list.wlan_scan_response_handler == 0 && rsi_driver_cb->wlan_cb->state != RSI_WLAN_STATE_SCAN_DONE)
                {
                    status = rsi_wlan_scan_async((int8_t *)WifiAPSet.SSID, CHANNEL_NO, wlanScanCallback);
                }

                if(rsi_driver_cb->wlan_cb->state != RSI_WLAN_STATE_SCAN_DONE)
                    return -1;
            }

            initState = 5;
        break;

        case 5:
            // Connect to an Access point
            if(rsi_driver_cb->wlan_cb->state == RSI_WLAN_STATE_SCAN_DONE && rsi_driver_cb->wlan_cb->expected_response == RSI_WLAN_RSP_CLEAR)
            {
                if(wifiEapSet.enable)
                    status = rsi_wlan_connect_async((int8_t *)WifiAPSet.SSID, WifiAPSet.SECURITY_TYPE, &eapScrect, wlanConnectCallback);            // EAP mode
                else
    			    status = rsi_wlan_connect_async((int8_t *)WifiAPSet.SSID, WifiAPSet.SECURITY_TYPE, WifiAPSet.PASSWORD, wlanConnectCallback);    // Normal mode

                return -1;
            }
            else
            {
                rsi_wait_on_wlan_semaphore(&rsi_driver_cb_non_rom->wlan_cmd_sem, 1);
                
                if(rsi_driver_cb->wlan_cb->expected_response == RSI_WLAN_RSP_JOIN)
                    return -1;
            }
            
            initState = 6;
        break;

        case 6:
            // IP 할당 및 Callback 등록
            if(rsi_driver_cb->wlan_cb->state == RSI_WLAN_STATE_CONNECTED && rsi_driver_cb->wlan_cb->expected_response == RSI_WLAN_RSP_CLEAR)
            {
                // SPI의 속도 25MHz 이상일 때 설정
                rsi_spi_high_speed_enable();

                // Callback 함수 등록
                rsi_wlan_nwk_register_webpage_req_cb(RSI_WLAN_NWK_URL_REQ_CB, httpCallback);                            // HTTP 데이터 callback
                rsi_wlan_register_callbacks(RSI_WLAN_SOCKET_CONNECT_NOTIFY_CB, socket_connect_response_handler);        // 소켓 연결 callback

                // Configure IP
                if(!WifiEthSet.DHCP)
                {
                    // Set Static IP
                    status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_STATIC, (uint8_t *)&WifiEthSet.DEVICE_IP, (uint8_t *)&WifiEthSet.NETMASK, (uint8_t *)&WifiEthSet.GATEWAY, resp_buf, sizeof(resp_buf),0);
                }
                else
                {
                    // Get DHCP IP
                    status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, resp_buf, sizeof(resp_buf),0);
                }
            }
            else
            {
                // Wait on WLAN semaphore
                rsi_wait_on_wlan_semaphore(&rsi_driver_cb_non_rom->wlan_cmd_sem, 1);
            }

            // IP 설정 완료
            if(rsi_driver_cb->wlan_cb->state == RSI_WLAN_STATE_IP_CONFIG_DONE)
            {
                // Get MAC address
                WifiEthSet.MACADDRESS[0] = resp_buf[0];
                WifiEthSet.MACADDRESS[1] = resp_buf[1];
                WifiEthSet.MACADDRESS[2] = resp_buf[2];
                WifiEthSet.MACADDRESS[3] = resp_buf[3];
                WifiEthSet.MACADDRESS[4] = resp_buf[4];
                WifiEthSet.MACADDRESS[5] = resp_buf[5];

                // IP
                WifiEthSet.DEVICE_IP[0]  = resp_buf[6];
                WifiEthSet.DEVICE_IP[1]  = resp_buf[7];
                WifiEthSet.DEVICE_IP[2]  = resp_buf[8];
                WifiEthSet.DEVICE_IP[3]  = resp_buf[9];

                // NetMask
                WifiEthSet.NETMASK[0]   = resp_buf[10];         // 255.255.255.0
                WifiEthSet.NETMASK[1]   = resp_buf[11];
                WifiEthSet.NETMASK[2]   = resp_buf[12];
                WifiEthSet.NETMASK[3]   = resp_buf[13];
                
                // Gateway
                WifiEthSet.GATEWAY[0]   = resp_buf[14];         // 192.168.0.1
                WifiEthSet.GATEWAY[1]   = resp_buf[15];
                WifiEthSet.GATEWAY[2]   = resp_buf[16];
                WifiEthSet.GATEWAY[3]   = resp_buf[17];
                printf("%d, set ip, %d.%d.%d.%d\n", HAL_GetTick(), WifiEthSet.DEVICE_IP[0], WifiEthSet.DEVICE_IP[1], WifiEthSet.DEVICE_IP[2], WifiEthSet.DEVICE_IP[3]);


                // // Firmware update mode start TODO: 실제 HTTP로 펌웨어 업데이트를 시작하기 전에 실행해줘야 한다. (INDEX.html 페이지 주석처리 필요)
                // rsi_req_wireless_fwup();

            }
            else
                // IP 설정 대기 중
                return -1;


            // Init End
            if(WifiEthSet.DEVICE_IP[0] != 0)
                initState = 7;
        break;
        
        default:
        break;
    }

    if(initState == 7)
    {
        initState = 0;              // 초기화 완료 시 index 초기화
        return WIFI_INIT_SUCCESS;
    }
    else
        return WIFI_WAIT_MESSAGE;
}


// Check Connection with AP
void CheckConnectAp(void)
{
    static uint8_t errDisplayOnce = 1;

    // Get AP State
    getApState();

    // AP Disconnection check
    if(WifiAPSet.Status == OFF)
    {
        // Init status
        WifiAPSet.Status   = 0;
        WifiAPSet.RSSI     = 0;
        TRIG.MointoringEthernet = 0;        // Ethernet monitoring

        // Init socket info
        soketInitWhenDisconnect();

        // Init event monitoring Ack
        initGraphEventAckWait();
        initRealtimeEventAckWait();

        // Error display
        if(errDisplayOnce)
        {
            // AP disconnection err
            if(wifiError == 0)
            {
                wifiError = NO_CONNECTION_WIFI;
                errDisplayOnce = 0;                  // Wifi는 일정 시간동안 끊어져 있기 때문에 한번 display하고 다시 접속 전까지 표시 막음
            }
        }
    }
    else
    {
        WifiAPSet.Status = ON;
        errDisplayOnce   = ON;
    }
}

// AP 연결된 상태에서 Socket create, Multicast 실행
void RequestNetworkSetting(void)
{
    static uint32_t rssiTime;
    static uint32_t connectTimeout; 
    static uint8_t  connectSocketType = TCP_CLIENT_MODBUS_SOCKET;
    static uint32_t acceptTimeout;
    static uint8_t  acceptSocketType = TCP_SERVER_MODBUS_LISTEN;
    uint8_t socketType = 0;

    if(WifiAPSet.Status == ON)
    {
        // Socket Server create  (100ms 마다 Create)
        if(HAL_GetTick() - WifiTimeout.createSocketTime > 100)
        {
            // 생성이 안된 Type 찾기
            for(uint8_t type = 1; type < RSI_NUMBER_OF_SOCKETS; type++)
            {
                // UDP 소켓 생성 시 Socket 번호가 0번이라 2번 생성되는 것 방지
                if(type == UDP_SOCKET && socketControl.socketState[0].type != 0)
                    type++;

                if(findSocketDescription(type) == 0)
                {
                    socketType = type;
                    break;
                }
            }

            // Socket 생성
            if(socketType != 0)
            {
                // Make socket struct data
                makeSocket(socketType);
            }
                
            // Loop Time reset
            WifiTimeout.createSocketTime = HAL_GetTick();
        }


        // TCP Server 소켓 접속 허용
        if(HAL_GetTick() - acceptTimeout > 100)
        {
            acceptTcpServer(findSocketDescription(acceptSocketType++));

            if(acceptSocketType > TCP_SERVER_EVENT_LISTEN)
                acceptSocketType = TCP_SERVER_MODBUS_LISTEN;

            acceptTimeout = HAL_GetTick();
        }

        // TCP Client 소켓 접속 시도
        if(HAL_GetTick() - connectTimeout > 5000)
        {
            trySocketConnect(connectSocketType++);
        
            if(connectSocketType > TCP_CLIENT_EVENT_SOCKET)
                connectSocketType = TCP_CLIENT_MODBUS_SOCKET;

            connectTimeout = HAL_GetTick();
        }
        
        // Multicast setting
        initMulticast();
    }

    // RSSI check (1.5s 마다)
    if(HAL_GetTick() - rssiTime > 1500)
    {
        WifiAPSet.RSSI = getRssi();

        rssiTime = HAL_GetTick();
    }
}



// Count time (ms)
void WifiSysLoop(void)
{
    // Wifi AP Connection check
    if(WifiAPSet.Status)
    {
        multiWaitCount++;                       // Multicast send wait time
    }
}

// Wifi module reset process when shock electric damage
void ResetWifiModule(void)
{
    static uint32_t ApDisconCount;
    static uint32_t ResetGpioCount;
    static uint32_t rebootTime;
    static uint8_t  ResetState;
    static uint8_t  apConnectOnce;

    // Save AP State (AP가 연결이 된 적이 있는지 저장)
    if(WifiAPSet.Status)
        apConnectOnce = ON;

    // Check AP Reboot
    if(WifiAPSet.Status == OFF && HAL_GetTick() > 40000 && apConnectOnce)
    {
        if(ApDisconCount == 0)
            ApDisconCount = HAL_GetTick();

        // 30초 이상 연결이 끊어졌을 경우
        if(HAL_GetTick() - ApDisconCount > 30000)
        {
            WifiInitOn = ON;
        }
    }
    else
        ApDisconCount = 0;

    
    // Communication Recovery process (after 40s)
    if(WifiInitOn && HAL_GetTick() > 40000)
    {
        switch(ResetState)
        {
            case 0:
                // Check reboot time (다시 재부팅할 경우 30초 이상 지났을 때 가능)
                if(HAL_GetTick() - rebootTime > 30000)
                {
                    // RS9113 Reset GPIO
                    HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port, WIFI_RESET_Pin, OFF);

                    Flag_SendOledData = OFF;
                    WifiAPSet.Status  = OFF;
                    apConnectOnce = OFF;
                    
                    WifiCtrl = WIFI_STOP;       // 잠시 중지
                    WifiState = WIFI_NOTUSE;
                    MultiStep = 0;

                    InitRetryCount = 0xFE;         
                    multiWaitCount = 6000;   

                    // Init socket info
                    soketInitWhenDisconnect();

                    WifiMsg.MsgMulticast = 1;
                    WifiMsg.MsgRSSI = 1;
                    WifiMsg.MsgSocket = 1;

                    ResetState = 1;
                    rebootTime = HAL_GetTick();
                }
            break;

            // Reboot RS9113 GPIO
            case 1:
                // RS9113 Reboot GPIO (500ms delay)
                if(HAL_GetTick() - ResetGpioCount > 500)
                {
                    HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port, WIFI_RESET_Pin, ON);  
                    ResetGpioCount = HAL_GetTick();
                    ResetState = 2;
                }    
            break;

            // Complete
            case 2:
                if(HAL_GetTick() - ResetGpioCount > 500)
                {
                    WifiInitOn = OFF;
                    ResetState = 0;

                    if(WifiAPSet.Status == OFF)
                        WifiCtrl = WIFI_INIT;
                }
            break;
            default:
                ResetState = 0;
            break;
        }
    }
}


// Driver lock when wifi disconnect, Module reset when wifi update
void WifiResetLock(void)
{
    static uint8_t OneTimeLock = ON;
    static uint8_t DisconLockCheck;
    static uint32_t CountLockTime;
    static uint8_t CheckWifiOn;
    
    // 초기 Init Lock release
    if(OneTimeLock)
    {
        if(Flag.Lock == WIFI_INIT_LOCK)
            Flag.Lock = OFF;

        OneTimeLock = OFF;
    }


    // Wifi on된 거 체크
    if(WifiCtrl == WIFI_START)
        CheckWifiOn = ON;

    // Driver lock when wifi disconnect some times(sec)
    if(CheckWifiOn && Para[LOCK_WIFI_DICONNECT_TIME].val != 0)
    {
        // Wifi disconnect
        if(WifiAPSet.Status == OFF)
        {
            if(CountLockTime == 0)
                CountLockTime = HAL_GetTick();

            // Setting 시간 후 Lock,  1회만 실행
            if(HAL_GetTick() - CountLockTime > Para[LOCK_WIFI_DICONNECT_TIME].val * 1000 + 3000 && DisconLockCheck == OFF)
            {
                Flag.Lock = ON;
                DisconLockCheck = ON;
            }
        }
        // Connect
        else if(CountLockTime != 0)
        {
            if(DisconLockCheck)
                Flag.Lock = OFF;

            CountLockTime = 0;
            DisconLockCheck = OFF;
        }
    }
}


/* Monitoring 관리 --------------------------------------------------------------------------------------------*/

/**
 * @brief Ethernet 모니터링 데이터를 전송할 때 활성화되어 았는 Socket으로 전송
 * 
 * @param length : 전송 길이
 * @param type : 0 = Realtime monitoring, 1 = Graph monitoring,
 */
void socketMonitoringSend(uint16_t length, uint8_t type)
{
    uint8_t sendOn;
    uint8_t socketNum;

    // Monitoring 종류 선택
    switch (type)
    {
        case MONITORING_REALTIME:
            // Realtime 모니터링 전송
            for(uint8_t i=0; i <= RSI_NUMBER_OF_SOCKETS; i++)
            {
                sendOn = OFF;
                socketNum = i;

                // Realtiem flag 모드
                if(socketControl.socketState[socketNum].monitoring.flagRealtime)
                    sendOn = ON;

                // Auto data ouput 모드
                if(Para[AUTO_UPDATE_EVENT].val)
                {
                    if(socketControl.socketState[socketNum].type == TCP_SERVER_MODBUS_SOCKET   ||
                       socketControl.socketState[socketNum].type == TCP_SERVER_MODBUS_SOCKET_2 || 
                       socketControl.socketState[socketNum].type == TCP_CLIENT_MODBUS_SOCKET)
                    {
                        sendOn = ON;
                    }
                }

                if(sendOn)
                {
                    if(socketControl.socketState[socketNum].connect)
                    {
                        changeLastSocket(socketNum, ON);                            // 이전 통신 Socket 저장
                        mbMakeandSend_04(GRAPH_REALTIME_START_ADDRESS, length, MODBUS_ETHERNET_TYPE);
                        changeLastSocket(socketNum, OFF);                           // Socket 번호 복원
                    }
                }
            }
            break;

        case MONITORING_GRAPH_CH1:
            // Graph data 전송
            for(uint8_t socketNum=0; socketNum <= RSI_NUMBER_OF_SOCKETS; socketNum++)
            {
                if(socketControl.socketState[socketNum].monitoring.flagGraph)
                {
                    if(socketControl.socketState[socketNum].connect)
                    {
                        changeLastSocket(socketNum, ON);        // 이전 통신 Socket 저장

                        if(TRIG.Channel[0])
                            mbMakeandSend_64(GRAPH_DATA_CAHNNEL1_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE, length, MODBUS_ETHERNET_TYPE);

                        changeLastSocket(socketNum, OFF);       // Socket 번호 복원
                    }
                }
            }
            break;

        case MONITORING_GRAPH_CH2:
            // Graph data 전송
            for(uint8_t socketNum=0; socketNum <= RSI_NUMBER_OF_SOCKETS; socketNum++)
            {
                if(socketControl.socketState[socketNum].monitoring.flagGraph)
                {
                    if(socketControl.socketState[socketNum].connect)
                    {
                        changeLastSocket(socketNum, ON);        // 이전 통신 Socket 저장

                        if(TRIG.Channel[1])
                            mbMakeandSend_64(GRAPH_DATA_CAHNNEL2_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE, length, MODBUS_ETHERNET_TYPE);

                        changeLastSocket(socketNum, OFF);       // Socket 번호 복원
                    }
                }
            }
            break;

        case MONITORING_GRAPH_REALTIME:
            // Graph data 전송
            for(uint8_t socketNum=0; socketNum <= RSI_NUMBER_OF_SOCKETS; socketNum++)
            {
                if(socketControl.socketState[socketNum].monitoring.flagGraph)
                {
                    if(socketControl.socketState[socketNum].connect)
                    {
                        changeLastSocket(socketNum, ON);        // 이전 통신 Socket 저장

                        // Realtime data
			            mbMakeandSend_65(GRAPH_REALTIME_START_ADDRESS, sizeof(pollingRealtimeDataStruct) / sizeof(uint16_t), MODBUS_ETHERNET_TYPE);

                        changeLastSocket(socketNum, OFF);       // Socket 번호 복원
                    }
                }
            }
            break;
        
        default:
            break;
    }
}


/**
 * @brief Monitoring을 해당하는 Modbus socket에 따라 활성화.
 * 
 * @param flagSocket : 매개변수 socketType의 값 의미 (0: Socket Type, 1: Socket number)
 * @param socketType : 통신한 Socket의 type
 * @param eventType  : 활성화하려는 Monitoiring 종류
 * @param set        : ON, OFF
 */
void setSocketMonitoringControl(uint8_t flagSocket ,uint8_t socketType, uint8_t eventType, uint8_t set)
{
    uint8_t socketDescription;

    // Socket type의 종류
    if(flagSocket == SOCKET_TYPE)
    {
        // Find socket description
        switch (socketType)
        {
            case TCP_SERVER_MODBUS_SOCKET:
            case TCP_SERVER_MODBUS_SOCKET_2:
                socketDescription = findSocketDescription(socketType);
                break;
            
            case TCP_CLIENT_MODBUS_SOCKET:
                socketDescription = findSocketDescription(TCP_CLIENT_MODBUS_SOCKET);
                break;

            default:
                socketDescription = 0;
                break;
        }
    }
    else
        socketDescription = socketType;     // 실제 socket number일 때
    

    // Check socket limit
    if(socketDescription > RSI_NUMBER_OF_SOCKETS || socketDescription == 0)
        return;

    // 다른 소켓에서 모니터링 사용 중이면 동시 출력 제한
    if(set == ON)
    {
        for(uint8_t i=0; i <= RSI_NUMBER_OF_SOCKETS; i++)
        {
            // 이벤트가 다른 소켓에서 사용중인지 판단
            switch (eventType)
            {
                case MONITORING_REALTIME:
                    if(socketControl.socketState[i].monitoring.flagRealtime)
                        eventType = 0xFF;
                    break;

                case MONITORING_GRAPH:
                    if(socketControl.socketState[i].monitoring.flagGraph)
                        eventType = 0xFF;
                    break;
            }
        }
    }

    // Set
    switch (eventType)
    {
        case MONITORING_REALTIME:
            socketControl.socketState[socketDescription].monitoring.flagRealtime = set;
            break;

        case MONITORING_GRAPH:
            socketControl.socketState[socketDescription].monitoring.flagGraph = set;
            break;

        default:
            break;
    }
}


/**
 * @brief WI-FI 관련 Error를 표시한다.
 * @note  WI-FI가 체결 도중에 끊어졌을 경우 체결이 완료 되고나서 에러 표시하도록 딜레이 시킨다.
 * 
 * */
void wifiErrorDisplay(void)
{
    if(wifiError == NO_CONNECTION_WIFI)
    {
        if(system_err == 0 && Flag.Run == 0)
        {
            system_err = wifiError;
            wifiError  = 0;
        }
    }
}


void wlanScanCallback(uint16_t status, const uint8_t* buffer, const uint16_t length)
{
    rsi_rsp_scan_t scanApInfo;

    memcpy(&scanApInfo, buffer, length);

    // Scan된 네트워크 정보 입력
    if(scanApInfo.scan_info[0].ssid[0] == WifiAPSet.SSID[0] && scanApInfo.scan_info[0].ssid[1] == WifiAPSet.SSID[1])
    {
        WifiAPSet.SECURITY_TYPE = scanApInfo.scan_info[0].security_mode;
        WifiAPSet.Channel = scanApInfo.scan_info[0].rf_channel;
        // WifiAPSet.RSSI = scanApInfo.scan_info[0].rssi_val;

        // Password - PMK
        if(WifiAPSet.PasswordMode)
            WifiAPSet.SECURITY_TYPE = RSI_WPA2_PMK;
    }
}

void wlanConnectCallback(uint16_t status, const uint8_t* buffer, const uint16_t length)
{
    // WIFI 모듈이 AP에 접속 성공
}

void acceptCallback(int32_t sock_id, int16_t dest_port, uint8_t* ip_addr, int16_t ip_version)
{
    // 접속된 Client 데이터 저장
    socketControl.socketState[sock_id].netInfo.destPort = dest_port;           // Client Port
    memcpy(socketControl.socketState[sock_id].netInfo.destIp, ip_addr, 4);     // Client ip address
}


void connectCallback(int32_t sock_id, uint16_t dest_port, uint8_t* ip_addr, int16_t ip_version)
{
    // 접속한 Server 데이터 저장
    // socketControl.socketState[socket_id].netInfo.destPort = dest_port;           // Server Port
    // memcpy(socketControl.socketState[socket_id].netInfo.destIp, ip_addr, 4);     // Server ip address
}

// HTML 데이터 요청 Callback
void httpCallback(uint8_t type, uint8_t *url_name, uint8_t *post_content_buffer, uint32_t post_content_length, uint32_t status)
{
    // HTML 상태 확인
    if(status == 0)
    {
        // HTTP URL, Data 받는 것 설정
        memcpy(httpUriBuf, url_name, 40);
        WifiMsg.MsgHttp = ON;

        // POST 데이터 받기
        if(post_content_length > 512)
            post_content_length = 512;

        // POST 데이터 Copy
        if(post_content_length)
            memcpy(httpRecvBuf, post_content_buffer, post_content_length);
    }
}

// 모든 소켓이 생성되어 접속될 때 호출된다 - 소켓에 접속 유무에 따라 status의 값이 달라지고 그에맞게 처리가 필요
void socket_connect_response_handler(uint16_t status, uint8_t *buffer, const uint32_t length)
{
	int32_t sockID;

	// Check status
	if(status == 0)
	{

	}
	else
	{
		// Socket Error
		rsi_rsp_socket_create_t *socket_rsp = (rsi_rsp_socket_create_t *)buffer;

		// Get socket ID
		sockID = rsi_bytes2R_to_uint16(socket_rsp->socket_id);
		rsi_socket_pool[sockID].destination_port = 0;
	}
}




/**
 * @brief Create a TCP Server 소켓
 * 
 * @param port : Port 번호
 * @param maxConnectNum : 최대 연결 가능한 개수
 * @return int (-1: 실패, 그외: 소켓 번호)
 */
int createTcpServer(uint16_t port, uint8_t maxConnectNum)
{
    uint8_t socketNum;
    int32_t status;
    struct rsi_sockaddr_in server_addr;
    
    // Create socket
    socketNum = rsi_socket(AF_INET, SOCK_STREAM, 0);

    // Check socket number
    if(socketNum > 10 || socketNum < 0)
        return - 1;

    // Set server socket
    server_addr.sin_family = AF_INET;                                               // family type
    server_addr.sin_port = htons(port);                                             // local port number
    server_addr.sin_addr.s_addr = rsi_bytes4R_to_uint32(WifiEthSet.DEVICE_IP);      // IP Address

    // Bind socket
    status = rsi_bind(socketNum, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));

    // Socket listen
    status = rsi_listen(socketNum, maxConnectNum);
    if (status != RSI_SUCCESS) {
        return -1;
    }

    // Set recv timeout
    rsi_socket_pool[socketNum].read_time_out = 10;      // 10ms (이걸 따로 설정 안하면 무한대기)

    return socketNum;
}


/**
 * @brief Create a TCP Client 소켓
 * 
 * @param port : Port 번호
 * @return int (-1: 생성 실패, 그외: Client 소켓 번호)
 */
int createTcpClient(uint16_t port)
{
    uint8_t socketNum;
    struct rsi_sockaddr_in client_addr;
    
    socketNum = rsi_socket(AF_INET, SOCK_STREAM, 0);

    // Check socket number
    if(socketNum > 10 || socketNum < 0)
        return - 1;

    // Set client socket
    client_addr.sin_family = AF_INET;                                               // family type
    client_addr.sin_port = htons(port);                                             // local port number
    client_addr.sin_addr.s_addr = rsi_bytes4R_to_uint32(WifiEthSet.DEVICE_IP);      // IP Address

    // Bind socket
    rsi_bind(socketNum, (struct rsi_sockaddr *)&client_addr, sizeof(client_addr));

    // Set recv timeout
    rsi_socket_pool[socketNum].read_time_out = 10;      // 10ms

    return socketNum;
}


/**
 * @brief Create a Udp Client 소켓
 * 
 * @param ip : 받고자 하는 ip 주소
 * @param port : 생성하고자 하는 소켓 번호
 * @return int (-값: 실패, 그외: 소켓 번호)
 */
int createUdpClient(uint8_t* ip, uint16_t port)
{
    int8_t socketNum;
    struct rsi_sockaddr_in client_addr;
    
    // Create UDP socket
    socketNum = rsi_socket(AF_INET, SOCK_DGRAM, 0);

    // Check socket number
    if(socketNum > 10 || socketNum < 0)
        return - 1;

    // Set client socket
    client_addr.sin_family = AF_INET;                                               // family type
    client_addr.sin_port = htons(port);                                             // local port number
    // client_addr.sin_addr.s_addr = rsi_bytes4R_to_uint32(WifiEthSet.DEVICE_IP);   // IP Address - 설정을 안 했을 때 모든 ip에서 오는 데이터를 받을 수 있다.

    // Bind socket
    rsi_bind(socketNum, (struct rsi_sockaddr *)&client_addr, sizeof(client_addr));

    // Set recv timeout
    rsi_socket_pool[socketNum].read_time_out = 10;      // 10ms

    return socketNum;
}


/**
 * @brief TCP 서버에서 Client 연결 허용
 * 
 * @param socketNum : TCP Server의 소켓 번호
 * @return int (-1: 연결 시도 없음, 그외: Client 소켓의 소켓 번호)
 */
int acceptTcpServer(uint8_t socketNum)
{
    int32_t new_socket = 0;

    // Check socket alive
    if(rsi_socket_pool[socketNum].sock_state != 3)
        return -1;

    // Check socket try one time (접속 개수를 1개만 하기위해 설정
    if(socketControl.socketState[socketNum].waitConnect)
        return -1;

    // Server 소켓에 접속 가능한 숫자만큼 연결 허용
    if(rsi_socket_pool[socketNum].backlog_current_count < rsi_socket_pool[socketNum].backlogs)
    {
        // TCP Server로 Client 접속 허용 
        socketControl.socketState[socketNum].waitConnect = ON;
        new_socket = rsi_accept_async(socketNum, acceptCallback);

        if (new_socket < 0) {
            // 접속한 Clientt 없음
            return -1;
        }
    }

    return new_socket;
}




/**
 * @brief TCP Client로 server에 접속 시도
 * 
 * @param socketNum : Client 소켓 번호
 * @param ipAddress : Server ip 주소
 * @param port : server 포트 번호
 * @return int (0: 성공, 그외: 실패)
 */
int connectTcpClient(uint8_t socketNum, uint8_t* ipAddress, uint16_t port)
{
    struct rsi_sockaddr_in server_addr;
    int32_t status = -1;

    // Check ip, port
    if(ipAddress[0] == 0 || port == 0)
        return -1;

    // Check socket alive
    if(rsi_socket_pool[socketNum].sock_state > 2 || rsi_socket_pool[socketNum].sock_state == 0)
        return -1;

    // Connect 시도 callback이 실행되기 전에 1회만 되도록 filter
    if(rsi_socket_pool[socketNum].destination_port != 0)
    	return -1;

    // Set server socket
    server_addr.sin_family = AF_INET;                                               // family type
    server_addr.sin_port = htons(port);                                             // local port number
    server_addr.sin_addr.s_addr = rsi_bytes4R_to_uint32(ipAddress);                 // IP Address

    if(rsi_socket_pool[socketNum].sock_state < 4)
    {
        // Server 소켓의 접속 시도
        status = rsi_connect(socketNum, (struct rsi_sockaddr *)&server_addr, sizeof(server_addr));
        if (status < 0) {
            // 접속한 Client 없음
        	rsi_socket_pool[socketNum].destination_port = 0;
            return -1;
        }
    }

    return status;
}


/**
 * @brief Socket 번호에 따라 TCP 메시지 전송
 * 
 * @param socketDescription : 전송하고자 하는 Socket 번호 (타입 x)
 * @param data : 데이터
 * @param length : 전송 길이
 * @return int32_t retval - 0: 성공, -1: 실패
 */
int32_t tcpSendData(uint8_t socketDescription, uint8_t* data, uint16_t length)
{
    int8_t  status = -1;
    uint16_t onceLength;
    
    // Check max socket
    if(socketDescription > RSI_NUMBER_OF_SOCKETS)
        return -1;

    // Check TCP, AP Connection 
    if(socketControl.socketState[socketDescription].connect && WifiAPSet.Status)
    {
        // 1회당 전송 길이 재정의
        // 데이터를 1024 byte 씩 나누어서 전송
        for(uint16_t index = 0; index * 1024 < length; index++)
        {
            // 마지막 데이터 영역인지 확인
            if(index * 1024 + 1024 > length)
            {
                onceLength = length - index * 1024;
                rsi_send(socketDescription, (const int8_t *)(data + index * 1024), onceLength, 0);

                if(length >= 1024)
                   HAL_Delay(10);
            }
            else
            {
                onceLength = 1024;
                rsi_send(socketDescription, (const int8_t *)(data + index * 1024), onceLength, 0);

                // 전송 Delay - 전송 데이터가 너무 길어질 경우 휴식 시간이 필요 (FIXME: Graph 전송 - 30 Delay 시 - 최대 100ms 정도 Delay 발생)
                if(length >= 1024)
                   HAL_Delay(30);
            }
        }
    }

    return status;
}


/**
 * @brief UDP 메시지 전송
 * 
 * @param socketDescription : UDP 소켓의 소켓 번호
 * @param data : 전송 데이터
 * @param length : 전송 길이
 * @param ipAddress : 목표 IP address
 * @param port : 목표 Port number
 * @return int32_t (-1: 실패, 그외: 성공 = 전송 개수)
 */
int32_t udpSendData(uint8_t socketDescription, uint8_t* data, uint16_t length, uint8_t* ipAddress, uint16_t port)
{
    struct rsi_sockaddr_in client_addr;

    // Check max socket
    if(socketDescription > RSI_NUMBER_OF_SOCKETS)
        return -1;

    client_addr.sin_family = AF_INET;                                               // family type
    client_addr.sin_addr.s_addr = rsi_bytes4R_to_uint32(ipAddress);                 // IP Address
    client_addr.sin_port = port;                                                    // Port

    if(rsi_socket_pool[socketDescription].sock_state == RSI_SOCKET_STATE_CONNECTED && WifiAPSet.Status)
        rsi_sendto(socketDescription, (int8_t*)data, length, 0, (struct rsi_sockaddr *)&client_addr, sizeof(client_addr));

    return 0;
}


/**
 * @brief TCP 메시지 받기
 * 
 * @param socketDescription : TCP 소켓 번호
 * @param data : 데이터 받을 버퍼
 * @param length : 받을 데이터 길이
 * @return int32_t (-1: 실패, 그외: 성공 = 받은 데이터 길이)
 */
int32_t tcpRecvData(uint8_t socketDescription, uint8_t* data, uint16_t length)
{
    int32_t status = -1;
    uint16_t recvLength = 0;

    // Check max socket
    if(socketDescription > RSI_NUMBER_OF_SOCKETS)
        return - 1;

    // Check AP State
    if(socketControl.socketState[socketDescription].connect == OFF || WifiAPSet.Status == OFF)
        return -1;

    // Check socket alive
    if(rsi_socket_pool[socketDescription].sock_state <= RSI_SOCKET_STATE_CREATE)
        return -1;

    // Set buffer pointer
    if(rsi_socket_pool[socketDescription].recv_buffer == 0)
    	rsi_socket_pool[socketDescription].recv_buffer = (uint8_t *)data;

    // Recv (-1: 실패, 성공시 받은 개수)
    if(rsi_socket_pool_non_rom[socketDescription].recv_pending_bit || rsi_socket_pool_non_rom[socketDescription].socket_wait_bitmap == 0)
    {
    	// Save received length
    	recvLength = rsi_socket_pool[socketDescription].recv_buffer_length;

        status = rsi_recv(socketDescription, data, length, 0);
        if(status > 0 && status < 10000)
        {
            // 받은 데이터 설정 저장
            setSocketDataRecv(socketDescription, data, recvLength);
        }
        else if(status == -1)
            rsi_socket_pool[socketDescription].recv_buffer = (uint8_t *)data;     // 데이터 받을 Buffer 포인터 추가
    }

    return status;
}


/**
 * @brief UDP Message 받기
 * 
 * @param socketDescription : 소켓 번호
 * @param data : 데이터 받을 버퍼 주소
 * @param length : 데이터 길이
 * @return int32_t (-1: 에러, 양수: 받은 데이터 길이)
 */
int32_t udpRecvData(uint8_t socketDescription, uint8_t* data, uint16_t length)
{
    int32_t status = -1;
    uint16_t recvLength = 0;

    // Check max socket
    if(socketDescription > RSI_NUMBER_OF_SOCKETS)
        return - 1;

    // Check AP State
    if(WifiAPSet.Status == OFF)
        return -1;

    // Check socket alive
    if(rsi_socket_pool[socketDescription].sock_state != RSI_SOCKET_STATE_CONNECTED)
        return -1;

    // Set buffer pointer
    if(rsi_socket_pool[socketDescription].recv_buffer == 0)
    	rsi_socket_pool[socketDescription].recv_buffer = (uint8_t *)data;

    // Recv (-1: 실패, 성공시 받은 개수)
    if(rsi_socket_pool_non_rom[socketDescription].recv_pending_bit || rsi_socket_pool_non_rom[socketDescription].socket_wait_bitmap == 0)
    {
        // Save received length
    	recvLength = rsi_socket_pool[socketDescription].recv_buffer_length;

        status = rsi_recv(socketDescription, data, length, 0);
        if(status > 0 && status < 10000)
        {
            // 전송한 주소 저장
            memcpy(socketControl.socketState[socketDescription].netInfo.destIp, rsi_socket_pool[socketDescription].destination_ip_addr.ipv4, 4);    // 메시지 Send한 ip 주소
            socketControl.socketState[socketDescription].netInfo.destPort = rsi_socket_pool[socketDescription].destination_port;                    // 메시지 Send한 port 번호

            // 받은 데이터 설정 저장
            setSocketDataRecv(socketDescription, data, recvLength);
        }
        else if(status == -1)
            rsi_socket_pool[socketDescription].recv_buffer = (uint8_t *)data;
    }

    return status;
}


/**
 * @brief TCP 소켓의 접속 시도 (TCP Client)
 * 
 * @param type : 시도하려는 소켓 번호
 */
void trySocketConnect(uint8_t type)
{
    uint8_t socketNum;

    socketNum = findSocketDescription(type);

    // Check max socket
    if(socketNum > RSI_NUMBER_OF_SOCKETS || socketNum == 0)
        return;

    // 소켓의 타입별로 접속 시도
    switch (type)
    {
        // TCP Client
        case TCP_CLIENT_MODBUS_SOCKET:
            connectTcpClient(socketNum, WifiEthSet.TARGET_IP, WifiEthSet.TARGET_PORT);
        break;
        case TCP_CLIENT_EVENT_SOCKET:
            connectTcpClient(socketNum, WifiEthSet.TARGET_IP, WifiEthSet.TARGET_PORT + 1);
            break;
        
        default:
            break;
    }
}


/**
 * @brief TCP 소켓의 접속 시도 및 접속 허용 (TCP Server, Client)
 * 
 * @param type : 시도하려는 소켓 번호
 */
void trySocketRecv(uint8_t type)
{

}


/*- RSSI -------------------------------------------------------*/
/**
 * @brief Get the Rssi 신호 세기
 * 
 * @return uint8_t (RSSI의 신호 세기)
 */
uint8_t getRssi(void)
{
    int32_t status;
    uint8_t retval;
    uint8_t RSSI_Strength[2] = {0, };

    // Get RSSI (Signal Strength)  Max:-30 ~ Min:-99
    status = rsi_wlan_get(RSI_RSSI, RSSI_Strength, sizeof(rsi_rsp_rssi_t));
    if(status < 0)
    {
        // Read fail
        retval = 0;
    }
    else
    {
        // WIFI 세기
        if(RSSI_Strength[0] <= 97 && RSSI_Strength[0] > 73)
        {
            retval = 1;     // 안테나 개수 (숫자가 클수록 신호세기 강함)
        }
        else if(RSSI_Strength[0] <= 73 && RSSI_Strength[0] > 59)
        {
            retval = 2;
        }
        else if(RSSI_Strength[0] <= 59 && RSSI_Strength[0] > 38)
        {
            retval = 3;
        }
        else if(RSSI_Strength[0] <= 38 && RSSI_Strength[0] > 24)
        {
            retval = 4;
        }
        else if(RSSI_Strength[0] <= 24 && RSSI_Strength[0] >= 1)
        {
            retval = 5;
        }
        else
        {
            retval = 0; 
        }
    }

    return retval;
}



/*- AP State -------------------------------------------------------*/
void getApState(void)
{
    static uint32_t timeout;
    int32_t status;

    // AP 연결 상태 확인 - 1000ms 주기
    if(HAL_GetTick() - timeout > 1000)
    {
        timeout = HAL_GetTick();

        status = rsi_wlan_get(RSI_CONNECTION_STATUS, 0, 0);
        if(status != 0)
            WifiAPSet.Status = OFF;     // AP 연결 에러
        else
            WifiAPSet.Status = ON;
    }
    
    // AP 연결이 완전히 끊어진 것 WIFI 모듈에서 감지
    if(rsi_driver_cb->wlan_cb->state <= RSI_WLAN_STATE_BAND_DONE)
    {
        // WIFI 모듈 재 초기화
        WifiCtrl  = WIFI_INIT;
        WifiState = WIFI_NOTUSE;
    }
}

