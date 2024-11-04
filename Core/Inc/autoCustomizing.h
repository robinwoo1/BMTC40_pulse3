/**
  ******************************************************************************
  * @file    autoCustomizing.h
  * @author  jun
  * @brief   Header file of auto customizing.
  ******************************************************************************
  * @version
  * 2022.03.07 - 체결 Auto customizing
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 1. SaveAutoSettingParaValue()
  *    주기적으로 실행
  * 
  * 2. selectAutoCustomMode()
  *    Modbus func: 4 에서 실행, A: 4009, 4010, 4011, 4012
  *
  * 3. AutoSettingSpeedAndSetting()
  *    Velocity 에서 체결 마지막에 실행
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef AUTO_CUSTOMIZING_H
#define AUTO_CUSTOMIZING_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Defines ------------------------------------------------------------------*/
// Auto customizing 설정 모드
#define SOFT_JOINT_MODE                     0
#define HARD_JOINT_MODE                     1
#define START_AUTO_CUSTOMIZING              2
#define SAVE_AUTO_CUSTOMIZING               3



/* Structs ------------------------------------------------------------------*/
typedef struct 
{
    int FlagStart;
    int FlagSetting;
    int FlagTgSpeed;
    int FlagFrSpeed;
    int FlagSavePara;
    long AvgA2;
    long CurrentA1;
    long CurrentA2;
    uint16_t CurrnetSpeed;
    uint16_t CurrentSettingPoint;
    uint16_t CurrnetFSpeed;
    uint16_t CurrentFAngle;
    int CntFasteningSq;
    int OldSpeed;
    float OldSumAcc;
    long OldFreeAngle;
}auto_setting;



/* Extern ------------------------------------------------------------------*/
extern auto_setting AutoSetting;


/* Function ------------------------------------------------------------------*/
void SaveAutoSettingParaValue(void);
void selectAutoCustomMode(uint8_t type, uint16_t data);
void AutoSettingSpeedAndSetting(void);



#ifdef __cplusplus
    }
#endif

#endif



