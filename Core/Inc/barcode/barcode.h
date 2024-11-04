/**
  ******************************************************************************
  * @file    barcode.h
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
  *    presetBarcodeSetWrite()
  *    modelBarcodeSetRead()
  *    modelBarcodeSetWrite()
  *    바코드의 설정 값 읽고 쓸때 사용
  * 
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BARCODE_H
#define __BARCODE_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/

// FIXME: 바코드 기능 활성화 여부
#define PRESET_BARCODE_ACTIVE               1                       // 바코드에 따른 preset 변경 
#define MODEL_BARCODE_ACTIVE                0                       // 바코드에 따른 model 변경
#define BARCODE_REALTIME_EVENT_ACTIVE       1                       // 바코드 읽을 때 Realtime 이벤트 발생 여부


// Barcode
#define NUMBER_OF_BARCODE_INDEX			    20                      // Barcode 목록 개수
#define NUMBER_OF_BARCODE_CHARACTERS	    64                      // 1목록 당 Barcode 데이터 개수


#define BARCODE_SCAN_TIMEOUT                10000                   // 10 sec - 바코드 스캔 후 데이터를 받지 못했을 때 종료하는 기준 시간 
#define BARCODE_RX_COMPLETE_WAIT_TIMEOUT    100                     // 100 ms - 바코드 데이터를 받기 시작하고 완료를 판단한 기준 시간


#define BARCODE_START_ASCII                 32                      // ' ' 공백 문자
#define BARCODE_END_ASCII                   126                     // '~' 문자


// QSPI - Memory에 저장 시작 주소
#define MEM_PRESET_BARCODE_START_ADDRESS                0x0C00000
#define MEM_MODEL_BARCODE_START_ADDRESS                 0x0C01000



/* Enums ---------------------------------------------------------------------*/

typedef enum {
    BAROCDE_STATE_INIT = 0,
    BARCODE_STATE_RUN,
    BARCODE_STATE_END,
    BARCODE_STATE_REINIT,

}BARCODE_STATE;



/* Struct ---------------------------------------------------------------------*/

typedef struct
{
    uint8_t     state;
    uint8_t     readStart;
    uint32_t    readWaitTime;

    uint8_t     BarRx_data[2];                                  // Barcode ringbuffer data
    uint8_t     data[NUMBER_OF_BARCODE_CHARACTERS];

}barcodeStateStruct;



/**
 * @brief 데이터 1byte 씩 정렬
 */
#pragma pack(push,1)

typedef struct
{
	uint16_t    pm;                                             // Preset
    uint64_t    mask;                                           // Barcode 판단 시 판단에 사용할 bit mask
    uint8_t     reserve[6];
	char        d_barcode[NUMBER_OF_BARCODE_CHARACTERS];		// Barcode char data

} t_barcode;



typedef struct
{
	uint8_t     current_barcode_no;                                 // 현재 설정하고자하는 barcode의 번호
    uint8_t     stepComplete;                                       // Model - barcode 기능에서 barcode가 읽혀 step이 완료되었을 때

	uint8_t     flag_scan_barcode_index;                            // Barcode를 적용하고자 하는 preset 번호
	uint8_t     flag_reset_all;                                     // Barcode remove from memory
	uint8_t     flag_barcode_read_complete;                         // Read start
	uint8_t     save_data_no; 		                                // Save to flash하는 flag
	t_barcode   barcode[NUMBER_OF_BARCODE_INDEX];                   // Barcode list

    uint8_t     barcedTempIndex;
    t_barcode   barcodeTemp;										// 바코드로 읽은 데이터를 임시 저장

} t_barcode_settings;

#pragma pack(pop)
 



/* Extern -----------------------------------------------------------------------*/

extern barcodeStateStruct       barcode;

extern t_barcode_settings       presetBarcode;
extern t_barcode_settings       modelStepBarcode;




/* Functions --------------------------------------------------------------------*/

void barcodeProcess(void);

void initBarcode(void);
void runBarcode(void);


void checkBarcodeComm(void);
void barcodeRxStart(void);
void barcodeRxCallback(void);


void checkBarcodeMessage(void);
void barcodeChangePreset(void);
void barcodeReadForMem(t_barcode_settings* barcodeStr);


void BarcodeReadTrigger(void);


void ReadBarcodeDataAll(t_barcode_settings* BarcodeStruct, uint32_t base_address);
void WriteBarcodeData(uint32_t baseAddress, uint8_t index, t_barcode_settings* BarcodeStruct);
void Clear_BarcodeData(int no, t_barcode_settings *barcodeStruct);
void ClearAll_BarcodeData(t_barcode_settings *barcodeStruct, uint32_t baseAddress);



// MODBUS
uint16_t presetBarcodeSetRead(uint16_t address);
int8_t   presetBarcodeSetWrite(uint16_t address, uint16_t data);
uint16_t modelBarcodeSetRead(uint16_t address);
int8_t   modelBarcodeSetWrite(uint16_t address, uint16_t data);




#ifdef __cplusplus
    }
#endif

#endif





