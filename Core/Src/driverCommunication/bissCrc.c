/**
  ******************************************************************************
  * @file    bissCrc.c
  * @author  jun
  * @brief   Source file of monitoring event task.
  * @note    SPI 통신 설정
  ******************************************************************************
  */

/* Include ---------------------------------------------------------------------*/
#include "bissCrc.h"
#include "main.h"


/* Variable ---------------------------------------------------------------------*/


/* Extern -----------------------------------------------------------------------*/



/* Static Functions --------------------------------------------------------------*/
static uint8_t crctable[256] = {0, };


/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief CRC 6의 테이블을 만든다
 * 
 * @note  1. Poly: X^6 + x + 1    (0x03)
 */
void makeCrc6Table(void) {
    uint8_t crc;

    for(uint16_t i=0; i < 256; i++)
    {
        crc = i;

        for (int k = 0; k < 8; k++)
            crc = crc & 0x80 ? (crc << 1) ^ (0x03 << 2) : crc << 1;     // 0x03 = X^6 + x + 1 을 6bit이기 때문에 2bit shift 한다.

        crctable[i] = crc;
    }
}


/**
 * @brief 메시지의 CRC 값(CRC6) 을 만든다
 * 
 * @param data : CRC를 만들 메시지 데이터
 * @param length : 메시지 길이
 * @return uint8_t : 최종 CRC 값
 */
uint8_t makeCrc6FromTable(uint8_t* data, uint8_t length) {
    uint8_t crc = 0;

    while (length--)
    {
        crc = crctable[crc ^ (*data++)];
    }

    // 최종 ">> 2"는 6Bit 계산 시 left로 2bit shift 했었기 때문에 다시 right로 2bit 이동한다. 
    crc = crc >> 2;

    // CRC의 3번 bit 반전 (FIXME: BISS 통신 때문에 적용됨)
    if(crc & 0x04)
        crc = crc & 0xFB;
    else
        crc = crc | 0x04;


    return crc;        
}



/**
 * @brief CRC-6 의 계산
 * 
 * @note   받은 메시지를 BIT 별로 XOR를 하여 CRC를 계산한다.
 * @return uint8_t  (계산된 CRC 값)
 */
uint8_t makeCrcDirect(uint8_t* msgBuf)
{
    char crcVal[6] = {0, };
    char DoInvert;
    uint32_t crcResult = 0;
    uint32_t messageVal;


    // 배열 버퍼를 하나의 변수에 값 할당 (BISS 통신 때문에 데이터 변경)
    messageVal = ((uint32_t)(msgBuf[0] & 0x3F) << 11) + ((uint32_t)msgBuf[1] << 3) + (msgBuf[2] >> 5);


    // CRC 계산하기 위해 XOR  - 정수에서 바로 계산하기 위해서는 비트 별 연산이 필요
    for (uint8_t i=0; i < BISS_MESSAGE_LENGTH; ++i)
    {
        DoInvert = (1 == ((messageVal >> (BISS_MESSAGE_LENGTH - 1 - i)) & 0b1)) ^ crcVal[5];           // XOR required ?

        crcVal[5] = crcVal[4];
        crcVal[4] = crcVal[3];
        crcVal[3] = crcVal[2];
        crcVal[2] = crcVal[1];
        crcVal[1] = crcVal[0] ^ DoInvert;
        crcVal[0] = DoInvert;
    }

    // 배열 값 정수로 변환
    for(uint8_t i=0; i < 6; i++)
    {
        crcResult += (crcVal[i] ? 1 << i : 0);
    }

    // CRC의 3번 bit 반전 (FIXME: BISS 통신 때문에 적용됨)
    if(crcResult & 0x04)
        crcResult = crcResult & 0xFFFFFFFB;
    else
        crcResult = crcResult | 0x04;


    return crcResult;
}



/**
 * @brief CRC-8 계산을 위한 테이블
 */
void makeCrc8Table( void ) {		
    int i, j;		
    unsigned long poly, c;	
    // terms of polynomial defining this crc (except x^8)
    static const uint8_t p[] = {0, 1, 2};		
		
    // make exclusive-or pattern from polynomial (0x07)
    poly = 0L;		
    for ( i = 0; i < sizeof( p ) / sizeof( uint8_t ); i++ ) {		
        poly |= 1L << p[i];		
    }		
		
    for ( i = 0; i < 256; i++ ) {		
        c = i;		
        for ( j = 0; j < 8; j++ ) {		
            c = ( c & 0x80 ) ? poly ^ ( c << 1 ) : ( c << 1 );		
        }		
        crctable[i] = (uint8_t) c;		
    }		
}	
