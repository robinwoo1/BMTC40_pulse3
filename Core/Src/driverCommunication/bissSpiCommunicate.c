/**
  ******************************************************************************
  * @file    bissSpiCommunicate.c
  * @author  jun
  * @brief   Source file of BISS와 SPI 통신을 하여 엔코더 데이터를 받는다.
  * 
  * @note    BISS와 다이렉트 SPI 통신 설정
  *          1. Timeout:  SPI 통신 후 최소 2us
  *          2. 통신 주기: 16.666us  (100 / 6)
  *          3. 통신 처리시간: 메시지를 전부 받는데 10us 정도 소요된다.
  *          4. 통신 소요 시간: (데이터 요청 ?us) + (데이터 받고 SVPWM 하는데 2.2us)
  *
  ******************************************************************************
  */


/* Include ----------------------------------------------------------------------*/
#include "bissSpiCommunicate.h"
#include "string.h"
#include "main.h"
#include "BldcControl.h"
#include "variable.h"
#include "svpwm.h"
#include <stdlib.h>


/* Variable ---------------------------------------------------------------------*/



/* Extern -----------------------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;

bissSpiTypedef bissSpi;                 // SPI 통신 관련
bissLsTypedef bissLs;                   // BISS 패킷 형식 - LS
bissMaxonTypedef bissMaxon;             // BISS 패킷 형식 - Maxon

/* Static Functions --------------------------------------------------------------*/


/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief BISS 엔코더와 SPI 통신 설정
 */
void bissSpiSetting(void)
{
    // HAL_GPIO_WritePin(SPI5_NSS_GPIO_Port, SPI5_NSS_Pin, GPIO_PIN_RESET); // NSS 신호 GPIO 없음
}


/**
 * @brief BISS 엔코더와 SPI 통신으로 데이터를 Receive 한다. - DMA 방식
 * 
 * @param hspi       : SPI struct
 * @param receiveBuf : 응답 buffer
 * @param length     : 응답 개수
 */
void bissSpiRecvDma(void)
{
    // Check SPI receive state
    if(bissSpi.commComplete == OFF && hspi1.State == HAL_SPI_STATE_READY)
    {
        bissSpi.commComplete = ON;
        HAL_SPI_Receive_DMA(&hspi1, spiRecvBuf, 5);
    }
    else
    {
        bissSpi.spiErrorCount++;
        bissSpi.totalErrCnt++;
    }
}



/**
 * @brief SPI의 응답 완료 후 동작 - FIXME: 토크 값 받는 함수가 있어서 외부로 이동
 * 
 * @param hspi : SPI struct
 */
// void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
// {
//     if (hspi->Instance == SPI1)
//     {
//         /* 여기 코드 실행되는데 2.2us*/
        
//         // Receive 완료 후 데이터 copy
//         memcpy(bissSpi.spiRxComplete, spiRecvBuf, 4);

//         // 통신 데이터 처리
//         maxonBissDataAnalyze();

//         // SVPWM 실행 (받은 위치 데이터를 바로 적용)
//         svpwmPositionApply();
//     }
// }


/**
 * @brief LS 드라이버 BISS 통신 데이터 분석
 */
void lsBissDataAnalyze(void)
{
    // LS 는 다시 정의 해야한다.
    
    // START 1
    bissLs.start    = (bissSpi.spiRxComplete[0] & 0b10000000) >> 7;
    // CDS 1
    bissLs.cds      = (bissSpi.spiRxComplete[0] & 0b01000000) >> 6;

    // MT 16
    bissLs.mt       = (bissSpi.spiRxComplete[0] & 0b00111111) << 10;
    bissLs.mt      += (bissSpi.spiRxComplete[1] & 0b11111111) << 2;
    bissLs.mt      += (bissSpi.spiRxComplete[2] & 0b11000000) >> 6;

    // DATA 21
    bissLs.data     = (bissSpi.spiRxComplete[2] & 0b00111111) << 15;
    bissLs.data    += (bissSpi.spiRxComplete[3] & 0b11111111) << 7;
    bissLs.data    += (bissSpi.spiRxComplete[4] & 0b11111110) >> 1;

    // ERR 1, WARN 1
    bissLs.nerr     = (bissSpi.spiRxComplete[4] & 0b00000001);
    bissLs.nwarn    = (bissSpi.spiRxComplete[5] & 0b10000000) >> 7;

    // CRC 6
    bissLs.crc      = (bissSpi.spiRxComplete[5] & 0b01111110) >> 1;
}


/**
 * @brief Maxon 드라이버 BISS 통신 데이터 분석
 */
void maxonBissDataAnalyze(void)
{
    uint8_t crc;
    uint8_t crcData[3];

    /* 앞에 2 clock을 건너뛰고 시작 */

    /* 시작 Bit의 위치를 찾기 (010이 나오는 것을 찾는다) */ 

    // Bit 판단 버퍼 (1101 0000, 1110 1000, 1111 0100, 1111 1010)
    uint8_t bitJudgeBuf[8] = {0xD0, 0xE8, 0xF4, 0xFA};
    uint8_t bitMaskBuf[8] = {0xF8, 0xFC, 0xFE, 0xFF};
    uint8_t bitShiftNum = 0;
    int     m_diff;
    
    // 시작 Bit 찾기 (010 데이터)
    for(uint8_t i=0; i < 4; i++)
    {
        if(bitJudgeBuf[i] == (bissSpi.spiRxComplete[0] & bitMaskBuf[i]))
        {
            bitShiftNum = i;
            break;
        }
    }

    // CRC 초기화
    bissMaxon.crc = 0x0;

    switch (bitShiftNum)
    {
        case 0:
            // DATA 12
            bissMaxon.data     = (bissSpi.spiRxComplete[0] & 0b00000111) << 9;
            bissMaxon.data    += (bissSpi.spiRxComplete[1] & 0b11111111) << 1;
            bissMaxon.data    += (bissSpi.spiRxComplete[2] & 0b10000000) >> 7;

            // CRC 6
            bissMaxon.crc      = (bissSpi.spiRxComplete[2] & 0b00011111) << 1;
            bissMaxon.crc     += (bissSpi.spiRxComplete[3] & 0b10000000) >> 7;

            // CRC 계산에 맞게 데이터 변경
            crcData[0] = ((bissSpi.spiRxComplete[0] & 0b00100000) >> 5);
            crcData[1] = ((bissSpi.spiRxComplete[0] & 0b00011111) << 3) + ((bissSpi.spiRxComplete[1] & 0b11100000) >> 5);
            crcData[2] = ((bissSpi.spiRxComplete[1] & 0b00011111) << 3) + ((bissSpi.spiRxComplete[2] & 0b11100000) >> 5);
        break;
        
        case 1:
            // Data (1bit shift)
            bissMaxon.data     = (bissSpi.spiRxComplete[0] & 0b00000011) << 10;
            bissMaxon.data    += (bissSpi.spiRxComplete[1] & 0b11111111) << 2;
            bissMaxon.data    += (bissSpi.spiRxComplete[2] & 0b11000000) >> 6;

            // CRC 6
            bissMaxon.crc      = (bissSpi.spiRxComplete[2] & 0b00001111) << 2;
            bissMaxon.crc     += (bissSpi.spiRxComplete[3] & 0b11000000) >> 6;

            // CRC 계산에 맞게 데이터 변경
            crcData[0] = ((bissSpi.spiRxComplete[0] & 0b00010000) >> 4);
            crcData[1] = ((bissSpi.spiRxComplete[0] & 0b00001111) << 4) + ((bissSpi.spiRxComplete[1] & 0b11110000) >> 4);
            crcData[2] = ((bissSpi.spiRxComplete[1] & 0b00001111) << 4) + ((bissSpi.spiRxComplete[2] & 0b11110000) >> 4);
        break;

        case 2:
            // Data (2bit shift)
            bissMaxon.data     = (bissSpi.spiRxComplete[0] & 0b0000001) << 11;
            bissMaxon.data    += (bissSpi.spiRxComplete[1] & 0b11111111) << 3;
            bissMaxon.data    += (bissSpi.spiRxComplete[2] & 0b11100000) >> 5;

            // CRC 6
            bissMaxon.crc      = (bissSpi.spiRxComplete[2] & 0b00000111) << 3;
            bissMaxon.crc     += (bissSpi.spiRxComplete[3] & 0b11100000) >> 5;

            // CRC 계산에 맞게 데이터 변경
            crcData[0] = ((bissSpi.spiRxComplete[0] & 0b00001000) >> 3);
            crcData[1] = ((bissSpi.spiRxComplete[0] & 0b00000111) << 5) + ((bissSpi.spiRxComplete[1] & 0b11111000) >> 3);
            crcData[2] = ((bissSpi.spiRxComplete[1] & 0b00000111) << 5) + ((bissSpi.spiRxComplete[2] & 0b11111000) >> 3);
        break;

        case 3:
            // Data (3bit shift)
            bissMaxon.data     = (bissSpi.spiRxComplete[1] & 0b11111111) << 4;
            bissMaxon.data    += (bissSpi.spiRxComplete[2] & 0b11110000) >> 3;

            // CRC 6
            bissMaxon.crc      = (bissSpi.spiRxComplete[2] & 0b00000011) << 4;
            bissMaxon.crc     += (bissSpi.spiRxComplete[3] & 0b11110000) >> 4;

            // CRC 계산에 맞게 데이터 변경
            crcData[0] = ((bissSpi.spiRxComplete[0] & 0b00000100) >> 2);
            crcData[1] = ((bissSpi.spiRxComplete[0] & 0b00000011) << 6) + ((bissSpi.spiRxComplete[1] & 0b11111100) >> 2);
            crcData[2] = ((bissSpi.spiRxComplete[1] & 0b00000011) << 6) + ((bissSpi.spiRxComplete[2] & 0b11111100) >> 2);
        break;
    }

    // CRC 계산
    crc = makeCrc6FromTable(crcData, 3);  

    // CRC 확인
    if(crc == bissMaxon.crc)
    {
        // 실제 사용 데이터 할당 (절대치 엔코더)
        bldcControl.absoluteAngle = bissMaxon.data;
        
        // 상태 표시
        bissSpi.spiErrorCount = 0;
        bissSpi.commState = 1;
    }
    else
    {
        bissSpi.spiErrorCount++;
        bissSpi.totalErrCnt++;

        // 에러 발생 시 처리 - 변화된 각도 재반영
        m_diff = bldcControl.absoluteAngleOld - bldcControl.absoluteAngleOld2;

        // Check 4096 Pulse overflow
        if(m_diff > 3000)
        {
            if(bldcControl.absoluteAngleOld > bldcControl.absoluteAngleOld2)
            {
                m_diff -= 4096;
                m_diff = -abs(m_diff);
            }
        }
        else if(m_diff < -3000)
        {
            if(bldcControl.absoluteAngleOld < bldcControl.absoluteAngleOld2)
            {
                m_diff += 4096;
                m_diff = abs(m_diff);
            }
        }

        m_diff = bldcControl.absoluteAngle + m_diff * 0.9;

        // Limit 확인
        if(m_diff > 4096)
            m_diff = m_diff - 4096;
        else if(m_diff < 0)
            m_diff = 4096 + m_diff;

        bldcControl.absoluteAngle = m_diff;
    }

    // 통신 완료
    bissSpi.commComplete = OFF;
}


/**
 * @brief BISS 엔코더와 계속해서 통신 중인지 판단.
 * 
 * @note 통신이 실패하고 있을 경우 알람 발생.
 */
void checkBissEncoderComm(void)
{
    static uint8_t bissCommState[2] = {0, };

    // 통신 정상 유무 판단
    if(bissSpi.spiErrorCount > 0xF)
        bissSpi.commState = 0;

    // Error count limit
    if(bissSpi.spiErrorCount > 0xFFFFFF)
        bissSpi.spiErrorCount = 0xFF;

    // 엔코더와 통신상태 저장
    bissCommState[1] = bissCommState[0];
    bissCommState[0] = bissSpi.commState;

    // 엔코더 상태 에러 발생
    if(bissCommState[1] > bissCommState[0] && bissSpi.commState == 0)
    {
        // // 엔코더 읽기 오류 알람
        // if(system_err == 0)
        //     system_err = ENCODER_OPEN_ERROR;
    }

    // SPI 통신 비정상 - 모터 정지
    if(bissSpi.commState == 0)
    {
        if(Flag.Run && Flag.Stopping == OFF)
            Flag.Stopping = ON;
    }

    // 통신 에러 상태 복구
    if(bissSpi.spiErrorCount > 0xF)
    {
        // 통신 초기화
        HAL_SPI_DMAStop(&hspi1);
        bissSpi.commComplete = OFF;

        bissSpi.spiErrorCount = 0;
    }
}



/**
 * @brief Encoder pulse의 변화량을 보고 에러 상황을 구분
 * 
 * @note  1. Encoder의 값이 600 pulse이상 차이가 발생한 경우 에러
 *        2. 1ms 루틴 안에서 실행
 *        3. 1ms 동안 8000rpm일 때 746 이하로 돔 
 *        4. "pulseDiffLimit" 는 최대 RPM, Encoder pulse 값에 따라 변경됨
 */
void checkEncoderError(void)
{
    static int      pulseOld;
    static int      pulseDiffLimit;
    static uint8_t  init;
    int pulse;

    // Get pulse
    pulse = bldcControl.absoluteAngle;

    // Check pulse init
    if(init == OFF && bldcControl.flagZeroAngleOn)
    {
        // 계산에 필요한 값이 초기화 되었을 때 (최대 속도, 1회전 Pulse 개수)
        if(Motor.num_pulse != 0 && Motor.max_motor_speed != 0)
        {
            pulseDiffLimit = (int)(Motor.max_motor_speed * Motor.num_pulse / 60 / 1000) + 200;   // 최대 RPM에서 1ms당 Encoder pulse 발생 개수 (746 정도)
            pulseOld = pulse;
            init = ON;
        }
    }

    // Z Pulse 다시 찾아 초기화 필요한 경우
    if(bldcControl.flagZeroAngleOn == OFF)
        init = OFF;

    // 초기화 안 되었으면 실행 X
    if(init == OFF)
        return;    

    // Biss 통신 에러 중일 때는 초기화
    if(bissSpi.commState == 0)
        pulseOld = pulse;

    // Check pulse 증가량 
    if(pulseOld > pulse)
    {
        // Pulse 값이 4096 overflow해서 초기화 되었을 때
        if(pulseOld - pulse > 2000)
        {
            // if(4096 - pulseOld + pulse > pulseDiffLimit)
            // {
            //     system_err = ENCODER_OPEN_ERROR;
            // }
        }
        // 역으로 회전했을 때 (Pulse 감소)
        else if(pulseOld - pulse > pulseDiffLimit)
        {
            // system_err = ENCODER_OPEN_ERROR;
        }
    }
    else
    {
        // 역으로 Pulse 값이 4096 overflow 했을 때
        if(pulse - pulseOld > 2000)
        {
            if(4096 - pulse + pulseOld > pulseDiffLimit)
            {
                // system_err = ENCODER_OPEN_ERROR;
            }
        }
        // 정으로 회전했을 때 (Pulse 증가)
        else if(pulse - pulseOld > pulseDiffLimit)
        {
            // system_err = ENCODER_OPEN_ERROR;
        }
    }

    // Set pulse
    pulseOld = pulse;    
    
    // 에러 발생 시 Driver lock
    if(system_err == ENCODER_OPEN_ERROR)
    {
        if(Flag.Run && Flag.Stopping == OFF)
            Flag.Stopping = ON;
    }
}
