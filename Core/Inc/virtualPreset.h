/**
  ******************************************************************************
  * @file    virtualPreset.h
  * @author  jun
  * @brief   Header file of event task with QSPI Memory.
  ******************************************************************************
  * @version
  * 2022.05.23 - Virtual preset의 Preset 1번 기억하는 동작 과정으로 수정
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 1. 
  *    FASTENING_START_ADDRESS
  *    주소 및 파라미터의 개수 설정
  * 
  * - .c 파일
  * 1. startVirtualPreset
  *    모드 버스 - 0x06 - 5200  Virtual preset 통신 데이터 받았을 때 실행
  * 
  * 2. checkParaSaveWithVirtual
  *    모드 버스 - 0x06 - 일반 파라미터 변경하려고 할 때 마지막에 실행
  * 
  * 3. checkVirtualPresetChange
  *    Realtime 데이터 생성하기 전에 실행
  * 
  * 4. virtualPrsetState
  *    BM - LCD에 "VP" 표시하기 위해 사용
  * 
  * 
  * 
  * 
  * @note 동작 과정 
  * 
  * - Virtual mode 활성화
  * 1. 5200으로 Virtual Fastening, Advanced 데이터 받음
  * 2. VP 모드로 전환
  * 3. Fastening, Advanced 데이터를 임시버퍼에 저장
  * 4. Preset 1로 전환
  * 5. Preset change 이벤트 발생
  * 
  * 
  * - Virtual mode 활성화 중일 때
  * 1. Preset 1으로 값 저장 시 Preset 1에는 반영 안 하고 임시 버퍼에만 저장, 메모리에 파라미터 저장
  * 
  * 
  * - Virtual mode 비활성화
  * 1. Preset 변경 시
  * 2. VP 모드 OFF
  * 3. 임시 버퍼 값 - 실제 Preset 1으로 복원, 체결 값 재설정
  * 
  * 
  * - VP모드에서 Preset 1 설정 시 - 임시 버퍼에 값 저장 및 메모리에 저장
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef VIRTUAL_PRESET_H
#define VIRTUAL_PRESET_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Include --------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "parameter.h"


/* Defines ---------------------------------------------------------------------*/
// 파라미터 시작 주소 및 개수
#define FASTENING_START_ADDRESS             TCAM_ACTM1               // Fastening 시작 주소
#define ADVANCED_START_ADDRESS              ADVANCED_MODE1           // Advanced 시작 주소

#define VIRTUAL_PRESET_DISPLAY_NUM          255                     // Virtual preset 사용 시 외부에 표시하는 숫자



/* Struct ---------------------------------------------------------------------*/
// Virtual preset Struct
typedef struct
{
    uint8_t  virtualMode;                               // Virtual Preset 모드 (0: Preset mode, 1: Virtual preset mode)
    uint8_t  presetChange;                              // Preset 변경된 것 확인

    uint16_t fastenBackUp[PRESET_SIZE];                 // Preset 1 Fastening 파라미터 백업
    uint16_t advancedBackUp[ADV_PARA_SIZE];             // Preset 1 Advanced 파라미터 백업

}virtualPresetStruct;



/* Funcions --------------------------------------------------------------------*/

void startVirtualPreset(void);
void stopVirtualPreset(void);

void checkParaSaveWithVirtual(uint16_t address, uint16_t value);
void checkVirtualPresetChange(void);
uint8_t checkVirtualPresetAddress(uint16_t address);

uint8_t virtualPrsetState(void);
void vpPresetChangeActive(void);

void backupVirtualPreset1(void);
void restoreVirtualPreset1(void);

void setVirtualBuf(uint16_t address, uint16_t value);
uint16_t getVirtualBuf(uint16_t address);




#ifdef __cplusplus
    }
#endif

#endif
