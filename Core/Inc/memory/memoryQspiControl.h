/**
  ******************************************************************************
  * @file    eventGraphTask.h
  * @author  jun
  * @brief   Header file of event task with QSPI Memory.
  ******************************************************************************
  * @version
  * 2021.11.01 - event 저장 및 전송 처리
  * 2022.01.14 - Memory 별 통신 방법 추가
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 1. MANUFACTURER_ID 설정
  * 
  * - .c 파일
  * 1. initQspiMemory()
  *    제일 먼저 1회 실행
  * 
  * 2. saveMemoryData(), readMemoryData()
  *    필요한 곳에 실행
  * 
  * 3. returnMemoryErrorState()
  *    에러 반영 필요시 주기적으로 실행
  * 
  * 4. HAL_QPSI_TIMEOUT_DEFAULT_VALUE
  *    define된 값 바꿀 필요 있음 (200 정도로 - Memory와 통신 못 할경우 대기 시간이 길어진다)
  * 
  * 5. MONITORING_DATA_READ_WRITE
  *    저장이 안될 때 발생하는 에러를 자동 리셋되지 않도록 설정
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MEMORY_QSPI_CONTROL_H
#define MEMORY_QSPI_CONTROL_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Include --------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"


/* Defines ---------------------------------------------------------------------*/
// Flash memory erase
#define FLASH_ERASE_ADDRESS           0x1000                // (4096) 4kbyte를 기준으로 지운다

// Manufacturer ID
#define MANUFACTURER_ID_W25Q          0xEF                  // Manufacturer ID - W25Q128
#define MANUFACTURER_ID_N25Q          0x20                  // Manufacturer ID - N25Q128
#define MANUFACTURER_ID               MANUFACTURER_ID_N25Q  // FIXME: 사용하는 FLASH 선택



/* Includes ------------------------------------------------------------------*/


/* Struct ---------------------------------------------------------------------*/
typedef struct 
{
    uint8_t flashOperation;             // Manufacturer ID 확인 후 QSPI 사용해도 되는지 판단하는 변수

    uint8_t commandId;                  // Memory의 타입이 다를 때 Manucaturer id (0: 유지, 이외: command 변화 필요)
    QSPI_CommandTypeDef qspiReadId;     // QSPI Read ID 명령어 시 command
    QSPI_CommandTypeDef qspiRead;       // QSPI Read 명령어 시 command
    QSPI_CommandTypeDef qspiWrite;      // QSPI write 명령어 시 command

    uint16_t error;                     // 에러 상태

}MemoryQspi;


/* Global vriables --------------------------------------------------------------------*/
extern MemoryQspi memoryQspi;



/* Funcions --------------------------------------------------------------------*/
// QSPI 초기화
void initQspiMemory(void);
void changeQspiSetting(uint8_t manufacturerId);

// Memory 유무 확인
void checkFlashMemoryId(uint8_t manufacturerId);

// Memory read, write 동작
void saveMemoryData(uint32_t baseAddress, uint32_t index, uint32_t indexOffset, uint8_t* buf, uint16_t length);
void readMemoryData(uint32_t baseAddress, uint32_t index, uint32_t indexOffset, uint8_t* buf, uint16_t length);
void eraseMemorySubsector(uint32_t subSectorIndex);

void saveMemoryDataNoneWait(uint32_t baseAddress, uint32_t index, uint32_t indexOffset, uint8_t* buf, uint16_t length);
void eraseMemoryNoneWait(uint32_t baseAddress, uint32_t index, uint32_t indexOffset, uint16_t length);

// Memory 상태
void returnMemoryErrorState(uint16_t* error, uint16_t errCode);






#ifdef __cplusplus
    }
#endif

#endif