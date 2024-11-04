/**
 * @file Modbus multi write 기능 지원
 *    
 *
 * @note 패킷 구조
 * 1. RTU 
 * - Write request
 * | Slave ID | Function code | Address H | Address L | Data length H | Data length L | Data byte | Data 1 H | Data 1 L | CRC L | CRC H | 
 * 
 * - Response
 * | Slave ID | Function code | Address H | Address L | Data length H | Data length L | CRC L | CRC H |
 * 
 * 2. TCP
 * - Write request
 * | T ID H | T ID L | P ID H | P ID L | Lengh H | Length L | Slave ID | Function code | Address H | Address L | Data length H | Data length L | Data byte | Data 1 H | Data 1 L | 
 * 
 * - Response
 * | T ID H | T ID L | P ID H | P ID L | Lengh H | Length L | Slave ID | Function code | Address H | Address L | Data length H | Data length L |
 * 
 * 
 * @note 사용법
 * 1. checkFunctionCode16RTU()
 *    UART 데이터를 받고 MODBUS 패킷인지 분석하는 곳에 배치
 * 
 * 2. checkFunctionCode16TCP()
 *    Ethernet 데이터를 받고 MODBUS 패킷인지 분석하는 곳에 배치
 * 
 * 3. mbProcess_16()
 *    다른 function code 실행하는 곳에 배치
 * 
 * 4. MODBUS 패킷 분석하는 곳에 다른 Function code 에러 예외처리에 0x10(16) 추가
 * 
 * 5. paraMultiSaveSequential()
 *    주기적으로 실행
 * 
 * 
 * @note 동작 순서
 * 1. Multi write - Func 16 명령 받음
 * 2. 모든 패킷이 들어올 때까지 대기
 * 3. 메시지 분석 - 명령 데이터 저장
 * 4. 저장된 명령 데이터 1개씩 - Func 6 함수로 실행 (2ms 간격)
 * 5. 현재 값과 명령 값이 같을 경우 저장 X, Min Max 등 범위 잘못될 시 Multi 저장 종료
 * 6. 완료되었을 때 func16 응답 명령 전송
 * 
 * ps. Virtual preset 주소로 전송될 경우 모든 파라미터는 메모리 저장은 실행하지 않는다.
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODBUS_MULTI_WRITE_H
#define __MODBUS_MULTI_WRITE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Defines -----------------------------------------------------------------*/
// Modbus
#define MODBUS_MULTI_RTU_HEADER_SIZE                    9           // 순수 Data의 값을 제외한 모든 Header의 크기
#define MODBUS_MULTI_TCP_HEADER_SIZE                    13

#define MODBUS_RTU_FRONT_HEADER_SIZE                    7           // 메시지에서 Data 앞에 붙는 Header의 크기
#define MODBUS_TCP_FRONT_HEADER_SIZE                    13


// Parameter save
#define MAX_PARA_MULTI_SAVE_SIZE                        123



/* Structs -----------------------------------------------------------------*/
typedef struct 
{
    uint16_t StartAddress;      // data R/W 하려는 시작 주소
    uint16_t CntRegister;       // data 개수
    uint8_t  DataStartPoint;    // Buffer에서 data의 시작 주소
    uint8_t* DataPointer;       // data buffer의 pointer
    
}ModbusCommTypedef;


typedef struct 
{
    uint16_t StartAddress;      // data R/W 하려는 시작 주소
    uint16_t CntRegister;       // data 개수

    uint8_t  saveStart;         // 파라미터 저장 시작
    uint8_t  index;             // 저장하려는 buffer의 index
    uint16_t paraBuf[128];      // 파라미터 값을 저장하고 있는 buffer
    
    uint8_t  type;              // 어느 포트에서 온 메시지 (Serial: 1, Ethernet: 2)
    uint8_t  socketNumber;      // Ethernet 사용 시 연결 되었던 소켓 번호
    uint8_t  messageHeader[16]; // 응답을 줄 때 사용할 header를 저장한 메시지

    uint8_t noSaveFlag;         // Virtual preset 모드일 때 파라미터를 저장하지 않는 동작을 할지 결정

}ParaSaveSequenceTypedef;



/* Functions -----------------------------------------------------------------*/
void mbProcess_16(int type, uint8_t* buf);
void mbSendReturnData_16(int type, uint8_t* buf);

int checkFunctionCode16RTU(uint8_t* buf, uint16_t* length);
int checkFunctionCode16TCP(uint8_t* buf, uint16_t* length);

// Min, Max 확인
void paraMultiSaveSequential(void);



/* Exported types ------------------------------------------------------------*/




#ifdef __cplusplus
}
#endif

#endif