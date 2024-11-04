/**
  ******************************************************************************
  * @file    controllerParameter.c
  * @author  jun
  * @brief   Source file of controllerParameter.
  * @note    Controller에 대한 정보
  ******************************************************************************
  */


/* Include ---------------------------------------------------------------------*/
#include "controllerParameter.h"
#include "variable.h"
#include "backup.h"
#include "RTC.h"
#include "wifiTask.h"



/* Variable ---------------------------------------------------------------------*/
controllerParaStruct controllerPara = {0, }; 
contorllerParaProcessStruct controllerParaCommand = {0, 1};



/* Preivate Funcions ----------------------------------------------------------------------*/
extern NetworkTypeDef    WifiEthSet;             // Ethernet Config: IP, Gateway, Netmask
 


/* Funcions ----------------------------------------------------------------------*/


void controllerParaProcess(void)
{
    // Controller parameter 읽기 실행
    if(controllerParaCommand.readCommand)
    {
        readControllerParaMemory();

        controllerParaCommand.readCommand = 0;
    }

    // Controller parameter 저장 실행
    if(controllerParaCommand.saveCommand)
    {
        saveControllerParaMemory();

        controllerParaCommand.saveCommand = 0;
    }
}


void saveControllerParaMemory(void)
{
    controllerPara.productDate[0] = rtcCalendar.year;
    controllerPara.productDate[1] = (rtcCalendar.month << 8) + rtcCalendar.date;

    // Model
    // controllerPara.model = Info_DrvModel[Para[DRIVER_MODEL].val].con_model;

    // Serial number  
    SPI_WriteMemoryData(CONTROLLER_PARA_SERIAL_NUMBER_ADDRESS, (uint8_t*)controllerPara.serialNumber, sizeof(controllerPara.serialNumber));

    // Product date
    SPI_WriteMemoryData(CONTROLLER_PARA_PRODUCT_DATE_ADDRESS, (uint8_t*)controllerPara.productDate, sizeof(controllerPara.productDate));


    // Mac address
    #if CONTROLLER_PARA_MAC_ADDRESS_ACTIVE
    Save_Mac_Address((controllerPara.macAddress[0] >> 8) & 0xFF, 0);
    Save_Mac_Address((controllerPara.macAddress[0] >> 0) & 0xFF, 1);
    Save_Mac_Address((controllerPara.macAddress[1] >> 8) & 0xFF, 2);
    Save_Mac_Address((controllerPara.macAddress[1] >> 0) & 0xFF, 3);
    Save_Mac_Address((controllerPara.macAddress[2] >> 8) & 0xFF, 4);
    Save_Mac_Address((controllerPara.macAddress[2] >> 0) & 0xFF, 5);
    #endif
}



void readControllerParaMemory(void)
{
    // Model
    controllerPara.model = Info_DrvModel[Para[DRIVER_MODEL].val].con_model;

    // Model name
    switch(controllerPara.model)
    {
        case BMT40:
            #ifdef HANTAS_FIRMWARE
                sprintf((char*)controllerPara.modelName, "%s", CONTROLLER_MODEL_NAME_HANTAS);
            #else
                sprintf((char*)controllerPara.modelName, "%s", CONTROLLER_MODEL_NAME_MOUNTZ);
            #endif
        break;
    }

    // Serial number  
    SPI_ReadMemoryData(CONTROLLER_PARA_SERIAL_NUMBER_ADDRESS, (uint8_t*)controllerPara.serialNumber, sizeof(controllerPara.serialNumber));

    // Product date
    SPI_ReadMemoryData(CONTROLLER_PARA_PRODUCT_DATE_ADDRESS, (uint8_t*)controllerPara.productDate, sizeof(controllerPara.productDate));

    // Mac address
    #if CONTROLLER_PARA_MAC_ADDRESS_ACTIVE        
        // Controller 내부 MAC 주소 읽기
        Read_Mac_Address();
    #endif
}




