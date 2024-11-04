/**
 * @file Modbus multi write 기능 지원
 * 
 * @note 모델 별로 예외 파라미터 처리 필요.
 */

/* Includes ------------------------------------------------------------------*/
#include "modbusMultiWrite.h"
#include "common.h"
#include "variable.h"
#include "modbus.h"
#include "usbd_cdc_if.h"
#include "backup.h"
#include "wifiTask.h"    
#include "main_bg.h"
#include "virtualPreset.h"
#include "socketControl.h"

/* Variables ------------------------------------------------------------------*/
ParaSaveSequenceTypedef saveParaStr;
ParaSaveSequenceTypedef saveSettingStr;


/* Functions ------------------------------------------------------------------*/

/* Modbus 관련 ----------------------------------------------------------------------------------*/

/**
 * @brief Function code 16 Multi write의 메시지가 왔는지 판별
 * 
 * @param buf    : Data를 받은 버퍼
 * @param length : Data 받은 길이
 * @return  -1: 에러, 1: 판별 완료
 */
int checkFunctionCode16RTU(uint8_t* buf, uint16_t* length)
{
    int retval = -1;
    uint16_t Chksum_a;
    uint8_t crc_data[2];

    // Function code: 16 (MODBUS RTU)
    if (buf[1] == 0x10)
    {
        // Data Length check (Write 개수 123개 초과)
        if(((buf[4] << 8) + buf[5]) > MAX_PARA_MULTI_SAVE_SIZE)
        {
            mbSendErrorCode(0x0E, 1);       // Data range error
            *length = 0;
            retval = -1;
        }

        // Wait receive complete (Data의 개수 + Header 크기)
        if(*length >= ((buf[4] << 8) + buf[5]) * 2 + MODBUS_MULTI_RTU_HEADER_SIZE)
        {
            // Make CRC
            Chksum_a = mbGetCRC(buf, *length);
            crc_data[1] = (uint8_t)((Chksum_a >> 8) & 0xFF);
            crc_data[0] = (uint8_t)(Chksum_a & 0xFF);

            // Check CRC
            if ((crc_data[0] == buf[*length - 2]) && (crc_data[1] == buf[*length - 1]))
            {
                // Modbus message receive
                *length = 0;
                retval = 1;
                Flag.ReceiveModBusData = ON;
            }
            else
            {
                *length = 0;
                retval = -1;
                mbSendErrorCode(0x07, 1);       // crc error
            }
        }
    }
    
    return retval;
}


/**
 * @brief Modbus function code 16 메시지인지 판별
 * 
 * @param buf    : Data가 있는 buf
 * @param length : Data의 길이
 * @return  -1: 에러, 1: 판별 완료
 */
int checkFunctionCode16TCP(uint8_t* buf, uint16_t* length)
{
    int retval = -1;

    // Function code: 16 (MODBUS TCP)
    if(buf[7] == 0x10)
    {
        // Data Length check (Write 개수 123개 초과)
        if((buf[10] << 8) + buf[11] > MAX_PARA_MULTI_SAVE_SIZE)
        {
            mbSendErrorCode(0x0E, 1);       // Data range error
            *length = 0;
            retval = -1;
        }

        // Wait receive complete (Data의 개수 + Header 크기)
        if(*length >= ((buf[10] << 8) + buf[11]) * 2 + MODBUS_MULTI_TCP_HEADER_SIZE)
        {
            retval = ON;

            *length = 0;
            FlagFasten.etReceiveModBusData = ON;
        }
    }

    return retval;
}


/**
 * @brief Modbus function code 16 처리
 * 
 * @param type : 1 (Serial) , 2 (Ethernet) 
 */
void mbProcess_16(int type, uint8_t* buf)
{
	uint8_t  error = 0;
    ModbusCommTypedef modbus;

    // RS232
	if (type == 1)		
	{
		modbus.StartAddress = (((uint16_t)buf[2] << 8) & 0xff00) + (buf[3] & 0xff);        // Parameter Address
		modbus.CntRegister  = (((uint16_t)buf[4] << 8) & 0xff00) + (buf[5] & 0xff);		   // data count
        modbus.DataStartPoint = MODBUS_RTU_FRONT_HEADER_SIZE;                              // Data 시작 주소
        modbus.DataPointer = buf;                                                          // Serial data pointer
	}
    // Ethernet
	else if (type == 2)	
	{
		modbus.StartAddress = (((uint16_t)buf[2+6] << 8) & 0xff00) + (buf[3+6] & 0xff);    // Parameter address
		modbus.CntRegister  = (((uint16_t)buf[4+6] << 8) & 0xff00) + (buf[5+6] & 0xff);	   // data count
        modbus.DataStartPoint = MODBUS_TCP_FRONT_HEADER_SIZE;                              // Data 시작 주소
        modbus.DataPointer = buf;                                                          // Ethernet data pointer
	}
    else
        return;

    // parameter address
	if (modbus.StartAddress <= CONTROLLER_PARA_END_ADDRESS)	
	{
        // Wirte 주소 확인
		if ((modbus.StartAddress > 0) && ((modbus.StartAddress + modbus.CntRegister) <= CONTROLLER_PARA_END_ADDRESS + 1))
		{
            // Check data length
            if(modbus.CntRegister > MAX_PARA_MULTI_SAVE_SIZE)
                error = ON;

            // 저장 중인 것 확인
            if(saveParaStr.saveStart == OFF && error == OFF)
            {
                // Make parameter value list
                for(uint16_t i=0; i < modbus.CntRegister; i++)
                {
                    saveParaStr.paraBuf[i] = (modbus.DataPointer[modbus.DataStartPoint + i * 2] << 8) + modbus.DataPointer[modbus.DataStartPoint + i * 2 + 1];
                }

                // Parameter save start setting
                saveParaStr.StartAddress = modbus.StartAddress;
                saveParaStr.CntRegister = modbus.CntRegister;
                saveParaStr.type = type;
                saveParaStr.socketNumber = socketControl.lastCommSocket;
                saveParaStr.saveStart = ON;
                saveParaStr.index = 0;
                saveParaStr.noSaveFlag = OFF;
                
                memcpy(saveParaStr.messageHeader, buf, MODBUS_TCP_FRONT_HEADER_SIZE);   // TCP가 사이즈가 커서 TCP로 SIZE로 사용
            }
            else
            {
                mbSendErrorCode(0x0E, 1);       // Data range error (저장 중이라 Error 전송)
            }
		}
        else
            mbSendErrorCode(0x02, 1);           // Address range error
	}
    else
        mbSendErrorCode(0x02, 1);                   // Address range error
}


/**
 * @brief Multi write의 응답
 * 
 * @param type : 1 - 시리얼, 2 - 이더넷
 * @param buf  : 응답 데이터를 가지고 있는 Buffer pointer
 */
void mbSendReturnData_16(int type, uint8_t* buf)
{
	uint8_t i;
    uint16_t j = 0;

	if (type == 1)		
	{
        // RS232
        // Make CRC
        j = mbGetCRC(buf, 6 + 2);
        buf[6] = j & 0xFF;
        buf[7] = (j >> 8) & 0xFF;

        // Send
        i = CDC_Transmit_FS((uint8_t*)buf, 8);
        
		if (i != 0)
		{
			system_err = USB_COMMUNICATION_FAIL;
		}
	}
	else if (type == 2)	
	{
        // Ethernet
        // Length 수정
        buf[4] = 0x00;
        buf[5] = 0x06;

		tcpSendData(socketControl.lastCommSocket, (uint8_t*)buf, 12);
	}
}




/* Parameter 저장 관련 ----------------------------------------------------------------------------------*/

/**
 * @brief 파라미터를 1개씩 순차적으로 저장
 * 
 * @note
 * 1. 최대 123개씩
 * 2. 2ms 간격으로 저장
 * 3. Modbus multi write에서 사용
 */
void paraMultiSaveSequential(void)
{
    static uint32_t timeout;
    int8_t retval = 0;

    // Check start
    if(saveParaStr.saveStart)
    {
        // Check para count
        if(saveParaStr.CntRegister > MAX_PARA_MULTI_SAVE_SIZE)
        {
            saveParaStr.saveStart = OFF;
            mbSendErrorCode(0x0E, saveParaStr.type);
            return;
        }

        // Check start address
        if(saveParaStr.StartAddress + saveParaStr.CntRegister > CONTROLLER_PARA_END_ADDRESS + 1)
        {
            saveParaStr.saveStart = OFF;
            mbSendErrorCode(0x0E, saveParaStr.type);
            return;
        }

        // 초기 시작 값 설정
        if(saveParaStr.index == 0)
        {
            timeout = 0;
        }

        // Check save loop time (1ms 이상)
        if(HAL_GetTick() != timeout && HAL_GetTick() > 2)
        {
            // Function code 6 기능을 1개씩 실행
            retval = mbMakeandSend_06(saveParaStr.StartAddress + saveParaStr.index, saveParaStr.paraBuf[saveParaStr.index], saveParaStr.type);

            // Index 증가
            saveParaStr.index++;
            timeout = HAL_GetTick();

            // Check Error
            if(retval == -1)
            {
                saveParaStr.saveStart = OFF;

                changeLastSocket(saveParaStr.socketNumber, ON);
                mbSendErrorCode(0x0E, saveParaStr.type);		// data range error
                changeLastSocket(saveParaStr.socketNumber, OFF);
            }
        }

        // Check Complete (정상 메시지 전송)
        if(saveParaStr.index == saveParaStr.CntRegister)
        {
            changeLastSocket(saveParaStr.socketNumber, ON);
            mbSendReturnData_16(saveParaStr.type, saveParaStr.messageHeader);
            changeLastSocket(saveParaStr.socketNumber, OFF);
            saveParaStr.saveStart = OFF;
        }
    }
}


