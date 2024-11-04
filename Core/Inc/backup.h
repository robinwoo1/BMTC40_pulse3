
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BACKUP_H
#define _BACKUP_H

#include "stm32f7xx_hal.h"
#include "common.h"




/* Defines ---------------------------------------------------------------------------*/

#define PARAMETER_BACKUP_LIST_SIZE   5




/* Struct ---------------------------------------------------------------------------*/

typedef struct
{
    uint16_t    address;
    uint16_t    data;
} parameterBackListStruct;


typedef struct
{
    uint16_t    setAddress;
    uint16_t    setData;

    parameterBackListStruct list[PARAMETER_BACKUP_LIST_SIZE];

} parameterBackUpStruct;




/* Extern ---------------------------------------------------------------------------*/

extern parameterBackUpStruct parameterBackup;




/* Functions ---------------------------------------------------------------------------*/

uint16_t SPI_ReadPara(uint16_t paraNo);
void SPI_WritePara(uint16_t paraNo, uint16_t data);
void SPI_WriteMemoryData(uint16_t address, uint8_t* buf, uint16_t length);
void SPI_ReadMemoryData(uint16_t address, uint8_t* buf, uint16_t length);


void ParameterSaveWaitBuf(uint16_t address, uint16_t value, uint8_t index, uint16_t* addrBuf, uint16_t* valBuf);
int ParameterReadFromMemory(int parano);
void ParameterWriteToMemory(int setno, uint16_t data);


void ParameterInitial(void);
void Init_Para_limit(void);
void paraMinMaxCheck(void);
void paraRevisionMapping(void);


void SendSaveParameter(int pnum, uint16_t pvalue);
void mbSavePara(uint16_t data, uint16_t addr);
uint8_t selectEventType(void);

uint16_t ramp_up_speed_setting_min(void);
uint16_t ramp_up_speed_setting_default(void);
void Save_Converting_Torque_Speed(void);

void ParameterInitial_hh(void);
void default_ParaValueCalculation(float data);
void ParaChangeResetting(void);


float convertUnit(uint8_t currentUnit, uint8_t changeUnit, float value);





// Crowfoot 
float getCrowFootRatio(void);



#endif 
