/**
  ******************************************************************************
  * @file    graphMonitoring.c
  * @author  jun
  * @brief   Source file of graph moinitoring.
  * @note    일반 graph 모니터링 구현
  ******************************************************************************
  */


/* Include ---------------------------------------------------------------------*/
#include "graphMonitoring.h"
#include "realtimeMonitoring.h"
#include "ringBuffer.h"
#include "main.h"
#include "variable.h"
#include "modbus.h"
#include "velocity.h"
#include "advancedPlus.h"
#include "wifiTask.h"
#include "eventGraphTask.h"
 

/* Variable ---------------------------------------------------------------------*/
graphMonitoringReusltStruct graphMonitoringResult;			// 현재 Graph가 발생했을 때에 정보

graphQueueStruct graphQueue;								// Graph 데이터를 순서대로 Queue에 쌓기 위한 buffer
graphMonitoringReusltStruct graphMonLastData;				// 외부 표시를 위한 Graph - realtime
graphMultiStruct graphMultiSequence;						// Multi sequence 중 graph - realtime




/* Private Variable -----------------------------------------------------------------------*/
static graphSetResultStruct graphStepCurrent;
static uint8_t graphStepQueue[GRAPH_STEP_INDEX_SIZE];
static uint8_t graphStepUsed[GRAPH_STEP_INDEX_SIZE];



/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief 그래프 모니터링 - 데이터 전송 
 */
void graphMonitoringProcess(void)
{
	static uint32_t timeout;

	// Graph trigger timeout 500ms
	if(TRIG.Capture)
	{
		if(timeout == 0)
			timeout = HAL_GetTick();

		if(HAL_GetTick() - timeout > 500 && HAL_GetTick() > 500)
		{
			TRIG.Capture = OFF;
		}
	}
	else
		timeout = 0;


	// Graph의 결과 버퍼 관리
	graphExternalSave();


	// Make graph result data
	makeGraphResultData();

	// Send graph result data
	sendGraphResultData();
}


/**
 * @brief Graph data를 만들고 Queue에 쌓는다
 */
void makeGraphResultData(void)
{
	if (TRIG.Capture && graphMonitoringResult.realtimeOn)
    {
		// Endian 정렬
		floatFrom16 convert;
		graphMonitoringReusltStruct graphData;

		for(uint16_t i = 0; i < GRAPH_DATA_SIZE; i++)
		{
			convert.fVal = gMonitoringData[0][i];
			graphData.data[0][i * 2] = convert.raw.integer_high;
			graphData.data[0][i * 2 + 1] = convert.raw.integer_low;

			convert.fVal = gMonitoringData[1][i];
			graphData.data[1][i * 2] = convert.raw.integer_high;
			graphData.data[1][i * 2 + 1] = convert.raw.integer_low;
		}


		// Check graph index length
		memcpy(graphData.realtimeData, mbMData, sizeof(graphData.realtimeData));

		// Multi sequence에 해당하는 graph일 경우 마지막 체결 결과를 반영
		if(FlagFasten.CompleteMultisequence)
		{
			memcpy(graphData.realtimeData, graphMultiSequence.realtime, sizeof(graphData.realtimeData));
			FlagFasten.CompleteMultisequence = OFF;
		}

		// Add graph data to queue
		addGraphQueue(&graphData);


		// Graph monitoring flag off
        TRIG.Capture = OFF;
		graphMonitoringResult.realtimeOn = OFF;
	}
}


/**
 * @brief Queue에 쌓았던 Graph data를 전송한다
 */
void sendGraphResultData(void)
{
	static uint32_t timeout;
	static uint16_t length;

	// static uint8_t  timeoutSize = 100;
  static uint32_t  timeoutSize = 100;
	static uint8_t graphSendState = 0;
	static uint32_t timeoutStep;

	// 200ms 주기로 Grpah 데이터 전송
	if(HAL_GetTick() - timeout > 200 && HAL_GetTick() > 100)
	{
		switch(graphSendState)
		{
			case 0:
				// Queue에 남은 데이터 유무 확인
				if(graphQueue.startIndex != graphQueue.endIndex)
				{
					// Get graph data from Queue
					deGraphQueue(&graphMonLastData);

					// Send graph data length
					if(graphMonLastData.setting.channel1Count)
						length = graphMonLastData.setting.channel1Count * 2 + GRAPH_TRIGGER_HEADER_ADDR_SIZE;
					else if(graphMonLastData.setting.channel2Count)
						length = graphMonLastData.setting.channel2Count * 2 + GRAPH_TRIGGER_HEADER_ADDR_SIZE;

					// Wait 시간 결정
					if(length / 2 > 500)
						timeoutSize = 450;
					else if(length / 2 > 350)
						timeoutSize = 350;
					else if(length / 2 > 200)
						timeoutSize = 300;
					else
						timeoutSize = 200;


					// HTTP Server - graph monitoring 완료
					if(httpMonitoring.graphStart)
						httpMonitoring.graphCapture = TRUE;      

					graphSendState++;
				}
			break;

			case 1:
				// CHANNEL 1
				if(HAL_GetTick() - timeoutStep > timeoutSize)
				{
					// RS232 - 그래프 데이터 모니터링
					if (TRIG.MonitoringRS232)
					{
						// 채널 별 활성화 여부에 따라 전송
						if(TRIG.Channel[0])
						{
							mbMakeandSend_64(GRAPH_DATA_CAHNNEL1_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE, length, MODBUS_RS232_TYPE);
						}
					}


					// Ethernet - 그래프 모니터링
					if (TRIG.MointoringEthernet)
					{
						socketMonitoringSend(length, MONITORING_GRAPH_CH1);
					}

					graphSendState++;
					timeoutStep = HAL_GetTick();
				}
			break;

			case 2:
				// CHANNEL 2
				if(HAL_GetTick() - timeoutStep > timeoutSize)
				{
					// RS232 - 그래프 데이터 모니터링
					if (TRIG.MonitoringRS232)
					{
						// 채널 별 활성화 여부에 따라 전송
						if(TRIG.Channel[1])
						{
							mbMakeandSend_64(GRAPH_DATA_CAHNNEL2_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE, length, MODBUS_RS232_TYPE);
						}	
					}

					// Ethernet - 그래프 모니터링
					if (TRIG.MointoringEthernet)
					{
						socketMonitoringSend(length, MONITORING_GRAPH_CH2);
					}

					graphSendState++;
					timeoutStep = HAL_GetTick();
				}
			break;
			
			case 3:
				// REALTIME
				if(HAL_GetTick() - timeoutStep > timeoutSize)
				{
					// RS232 - 그래프 데이터 모니터링
					if (TRIG.MonitoringRS232)
					{
						// Realtime data
						mbMakeandSend_65(GRAPH_REALTIME_START_ADDRESS, sizeof(pollingRealtimeDataStruct) / sizeof(uint16_t), MODBUS_RS232_TYPE);
					}

					// Ethernet - 그래프 모니터링
					if (TRIG.MointoringEthernet)
					{
						socketMonitoringSend(length, MONITORING_GRAPH_REALTIME);
					}

					graphSendState = 0;
					timeoutStep = HAL_GetTick();

					timeout = HAL_GetTick();
				}
			break;
		}
	}
}




void Collect_GraphData(void)
{
    uint16_t   i=0;
    float      SampingPulseValue;
    float      SampingPulseValueAdd;
    static int Cnt_monitoringDataForTqAngle;
    static int run_edge[2] = {0,0};
    static uint8_t targetTorqueOn;
    static uint32_t runTime;
	static uint16_t samplingRate;
    static uint16_t SamplingAngle;

    // Graph 모니터링을 OFF 했다 나중에 다시 시작했을 때 값 초기화
    if(HAL_GetTick() - runTime > 100 && HAL_GetTick() > 100)
        run_edge[1] = OFF;

    runTime = HAL_GetTick();


    // Start edge
    run_edge[0] = run_edge[1];
    run_edge[1] = Flag.Run | Flag.RunningMultisequence;		// Multi sequence + Preset (Multi 사용 시 완전히 종료된 후 값을 얻는다)
    

    if (run_edge[0] < run_edge[1])
    {
		// START
        TRIG.DataCount = 0;
        Cnt_monitoringDataForTqAngle = 1;
        TRIG.Flag_MonitoringData_Overflow = OFF;
		graphMonitoringResult.overflow = OFF;
        targetTorqueOn = 0;

		samplingRate = TRIG.SamplingRate * GRAPH_EVENT_SAMPLING_1MS;
        SamplingAngle = TRIG.SamplingRate;

		// Multi sequence 실행 시 - Flag.RunningMultisequence의 활성화가 Fastening 보다 빠르기 때문에 관련 변수 초기화
		if(Flag.RunningMultisequence)
		{
			mPulse.After_Fastening_Start_abs = 0;
			mPulse.ContinuSnugAngle = 0;
			FlagFasten.FindedTargetTorqueStep = OFF;
		}
    }
    else if (run_edge[0] > run_edge[1])
    {
		// END
        if (TRIG.Option == 1 && Flag.ControlFL == FASTENING_CONTROL) 	
        {
			// fastening
			TRIG.Capture = ON;
        }
        else if (TRIG.Option == 2 && Flag.ControlFL == LOOSENING_CONTROL)
        {
			// Loosening
			TRIG.Capture = ON;
        }
        else
        {
			// Both
            TRIG.Capture = ON;
		}


		// Graph step
		if(TRIG.Flag_MonitoringData_Overflow)
		{
			// step change
			saveGraphStepChange(&graphStepCurrent, GRAPH_DATA_SIZE, graphStepQueue);

			// Step increase
			increaseGraphStepIndex(&graphStepCurrent);
		}
		else
			saveGraphStepChange(&graphStepCurrent, TRIG.DataCount, graphStepQueue);

		

		// 최종 토크 표시
		if(targetTorqueOn == ON)
		{
			for (i=0; i < 2; i++) 
			{
				gMonitoringData[i][TRIG.DataCount] = SelectDisplayData(TRIG.Channel[i]);

				// 토크 모니터링 중일 때 최종 토크 표시
				if(TRIG.Channel[i] == 1)
				{
					gMonitoringData[i][TRIG.DataCount] = SelectDisplayData(20);
					targetTorqueOn = 2;
				}
			}

			TRIG.DataCount++;

			if (TRIG.DataCount >= GRAPH_DATA_SIZE)
			{
				TRIG.DataCount = 0;
				TRIG.Flag_MonitoringData_Overflow = ON;
			}
		}


		

		// Data 없을 시 Dummy
		if(TRIG.DataCount == 0)
		{
			TRIG.DataCount = 1;

			gMonitoringData[0][0] = 0;
			gMonitoringData[1][0] = 0;
		}			

		// Copy graph setting data
		getGraphResultSetting((uint8_t*)&graphMonitoringResult.setting.channel1Type);


		// Graph 모니터링 완료 알림
		TRIG.graphStop = ON;
    }


    // 동작 중 Graph data 저장
    if (run_edge[1])
    {
        if (TRIG.Channel[0] == 8)       // torque/angle monitoring
        {
            // Clear channel 2
            TRIG.Channel[1] = 0;


            SampingPulseValue = MathConv.SamplingPulsConv * (float)SamplingAngle;       // samping pulse count
            SampingPulseValueAdd = SampingPulseValue * (float)Cnt_monitoringDataForTqAngle;
            if (SampingPulseValueAdd <= mPulse.After_Fastening_Start_abs)
            {
				// Graph step
				if(TRIG.Flag_MonitoringData_Overflow)
				{
					// step change
					saveGraphStepChange(&graphStepCurrent, GRAPH_DATA_SIZE, graphStepQueue);

					// Step increase
					increaseGraphStepIndex(&graphStepCurrent);
				}
				else
					saveGraphStepChange(&graphStepCurrent, TRIG.DataCount, graphStepQueue);


                gMonitoringData[0][TRIG.DataCount] = SelectDisplayData(1);
                Cnt_monitoringDataForTqAngle++;

                TRIG.DataCount++;
            }
        }
        else
        {
            // 최종 토크 표시
            if (FlagFasten.FindedTargetTorqueStep == ON && targetTorqueOn == OFF)
                targetTorqueOn = ON;

            if(!(cnt_250us % samplingRate) && (TRIG.DataCount < GRAPH_DATA_SIZE))
            {
				// Graph step
				if(TRIG.Flag_MonitoringData_Overflow)
				{
					// step change
					saveGraphStepChange(&graphStepCurrent, GRAPH_DATA_SIZE, graphStepQueue);

					// Step increase
					increaseGraphStepIndex(&graphStepCurrent);
				}
				else
					saveGraphStepChange(&graphStepCurrent, TRIG.DataCount, graphStepQueue);


                for (i=0; i < 2; i++) 
                {
                    gMonitoringData[i][TRIG.DataCount] = SelectDisplayData(TRIG.Channel[i]);
                } 

                // 토크 모니터링 중일 때 최종 토크 표시
                for (i=0; i < 2; i++) 
                {
                    if(targetTorqueOn == ON)
                    {
						if(TRIG.Channel[i] == 1)
                        	gMonitoringData[i][TRIG.DataCount] = SelectDisplayData(20);
						else
		                    gMonitoringData[i][TRIG.DataCount] = SelectDisplayData(TRIG.Channel[i]);

                        targetTorqueOn = 2;
                    }

                    // 최종 Gradient 값 표시 (Gradient 그래프에 마지막 값 표시)
                    if(AtSeating.finalGradient != 0 && TRIG.Channel[i] == 8)
                    {
                        gMonitoringData[i][TRIG.DataCount] = SelectDisplayData(21);
                        AtSeating.finalGradient = 0;
                    }
                }

                TRIG.DataCount++;
            }
        }

        if (TRIG.DataCount >= GRAPH_DATA_SIZE)
        {
            TRIG.DataCount = 0;
            TRIG.Flag_MonitoringData_Overflow = ON;
			graphMonitoringResult.overflow = ON;
        }   
    }
}


/**
 * @brief Get the Graph Monitoring Realtime
 */
void getGraphMonitoringRealtime(void)
{
	pollingRealtimeDataStruct* realtimePointer;

	realtimePointer = (pollingRealtimeDataStruct*)mbMData;

	// 체결이 있을 경우
	if(realtimePointer->fasteningTime)
	{
		graphMonitoringResult.realtimeOn = ON;

		// Data copy
		// memcpy(graphMonitoringResult.realtimeData, mbMData, sizeof(graphMonitoringResult.realtimeData));
	}
}



/* Step control ----------------------------------------------------------------------*/

/**
 * @brief Graph - Graph의 step 정보를 Queue에 넣는다
 * 
 * @param step : step 정보
 */
void setGraphStepQueue(uint8_t step)
{
	// Check 이미 Step에 대한 정보가 설정 되어 있는지
	for(uint8_t i=0; i < sizeof(graphStepQueue); i++)
	{
		if(graphStepQueue[i] == step)
			return;

		if(graphStepUsed[i] == step)
			return;
	}

	// 저장이 되지 않은 step 순서대로 저장
	for(uint8_t i=0; i < sizeof(graphStepQueue); i++)
	{
		if(graphStepQueue[i] == 0)
		{
			graphStepQueue[i] = step;
			graphStepUsed[i] = step;
			break;
		}
	}

	// Event graph step
	// Check 이미 Step에 대한 정보가 설정 되어 있는지
	for(uint8_t i=0; i < sizeof(graphEventMonStepQueue); i++)
	{
		if(graphEventMonStepQueue[i] == step)
			return;

		if(graphEventMonStepUsed[i] == step)
			return;
	}

	// 저장이 되지 않은 step 순서대로 저장
	for(uint8_t i=0; i < sizeof(graphEventMonStepQueue); i++)
	{
		if(graphEventMonStepQueue[i] == 0)
		{
			graphEventMonStepQueue[i] = step;
			graphEventMonStepUsed[i] = step;
			break;
		}
	}
}


/**
 * @brief Graph - Graph의 step 정보를 Queue에서 가져온다
 * 
 * @return uint8_t : step 정보
 */
uint8_t getGraphStepQueue(uint8_t* queue)
{
	uint8_t retval = 0;

	for(uint8_t i=0; i < GRAPH_STEP_INDEX_SIZE; i++)
	{
		if(queue[i] != 0)
		{
			retval = queue[i];
			queue[i] = 0;
			break;
		}
	}

	return retval;
}


/**
 * @brief Graph step을 저장하는 Queue를 초기화 한다
 */
void clearGraphStepQueue(void)
{
	memset(graphStepQueue, 0, sizeof(graphStepQueue));
	memset(graphStepUsed, 0, sizeof(graphStepUsed));
	memset(graphStepCurrent.stepInfo, 0, sizeof(graphStepCurrent.stepInfo));

	memset(graphEventMonStepQueue, 0, sizeof(graphEventMonStepQueue));
	memset(graphEventMonStepUsed, 0, sizeof(graphEventMonStepUsed));
	memset(graphEventStepInfo.stepInfo, 0, sizeof(graphEventStepInfo.stepInfo));
}


/**
 * @brief Graph - Graph의 step 정보를 저장한다
 * 
 * @param graph : Graph step struct
 * @param index : Graph의 현대 index
 */
void saveGraphStepChange(graphSetResultStruct* graph, uint16_t index, uint8_t* stepQueue)
{
	uint8_t step;

	for(uint8_t stepIndex = 0; stepIndex < GRAPH_STEP_INDEX_SIZE; stepIndex++)
	{
		// Queue에 저장이 되어있던 step 정보를 가져온다
		step = getGraphStepQueue(stepQueue);

		// 남아있는 STEP이 없을 때
		if(step == 0)
			break;

		// STEP 정보가 있을 때
		if(step)
		{
			// 순서대로 Step 저장
			for(uint8_t i=0; i < GRAPH_STEP_INDEX_SIZE; i++)
			{
				// Check 동일한 step
				if(graph->stepInfo[i].stepId == step)
					break;

				// Check step setted
				if(graph->stepInfo[i].stepId == 0)
				{
					graph->stepInfo[i].stepId = step;
					graph->stepInfo[i].index = index;
					break;
				}
			}
		}
	}
}


/**
 * @brief Graph - Graph의 data index가 overflow 되었을 때 Step 정보에 있던 index를 증가시켜준다
 * 
 * @param graph : step 정보 struct
 */
void increaseGraphStepIndex(graphSetResultStruct* graph)
{
	// Index increase
	for(uint8_t i=0; i < GRAPH_STEP_INDEX_SIZE; i++)
	{
		// Check step setted
		if(graph->stepInfo[i].stepId != 0)
		{
			if(graph->stepInfo[i].index != 0)
				graph->stepInfo[i].index--;

			if(graph->stepInfo[i].index == 0)
			{
				// Erase step
				graph->stepInfo[i].stepId = 0;
				graph->stepInfo[i].index = 0;
			}
		}
	}

	// Check step 순서 재정렬
	for(uint8_t i=0; i < GRAPH_STEP_INDEX_SIZE - 1; i++)
	{
		if(graph->stepInfo[i].stepId == 0 && graph->stepInfo[i + 1].stepId != 0)
		{
			graph->stepInfo[i].stepId = graph->stepInfo[i + 1].stepId;
			graph->stepInfo[i].index = graph->stepInfo[i + 1].index;
		}
	}
}





/* Graph data ----------------------------------------------------------------------*/

/**
 * @brief Get the Graph Result Setting
 * 
 * @param buf : 그래프 설정 값을 저장할 buf pointer
 */
void getGraphResultSetting(uint8_t* buf)
{
	graphSetResultStruct  info;
	uint16_t channel1 = 0, channel2 = 0;
	

	if(TRIG.Channel[0] != 0)
	{
		if(graphMonitoringResult.overflow)
			channel1 = GRAPH_DATA_SIZE;
		else
			channel1 = TRIG.DataCount;
	}

	if(TRIG.Channel[1] != 0)
	{
		if(graphMonitoringResult.overflow)
			channel2 = GRAPH_DATA_SIZE;
		else
			channel2 = TRIG.DataCount;
	}


	info.channel1Type = TRIG.Channel[0];
	info.channel2Type = TRIG.Channel[1];
	info.channel1Count = channel1;
	info.channel2Count = channel2;
	info.samplingRate = TRIG.SamplingRate;


	// Step information
	memcpy(&info.stepInfo[0], &graphStepCurrent.stepInfo[0], sizeof(graphStepStruct) * GRAPH_STEP_INDEX_SIZE);

	// Copy ALL
	memcpy(buf, &info, sizeof(info));
}




/**
 * @brief Graph의 모니터링이 완료 후 - 그래프 데이터 정렬
 */
void graphExternalSave(void)
{
    static float MonitoringDataTmp[GRAPH_DATA_SIZE];
    uint16_t   offsetLength = 0;

	// Graph의 모니터링이 완료 되었을 때
	if(TRIG.graphStop == ON)
	{
		TRIG.graphStop = OFF;

		// Check overflow
		if (TRIG.Flag_MonitoringData_Overflow)
		{
			offsetLength = GRAPH_DATA_SIZE - TRIG.DataCount;

			// 채널별 Overflow 순서 재정렬
			for(uint8_t i=0; i < 2; i++)
			{
				// 채널 활성화되어 있을 경우
				if(TRIG.Channel[i] != 0)
				{
					memcpy(MonitoringDataTmp, gMonitoringData[i], TRIG.DataCount * sizeof(float));										// 후반부 데이터 임시 저장
					memcpy(gMonitoringData[i], &gMonitoringData[i][TRIG.DataCount], offsetLength * sizeof(float));						// 전반부 데이터 재정렬
					memcpy(&gMonitoringData[i][GRAPH_DATA_SIZE - TRIG.DataCount], MonitoringDataTmp, TRIG.DataCount * sizeof(float));	// 후반부 데이터 재정렬
				}
			}
			
			TRIG.Flag_MonitoringData_Overflow = OFF;
			TRIG.DataCount = GRAPH_DATA_SIZE;
		}
		else 	
		{
			// No overflow 
			// GRAPH_DATA_SIZE 보다 작으면 마지막에 0을 넣는다.
			if (TRIG.DataCount < GRAPH_DATA_SIZE - 1)		
			{
				for(uint8_t i=0; i < 2; i++)
				{
					if (TRIG.Channel[i] == 4)
					{
						// Angle 채널 - 길이 늘리기
						gMonitoringData[i][TRIG.DataCount] = gMonitoringData[i][TRIG.DataCount - 1];
					}
					else
					{
						// 그외 채널 - 0으로 종료
						gMonitoringData[i][TRIG.DataCount] = 0;
					}
				}

				TRIG.DataCount++;				
			}	
		}
	}
}



/* Graph data Queue ----------------------------------------------------------------------*/

/**
 * @brief Graph의 데이터를 Queue에 저장한다
 * 
 * @param dataBuf : 저장할 Graph data
 */
void addGraphQueue(graphMonitoringReusltStruct* dataBuf)
{
	pollingRealtimeDataStruct* realtimePointer;
	realtimePointer = (pollingRealtimeDataStruct*)dataBuf->realtimeData;

	// Check queue limit
	if(graphQueue.endIndex > graphQueue.startIndex && graphQueue.endIndex - graphQueue.startIndex >= GRAPH_QUEUE_INDEX_SIZE - 1)
		return;
	else if(graphQueue.startIndex > graphQueue.endIndex && GRAPH_QUEUE_INDEX_SIZE + graphQueue.endIndex - graphQueue.startIndex >= GRAPH_QUEUE_INDEX_SIZE - 1)
		return;


	// Queue에 데이터 copy

	// INIT
	memset(graphQueue.channel1[graphQueue.endIndex], 0, sizeof(graphQueue.channel1[graphQueue.endIndex]));
	memset(graphQueue.channel2[graphQueue.endIndex], 0, sizeof(graphQueue.channel2[graphQueue.endIndex]));

	// Channel 1
	if(realtimePointer->graph.channel1Count)
		memcpy(graphQueue.channel1[graphQueue.endIndex], dataBuf->data[0], realtimePointer->graph.channel1Count * sizeof(float));

	// Channel 2
	if(realtimePointer->graph.channel2Count)
		memcpy(graphQueue.channel2[graphQueue.endIndex], dataBuf->data[1], realtimePointer->graph.channel2Count * sizeof(float));

	// Realtime data
	memcpy(graphQueue.realtime[graphQueue.endIndex], dataBuf->realtimeData, sizeof(graphQueue.realtime[0]));


	// Increase index
	graphQueue.endIndex++;

	// Check index limit
	if(graphQueue.endIndex >= GRAPH_QUEUE_INDEX_SIZE)
		graphQueue.endIndex = 0;
}


/**
 * @brief Graph 데이터를 Queue에서 뺀다
 * 
 * @param saveBuf : 저장하고자하는 buffer 위치
 */
void deGraphQueue(graphMonitoringReusltStruct* saveBuf)
{
	pollingRealtimeDataStruct* realtimePointer;
	realtimePointer = (pollingRealtimeDataStruct*)graphQueue.realtime[graphQueue.startIndex];

	// Check queue limit
	if(graphQueue.startIndex == graphQueue.endIndex)
		return;


	// INIT
	memset(saveBuf->data[0], 0, sizeof(saveBuf->data[0]));
	memset(saveBuf->data[1], 0, sizeof(saveBuf->data[1]));

	// Channel 1
	if(realtimePointer->graph.channel1Count)
		memcpy(saveBuf->data[0], graphQueue.channel1[graphQueue.startIndex], realtimePointer->graph.channel1Count * sizeof(float));

	// Channel 2
	if(realtimePointer->graph.channel2Count)
		memcpy(saveBuf->data[1], graphQueue.channel2[graphQueue.startIndex], realtimePointer->graph.channel2Count * sizeof(float));

	// Realtime data
	memcpy(saveBuf->realtimeData, graphQueue.realtime[graphQueue.startIndex], sizeof(saveBuf->realtimeData));

	// Graph setting
	memcpy(&saveBuf->setting, &realtimePointer->graph, sizeof(saveBuf->setting));



	// Increase index
	graphQueue.startIndex++;

	// Check index limit
	if(graphQueue.startIndex >= GRAPH_QUEUE_INDEX_SIZE)
		graphQueue.startIndex = 0;
}
