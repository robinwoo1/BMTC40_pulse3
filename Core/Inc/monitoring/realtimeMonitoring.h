/**
  ******************************************************************************
  * @file    realtiemMonitoring.h
  * @author  jun
  * @brief   Header file of barcode operation process.
  ******************************************************************************
  * @version
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 활성화 기능을 1로 설정
  * 1. PRESET_BARCODE_ACTIVE
  * 2. MODEL_BARCODE_ACTIVE
  * 
  * 
  * - .C 파일
  * 1. barcodeRxStart()
  *    DMA Error callback 발생 시 1회 실행
  * 
  * 2. barcodeRxCallback()
  *    Receive callback 발생 시 1회 실행 - circular 동작 시
  * 
  * 3. barcodeProcess()
  *    Main loop 에서 계속해서 실행
  * 
  * 4. presetBarcodeSetRead()
  * 
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __REALTIME_MONITORING_H
#define __REALTIME_MONITORING_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "graphMonitoring.h"



/* Define ------------------------------------------------------------------*/

// FIXME: Realtime 기능 활성화 여부
#define REALTIME_MONITORING_REVISION            0
#define REALTIME_BATTERY_TOOL_ACTIVE            1             // Battery tool 일때 다르게 동작하는 것을 활성화 시킨다
#define REALTIME_PARAMON_TOUCH_ACTIVE           0             // Paramon touch로 데이터 전송하는 것 활성화
#define REALTIME_LCD_SDCARD_ACTIVE              0             // LCD - SD Card로 데이터 전송하는 것 활성화



#define POLLING_REALTIME_MAX_INDEX		        10            // Queue에 쌓는 최대 Realtime index 개수
#define POLLING_REALTIME_MAX_SIZE               256           // Relatime 1개당 데이터 크기

#define REALTIME_STATUS_INDEX					12            // Realtime에서 Status 상태에 해당하는 위치
#define REALTIME_BARCODE_SIZE                   32            // 바코드 관련 주소는 32개, 주소당 2개의 ASCII값

#define REALTIME_UPDATE_MINIUM_TIMEOUT          100           // Realtime 이벤트가 연속적으로 발생했을 때 최소 갱신 시간 (100ms 이후에 값 변경)



/* Enums ---------------------------------------------------------------------*/


/* Struct ---------------------------------------------------------------------*/

typedef struct 
{
    uint16_t    revision;
    uint16_t    eventId;
    uint16_t    date[2];
    uint16_t    time[2];

    uint16_t    fasteningTime;
    uint16_t    preset;
    uint16_t    unit;
    uint16_t    screwCount;
    uint16_t    fl;
    uint16_t    errorCode;
    uint16_t    status;

    uint16_t    targetTQ[2];
    uint16_t    convetTQ[2];
    uint16_t    seatingTQ[2];
    uint16_t    clampTQ[2];
    uint16_t    prevailingTQ[2];
    uint16_t    snugTQ[2];

    uint16_t    speed;
    uint16_t    angle1;
    uint16_t    angle2;
    uint16_t    angle3;
    uint16_t    snugAngle;

    uint16_t    reserve[8];
    uint16_t    barcode[REALTIME_BARCODE_SIZE];

    graphSetResultStruct    graph;

}pollingRealtimeDataStruct;


typedef struct
{
	uint8_t 	saveIndex;
	uint8_t 	sendIndex;

	uint8_t	data[POLLING_REALTIME_MAX_INDEX][POLLING_REALTIME_MAX_SIZE];
}pollingRealtimeStruct;





/* Extern -----------------------------------------------------------------------*/
extern pollingRealtimeStruct realPollStruct;




/* Functions --------------------------------------------------------------------*/

void realtimeMonitoringProcess(void);


void mbGetMonitoringData(pollingRealtimeDataStruct* buf);
uint8_t judgeRealtimeInitFastenFromError(uint8_t errorCode);


void getRealtimeData(pollingRealtimeStruct* realtime, uint16_t* buf);
void putRealtimeData(pollingRealtimeStruct* realtime);
uint8_t getRealtimeFlag(pollingRealtimeStruct* realtime);






#ifdef __cplusplus
    }
#endif

#endif





