/**
  ******************************************************************************
  * @file    memoryQspiContorl.c
  * @author  jun
  * @brief   Source file of monitoring event task.
  * @note    Monitoring event 메모리 저장 및 데이터 전송
  ******************************************************************************
  */


 /**
 * @note 외부 Flash 메모리 구조
 * 
 * @param 메모리 스펙
 * - Sector    크기  : subsector * 16 byte
 * - Sector    개수  : 256 개
 * - Subsector 크기  : 4096 byte   
 * - Subsector 개수  : 4096 개
 * - Subsector erase time: 0.25 ~ 0.8s
 * 
 * @param Graph event
 * - 시작 메모리 주소 : 4096 * 2050  (실제 메모리 주소는 byte당 1씩 결정된다.)
 * - Index 범위      : 0 ~ 500      (총 501개)
 * - Index 당 크기   : 4096 byte    (Index 1개당 4096 byte의 데이터 크기, Subsector 1개당 index 1개)
 * - 사용하는 Memory subsector: 2048 ~ 2548 (501개)
 * 
 * @param Realtime event
 * - 시작 메모리 주소 : 0
 * - Index 범위      : 0 ~ 65031    (65032개)
 * - Index 당 크기   : 128 byte
 * - 사용하는 Memory subsector: 0 ~ 2032 (2033개)
 */


/* Include ---------------------------------------------------------------------*/
#include "memoryQspiControl.h"
#include "stm32469i_discovery_qspi.h"
#include "stm32f7xx_hal_qspi.h"

/* Defines ---------------------------------------------------------------------*/
#define     ON      1
#define     OFF     0


/* Variable ---------------------------------------------------------------------*/
MemoryQspi memoryQspi;


/* Extern -----------------------------------------------------------------------*/
extern QSPI_HandleTypeDef hqspi;


/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief Flash memory의 QSPI를 확인 및 타입에 맞게 통신 방법 설정
 */
void initQspiMemory(void)
{
    uint8_t id;
    uint8_t buf[8];

    // Check memory id
    checkFlashMemoryId(MANUFACTURER_ID);

    // Check error
    if(memoryQspi.flashOperation == OFF)
    {
        // Find memory type
        for(uint8_t i=0; i < 2; i++)
        {
            // Change id
            switch (i)
            {
                case 0:
                    id = MANUFACTURER_ID_N25Q;
                    break;
                case 1:
                    id = MANUFACTURER_ID_W25Q;
                    break;
                default:
                    id = MANUFACTURER_ID_N25Q;
                    break;
            }

            // Set QSPI command
            changeQspiSetting(id);
            // Read memory id
            checkFlashMemoryId(id);

            // 통신 성공 시
            if(memoryQspi.flashOperation == ON)
                break;
        }
    }

    // Check read complete
    readMemoryData(0, 1, 8, buf, 1);
    // if(memoryQspi.error == ON)
    //     memoryQspi.flashOperation = OFF;     // BM 시리즈는 즉시 에러 표시되도록
}


/**
 * @brief Flash의 종류에 따라 QSPI통신 방식 변경
 * 
 * @param manufacturerId : Memory type별 id
 */
void changeQspiSetting(uint8_t manufacturerId)
{
    // Set command change
    memoryQspi.commandId = manufacturerId;

    // Change QSPI command setting
    switch (manufacturerId)
    {
        case MANUFACTURER_ID_N25Q:
            // Read id
            memoryQspi.qspiReadId.Instruction = READ_MANUFACTURER_ID_N25Q;
            memoryQspi.qspiReadId.AddressMode = QSPI_ADDRESS_NONE;
            memoryQspi.qspiReadId.DummyCycles = 0;
            memoryQspi.qspiReadId.DataMode = QSPI_DATA_1_LINE;

            // Read QSPI 
            memoryQspi.qspiRead.Instruction = QUAD_INOUT_FAST_READ_CMD;
            memoryQspi.qspiRead.AddressMode = QSPI_ADDRESS_4_LINES;
            memoryQspi.qspiRead.DummyCycles = N25Q128A_DUMMY_CYCLES_READ_QUAD;

            // Write QSPI
            memoryQspi.qspiWrite.Instruction = EXT_QUAD_IN_FAST_PROG_CMD;
            memoryQspi.qspiWrite.AddressMode = QSPI_ADDRESS_4_LINES;
            memoryQspi.qspiWrite.DummyCycles = 0;
            break;

        case MANUFACTURER_ID_W25Q:
            // Read id
            memoryQspi.qspiReadId.Instruction = READ_MANUFACTURER_ID_W25Q;
            memoryQspi.qspiReadId.AddressMode = QSPI_ADDRESS_1_LINE;
            memoryQspi.qspiReadId.DummyCycles = 0;
            memoryQspi.qspiReadId.DataMode = QSPI_DATA_1_LINE;

            // Read QSPI 
            memoryQspi.qspiRead.Instruction = QUAD_INOUT_FAST_READ_CMD;
            memoryQspi.qspiRead.AddressMode = QSPI_ADDRESS_4_LINES;
            memoryQspi.qspiRead.DummyCycles = 6;

            // Write QSPI
            memoryQspi.qspiWrite.Instruction = QUAD_IN_FAST_PROG_CMD;
            memoryQspi.qspiWrite.AddressMode = QSPI_ADDRESS_1_LINE;
            memoryQspi.qspiWrite.DummyCycles = 0;
            break;
        
        default:
            memoryQspi.commandId = 0;
            break;
    }
}




/* Memory IC 유무 확인 ----------------------------------------------------------------------*/
/**
 * @brief Manufacturer ID 읽고서 QSPI 사용가능한 지 판단
 */
void checkFlashMemoryId(uint8_t manufacturerId)
{
    uint8_t buf[17];
    uint8_t state;

    // Read ID
    state = BSP_QSPI_Read_Id(buf, 0, 4);

    // Check Manufacturer ID
    if(buf[0] == manufacturerId && state == HAL_OK)
        memoryQspi.flashOperation = ON;
    else
        memoryQspi.flashOperation = OFF;
}




/* Memory 관리 ----------------------------------------------------------------------*/
/**
 * @brief Save data in Flash memory
 * 
 * @param baseAddress   : 기본 시작이 되는 메모리 주소
 * @param index         : index 값
 * @param indexOffset   : index당 다음 메모리 주소까지 사용하는 주소 양
 * @param buf           : 저장할 데이터 버퍼
 * @param length        : 저장할 데이터 길이
 */
void saveMemoryData(uint32_t baseAddress, uint32_t index, uint32_t indexOffset, uint8_t* buf, uint16_t length)
{
    uint32_t ByteAddress;

    // Check flash 동작 유무
    if(memoryQspi.flashOperation == OFF)
    {
        memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
        return;
    }
    
    // Memory 동작 상태 확인
    QSPI_AutoPollingMemReady(&hqspi, N25Q128A_SUBSECTOR_ERASE_MAX_TIME);

    // 메모리 저장 전 Block 제거
    ByteAddress = index * indexOffset + baseAddress;   // 1 Byte당 주소 (실제주소)

    for(uint32_t addrress = ByteAddress; addrress < ByteAddress + length; addrress++)
    {
        // Check New block  (Erase 명령후 16ms 정도 Delay 발생)
        if((addrress % FLASH_ERASE_ADDRESS) == 0)
            BSP_QSPI_Erase_Block(addrress);      // Subsector를 지울 때 실제주소를 넣으면  그 Block 초기화
    }

    // write Graph data
    if( BSP_QSPI_Write((uint8_t *)buf, ByteAddress, length) != QSPI_OK )
    {
        memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
    }
}


/**
 * @brief Save data in Flash memory (기다리는 동작 없이)
 * 
 * @note  반드시 앞에 함수에서 Memory를 Erase하는 동작 수행 필요
 * 
 * @param baseAddress   : 기본 시작이 되는 메모리 주소
 * @param index         : index 값
 * @param indexOffset   : index당 다음 메모리 주소까지 사용하는 주소 양
 * @param buf           : 저장할 데이터 버퍼
 * @param length        : 저장할 데이터 길이
 */
void saveMemoryDataNoneWait(uint32_t baseAddress, uint32_t index, uint32_t indexOffset, uint8_t* buf, uint16_t length)
{
    uint32_t ByteAddress;

    // Check flash 동작 유무
    if(memoryQspi.flashOperation == OFF)
    {
        memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
        return;
    }   

    ByteAddress = index * indexOffset + baseAddress;   // 1 Byte당 주소 (실제주소)

    // 앞에 함수에서 Memory 지우는 것 실행 후 Memory 동작 상태 확인
    QSPI_AutoPollingMemReady(&hqspi, N25Q128A_SUBSECTOR_ERASE_MAX_TIME);

    // write Graph data
    if( BSP_QSPI_Write((uint8_t *)buf, ByteAddress, length) != QSPI_OK )
    {
        memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
    }
}


/**
 * @brief Read data in Flash memory
 * 
 * @param baseAddress   : 기본 시작이 되는 메모리 주소
 * @param index         : index 값
 * @param indexOffset   : index당 다음 메모리 주소까지 사용하는 주소 양
 * @param buf           : 읽고 저장할 데이터 버퍼
 * @param length        : 읽을 데이터 길이
 */
void readMemoryData(uint32_t baseAddress, uint32_t index, uint32_t indexOffset, uint8_t* buf, uint16_t length)
{
    uint32_t ByteAddress;

    // Check flash 동작 유무
    if(memoryQspi.flashOperation == OFF)
    {
        memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
        return;
    }   

    // Memory 동작 상태 확인
    QSPI_AutoPollingMemReady(&hqspi, N25Q128A_SUBSECTOR_ERASE_MAX_TIME);

    ByteAddress = index * indexOffset + baseAddress;         // 1 Byte당 주소 (실제주소)

    if( BSP_QSPI_Read((uint8_t *)buf, ByteAddress, length) != QSPI_OK )
    {
        memoryQspi.error = ON;     // MONITORING_DATA_READ_WRITE
    }   
}


/**
 * @brief 메모리의 Subsector 블럭을 지운다.
 * 
 * @param subSectorIndex : 지우고자 하는 Subsector index
 */
void eraseMemorySubsector(uint32_t subSectorIndex)
{
    uint32_t ByteAddress;

    // Check flash 동작 유무
    if(memoryQspi.flashOperation == OFF)
        return;

    // Index를 memory address로 변경
    ByteAddress = subSectorIndex * FLASH_ERASE_ADDRESS;

    // Check New block
    if((ByteAddress % FLASH_ERASE_ADDRESS) == 0)
    {
        BSP_QSPI_Erase_Block(ByteAddress);      // Subsector를 지울 때 실제주소를 넣으면  그 Block 초기화
    }    
}


/**
 * @brief 메모리의 Subsector 블럭을 지우고 정상 모드 대기 없이 실행.
 * 
 * @param baseAddress   : 기본 시작이 되는 메모리 주소
 * @param index         : index 값
 * @param indexOffset   : index당 다음 메모리 주소까지 사용하는 주소 양
 */
void eraseMemoryNoneWait(uint32_t baseAddress, uint32_t index, uint32_t indexOffset, uint16_t length)
{
    uint32_t ByteAddress;

    // Check flash 동작 유무
    if(memoryQspi.flashOperation == OFF)
        return;

    // Memory 동작 상태 확인
    QSPI_AutoPollingMemReady(&hqspi, N25Q128A_SUBSECTOR_ERASE_MAX_TIME);
    

    ByteAddress = index * indexOffset + baseAddress;   // 1 Byte당 주소 (실제주소)

    for(uint32_t addrress = ByteAddress; addrress < ByteAddress + length; addrress++)
    {
        // Check New block  (Erase 명령후 16ms 정도 Delay 발생)
        if((addrress % FLASH_ERASE_ADDRESS) == 0)
            BSP_QSPI_Erase_Block_None_Wait(addrress);      // Subsector를 지울 때 실제주소를 넣으면  그 Block 초기화
    }
}



/* Memory 동작 상태 --------------------------------------------------------------------------------------*/

/**
 * @brief QSPI 메모리의 에러 상태를 Error 변수에 반영
 * 
 * @param error     : 반영할 error 상태 변수
 * @param errCode   : 적용할 에러 코드 (MONITORING_DATA_READ_WRITE: 108)
 */
void returnMemoryErrorState(uint16_t* error, uint16_t errCode)
{
    static uint32_t count;
    static uint16_t errorState[2];

    // 저장 메모리가 없으면 에러 표시 X
    if(memoryQspi.flashOperation == OFF)
        return;

    // 에러 상태 저장
    errorState[0] = errorState[1];
    errorState[1] = memoryQspi.error;

    // 에러가 발생한 처음에 count 증가
    if(errorState[0] != errorState[1] && memoryQspi.error != 0)
        count++;
    
    // 에러 표시는 1회만 표시
    if(count > 1)
        memoryQspi.error = 0;
    
    // QSPI 에러 상태
    if(memoryQspi.error)
    {
        // Error 상태가 아닐 때 값 반영
        if(*error == 0)
            *error = errCode;

        memoryQspi.error = 0;
    }

    // 3회 이상 저장 에러 발생 시 
    if(count >= 3)
    {
        memoryQspi.flashOperation = OFF;
    }
}


