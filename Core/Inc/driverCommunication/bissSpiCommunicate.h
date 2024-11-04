/**
  ******************************************************************************
  * @file    bissSpiCommunicate.h
  * @author  jun
  * @brief   Header file of BISS와 직접 SPI 통신하여 메시지 분석.
  ******************************************************************************
  * @version
  * 
  * 
  * 
  * 7. checkEncoderError
  *    엔코더 값 오류 있는지 확인 - 1ms 주기 실행
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BISS_SPI_COMMUNICATE_H
#define __BISS_SPI_COMMUNICATE_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Include --------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "bissCrc.h"

/* Defines ---------------------------------------------------------------------*/
#define BISS_MESSAGE_LENGTH               17        // Maxon motor BISS 엔코더 메시지 길이


/* Struct ----------------------------------------------------------------------*/
/**
 * @brief LS 절대치 BISS 엔코더 - 총 47 bit
 */
typedef struct {
    unsigned int start      : 1;
    unsigned int cds        : 1;
    unsigned int mt         : 16;

    unsigned int data       : 21;

    unsigned int nerr       : 1;
    unsigned int nwarn      : 1;

    unsigned int crc        : 6;

} bissLsTypedef;


/**
 * @brief Maxon 절대치 BISS 엔코더 -  총 23 bit
 */
typedef struct {
    unsigned int ack        : 1;
    unsigned int start      : 1;
    unsigned int cds        : 1;

    unsigned int data       : 12;

    unsigned int nerr       : 1;
    unsigned int nwarn      : 1;

    unsigned int crc        : 6;

} bissMaxonTypedef;



typedef struct {
    uint8_t     commState;                  // 통신 상태 확인
    uint32_t    spiErrorCount;              // 통신 에러 횟수
    uint32_t    totalErrCnt;                // 총 통신 에러 획수

    uint8_t     spiRxComplete[10];          // BISS SPI 통신 완료 후 Copy한 패킷
    uint8_t     commComplete;

} bissSpiTypedef;



/* Global vriables ------------------------------------------------------------------*/
extern bissSpiTypedef bissSpi;


/* Funcions --------------------------------------------------------------------------*/
void bissSpiSetting(void);
void bissSpiRecvDma(void);

void lsBissDataAnalyze(void);
void maxonBissDataAnalyze(void);

void checkBissEncoderComm(void);
void checkEncoderError(void);





#ifdef __cplusplus
    }
#endif

#endif
