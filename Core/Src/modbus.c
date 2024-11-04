/**
  ******************************************************************************
  * @file    modbus.c
  * @brief   modbus Service Routines.
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "main.h"
#include "common.h"
#include <stdlib.h>
#include "variable.h"
#include "wifiTask.h"
#include "velocity.h"
#include "main_bg.h"
#include "modbus.h"
#include "backup.h"
#include "modbusMultiWrite.h"
#include "advancedPlus.h"
#include "socketControl.h"
#include "eventGraphTask.h"
#include "eventRealtimeTask.h"
#include "virtualPreset.h"
#include "totalScrewCount.h"
#include "barcode.h"
#include "RTC.h"
#include "realtimeMonitoring.h"
#include "graphMonitoring.h"
#include "autoCustomizing.h"
#include "multicastLudp.h"


/* Externs ------------------------------------------------------------------*/
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);
extern MultiAckTypdef       multiAck;               // Multicast Ack data struct 
extern NetworkTypeDef    	WifiEthSet;             // Ethernet Config: IP, Gateway, Netmask


/* Functions ------------------------------------------------------------------*/
void Modbus_RTU_Process(int type)
{
	uint8_t data=0;

	if (type == MODBUS_RS232_TYPE)
		data = ComReadBufferMODBUS[1];		// RS232
	else if (type == MODBUS_ETHERNET_TYPE)
		data = etReadBuffer[1+6];			// Ethernet


	if (data == 0x03)		
	{
		// Read parameter
		mbProcess_03(type);
	}
	else if (data == 0x04)	
	{
		// Read monitoring data
		mbProcess_04(type);
	}	
	else if(data == 0x06)	
	{
		// Save single parameter
		mbProcess_06(type);
	}
	else if (data == 0x11)	
	{
		// Send device ID 
		mbProcess_17(type);
	}
	else if (data == 0x10)	
	{
		// Modbus Multi write
		if(type == MODBUS_RS232_TYPE)
			mbProcess_16(type, ComReadBufferMODBUS);
		else
			mbProcess_16(type, etReadBuffer);
	}
}




void mbProcess_03(int type)
{
	uint16_t StartAddress=0, CntRegister=0;

	if (type == MODBUS_RS232_TYPE)		// RS232
	{
		StartAddress = (((uint16_t)ComReadBufferMODBUS[2] << 8) & 0xff00) + (ComReadBufferMODBUS[3] & 0xff);
		CntRegister  = (((uint16_t)ComReadBufferMODBUS[4] << 8) & 0xff00) + (ComReadBufferMODBUS[5] & 0xff);		// data count
	}
	else if (type == MODBUS_ETHERNET_TYPE)	// Ethernet
	{
		StartAddress = (((uint16_t)etReadBuffer[2+6] << 8) & 0xff00) + (etReadBuffer[3+6] & 0xff);
		CntRegister  = (((uint16_t)etReadBuffer[4+6] << 8) & 0xff00) + (etReadBuffer[5+6] & 0xff);		// data count
	}


	// Check Parameter address register
	if (StartAddress <= PARA_END_MAX_ADDRESS)
	{
		// Value address
		if ((StartAddress + CntRegister) <= PARA_END_MAX_ADDRESS + 1)
		{
			mbMakeandSend_03(StartAddress, CntRegister, type);
		}
		else
		{
			mbSendErrorCode(0x02, type);		// Address error
		}
	}
	else
		mbSendErrorCode(0x02, type);			// Address error
}


void mbProcess_04(int type)
{
	uint16_t StartAddress=0, CntRegister=0;

	if (type == MODBUS_RS232_TYPE)	// RS232
	{
		StartAddress = (((uint16_t)ComReadBufferMODBUS[2] << 8) & 0xff00) + (ComReadBufferMODBUS[3] & 0xff);
		CntRegister  = (((uint16_t)ComReadBufferMODBUS[4] << 8) & 0xff00) + (ComReadBufferMODBUS[5] & 0xff);	// data count
	}
	else if (type == MODBUS_ETHERNET_TYPE)	// Ethernet
	{
		StartAddress = (((uint16_t)etReadBuffer[2+6] << 8) & 0xff00) + (etReadBuffer[3+6] & 0xff);
		CntRegister  = (((uint16_t)etReadBuffer[4+6] << 8) & 0xff00) + (etReadBuffer[5+6] & 0xff);		// data count
	}

	
	// Check address
	if (StartAddress + CntRegister <= 9999)
	{
		// 모니터링 데이터
		mbMakeandSend_04(StartAddress, CntRegister, type);
	}
	else
	{
		mbSendErrorCode(0x02, type);		// data address error
	}
}


void mbProcess_06(int type)
{
	uint16_t StartAddress=0;
	uint16_t p_value=0;
	int8_t retval = 0;

	if (type == MODBUS_RS232_TYPE)	   // RS232
	{
		StartAddress = (((uint16_t)ComReadBufferMODBUS[2] << 8) & 0xff00) + (ComReadBufferMODBUS[3] & 0xff);
		p_value 	 = (((uint16_t)ComReadBufferMODBUS[4] << 8) & 0xff00) + (ComReadBufferMODBUS[5] & 0xff);	// data value
	}
	else if (type == MODBUS_ETHERNET_TYPE)	// Ethernet
	{
		StartAddress = (((uint16_t)etReadBuffer[2+6] << 8) & 0xff00) + (etReadBuffer[3+6] & 0xff);
		p_value 	 = (((uint16_t)etReadBuffer[4+6] << 8) & 0xff00) + (etReadBuffer[5+6] & 0xff);	// data value
	}


	// Check address
	if (StartAddress <= CONTROLLER_PARA_END_ADDRESS)
	{
		retval = mbMakeandSend_06(StartAddress, p_value, type);

		// Check error
		if(retval >= 0)
			mbSendReturnData_06(type);	
		else
			mbSendErrorCode(0x0E, type);		// data range error
	}
	else
		mbSendErrorCode(0x02, type);			// Address error
}


void mbMakeandSend_03(uint16_t st_adr, uint16_t cnt_reg, int type)
{
	uint16_t i,j;
	int pCnt = 0;
	uint8_t* pointerData;
	uint16_t pData=0;
	uint16_t address;

	pCnt = 0;

	if (type == MODBUS_RS232_TYPE) 
		MakePacket_T[pCnt++] = 01;	    	// ID - RS232 			Para[DRIVER_ID].val
	else if (type == MODBUS_ETHERNET_TYPE) 
		MakePacket_T[pCnt++] = 0x01;		// ID - Ethernet

	MakePacket_T[pCnt++] = 0x03;			// function code
	MakePacket_T[pCnt++] = cnt_reg * 2;		// data byte 수

	// Make data
	for (i=0; i < cnt_reg; i++)
	{
		pData = 0;
		address = st_adr + i;

		// Check address range
		if(address <= PARA_END_VAL_ADDRESS)
		{
			pData = getModbusPara(address);
		}
		else if(address <= PARA_END_MIN_ADDRESS && address >= PARA_START_MIN_ADDRESS)
		{
			pData = getModbusPara(address);
		}
		else if(address <= PARA_END_MAX_ADDRESS && address >= PARA_START_MAX_ADDRESS)
		{
			pData = getModbusPara(address);
		}
		else if(address <= REMOTE_END_ADDRESS)
		{
			pData = getModbusRemoteControl(address);
		}
		else if(address <= AUTO_CUSTOM_END_ADDRESS)
		{
			#if MODBUS_AUTO_CUSTOMIZING_ACTIVE
			pData = getModbusAutoCustom(address);
			#endif
		}
		else if(address <= SENSOR_CAL_END_ADDRESS)
		{
			#if MODBUS_SENSOR_CALIBRATION_ACTIVE
			pData = getModbusSensor(address);
			#endif
		}
		else if(address <= VIRTUAL_PRESET_END_ADDRESS)
		{
			#if MODBUS_VIRTUAL_PRESET_ACTIVE
			pData = getModbusVirtualPreset(address);
			#endif
		}
		else if(address <= VIRTUAL_ADVANCE_END_ADDRESS)
		{
			#if MODBUS_VIRTUAL_ADVANCED_ACTIVE
			pData = getModbusVirtualAdvanced(address);
			#endif
		}
		else if(address <= VIRTUAL_MODEL_END_ADDRESS)
		{
			#if MODBUS_VIRTUAL_MODEL_ACTIVE
			pData = getModbusVirtualModel(address);
			#endif
		}
		else if(address <= REALTIME_EVENT_END_ADDRESS)
		{
			#if MODBUS_REALTIME_EVENT_ACTIVE
			pData = getModbusRealtimeEvent(address, type);
			#endif
		}
		else if(address <= GRAPH_MONITORING_SETTING_END_ADDRESS)
		{
			#if MODBUS_GRAPH_MONITORING_ACTIVE 
			pData = getModbusGraphMonitoringSetting(address, type);
			#endif
		}
		else if(address <= GRAPH_EVENT_END_ADDRESS)
		{
			#if MODBUS_GRAPH_EVENT_ACTIVE
			pData = getModbusGraphEvent(address, type);
			#endif
		}
		else if(address <= BARCODE_END_ADDRESS)
		{
			#if PRESET_BARCODE_ACTIVE
			pData = getModbusPresetBarcode(address);
			#endif
		}
		else if(address <= MODEL_BARCODE_END_ADDRESS)
		{
			#if MODEL_BARCODE_ACTIVE
			pData = getModbusModelBarcode(address);
			#endif
		}
		else if(address <= MAINTENANCE_END_ADDRESS)
		{
			#if MODBUS_MAINTENACNCE_ACTIVE
			pData = getModbusMaintenance(address);
			#endif
		}
		else if(address <= DRIVER_REG_END_ADDRESS)
		{
			pData = getModbusDriver(address);
		}
		else if(address <= CONTROLLER_PARA_END_ADDRESS)
		{
			#if MODBUS_CONTROLLER_PARA_ACTIVE
			pData = getModbusControllerPara(address);
			#endif
		}
		else
			pData = 0;

		// Pointer 주소 할당
		pointerData = (uint8_t*)&pData;

		// Big endian으로 값 설정 (pointer 주소 이용)
		MakePacket_T[pCnt++] = pointerData[1];
		MakePacket_T[pCnt++] = pointerData[0];
	}

	// Make CRC
	j = mbGetCRC(MakePacket_T, pCnt+2);
	MakePacket_T[pCnt++] = j & 0xff;
	MakePacket_T[pCnt++] = (j >> 8) & 0xff;

	if (type == MODBUS_RS232_TYPE)		
	{
		// RS232
		CDC_Transmit_FS((uint8_t*)MakePacket_T, pCnt);
	}
	else if (type == MODBUS_ETHERNET_TYPE)	
	{
		// Ethernet
		j = 0;

		// Make Header
		mbMakeET_MBAP();
		for(i = 0; i < pCnt; i++)
		{
			gSENDDATABUF[i+6] = MakePacket_T[i];
		}	

		gSendDataLen = pCnt - 2;	// CRC  제외.
		gSENDDATABUF[4] = (uint8_t)(gSendDataLen >> 8);
		gSENDDATABUF[5] = (uint8_t)gSendDataLen;
		gSendDataLen += 6; 			// add header length
		tcpSendData(socketControl.lastCommSocket, gSENDDATABUF, gSendDataLen);
	}
}


uint32_t mbMakeandSend_04(uint16_t st_adr, uint16_t cnt_reg, int type)
{
	uint16_t i,j;
	uint16_t pCnt;
	uint16_t pData=0;
	uint8_t* pointerData;
	uint32_t result=0;
	uint16_t address;

	pCnt = 0;

	if (type == MODBUS_RS232_TYPE) 
		MakePacket_T[pCnt++] = 01;	    	// ID - RS232
	else if (type == MODBUS_ETHERNET_TYPE) 
		MakePacket_T[pCnt++] = 0x01;		// ID - Ethernet

	MakePacket_T[pCnt++] = 0x04;			// function code
	MakePacket_T[pCnt++] = cnt_reg * 2;		// data byte 수
	

	// Make data
	for (i = 0; i < cnt_reg; i++)
	{
		pData = 0;
		address = st_adr + i;

		if(address <= INFORMATION_END_ADDRESS)
		{
			// Driver information
			pData = getModbusInformation(address);
		}
		else if(address <= CURRENT_STATE_END_ADDRESS)
		{
			// Current state
			#if MODBUS_CURRENT_STATE_MONITORING_ACTIVE
			pData = getModbusCurrentStateMonitoring(address);
			#endif
		}
		else if(address <= GRAPH_MONI_END_ADDRESS)
		{
			// Graph monitoring
			#if MODBUS_GRAPH_MONITORING_ACTIVE
			pData = getModbusGraphMonitoring(address);
			#endif
		}
		else if(address <= ERROR_END_ADDRESS)
		{
			// Error code monitoring
			#if MODBUS_ERROR_CODE_ACTIVE
			pData = getModbusErrorCode(address);
			#endif
		}
		else if(address <= FIELDBUS_STATE_END_ADDRESS)
		{
			// Fieldbus state
			#if MODBUS_FIELDBUS_ACTIVE
			pData = getModbusFieldbus(address);
			#endif
		}

		// Pointer 주소 할당
		pointerData = (uint8_t*)&pData;

		// Big endian으로 값 설정 (pointer 주소 이용)
		MakePacket_T[pCnt++] = pointerData[1];
		MakePacket_T[pCnt++] = pointerData[0];
	}

	j = mbGetCRC(MakePacket_T, pCnt+2);
	MakePacket_T[pCnt++] = j & 0xff;
	MakePacket_T[pCnt++] = (j >> 8) & 0xff;

	if (type == MODBUS_RS232_TYPE)		
	{
		// RS232
		CDC_Transmit_FS((uint8_t*)MakePacket_T, pCnt);
	}
	else if (type == MODBUS_ETHERNET_TYPE)	
	{
		// Ethernet
		j = 0;

		// Make Header
		mbMakeET_MBAP();
		for(i = 0; i < pCnt; i++)
		{
			gSENDDATABUF[i+6] = MakePacket_T[i];
		}	

		gSendDataLen = pCnt - 2;	// CRC  제외.
		gSENDDATABUF[4] = (uint8_t)(gSendDataLen >> 8);
		gSENDDATABUF[5] = (uint8_t)gSendDataLen;
		gSendDataLen += 6; 			// add header length
		tcpSendData(socketControl.lastCommSocket, gSENDDATABUF, gSendDataLen);
	}

	return result;
}


int8_t mbMakeandSend_06(uint16_t st_adr, uint16_t data, int type)
{
	int8_t result=0;

	// Check address range
	if(st_adr == 4500)
	{
		// 펌웨어 업데이트 명령 주소
		st_adr = FIRMWARE_UPDATE_START;
		result = setModbusRemoteControl(st_adr, data);
	}
	else if(st_adr <= PARA_END_VAL_ADDRESS)
	{
		result = setModbusPara(st_adr, data);
	}
	else if(st_adr <= PARA_END_MIN_ADDRESS && st_adr >= PARA_START_MIN_ADDRESS)
	{
		result = setModbusPara(st_adr, data);
	}
	else if(st_adr <= PARA_END_MAX_ADDRESS && st_adr >= PARA_START_MAX_ADDRESS)
	{
		result = setModbusPara(st_adr, data);
	}
	else if(st_adr <= REMOTE_END_ADDRESS)
	{
		result = setModbusRemoteControl(st_adr, data);
	}
	else if(st_adr <= AUTO_CUSTOM_END_ADDRESS)
	{
		#if MODBUS_AUTO_CUSTOMIZING_ACTIVE
		result = setModbusAutoCustom(st_adr, data);
		#endif
	}
	else if(st_adr <= SENSOR_CAL_END_ADDRESS)
	{
		#if MODBUS_SENSOR_CALIBRATION_ACTIVE
		result = setModbusSensor(st_adr, data);
		#endif
	}
	else if(st_adr <= BIT_SOCKET_COMM_END_ADDRESS)
	{
		#if MODBUS_BIT_SOCKET_TRAY_COMM_ACTIVE
		result = setModbusBitSocketTrayComm(st_adr, data, type);
		#endif
	}
	else if(st_adr <= VIRTUAL_PRESET_END_ADDRESS)
	{
		#if MODBUS_VIRTUAL_PRESET_ACTIVE
		result = setModbusVirtualPreset(st_adr, data);
		#endif
	}
	else if(st_adr <= VIRTUAL_ADVANCE_END_ADDRESS)
	{
		#if MODBUS_VIRTUAL_ADVANCED_ACTIVE
		result = setModbusVirtualAdvanced(st_adr, data);
		#endif
	}
	else if(st_adr <= VIRTUAL_MODEL_END_ADDRESS)
	{
		#if MODBUS_VIRTUAL_MODEL_ACTIVE
		result = setModbusVirtualModel(st_adr, data);
		#endif
	}
	else if(st_adr <= REALTIME_EVENT_END_ADDRESS)
	{
		#if MODBUS_REALTIME_EVENT_ACTIVE
		result = setModbusRealtimeEvent(st_adr, data, type);
		#endif
	}
	else if(st_adr <= GRAPH_MONITORING_SETTING_END_ADDRESS)
	{
		#if MODBUS_GRAPH_MONITORING_ACTIVE 
		result = setModbusGraphMonitoringSetting(st_adr, data, type);
		#endif
	}
	else if(st_adr <= GRAPH_EVENT_END_ADDRESS)
	{
		#if MODBUS_GRAPH_EVENT_ACTIVE
		result = setModbusGraphEvent(st_adr, data, type);
		#endif
	}
	else if(st_adr <= BARCODE_END_ADDRESS)
	{
		#if PRESET_BARCODE_ACTIVE
		result = setModbusPresetBarcode(st_adr, data);
		#endif
	}
	else if(st_adr <= MODEL_BARCODE_END_ADDRESS)
	{
		#if MODEL_BARCODE_ACTIVE
		result = setModbusModelBarcode(st_adr, data);
		#endif
	}
	else if(st_adr <= MAINTENANCE_END_ADDRESS)
	{
		#if MODBUS_MAINTENACNCE_ACTIVE
		result = setModbusMaintenance(st_adr, data);
		#endif
	}
	else if(st_adr <= DRIVER_REG_END_ADDRESS)
	{
		result = setModbusDriver(st_adr, data);
	}
	else if(st_adr <= CONTROLLER_PARA_END_ADDRESS)
	{
		#if MODBUS_CONTROLLER_PARA_ACTIVE
		result = setModbusControllerPara(st_adr, data);
		#endif
	}
	else
		result = -1;


	return result;
}


void mbProcess_17(int type)
{
	uint16_t i,j;
	uint16_t pCnt;

	pCnt = 0;
	MakePacket_T[pCnt++] = 01;						// Slave address
	MakePacket_T[pCnt++] = 0x11;					// function
	MakePacket_T[pCnt++] = 17;						// data 수..

	MakePacket_T[pCnt++] = 0;						// ID
	MakePacket_T[pCnt++] = 01;

	MakePacket_T[pCnt++] = (controllerPara.model >> 8) & 0xff;	// controller model
	MakePacket_T[pCnt++] = controllerPara.model & 0xff;

	MakePacket_T[pCnt++] = (dset[4] >> 8) & 0xff;	// driver model, send real driver data
	MakePacket_T[pCnt++] = dset[4] & 0xff;
	
	MakePacket_T[pCnt++] = ((controllerPara.firmwareVersion[0] * 1000 + controllerPara.firmwareVersion[1] * 10 + controllerPara.firmwareVersion[2]) >> 8) & 0xff;	// software version
	MakePacket_T[pCnt++] = (controllerPara.firmwareVersion[0] * 1000 + controllerPara.firmwareVersion[1] * 10 + controllerPara.firmwareVersion[2]) & 0xff;

	MakePacket_T[pCnt++] = dset[8] & 0xff;	// driver sn 	(long)dset[8] << 24;
	MakePacket_T[pCnt++] = dset[7] & 0xff;	// 				(long)dset[7] << 16;
	MakePacket_T[pCnt++] = PRODUCT_CODE & 0xff;		// 		제품 코드 (19: BMT)
	MakePacket_T[pCnt++] = dset[6] & 0xff;	// 				(long)dset[6] << 8;
	MakePacket_T[pCnt++] = dset[5] & 0xff;	// 				(long)dset[5]

	MakePacket_T[pCnt++] = (getTotalScrewCount(TOTAL_SCREW_ADDRESS) >> 8) & 0xff;	
	MakePacket_T[pCnt++] = (getTotalScrewCount(TOTAL_SCREW_ADDRESS) >> 0) & 0xff;
	MakePacket_T[pCnt++] = (getTotalScrewCount(TOTAL_SCREW_ADDRESS + 1) >> 8)  & 0xff;
	MakePacket_T[pCnt++] = getTotalScrewCount(TOTAL_SCREW_ADDRESS + 1) & 0xff;

	j = mbGetCRC(MakePacket_T, pCnt+2);
	MakePacket_T[pCnt++] = j & 0xff;
	MakePacket_T[pCnt++] = (j >> 8) & 0xff;

	if (type == MODBUS_RS232_TYPE)		 // RS232
	{
		CDC_Transmit_FS((uint8_t*)MakePacket_T, pCnt);
	}
	else if (type == MODBUS_ETHERNET_TYPE)	 // Ethernet
	{
		mbMakeET_MBAP();
		MakePacket_T[0] = 0;	// ID
		for(i=0; i < pCnt; i++)	// 받은 값을 다시 보낸다.
		{
			gSENDDATABUF[i+6] = MakePacket_T[i];
		}	

		gSendDataLen = pCnt - 2;	// CRC 제외..
		gSENDDATABUF[4] = (uint8_t)(gSendDataLen >> 8);
		gSENDDATABUF[5] = (uint8_t)gSendDataLen;
		gSendDataLen += 6; 			// add header length
		tcpSendData(socketControl.lastCommSocket, gSENDDATABUF, gSendDataLen);
	}
}


uint32_t mbMakeandSend_64(uint16_t st_adr, uint16_t cnt_reg, int type)
{
	uint16_t i,j;
	uint16_t pCnt;
	uint16_t pData=0;
	uint8_t* pointerData;
	uint32_t result=0;
	uint16_t address;

	pCnt = 0;

	if (type == MODBUS_RS232_TYPE) 
		MakePacket_T[pCnt++] = 01;	    				// ID - RS232
	else if (type == MODBUS_ETHERNET_TYPE) 
		MakePacket_T[pCnt++] = 0x01;					// ID - Ethernet

	MakePacket_T[pCnt++] = 0x64;						// function code
	MakePacket_T[pCnt++] = (cnt_reg * 2) >> 8;			// data byte 수 High
	MakePacket_T[pCnt++] = (cnt_reg * 2) & 0xFF;		// data byte 수 Low
	

	// Make data
	for (i = 0; i < cnt_reg; i++)
	{
		pData = 0;
		address = st_adr + i;

		if(address <= GRAPH_DATA_CAHNNEL2_1999)
		{
			// Graph monitoring
			#if MODBUS_GRAPH_MONITORING_ACTIVE

			// Graph의 Header
			if(st_adr >= GRAPH_DATA_CAHNNEL1_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE && address < GRAPH_DATA_CAHNNEL1_1)
			{
				// 채널의 헤더 (Channel 1)
				if(address == GRAPH_DATA_CAHNNEL1_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE && i == 0)
					pData = GRAPH_TRIGGER_CHANNEL1;
				else if(address == GRAPH_DATA_CAHNNEL1_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE + 1 && i == 1)
					pData = graphMonLastData.setting.channel1Count;
			}
			else if(st_adr >= GRAPH_DATA_CAHNNEL2_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE && address < GRAPH_DATA_CAHNNEL2_1)
			{
				// Channel 2
				if(address == GRAPH_DATA_CAHNNEL2_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE && i == 0)
					pData = GRAPH_TRIGGER_CHANNEL2;
				else if(address == GRAPH_DATA_CAHNNEL2_1 - GRAPH_TRIGGER_HEADER_ADDR_SIZE + 1 && i == 1)
					pData = graphMonLastData.setting.channel2Count;
			}
			else
				// Graph data
				pData = getModbusGraphMonitoring(address);

			#endif
		}

		// Pointer 주소 할당
		pointerData = (uint8_t*)&pData;

		// Big endian으로 값 설정 (pointer 주소 이용)
		MakePacket_T[pCnt++] = pointerData[1];
		MakePacket_T[pCnt++] = pointerData[0];
	}

	j = mbGetCRC(MakePacket_T, pCnt+2);
	MakePacket_T[pCnt++] = j & 0xff;
	MakePacket_T[pCnt++] = (j >> 8) & 0xff;

	if (type == MODBUS_RS232_TYPE)		
	{
		// RS232
		CDC_Transmit_FS((uint8_t*)MakePacket_T, pCnt);
	}
	else if (type == MODBUS_ETHERNET_TYPE)	
	{
		// Ethernet
		j = 0;

		// Make Header
		mbMakeET_MBAP();
		for(i = 0; i < pCnt; i++)
		{
			gSENDDATABUF[i+6] = MakePacket_T[i];
		}	

		gSendDataLen = pCnt - 2;	// CRC  제외.
		gSENDDATABUF[4] = (uint8_t)(gSendDataLen >> 8);
		gSENDDATABUF[5] = (uint8_t)gSendDataLen;
		gSendDataLen += 6; 			// add header length

		tcpSendData(socketControl.lastCommSocket, gSENDDATABUF, gSendDataLen);
	}

	return result;
}


uint32_t mbMakeandSend_65(uint16_t st_adr, uint16_t cnt_reg, int type)
{
	uint16_t i,j;
	uint16_t pCnt;
	uint16_t pData=0;
	uint8_t* pointerData;
	uint32_t result=0;
	uint16_t address;

	pCnt = 0;

	if (type == MODBUS_RS232_TYPE) 
		MakePacket_T[pCnt++] = 01;	    				// ID - RS232
	else if (type == MODBUS_ETHERNET_TYPE) 
		MakePacket_T[pCnt++] = 0x01;					// ID - Ethernet

	MakePacket_T[pCnt++] = 0x65;						// function code
	MakePacket_T[pCnt++] = (cnt_reg * 2) >> 8;			// data byte 수 High
	MakePacket_T[pCnt++] = (cnt_reg * 2) & 0xFF;		// data byte 수 Low
	

	// Make data
	for (i = 0; i < cnt_reg; i++)
	{
		pData = 0;
		address = st_adr + i;

		if(address <= GRAPH_REALTIME_END_ADDRESS)
		{
			// Graph monitoring
			#if MODBUS_GRAPH_MONITORING_ACTIVE

			pData = graphMonLastData.realtimeData[address - GRAPH_REALTIME_START_ADDRESS];
			
			#endif
		}

		// Pointer 주소 할당
		pointerData = (uint8_t*)&pData;

		// Big endian으로 값 설정 (pointer 주소 이용)
		MakePacket_T[pCnt++] = pointerData[1];
		MakePacket_T[pCnt++] = pointerData[0];
	}

	j = mbGetCRC(MakePacket_T, pCnt+2);
	MakePacket_T[pCnt++] = j & 0xff;
	MakePacket_T[pCnt++] = (j >> 8) & 0xff;

	if (type == MODBUS_RS232_TYPE)		
	{
		// RS232
		CDC_Transmit_FS((uint8_t*)MakePacket_T, pCnt);
	}
	else if (type == MODBUS_ETHERNET_TYPE)	
	{
		// Ethernet
		j = 0;

		// Make Header
		mbMakeET_MBAP();
		for(i = 0; i < pCnt; i++)
		{
			gSENDDATABUF[i+6] = MakePacket_T[i];
		}	

		gSendDataLen = pCnt - 2;	// CRC  제외.
		gSENDDATABUF[4] = (uint8_t)(gSendDataLen >> 8);
		gSENDDATABUF[5] = (uint8_t)gSendDataLen;
		gSendDataLen += 6; 			// add header length
		tcpSendData(socketControl.lastCommSocket, gSENDDATABUF, gSendDataLen);
	}

	return result;
}


uint32_t mbMakeandSend_eventMonitoring(uint16_t st_adr, uint16_t cnt_reg, int type)
{
	uint16_t i,j;
	uint16_t pCnt;
	uint16_t pData=0;
	uint8_t* pointerData;
	uint32_t result=0;
	uint16_t address;

	pCnt = 0;

	if (type == MODBUS_RS232_TYPE) 
		MakePacket_T[pCnt++] = 01;	    	// ID - RS232
	else if (type == MODBUS_ETHERNET_TYPE) 
		MakePacket_T[pCnt++] = 0x01;		// ID - Ethernet

	MakePacket_T[pCnt++] = 0x04;			// function code
	MakePacket_T[pCnt++] = cnt_reg * 2;		// data byte 수
	

	// Make data
	for (i = 0; i < cnt_reg; i++)
	{
		pData = 0;
		address = st_adr + i;

		if(address <= GRAPH_DATA_CAHNNEL2_1999)
		{
			// Graph event monitoring
			#if MODBUS_GRAPH_EVENT_ACTIVE
			pData = getModbusGraphMonitoring(address);
			#endif
		}

		// Pointer 주소 할당
		pointerData = (uint8_t*)&pData;

		// Big endian으로 값 설정 (pointer 주소 이용)
		MakePacket_T[pCnt++] = pointerData[1];
		MakePacket_T[pCnt++] = pointerData[0];
	}

	j = mbGetCRC(MakePacket_T, pCnt+2);
	MakePacket_T[pCnt++] = j & 0xff;
	MakePacket_T[pCnt++] = (j >> 8) & 0xff;

	if (type == MODBUS_RS232_TYPE)		
	{
		// RS232
		CDC_Transmit_FS((uint8_t*)MakePacket_T, pCnt);
	}
	else if (type == MODBUS_ETHERNET_TYPE)	
	{
		// Ethernet
		j = 0;

		// Make Header
		mbMakeET_MBAP();
		for(i = 0; i < pCnt; i++)
		{
			gSENDDATABUF[i+6] = MakePacket_T[i];
		}	

		gSendDataLen = pCnt - 2;	// CRC  제외.
		gSENDDATABUF[4] = (uint8_t)(gSendDataLen >> 8);
		gSENDDATABUF[5] = (uint8_t)gSendDataLen;
		gSendDataLen += 6; 			// add header length
		tcpSendData(socketControl.lastCommSocket, gSENDDATABUF, gSendDataLen);
	}

	return result;
}



/**
 * @brief Make CRC Data
 * 
 * @param data : CRC를 계산할 데이터
 * @param Length : CRC 계산할 데이터 길이
 * @return uint16_t : CRC 값
 */
uint16_t mbGetCRC(uint8_t *data, int Length)
{
	int i, j;
	uint16_t CRCFull = 0xFFFF;
	char CRCLSB;

	for (i = 0; i < Length - 2; i++)
	{
		CRCFull = (uint16_t)(CRCFull ^ data[i]);

		for (j = 0; j < 8; j++)
		{
			CRCLSB  = (char)(CRCFull & 0x0001);
			CRCFull = (uint16_t)((CRCFull >> 1) & 0x7FFF);

			if (CRCLSB == 1)
				CRCFull = (uint16_t)(CRCFull ^ 0xA001);
		}
	}

	return CRCFull;
}


/**
 * @brief MODBUS 통신 에러 응답 메시지
 * 
 * @param code : Error 코드
 * @param type : 통신 타입 (1: RS232, 2: Ethernet)
 */
void mbSendErrorCode(int code, int type)
{
	uint16_t j = 0;

	MakePacket_T[0] = 01;							 // send ID
	MakePacket_T[1] = ComReadBufferMODBUS[1] + 0x80; // Function code
	MakePacket_T[2] = code;							 // error code

	if (type == MODBUS_RS232_TYPE)
	{
		// RS232
		j = mbGetCRC(MakePacket_T, 3 + 2);
		MakePacket_T[3] = j & 0xff;
		MakePacket_T[4] = (j >> 8) & 0xff;

		CDC_Transmit_FS((uint8_t*)MakePacket_T, 5);
	}
	else if (type == MODBUS_ETHERNET_TYPE)
	{
		// Ethernet
		gSendDataLen = 0;
		gSENDDATABUF[gSendDataLen++] = etReadBuffer[0];
		gSENDDATABUF[gSendDataLen++] = etReadBuffer[1];
		gSENDDATABUF[gSendDataLen++] = etReadBuffer[2];
		gSENDDATABUF[gSendDataLen++] = etReadBuffer[3];
		gSENDDATABUF[gSendDataLen++] = 0x00;
		gSENDDATABUF[gSendDataLen++] = 0x03;							// Protocol Header byte length
		gSENDDATABUF[gSendDataLen++] = 0x00;							// ID
		gSENDDATABUF[gSendDataLen++] = etReadBuffer[1+6] + 0x80;		// Function code
		gSENDDATABUF[gSendDataLen++] = code;							// error code

		tcpSendData(socketControl.lastCommSocket, gSENDDATABUF, gSendDataLen);
	}
}


/**
 * @brief MODBUS의 TCP 응답 메시지를 보낼 때 Header 부분을 동일하게 Copy
 */
void mbMakeET_MBAP(void)
{
	gSENDDATABUF[0] = etReadBuffer[0];
	gSENDDATABUF[1] = etReadBuffer[1];

	gSENDDATABUF[2] = etReadBuffer[2];
	gSENDDATABUF[3] = etReadBuffer[3];

	gSENDDATABUF[4] = 0; 	// 나중에 입력.
	gSENDDATABUF[5] = 0;
}


/**
 * @brief MODBUS - Function code 6의 응답 리턴 메시지
 * 
 * @param type : 통신 타입 (0: RS232, 1: Ethernet)
 */
void mbSendReturnData_06(int type)
{
	uint8_t i;

	if (type == MODBUS_RS232_TYPE)
	{
		// RS232
		i = CDC_Transmit_FS((uint8_t*)ComReadBufferMODBUS, 8);
		if (i != 0)
		{
			system_err = USB_COMMUNICATION_FAIL;
		}
	}
	else if (type == MODBUS_ETHERNET_TYPE)
	{
		// Ethernet
		tcpSendData(socketControl.lastCommSocket, etReadBuffer, 12);
	}
}




/* Parameter ---------------------------------------------------------------------------*/

uint16_t getModbusPara(uint16_t address)
{
	uint16_t retval = 0;

	// Get 16bit single data
	if(address <= PARA_END_VAL_ADDRESS)
	{
		retval = getParaValSingle(address);
	}
	else if(address <= PARA_END_MIN_ADDRESS && address >= PARA_START_MIN_ADDRESS)
	{
		retval = getParaMinSingle(address - PARA_START_MIN_ADDRESS);
	}
	else if(address <= PARA_END_MAX_ADDRESS && address >= PARA_START_MAX_ADDRESS)
	{
		retval = getParaMaxSingle(address - PARA_START_MAX_ADDRESS);
	}

	return retval;
}


int8_t setModbusPara(uint16_t address, uint16_t data)
{
	floatFrom16 convertVal;
	floatFrom16 convertMax;
	floatFrom16 convertMin;
	int8_t retval = 0;

	// Get 16bit single data
	if(address <= PARA_END_VAL_ADDRESS)
	{
		if(Para[address].type == VAL_FLOAT_H)
		{
			// MIN, MAX 판단하는 것에 문제 있음 : 소수점 단위에 OVER가 발생할 수 있음
			if(data < Para[address].min || data > Para[address].max)
			{
				retval = -1;
			}
		}
		else if(Para[address].type == VAL_FLOAT_L)
		{
			// 전체 값을 보고 비교한다
			convertVal.raw.integer_high = Para[address - 1].val;
			convertVal.raw.integer_low = data;

			convertMax.raw.integer_high = Para[address - 1].max;
			convertMax.raw.integer_low = Para[address].max;

			convertMin.raw.integer_high = Para[address - 1].min;
			convertMin.raw.integer_low = Para[address].min;

			if(convertVal.fVal < convertMin.fVal || convertVal.fVal > convertMax.fVal)
			{
				retval = -1;
			}
		}
		else
		{
			if(data < Para[address].min || data > Para[address].max)
			{
				retval = -1;
			}
		}

		if(retval == 0)
		{
			// parameter save
			mbSavePara(data, address);

			#if MODBUS_VIRTUAL_PRESET_ACTIVE
			// Check Virtual preset mode parameter
			checkParaSaveWithVirtual(address, data);
			#endif
		}
	}
	else if(address <= PARA_END_MIN_ADDRESS && address >= PARA_START_MIN_ADDRESS)
	{
		// setParaMinSingle(address, data);
	}
	else if(address <= PARA_END_MAX_ADDRESS && address >= PARA_START_MAX_ADDRESS)
	{
		// setParaMaxSingle(address, data);
	}

	return retval;
}



/* Remote Control ---------------------------------------------------------------------------*/

#if MODBUS_REMOTE_CONTROL_ACTIVE

uint16_t getModbusRemoteControl(uint16_t address)
{
	uint16_t retval = 0;


	if(address >= RTC_YEAR && address <= RTC_SEC)
	{
		#if RTC_ACTIVE

		rtcTime rtcVal;
		getRtc(&rtcVal);

		switch (address)
		{
			case RTC_YEAR:
				retval = rtcVal.year;
			break;
			case RTC_MONTH:
				retval = rtcVal.month;
			break;
			case RTC_DAY:
				retval = rtcVal.date;
			break;
			case RTC_HOUR:
				retval = rtcVal.hour;
			break;
			case RTC_MINUTE:
				retval = rtcVal.minute;
			break;
			case RTC_SEC:
				retval = rtcVal.second;
			break;
		}

		#endif
	}
	else
	{
		switch(address)
		{
			case DRIVER_LOCK:
				retval = Flag.LockCommand;
			break;
		}
	}

	return retval;
}


int8_t setModbusRemoteControl(uint16_t address, uint16_t data)
{
	int8_t retval = 0;

	// RTC
	if(address >= RTC_YEAR && address <= RTC_SEC)
	{
		#if RTC_ACTIVE

		rtcTime rtcVal;
		getRtc(&rtcVal);

		switch (address)
		{
			case RTC_YEAR:
				rtcVal.year = data;
			break;
			case RTC_MONTH:
				rtcVal.month = data;
			break;
			case RTC_DAY:
				rtcVal.date = data;
			break;
			case RTC_HOUR:
				rtcVal.hour = data;
			break;
			case RTC_MINUTE:
				rtcVal.minute = data;
			break;
			case RTC_SEC:
				rtcVal.second = data;
			break;
		}

		setRtc(&rtcVal);

		#endif
	}
	else
	{
		switch (address)
		{
			case PARA_INIT:
				if(data == 77)
				{
					SystemResetValue = RESET_ALL_PARA;
					Flag.ParaInitialize = ON;
				}
			break;

			case ALARM_RESET:
				if(system_err)
					Flag.Reset = ON;
			break;

			case DRIVER_LOCK:
				if (data == 0)
				{
					// UNLock
					Flag.LockCommand = 0;
					Flag.Lock = OFF;
				}
				else if (data == 1)	
					Flag.LockCommand = 1;		// all lock	
				else if (data == 2)
					Flag.LockCommand = 2;		// Loosening lock	
				else if (data == 3)
					Flag.LockCommand = 3;		// Fastening lock	
			break;

			case PRESET_CHANGE:
				if (!Flag.Run && (data > 0) && (data <= PRESET_INDEX_SIZE + 2))
				{
					Processing_number = data;
					Set_Torque_variable();

					StatusOutput = rPRESET_CHANGE;
					ClearSendData_Init(ON);
					putRealtimeData(&realPollStruct);
				}
			break;

			case ERROR_LIST_INIT:
				if (data == 77)
				{
					// Error list 초기화
					for(uint8_t index = 0; index < ERROR_CODE_INDEX_SIZE; index++)
						removeErrorCodeList(index);
				}
			break;

			case DRIVER_RUN_STOP:
				if (data == 1)
				{
					if(Flag.Lock == OFF)
					{
						// Driver Run
						if (Flag.Multi_Mode && (Flag.ControlFL == FASTENING_CONTROL))
							Flag.Multi_Start = ON;
						else
							Flag.InternalRun = ON;
					}
				}
				else
				{
					if(Flag.InternalRun || Flag.ExternalRun || Flag.Multi_Start)
					{
						// Driver Stop
						Flag.Stopping = ON;
						Flag.Multi_Start = OFF;
					}
				}
			break;

			case F_L_CONTROL:
				if(Flag.Run == OFF)
				{
					if (data == 0)
						Flag.ControlFL = FASTENING_CONTROL;
					else if (data == 1)
						Flag.ControlFL = LOOSENING_CONTROL;

					if(data == 0 || data == 1)
					{
						StatusOutput = rF_L;
						ClearSendData_Init(ON);
						putRealtimeData(&realPollStruct);
					}
				}
			break;

			case IO_REMOTE_STATE:
				#if MODBUS_IO_ACTIVE
				if (FlagFasten.OutputTestEnable)
					ParaMode.outputModeIo = data;
				#endif
			break;

			case IO_REMOTE_ACTIVE:
				#if MODBUS_IO_ACTIVE
					FlagFasten.OutputTestEnable = data;		// Output test mode 활성화
					ParaMode.outputModeIo = 0x00;			// Output 신호 off
				#endif
			break;

			case MODEL_CHANGE:
				#if MODBUS_VIRTUAL_MODEL_ACTIVE
				if (!Flag.Run && (data > 0) && (data <= MODEL_INDEX_SIZE))
				{
					if(Para[MODEL_SELECT].val)
					{
						Model_Number = data;
						// step이 2이상 진행을 해야 error 조건이 됨
						if (Model.StartModel && Model.BeginStep && (Model.CurrentStep > 1)) 
							system_err = MODEL_CANCEL_ERROR;

						FlagFasten.ReloadModelData = ON;	
						Flag_UseParaMonTouch_EtcEvent = ON;		// Model 변경시 paramon에게 data send event
					}
				}
				#endif
			break;

			case DRIVER_PARA_INIT:
				if (data == 777)
					Flag.DriverParaInit = TRUE;
			break;

			case FIRMWARE_UPDATE_START:
				if (data == 7777)
				{
					Flag.FirmwareUpdate = ON;
				}
			break;
		}
	}

	return retval;
}

#endif



/* Auto customizing ---------------------------------------------------------------------------*/

#if MODBUS_AUTO_CUSTOMIZING_ACTIVE

uint16_t getModbusAutoCustom(uint16_t address)
{
	uint16_t retval = 0;

	switch (address)
	{
		case AUTO_CUSTOMIZE_SORT_HART_JOINT:
			retval = AutoSetting.FlagSetting;
		break;

		case AUTO_CUSTOMIZE_ACTIVE:
			retval = AutoSetting.FlagStart;
		break;

		case AUTO_CUSTOMIZE_SAVE:
			retval = 0;
		break;

		case AUTO_CUSTOMIZE_ANGLE1:
			retval = AutoSetting.CurrentA1;
		break;

		case AUTO_CUSTOMIZE_ANGLE2:
			retval = AutoSetting.CurrentA2;
		break;

		case AUTO_CUSTOMIZE_SPEED:
			retval = AutoSetting.CurrnetSpeed;
		break;

		case AUTO_CUSTOMIZE_FREE_SPEED:
			retval = AutoSetting.CurrnetFSpeed;
		break;

		case AUTO_CUSTOMIZE_FREE_ANGLE:
			retval = AutoSetting.CurrentFAngle;
		break;

		case AUTO_CUSTOMIZE_SEATING_POINT:
			retval = AutoSetting.CurrentSettingPoint;
		break;
	}

	return retval;
}


int8_t setModbusAutoCustom(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	switch (address)
	{
		case AUTO_CUSTOMIZE_SORT_HART_JOINT:
			if(data == 0)
			{
				// Soft joint
				AutoSetting.FlagTgSpeed = ON;
				AutoSetting.FlagSetting = ON;
				AutoSetting.FlagFrSpeed = OFF;
			}
			else
			{
				// Hard joint
				AutoSetting.FlagFrSpeed = ON;
				AutoSetting.FlagSetting = OFF;
				AutoSetting.FlagTgSpeed = OFF;
			}
		break;

		case AUTO_CUSTOMIZE_ACTIVE:
			if (AutoSetting.FlagTgSpeed || AutoSetting.FlagFrSpeed)
				AutoSetting.FlagStart = data;

			// Start 버튼 클릭시 초기화
			AutoSetting.CurrentA1 = 0;
			AutoSetting.CurrentA2 = 0;
			AutoSetting.CurrnetSpeed = 0;
			AutoSetting.CurrentSettingPoint = 0;
			AutoSetting.CurrnetFSpeed = 0;
			AutoSetting.CurrentFAngle = 0;

			Set_Torque_variable();
		break;

		case AUTO_CUSTOMIZE_SAVE:
			if (AutoSetting.CurrnetSpeed || AutoSetting.CurrentSettingPoint || AutoSetting.CurrnetFSpeed || AutoSetting.CurrentFAngle)
			{
				AutoSetting.FlagSavePara = ON;
			}
		break;
	}

	return retval;
}

#endif




/* Sensor calibration ---------------------------------------------------------------------------*/

#if MODBUS_SENSOR_CALIBRATION_ACTIVE

uint16_t getModbusSensor(uint16_t address)
{
	uint16_t retval = 0;

	switch (address)
	{
		case SENSOR_CAL_SEN_CALC_START:
		break;

		case SENSOR_CAL_SENSOR_VAL:
			retval = Tq_SensorValue_Avg;
		break;

		case SENSOR_CAL_OFFSET_VAL:
			retval = SensorOffset;
		break;

		case SENSOR_CAL_CALIB_VAL:
			retval = (dset[10] << 8) + dset[11] - SensorOffset;
		break;
	}

	return retval;
}


int8_t setModbusSensor(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	switch (address)
	{
		case SENSOR_CAL_SEN_CALC_START:
			if (data == 1)
				Flag_GetSensorOffset = ON;
		break;
	}

	return retval;
}

#endif




/* Bit socket tray ---------------------------------------------------------------------------*/

#if MODBUS_BIT_SOCKET_TRAY_COMM_ACTIVE

uint16_t getModbusBitSocketTrayComm(uint16_t address)
{
	uint16_t retval = 0;

	switch (address)
	{
		case BIT_SOCKET_TRAY_COMM_ENABLE:
			retval = Flag_bitsocket_mode;
		break;
	}

	return retval;
}


int8_t setModbusBitSocketTrayComm(uint16_t address, uint16_t data, uint8_t type)
{
	int8_t retval = 0;
	
	switch (address)
	{
		case BIT_SOCKET_TRAY_COMM_ENABLE:
			if(data == 1 && type == MODBUS_ETHERNET_TYPE)
			{
				Flag_bitsocket_mode = ON;
				setBitsocketConntion(socketControl.lastCommSocket);
			}
			else
				Flag_bitsocket_mode = OFF;		
		break;
	}

	return retval;
}

#endif




/* Virtual preset ---------------------------------------------------------------------------*/

#if MODBUS_VIRTUAL_PRESET_ACTIVE

uint16_t getModbusVirtualPreset(uint16_t address)
{
	uint16_t retval = 0;

	if(address <= VIRTUAL_PRESET_START_ADDRESS + PRESET_SIZE)
	{
		// Fix address (Preset의 시작 주소로 변경)
		address -= VIRTUAL_PRESET_START_ADDRESS;
		address += TCAM_ACTM1;

		retval = getParaValSingle(address);
	}

	return retval;
}


int8_t setModbusVirtualPreset(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	if(address <= VIRTUAL_PRESET_START_ADDRESS + PRESET_SIZE)
	{
		FlagFasten.NoSavePara = ON;
		
		// Fix address (Preset의 시작 주소로 변경)
		address -= VIRTUAL_PRESET_START_ADDRESS;
		address += TCAM_ACTM1;
			
		mbSavePara(data, address);

		// Start virtual preset
		startVirtualPreset();
	}

	return retval;
}

#endif



/* Virtual Advanced ---------------------------------------------------------------------------*/

#if MODBUS_VIRTUAL_ADVANCED_ACTIVE

uint16_t getModbusVirtualAdvanced(uint16_t address)
{
	uint16_t retval = 0;

	if(address <= VIRTUAL_ADVANCE_START_ADDRESS + ADV_PARA_SIZE)
	{
		// Fix address (Advanced 시작 주소로 변경)
		address -= VIRTUAL_ADVANCE_START_ADDRESS;
		address += ADVANCED_MODE1;

		retval = getParaValSingle(address);
	}

	return retval;
}


int8_t setModbusVirtualAdvanced(uint16_t address, uint16_t data)
{
	int8_t retval = 0;

	if(address <= VIRTUAL_ADVANCE_START_ADDRESS + ADV_PARA_SIZE)
	{
		FlagFasten.NoSavePara = ON;

		// Fix address (Advanced 시작 주소로 변경)
		address -= VIRTUAL_ADVANCE_START_ADDRESS;
		address += ADVANCED_MODE1;

		mbSavePara(data, address);

		// Start virtual preset
		startVirtualPreset();
	}

	return retval;
}

#endif



/* Virtual Model ---------------------------------------------------------------------------*/

#if MODBUS_VIRTUAL_MODEL_ACTIVE

uint16_t getModbusVirtualModel(uint16_t address)
{
	uint16_t retval = 0;

	if(address <= VIRTUAL_MODEL_START_ADDRESS + MODEL_SIZE)
	{
		// Fix address (Model의 시작 주소로 변경)
		address -= VIRTUAL_MODEL_START_ADDRESS;
		address += MODEL1_1;

		retval = getParaValSingle(address);
	}

	return retval;
}


int8_t setModbusVirtualModel(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	if(address <= VIRTUAL_MODEL_START_ADDRESS + MODEL_SIZE)
	{
		// Fix address (Model의 시작 주소로 변경)
		address -= VIRTUAL_MODEL_START_ADDRESS;
		address += MODEL1_1;

		FlagFasten.NoSavePara = ON;

		mbSavePara(data, address);

		// // Start virtual model
		// startVirtualPreset();
	}

	return retval;
}

#endif



/* Realtime event Control ---------------------------------------------------------------------------*/

#if MODBUS_REALTIME_EVENT_ACTIVE

uint16_t getModbusRealtimeEvent(uint16_t address, uint8_t type)
{
	uint16_t retval = 0;

	switch (address)
	{
		case REALTIME_TRIGGER_MONITORING_ENABLE:
			if (type == MODBUS_RS232_TYPE) 	 		
			{
				retval = TRIG_REAL.MonitoringRS232;
			}
			else
			{
				retval = TRIG_REAL.MointoringEthernet;
			}
		break;

		case REALTIME_USB_START_INDEX:
			retval = GraphMem.index.last_send_data_no;
		break;

		case REALTIME_USB_END_INDEX:
			retval = GraphMem.index.save_memory_data_no;
		break;

		case REALTIME_USB_1CYCLE_OVER:
			retval = returnMemorySavedValue();
		break;
	}

	return retval;
}


int8_t setModbusRealtimeEvent(uint16_t address, uint16_t data, uint8_t type)
{
	int8_t retval = 0;
	
	switch (address)
	{
		case REALTIME_TRIGGER_MONITORING_ENABLE:
			if (type == MODBUS_RS232_TYPE) 	 		
			{
				// RS232
				if (data == 1)
				{
					TRIG_REAL.TimeOutCntRS232 = 0;
					TRIG_REAL.MonitoringRS232 = TRUE;
				}
				else
					TRIG_REAL.MonitoringRS232 = FALSE;
			}
			else if (type == MODBUS_ETHERNET_TYPE) 	
			{
				// Ethernet
				if (data == 1)
				{
					TRIG_REAL.TimeOutCntEthernet = 0;
					TRIG_REAL.MointoringEthernet = TRUE;

					setSocketMonitoringControl(SOCKET_DESCRIPTION, socketControl.lastCommSocket, MONITORING_REALTIME, ON);
				}
				else
				{
					TRIG_REAL.MointoringEthernet = FALSE;			
					setSocketMonitoringControl(SOCKET_DESCRIPTION, socketControl.lastCommSocket, MONITORING_REALTIME, OFF);
				}
			}
		break;

		case REALTIME_USB_ENABLE:
			setRealtimeSelect(data);
		break;

		case REALTIME_USB_START_INDEX:
		break;

		case REALTIME_USB_END_INDEX:
		break;

		case REALTIME_USB_1CYCLE_OVER:
		break;
	}

	return retval;
}

#endif




/* Graph event Control ---------------------------------------------------------------------------*/

#if MODBUS_GRAPH_MONITORING_ACTIVE

uint16_t getModbusGraphMonitoringSetting(uint16_t address, uint8_t type)
{
	uint16_t retval = 0;

	switch (address)
	{
		/* 일반 그래프 모니터링 */
		case GRAPH_TRIGGER_MONITORING_ENABLE:
			if(type == MODBUS_RS232_TYPE)
			{
				if(TRIG.MonitoringRS232)
					retval = 1;
				else
					retval = 0;

				TRIG.TimeOutCntRS232 = 0;
			}
			else
			{
				if(TRIG.MointoringEthernet)
					retval = 1;
				else
					retval = 0;

				TRIG.TimeOutCntEthernet = 0;
			}
		break;
		
		case GRAPH_MONITOINRG_CHANNEL1:
			retval = TRIG.Channel[0];
		break;

		case GRAPH_MONITOINRG_CHANNEL2:
			retval = TRIG.Channel[1];
		break;

		case GRAPH_MONITOINRG_SAMPLING:
			retval = TRIG.SamplingRate;
		break;

		case GRAPH_MONITOINRG_DIRECTION:
			retval = TRIG.Option;
		break;
	}

	return retval;
}


int8_t setModbusGraphMonitoringSetting(uint16_t address, uint16_t data, uint8_t type)
{
	int8_t retval = 0;
	
	switch (address)
	{
		/* 일반 그래프 모니터링 */
		case GRAPH_TRIGGER_MONITORING_ENABLE:
			if (type == MODBUS_RS232_TYPE) 		
			{
				// RS232
				if (data == 1)
				{
					TRIG.TimeOutCntRS232 = 0;
					TRIG.MonitoringRS232 = TRUE;
				}	
				else 	
					TRIG.MonitoringRS232 = FALSE;
			}
			else if (type == MODBUS_ETHERNET_TYPE) 		
			{
				// ethernet
				if (data == 1)
				{
					TRIG.TimeOutCntEthernet = 0;
					TRIG.MointoringEthernet = TRUE;

					setSocketMonitoringControl(SOCKET_DESCRIPTION, socketControl.lastCommSocket, MONITORING_GRAPH, ON);
				}	
				else
				{
					TRIG.MointoringEthernet = FALSE;
					setSocketMonitoringControl(SOCKET_DESCRIPTION, socketControl.lastCommSocket, MONITORING_GRAPH, OFF);
				}	
			}
		break;
		
		case GRAPH_MONITOINRG_CHANNEL1:
			TRIG.Channel[0] = data;
		break;

		case GRAPH_MONITOINRG_CHANNEL2:
			TRIG.Channel[1] = data;
		break;

		case GRAPH_MONITOINRG_SAMPLING:
			if(data >= 2 && data <= 30)
				TRIG.SamplingRate = data;	// 2 ~ 30 ms
			else
				retval = -1;
		break;

		case GRAPH_MONITOINRG_DIRECTION:
			TRIG.Option = data;
		break;
	}

	return retval;
}

#endif



/* Graph event Control ---------------------------------------------------------------------------*/

#if MODBUS_GRAPH_EVENT_ACTIVE

uint16_t getModbusGraphEvent(uint16_t address, uint8_t type)
{
	uint16_t retval = 0;

	switch (address)
	{
		/* 고해상도 그래프 모니터링 */
		case GRAPH_EVENT_SAVED_DATA_NUM:
			retval = graphRemainData();
		break;

		case GRAPH_EVENT_REVISION:
			retval = MAX_REVISION_NUM;
		break;

		case GRAPH_EVENT_SETTING_H:
			retval = (uint16_t)(getGraphSetting() >> 16);
		break;
		case GRAPH_EVENT_SETTING_L:
			retval = (uint16_t)(getGraphSetting() & 0xFF);
		break;

		case GRAPH_EVENT_RECEIVE_ACK:
		break;

		case GRAPH_EVENT_KEEP_ALIVE:
			retval = returnEventSocketKeepalive(socketControl.lastCommSocket);
		break;
	}

	return retval;
}


int8_t setModbusGraphEvent(uint16_t address, uint16_t data, uint8_t type)
{
	static uint32_t graphSettingHighVal;
	int8_t retval = 0;
	uint32_t convertData = 0;
	
	switch (address)
	{
		/* 고해상도 그래프 모니터링 */
		case GRAPH_EVENT_SAVED_DATA_NUM:
		break;

		case GRAPH_EVENT_REVISION:
		break;

		case GRAPH_EVENT_SETTING_H:
			graphSettingHighVal = (uint32_t)data << 16;
		break;

		case GRAPH_EVENT_SETTING_L:
			// Graph event setting
			convertData = graphSettingHighVal & 0xFFFF0000;
			convertData += (uint32_t)data;
			retval = setGraphSetting(convertData, socketControl.lastCommSocket);

			// 고해상도 그래프 활성화
			if(GraphMem.setting.eventSendActive)
				setEventSocketActivate(SOCKET_DESCRIPTION, socketControl.lastCommSocket, MONITORING_EVENT_GRAPH, ON);
			else
				setEventSocketActivate(SOCKET_DESCRIPTION, socketControl.lastCommSocket, MONITORING_EVENT_GRAPH, OFF);
		break;

		case GRAPH_EVENT_RECEIVE_ACK:
			// Graph evnet ack
			if ((data == 1) && GraphMem.ack.eventAckWait)
			{
				initGraphEventAckWait();
				// Index 증가
				++GraphMem.index.last_send_data_no;
				graphMemIndexManage();
			}
		break;

		case GRAPH_EVENT_KEEP_ALIVE:
		break;
	}

	return retval;
}


#endif




/* Barcode ---------------------------------------------------------------------------*/

#if PRESET_BARCODE_ACTIVE

uint16_t getModbusPresetBarcode(uint16_t address)
{
	uint16_t retval = 0;

	retval = presetBarcodeSetRead(address);

	return retval;
}


int8_t setModbusPresetBarcode(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	presetBarcodeSetWrite(address, data);

	return retval;
}

#endif




/* Model Barcode ---------------------------------------------------------------------------*/

#if MODEL_BARCODE_ACTIVE

uint16_t getModbusModelBarcode(uint16_t address)
{
	uint16_t retval = 0;

	retval = modelBarcodeSetRead(address);

	return retval;
}


int8_t setModbusModelBarcode(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	modelBarcodeSetWrite(address, data);

	return retval;
}

#endif



/* Driver ---------------------------------------------------------------------------*/

#if MODBUS_DRIVER_ACTIVE

uint16_t getModbusDriver(uint16_t address)
{
	uint16_t retval = 0;

	if(address >= DRIVER_PARA1 && address <= DRIVER_PARA15)
	{
		switch (address)
		{
			case DRIVER_PARA1:
				retval = dset[0];
			break;
			case DRIVER_PARA2:
				retval = dset[1];
			break;
			case DRIVER_PARA3:
				retval = dset[2];
			break;
			case DRIVER_PARA4:
				retval = dset[3];
			break;
			case DRIVER_PARA5:
				retval = dset[9];
			break;
			case DRIVER_PARA6:
				retval = dset[4];
			break;
			case DRIVER_PARA7:
				retval = dset[5];
			break;
			case DRIVER_PARA8:
				retval = dset[6];
			break;
			case DRIVER_PARA9:
				retval = PRODUCT_CODE;
			break;
			case DRIVER_PARA10:
				retval = dset[7];
			break;
			case DRIVER_PARA11:
				retval = dset[8];
			break;
			case DRIVER_PARA12:
				retval = dset[9];
			break;
		}
	}

	return retval;
}


int8_t setModbusDriver(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	uint8_t index = 0;

	switch (address)
	{
		case DRIVER_PARA1:
			dset[0] = data;
			index = 0;
		break;
		case DRIVER_PARA2:
			dset[1] = data;
			index = 1;
		break;
		case DRIVER_PARA3:
			dset[2] = data;
			index = 2;
		break;
		case DRIVER_PARA4:
			dset[3] = data;
			index = 3;
		break;
		case DRIVER_PARA5:
			dset[9] = data;

			index = 9;
			Para[TORQUE_COMPENSATION_MAIN].val = data;
			SendSaveParameter(TORQUE_COMPENSATION_MAIN, data);
		break;
		case DRIVER_PARA6:
			dset[4] = data;
			index = 4;
		break;
		case DRIVER_PARA7:
			dset[5] = data;
			index = 5;
		break;
		case DRIVER_PARA8:
			dset[6] = data;
			index = 6;
		break;
		case DRIVER_PARA9:
		break;
		case DRIVER_PARA10:
			dset[7] = data;
			index = 7;
		break;
		case DRIVER_PARA11:
			dset[8] = data;
			index = 8;
		break;
		case DRIVER_PARA12:
			dset[9] = data;
			index = 9;
		break;
		case DRIVER_PARA13:
			dset[10] = data;
			index = 10;
		break;
		case DRIVER_PARA14:
			dset[11] = data;
			index = 11;
		break;
		case DRIVER_PARA15:
			dset[12] = data;
			index = 12;
		break;
	}

	Flag.DriverSaveParaData = ON;
	DriverSaveParaNumber = index;
	dParameterValue[index] = dset[index];

	return retval;
}

#endif




/* Controller set parameter ---------------------------------------------------------------------------*/

#if MODBUS_CONTROLLER_PARA_ACTIVE

uint16_t getModbusControllerPara(uint16_t address)
{
	uint16_t retval = 0;

	// Controller Model
	if(address == CONTROLLER_MODEL)
	{
		retval = controllerPara.model;
	}

	// Controller serial number
	if(address >= CONTROLLER_SERIAL_NUMBER1 && address <= CONTROLLER_SERIAL_NUMBER5)
	{
		retval = controllerPara.serialNumber[address - CONTROLLER_SERIAL_NUMBER1];
	}

	// Controller Product code
	if(address >= CONTROLLER_PRODUCT_DATE_1 && address <= CONTROLLER_PRODUCT_DATE_2)
	{
		retval = controllerPara.productDate[address - CONTROLLER_PRODUCT_DATE_1];
	}

	// Controller Mac address
	if(address >= CONTROLLER_MAC_ADDRESS_1 && address <= CONTROLLER_MAC_ADDRESS_3)
	{
		// retval = controllerPara.macAddress[address - CONTROLLER_MAC_ADDRESS_1];

		// BM - WIFI 모듈에서 MAC 주소 반영
        retval = ((uint16_t)WifiEthSet.MACADDRESS[(address - CONTROLLER_MAC_ADDRESS_1) * 2] << 8) + \
				 (WifiEthSet.MACADDRESS[(address - CONTROLLER_MAC_ADDRESS_1) * 2 + 1]);
	}

	return retval;
}


int8_t setModbusControllerPara(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	// Controller Model
	if(address == CONTROLLER_MODEL)
	{
	}

	// Controller serial number
	if(address >= CONTROLLER_SERIAL_NUMBER1 && address <= CONTROLLER_SERIAL_NUMBER5)
	{
		controllerPara.serialNumber[address - CONTROLLER_SERIAL_NUMBER1] = data;
	}

	// Controller Product code
	if(address >= CONTROLLER_PRODUCT_DATE_1 && address <= CONTROLLER_PRODUCT_DATE_2)
	{
		controllerPara.productDate[address - CONTROLLER_PRODUCT_DATE_1] = data;
	}

	// Controller Mac address
	if(address >= CONTROLLER_MAC_ADDRESS_1 && address <= CONTROLLER_MAC_ADDRESS_3)
	{
		// controllerPara.macAddress[address - CONTROLLER_MAC_ADDRESS_1] = data;
	}

	// Set save command
	controllerParaCommand.saveCommand = ON;

	return retval;
}

#endif






/* Maintenance ---------------------------------------------------------------------------*/

#if MODBUS_MAINTENACNCE_ACTIVE

uint16_t getModbusMaintenance(uint16_t address)
{
	uint16_t retval = 0;

	if(address == MAINTENANCE_ENABLE)
	{
		retval = getTotalScrewCount(TOTAL_SCREW_ALARM_ACTIVE_ADDRESS);
	}

	if(address >= MAINTENANCE_ALARM_COUNT_1 && address <= MAINTENANCE_ALARM_COUNT_2)
	{
		address -= MAINTENANCE_ALARM_COUNT_1;
		retval = getTotalScrewCount(LIMIT_SCREW_ADDRESS + address);
	}
	
	if(address >= MAINTENANCE_START_COUNT_1 && address <= MAINTENANCE_START_COUNT_2)
	{
		address -= MAINTENANCE_START_COUNT_1;
		retval = getTotalScrewCount(RESET_SCREW_ADDRESS + address);
	}

	if(address >= MAINTENANCE_USED_COUNT_1 && address <= MAINTENANCE_USED_COUNT_2)
	{
		address -= MAINTENANCE_USED_COUNT_1;
		retval = getTotalScrewCount(TOTAL_SCREW_ADDRESS + address);
	}

	return retval;
}


int8_t setModbusMaintenance(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	if(address == MAINTENANCE_ENABLE)
	{
		setTotalScrewCount(TOTAL_SCREW_ALARM_ACTIVE_ADDRESS, data);
	}

	if(address >= MAINTENANCE_ALARM_COUNT_1 && address <= MAINTENANCE_ALARM_COUNT_2)
	{
		address -= MAINTENANCE_ALARM_COUNT_1;
		setTotalScrewCount(LIMIT_SCREW_ADDRESS + address, data);

		// Total screw count reset (히든키: 0x77BB77BB = 2008774587)
		if(screwCount.limitScrew == 0x77BB77BB && screwCount.totalScrew > 500000)
		{
			screwCount.totalScrew = 0;
		}
	}

	if(address >= MAINTENANCE_START_COUNT_1 && address <= MAINTENANCE_START_COUNT_2)
	{
		// Reset count limit
		if(data > screwCount.totalScrew)
			data = screwCount.totalScrew;

		address -= MAINTENANCE_START_COUNT_1;
		setTotalScrewCount(RESET_SCREW_ADDRESS + address, data);
	}

	if(address >= MAINTENANCE_USED_COUNT_1 && address <= MAINTENANCE_USED_COUNT_2)
	{
		address -= MAINTENANCE_USED_COUNT_1;
		// setTotalScrewCount(TOTAL_SCREW_ADDRESS + address, data);
	}

	return retval;
}

#endif






/* Information data (Function code 17) ---------------------------------------------------------------------------*/

uint16_t getModbusInformation(uint16_t address)
{
	uint16_t retval = 0;
	uint8_t index;
	char tempNum[16] = "";
	char serialNum[16] = "";
	uint16_t charHigh = 0, charLow = 0;


	switch (address)
	{
		case INFORMATION_DRIVER_ID:
			retval = Para[DRIVER_ID].val;
		break;

		case INFORMATION_DRIVER_MODEL_NUMBER:
			retval = Para[DRIVER_MODEL].val;
		break;

		case INFORMATION_CONTROLLER_MODEL_NUMBER:
			retval = controllerPara.model;
		break;

		case INFORMATION_CONTROLLER_ADVANCED_TYPE:
			retval = MODBUS_ADVANCED_TYPE;
		break;
	}

	// Driver model name
	if(address >= INFORMATION_DRIVER_MODEL_NAME_1 && address <= INFORMATION_DRIVER_MODEL_NAME_16)
	{
		index = address - INFORMATION_DRIVER_MODEL_NAME_1;

		if(index * 2 < strlen(Info_DrvModel[dset[4]].name))
			charHigh = (Info_DrvModel[dset[4]].name[index * 2] << 8);

		if(index * 2 + 1 < strlen(Info_DrvModel[dset[4]].name))
			charLow = (Info_DrvModel[dset[4]].name[index * 2 + 1] << 0);
		
		retval = charHigh + charLow;
	}

	// Driver serial number
	if(address >= INFORMATION_DRIVER_SERIAL_NUMBER_1 && address <= INFORMATION_DRIVER_SERIAL_NUMBER_5)
	{
		index = address - INFORMATION_DRIVER_SERIAL_NUMBER_1;

		for(uint8_t i = 0; i < 4; i++)
		{
			sprintf(tempNum, "%02d", dset[5 + i]);     // 2자리가 나오도록  (EX: 09 00 10)
			tempNum[2] = 0;
			strcat(serialNum, tempNum);

			if(i == 1)
			{
				sprintf(tempNum, "%02d", PRODUCT_CODE);
				tempNum[2] = 0;
				strcat(serialNum, tempNum);                 // BMT 제품코드
			}
		}
		
		retval = (serialNum[index * 2] << 8) + serialNum[index * 2 + 1];
	}


	// Controller model name
	if(address >= INFORMATION_CONTROLLER_MODEL_NAME_1 && address <= INFORMATION_CONTROLLER_MODEL_NAME_16)
	{
		index = address - INFORMATION_CONTROLLER_MODEL_NAME_1;

		retval = controllerPara.modelName[index];
	}

	// Controller serial number
	if(address >= INFORMATION_CONTROLLER_SERIAL_NUMBER_1 && address <= INFORMATION_CONTROLLER_SERIAL_NUMBER_5)
	{
		index = address - INFORMATION_CONTROLLER_SERIAL_NUMBER_1;
		
		retval = controllerPara.serialNumber[index];
	}


	// Firmware version
	if(address >= INFORMATION_CONTROLLER_FIRMWARE_1 && address <= INFORMATION_CONTROLLER_FIRMWARE_3)
	{
		index = address - INFORMATION_CONTROLLER_FIRMWARE_1;
		
		retval = controllerPara.firmwareVersion[index];
	}

	// Controller Product date
	if(address >= INFORMATION_CONTROLLER_PRODUCT_DATE_1 && address <= INFORMATION_CONTROLLER_PRODUCT_DATE_2)
	{
		index = address - INFORMATION_CONTROLLER_PRODUCT_DATE_1;
		
		retval = controllerPara.productDate[index];
	}


	// Mac address
	if(address >= INFORMATION_MAC_ADDRESS_1 && address <= INFORMATION_MAC_ADDRESS_3)
	{
		index = address - INFORMATION_MAC_ADDRESS_1;
		
		#ifdef WIFI_MAC_ADDRESS_ACTIVE
		retval = (WifiEthSet.MACADDRESS[index * 2] << 8) + WifiEthSet.MACADDRESS[index * 2 + 1];
		#else
		retval = controllerPara.macAddress[index];
		#endif
	}

	return retval;
}


int8_t setModbusInformation(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	return retval;
}





/* Current state monitoring ---------------------------------------------------------------------------*/

#if MODBUS_CURRENT_STATE_MONITORING_ACTIVE

uint16_t getModbusCurrentStateMonitoring(uint16_t address)
{
	uint16_t retval = 0;
	floatFrom16 convert;

	switch (address)
	{
		case CURRENT_TORQUE_H:
			convert.fVal = RealTq_fromSensor_reflectedSettingUnit * inv_Torque_ConvertValue * getCrowFootRatio() * screw_type;
			retval = convert.raw.integer_high;
		break;
		case CURRENT_TORQUE_L:
			convert.fVal = RealTq_fromSensor_reflectedSettingUnit * inv_Torque_ConvertValue * getCrowFootRatio() * screw_type;
			retval = convert.raw.integer_low;
		break;
		case CURRENT_SPEED:
			retval = (uint16_t)(w_rpm_display / deceleration_ratio);
		break;
		case CURRENT_CURRENT_H:
			convert.fVal = i_real * screw_type;
			retval = convert.raw.integer_high;
		break;
		case CURRENT_CURRENT_L:
			convert.fVal = i_real * screw_type;
			retval = convert.raw.integer_low;
		break;
		case CURRENT_PRESET:
			// Virtual preset 모드일 경우 별도 preset 번호 표시 
			if(virtualPrsetState())
				retval = VIRTUAL_PRESET_DISPLAY_NUM;
			else
				retval = Processing_number;
		break;
		case CURRENT_MODEL:
			if(Para[MODEL_SELECT].val)
				retval = Model_Number;
		break;
		case CURRENT_TORQUE_UP_IO:
			retval = Flag.TorqueUpCompleteOut;
		break;
		case CURRENT_FASTENING_OK_IO:
			retval = Flag.FasteningCompleteOut;
		break;
		case CURRENT_READY:
			if (system_err || Flag.Lock || Flag.Run || Flag.TorqueUpCompleteOut)		// check ready
				retval = 0;
			else
				retval = 1;
		break;
		case CURRENT_RUN:
			retval = Flag.Run;
		break;
		case CURRENT_ALARM:
			retval = system_err;
		break;
		case CURRENT_FL:
			retval = Flag.ControlFL;
		break;
		case CURRENT_SCREW_COUNT:
			retval = ScrewCountValue;
		break;
		case CURRENT_INPUT_IO:
			#if MODBUS_IO_ACTIVE
			retval = ExtInput.all;
			#endif
		break;
		case CURRENT_OUTPUT_IO:
			#if MODBUS_IO_ACTIVE
			if(FlagFasten.OutputTestEnable)
				retval = (uint16_t)(ParaMode.outputModeIo);			// Output mode
			else
				retval = (uint16_t)(ExtOutput.all);					// 정상 I/O 모드
			#endif
		break;
		case CURRENT_TEMPERATURE_H:
			convert.fVal = Temperature_real_Flt_motor;
			retval = convert.raw.integer_high;
		break;
		case CURRENT_TEMPERATURE_L:
			convert.fVal = Temperature_real_Flt_motor;
			retval = convert.raw.integer_low;
		break;
		case CURRENT_LOCK:
			retval = Flag.Lock;
		break;
	}


	return retval;
}


int8_t setModbusCurrentStateMonitoring(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
	
	return retval;
}

#endif



/* Graph monitoring ---------------------------------------------------------------------------*/

#if MODBUS_GRAPH_MONITORING_ACTIVE

uint16_t getModbusGraphMonitoring(uint16_t address)
{
	uint16_t retval = 0;

	// Realtime data
	if(address <= GRAPH_REALTIME_END_ADDRESS && address >= GRAPH_REALTIME_START_ADDRESS)
	{
		retval = mbMData[address - GRAPH_REALTIME_START_ADDRESS];
	}

	// Graph Channel 1 data
	if(address <= GRAPH_DATA_CAHNNEL1_1999 && address >= GRAPH_DATA_CAHNNEL1_1)
	{
		retval = graphMonLastData.data[0][address - GRAPH_DATA_CAHNNEL1_1];
	}

	// Graph Channel 2 data
	if(address <= GRAPH_DATA_CAHNNEL2_1999 && address >= GRAPH_DATA_CAHNNEL2_1)
	{
		retval = graphMonLastData.data[1][address - GRAPH_DATA_CAHNNEL2_1];
	}

	return retval;
}


int8_t setModbusGraphMonitoring(uint16_t address, uint16_t data)
{
	int8_t retval = 0;

	return retval;
}

#endif




/* Error code ---------------------------------------------------------------------------*/

#if MODBUS_ERROR_CODE_ACTIVE

uint16_t getModbusErrorCode(uint16_t address)
{
	uint16_t retval = 0;
	uint8_t index;

	// Realtime data
	if(address <= ERROR32_TIME2 && address >= ERROR1_CODE)
	{
		index = (address - ERROR1_CODE) / (ERROR2_CODE - ERROR1_CODE);
		address = address - (ERROR2_CODE - ERROR1_CODE) * index;

		switch(address)
		{
			case ERROR1_CODE:
				retval = errorCodeList[index].code;
			break;
			case ERROR1_DATE1:
				retval = errorCodeList[index].time[3];
			break;
			case ERROR1_DATE2:
				retval = errorCodeList[index].time[2];
			break;
			case ERROR1_TIME1:
				retval = errorCodeList[index].time[1];
			break;
			case ERROR1_TIME2:
				retval = errorCodeList[index].time[0];
			break;
		}
	}

	return retval;
}


int8_t setModbusErrorCode(uint16_t address, uint16_t data)
{
	int8_t retval = 0;

	return retval;
}

#endif



/* Fieldbus ---------------------------------------------------------------------------*/

#if MODBUS_FIELDBUS_ACTIVE

uint16_t getModbusFieldbus(uint16_t address)
{
	uint16_t retval = 0;
	uint8_t index;
	uint8_t checkByteHigh;
	floatFrom16 convert;

	switch (address)
	{
		case FIELDBUS_DIGITAL_INPUT:
			retval = anybusIo.inputBit >> 16;
		break;

		case FIELDBUS_DIGITAL_INPUT + 1:
			retval = anybusIo.inputBit & 0xFF;
		break;

		case FIELDBUS_DIGITAL_OUTPUT:
			retval = anybusIo.outputBit >> 16;
		break;

		case FIELDBUS_DIGITAL_OUTPUT + 1:
			retval = anybusIo.outputBit & 0xFF;
		break;
	}

	// Monitoring data
	if(address <= FIELDBUS_MONITOR_DATA_30 && address >= FIELDBUS_MONITOR_DATA_1)
	{
		// Check high, low byte
		if(address % 2 == 0)
			checkByteHigh = ON;			// High byte
		else
			checkByteHigh = OFF;		// Low byte


		// Get address -> adnybus monitoring index
		index = (address - FIELDBUS_MONITOR_DATA_1) / 2 + 2;

		// Check index limit
		if(index <= 32)
		{
			switch (APPL_asAdiEntryList[index].bDataType)
			{
				case 5:
					// UINT16
					if(checkByteHigh)
						retval = 0;
					else
						retval = *APPL_asAdiEntryList[index].uData.sUINT16.piValuePtr;
				break;

				case 6:
					// UINT32
					if(checkByteHigh)
						retval = (uint16_t)((*APPL_asAdiEntryList[index].uData.sUINT32.plValuePtr) >> 16);
					else
						retval = (uint16_t)((*APPL_asAdiEntryList[index].uData.sUINT32.plValuePtr) >> 0);
				break;

				case 18:
					// FLOAT
					convert.fVal = *APPL_asAdiEntryList[index].uData.sFLOAT32.prValuePtr;

					if(checkByteHigh)
						retval = convert.raw.integer_low;
					else
						retval = convert.raw.integer_high;
				break;
			}
		}
	}

	return retval;
}


int8_t setModbusFieldbus(uint16_t address, uint16_t data)
{
	int8_t retval = 0;

	return retval;
}

#endif


