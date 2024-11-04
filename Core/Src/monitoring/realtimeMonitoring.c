/**
  ******************************************************************************
  * @file    realtimeMonitoring.c
  * @author  jun
  * @brief   Source file of Realtime moinitoring.
  * @note    일반 Realtime 모니터링 구현
  ******************************************************************************
  */


/* Include ---------------------------------------------------------------------*/
#include "realtimeMonitoring.h"
#include "graphMonitoring.h"
#include "ringBuffer.h"
#include "httpServer.h"
#include "variable.h"
#include "modbus.h"
#include "virtualPreset.h"
#include "barcode.h"
#include "advancedPlus.h"
#include "eventRealtimeTask.h"
#include "eventGraphTask.h"
#include "wifiTask.h"
#include "timer.h"
#include "backup.h"
#include "RTC.h"
 

/* Variable ---------------------------------------------------------------------*/
pollingRealtimeStruct realPollStruct;



/* Extern -----------------------------------------------------------------------*/




/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief Queue에서 Realtime 데이터를 가져오고 데이터를 전송한다.
 */
void realtimeMonitoringProcess(void)
{
    uint16_t sendLength;
	// pollingRealtimeDataStruct* realtimeResult;

    // Check realtime monitoring

    // Realtime 전송할 것이 존재하는지 확인 및 활성화
    TRIG_REAL.Capture = getRealtimeFlag(&realPollStruct);


    // modbus용 make data (Real time)
    if (TRIG_REAL.Capture)
    {
		// Get realtime data
        getRealtimeData(&realPollStruct, mbMData);


        // 전송 data 개수 구하기
        sendLength = sizeof(pollingRealtimeDataStruct) / sizeof(uint16_t);

		// RS232 통신
        if (TRIG_REAL.MonitoringRS232)
		{
        	// Trigger 모니터링
            mbMakeandSend_04(GRAPH_REALTIME_START_ADDRESS, sendLength, 1);
		}
		else if(Para[AUTO_UPDATE_EVENT].val)
		{
			#if REALTIME_BATTERY_TOOL_ACTIVE == OFF
			// Auto data output
			if (Para[AUTO_UPDATE_PORT].val == 0) 
                mbMakeandSend_04(GRAPH_REALTIME_START_ADDRESS, sendLength, 1);
			#endif
		}


		// Ethernet 통신
        if (TRIG_REAL.MointoringEthernet)
            socketMonitoringSend(sendLength, MONITORING_REALTIME);
        else if(Para[AUTO_UPDATE_EVENT].val)
		{
			#if REALTIME_BATTERY_TOOL_ACTIVE == OFF

			// Auto data output
			if (Para[AUTO_UPDATE_PORT].val == 1)
            	socketMonitoringSend(sendLength, MONITORING_REALTIME);

			#else
            	socketMonitoringSend(sendLength, MONITORING_REALTIME);
			#endif
		}

		// HTTP Server
//		realtimeResult = (pollingRealtimeDataStruct*)mbMData;
//        if(httpMonitoring.realtimeStart && httpMonitoring.realtimeCapture == OFF)
//        {
//			// Graph monitoring 비교
//			if(httpMonitoring.graphStart && realtimeResult->fasteningTime == 0)
//			{
//				// Graph - realtime은 체결 시간이 있을 때만 표시
//            	httpMonitoring.realtimeCapture = OFF;
//			}
//			else
//			{
//            	httpMonitoring.realtimeCapture = ON;
//				memcpy(httpMonitoring.realtimeData, mbMData, sizeof(httpMonitoring.realtimeData));
//			}
//		}


        // Graph event monitoring에 필요한 realTime
        fasteningResultOn();

		// Graph monitoring에서 Realtime 데이터 복사
		getGraphMonitoringRealtime();


        TRIG_REAL.Capture = OFF;
    }
}


/**
 * @brief Realtime 데이터를 생성
 * 
 * @param buf : 생성된 데이터를 저장할 버퍼
 */
void mbGetMonitoringData(pollingRealtimeDataStruct* buf)
{
	static uint16_t cnt;
    floatFrom16 convert;
	uint8_t directionFL;
	uint16_t data;

	// If Error is not Fastening data then Clear (300 ~ 356이외)
	if(judgeRealtimeInitFastenFromError(system_err) == ON)
		ClearSendData_Init(ON);
	
	// Barcode -> Clear fastening data
	if(StatusOutput == rBarcode)
		ClearSendData_Init(ON);


	// Buffer init
	memset(buf, 0, sizeof(pollingRealtimeDataStruct));

	// Check direction (Multi sequence에 따라 현재 FL말고 실제로 실행되서 측정된 방향을 가지고 정해야할 경우 발생)
	if(Fastening_time)
		directionFL = FASTENING_CONTROL;
	else if(Disp_Loosening_time)
		directionFL = LOOSENING_CONTROL;
	else
		directionFL = Flag.ControlFL;


	// Make data
    buf->revision = REALTIME_MONITORING_REVISION;										// Revision (0)
    buf->eventId = ++cnt;																// Event id (1)
    

    #if RTC_ACTIVE
    rtcTime rtc;
    getRtc(&rtc);

    buf->date[0] = rtc.year;
    buf->date[1] = (rtc.month << 8) + rtc.date;
    buf->time[0] = (rtc.hour << 8) + rtc.minute;
    buf->time[1] = (rtc.second << 8);
    #endif


    if (directionFL == LOOSENING_CONTROL)
		data = Disp_Loosening_time;	            										// Loosening_time (6)
	else
		data = Fastening_time;		            										// Fastening_time

    buf->fasteningTime = data;


	if(virtualPrsetState())
		buf->preset = VIRTUAL_PRESET_DISPLAY_NUM;										// Preset (7)
	else if(StatusOutput == rPRESET_CHANGE)
		buf->preset = Processing_number;												
	else if(Flag.Multi_Mode)
		buf->preset = Processing_number;												
	else
    	buf->preset = Fastening_number;													


    buf->unit = Para[UNIT].val;															// Unit (8)
	buf->screwCount = ScrewCountValue;	                								// Screw (9)

    if (directionFL == FASTENING_CONTROL)												// fasten-loosen (10)
		buf->fl = 0;
	else
		buf->fl = 1;

	buf->errorCode = system_err;		                								// Error (11)
	buf->status = StatusOutput;															// Status (12)

    convert.fVal = advancedTargetTorque(Fastening_number);
	buf->targetTQ[0] = convert.raw.integer_high;										// Target torque (13)
	buf->targetTQ[1] = convert.raw.integer_low;								

    convert.fVal = (Torque_cnv.fastenComplete * getCrowFootRatio() * inv_Torque_ConvertValue);
	buf->convetTQ[0] = convert.raw.integer_high;		        						// C torque (15)
	buf->convetTQ[1] = convert.raw.integer_low;		                        

    convert.fVal = (Torque_cnv.seating * getCrowFootRatio() * inv_Torque_ConvertValue);
	buf->seatingTQ[0] = convert.raw.integer_high;										// Seating torque
	buf->seatingTQ[1] = convert.raw.integer_low;

    convert.fVal = (Torque_cnv.clamp * getCrowFootRatio() * inv_Torque_ConvertValue);
	buf->clampTQ[0] = convert.raw.integer_high;											// Clamp torque
	buf->clampTQ[1] = convert.raw.integer_low;	

    convert.fVal = (Torque_cnv.prevailing * getCrowFootRatio() * inv_Torque_ConvertValue);
	buf->prevailingTQ[0] = convert.raw.integer_high;									// Prevailing torque
	buf->prevailingTQ[1] = convert.raw.integer_low;	

    convert.fVal = (Torque_cnv.snug * getCrowFootRatio() * inv_Torque_ConvertValue);
	buf->snugTQ[0] = convert.raw.integer_high;											// Snug torque
	buf->snugTQ[1] = convert.raw.integer_low;	


	if (directionFL == FASTENING_CONTROL)
		buf->speed = Para[FASTEN_SPEED1 + ((Fastening_number-1)*PRESET_SIZE)].val;		// speed (25)
	else if(directionFL == LOOSENING_CONTROL && ((Para[CROWFOOT_ENABLE].val) && (Para[CROWFOOT_REVERSE_TORQUE_H].val) && Para[CROWFOOT_REVERSE_SPEED].val))
		buf->speed = Para[CROWFOOT_REVERSE_SPEED].val;		                            // speed - Crowfoot reverse
	else
		buf->speed = Para[LOOSENING_SPEED].val;				                            // Loosening speed

	buf->angle1 = (uint16_t)((float)(mPulse.After_Fastening_Start_ForA1) * MathConv.AngleConv_mPulseToDeg);	// Angle A1 (26)

	if (directionFL == FASTENING_CONTROL)
	{
		if (mPulse.After_Fastening_Start_ForA1 != 0 || mPulse.After_Fastening_Start_ForA2 != 0)
		{
			buf->angle2 = (uint16_t)((float)(mPulse.After_Fastening_Start_ForA2) * MathConv.AngleConv_mPulseToDeg);
			buf->angle3 = buf->angle1 + buf->angle2;
		}
		else
		{
			buf->angle2 = 0;
			buf->angle3 = 0;
		}
	}
	else
	{
		buf->angle1 = (uint16_t)((float)(mPulse.DisplayLoosening) * MathConv.AngleConv_mPulseToDeg);
		buf->angle2 = 0;
		buf->angle3 = buf->angle1;
	}

    // Snug angle (29)
	if (FastenSeq.SnugTQ_Nm != 0)
		buf->snugAngle = (uint16_t)((float)(mPulse.RealSnugTq) * MathConv.AngleConv_mPulseToDeg);
	else
		buf->snugAngle = 0;

	// Reserve (30)
	memset(buf->reserve, 0, sizeof(buf->reserve));

	// Barcode (46)
	for(uint8_t i=0; i < REALTIME_BARCODE_SIZE; i++)
		buf->barcode[i] = (barcode.data[i * 2] << 8) + barcode.data[i * 2 + 1];


	// 그래프 활성화 되어 있을 경우
	if(buf->fasteningTime)
	{
		// Step info
		getGraphResultSetting((uint8_t*)&buf->graph);

		// Multi sequence 중 Realtime 반영
		if(Flag.RunningMultisequence)
			memcpy(graphMultiSequence.realtime, buf, sizeof(graphMultiSequence.realtime));
	}
}


/**
 * @brief 에러 발생했을 때 Fastening 관련된 정보를 초기화할 것인지 결정
 * 
 * @param errorCode : 에러코드
 * @return uint8_t : 0: 미초기화, 1: 초기화
 */
uint8_t judgeRealtimeInitFastenFromError(uint8_t errorCode)
{
	uint8_t retval = 0;

	// Fastening과 관련되지 않은 에러 판단
	if(errorCode == 0)
		retval = OFF;
	else if(errorCode == OVER_SPEED || errorCode == NO_SPEED_ERROR || errorCode == NO_RAMPUP_TORQUE || errorCode == OVER_CURRENT)
		retval = OFF;
	else if(system_err < FASTENING_TIMEOUT || system_err > ADVACNED_PARA_ERROR)
		retval = ON;

	return retval;
}





/* Realtime monitoring------------------------------------------------------------------------------------------------*/

/**
 * @brief Realtime monitoring 시 데이터를 생성 및 버퍼에 저장 (최대 10개)
 * 
 * @param realtime : Realtime struct
 */
void putRealtimeData(pollingRealtimeStruct* realtime)
{
	uint8_t type;

	// Check limit
	if(realtime->saveIndex > realtime->sendIndex && realtime->saveIndex - realtime->sendIndex >= POLLING_REALTIME_MAX_INDEX - 1)
		return;
	else if(realtime->saveIndex < realtime->sendIndex && POLLING_REALTIME_MAX_INDEX - realtime->sendIndex + realtime->saveIndex >= POLLING_REALTIME_MAX_INDEX - 1)
		return;

	// VP 모드 Preset 변경 확인
	if(StatusOutput == rPRESET_CHANGE)
		vpPresetChangeActive();

	// Check 저장할 event type
	type = selectEventType();

	if(type)
	{
		// Make realtime
		mbGetMonitoringData((pollingRealtimeDataStruct *)realtime->data[realtime->saveIndex]);

		// Index
		realtime->saveIndex++;
		if(realtime->saveIndex >= POLLING_REALTIME_MAX_INDEX)
			realtime->saveIndex = 0;
	}
}


/**
 * @brief Get the Realtime Data object
 * 
 * @param realtime : Realtime struct
 * @param buf : 데이터를 받을 버퍼 포인터
 */
void getRealtimeData(pollingRealtimeStruct* realtime, uint16_t* buf)
{
	// Check limit
	if(realtime->saveIndex == realtime->sendIndex)
		return;

	// Copy
	memcpy(buf, realtime->data[realtime->sendIndex], sizeof(realtime->data[0]));

	// Index
	realtime->sendIndex++;
	if(realtime->sendIndex >= POLLING_REALTIME_MAX_INDEX)
		realtime->sendIndex = 0;
}


/**
 * @brief Set the Realtime Flag object (TRIG_REAL.Capture 활성화)
 * 
 * @param realtime :Realtime struct
 * @return uint8_t (0: OFF, 1: ON)
 */
uint8_t getRealtimeFlag(pollingRealtimeStruct* realtime)
{
	static uint32_t timeout;
	uint8_t retval = 0;

	// Check timeout (연속적으로 Realtime이 발생되었을 때 최소 100ms 간격으로 전송될 수 있도록 timeout 설정)
	if(HAL_GetTick() - timeout > REALTIME_UPDATE_MINIUM_TIMEOUT && HAL_GetTick() > REALTIME_UPDATE_MINIUM_TIMEOUT)
	{
		// Check remain index
		if(realtime->saveIndex != realtime->sendIndex)
		{
			retval = ON;
			timeout = HAL_GetTick();
		}
	}
	
	return retval; 
}





