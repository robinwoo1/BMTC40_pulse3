/**
  ******************************************************************************
  * @file    virtualPreset.c
  * @author  jun
  * @brief   Source file of monitoring event task.
  * @note    Virtual preset에 관한 데이터를 관리
  ******************************************************************************
  */

/* Include ---------------------------------------------------------------------*/
#include "virtualPreset.h"
#include "variable.h"
#include "common.h"
#include "timer.h"
#include "main_bg.h"
#include "backup.h"
#include "modbus.h"
#include "realtimeMonitoring.h"


/* Variable ---------------------------------------------------------------------*/
virtualPresetStruct vPreset;                // Virtual preset 구조체



/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief Virtual preset 모드를 시작한다
 */
void startVirtualPreset(void)
{
    // Virtual preset mode 확인
    if(vPreset.virtualMode == OFF)
    {
        // Virtual preset mode 활성화;
        vPreset.virtualMode = ON;

        // 실제 preset 1 파라미터를 Backup
        backupVirtualPreset1();

        // Preset 1 모드로 변경
        Processing_number = 1;
        Fastening_number = 1;

        StatusOutput = rPRESET_CHANGE;
        ClearSendData_Init(ON);
        putRealtimeData(&realPollStruct);
    }
}


/**
 * @brief Virtual preset 모드를 종료한다
 */
void stopVirtualPreset(void)
{
    // Virtual preset mode 확인
    if(vPreset.virtualMode == ON)
    {
        // Virtual preset mode 비활성화
        vPreset.virtualMode = OFF;

        // Restore preset 1 파라미터
        restoreVirtualPreset1();

        // 체결 값 재설정
        Set_Torque_variable();
        Init_Para_limit();
    }
}


/**
 * @brief 데이터를 저장하려고 할 때 Virtual preset으로 저장 가능한 상태인지 판단 및 설정
 * 
 * @param address : 저장하려는 주소
 * @param value   : 저장하려는 값
 */
void checkParaSaveWithVirtual(uint16_t address, uint16_t value)
{
    uint8_t retval;

    // Virtual preset mode 확인
    if(virtualPrsetState())
    {
        // Virtual preset 저장 주소인지 확인
        retval = checkVirtualPresetAddress(address);

        // Virtual preset 값 관련 값 (파라미터를 저장만 하고 Para 버퍼에 반영하지 않는다)
        if(retval)
        {
            // 데이터 저장 명령 취소
            Flag.SaveParameter = OFF;

            // 파라미터 저장 실행
            if(value >= Para[address].min && value <= Para[address].max)
            {
                ParameterWriteToMemory(address, value);

                // 임시 버퍼에 값 저장
                setVirtualBuf(address, value);
            }
        }
    }
}


/**
 * @brief Preset의 값이 변경되었을 때 Virtual preset 처리
 * 
 * @param realtimeFlag : Realtime 이벤트 flag
 * @param status       : 이벤트 종류
 */
void checkVirtualPresetChange(void)
{
    static uint8_t virtualState[2] = {0, };
    static uint32_t timeout;
    static uint32_t changeOn;

    virtualState[0] = virtualState[1];
    virtualState[1] = virtualPrsetState();

    // VP 모드 활성화 시 Timeout 시작
    if(virtualState[0] < virtualState[1])
        timeout = HAL_GetTick();

    // VP 모드 활성화 이후 100ms 지나는 것 체크
    if(HAL_GetTick() - timeout > 100 && timeout != 0)
        changeOn = ON;

    // VP 모드 해제 - 변수 초기화
    if(virtualState[1] == 0)
    {
        changeOn = OFF;
        timeout = 0;
    }

    // Virtual preset이 ON인 상태일 때
    if(virtualState[0] == virtualState[1] && virtualState[1] == ON && changeOn)
    {
        // Preset이 변경되었을 때
        if(vPreset.presetChange)
        {
            stopVirtualPreset();
            changeOn = OFF;
        }
    }
    else
    {
        // VP 모드가 X, VP 모드 활성화된지 100ms가 안되었으면 OFF
        vPreset.presetChange = OFF;     
    }
}


/**
* @brief Preset이 변경된 것 VP에서 확인하기 위한 변수 활성화
 */
void vpPresetChangeActive(void)
{
    vPreset.presetChange = ON;
}


/**
 * @brief 현재 주소가 Virtual preset의 저장하는 주소인지 판단
 * 
 * @param address   : 저장하려는 주소
 * @return uint8_t  (0: 저장하려는 주소 X,  1: 저장하려는 주소 O)
 */
uint8_t checkVirtualPresetAddress(uint16_t address)
{
    uint8_t retval = 0;
    
    // Virtual preset의 범위 확인
    if(address >= FASTENING_START_ADDRESS && address < FASTENING_START_ADDRESS + PRESET_SIZE)
        retval = ON;
    else if(address >= ADVANCED_START_ADDRESS && address < ADVANCED_START_ADDRESS + ADV_PARA_SIZE)
        retval = ON;

    return retval;
}


/**
 * @brief 현재 Virtual preset 모드가 활성화 되어 있는지 전달
 * 
 * @return uint8_t 0: 비활성화, 1: 활성화
 */
uint8_t virtualPrsetState(void)
{
    return vPreset.virtualMode;
}


/**
 * @brief 실제 Preset 1의 파라미터 값을 Backup
 */
void backupVirtualPreset1(void)
{
    // Fastenieng 데이터 저장
    for(uint16_t i = FASTENING_START_ADDRESS; i < FASTENING_START_ADDRESS + PRESET_SIZE; i++)
    {
        setVirtualBuf(i, Para[i].val);
    } 

    // Advanced 데이터 저장
    for(uint16_t i = ADVANCED_START_ADDRESS; i < ADVANCED_START_ADDRESS + ADV_PARA_SIZE; i++)
    {
        setVirtualBuf(i, Para[i].val);
    }
}


/**
 * @brief Backup 해두었던 Preset 1의 파라미터를 restore하여 반영
 */
void restoreVirtualPreset1(void)
{
    // Fastenieng 데이터 저장
    for(uint16_t i = FASTENING_START_ADDRESS; i < FASTENING_START_ADDRESS + PRESET_SIZE; i++)
    {
        Para[i].val = getVirtualBuf(i);
    } 

    // Advanced 데이터 저장
    for(uint16_t i = ADVANCED_START_ADDRESS; i < ADVANCED_START_ADDRESS + ADV_PARA_SIZE; i++)
    {
        Para[i].val = getVirtualBuf(i);
    }
}


/**
 * @brief Virtual preset 버퍼에 값 저장
 * 
 * @param address : 저장하려는 주소
 * @param value   : 저장하려는 값
 */
void setVirtualBuf(uint16_t address, uint16_t value)
{
    // Virtual preset의 범위 확인
    if(address >= FASTENING_START_ADDRESS && address < FASTENING_START_ADDRESS + PRESET_SIZE)
        vPreset.fastenBackUp[address - FASTENING_START_ADDRESS] = value;
    else if(address >= ADVANCED_START_ADDRESS && address < ADVANCED_START_ADDRESS + ADV_PARA_SIZE)
        vPreset.advancedBackUp[address - ADVANCED_START_ADDRESS] = value;
}


/**
 * @brief Virtual preset 버퍼에서 값 찾기
 * 
 * @param address : 확인 하고자 하는 주소
 * @return uint16_t (Virtual preset의 값 또는 0)
 */
uint16_t getVirtualBuf(uint16_t address)
{
    uint16_t retval = 0;

    // Virtual preset의 범위 확인
    if(address >= FASTENING_START_ADDRESS && address < FASTENING_START_ADDRESS + PRESET_SIZE)
        retval = vPreset.fastenBackUp[address - FASTENING_START_ADDRESS];
    else if(address >= ADVANCED_START_ADDRESS && address < ADVANCED_START_ADDRESS + ADV_PARA_SIZE)
        retval = vPreset.advancedBackUp[address - ADVANCED_START_ADDRESS];

    return retval;
}
