/* Includes ------------------------------------------------------------------*/
#include "httpServer.h"
#include <stdlib.h>
#include "backup.h"
#include "main.h"
#include "variable.h"
#include "wifiTask.h"
#include "rsi_nwk.h"
#include "realtimeMonitoring.h"
#include "graphMonitoring.h"


/* Global variables -----------------------------------------------------------*/
httpServer_webContent web_content[MAX_CONTENT_CALLBACK];		// Webpage content
httpMonitoringTypedef httpMonitoring;							// HTTP Monitoring, Login check

uint8_t httpLoginCheck = 0;				// HTTP LOGIN access check
uint8_t httpState = httpRecvState;		// HTTP Process State

uint8_t httpUriBuf[128];
uint8_t httpRecvBuf[512];


/* Extern variables ---------------------------------------------------------*/
extern  WifiMsgState      WifiMsg;
extern  NetworkTypeDef    WifiEthSet;

/* Private Functions ---------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static uint16_t total_content_cnt = 0;

/* Functions ------------------------------------------------------------------*/

/********************************************************************************************************************
 * HTTP WEB SERVER Process
 * 
 * Sequence: * Recv data LOOP   - Check Read, Write, Monitor, WebPage, Login, Parameter Upload - Send (Move send loop)
 * 			 * Send LOOP	    - Send data (several times)
 * 			 * Para Upload Loop - Recv data (several times) - Check recv para size - Check Min, Max - Save parameter
 * ******************************************************************************************************************/
void httpServerProcess(void)
{
    char 	 uri[MAX_URI_SIZE] = "", tempBuf[16] = "";
    uint16_t contentNum, IdPos;
    int8_t   status = 0;
    uint32_t addrStart = 0;
    uint32_t recvDataBuf[16] ={0, };
	uint8_t graphSendType = OFF;
	floatFrom16 convertVal;
	floatFrom16 convertMax;
	floatFrom16 convertMin;


    static uint8_t *webPage;
    static char     jsonBuf[2048]  = "";
	static float   	GraphBuf[1024]  = {0, };
	static uint16_t UploadBuf[PARA_SIZE] = {0,};
    static uint32_t currentLen = 0, firstSend = 0, fileLen = 0, completeLen = 0;

	static uint32_t delayCount;			// Message send delay count
	static uint16_t certifiTotalLength;
	static uint16_t certifiLength;

    switch(httpState)
    {
        case httpRecvState:
			// Get uri address
    		memcpy(uri, (char *)(httpUriBuf +1), 40);

            // transmit parameter data	(Sequence: Page number -> Find start address -> check preset, min, max -> Send)
			if(strncmp(uri, "data", 4) == 0)
			{
				// 파라미터 요청 메시지 처리
				paraHttpRequest((uint8_t*)uri, jsonBuf);

                webPage = (uint8_t *)jsonBuf;
				fileLen = strlen(jsonBuf);
			}
			// Received parameter value (ex: "send_ STARTADDRESS, DATA.JSON")
			else if (strncmp((char *)uri, "send_", 5) == 0)
			{
				// 파라미터 저장 메시지 처리
				paraHttpSave((char*)uri, jsonBuf);
				
				webPage = (uint8_t *)jsonBuf;
				fileLen = strlen(jsonBuf) * sizeof(char);
			}
			/**	  Remote process   **/
			else if(strncmp((char*)uri, "remote_", sizeof("remote_") -1) == 0)
			{
				/**  REMOTE process  **/
				remoteProcess(uri, jsonBuf);
			
				webPage = (uint8_t *)jsonBuf;
                fileLen = strlen(jsonBuf) * sizeof(char);
			}
			/**	  Real Time Process   **/
			else if(strncmp((char*)uri, "realtime_", sizeof("realtime_") -1) == 0)
			{
				// Check webpage load complete
				if(strncmp((char*)(uri + 9), "0", 1) == 0)
				{
					sprintf(jsonBuf, "OK,");		// 0K (대문자)

					// Real time HTTP Capture Flag Init
					httpMonitoring.realtimeCapture = 0;
				}
				else if(strncmp((char*)(uri + 9), "1", 1) == 0)
				{
					// Real time HTTP Capture Flag Start
					httpMonitoring.realtimeStart = ON;

					realTimeProcess((uint8_t*)jsonBuf, &fileLen);
				}
				else if(strncmp((char*)(uri + 9), "request", 7) == 0)
				{
					sprintf(jsonBuf, "%s,%d.%02d.%d,%02d%02d%02d%02d%02d", Info_DrvModel[Para[DRIVER_MODEL].val].name, \
															   controllerPara.firmwareVersion[0], controllerPara.firmwareVersion[1], controllerPara.firmwareVersion[2], \
															   dset[5], dset[6], PRODUCT_CODE, dset[7], dset[8]);		// Model name, Firmware, Serial
				}
				// Error request
				else
				{
					sprintf(jsonBuf, "FAIL,");
				}
				
				webPage = (uint8_t *)jsonBuf;
				fileLen = sizeof(char) * strlen(jsonBuf);
			}
			// Graph process
			else if(strncmp((char*)uri, "graph_", sizeof("graph_") -1) == 0)	
			{
				// Received STOP Flag data
				if(strncmp((char *)(uri + 6), "stop", 4) == 0)
				{
					httpMonitoring.graphStart = FALSE;
					httpMonitoring.graphCapture = FALSE;
					httpMonitoring.realtimeStart = FALSE;
					httpMonitoring.realtimeCapture = FALSE;

					sprintf(jsonBuf, "STOP");
				}
				// Save parameter data
				else if(strncmp((char *)(uri + 6), "set", 3) == 0)
				{
					// Find ID position
					IdPos = strcspn((char *)uri, ",") + 1;
					// String to integer
					recvDataBuf[0] = atoi(uri + IdPos);

					IdPos += strcspn((char *)(uri + IdPos), ",") + 1;
					recvDataBuf[1] = atoi(uri + IdPos);

					IdPos += strcspn((char *)(uri + IdPos), ",") + 1;
					recvDataBuf[2] = atoi(uri + IdPos);

					IdPos += strcspn((char *)(uri + IdPos), ",") + 1;
					recvDataBuf[3] = atoi(uri + IdPos);

					// Data check
					uint8_t channel1 = (uint8_t)recvDataBuf[0];
					uint8_t channel2 = (uint8_t)recvDataBuf[1];
					uint8_t SamplingRate = (uint8_t)recvDataBuf[2];
					uint8_t Option       = (uint8_t)recvDataBuf[3];

					if((channel1 >= 1 && channel1 <= 8) && (channel2 >= 0 && channel2 <= 8) && (SamplingRate >= 2 && SamplingRate <= 60) && (Option >= 1 && Option <= 3))
					{
						TRIG.Channel[0]   = (int)channel1;
						TRIG.Channel[1]   = (int)channel2;
						TRIG.SamplingRate = (int)SamplingRate;
						TRIG.Option       = (int)Option;
						httpMonitoring.graphStart = TRUE;
						httpMonitoring.graphCapture = FALSE;
						httpMonitoring.realtimeStart = TRUE;

						sprintf(jsonBuf, "Save parameter");		
					}
					else
						sprintf(jsonBuf, "Error");	
				}
				else if(strncmp((char *)(uri + 6), "start", 5) == 0)
				{
					httpMonitoring.graphStart = TRUE;
					httpMonitoring.realtimeStart = TRUE;
					
					if(httpMonitoring.graphCapture)
						sprintf(jsonBuf, "ok");
					else
						sprintf(jsonBuf, "no");	
				}
				else if(strncmp((char *)(uri + 6), "realtime", 8) == 0)
				{
					graphRealtimeProcess(GraphBuf, jsonBuf);
				}
				else if(strncmp((char *)(uri + 6), "ch1", 3) == 0)
				{
					graphCh1Process(GraphBuf, jsonBuf);
					graphSendType = ON;
				}
				else if(strncmp((char *)(uri + 6), "ch2", 3) == 0)
				{
					graphCh2Process(GraphBuf, jsonBuf);

					graphSendType = ON;
					httpMonitoring.graphCapture = 0;
				}
				else
					sprintf(jsonBuf, "Error");	

				GraphTimeout = 0;	// Graph timeout reset

				// Check graph data ready
				if(graphSendType)
                {
                    webPage = (uint8_t *)GraphBuf;
                    fileLen = TRIG.DataCount * sizeof(float) + sizeof(float);
                }
				else
				{
					webPage = (uint8_t *)jsonBuf;
					fileLen = strlen(jsonBuf) * sizeof(char);
				}
			}
            // Find web page
            else if(find_userReg_webContent((uint8_t *)uri, (uint16_t *)&contentNum, (uint32_t *)&fileLen))
            {
				// Login not yet -> Call login.html (contentNum 2이상: webPage)
				if(!httpLoginCheck && contentNum >= 2)
				{
					strcpy(uri, INITIAL_WEBPAGE);
					find_userReg_webContent((uint8_t *)uri, (uint16_t *)&contentNum, (uint32_t *)&fileLen);
				}

                webPage = web_content[contentNum].content;

				// Init Flag (Start로 Monitoring 하다가 Stop 안했을 때 init)
				httpMonitoring.realtimeStart = OFF;
				httpMonitoring.realtimeCapture = OFF;
				httpMonitoring.graphStart = OFF;
				httpMonitoring.graphCapture = OFF;
            }
            /**  Login process  **/
            else if(strncmp((char *)uri, "login.text", 10) == 0)
            {
				sprintf(jsonBuf, "bmtoolPS%d",Para[PASSWORD].val);

                // Login 정보랑 비교
                if(strcmp((char *)(uri + 10), jsonBuf) == 0)
				{
					sprintf(jsonBuf, "ok,");
					httpLoginCheck = 1;				// Login ON
				}
                else
				{
					// 대문자 비교
					sprintf(jsonBuf, "BMTOOLPS%d",Para[PASSWORD].val);

					if(strcmp((char *)(uri + 10), jsonBuf) == 0)
					{
						sprintf(jsonBuf, "ok,");
						httpLoginCheck = 1;				// Login ON
					}
					else
						sprintf(jsonBuf, "false,");
				}

                webPage = (uint8_t *)jsonBuf;
                fileLen = strlen(jsonBuf) * sizeof(char);
            }
			else if(strncmp(uri, "system_", 7) == 0)
			{
				// WIFI Firmware version,  Parameter size
				snprintf(tempBuf, 7, "%s", WifiEthSet.FirmVersion);
				strcat(jsonBuf, tempBuf);
				sprintf(tempBuf, ",%d", PARA_SIZE);
				strcat(jsonBuf, tempBuf);

                webPage = (uint8_t *)jsonBuf;
				fileLen = strlen(jsonBuf);
			}
			else if(strncmp((char *)uri, "cert_", sizeof("cert_") -1) == 0)
			{
				// 1. EAP 인증서 다운로드
				// 2. 다운로드 완료 후 rsi_set_certificate() 실행

				// 인증서 버퍼 초기화
				if(strncmp((char *)uri, "cert_0", sizeof("cert_0") -1) == 0)
				{
					memset(eapCertification, 0, sizeof(eapCertification));
					certifiTotalLength = 0;
				}

				// 인증서 저장
				if(strncmp((char *)uri, "cert_1", sizeof("cert_1") -1) == 0)
				{
					certifiLength = atoi((char *)(uri + 7));

					// Buffer limit
					if(certifiTotalLength < 8000)
					{
						memcpy(&eapCertification[certifiTotalLength], httpRecvBuf, certifiLength);
						certifiTotalLength += certifiLength;
					}
				}

				// 인증서 저장 완료
				if(strncmp((char *)uri, "cert_2", sizeof("cert_2") -1) == 0)
				{
					status = rsi_wlan_set_certificate(RSI_EAP_CLIENT, eapCertification, certifiTotalLength);
					printf("Save cert: %d, %d\n", status, certifiTotalLength);

					HAL_Delay(1000);
					eapCertification[0] = 0;
				}

				// 인증서 저장 완료
				if(strncmp((char *)uri, "cert_4", sizeof("cert_4") -1) == 0)
				{
					eapCertification[0] = 0;
            		rsi_wlan_set_certificate(RSI_EAP_CLIENT, eapCertification, 0);

					HAL_Delay(1000);
					eapCertification[0] = 0;
				}

				// Send response
				sprintf(jsonBuf, "ok");
				webPage = (uint8_t *)jsonBuf;
				fileLen = strlen(jsonBuf) * sizeof(char);
			}
			// check Parameter Upload DATA
			else if(strncmp((char *)uri, "parameter", 9) == 0)
			{
				httpState  = httpUploadState;
				contentNum = 0;
				fileLen	   = atoi((char *)(uri + 9));		// file length

				// First send data
				if(fileLen == 0)
				{
					currentLen = 0;
					fileLen = 100;
				}						

				for(uint16_t i=0; i < fileLen; i++)
				{
					// ',' 까지 숫자 구함
					UploadBuf[contentNum + currentLen] = atoi((const char *)(httpRecvBuf + addrStart));
					addrStart += strcspn((char *)(httpRecvBuf + addrStart), ",") + 1;	// ',' 까지의 Offset

					contentNum++;
				}

				fileLen = contentNum;		// Parameter size

				return;
			}
            // Bad request (Not Found)
            else
            {
                webPage = (uint8_t *)ERROR_HTML_PAGE;
                fileLen = sizeof(ERROR_HTML_PAGE) * sizeof(char);
            }

            // Next state
			httpState = httpSendState;
			firstSend = 0;
        break;

        case httpSendState:

			// Delay check
			// if(HAL_GetTick() - delayCount < 30)
			if(HAL_GetTick() - delayCount < 5)
				return;

			// Send webserver data
			status = rsi_webpage_send(0, webPage + completeLen, fileLen - completeLen);

            if(status == 0)
            {
            	// 완료
            	httpState = httpRecvState;
            }
			else if(status == -3)
			{
				// 아직 전송할 데이터 남음
				completeLen += RSI_MAX_HOST_WEBPAGE_SEND_SIZE;
				delayCount = HAL_GetTick();
			}
			else
			{
				// Error
            	httpState = httpRecvState;
			}

			if(httpState == httpRecvState)
			{
				// Init Data buffer
				if(jsonBuf[0] != 0)
					memset(jsonBuf, 0, sizeof(jsonBuf));
				if(GraphBuf[0] != 0 || GraphBuf[1] != 0)
					memset(GraphBuf, 0, sizeof(GraphBuf));
				if(UploadBuf[0] != 0 || UploadBuf[1] != 0)
					memset(UploadBuf, 0, sizeof(UploadBuf));
					
				// Init
				currentLen = 0;
				fileLen    = 0;
				completeLen = 0;

				// Flag init
				WifiMsg.MsgHttp = 0;
				WifiMsg.MsgWebpageRSP = 0;
			}
			
        break;

		case httpUploadState:
			/** 
			 * - Sequence 
			 * 	  1.Receive all data (Wait more parameter -> httpMaintainState, Finish -> httpSendState)
			 * 	  2.Check Min, Max
			 * 	  3.Save parameter
			 * */

			currentLen += fileLen;

			// 현재 받은 data 길이 check
			if(currentLen < PARAMETER_TOTAL_SIZE)		// 725 size
			{
				// Send receive Ok
				sprintf(jsonBuf, "ok");

				// Change state, Maintain buffer
				httpState = httpMaintainState;
			}
			// File receive Success				
			else if(currentLen == PARAMETER_TOTAL_SIZE)
			{
				// min max check
				for(uint16_t i=0; i < currentLen; i++)		// fileLen: parameter size
				{
					if ((UploadBuf[i] >= Para[i+1].min) == 0 || (UploadBuf[i] <= Para[i+1].max) == 0)
					{
						// Send Fail
						if(Para[i + 1].type == VAL_FLOAT_H)
						{
							// MIN, MAX 판단하는 것에 문제 있음 : 소수점 단위에 OVER가 발생할 수 있음
							if(UploadBuf[i] < Para[i + 1].min || UploadBuf[i] > Para[i + 1].max)
							{
								sprintf(jsonBuf, "Fail");
							}
						}
						else if(Para[i + 1].type == VAL_FLOAT_L)
						{
							// 전체 값을 보고 비교한다
							convertVal.raw.integer_high = Para[i + 1 - 1].val;
							convertVal.raw.integer_low = UploadBuf[i];

							convertMax.raw.integer_high = Para[i + 1 - 1].max;
							convertMax.raw.integer_low = Para[i + 1].max;

							convertMin.raw.integer_high = Para[i + 1 - 1].min;
							convertMin.raw.integer_low = Para[i + 1].min;

							if(convertVal.fVal < convertMin.fVal || convertVal.fVal > convertMax.fVal)
							{
								sprintf(jsonBuf, "Fail");
							}
						}
					}
					else
					{
						if(UploadBuf[i] < Para[i + 1].min || UploadBuf[i] > Para[i + 1].max)
						{
							sprintf(jsonBuf, "Fail");
						}
					}
				}


				// Fail check
				if(strncmp(jsonBuf, "Fail", 4) != 0)
				{
					// Save Data to memory
					for(uint16_t i=0; i < currentLen; i++)				// currentLen = 725
					{
						Para[i+1].val = UploadBuf[i];
						ParameterWriteToMemory(i + 1, UploadBuf[i]);	// Addresss start: 1
					}

					// Save dset[9] (TORQUE_COMPENSATION_MAIN)
					dset[9] = (uint8_t)Para[TORQUE_COMPENSATION_MAIN].val;
					dParameterValue[9] = (uint8_t)Para[TORQUE_COMPENSATION_MAIN].val;
					DriverSaveParaNumber 	= 9;
					Flag.DriverSaveParaData = ON;

					Set_Torque_variable();	// Torque, Angle 값 사항 반영

					sprintf(jsonBuf, "OK");
					currentLen = 0;
				}

				// Change state, Init buffer
				httpState = httpSendState;
			}
			// Received Data error
			else
			{
				sprintf(jsonBuf, "Fail");
				httpState = httpSendState;
			}


			// Check Motor running
			if(Flag.Run != 0)
			{
				sprintf(jsonBuf, "Motor Run Error");
				httpState = httpSendState;
			}

			// HTTP OK message
			webPage = (uint8_t *)jsonBuf;
			fileLen = strlen(jsonBuf);

		break;
			
		// Maintain에서는 Data 초기화를 안해서 UploadBuf 유지
        case httpMaintainState:

			rsi_webpage_send(0, webPage, fileLen);
			httpState = httpRecvState;  

            // Init Data buffer
            if(jsonBuf[0] != 0)
                memset(jsonBuf, 0, fileLen);

			// Init
            fileLen = 0;
            WifiMsg.MsgHttp = 0;

            httpState = httpRecvState;
        break;

        default:
            httpState = httpRecvState;
        break;
    }
}




void graphRealtimeProcess(float * GraphBuf, char * jsonBuf)
{
	char makeChar[64] = {0, };
	pollingRealtimeDataStruct* realtime;

	jsonBuf[0] = 'o';		// 0k (대문자, 소문자)
	jsonBuf[1] = 'k';
	jsonBuf[2] = ',';

	floatFrom16 convert;
	realtime = (pollingRealtimeDataStruct*)httpMonitoring.realtimeData;
	
	sprintf(makeChar, "%d,", realtime->eventId);
	strcat((char*)jsonBuf, makeChar);
	
	sprintf(makeChar, "%d,", realtime->revision);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", (realtime->time[0] << 16) + realtime->time[1]);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->fasteningTime);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->preset);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->unit);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->screwCount);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->fl);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->errorCode);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->status);
	strcat((char*)jsonBuf, makeChar);

	convert.raw.integer_high = realtime->targetTQ[0];
	convert.raw.integer_low = realtime->targetTQ[1];
	sprintf(makeChar, "%.3f,", convert.fVal);
	strcat((char*)jsonBuf, makeChar);

	convert.raw.integer_high = realtime->convetTQ[0];
	convert.raw.integer_low = realtime->convetTQ[1];
	sprintf(makeChar, "%.3f,", convert.fVal);
	strcat((char*)jsonBuf, makeChar);

	convert.raw.integer_high = realtime->seatingTQ[0];
	convert.raw.integer_low = realtime->seatingTQ[1];
	sprintf(makeChar, "%.3f,", convert.fVal);
	strcat((char*)jsonBuf, makeChar);

	convert.raw.integer_high = realtime->clampTQ[0];
	convert.raw.integer_low = realtime->clampTQ[1];
	sprintf(makeChar, "%.3f,", convert.fVal);
	strcat((char*)jsonBuf, makeChar);

	convert.raw.integer_high = realtime->prevailingTQ[0];
	convert.raw.integer_low = realtime->prevailingTQ[1];
	sprintf(makeChar, "%.3f,", convert.fVal);
	strcat((char*)jsonBuf, makeChar);

	convert.raw.integer_high = realtime->snugTQ[0];
	convert.raw.integer_low = realtime->snugTQ[1];
	sprintf(makeChar, "%.3f,", convert.fVal);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->speed);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->angle1);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->angle2);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->angle3);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "%d,", realtime->snugAngle);
	strcat((char*)jsonBuf, makeChar);

	sprintf(makeChar, "0,");
	strcat((char*)jsonBuf, makeChar);						// Type of channel 1

	sprintf(makeChar, "0,");
	strcat((char*)jsonBuf, makeChar);						// 2

	sprintf(makeChar, "0,");
	strcat((char*)jsonBuf, makeChar);						// Count of channel 1

	sprintf(makeChar, "0,");
	strcat((char*)jsonBuf, makeChar);						// 2

	sprintf(makeChar, "0,");
	strcat((char*)jsonBuf, makeChar);						// Sampling rate

	sprintf(makeChar, "0,");
	strcat((char*)jsonBuf, makeChar);						// Graph step

	// Barcode
	for(uint8_t i=0; i < REALTIME_BARCODE_SIZE; i++)
	{
		sprintf(makeChar, "%c", (char)(realtime->barcode[i] >> 8));
		strcat((char*)jsonBuf, makeChar);
		sprintf(makeChar, "%c", (char)(realtime->barcode[i] & 0xFF));
		strcat((char*)jsonBuf, makeChar);
	}

	// END
	sprintf(makeChar, ",");
	strcat((char*)jsonBuf, makeChar);

	httpMonitoring.realtimeCapture = OFF;
}


void graphCh1Process(float * GraphBuf, char * jsonBuf)
{
	// Graph가 준비됨
	if(httpMonitoring.graphCapture)
	{
		GraphBuf[0] = (0x6F << 8) + 0x6B;		// ok

		// Channel 1 활성화 되었을 때
		if(TRIG.Channel[0])
		{
			GraphBuf[1] = TRIG.DataCount;		// Total data size

			memcpy(&GraphBuf[2], gMonitoringData[0], TRIG.DataCount * sizeof(float));
		}
		else
			GraphBuf[1] = 0;					// Total data size
	}
	else
	{	// Graph 준비 안됨
		sprintf((char*)GraphBuf,"Not Ready");
	}
}


void graphCh2Process(float * GraphBuf, char * jsonBuf)
{
	// Graph가 준비됨
	if(httpMonitoring.graphCapture)
	{
		GraphBuf[0] = (0x6F << 8) + 0x6B;		// ok

		// Channel 2 활성화 되었을 때
		if(TRIG.Channel[1])
		{
			GraphBuf[1] = TRIG.DataCount;		// Total data size

			memcpy(&GraphBuf[2], gMonitoringData[1], TRIG.DataCount * sizeof(float));
		}
		else
			GraphBuf[1] = 0;					// Total data size
	}
	else
	{	// Graph 준비 안됨
		sprintf((char*)GraphBuf,"Not Ready");
	}
}


void remoteProcess(char* messageBuf, char* jsonBuf)
{
    uint8_t  IdPos;
    uint32_t recvDataBuf[16] = {0, };
    uint32_t addrStart = 0;
	uint8_t ready = 0;
	char tempBuf[150] = "";

	// Find ID position
    IdPos = strcspn(messageBuf, ",") + 1;
    // String to integer
    recvDataBuf[0] = atoi(messageBuf + IdPos);
    addrStart      = atoi(messageBuf + 7);		

	// Save data
	switch(addrStart)
	{
		// Tool Run
		case 5000:
			if(recvDataBuf[0])
			{
				if(Flag.Lock == OFF)
				{
					if (Flag.Multi_Mode && (Flag.ControlFL == FASTENING_CONTROL)) 	// mult mode는 fastening에서만 동작..
						Flag.Multi_Start = ON;
					else
						Flag.InternalRun = ON;
				}
			}
			else
			{
				if(Flag.InternalRun || Flag.ExternalRun || Flag.Multi_Start)
				{
					Flag.Stopping = ON;
					Flag.Multi_Start = OFF;
				}
			}
		break;

		// F/L Control
		case 5001:
			if(recvDataBuf[0])
				Flag.ControlFL = LOOSENING_CONTROL;
			else
				Flag.ControlFL = FASTENING_CONTROL;
		break;

		// Lock
		case 5002:
			if(recvDataBuf[0] == 0)
			{
				Flag.LockCommand = 0;
				Flag.Lock = OFF;
			}
			else if(recvDataBuf[0] >= 1 && recvDataBuf[0] <= 3)
				Flag.LockCommand = recvDataBuf[0];
		break;

		// Preset select
		case 5003:
			if(!Flag.Run && (recvDataBuf[0] > 0) && (recvDataBuf[0] <= PRESET_INDEX_SIZE + 2))
			{
				Processing_number = recvDataBuf[0];
				Set_Torque_variable();
			}
			else	// Fail
				ready = 0xFF;
		break;

		// Reset
		case 5005:
			if(system_err)
				Flag.Reset = ON;
		break;

		// Parameter initialize
		case 5006:
			SystemResetValue = RESET_ALL_PARA;
			Flag.ParaInitialize = ON;
		break;
	}

	// Send Remote data
	if(addrStart == 100)
	{
		// START
		if(recvDataBuf[0] != 0)
		{
			if (!Flag.Run && !Flag.TorqueUpCompleteOut && !system_err)		// check ready
				ready = 1;	// ON
			else
				ready = 0;	// OFF

			jsonBuf[0] = 0x6F;		// O
			jsonBuf[1] = 0x6B;		// K
			jsonBuf[2] = 0x2C;		// ,
			sprintf(tempBuf, "%.3f,%d,%.2f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.1f,", RealTq_fromSensor_reflectedSettingUnit * getCrowFootRatio() * screw_type,	// Torque (1)
									(uint16_t)(w_rpm_display / deceleration_ratio),		// Speed (rpm)
									i_real * screw_type,		// Current
									Processing_number,				// Preset
									Flag.TorqueUpCompleteOut,		// Torque up
									Flag.FasteningCompleteOut,		// Fastening complete
									ready,							// Tool ready (7)
									Flag.Run,						// Tool run
									system_err,						// Error
									Flag.ControlFL,					// F/L
									ScrewCountValue,				// Remain screw count
									0,								// Input IO
									0,								// Output IO
									Temperature_real_Flt_motor		// Temperature (14)
									);
			strcat(jsonBuf, tempBuf);
		}
		else
		{
			// First start when remote page open -> send lock, preset data
			sprintf(tempBuf, "%d,%d,", Flag.LockCommand, Processing_number);
			strcat(jsonBuf, tempBuf);
		}
	}
	else if(ready == 0xFF)			// Save Fail
		sprintf(jsonBuf, "%s", "FAIL");
	else							// Success
		sprintf(jsonBuf, "%s", "ok");

}

void realTimeProcess(uint8_t* jsonBuf, uint32_t* fileLen)
{
	char makeChar[64] = {0, };
	pollingRealtimeDataStruct* realtime;

	if(httpMonitoring.realtimeCapture)
	{
		jsonBuf[0] = 'O';		// 0k (대문자, 소문자)
		jsonBuf[1] = 'k';
		jsonBuf[2] = ',';

    	floatFrom16 convert;
		realtime = (pollingRealtimeDataStruct*)mbMData;
		
		sprintf(makeChar, "%d,", realtime->eventId);
		strcat((char*)jsonBuf, makeChar);
		
		sprintf(makeChar, "%d,", realtime->revision);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", (realtime->time[0] << 16) + realtime->time[1]);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->fasteningTime);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->preset);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->unit);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->screwCount);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->fl);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->errorCode);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->status);
		strcat((char*)jsonBuf, makeChar);

		convert.raw.integer_high = realtime->targetTQ[0];
		convert.raw.integer_low = realtime->targetTQ[1];
		sprintf(makeChar, "%.3f,", convert.fVal);
		strcat((char*)jsonBuf, makeChar);

		convert.raw.integer_high = realtime->convetTQ[0];
		convert.raw.integer_low = realtime->convetTQ[1];
		sprintf(makeChar, "%.3f,", convert.fVal);
		strcat((char*)jsonBuf, makeChar);

		convert.raw.integer_high = realtime->seatingTQ[0];
		convert.raw.integer_low = realtime->seatingTQ[1];
		sprintf(makeChar, "%.3f,", convert.fVal);
		strcat((char*)jsonBuf, makeChar);

		convert.raw.integer_high = realtime->clampTQ[0];
		convert.raw.integer_low = realtime->clampTQ[1];
		sprintf(makeChar, "%.3f,", convert.fVal);
		strcat((char*)jsonBuf, makeChar);

		convert.raw.integer_high = realtime->prevailingTQ[0];
		convert.raw.integer_low = realtime->prevailingTQ[1];
		sprintf(makeChar, "%.3f,", convert.fVal);
		strcat((char*)jsonBuf, makeChar);

		convert.raw.integer_high = realtime->snugTQ[0];
		convert.raw.integer_low = realtime->snugTQ[1];
		sprintf(makeChar, "%.3f,", convert.fVal);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->speed);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->angle1);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->angle2);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->angle3);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "%d,", realtime->snugAngle);
		strcat((char*)jsonBuf, makeChar);

		sprintf(makeChar, "0,");
		strcat((char*)jsonBuf, makeChar);						// Type of channel 1

		sprintf(makeChar, "0,");
		strcat((char*)jsonBuf, makeChar);						// 2

		sprintf(makeChar, "0,");
		strcat((char*)jsonBuf, makeChar);						// Count of channel 1

		sprintf(makeChar, "0,");
		strcat((char*)jsonBuf, makeChar);						// 2

		sprintf(makeChar, "0,");
		strcat((char*)jsonBuf, makeChar);						// Sampling rate

		sprintf(makeChar, "0,");
		strcat((char*)jsonBuf, makeChar);						// Graph step

		// Barcode
		for(uint8_t i=0; i < REALTIME_BARCODE_SIZE; i++)
		{
			sprintf(makeChar, "%c", (char)(realtime->barcode[i] >> 8));
			strcat((char*)jsonBuf, makeChar);
			sprintf(makeChar, "%c", (char)(realtime->barcode[i] & 0xFF));
			strcat((char*)jsonBuf, makeChar);
		}

		// END
		sprintf(makeChar, ",");
		strcat((char*)jsonBuf, makeChar);
		

		httpMonitoring.realtimeCapture = 0;
	}
	else
	{
		// Real time HTTP Capture Flag (background에서 실행)
		httpMonitoring.realtimeStart = ON;

		sprintf((char*)jsonBuf, "No");
		memset(fileLen, 7, 1);
	}
}

/**
  * @brief  Upload webpage.
  * @param  content_name: 받은 메세지에서 판단할 웹페이지 이름
  * @param  content: defined name
  * @retval None
  */
void reg_httpServer_webContent(uint8_t * content_name, uint8_t * content)
{
	uint16_t name_len;
	uint32_t content_len;

	if(content_name == NULL || content == NULL)
		return;
	else if(total_content_cnt >= MAX_CONTENT_CALLBACK)
		return;

	name_len = strlen((char *)content_name);
	content_len = strlen((char *)content);

	web_content[total_content_cnt].content_name = malloc(name_len + 1);
	strcpy((char *)web_content[total_content_cnt].content_name, (const char *)content_name);
	web_content[total_content_cnt].content_len = content_len;
	web_content[total_content_cnt].content = content;

	total_content_cnt++;
}

/**
  * @brief  Find webpage content.
  * @param  content_name: 찾는 웹페이지 이름
  * @param  content_num: 웹페이지 넘버
  * @param  file_len: file length
  * @retval Error check
  */
uint8_t find_userReg_webContent(uint8_t * content_name, uint16_t * content_num, uint32_t * file_len)
{
	uint16_t i;
	uint8_t ret = 0; // '0' means 'File Not Found'

	for(i = 0; i < total_content_cnt; i++)
	{
		if(!strcmp((char *)content_name, (char *)web_content[i].content_name))
		{
			*file_len = web_content[i].content_len;
			*content_num = i;
			ret = 1; // If the requested content found, ret set to '1' (Found)
			break;
		}
	}
	return ret;
}



void httpWebpageLoad(void)
{
    reg_httpServer_webContent((uint8_t *)"Chartmin.js", 	   (uint8_t *)chartmin_js);
    reg_httpServer_webContent((uint8_t *)"index.html", 		   (uint8_t *)login_page);
    reg_httpServer_webContent((uint8_t *)"login.html", 		   (uint8_t *)login_page);
    reg_httpServer_webContent((uint8_t *)"fastening.html", 	   (uint8_t *)fastening_page);
    reg_httpServer_webContent((uint8_t *)"advanced.html", 	   (uint8_t *)advanced_page);
    reg_httpServer_webContent((uint8_t *)"controller.html",    (uint8_t *)controller_page);
    reg_httpServer_webContent((uint8_t *)"Network.html",  	   (uint8_t *)network_page);
    reg_httpServer_webContent((uint8_t *)"multisequence.html", (uint8_t *)multisequence_page);
    reg_httpServer_webContent((uint8_t *)"graph.html", 		   (uint8_t *)graph_page);
    reg_httpServer_webContent((uint8_t *)"realtime.html", 	   (uint8_t *)realtime_page);
	reg_httpServer_webContent((uint8_t *)"system.html", 	   (uint8_t *)system_page);
	reg_httpServer_webContent((uint8_t *)"remote.html", 	   (uint8_t *)remote_page);
}


/**
 * @brief Parameter 관련 요청이 있을 경우 응답 메시지를 만든다.
 * @param messageBuf : 요청 메시지 포인터
 * @param sendPointer: 전송하는 메시지 포인터
 * 
 * @note  Message 예시: data_val_123_10.json
 * */
void paraHttpRequest(uint8_t* messageBuf, char* sendPointer)
{
    uint32_t addrStart = 0, jsonSize;
    uint8_t  dataType = 0, IdPos;
    char     tempBuf[16] = "";
    

    // Get request type
    if(strncmp((char *)(messageBuf + 5), "val", 3) == 0)
        dataType = HTTP_VAL;
    else if(strncmp((char *)(messageBuf + 5), "min", 3) == 0)
        dataType = HTTP_MIN;
    else if(strncmp((char *)(messageBuf + 5), "max", 3) == 0)
        dataType = HTTP_MAX;
    else if(strncmp((char *)(messageBuf + 5), "ori", 3) == 0)
        dataType = HTTP_NUM;
    
    // 구분자 위치
    IdPos = strcspn((char*)messageBuf + 9, "_") + 1;

    // 주소, 길이
    addrStart = atoi((char *)(messageBuf + 9));                 // 시작 주소
    jsonSize = atoi((char *)(messageBuf + 9 + IdPos));          // 요청 개수

    // Limit check
    if(addrStart + jsonSize <= PARA_SIZE + 1 && jsonSize < 250 && dataType != 0)
    {
        // Check Min, Max, val
        if(dataType == HTTP_VAL)
        {
            // Value data
            for(uint8_t i=0; i < jsonSize; i++)
            {
				if(Para[addrStart + i].type == VAL_FLOAT_H)
					sprintf(tempBuf, "%.3f,", getParaVal(addrStart + i));
				else if(Para[addrStart + i].type == VAL_UINT32_H)
					sprintf(tempBuf, "%lu,",  (((uint32_t)getParaValSingle(addrStart + i)) << 16) + getParaValSingle(addrStart + i + 1));
                else
					sprintf(tempBuf, "%.0f,", getParaVal(addrStart + i));
				
				strcat(sendPointer, tempBuf);

				if(Para[addrStart + i].type == VAL_FLOAT_H || Para[addrStart + i].type == VAL_UINT32_H || Para[addrStart + i].type == VAL_INT_32_H)
				{
					sprintf(tempBuf, "%d,", 0);
                	strcat(sendPointer, tempBuf);
					
					i++;
				}
            }
        }
        // Min
        else if(dataType == HTTP_MIN)
        {
            // Get parameter data
            for(uint8_t i=0; i < jsonSize; i++)
            {
				if(Para[addrStart + i].type == VAL_FLOAT_H)
					sprintf(tempBuf, "%.3f,", getParaMin(addrStart + i));
				else if(Para[addrStart + i].type == VAL_UINT32_H)
					sprintf(tempBuf, "%lu,",  (((uint32_t)getParaMinSingle(addrStart + i)) << 16) + getParaMinSingle(addrStart + i + 1));
                else
					sprintf(tempBuf, "%.0f,", getParaMin(addrStart + i));
					
                strcat(sendPointer, tempBuf);

				if(Para[addrStart + i].type == VAL_FLOAT_H || Para[addrStart + i].type == VAL_UINT32_H || Para[addrStart + i].type == VAL_INT_32_H)
				{
					sprintf(tempBuf, "%d,", 0);
                	strcat(sendPointer, tempBuf);

					i++;
				}
            }
        }
        // Max
        else if(dataType == HTTP_MAX)
        {
            for(uint8_t i=0; i < jsonSize; i++)
            {
				if(Para[addrStart + i].type == VAL_FLOAT_H)
					sprintf(tempBuf, "%.3f,", getParaMax(addrStart + i));
				else if(Para[addrStart + i].type == VAL_UINT32_H)
					sprintf(tempBuf, "%lu,",  (((uint32_t)getParaMaxSingle(addrStart + i)) << 16) + getParaMaxSingle(addrStart + i + 1));
                else
					sprintf(tempBuf, "%.0f,", getParaMax(addrStart + i));

                strcat(sendPointer, tempBuf);

				if(Para[addrStart + i].type == VAL_FLOAT_H || Para[addrStart + i].type == VAL_UINT32_H || Para[addrStart + i].type == VAL_INT_32_H)
				{
					sprintf(tempBuf, "%d,", 0);
                	strcat(sendPointer, tempBuf);
					
					i++;
				}
            }
        }
        // Parameter origin
        else if(dataType == HTTP_NUM)
        {
			for(uint8_t i=0; i < jsonSize; i++)
            {
				sprintf(tempBuf, "%d,", getParaValSingle(addrStart + i));
            	strcat(sendPointer, tempBuf);
			}
        }
    }
    else
    {

    }       
}


/**
 * @brief Parameter 쓰기 요청이 왔을 때 처리
 * @param messageBuf : 받은 메시지 포인터
 * @param sendPointer: 응답하는 메시지 포인터 
 * 
 * */
void paraHttpSave(char* messageBuf, char* sendPointer)
{
    uint8_t  IdPos;
    float recvDataBuf[16] ={0, };
	uint64_t val64;
	uint64_t min64;
	uint64_t max64;
    uint32_t addrStart = 0;


    // Find ADDRESS data position
    IdPos = strcspn(messageBuf, ",") + 1;		

    // String to integer
    recvDataBuf[0] = atof(messageBuf + IdPos);
    addrStart      = atoi(messageBuf + 5);

    
	// Check address
	if(addrStart > BACKUP_SIZE + 1)
	{
		sprintf(sendPointer, "Fail");
	}
	else
	{
		// Check type
		if(Para[addrStart].type == VAL_UINT32_H || Para[addrStart].type == VAL_INT_32_H)
		{
			val64 = atoll(messageBuf + IdPos);
			min64 = ((((uint32_t)getParaMinSingle(addrStart)) << 16) + getParaMinSingle(addrStart + 1));
			max64 = ((((uint32_t)getParaMaxSingle(addrStart)) << 16) + getParaMaxSingle(addrStart + 1));

			if(val64 >= min64 && val64 <= max64)
			{
				mbSavePara((uint16_t)(val64 >> 16), addrStart);
				mbSavePara((uint16_t)(val64 >> 0), addrStart + 1);

				// Send Ok
				sprintf(sendPointer, "ok");
			}
			else
			{
				// Send Fail
				sprintf(sendPointer, "Fail");
			}
		}
		else
		{
			// min max check
			if ((recvDataBuf[0] >= getParaMin(addrStart)) && (recvDataBuf[0] <= getParaMax(addrStart))) 		
			{
				if(Para[addrStart].type == VAL_FLOAT_H)
				{
					floatFrom16 convert;
					convert.fVal = recvDataBuf[0];
					mbSavePara((uint16_t)convert.raw.integer_high, addrStart);
					mbSavePara((uint16_t)convert.raw.integer_low, addrStart + 1);
				}
				else
				{
					mbSavePara((uint16_t)recvDataBuf[0], addrStart);
				}

				// Send Ok
				sprintf(sendPointer, "ok");
			}
			else
			{
				// Send Fail
				sprintf(sendPointer, "Fail");
			}
		}
	}
}
