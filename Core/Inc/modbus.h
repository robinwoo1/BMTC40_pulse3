
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MODBUS_H
#define _MODBUS_H

#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/

// 기능 활성화 여부
#define MODBUS_REMOTE_CONTROL_ACTIVE					ON
#define MODBUS_AUTO_CUSTOMIZING_ACTIVE					ON
#define MODBUS_SENSOR_CALIBRATION_ACTIVE				ON
#define MODBUS_BIT_SOCKET_TRAY_COMM_ACTIVE				ON
#define MODBUS_VIRTUAL_PRESET_ACTIVE					ON
#define MODBUS_VIRTUAL_ADVANCED_ACTIVE					ON
#define MODBUS_VIRTUAL_MODEL_ACTIVE						OFF

#define MODBUS_REALTIME_EVENT_ACTIVE					ON
#define MODBUS_GRAPH_EVENT_ACTIVE						ON

#define MODBUS_DRIVER_ACTIVE							ON
#define MODBUS_CONTROLLER_PARA_ACTIVE					ON
#define MODBUS_MAINTENACNCE_ACTIVE					    ON

#define MODBUS_CURRENT_STATE_MONITORING_ACTIVE		    ON
#define MODBUS_REALTIME_MONITORING_ACTIVE				ON
#define MODBUS_GRAPH_MONITORING_ACTIVE					ON
#define MODBUS_ERROR_CODE_ACTIVE					    ON
#define MODBUS_FIELDBUS_ACTIVE					        OFF

#define MODBUS_IO_ACTIVE								OFF
#define MODBUS_ADVANCED_TYPE                            ON




/* Struct ------------------------------------------------------------------*/

typedef enum
{
    MODBUS_RS232_TYPE = 1,
    MODBUS_ETHERNET_TYPE,
} MODBUS_TYPE_ENUM;




/* Externs ------------------------------------------------------------------*/


/* Function ------------------------------------------------------------------*/

void Modbus_RTU_Process(int type);

void mbProcess_03(int type);
void mbProcess_04(int type);
void mbProcess_06(int type);
void mbProcess_17(int type);

void mbMakeandSend_03(uint16_t st_adr, uint16_t cnt_reg, int type);
uint32_t mbMakeandSend_04(uint16_t st_adr, uint16_t cnt_reg, int type);
int8_t mbMakeandSend_06(uint16_t st_adr, uint16_t data, int type);
uint32_t mbMakeandSend_64(uint16_t st_adr, uint16_t cnt_reg, int type);
uint32_t mbMakeandSend_65(uint16_t st_adr, uint16_t cnt_reg, int type);
uint32_t mbMakeandSend_eventMonitoring(uint16_t st_adr, uint16_t cnt_reg, int type);


void mbSendReturnData_06(int type);

void mbMakeET_MBAP(void);
uint16_t mbGetCRC(uint8_t *data, int Length);
void mbSendErrorCode(int code, int type);






// Get MODBUS 기능

uint16_t getModbusPara(uint16_t address);
uint16_t getModbusRemoteControl(uint16_t address);
uint16_t getModbusAutoCustom(uint16_t address);
uint16_t getModbusSensor(uint16_t address);
uint16_t getModbusBitSocketTrayComm(uint16_t address);
uint16_t getModbusVirtualPreset(uint16_t address);
uint16_t getModbusVirtualAdvanced(uint16_t address);
uint16_t getModbusVirtualModel(uint16_t address);

uint16_t getModbusRealtimeEvent(uint16_t address, uint8_t type);
uint16_t getModbusGraphMonitoringSetting(uint16_t address, uint8_t type);
uint16_t getModbusGraphEvent(uint16_t address, uint8_t type);

uint16_t getModbusPresetBarcode(uint16_t address);
uint16_t getModbusModelBarcode(uint16_t address);
uint16_t getModbusDriver(uint16_t address);
uint16_t getModbusControllerPara(uint16_t address);
uint16_t getModbusMaintenance(uint16_t address);

uint16_t getModbusCurrentStateMonitoring(uint16_t address);
uint16_t getModbusGraphMonitoring(uint16_t address);
uint16_t getModbusInformation(uint16_t address);
uint16_t getModbusErrorCode(uint16_t address);
uint16_t getModbusFieldbus(uint16_t address);




// Set MODBUS 기능

int8_t setModbusPara(uint16_t address, uint16_t data);
int8_t setModbusRemoteControl(uint16_t address, uint16_t data);
int8_t setModbusAutoCustom(uint16_t address, uint16_t data);
int8_t setModbusSensor(uint16_t address, uint16_t data);
int8_t setModbusBitSocketTrayComm(uint16_t address, uint16_t data, uint8_t type);
int8_t setModbusVirtualPreset(uint16_t address, uint16_t data);
int8_t setModbusVirtualAdvanced(uint16_t address, uint16_t data);
int8_t setModbusVirtualModel(uint16_t address, uint16_t data);

int8_t setModbusRealtimeEvent(uint16_t address, uint16_t data, uint8_t type);
int8_t setModbusGraphMonitoringSetting(uint16_t address, uint16_t data, uint8_t type);
int8_t setModbusGraphEvent(uint16_t address, uint16_t data, uint8_t type);

int8_t setModbusPresetBarcode(uint16_t address, uint16_t data);
int8_t setModbusModelBarcode(uint16_t address, uint16_t data);
int8_t setModbusDriver(uint16_t address, uint16_t data);
int8_t setModbusControllerPara(uint16_t address, uint16_t data);
int8_t setModbusMaintenance(uint16_t address, uint16_t data);


int8_t setModbusCurrentStateMonitoring(uint16_t address, uint16_t data);
int8_t setModbusGraphMonitoring(uint16_t address, uint16_t data);
int8_t setModbusInformation(uint16_t address, uint16_t data);
int8_t setModbusErrorCode(uint16_t address, uint16_t data);
int8_t setModbusFieldbus(uint16_t address, uint16_t data);




#endif 
