/**
  ******************************************************************************
  * @file           : parameter.h
  * @brief          : Parameter manager header
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Hantas.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef PARAMETER_H
#define PARAMETER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "etcParameter.h"
#include "statusParameter.h"



/* Exported defines ----------------------------------------------------------*/

#define PARAMETER_REVISION                              PARAMETER_REVISION_2
#define PARAMETER_REVISION_2                            0x55AC
#define PARAMETER_REVISION_1                            0x55AB
#define PARAMETER_REVISION_0                            0x55AA


#define PARA_SIZE                                       3999
#define BACKUP_SIZE                                     PARA_SIZE-1


// 기능 별 파라미터 크기
#define PRESET_INDEX_SIZE                               31
#define MODEL_INDEX_SIZE                                15

#define PRESET_SIZE                                     30
#define ADV_PARA_SIZE		                            45
#define MODEL_SIZE                                      30


// 파라미터 주소 
#define PARA_START_VAL_ADDRESS                          0
#define PARA_END_VAL_ADDRESS                            3999
#define PARA_START_MIN_ADDRESS                          10000
#define PARA_END_MIN_ADDRESS                            13999
#define PARA_START_MAX_ADDRESS                          20000
#define PARA_END_MAX_ADDRESS                            23999

#define REMOTE_START_ADDRESS                            5000
#define REMOTE_END_ADDRESS                              5099

#define AUTO_CUSTOM_START_ADDRESS                       5100
#define AUTO_CUSTOM_END_ADDRESS                         5149

#define SENSOR_CAL_START_ADDRESS                        5150
#define SENSOR_CAL_END_ADDRESS                          5179

#define BIT_SOCKET_COMM_START_ADDRESS                   5180
#define BIT_SOCKET_COMM_END_ADDRESS                     5199

#define VIRTUAL_PRESET_START_ADDRESS                    5200
#define VIRTUAL_PRESET_END_ADDRESS                      5229

#define VIRTUAL_ADVANCE_START_ADDRESS                   5230
#define VIRTUAL_ADVANCE_END_ADDRESS                     5279

#define VIRTUAL_MODEL_START_ADDRESS                     5280
#define VIRTUAL_MODEL_END_ADDRESS                       5349

#define REALTIME_EVENT_START_ADDRESS                    5350
#define REALTIME_EVENT_END_ADDRESS                      5369

#define GRAPH_MONITORING_SETTING_START_ADDRESS          5370
#define GRAPH_MONITORING_SETTING_END_ADDRESS            5389

#define GRAPH_EVENT_START_ADDRESS                       5390
#define GRAPH_EVENT_END_ADDRESS                         5499

#define BARCODE_START_ADDRESS                           6000
#define BARCODE_END_ADDRESS                             6999

#define MODEL_BARCODE_START_ADDRESS                     7000
#define MODEL_BARCODE_END_ADDRESS                       7999

#define MAINTENANCE_START_ADDRESS                       8000
#define MAINTENANCE_END_ADDRESS                         8999

#define DRIVER_REG_START_ADDRESS                        9000
#define DRIVER_REG_END_ADDRESS                          9099

#define CONTROLLER_PARA_START_ADDRESS                   9100
#define CONTROLLER_PARA_END_ADDRESS                     9199







/* Exported types ------------------------------------------------------------*/

typedef enum
{
    VAL_UINT8,                      // uint8_t
    VAL_UINT16,                     // uint16_t
    VAL_UINT32_H,                   // uint32_t High
    VAL_UINT32_L,                   // uint32_t Low
    VAL_INT_32_H,                   // signed 32 High
    VAL_INT_32_L,                   // signed 32 Low
    VAL_FLOAT_H,                    // Float type High
    VAL_FLOAT_L                     // Float type Low

}VALUE_TYPE;


typedef enum
{
    VAL_SINGLE,                     // 해당 파라미터의 값만 반영
    VAL_FULL,                       // 해당 파라미터와 연관된 값까지 반영
    
}VALUE_JUDGE_TYPE;


typedef struct 
{
    uint16_t    val;                // Setted value
    uint16_t    dft;                // Default value
    uint16_t    min;                // Minium value
    uint16_t    max;                // Maxium value

    uint8_t     type;               // Value 형

} para;


typedef union {
 
    float fVal;
    struct
    {
 
        // Order is important.
        // Here the members of the union data structure
        // use the same memory (32 bits).
        // The ordering is taken
        // from the LSB to the MSB.
        uint16_t integer_low : 16;
        uint16_t integer_high : 16;
 
    } raw;
} floatFrom16;




/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
extern para Para[PARA_END_VAL_ADDRESS + 1];


/* Public functions ----------------------------------------------------------*/

float getParaVal(uint16_t address);
float getParaMin(uint16_t address);
float getParaMax(uint16_t address);
float getParaDef(uint16_t address);

uint16_t getParaValSingle(uint16_t address);
uint16_t getParaMinSingle(uint16_t address);
uint16_t getParaMaxSingle(uint16_t address);
uint16_t getParaDefSingle(uint16_t address);

void setParaVal(uint16_t address, float value);
void setParaMin(uint16_t address, float value);
void setParaMax(uint16_t address, float value);
void setParaDef(uint16_t address, float value);
void setTypeMin(uint16_t address, float value, para* buf, uint8_t type);
void setTypeMax(uint16_t address, float value, para* buf, uint8_t type);

void setParaValSingle(uint16_t address, uint16_t value);
void setParaMinSingle(uint16_t address, uint16_t value);
void setParaMaxSingle(uint16_t address, uint16_t value);
void setParaDefSingle(uint16_t address, uint16_t value);

void changeParaType(uint16_t address, uint8_t type);

float getFloatFrom16(uint16_t value1, uint16_t value2);



/* Enum - ADDRESS ----------------------------------------------------------*/

/* user parameter*/
const typedef enum
{   
    DUMMY_INDEX = 0,
    ZERO_DUMMY = 0,


    /* FASTENING ----------------------------------------------------------------------------- */

    TCAM_ACTM1 = 1,                                   // 0001           select torque/angle     
    FASTEN_TORQUE1_H,                                 // 0002           toque   
    FASTEN_TORQUE1_L,                                 // 0003           toque   
    TORQUE_MIN_MAX1_H,                                // 0004           %
    TORQUE_MIN_MAX1_L,                                // 0005           %
    TARGET_ANGLE1,                                    // 0006           degree
    FASTEN_MIN_TURNS1,                                // 0007           
    FASTEN_MAX_TURNS1,                                // 0008           
    SNUG_TORQUE1_H,                                   // 0009           
    SNUG_TORQUE1_L,                                   // 0010           
    FASTEN_SPEED1,                                    // 0011           speed
    FREE_FASTEN_ANGLE1,                               // 0012           0.1 turn
    FREE_FASTEN_SPEED1,                               // 0013           
    SOFT_START1,                                      // 0014           
    FASTEN_SEATTING_POINT_RATE1,                      // 0015           %
    FASTEN_TQ_RISING_TIME1,                           // 0016           ms
    RAMP_UP_SPEED1,                                   // 0017           ms
    TORQUE_COMPENSATION1,                             // 0018         
    TORQUE_OFFSET1,                                   // 0019
    MAX_FASTEN_PULSE_COUNT1,                          // 0020


    TCAM_ACTM2 =  31,                                 // 31
    FASTEN_TORQUE2_H,                               
    FASTEN_TORQUE2_L,                               
    TORQUE_MIN_MAX2_H,                              
    TORQUE_MIN_MAX2_L,                              
    TARGET_ANGLE2,                                  
    FASTEN_MIN_TURNS2,                              
    FASTEN_MAX_TURNS2,                              
    SNUG_TORQUE2_H,                                 
    SNUG_TORQUE2_L,                                 
    FASTEN_SPEED2,                                  
    FREE_FASTEN_ANGLE2,                             
    FREE_FASTEN_SPEED2,                             
    SOFT_START2,                                    
    FASTEN_SEATTING_POINT_RATE2,                    
    FASTEN_TQ_RISING_TIME2,                         
    RAMP_UP_SPEED2,                                 
    TORQUE_COMPENSATION2,                           
    TORQUE_OFFSET2,                                   
    MAX_FASTEN_PULSE_COUNT2,

    TCAM_ACTM3 = 61,                                  // 61
    FASTEN_TORQUE3_H,                               
    FASTEN_TORQUE3_L,                               
    TORQUE_MIN_MAX3_H,                              
    TORQUE_MIN_MAX3_L,                              
    TARGET_ANGLE3,                                  
    FASTEN_MIN_TURNS3,                              
    FASTEN_MAX_TURNS3,                              
    SNUG_TORQUE3_H,                                 
    SNUG_TORQUE3_L,                                 
    FASTEN_SPEED3,                                  
    FREE_FASTEN_ANGLE3,                             
    FREE_FASTEN_SPEED3,                             
    SOFT_START3,                                    
    FASTEN_SEATTING_POINT_RATE3,                    
    FASTEN_TQ_RISING_TIME3,                         
    RAMP_UP_SPEED3,                                 
    TORQUE_COMPENSATION3,                           
    TORQUE_OFFSET3,                                   
    MAX_FASTEN_PULSE_COUNT3,

    TCAM_ACTM4 = 91,                                  // 91
    FASTEN_TORQUE4_H,                               
    FASTEN_TORQUE4_L,                               
    TORQUE_MIN_MAX4_H,                              
    TORQUE_MIN_MAX4_L,                              
    TARGET_ANGLE4,                                  
    FASTEN_MIN_TURNS4,                              
    FASTEN_MAX_TURNS4,                              
    SNUG_TORQUE4_H,                                 
    SNUG_TORQUE4_L,                                 
    FASTEN_SPEED4,                                  
    FREE_FASTEN_ANGLE4,                             
    FREE_FASTEN_SPEED4,                             
    SOFT_START4,                                    
    FASTEN_SEATTING_POINT_RATE4,                    
    FASTEN_TQ_RISING_TIME4,                         
    RAMP_UP_SPEED4,                                 
    TORQUE_COMPENSATION4,                           
    TORQUE_OFFSET4,                                   
    MAX_FASTEN_PULSE_COUNT4,

    TCAM_ACTM5 = 121,                                 // 121
    FASTEN_TORQUE5_H,                               
    FASTEN_TORQUE5_L,                               
    TORQUE_MIN_MAX5_H,                              
    TORQUE_MIN_MAX5_L,                              
    TARGET_ANGLE5,                                  
    FASTEN_MIN_TURNS5,                              
    FASTEN_MAX_TURNS5,                              
    SNUG_TORQUE5_H,                                 
    SNUG_TORQUE5_L,                                 
    FASTEN_SPEED5,                                  
    FREE_FASTEN_ANGLE5,                             
    FREE_FASTEN_SPEED5,                             
    SOFT_START5,                                    
    FASTEN_SEATTING_POINT_RATE5,                    
    FASTEN_TQ_RISING_TIME5,                         
    RAMP_UP_SPEED5,                                 
    TORQUE_COMPENSATION5,                           
    TORQUE_OFFSET5,                                   
    MAX_FASTEN_PULSE_COUNT5,

    TCAM_ACTM6 = 151,                                 // 151
    FASTEN_TORQUE6_H,                               
    FASTEN_TORQUE6_L,                               
    TORQUE_MIN_MAX6_H,                              
    TORQUE_MIN_MAX6_L,                              
    TARGET_ANGLE6,                                  
    FASTEN_MIN_TURNS6,                              
    FASTEN_MAX_TURNS6,                              
    SNUG_TORQUE6_H,                                 
    SNUG_TORQUE6_L,                                 
    FASTEN_SPEED6,                                  
    FREE_FASTEN_ANGLE6,                             
    FREE_FASTEN_SPEED6,                             
    SOFT_START6,                                    
    FASTEN_SEATTING_POINT_RATE6,                    
    FASTEN_TQ_RISING_TIME6,                         
    RAMP_UP_SPEED6,                                 
    TORQUE_COMPENSATION6,                           
    TORQUE_OFFSET6,                                   
    MAX_FASTEN_PULSE_COUNT6,

    TCAM_ACTM7 = 181,                                 // 181
    FASTEN_TORQUE7_H,                               
    FASTEN_TORQUE7_L,                               
    TORQUE_MIN_MAX7_H,                              
    TORQUE_MIN_MAX7_L,                              
    TARGET_ANGLE7,                                  
    FASTEN_MIN_TURNS7,                              
    FASTEN_MAX_TURNS7,                              
    SNUG_TORQUE7_H,                                 
    SNUG_TORQUE7_L,                                 
    FASTEN_SPEED7,                                  
    FREE_FASTEN_ANGLE7,                             
    FREE_FASTEN_SPEED7,                             
    SOFT_START7,                                    
    FASTEN_SEATTING_POINT_RATE7,                    
    FASTEN_TQ_RISING_TIME7,                         
    RAMP_UP_SPEED7,                                 
    TORQUE_COMPENSATION7,                           
    TORQUE_OFFSET7,                                   
    MAX_FASTEN_PULSE_COUNT7,

    TCAM_ACTM8 = 211,                                 // 211
    FASTEN_TORQUE8_H,                               
    FASTEN_TORQUE8_L,                               
    TORQUE_MIN_MAX8_H,                              
    TORQUE_MIN_MAX8_L,                              
    TARGET_ANGLE8,                                  
    FASTEN_MIN_TURNS8,                              
    FASTEN_MAX_TURNS8,                              
    SNUG_TORQUE8_H,                                 
    SNUG_TORQUE8_L,                                 
    FASTEN_SPEED8,                                  
    FREE_FASTEN_ANGLE8,                             
    FREE_FASTEN_SPEED8,                             
    SOFT_START8,                                    
    FASTEN_SEATTING_POINT_RATE8,                    
    FASTEN_TQ_RISING_TIME8,                         
    RAMP_UP_SPEED8,                                 
    TORQUE_COMPENSATION8,                           
    TORQUE_OFFSET8,                                   
    MAX_FASTEN_PULSE_COUNT8,

    TCAM_ACTM9 = 241,                                 // 241
    FASTEN_TORQUE9_H,                               
    FASTEN_TORQUE9_L,                               
    TORQUE_MIN_MAX9_H,                              
    TORQUE_MIN_MAX9_L,                              
    TARGET_ANGLE9,                                  
    FASTEN_MIN_TURNS9,                              
    FASTEN_MAX_TURNS9,                              
    SNUG_TORQUE9_H,                                 
    SNUG_TORQUE9_L,                                 
    FASTEN_SPEED9,                                  
    FREE_FASTEN_ANGLE9,                             
    FREE_FASTEN_SPEED9,                             
    SOFT_START9,                                    
    FASTEN_SEATTING_POINT_RATE9,                    
    FASTEN_TQ_RISING_TIME9,                         
    RAMP_UP_SPEED9,                                 
    TORQUE_COMPENSATION9,                           
    TORQUE_OFFSET9,                                   
    MAX_FASTEN_PULSE_COUNT9,

    TCAM_ACTM10 = 271,                                 // 271
    FASTEN_TORQUE10_H,                               
    FASTEN_TORQUE10_L,                               
    TORQUE_MIN_MAX10_H,                              
    TORQUE_MIN_MAX10_L,                              
    TARGET_ANGLE10,                                  
    FASTEN_MIN_TURNS10,                              
    FASTEN_MAX_TURNS10,                              
    SNUG_TORQUE10_H,                                 
    SNUG_TORQUE10_L,                                 
    FASTEN_SPEED10,                                  
    FREE_FASTEN_ANGLE10,                             
    FREE_FASTEN_SPEED10,                             
    SOFT_START10,                                    
    FASTEN_SEATTING_POINT_RATE10,                    
    FASTEN_TQ_RISING_TIME10,                         
    RAMP_UP_SPEED10,                                 
    TORQUE_COMPENSATION10,                           
    TORQUE_OFFSET10,                                   
    MAX_FASTEN_PULSE_COUNT10,

    TCAM_ACTM11 = 301,                                 // 301
    FASTEN_TORQUE11_H,                               
    FASTEN_TORQUE11_L,                               
    TORQUE_MIN_MAX11_H,                              
    TORQUE_MIN_MAX11_L,                              
    TARGET_ANGLE11,                                  
    FASTEN_MIN_TURNS11,                              
    FASTEN_MAX_TURNS11,                              
    SNUG_TORQUE11_H,                                 
    SNUG_TORQUE11_L,                                 
    FASTEN_SPEED11,                                  
    FREE_FASTEN_ANGLE11,                             
    FREE_FASTEN_SPEED11,                             
    SOFT_START11,                                    
    FASTEN_SEATTING_POINT_RATE11,                    
    FASTEN_TQ_RISING_TIME11,                         
    RAMP_UP_SPEED11,                                 
    TORQUE_COMPENSATION11,                           
    TORQUE_OFFSET11,                                   
    MAX_FASTEN_PULSE_COUNT11,

    TCAM_ACTM12 = 331,                                 // 331
    FASTEN_TORQUE12_H,                               
    FASTEN_TORQUE12_L,                               
    TORQUE_MIN_MAX12_H,                              
    TORQUE_MIN_MAX12_L,                              
    TARGET_ANGLE12,                                  
    FASTEN_MIN_TURNS12,                              
    FASTEN_MAX_TURNS12,                              
    SNUG_TORQUE12_H,                                 
    SNUG_TORQUE12_L,                                 
    FASTEN_SPEED12,                                  
    FREE_FASTEN_ANGLE12,                             
    FREE_FASTEN_SPEED12,                             
    SOFT_START12,                                    
    FASTEN_SEATTING_POINT_RATE12,                    
    FASTEN_TQ_RISING_TIME12,                         
    RAMP_UP_SPEED12,                                 
    TORQUE_COMPENSATION12,                           
    TORQUE_OFFSET12,                                   
    MAX_FASTEN_PULSE_COUNT12,

    TCAM_ACTM13 = 361,                                 // 361
    FASTEN_TORQUE13_H,                               
    FASTEN_TORQUE13_L,                               
    TORQUE_MIN_MAX13_H,                              
    TORQUE_MIN_MAX13_L,                              
    TARGET_ANGLE13,                                  
    FASTEN_MIN_TURNS13,                              
    FASTEN_MAX_TURNS13,                              
    SNUG_TORQUE13_H,                                 
    SNUG_TORQUE13_L,                                 
    FASTEN_SPEED13,                                  
    FREE_FASTEN_ANGLE13,                             
    FREE_FASTEN_SPEED13,                             
    SOFT_START13,                                    
    FASTEN_SEATTING_POINT_RATE13,                    
    FASTEN_TQ_RISING_TIME13,                         
    RAMP_UP_SPEED13,                                 
    TORQUE_COMPENSATION13,                           
    TORQUE_OFFSET13,                                   
    MAX_FASTEN_PULSE_COUNT13,

    TCAM_ACTM14 = 391,                                 // 391
    FASTEN_TORQUE14_H,                               
    FASTEN_TORQUE14_L,                               
    TORQUE_MIN_MAX14_H,                              
    TORQUE_MIN_MAX14_L,                              
    TARGET_ANGLE14,                                  
    FASTEN_MIN_TURNS14,                              
    FASTEN_MAX_TURNS14,                              
    SNUG_TORQUE14_H,                                 
    SNUG_TORQUE14_L,                                 
    FASTEN_SPEED14,                                  
    FREE_FASTEN_ANGLE14,                             
    FREE_FASTEN_SPEED14,                             
    SOFT_START14,                                    
    FASTEN_SEATTING_POINT_RATE14,                    
    FASTEN_TQ_RISING_TIME14,                         
    RAMP_UP_SPEED14,                                 
    TORQUE_COMPENSATION14,                           
    TORQUE_OFFSET14,
    MAX_FASTEN_PULSE_COUNT14,

    TCAM_ACTM15 = 421,                                 // 421
    FASTEN_TORQUE15_H,                               
    FASTEN_TORQUE15_L,                               
    TORQUE_MIN_MAX15_H,                              
    TORQUE_MIN_MAX15_L,                              
    TARGET_ANGLE15,                                  
    FASTEN_MIN_TURNS15,                              
    FASTEN_MAX_TURNS15,                              
    SNUG_TORQUE15_H,                                 
    SNUG_TORQUE15_L,                                 
    FASTEN_SPEED15,                                  
    FREE_FASTEN_ANGLE15,                             
    FREE_FASTEN_SPEED15,                             
    SOFT_START15,                                    
    FASTEN_SEATTING_POINT_RATE15,                    
    FASTEN_TQ_RISING_TIME15,                         
    RAMP_UP_SPEED15,                                 
    TORQUE_COMPENSATION15,       
    TORQUE_OFFSET15,                                   
    MAX_FASTEN_PULSE_COUNT15,

    TCAM_ACTM16 = 451,                                 // 451
    FASTEN_TORQUE16_H,                               
    FASTEN_TORQUE16_L,                               
    TORQUE_MIN_MAX16_H,                              
    TORQUE_MIN_MAX16_L,                              
    TARGET_ANGLE16,                                  
    FASTEN_MIN_TURNS16,                              
    FASTEN_MAX_TURNS16,                              
    SNUG_TORQUE16_H,                                 
    SNUG_TORQUE16_L,                                 
    FASTEN_SPEED16,                                  
    FREE_FASTEN_ANGLE16,                             
    FREE_FASTEN_SPEED16,                             
    SOFT_START16,                                    
    FASTEN_SEATTING_POINT_RATE16,                    
    FASTEN_TQ_RISING_TIME16,                         
    RAMP_UP_SPEED16,                                 
    TORQUE_COMPENSATION16,       
    TORQUE_OFFSET16,                                   
    MAX_FASTEN_PULSE_COUNT16,

    TCAM_ACTM17 = 481,                                 // 481
    FASTEN_TORQUE17_H,                               
    FASTEN_TORQUE17_L,                               
    TORQUE_MIN_MAX17_H,                              
    TORQUE_MIN_MAX17_L,                              
    TARGET_ANGLE17,                                  
    FASTEN_MIN_TURNS17,                              
    FASTEN_MAX_TURNS17,                              
    SNUG_TORQUE17_H,                                 
    SNUG_TORQUE17_L,                                 
    FASTEN_SPEED17,                                  
    FREE_FASTEN_ANGLE17,                             
    FREE_FASTEN_SPEED17,                             
    SOFT_START17,                                    
    FASTEN_SEATTING_POINT_RATE17,                    
    FASTEN_TQ_RISING_TIME17,                         
    RAMP_UP_SPEED17,                                 
    TORQUE_COMPENSATION17,       
    TORQUE_OFFSET17,                                   
    MAX_FASTEN_PULSE_COUNT17,

    TCAM_ACTM18 = 511,                                 // 511
    FASTEN_TORQUE18_H,                               
    FASTEN_TORQUE18_L,                               
    TORQUE_MIN_MAX18_H,                              
    TORQUE_MIN_MAX18_L,                              
    TARGET_ANGLE18,                                  
    FASTEN_MIN_TURNS18,                              
    FASTEN_MAX_TURNS18,                              
    SNUG_TORQUE18_H,                                 
    SNUG_TORQUE18_L,                                 
    FASTEN_SPEED18,                                  
    FREE_FASTEN_ANGLE18,                             
    FREE_FASTEN_SPEED18,                             
    SOFT_START18,                                    
    FASTEN_SEATTING_POINT_RATE18,                    
    FASTEN_TQ_RISING_TIME18,                         
    RAMP_UP_SPEED18,                                 
    TORQUE_COMPENSATION18,       
    TORQUE_OFFSET18,                                   
    MAX_FASTEN_PULSE_COUNT18,

    TCAM_ACTM19 = 541,                                 // 541
    FASTEN_TORQUE19_H,                               
    FASTEN_TORQUE19_L,                               
    TORQUE_MIN_MAX19_H,                              
    TORQUE_MIN_MAX19_L,                              
    TARGET_ANGLE19,                                  
    FASTEN_MIN_TURNS19,                              
    FASTEN_MAX_TURNS19,                              
    SNUG_TORQUE19_H,                                 
    SNUG_TORQUE19_L,                                 
    FASTEN_SPEED19,                                  
    FREE_FASTEN_ANGLE19,                             
    FREE_FASTEN_SPEED19,                             
    SOFT_START19,                                    
    FASTEN_SEATTING_POINT_RATE19,                    
    FASTEN_TQ_RISING_TIME19,                         
    RAMP_UP_SPEED19,                                 
    TORQUE_COMPENSATION19,       
    TORQUE_OFFSET19,                                   
    MAX_FASTEN_PULSE_COUNT19,

    TCAM_ACTM20 = 571,                                 // 571
    FASTEN_TORQUE20_H,                               
    FASTEN_TORQUE20_L,                               
    TORQUE_MIN_MAX20_H,                              
    TORQUE_MIN_MAX20_L,                              
    TARGET_ANGLE20,                                  
    FASTEN_MIN_TURNS20,                              
    FASTEN_MAX_TURNS20,                              
    SNUG_TORQUE20_H,                                 
    SNUG_TORQUE20_L,                                 
    FASTEN_SPEED20,                                  
    FREE_FASTEN_ANGLE20,                             
    FREE_FASTEN_SPEED20,                             
    SOFT_START20,                                    
    FASTEN_SEATTING_POINT_RATE20,                    
    FASTEN_TQ_RISING_TIME20,                         
    RAMP_UP_SPEED20,                                 
    TORQUE_COMPENSATION20,       
    TORQUE_OFFSET20,                                   
    MAX_FASTEN_PULSE_COUNT20,

    TCAM_ACTM21 = 601,                                 // 601
    FASTEN_TORQUE21_H,                               
    FASTEN_TORQUE21_L,                               
    TORQUE_MIN_MAX21_H,                              
    TORQUE_MIN_MAX21_L,                              
    TARGET_ANGLE21,                                  
    FASTEN_MIN_TURNS21,                              
    FASTEN_MAX_TURNS21,                              
    SNUG_TORQUE21_H,                                 
    SNUG_TORQUE21_L,                                 
    FASTEN_SPEED21,                                  
    FREE_FASTEN_ANGLE21,                             
    FREE_FASTEN_SPEED21,                             
    SOFT_START21,                                    
    FASTEN_SEATTING_POINT_RATE21,                    
    FASTEN_TQ_RISING_TIME21,                         
    RAMP_UP_SPEED21,                                 
    TORQUE_COMPENSATION21,       
    TORQUE_OFFSET21,                                   
    MAX_FASTEN_PULSE_COUNT21,

    TCAM_ACTM22 = 631,                                 // 631
    FASTEN_TORQUE22_H,                               
    FASTEN_TORQUE22_L,                               
    TORQUE_MIN_MAX22_H,                              
    TORQUE_MIN_MAX22_L,                              
    TARGET_ANGLE22,                                  
    FASTEN_MIN_TURNS22,                              
    FASTEN_MAX_TURNS22,                              
    SNUG_TORQUE22_H,                                 
    SNUG_TORQUE22_L,                                 
    FASTEN_SPEED22,                                  
    FREE_FASTEN_ANGLE22,                             
    FREE_FASTEN_SPEED22,                             
    SOFT_START22,                                    
    FASTEN_SEATTING_POINT_RATE22,                    
    FASTEN_TQ_RISING_TIME22,                         
    RAMP_UP_SPEED22,                                 
    TORQUE_COMPENSATION22,       
    TORQUE_OFFSET22,                                   
    MAX_FASTEN_PULSE_COUNT22,

    TCAM_ACTM23 = 661,                                 // 661
    FASTEN_TORQUE23_H,                               
    FASTEN_TORQUE23_L,                               
    TORQUE_MIN_MAX23_H,                              
    TORQUE_MIN_MAX23_L,                              
    TARGET_ANGLE23,                                  
    FASTEN_MIN_TURNS23,                              
    FASTEN_MAX_TURNS23,                              
    SNUG_TORQUE23_H,                                 
    SNUG_TORQUE23_L,                                 
    FASTEN_SPEED23,                                  
    FREE_FASTEN_ANGLE23,                             
    FREE_FASTEN_SPEED23,                             
    SOFT_START23,                                    
    FASTEN_SEATTING_POINT_RATE23,                    
    FASTEN_TQ_RISING_TIME23,                         
    RAMP_UP_SPEED23,                                 
    TORQUE_COMPENSATION23,       
    TORQUE_OFFSET23,                                   
    MAX_FASTEN_PULSE_COUNT23,

    TCAM_ACTM24 = 691,                                 // 691
    FASTEN_TORQUE24_H,                               
    FASTEN_TORQUE24_L,                               
    TORQUE_MIN_MAX24_H,                              
    TORQUE_MIN_MAX24_L,                              
    TARGET_ANGLE24,                                  
    FASTEN_MIN_TURNS24,                              
    FASTEN_MAX_TURNS24,                              
    SNUG_TORQUE24_H,                                 
    SNUG_TORQUE24_L,                                 
    FASTEN_SPEED24,                                  
    FREE_FASTEN_ANGLE24,                             
    FREE_FASTEN_SPEED24,                             
    SOFT_START24,                                    
    FASTEN_SEATTING_POINT_RATE24,                    
    FASTEN_TQ_RISING_TIME24,                         
    RAMP_UP_SPEED24,                                 
    TORQUE_COMPENSATION24,       
    TORQUE_OFFSET24,                                   
    MAX_FASTEN_PULSE_COUNT24,

    TCAM_ACTM25 = 721,                                 // 721
    FASTEN_TORQUE25_H,                               
    FASTEN_TORQUE25_L,                               
    TORQUE_MIN_MAX25_H,                              
    TORQUE_MIN_MAX25_L,                              
    TARGET_ANGLE25,                                  
    FASTEN_MIN_TURNS25,                              
    FASTEN_MAX_TURNS25,                              
    SNUG_TORQUE25_H,                                 
    SNUG_TORQUE25_L,                                 
    FASTEN_SPEED25,                                  
    FREE_FASTEN_ANGLE25,                             
    FREE_FASTEN_SPEED25,                             
    SOFT_START25,                                    
    FASTEN_SEATTING_POINT_RATE25,                    
    FASTEN_TQ_RISING_TIME25,                         
    RAMP_UP_SPEED25,                                 
    TORQUE_COMPENSATION25,       
    TORQUE_OFFSET25,                                   
    MAX_FASTEN_PULSE_COUNT25,

    TCAM_ACTM26 = 751,                                 // 751
    FASTEN_TORQUE26_H,                               
    FASTEN_TORQUE26_L,                               
    TORQUE_MIN_MAX26_H,                              
    TORQUE_MIN_MAX26_L,                              
    TARGET_ANGLE26,                                  
    FASTEN_MIN_TURNS26,                              
    FASTEN_MAX_TURNS26,                              
    SNUG_TORQUE26_H,                                 
    SNUG_TORQUE26_L,                                 
    FASTEN_SPEED26,                                  
    FREE_FASTEN_ANGLE26,                             
    FREE_FASTEN_SPEED26,                             
    SOFT_START26,                                    
    FASTEN_SEATTING_POINT_RATE26,                    
    FASTEN_TQ_RISING_TIME26,                         
    RAMP_UP_SPEED26,                                 
    TORQUE_COMPENSATION26,       
    TORQUE_OFFSET26,                                   
    MAX_FASTEN_PULSE_COUNT26,

    TCAM_ACTM27 = 781,                                 // 781
    FASTEN_TORQUE27_H,                               
    FASTEN_TORQUE27_L,                               
    TORQUE_MIN_MAX27_H,                              
    TORQUE_MIN_MAX27_L,                              
    TARGET_ANGLE27,                                  
    FASTEN_MIN_TURNS27,                              
    FASTEN_MAX_TURNS27,                              
    SNUG_TORQUE27_H,                                 
    SNUG_TORQUE27_L,                                 
    FASTEN_SPEED27,                                  
    FREE_FASTEN_ANGLE27,                             
    FREE_FASTEN_SPEED27,                             
    SOFT_START27,                                    
    FASTEN_SEATTING_POINT_RATE27,                    
    FASTEN_TQ_RISING_TIME27,                         
    RAMP_UP_SPEED27,                                 
    TORQUE_COMPENSATION27,       
    TORQUE_OFFSET27,                                   
    MAX_FASTEN_PULSE_COUNT27,

    TCAM_ACTM28 = 811,                                 // 811
    FASTEN_TORQUE28_H,                               
    FASTEN_TORQUE28_L,                               
    TORQUE_MIN_MAX28_H,                              
    TORQUE_MIN_MAX28_L,                              
    TARGET_ANGLE28,                                  
    FASTEN_MIN_TURNS28,                              
    FASTEN_MAX_TURNS28,                              
    SNUG_TORQUE28_H,                                 
    SNUG_TORQUE28_L,                                 
    FASTEN_SPEED28,                                  
    FREE_FASTEN_ANGLE28,                             
    FREE_FASTEN_SPEED28,                             
    SOFT_START28,                                    
    FASTEN_SEATTING_POINT_RATE28,                    
    FASTEN_TQ_RISING_TIME28,                         
    RAMP_UP_SPEED28,                                 
    TORQUE_COMPENSATION28,       
    TORQUE_OFFSET28,                                   
    MAX_FASTEN_PULSE_COUNT28,

    TCAM_ACTM29 = 841,                                 // 841
    FASTEN_TORQUE29_H,                               
    FASTEN_TORQUE29_L,                               
    TORQUE_MIN_MAX29_H,                              
    TORQUE_MIN_MAX29_L,                              
    TARGET_ANGLE29,                                  
    FASTEN_MIN_TURNS29,                              
    FASTEN_MAX_TURNS29,                              
    SNUG_TORQUE29_H,                                 
    SNUG_TORQUE29_L,                                 
    FASTEN_SPEED29,                                  
    FREE_FASTEN_ANGLE29,                             
    FREE_FASTEN_SPEED29,                             
    SOFT_START29,                                    
    FASTEN_SEATTING_POINT_RATE29,                    
    FASTEN_TQ_RISING_TIME29,                         
    RAMP_UP_SPEED29,                                 
    TORQUE_COMPENSATION29,       
    TORQUE_OFFSET29,                                   
    MAX_FASTEN_PULSE_COUNT29,

    TCAM_ACTM30 = 871,                                 // 871
    FASTEN_TORQUE30_H,                               
    FASTEN_TORQUE30_L,                               
    TORQUE_MIN_MAX30_H,                              
    TORQUE_MIN_MAX30_L,                              
    TARGET_ANGLE30,                                  
    FASTEN_MIN_TURNS30,                              
    FASTEN_MAX_TURNS30,                              
    SNUG_TORQUE30_H,                                 
    SNUG_TORQUE30_L,                                 
    FASTEN_SPEED30,                                  
    FREE_FASTEN_ANGLE30,                             
    FREE_FASTEN_SPEED30,                             
    SOFT_START30,                                    
    FASTEN_SEATTING_POINT_RATE30,                    
    FASTEN_TQ_RISING_TIME30,                         
    RAMP_UP_SPEED30,                                 
    TORQUE_COMPENSATION30,       
    TORQUE_OFFSET30,                                   
    MAX_FASTEN_PULSE_COUNT30,

    TCAM_ACTM31 = 901,                                 // 901
    FASTEN_TORQUE31_H,                               
    FASTEN_TORQUE31_L,                               
    TORQUE_MIN_MAX31_H,                              
    TORQUE_MIN_MAX31_L,                              
    TARGET_ANGLE31,                                  
    FASTEN_MIN_TURNS31,                              
    FASTEN_MAX_TURNS31,                              
    SNUG_TORQUE31_H,                                 
    SNUG_TORQUE31_L,                                 
    FASTEN_SPEED31,                                  
    FREE_FASTEN_ANGLE31,                             
    FREE_FASTEN_SPEED31,                             
    SOFT_START31,                                    
    FASTEN_SEATTING_POINT_RATE31,                    
    FASTEN_TQ_RISING_TIME31,                         
    RAMP_UP_SPEED31,                                 
    TORQUE_COMPENSATION31,       
    TORQUE_OFFSET31,                                   
    MAX_FASTEN_PULSE_COUNT31,





    // < ADVANCED ----------------------------------------------------------------------------- >

    ADVANCED_MODE1 = 1000,						      // 1000
    ADVANCED_PARA1_1_H,							      // 1001     
    ADVANCED_PARA1_1_L,							      // 1002    
    ADVANCED_PARA1_2_H,							      // 1003          
    ADVANCED_PARA1_2_L,							      // 1004          
    ADVANCED_PARA1_3_H,							      // 1005          
    ADVANCED_PARA1_3_L,							      // 1006          
    ADVANCED_PARA1_4_H,							      // 1007          
    ADVANCED_PARA1_4_L,							      // 1008         
    ADVANCED_PARA1_5_H,							      // 1009         
    ADVANCED_PARA1_5_L,							      // 1010          
    ADVANCED_PARA1_6_H,							      // 1011         
    ADVANCED_PARA1_6_L,							      // 1012         
    ADVANCED_PARA1_7_H,							      // 1013         
    ADVANCED_PARA1_7_L,							      // 1014         
    ADVANCED_PARA1_8_H,							      // 1015         
    ADVANCED_PARA1_8_L,							      // 1016         
    ADVANCED_PARA1_9_H,							      // 1017         
    ADVANCED_PARA1_9_L,							      // 1018         
    ADVANCED_PARA1_10_H,							  // 1019            
    ADVANCED_PARA1_10_L,							  // 1020           
    ADVANCED_PARA1_11_H,							  // 1021            
    ADVANCED_PARA1_11_L,							  // 1022           
    ADVANCED_PARA1_12_H,							  // 1023           
    ADVANCED_PARA1_12_L,							  // 1024           
    ADVANCED_PARA1_13_H,							  // 1025           
    ADVANCED_PARA1_13_L,							  // 1026           
    ADVANCED_PARA1_14_H,							  // 1027           
    ADVANCED_PARA1_14_L,							  // 1028 
    ADVANCED_PARA1_15_H,							  // 1029           
    ADVANCED_PARA1_15_L,							  // 1030
    ADVANCED_PARA1_16_H,							  // 1031           
    ADVANCED_PARA1_16_L,							  // 1032 
    ADVANCED_PARA1_17_H,							  // 1033           
    ADVANCED_PARA1_17_L,							  // 1034 
    ADVANCED_PARA1_18_H,							  // 1035           
    ADVANCED_PARA1_18_L,							  // 1036 
    ADVANCED_PARA1_19_H,							  // 1037           
    ADVANCED_PARA1_19_L,							  // 1038 

    FREE_REVERSE_ROTATION_SPEED1,				      // 1039      
    FREE_REVERSE_ROTATION_ANGLE1_H,				      // 1040   
    FREE_REVERSE_ROTATION_ANGLE1_L,				      // 1041   
    REVERS_ANGLE_SETTING_SPEED1,				      // 1042     
    REVERS_ANGLE_SETTING_ANGLE1,				      // 1043     
    REVERS_ANGLE_SETTING_FW_REV1,				      // 1044     


    ADVANCED_MODE2 = 1045,			                  // 1045
    ADVANCED_PARA2_1_H,							                
    ADVANCED_PARA2_1_L,							                
    ADVANCED_PARA2_2_H,							                
    ADVANCED_PARA2_2_L,							                
    ADVANCED_PARA2_3_H,							                
    ADVANCED_PARA2_3_L,							                
    ADVANCED_PARA2_4_H,							                
    ADVANCED_PARA2_4_L,							                
    ADVANCED_PARA2_5_H,							                
    ADVANCED_PARA2_5_L,							                
    ADVANCED_PARA2_6_H,							                
    ADVANCED_PARA2_6_L,							                
    ADVANCED_PARA2_7_H,							                
    ADVANCED_PARA2_7_L,							                
    ADVANCED_PARA2_8_H,							                
    ADVANCED_PARA2_8_L,							                
    ADVANCED_PARA2_9_H,							                
    ADVANCED_PARA2_9_L,							                
    ADVANCED_PARA2_10_H,							              
    ADVANCED_PARA2_10_L,							              
    ADVANCED_PARA2_11_H,							              
    ADVANCED_PARA2_11_L,							              
    ADVANCED_PARA2_12_H,							              
    ADVANCED_PARA2_12_L,							              
    ADVANCED_PARA2_13_H,							              
    ADVANCED_PARA2_13_L,							              
    ADVANCED_PARA2_14_H,							              
    ADVANCED_PARA2_14_L,							              
    ADVANCED_PARA2_15_H,							             
    ADVANCED_PARA2_15_L,							  
    ADVANCED_PARA2_16_H,							             
    ADVANCED_PARA2_16_L,							   
    ADVANCED_PARA2_17_H,							             
    ADVANCED_PARA2_17_L,							   
    ADVANCED_PARA2_18_H,							             
    ADVANCED_PARA2_18_L,							   
    ADVANCED_PARA2_19_H,							             
    ADVANCED_PARA2_19_L,							   

    FREE_REVERSE_ROTATION_SPEED2,				            
    FREE_REVERSE_ROTATION_ANGLE2_H,				          
    FREE_REVERSE_ROTATION_ANGLE2_L,				          
    REVERS_ANGLE_SETTING_SPEED2,				            
    REVERS_ANGLE_SETTING_ANGLE2,				            
    REVERS_ANGLE_SETTING_FW_REV2,				            


    ADVANCED_MODE3 = 1090,			                  // 1090
    ADVANCED_PARA3_1_H,							                
    ADVANCED_PARA3_1_L,							                
    ADVANCED_PARA3_2_H,							                
    ADVANCED_PARA3_2_L,							                
    ADVANCED_PARA3_3_H,							                
    ADVANCED_PARA3_3_L,							                
    ADVANCED_PARA3_4_H,							                
    ADVANCED_PARA3_4_L,							                
    ADVANCED_PARA3_5_H,							                
    ADVANCED_PARA3_5_L,							                
    ADVANCED_PARA3_6_H,							                
    ADVANCED_PARA3_6_L,							                
    ADVANCED_PARA3_7_H,							                
    ADVANCED_PARA3_7_L,							                
    ADVANCED_PARA3_8_H,							                
    ADVANCED_PARA3_8_L,							                
    ADVANCED_PARA3_9_H,							                
    ADVANCED_PARA3_9_L,							                
    ADVANCED_PARA3_10_H,							              
    ADVANCED_PARA3_10_L,							              
    ADVANCED_PARA3_11_H,							              
    ADVANCED_PARA3_11_L,							              
    ADVANCED_PARA3_12_H,							              
    ADVANCED_PARA3_12_L,							              
    ADVANCED_PARA3_13_H,							              
    ADVANCED_PARA3_13_L,							              
    ADVANCED_PARA3_14_H,							              
    ADVANCED_PARA3_14_L,							              
    ADVANCED_PARA3_15_H,							             
    ADVANCED_PARA3_15_L,							  
    ADVANCED_PARA3_16_H,							             
    ADVANCED_PARA3_16_L,							   
    ADVANCED_PARA3_17_H,							             
    ADVANCED_PARA3_17_L,							   
    ADVANCED_PARA3_18_H,							             
    ADVANCED_PARA3_18_L,							   
    ADVANCED_PARA3_19_H,							             
    ADVANCED_PARA3_19_L,							   

    FREE_REVERSE_ROTATION_SPEED3,				            
    FREE_REVERSE_ROTATION_ANGLE3_H,				          
    FREE_REVERSE_ROTATION_ANGLE3_L,				          
    REVERS_ANGLE_SETTING_SPEED3,				            
    REVERS_ANGLE_SETTING_ANGLE3,				            
    REVERS_ANGLE_SETTING_FW_REV3,				            


    ADVANCED_MODE4 = 1135,			                  // 1135
    ADVANCED_PARA4_1_H,							                
    ADVANCED_PARA4_1_L,							                
    ADVANCED_PARA4_2_H,							                
    ADVANCED_PARA4_2_L,							                
    ADVANCED_PARA4_3_H,							                
    ADVANCED_PARA4_3_L,							                
    ADVANCED_PARA4_4_H,							                
    ADVANCED_PARA4_4_L,							                
    ADVANCED_PARA4_5_H,							                
    ADVANCED_PARA4_5_L,							                
    ADVANCED_PARA4_6_H,							                
    ADVANCED_PARA4_6_L,							                
    ADVANCED_PARA4_7_H,							                
    ADVANCED_PARA4_7_L,							                
    ADVANCED_PARA4_8_H,							                
    ADVANCED_PARA4_8_L,							                
    ADVANCED_PARA4_9_H,							                
    ADVANCED_PARA4_9_L,							                
    ADVANCED_PARA4_10_H,							              
    ADVANCED_PARA4_10_L,							              
    ADVANCED_PARA4_11_H,							              
    ADVANCED_PARA4_11_L,							              
    ADVANCED_PARA4_12_H,							              
    ADVANCED_PARA4_12_L,							              
    ADVANCED_PARA4_13_H,							              
    ADVANCED_PARA4_13_L,							              
    ADVANCED_PARA4_14_H,							              
    ADVANCED_PARA4_14_L,							              
    ADVANCED_PARA4_15_H,							             
    ADVANCED_PARA4_15_L,							  
    ADVANCED_PARA4_16_H,							             
    ADVANCED_PARA4_16_L,							   
    ADVANCED_PARA4_17_H,							             
    ADVANCED_PARA4_17_L,							   
    ADVANCED_PARA4_18_H,							             
    ADVANCED_PARA4_18_L,							   
    ADVANCED_PARA4_19_H,							             
    ADVANCED_PARA4_19_L,							   

    FREE_REVERSE_ROTATION_SPEED4,				            
    FREE_REVERSE_ROTATION_ANGLE4_H,				          
    FREE_REVERSE_ROTATION_ANGLE4_L,				          
    REVERS_ANGLE_SETTING_SPEED4,				            
    REVERS_ANGLE_SETTING_ANGLE4,				            
    REVERS_ANGLE_SETTING_FW_REV4,				            


    ADVANCED_MODE5 = 1180,			                  // 1180
    ADVANCED_PARA5_1_H,							                
    ADVANCED_PARA5_1_L,							                
    ADVANCED_PARA5_2_H,							                
    ADVANCED_PARA5_2_L,							                
    ADVANCED_PARA5_3_H,							                
    ADVANCED_PARA5_3_L,							                
    ADVANCED_PARA5_4_H,							                
    ADVANCED_PARA5_4_L,							                
    ADVANCED_PARA5_5_H,							                
    ADVANCED_PARA5_5_L,							                
    ADVANCED_PARA5_6_H,							                
    ADVANCED_PARA5_6_L,							                
    ADVANCED_PARA5_7_H,							                
    ADVANCED_PARA5_7_L,							                
    ADVANCED_PARA5_8_H,							                
    ADVANCED_PARA5_8_L,							                
    ADVANCED_PARA5_9_H,							                
    ADVANCED_PARA5_9_L,							                
    ADVANCED_PARA5_10_H,							              
    ADVANCED_PARA5_10_L,							              
    ADVANCED_PARA5_11_H,							              
    ADVANCED_PARA5_11_L,							              
    ADVANCED_PARA5_12_H,							              
    ADVANCED_PARA5_12_L,							              
    ADVANCED_PARA5_13_H,							              
    ADVANCED_PARA5_13_L,							              
    ADVANCED_PARA5_14_H,							              
    ADVANCED_PARA5_14_L,							              
    ADVANCED_PARA5_15_H,							             
    ADVANCED_PARA5_15_L,							  
    ADVANCED_PARA5_16_H,							             
    ADVANCED_PARA5_16_L,							   
    ADVANCED_PARA5_17_H,							             
    ADVANCED_PARA5_17_L,							   
    ADVANCED_PARA5_18_H,							             
    ADVANCED_PARA5_18_L,							   
    ADVANCED_PARA5_19_H,							             
    ADVANCED_PARA5_19_L,							   
    
    FREE_REVERSE_ROTATION_SPEED5,				            
    FREE_REVERSE_ROTATION_ANGLE5_H,				          
    FREE_REVERSE_ROTATION_ANGLE5_L,				          
    REVERS_ANGLE_SETTING_SPEED5,				            
    REVERS_ANGLE_SETTING_ANGLE5,				            
    REVERS_ANGLE_SETTING_FW_REV5,				            


    ADVANCED_MODE6 = 1225,			                  // 1225
    ADVANCED_PARA6_1_H,							                
    ADVANCED_PARA6_1_L,							                
    ADVANCED_PARA6_2_H,							                
    ADVANCED_PARA6_2_L,							                
    ADVANCED_PARA6_3_H,							                
    ADVANCED_PARA6_3_L,							                
    ADVANCED_PARA6_4_H,							                
    ADVANCED_PARA6_4_L,							                
    ADVANCED_PARA6_5_H,							                
    ADVANCED_PARA6_5_L,							                
    ADVANCED_PARA6_6_H,							                
    ADVANCED_PARA6_6_L,							                
    ADVANCED_PARA6_7_H,							                
    ADVANCED_PARA6_7_L,							                
    ADVANCED_PARA6_8_H,							                
    ADVANCED_PARA6_8_L,							                
    ADVANCED_PARA6_9_H,							                
    ADVANCED_PARA6_9_L,							                
    ADVANCED_PARA6_10_H,							              
    ADVANCED_PARA6_10_L,							              
    ADVANCED_PARA6_11_H,							              
    ADVANCED_PARA6_11_L,							              
    ADVANCED_PARA6_12_H,							              
    ADVANCED_PARA6_12_L,							              
    ADVANCED_PARA6_13_H,							              
    ADVANCED_PARA6_13_L,							              
    ADVANCED_PARA6_14_H,							              
    ADVANCED_PARA6_14_L,							              
    ADVANCED_PARA6_15_H,							             
    ADVANCED_PARA6_15_L,							  
    ADVANCED_PARA6_16_H,							             
    ADVANCED_PARA6_16_L,							   
    ADVANCED_PARA6_17_H,							             
    ADVANCED_PARA6_17_L,							   
    ADVANCED_PARA6_18_H,							             
    ADVANCED_PARA6_18_L,							   
    ADVANCED_PARA6_19_H,							             
    ADVANCED_PARA6_19_L,							   

    FREE_REVERSE_ROTATION_SPEED6,				            
    FREE_REVERSE_ROTATION_ANGLE6_H,				          
    FREE_REVERSE_ROTATION_ANGLE6_L,				          
    REVERS_ANGLE_SETTING_SPEED6,				            
    REVERS_ANGLE_SETTING_ANGLE6,				            
    REVERS_ANGLE_SETTING_FW_REV6,				            


    ADVANCED_MODE7 = 1270,			                  // 1270
    ADVANCED_PARA7_1_H,							                
    ADVANCED_PARA7_1_L,							                
    ADVANCED_PARA7_2_H,							                
    ADVANCED_PARA7_2_L,							                
    ADVANCED_PARA7_3_H,							                
    ADVANCED_PARA7_3_L,							                
    ADVANCED_PARA7_4_H,							                
    ADVANCED_PARA7_4_L,							                
    ADVANCED_PARA7_5_H,							                
    ADVANCED_PARA7_5_L,							                
    ADVANCED_PARA7_6_H,							                
    ADVANCED_PARA7_6_L,							                
    ADVANCED_PARA7_7_H,							                
    ADVANCED_PARA7_7_L,							                
    ADVANCED_PARA7_8_H,							                
    ADVANCED_PARA7_8_L,							                
    ADVANCED_PARA7_9_H,							                
    ADVANCED_PARA7_9_L,							                
    ADVANCED_PARA7_10_H,							              
    ADVANCED_PARA7_10_L,							              
    ADVANCED_PARA7_11_H,							              
    ADVANCED_PARA7_11_L,							              
    ADVANCED_PARA7_12_H,							              
    ADVANCED_PARA7_12_L,							              
    ADVANCED_PARA7_13_H,							              
    ADVANCED_PARA7_13_L,							              
    ADVANCED_PARA7_14_H,							              
    ADVANCED_PARA7_14_L,							              
    ADVANCED_PARA7_15_H,							             
    ADVANCED_PARA7_15_L,							  
    ADVANCED_PARA7_16_H,							             
    ADVANCED_PARA7_16_L,							   
    ADVANCED_PARA7_17_H,							             
    ADVANCED_PARA7_17_L,							   
    ADVANCED_PARA7_18_H,							             
    ADVANCED_PARA7_18_L,							   
    ADVANCED_PARA7_19_H,							             
    ADVANCED_PARA7_19_L,							   

    FREE_REVERSE_ROTATION_SPEED7,				            
    FREE_REVERSE_ROTATION_ANGLE7_H,				          
    FREE_REVERSE_ROTATION_ANGLE7_L,				          
    REVERS_ANGLE_SETTING_SPEED7,				            
    REVERS_ANGLE_SETTING_ANGLE7,				            
    REVERS_ANGLE_SETTING_FW_REV7,				            


    ADVANCED_MODE8 = 1315,			                  // 1315
    ADVANCED_PARA8_1_H,							                
    ADVANCED_PARA8_1_L,							                
    ADVANCED_PARA8_2_H,							                
    ADVANCED_PARA8_2_L,							                
    ADVANCED_PARA8_3_H,							                
    ADVANCED_PARA8_3_L,							                
    ADVANCED_PARA8_4_H,							                
    ADVANCED_PARA8_4_L,							                
    ADVANCED_PARA8_5_H,							                
    ADVANCED_PARA8_5_L,							                
    ADVANCED_PARA8_6_H,							                
    ADVANCED_PARA8_6_L,							                
    ADVANCED_PARA8_7_H,							                
    ADVANCED_PARA8_7_L,							                
    ADVANCED_PARA8_8_H,							                
    ADVANCED_PARA8_8_L,							                
    ADVANCED_PARA8_9_H,							                
    ADVANCED_PARA8_9_L,							                
    ADVANCED_PARA8_10_H,							              
    ADVANCED_PARA8_10_L,							              
    ADVANCED_PARA8_11_H,							              
    ADVANCED_PARA8_11_L,							              
    ADVANCED_PARA8_12_H,							              
    ADVANCED_PARA8_12_L,							              
    ADVANCED_PARA8_13_H,							              
    ADVANCED_PARA8_13_L,							              
    ADVANCED_PARA8_14_H,							              
    ADVANCED_PARA8_14_L,							              
    ADVANCED_PARA8_15_H,							             
    ADVANCED_PARA8_15_L,							  
    ADVANCED_PARA8_16_H,							             
    ADVANCED_PARA8_16_L,							   
    ADVANCED_PARA8_17_H,							             
    ADVANCED_PARA8_17_L,							   
    ADVANCED_PARA8_18_H,							             
    ADVANCED_PARA8_18_L,							   
    ADVANCED_PARA8_19_H,							             
    ADVANCED_PARA8_19_L,							   

    FREE_REVERSE_ROTATION_SPEED8,				            
    FREE_REVERSE_ROTATION_ANGLE8_H,				          
    FREE_REVERSE_ROTATION_ANGLE8_L,				          
    REVERS_ANGLE_SETTING_SPEED8,				            
    REVERS_ANGLE_SETTING_ANGLE8,				            
    REVERS_ANGLE_SETTING_FW_REV8,				            


    ADVANCED_MODE9 = 1360,			                  // 1360
    ADVANCED_PARA9_1_H,							                
    ADVANCED_PARA9_1_L,							                
    ADVANCED_PARA9_2_H,							                
    ADVANCED_PARA9_2_L,							                
    ADVANCED_PARA9_3_H,							                
    ADVANCED_PARA9_3_L,							                
    ADVANCED_PARA9_4_H,							                
    ADVANCED_PARA9_4_L,							                
    ADVANCED_PARA9_5_H,							                
    ADVANCED_PARA9_5_L,							                
    ADVANCED_PARA9_6_H,							                
    ADVANCED_PARA9_6_L,							                
    ADVANCED_PARA9_7_H,							                
    ADVANCED_PARA9_7_L,							                
    ADVANCED_PARA9_8_H,							                
    ADVANCED_PARA9_8_L,							                
    ADVANCED_PARA9_9_H,							                
    ADVANCED_PARA9_9_L,							                
    ADVANCED_PARA9_10_H,							              
    ADVANCED_PARA9_10_L,							              
    ADVANCED_PARA9_11_H,							              
    ADVANCED_PARA9_11_L,							              
    ADVANCED_PARA9_12_H,							              
    ADVANCED_PARA9_12_L,							              
    ADVANCED_PARA9_13_H,							              
    ADVANCED_PARA9_13_L,							              
    ADVANCED_PARA9_14_H,							              
    ADVANCED_PARA9_14_L,							              
    ADVANCED_PARA9_15_H,							             
    ADVANCED_PARA9_15_L,							  
    ADVANCED_PARA9_16_H,							             
    ADVANCED_PARA9_16_L,							   
    ADVANCED_PARA9_17_H,							             
    ADVANCED_PARA9_17_L,							   
    ADVANCED_PARA9_18_H,							             
    ADVANCED_PARA9_18_L,							   
    ADVANCED_PARA9_19_H,							             
    ADVANCED_PARA9_19_L,							   

    FREE_REVERSE_ROTATION_SPEED9,				            
    FREE_REVERSE_ROTATION_ANGLE9_H,				          
    FREE_REVERSE_ROTATION_ANGLE9_L,				          
    REVERS_ANGLE_SETTING_SPEED9,				            
    REVERS_ANGLE_SETTING_ANGLE9,				            
    REVERS_ANGLE_SETTING_FW_REV9,				            


    ADVANCED_MODE10 = 1405,			                  // 1405
    ADVANCED_PARA10_1_H,							                
    ADVANCED_PARA10_1_L,							                
    ADVANCED_PARA10_2_H,							                
    ADVANCED_PARA10_2_L,							                
    ADVANCED_PARA10_3_H,							                
    ADVANCED_PARA10_3_L,							                
    ADVANCED_PARA10_4_H,							                
    ADVANCED_PARA10_4_L,							                
    ADVANCED_PARA10_5_H,							                
    ADVANCED_PARA10_5_L,							                
    ADVANCED_PARA10_6_H,							                
    ADVANCED_PARA10_6_L,							                
    ADVANCED_PARA10_7_H,							                
    ADVANCED_PARA10_7_L,							                
    ADVANCED_PARA10_8_H,							                
    ADVANCED_PARA10_8_L,							                
    ADVANCED_PARA10_9_H,							                
    ADVANCED_PARA10_9_L,							                
    ADVANCED_PARA10_10_H,							              
    ADVANCED_PARA10_10_L,							              
    ADVANCED_PARA10_11_H,							              
    ADVANCED_PARA10_11_L,							              
    ADVANCED_PARA10_12_H,							              
    ADVANCED_PARA10_12_L,							              
    ADVANCED_PARA10_13_H,							              
    ADVANCED_PARA10_13_L,							              
    ADVANCED_PARA10_14_H,							              
    ADVANCED_PARA10_14_L,							             
    ADVANCED_PARA10_15_H,							             
    ADVANCED_PARA10_15_L,							  
    ADVANCED_PARA10_16_H,							             
    ADVANCED_PARA10_16_L,							   
    ADVANCED_PARA10_17_H,							             
    ADVANCED_PARA10_17_L,							   
    ADVANCED_PARA10_18_H,							             
    ADVANCED_PARA10_18_L,							   
    ADVANCED_PARA10_19_H,							             
    ADVANCED_PARA10_19_L,							   

    FREE_REVERSE_ROTATION_SPEED10,				            
    FREE_REVERSE_ROTATION_ANGLE10_H,				          
    FREE_REVERSE_ROTATION_ANGLE10_L,				          
    REVERS_ANGLE_SETTING_SPEED10,				            
    REVERS_ANGLE_SETTING_ANGLE10,				            
    REVERS_ANGLE_SETTING_FW_REV10,				            


    ADVANCED_MODE11 = 1450,			                  // 1450
    ADVANCED_PARA11_1_H,							                
    ADVANCED_PARA11_1_L,							                
    ADVANCED_PARA11_2_H,							                
    ADVANCED_PARA11_2_L,							                
    ADVANCED_PARA11_3_H,							                
    ADVANCED_PARA11_3_L,							                
    ADVANCED_PARA11_4_H,							                
    ADVANCED_PARA11_4_L,							                
    ADVANCED_PARA11_5_H,							                
    ADVANCED_PARA11_5_L,							                
    ADVANCED_PARA11_6_H,							                
    ADVANCED_PARA11_6_L,							                
    ADVANCED_PARA11_7_H,							                
    ADVANCED_PARA11_7_L,							                
    ADVANCED_PARA11_8_H,							                
    ADVANCED_PARA11_8_L,							                
    ADVANCED_PARA11_9_H,							                
    ADVANCED_PARA11_9_L,							                
    ADVANCED_PARA11_10_H,							              
    ADVANCED_PARA11_10_L,							              
    ADVANCED_PARA11_11_H,							              
    ADVANCED_PARA11_11_L,							              
    ADVANCED_PARA11_12_H,							              
    ADVANCED_PARA11_12_L,							              
    ADVANCED_PARA11_13_H,							              
    ADVANCED_PARA11_13_L,							              
    ADVANCED_PARA11_14_H,							              
    ADVANCED_PARA11_14_L,							              
    ADVANCED_PARA11_15_H,							             
    ADVANCED_PARA11_15_L,							  
    ADVANCED_PARA11_16_H,							             
    ADVANCED_PARA11_16_L,							   
    ADVANCED_PARA11_17_H,							             
    ADVANCED_PARA11_17_L,							   
    ADVANCED_PARA11_18_H,							             
    ADVANCED_PARA11_18_L,							   
    ADVANCED_PARA11_19_H,							             
    ADVANCED_PARA11_19_L,							   

    FREE_REVERSE_ROTATION_SPEED11,				            
    FREE_REVERSE_ROTATION_ANGLE11_H,				          
    FREE_REVERSE_ROTATION_ANGLE11_L,				          
    REVERS_ANGLE_SETTING_SPEED11,				            
    REVERS_ANGLE_SETTING_ANGLE11,				            
    REVERS_ANGLE_SETTING_FW_REV11,				            


    ADVANCED_MODE12 = 1495,			                  // 1495
    ADVANCED_PARA12_1_H,							                
    ADVANCED_PARA12_1_L,							                
    ADVANCED_PARA12_2_H,							                
    ADVANCED_PARA12_2_L,							                
    ADVANCED_PARA12_3_H,							                
    ADVANCED_PARA12_3_L,							                
    ADVANCED_PARA12_4_H,							                
    ADVANCED_PARA12_4_L,							                
    ADVANCED_PARA12_5_H,							                
    ADVANCED_PARA12_5_L,							                
    ADVANCED_PARA12_6_H,							                
    ADVANCED_PARA12_6_L,							                
    ADVANCED_PARA12_7_H,							                
    ADVANCED_PARA12_7_L,							                
    ADVANCED_PARA12_8_H,							                
    ADVANCED_PARA12_8_L,							                
    ADVANCED_PARA12_9_H,							                
    ADVANCED_PARA12_9_L,							                
    ADVANCED_PARA12_10_H,							              
    ADVANCED_PARA12_10_L,							              
    ADVANCED_PARA12_11_H,							              
    ADVANCED_PARA12_11_L,							              
    ADVANCED_PARA12_12_H,							              
    ADVANCED_PARA12_12_L,							              
    ADVANCED_PARA12_13_H,							              
    ADVANCED_PARA12_13_L,							              
    ADVANCED_PARA12_14_H,							              
    ADVANCED_PARA12_14_L,							              
    ADVANCED_PARA12_15_H,							             
    ADVANCED_PARA12_15_L,							  
    ADVANCED_PARA12_16_H,							             
    ADVANCED_PARA12_16_L,							   
    ADVANCED_PARA12_17_H,							             
    ADVANCED_PARA12_17_L,							   
    ADVANCED_PARA12_18_H,							             
    ADVANCED_PARA12_18_L,							   
    ADVANCED_PARA12_19_H,							             
    ADVANCED_PARA12_19_L,							   

    FREE_REVERSE_ROTATION_SPEED12,				            
    FREE_REVERSE_ROTATION_ANGLE12_H,				          
    FREE_REVERSE_ROTATION_ANGLE12_L,				          
    REVERS_ANGLE_SETTING_SPEED12,				            
    REVERS_ANGLE_SETTING_ANGLE12,				            
    REVERS_ANGLE_SETTING_FW_REV12,				            


    ADVANCED_MODE13 = 1540,			                  // 1540
    ADVANCED_PARA13_1_H,							                
    ADVANCED_PARA13_1_L,							                
    ADVANCED_PARA13_2_H,							                
    ADVANCED_PARA13_2_L,							                
    ADVANCED_PARA13_3_H,							                
    ADVANCED_PARA13_3_L,							                
    ADVANCED_PARA13_4_H,							                
    ADVANCED_PARA13_4_L,							                
    ADVANCED_PARA13_5_H,							                
    ADVANCED_PARA13_5_L,							                
    ADVANCED_PARA13_6_H,							                
    ADVANCED_PARA13_6_L,							                
    ADVANCED_PARA13_7_H,							                
    ADVANCED_PARA13_7_L,							                
    ADVANCED_PARA13_8_H,							                
    ADVANCED_PARA13_8_L,							                
    ADVANCED_PARA13_9_H,							                
    ADVANCED_PARA13_9_L,							                
    ADVANCED_PARA13_10_H,							              
    ADVANCED_PARA13_10_L,							              
    ADVANCED_PARA13_11_H,							              
    ADVANCED_PARA13_11_L,							              
    ADVANCED_PARA13_12_H,							              
    ADVANCED_PARA13_12_L,							              
    ADVANCED_PARA13_13_H,							              
    ADVANCED_PARA13_13_L,							              
    ADVANCED_PARA13_14_H,							              
    ADVANCED_PARA13_14_L,							              
    ADVANCED_PARA13_15_H,							             
    ADVANCED_PARA13_15_L,							  
    ADVANCED_PARA13_16_H,							             
    ADVANCED_PARA13_16_L,							   
    ADVANCED_PARA13_17_H,							             
    ADVANCED_PARA13_17_L,							   
    ADVANCED_PARA13_18_H,							             
    ADVANCED_PARA13_18_L,							   
    ADVANCED_PARA13_19_H,							             
    ADVANCED_PARA13_19_L,							   

    FREE_REVERSE_ROTATION_SPEED13,				            
    FREE_REVERSE_ROTATION_ANGLE13_H,				          
    FREE_REVERSE_ROTATION_ANGLE13_L,				          
    REVERS_ANGLE_SETTING_SPEED13,				            
    REVERS_ANGLE_SETTING_ANGLE13,				            
    REVERS_ANGLE_SETTING_FW_REV13,				            


    ADVANCED_MODE14 = 1585,			                  // 1585
    ADVANCED_PARA14_1_H,							                
    ADVANCED_PARA14_1_L,							                
    ADVANCED_PARA14_2_H,							                
    ADVANCED_PARA14_2_L,							                
    ADVANCED_PARA14_3_H,							                
    ADVANCED_PARA14_3_L,							                
    ADVANCED_PARA14_4_H,							                
    ADVANCED_PARA14_4_L,							                
    ADVANCED_PARA14_5_H,							                
    ADVANCED_PARA14_5_L,							                
    ADVANCED_PARA14_6_H,							                
    ADVANCED_PARA14_6_L,							                
    ADVANCED_PARA14_7_H,							                
    ADVANCED_PARA14_7_L,							                
    ADVANCED_PARA14_8_H,							                
    ADVANCED_PARA14_8_L,							                
    ADVANCED_PARA14_9_H,							                
    ADVANCED_PARA14_9_L,							                
    ADVANCED_PARA14_10_H,							              
    ADVANCED_PARA14_10_L,							              
    ADVANCED_PARA14_11_H,							              
    ADVANCED_PARA14_11_L,							              
    ADVANCED_PARA14_12_H,							              
    ADVANCED_PARA14_12_L,							              
    ADVANCED_PARA14_13_H,							              
    ADVANCED_PARA14_13_L,							              
    ADVANCED_PARA14_14_H,							              
    ADVANCED_PARA14_14_L,							              
    ADVANCED_PARA14_15_H,							             
    ADVANCED_PARA14_15_L,							  
    ADVANCED_PARA14_16_H,							             
    ADVANCED_PARA14_16_L,							   
    ADVANCED_PARA14_17_H,							             
    ADVANCED_PARA14_17_L,							   
    ADVANCED_PARA14_18_H,							             
    ADVANCED_PARA14_18_L,							   
    ADVANCED_PARA14_19_H,							             
    ADVANCED_PARA14_19_L,							   

    FREE_REVERSE_ROTATION_SPEED14,				            
    FREE_REVERSE_ROTATION_ANGLE14_H,				          
    FREE_REVERSE_ROTATION_ANGLE14_L,				          
    REVERS_ANGLE_SETTING_SPEED14,				            
    REVERS_ANGLE_SETTING_ANGLE14,				            
    REVERS_ANGLE_SETTING_FW_REV14,				            


    ADVANCED_MODE15 = 1630,			                  // 1630
    ADVANCED_PARA15_1_H,							                
    ADVANCED_PARA15_1_L,							                
    ADVANCED_PARA15_2_H,							                
    ADVANCED_PARA15_2_L,							                
    ADVANCED_PARA15_3_H,							                
    ADVANCED_PARA15_3_L,							                
    ADVANCED_PARA15_4_H,							                
    ADVANCED_PARA15_4_L,							                
    ADVANCED_PARA15_5_H,							                
    ADVANCED_PARA15_5_L,							                
    ADVANCED_PARA15_6_H,							                
    ADVANCED_PARA15_6_L,							                
    ADVANCED_PARA15_7_H,							                
    ADVANCED_PARA15_7_L,							                
    ADVANCED_PARA15_8_H,							                
    ADVANCED_PARA15_8_L,							                
    ADVANCED_PARA15_9_H,							                
    ADVANCED_PARA15_9_L,							                
    ADVANCED_PARA15_10_H,							              
    ADVANCED_PARA15_10_L,							              
    ADVANCED_PARA15_11_H,							              
    ADVANCED_PARA15_11_L,							              
    ADVANCED_PARA15_12_H,							              
    ADVANCED_PARA15_12_L,							              
    ADVANCED_PARA15_13_H,							              
    ADVANCED_PARA15_13_L,							              
    ADVANCED_PARA15_14_H,							              
    ADVANCED_PARA15_14_L,							              
    ADVANCED_PARA15_15_H,							             
    ADVANCED_PARA15_15_L,							  
    ADVANCED_PARA15_16_H,							             
    ADVANCED_PARA15_16_L,							   
    ADVANCED_PARA15_17_H,							             
    ADVANCED_PARA15_17_L,							   
    ADVANCED_PARA15_18_H,							             
    ADVANCED_PARA15_18_L,							   
    ADVANCED_PARA15_19_H,							             
    ADVANCED_PARA15_19_L,							   

    FREE_REVERSE_ROTATION_SPEED15,				            
    FREE_REVERSE_ROTATION_ANGLE15_H,				          
    FREE_REVERSE_ROTATION_ANGLE15_L,				          
    REVERS_ANGLE_SETTING_SPEED15,				            
    REVERS_ANGLE_SETTING_ANGLE15,				            
    REVERS_ANGLE_SETTING_FW_REV15,		
    

    ADVANCED_MODE16 = 1675,			                  // 1675
    ADVANCED_PARA16_1_H,							                
    ADVANCED_PARA16_1_L,							                
    ADVANCED_PARA16_2_H,							                
    ADVANCED_PARA16_2_L,							                
    ADVANCED_PARA16_3_H,							                
    ADVANCED_PARA16_3_L,							                
    ADVANCED_PARA16_4_H,							                
    ADVANCED_PARA16_4_L,							                
    ADVANCED_PARA16_5_H,							                
    ADVANCED_PARA16_5_L,							                
    ADVANCED_PARA16_6_H,							                
    ADVANCED_PARA16_6_L,							                
    ADVANCED_PARA16_7_H,							                
    ADVANCED_PARA16_7_L,							                
    ADVANCED_PARA16_8_H,							                
    ADVANCED_PARA16_8_L,							                
    ADVANCED_PARA16_9_H,							                
    ADVANCED_PARA16_9_L,							                
    ADVANCED_PARA16_10_H,							              
    ADVANCED_PARA16_10_L,							              
    ADVANCED_PARA16_11_H,							              
    ADVANCED_PARA16_11_L,							              
    ADVANCED_PARA16_12_H,							              
    ADVANCED_PARA16_12_L,							              
    ADVANCED_PARA16_13_H,							              
    ADVANCED_PARA16_13_L,							              
    ADVANCED_PARA16_14_H,							              
    ADVANCED_PARA16_14_L,							              
    ADVANCED_PARA16_15_H,							             
    ADVANCED_PARA16_15_L,							  
    ADVANCED_PARA16_16_H,							             
    ADVANCED_PARA16_16_L,							   
    ADVANCED_PARA16_17_H,							             
    ADVANCED_PARA16_17_L,							   
    ADVANCED_PARA16_18_H,							             
    ADVANCED_PARA16_18_L,							   
    ADVANCED_PARA16_19_H,							             
    ADVANCED_PARA16_19_L,							   

    FREE_REVERSE_ROTATION_SPEED16,				            
    FREE_REVERSE_ROTATION_ANGLE16_H,				          
    FREE_REVERSE_ROTATION_ANGLE16_L,				          
    REVERS_ANGLE_SETTING_SPEED16,				            
    REVERS_ANGLE_SETTING_ANGLE16,				            
    REVERS_ANGLE_SETTING_FW_REV16,		
    

    ADVANCED_MODE17 = 1720,			                  // 1720
    ADVANCED_PARA17_1_H,							                
    ADVANCED_PARA17_1_L,							                
    ADVANCED_PARA17_2_H,							                
    ADVANCED_PARA17_2_L,							                
    ADVANCED_PARA17_3_H,							                
    ADVANCED_PARA17_3_L,							                
    ADVANCED_PARA17_4_H,							                
    ADVANCED_PARA17_4_L,							                
    ADVANCED_PARA17_5_H,							                
    ADVANCED_PARA17_5_L,							                
    ADVANCED_PARA17_6_H,							                
    ADVANCED_PARA17_6_L,							                
    ADVANCED_PARA17_7_H,							                
    ADVANCED_PARA17_7_L,							                
    ADVANCED_PARA17_8_H,							                
    ADVANCED_PARA17_8_L,							                
    ADVANCED_PARA17_9_H,							                
    ADVANCED_PARA17_9_L,							                
    ADVANCED_PARA17_10_H,							              
    ADVANCED_PARA17_10_L,							              
    ADVANCED_PARA17_11_H,							              
    ADVANCED_PARA17_11_L,							              
    ADVANCED_PARA17_12_H,							              
    ADVANCED_PARA17_12_L,							              
    ADVANCED_PARA17_13_H,							              
    ADVANCED_PARA17_13_L,							              
    ADVANCED_PARA17_14_H,							              
    ADVANCED_PARA17_14_L,							              
    ADVANCED_PARA17_15_H,							             
    ADVANCED_PARA17_15_L,							  
    ADVANCED_PARA17_16_H,							             
    ADVANCED_PARA17_16_L,							   
    ADVANCED_PARA17_17_H,							             
    ADVANCED_PARA17_17_L,							   
    ADVANCED_PARA17_18_H,							             
    ADVANCED_PARA17_18_L,							   
    ADVANCED_PARA17_19_H,							             
    ADVANCED_PARA17_19_L,							   

    FREE_REVERSE_ROTATION_SPEED17,				            
    FREE_REVERSE_ROTATION_ANGLE17_H,				          
    FREE_REVERSE_ROTATION_ANGLE17_L,				          
    REVERS_ANGLE_SETTING_SPEED17,				            
    REVERS_ANGLE_SETTING_ANGLE17,				            
    REVERS_ANGLE_SETTING_FW_REV17,		
    

    ADVANCED_MODE18 = 1765,			                  // 1765
    ADVANCED_PARA18_1_H,							                
    ADVANCED_PARA18_1_L,							                
    ADVANCED_PARA18_2_H,							                
    ADVANCED_PARA18_2_L,							                
    ADVANCED_PARA18_3_H,							                
    ADVANCED_PARA18_3_L,							                
    ADVANCED_PARA18_4_H,							                
    ADVANCED_PARA18_4_L,							                
    ADVANCED_PARA18_5_H,							                
    ADVANCED_PARA18_5_L,							                
    ADVANCED_PARA18_6_H,							                
    ADVANCED_PARA18_6_L,							                
    ADVANCED_PARA18_7_H,							                
    ADVANCED_PARA18_7_L,							                
    ADVANCED_PARA18_8_H,							                
    ADVANCED_PARA18_8_L,							                
    ADVANCED_PARA18_9_H,							                
    ADVANCED_PARA18_9_L,							                
    ADVANCED_PARA18_10_H,							              
    ADVANCED_PARA18_10_L,							              
    ADVANCED_PARA18_11_H,							              
    ADVANCED_PARA18_11_L,							              
    ADVANCED_PARA18_12_H,							              
    ADVANCED_PARA18_12_L,							              
    ADVANCED_PARA18_13_H,							              
    ADVANCED_PARA18_13_L,							              
    ADVANCED_PARA18_14_H,							              
    ADVANCED_PARA18_14_L,							              
    ADVANCED_PARA18_15_H,							             
    ADVANCED_PARA18_15_L,							  
    ADVANCED_PARA18_16_H,							             
    ADVANCED_PARA18_16_L,							   
    ADVANCED_PARA18_17_H,							             
    ADVANCED_PARA18_17_L,							   
    ADVANCED_PARA18_18_H,							             
    ADVANCED_PARA18_18_L,							   
    ADVANCED_PARA18_19_H,							             
    ADVANCED_PARA18_19_L,							   

    FREE_REVERSE_ROTATION_SPEED18,				            
    FREE_REVERSE_ROTATION_ANGLE18_H,				          
    FREE_REVERSE_ROTATION_ANGLE18_L,				          
    REVERS_ANGLE_SETTING_SPEED18,				            
    REVERS_ANGLE_SETTING_ANGLE18,				            
    REVERS_ANGLE_SETTING_FW_REV18,		

    

    ADVANCED_MODE19 = 1810,			                  // 1810
    ADVANCED_PARA19_1_H,							                
    ADVANCED_PARA19_1_L,							                
    ADVANCED_PARA19_2_H,							                
    ADVANCED_PARA19_2_L,							                
    ADVANCED_PARA19_3_H,							                
    ADVANCED_PARA19_3_L,							                
    ADVANCED_PARA19_4_H,							                
    ADVANCED_PARA19_4_L,							                
    ADVANCED_PARA19_5_H,							                
    ADVANCED_PARA19_5_L,							                
    ADVANCED_PARA19_6_H,							                
    ADVANCED_PARA19_6_L,							                
    ADVANCED_PARA19_7_H,							                
    ADVANCED_PARA19_7_L,							                
    ADVANCED_PARA19_8_H,							                
    ADVANCED_PARA19_8_L,							                
    ADVANCED_PARA19_9_H,							                
    ADVANCED_PARA19_9_L,							                
    ADVANCED_PARA19_10_H,							              
    ADVANCED_PARA19_10_L,							              
    ADVANCED_PARA19_11_H,							              
    ADVANCED_PARA19_11_L,							              
    ADVANCED_PARA19_12_H,							              
    ADVANCED_PARA19_12_L,							              
    ADVANCED_PARA19_13_H,							              
    ADVANCED_PARA19_13_L,							              
    ADVANCED_PARA19_14_H,							              
    ADVANCED_PARA19_14_L,							              
    ADVANCED_PARA19_15_H,							             
    ADVANCED_PARA19_15_L,							  
    ADVANCED_PARA19_16_H,							             
    ADVANCED_PARA19_16_L,							   
    ADVANCED_PARA19_17_H,							             
    ADVANCED_PARA19_17_L,							   
    ADVANCED_PARA19_18_H,							             
    ADVANCED_PARA19_18_L,							   
    ADVANCED_PARA19_19_H,							             
    ADVANCED_PARA19_19_L,							   

    FREE_REVERSE_ROTATION_SPEED19,				            
    FREE_REVERSE_ROTATION_ANGLE19_H,				          
    FREE_REVERSE_ROTATION_ANGLE19_L,				          
    REVERS_ANGLE_SETTING_SPEED19,				            
    REVERS_ANGLE_SETTING_ANGLE19,				            
    REVERS_ANGLE_SETTING_FW_REV19,		

    

    ADVANCED_MODE20 = 1855,			                  // 1855
    ADVANCED_PARA20_1_H,							                
    ADVANCED_PARA20_1_L,							                
    ADVANCED_PARA20_2_H,							                
    ADVANCED_PARA20_2_L,							                
    ADVANCED_PARA20_3_H,							                
    ADVANCED_PARA20_3_L,							                
    ADVANCED_PARA20_4_H,							                
    ADVANCED_PARA20_4_L,							                
    ADVANCED_PARA20_5_H,							                
    ADVANCED_PARA20_5_L,							                
    ADVANCED_PARA20_6_H,							                
    ADVANCED_PARA20_6_L,							                
    ADVANCED_PARA20_7_H,							                
    ADVANCED_PARA20_7_L,							                
    ADVANCED_PARA20_8_H,							                
    ADVANCED_PARA20_8_L,							                
    ADVANCED_PARA20_9_H,							                
    ADVANCED_PARA20_9_L,							                
    ADVANCED_PARA20_10_H,							              
    ADVANCED_PARA20_10_L,							              
    ADVANCED_PARA20_11_H,							              
    ADVANCED_PARA20_11_L,							              
    ADVANCED_PARA20_12_H,							              
    ADVANCED_PARA20_12_L,							              
    ADVANCED_PARA20_13_H,							              
    ADVANCED_PARA20_13_L,							              
    ADVANCED_PARA20_14_H,							              
    ADVANCED_PARA20_14_L,							              
    ADVANCED_PARA20_15_H,							             
    ADVANCED_PARA20_15_L,							  
    ADVANCED_PARA20_16_H,							             
    ADVANCED_PARA20_16_L,							   
    ADVANCED_PARA20_17_H,							             
    ADVANCED_PARA20_17_L,							   
    ADVANCED_PARA20_18_H,							             
    ADVANCED_PARA20_18_L,							   
    ADVANCED_PARA20_19_H,							             
    ADVANCED_PARA20_19_L,							   

    FREE_REVERSE_ROTATION_SPEED20,				            
    FREE_REVERSE_ROTATION_ANGLE20_H,				          
    FREE_REVERSE_ROTATION_ANGLE20_L,				          
    REVERS_ANGLE_SETTING_SPEED20,				            
    REVERS_ANGLE_SETTING_ANGLE20,				            
    REVERS_ANGLE_SETTING_FW_REV20,		

    

    ADVANCED_MODE21 = 1900,			                  // 1900
    ADVANCED_PARA21_1_H,							                
    ADVANCED_PARA21_1_L,							                
    ADVANCED_PARA21_2_H,							                
    ADVANCED_PARA21_2_L,							                
    ADVANCED_PARA21_3_H,							                
    ADVANCED_PARA21_3_L,							                
    ADVANCED_PARA21_4_H,							                
    ADVANCED_PARA21_4_L,							                
    ADVANCED_PARA21_5_H,							                
    ADVANCED_PARA21_5_L,							                
    ADVANCED_PARA21_6_H,							                
    ADVANCED_PARA21_6_L,							                
    ADVANCED_PARA21_7_H,							                
    ADVANCED_PARA21_7_L,							                
    ADVANCED_PARA21_8_H,							                
    ADVANCED_PARA21_8_L,							                
    ADVANCED_PARA21_9_H,							                
    ADVANCED_PARA21_9_L,							                
    ADVANCED_PARA21_10_H,							              
    ADVANCED_PARA21_10_L,							              
    ADVANCED_PARA21_11_H,							              
    ADVANCED_PARA21_11_L,							              
    ADVANCED_PARA21_12_H,							              
    ADVANCED_PARA21_12_L,							              
    ADVANCED_PARA21_13_H,							              
    ADVANCED_PARA21_13_L,							              
    ADVANCED_PARA21_14_H,							              
    ADVANCED_PARA21_14_L,							              
    ADVANCED_PARA21_15_H,							             
    ADVANCED_PARA21_15_L,							  
    ADVANCED_PARA21_16_H,							             
    ADVANCED_PARA21_16_L,							   
    ADVANCED_PARA21_17_H,							             
    ADVANCED_PARA21_17_L,							   
    ADVANCED_PARA21_18_H,							             
    ADVANCED_PARA21_18_L,							   
    ADVANCED_PARA21_19_H,							             
    ADVANCED_PARA21_19_L,							   

    FREE_REVERSE_ROTATION_SPEED21,				            
    FREE_REVERSE_ROTATION_ANGLE21_H,				          
    FREE_REVERSE_ROTATION_ANGLE21_L,				          
    REVERS_ANGLE_SETTING_SPEED21,				            
    REVERS_ANGLE_SETTING_ANGLE21,				            
    REVERS_ANGLE_SETTING_FW_REV21,		

    

    ADVANCED_MODE22 = 1945,			                  // 1945
    ADVANCED_PARA22_1_H,							                
    ADVANCED_PARA22_1_L,							                
    ADVANCED_PARA22_2_H,							                
    ADVANCED_PARA22_2_L,							                
    ADVANCED_PARA22_3_H,							                
    ADVANCED_PARA22_3_L,							                
    ADVANCED_PARA22_4_H,							                
    ADVANCED_PARA22_4_L,							                
    ADVANCED_PARA22_5_H,							                
    ADVANCED_PARA22_5_L,							                
    ADVANCED_PARA22_6_H,							                
    ADVANCED_PARA22_6_L,							                
    ADVANCED_PARA22_7_H,							                
    ADVANCED_PARA22_7_L,							                
    ADVANCED_PARA22_8_H,							                
    ADVANCED_PARA22_8_L,							                
    ADVANCED_PARA22_9_H,							                
    ADVANCED_PARA22_9_L,							                
    ADVANCED_PARA22_10_H,							              
    ADVANCED_PARA22_10_L,							              
    ADVANCED_PARA22_11_H,							              
    ADVANCED_PARA22_11_L,							              
    ADVANCED_PARA22_12_H,							              
    ADVANCED_PARA22_12_L,							              
    ADVANCED_PARA22_13_H,							              
    ADVANCED_PARA22_13_L,							              
    ADVANCED_PARA22_14_H,							              
    ADVANCED_PARA22_14_L,							              
    ADVANCED_PARA22_15_H,							             
    ADVANCED_PARA22_15_L,							  
    ADVANCED_PARA22_16_H,							             
    ADVANCED_PARA22_16_L,							   
    ADVANCED_PARA22_17_H,							             
    ADVANCED_PARA22_17_L,							   
    ADVANCED_PARA22_18_H,							             
    ADVANCED_PARA22_18_L,							   
    ADVANCED_PARA22_19_H,							             
    ADVANCED_PARA22_19_L,							   

    FREE_REVERSE_ROTATION_SPEED22,				            
    FREE_REVERSE_ROTATION_ANGLE22_H,				          
    FREE_REVERSE_ROTATION_ANGLE22_L,				          
    REVERS_ANGLE_SETTING_SPEED22,				            
    REVERS_ANGLE_SETTING_ANGLE22,				            
    REVERS_ANGLE_SETTING_FW_REV22,		

    

    ADVANCED_MODE23 = 1990,			                  // 1990
    ADVANCED_PARA23_1_H,							                
    ADVANCED_PARA23_1_L,							                
    ADVANCED_PARA23_2_H,							                
    ADVANCED_PARA23_2_L,							                
    ADVANCED_PARA23_3_H,							                
    ADVANCED_PARA23_3_L,							                
    ADVANCED_PARA23_4_H,							                
    ADVANCED_PARA23_4_L,							                
    ADVANCED_PARA23_5_H,							                
    ADVANCED_PARA23_5_L,							                
    ADVANCED_PARA23_6_H,							                
    ADVANCED_PARA23_6_L,							                
    ADVANCED_PARA23_7_H,							                
    ADVANCED_PARA23_7_L,							                
    ADVANCED_PARA23_8_H,							                
    ADVANCED_PARA23_8_L,							                
    ADVANCED_PARA23_9_H,							                
    ADVANCED_PARA23_9_L,							                
    ADVANCED_PARA23_10_H,							              
    ADVANCED_PARA23_10_L,							              
    ADVANCED_PARA23_11_H,							              
    ADVANCED_PARA23_11_L,							              
    ADVANCED_PARA23_12_H,							              
    ADVANCED_PARA23_12_L,							              
    ADVANCED_PARA23_13_H,							              
    ADVANCED_PARA23_13_L,							              
    ADVANCED_PARA23_14_H,							              
    ADVANCED_PARA23_14_L,							              
    ADVANCED_PARA23_15_H,							             
    ADVANCED_PARA23_15_L,							  
    ADVANCED_PARA23_16_H,							             
    ADVANCED_PARA23_16_L,							   
    ADVANCED_PARA23_17_H,							             
    ADVANCED_PARA23_17_L,							   
    ADVANCED_PARA23_18_H,							             
    ADVANCED_PARA23_18_L,							   
    ADVANCED_PARA23_19_H,							             
    ADVANCED_PARA23_19_L,							   

    FREE_REVERSE_ROTATION_SPEED23,				            
    FREE_REVERSE_ROTATION_ANGLE23_H,				          
    FREE_REVERSE_ROTATION_ANGLE23_L,				          
    REVERS_ANGLE_SETTING_SPEED23,				            
    REVERS_ANGLE_SETTING_ANGLE23,				            
    REVERS_ANGLE_SETTING_FW_REV23,		

    

    ADVANCED_MODE24 = 2035,			                  // 2035
    ADVANCED_PARA24_1_H,							                
    ADVANCED_PARA24_1_L,							                
    ADVANCED_PARA24_2_H,							                
    ADVANCED_PARA24_2_L,							                
    ADVANCED_PARA24_3_H,							                
    ADVANCED_PARA24_3_L,							                
    ADVANCED_PARA24_4_H,							                
    ADVANCED_PARA24_4_L,							                
    ADVANCED_PARA24_5_H,							                
    ADVANCED_PARA24_5_L,							                
    ADVANCED_PARA24_6_H,							                
    ADVANCED_PARA24_6_L,							                
    ADVANCED_PARA24_7_H,							                
    ADVANCED_PARA24_7_L,							                
    ADVANCED_PARA24_8_H,							                
    ADVANCED_PARA24_8_L,							                
    ADVANCED_PARA24_9_H,							                
    ADVANCED_PARA24_9_L,							                
    ADVANCED_PARA24_10_H,							              
    ADVANCED_PARA24_10_L,							              
    ADVANCED_PARA24_11_H,							              
    ADVANCED_PARA24_11_L,							              
    ADVANCED_PARA24_12_H,							              
    ADVANCED_PARA24_12_L,							              
    ADVANCED_PARA24_13_H,							              
    ADVANCED_PARA24_13_L,							              
    ADVANCED_PARA24_14_H,							              
    ADVANCED_PARA24_14_L,							              
    ADVANCED_PARA24_15_H,							             
    ADVANCED_PARA24_15_L,							  
    ADVANCED_PARA24_16_H,							             
    ADVANCED_PARA24_16_L,							   
    ADVANCED_PARA24_17_H,							             
    ADVANCED_PARA24_17_L,							   
    ADVANCED_PARA24_18_H,							             
    ADVANCED_PARA24_18_L,							   
    ADVANCED_PARA24_19_H,							             
    ADVANCED_PARA24_19_L,							   

    FREE_REVERSE_ROTATION_SPEED24,				            
    FREE_REVERSE_ROTATION_ANGLE24_H,				          
    FREE_REVERSE_ROTATION_ANGLE24_L,				          
    REVERS_ANGLE_SETTING_SPEED24,				            
    REVERS_ANGLE_SETTING_ANGLE24,				            
    REVERS_ANGLE_SETTING_FW_REV24,		

    

    ADVANCED_MODE25 = 2080,			                  // 2080
    ADVANCED_PARA25_1_H,							                
    ADVANCED_PARA25_1_L,							                
    ADVANCED_PARA25_2_H,							                
    ADVANCED_PARA25_2_L,							                
    ADVANCED_PARA25_3_H,							                
    ADVANCED_PARA25_3_L,							                
    ADVANCED_PARA25_4_H,							                
    ADVANCED_PARA25_4_L,							                
    ADVANCED_PARA25_5_H,							                
    ADVANCED_PARA25_5_L,							                
    ADVANCED_PARA25_6_H,							                
    ADVANCED_PARA25_6_L,							                
    ADVANCED_PARA25_7_H,							                
    ADVANCED_PARA25_7_L,							                
    ADVANCED_PARA25_8_H,							                
    ADVANCED_PARA25_8_L,							                
    ADVANCED_PARA25_9_H,							                
    ADVANCED_PARA25_9_L,							                
    ADVANCED_PARA25_10_H,							              
    ADVANCED_PARA25_10_L,							              
    ADVANCED_PARA25_11_H,							              
    ADVANCED_PARA25_11_L,							              
    ADVANCED_PARA25_12_H,							              
    ADVANCED_PARA25_12_L,							              
    ADVANCED_PARA25_13_H,							              
    ADVANCED_PARA25_13_L,							              
    ADVANCED_PARA25_14_H,							              
    ADVANCED_PARA25_14_L,							              
    ADVANCED_PARA25_15_H,							             
    ADVANCED_PARA25_15_L,							  
    ADVANCED_PARA25_16_H,							             
    ADVANCED_PARA25_16_L,							   
    ADVANCED_PARA25_17_H,							             
    ADVANCED_PARA25_17_L,							   
    ADVANCED_PARA25_18_H,							             
    ADVANCED_PARA25_18_L,							   
    ADVANCED_PARA25_19_H,							             
    ADVANCED_PARA25_19_L,							   

    FREE_REVERSE_ROTATION_SPEED25,				            
    FREE_REVERSE_ROTATION_ANGLE25_H,				          
    FREE_REVERSE_ROTATION_ANGLE25_L,				          
    REVERS_ANGLE_SETTING_SPEED25,				            
    REVERS_ANGLE_SETTING_ANGLE25,				            
    REVERS_ANGLE_SETTING_FW_REV25,		

    

    ADVANCED_MODE26 = 2125,			                  // 2125
    ADVANCED_PARA26_1_H,							                
    ADVANCED_PARA26_1_L,							                
    ADVANCED_PARA26_2_H,							                
    ADVANCED_PARA26_2_L,							                
    ADVANCED_PARA26_3_H,							                
    ADVANCED_PARA26_3_L,							                
    ADVANCED_PARA26_4_H,							                
    ADVANCED_PARA26_4_L,							                
    ADVANCED_PARA26_5_H,							                
    ADVANCED_PARA26_5_L,							                
    ADVANCED_PARA26_6_H,							                
    ADVANCED_PARA26_6_L,							                
    ADVANCED_PARA26_7_H,							                
    ADVANCED_PARA26_7_L,							                
    ADVANCED_PARA26_8_H,							                
    ADVANCED_PARA26_8_L,							                
    ADVANCED_PARA26_9_H,							                
    ADVANCED_PARA26_9_L,							                
    ADVANCED_PARA26_10_H,							              
    ADVANCED_PARA26_10_L,							              
    ADVANCED_PARA26_11_H,							              
    ADVANCED_PARA26_11_L,							              
    ADVANCED_PARA26_12_H,							              
    ADVANCED_PARA26_12_L,							              
    ADVANCED_PARA26_13_H,							              
    ADVANCED_PARA26_13_L,							              
    ADVANCED_PARA26_14_H,							              
    ADVANCED_PARA26_14_L,							              
    ADVANCED_PARA26_15_H,							             
    ADVANCED_PARA26_15_L,							  
    ADVANCED_PARA26_16_H,							             
    ADVANCED_PARA26_16_L,							   
    ADVANCED_PARA26_17_H,							             
    ADVANCED_PARA26_17_L,							   
    ADVANCED_PARA26_18_H,							             
    ADVANCED_PARA26_18_L,							   
    ADVANCED_PARA26_19_H,							             
    ADVANCED_PARA26_19_L,							   

    FREE_REVERSE_ROTATION_SPEED26,				            
    FREE_REVERSE_ROTATION_ANGLE26_H,				          
    FREE_REVERSE_ROTATION_ANGLE26_L,				          
    REVERS_ANGLE_SETTING_SPEED26,				            
    REVERS_ANGLE_SETTING_ANGLE26,				            
    REVERS_ANGLE_SETTING_FW_REV26,		

    

    ADVANCED_MODE27 = 2170,			                  // 2170
    ADVANCED_PARA27_1_H,							                
    ADVANCED_PARA27_1_L,							                
    ADVANCED_PARA27_2_H,							                
    ADVANCED_PARA27_2_L,							                
    ADVANCED_PARA27_3_H,							                
    ADVANCED_PARA27_3_L,							                
    ADVANCED_PARA27_4_H,							                
    ADVANCED_PARA27_4_L,							                
    ADVANCED_PARA27_5_H,							                
    ADVANCED_PARA27_5_L,							                
    ADVANCED_PARA27_6_H,							                
    ADVANCED_PARA27_6_L,							                
    ADVANCED_PARA27_7_H,							                
    ADVANCED_PARA27_7_L,							                
    ADVANCED_PARA27_8_H,							                
    ADVANCED_PARA27_8_L,							                
    ADVANCED_PARA27_9_H,							                
    ADVANCED_PARA27_9_L,							                
    ADVANCED_PARA27_10_H,							              
    ADVANCED_PARA27_10_L,							              
    ADVANCED_PARA27_11_H,							              
    ADVANCED_PARA27_11_L,							              
    ADVANCED_PARA27_12_H,							              
    ADVANCED_PARA27_12_L,							              
    ADVANCED_PARA27_13_H,							              
    ADVANCED_PARA27_13_L,							              
    ADVANCED_PARA27_14_H,							              
    ADVANCED_PARA27_14_L,							              
    ADVANCED_PARA27_15_H,							             
    ADVANCED_PARA27_15_L,							  
    ADVANCED_PARA27_16_H,							             
    ADVANCED_PARA27_16_L,							   
    ADVANCED_PARA27_17_H,							             
    ADVANCED_PARA27_17_L,							   
    ADVANCED_PARA27_18_H,							             
    ADVANCED_PARA27_18_L,							   
    ADVANCED_PARA27_19_H,							             
    ADVANCED_PARA27_19_L,							   

    FREE_REVERSE_ROTATION_SPEED27,				            
    FREE_REVERSE_ROTATION_ANGLE27_H,				          
    FREE_REVERSE_ROTATION_ANGLE27_L,				          
    REVERS_ANGLE_SETTING_SPEED27,				            
    REVERS_ANGLE_SETTING_ANGLE27,				            
    REVERS_ANGLE_SETTING_FW_REV27,		

    

    ADVANCED_MODE28 = 2215,			                  // 2215
    ADVANCED_PARA28_1_H,							                
    ADVANCED_PARA28_1_L,							                
    ADVANCED_PARA28_2_H,							                
    ADVANCED_PARA28_2_L,							                
    ADVANCED_PARA28_3_H,							                
    ADVANCED_PARA28_3_L,							                
    ADVANCED_PARA28_4_H,							                
    ADVANCED_PARA28_4_L,							                
    ADVANCED_PARA28_5_H,							                
    ADVANCED_PARA28_5_L,							                
    ADVANCED_PARA28_6_H,							                
    ADVANCED_PARA28_6_L,							                
    ADVANCED_PARA28_7_H,							                
    ADVANCED_PARA28_7_L,							                
    ADVANCED_PARA28_8_H,							                
    ADVANCED_PARA28_8_L,							                
    ADVANCED_PARA28_9_H,							                
    ADVANCED_PARA28_9_L,							                
    ADVANCED_PARA28_10_H,							              
    ADVANCED_PARA28_10_L,							              
    ADVANCED_PARA28_11_H,							              
    ADVANCED_PARA28_11_L,							              
    ADVANCED_PARA28_12_H,							              
    ADVANCED_PARA28_12_L,							              
    ADVANCED_PARA28_13_H,							              
    ADVANCED_PARA28_13_L,							              
    ADVANCED_PARA28_14_H,							              
    ADVANCED_PARA28_14_L,							              
    ADVANCED_PARA28_15_H,							             
    ADVANCED_PARA28_15_L,							  
    ADVANCED_PARA28_16_H,							             
    ADVANCED_PARA28_16_L,							   
    ADVANCED_PARA28_17_H,							             
    ADVANCED_PARA28_17_L,							   
    ADVANCED_PARA28_18_H,							             
    ADVANCED_PARA28_18_L,							   
    ADVANCED_PARA28_19_H,							             
    ADVANCED_PARA28_19_L,							   

    FREE_REVERSE_ROTATION_SPEED28,				            
    FREE_REVERSE_ROTATION_ANGLE28_H,				          
    FREE_REVERSE_ROTATION_ANGLE28_L,				          
    REVERS_ANGLE_SETTING_SPEED28,				            
    REVERS_ANGLE_SETTING_ANGLE28,				            
    REVERS_ANGLE_SETTING_FW_REV28,		




    ADVANCED_MODE29 = 2260,			                  // 2260
    ADVANCED_PARA29_1_H,							                
    ADVANCED_PARA29_1_L,							                
    ADVANCED_PARA29_2_H,							                
    ADVANCED_PARA29_2_L,							                
    ADVANCED_PARA29_3_H,							                
    ADVANCED_PARA29_3_L,							                
    ADVANCED_PARA29_4_H,							                
    ADVANCED_PARA29_4_L,							                
    ADVANCED_PARA29_5_H,							                
    ADVANCED_PARA29_5_L,							                
    ADVANCED_PARA29_6_H,							                
    ADVANCED_PARA29_6_L,							                
    ADVANCED_PARA29_7_H,							                
    ADVANCED_PARA29_7_L,							                
    ADVANCED_PARA29_8_H,							                
    ADVANCED_PARA29_8_L,							                
    ADVANCED_PARA29_9_H,							                
    ADVANCED_PARA29_9_L,							                
    ADVANCED_PARA29_10_H,							              
    ADVANCED_PARA29_10_L,							              
    ADVANCED_PARA29_11_H,							              
    ADVANCED_PARA29_11_L,							              
    ADVANCED_PARA29_12_H,							              
    ADVANCED_PARA29_12_L,							              
    ADVANCED_PARA29_13_H,							              
    ADVANCED_PARA29_13_L,							              
    ADVANCED_PARA29_14_H,							              
    ADVANCED_PARA29_14_L,							              
    ADVANCED_PARA29_15_H,							             
    ADVANCED_PARA29_15_L,							  
    ADVANCED_PARA29_16_H,							             
    ADVANCED_PARA29_16_L,							   
    ADVANCED_PARA29_17_H,							             
    ADVANCED_PARA29_17_L,							   
    ADVANCED_PARA29_18_H,							             
    ADVANCED_PARA29_18_L,							   
    ADVANCED_PARA29_19_H,							             
    ADVANCED_PARA29_19_L,							   

    FREE_REVERSE_ROTATION_SPEED29,				            
    FREE_REVERSE_ROTATION_ANGLE29_H,				          
    FREE_REVERSE_ROTATION_ANGLE29_L,				          
    REVERS_ANGLE_SETTING_SPEED29,				            
    REVERS_ANGLE_SETTING_ANGLE29,				            
    REVERS_ANGLE_SETTING_FW_REV29,		



    ADVANCED_MODE30 = 2305,			                  // 2305
    ADVANCED_PARA30_1_H,							                
    ADVANCED_PARA30_1_L,							                
    ADVANCED_PARA30_2_H,							                
    ADVANCED_PARA30_2_L,							                
    ADVANCED_PARA30_3_H,							                
    ADVANCED_PARA30_3_L,							                
    ADVANCED_PARA30_4_H,							                
    ADVANCED_PARA30_4_L,							                
    ADVANCED_PARA30_5_H,							                
    ADVANCED_PARA30_5_L,							                
    ADVANCED_PARA30_6_H,							                
    ADVANCED_PARA30_6_L,							                
    ADVANCED_PARA30_7_H,							                
    ADVANCED_PARA30_7_L,							                
    ADVANCED_PARA30_8_H,							                
    ADVANCED_PARA30_8_L,							                
    ADVANCED_PARA30_9_H,							                
    ADVANCED_PARA30_9_L,							                
    ADVANCED_PARA30_10_H,							              
    ADVANCED_PARA30_10_L,							              
    ADVANCED_PARA30_11_H,							              
    ADVANCED_PARA30_11_L,							              
    ADVANCED_PARA30_12_H,							              
    ADVANCED_PARA30_12_L,							              
    ADVANCED_PARA30_13_H,							              
    ADVANCED_PARA30_13_L,							              
    ADVANCED_PARA30_14_H,							              
    ADVANCED_PARA30_14_L,							              
    ADVANCED_PARA30_15_H,							             
    ADVANCED_PARA30_15_L,							  
    ADVANCED_PARA30_16_H,							             
    ADVANCED_PARA30_16_L,							   
    ADVANCED_PARA30_17_H,							             
    ADVANCED_PARA30_17_L,							   
    ADVANCED_PARA30_18_H,							             
    ADVANCED_PARA30_18_L,							   
    ADVANCED_PARA30_19_H,							             
    ADVANCED_PARA30_19_L,							   

    FREE_REVERSE_ROTATION_SPEED30,				            
    FREE_REVERSE_ROTATION_ANGLE30_H,				          
    FREE_REVERSE_ROTATION_ANGLE30_L,				          
    REVERS_ANGLE_SETTING_SPEED30,				            
    REVERS_ANGLE_SETTING_ANGLE30,				            
    REVERS_ANGLE_SETTING_FW_REV30,		

    ADVANCED_MODE31 = 2350,			                  // 2350
    ADVANCED_PARA31_1_H,							                
    ADVANCED_PARA31_1_L,							                
    ADVANCED_PARA31_2_H,							                
    ADVANCED_PARA31_2_L,							                
    ADVANCED_PARA31_3_H,							                
    ADVANCED_PARA31_3_L,							                
    ADVANCED_PARA31_4_H,							                
    ADVANCED_PARA31_4_L,							                
    ADVANCED_PARA31_5_H,							                
    ADVANCED_PARA31_5_L,							                
    ADVANCED_PARA31_6_H,							                
    ADVANCED_PARA31_6_L,							                
    ADVANCED_PARA31_7_H,							                
    ADVANCED_PARA31_7_L,							                
    ADVANCED_PARA31_8_H,							                
    ADVANCED_PARA31_8_L,							                
    ADVANCED_PARA31_9_H,							                
    ADVANCED_PARA31_9_L,							                
    ADVANCED_PARA31_10_H,							              
    ADVANCED_PARA31_10_L,							              
    ADVANCED_PARA31_11_H,							              
    ADVANCED_PARA31_11_L,							              
    ADVANCED_PARA31_12_H,							              
    ADVANCED_PARA31_12_L,							              
    ADVANCED_PARA31_13_H,							              
    ADVANCED_PARA31_13_L,							              
    ADVANCED_PARA31_14_H,							              
    ADVANCED_PARA31_14_L,							              
    ADVANCED_PARA31_15_H,							             
    ADVANCED_PARA31_15_L,							  
    ADVANCED_PARA31_16_H,							             
    ADVANCED_PARA31_16_L,							   
    ADVANCED_PARA31_17_H,							             
    ADVANCED_PARA31_17_L,							   
    ADVANCED_PARA31_18_H,							             
    ADVANCED_PARA31_18_L,							   
    ADVANCED_PARA31_19_H,							             
    ADVANCED_PARA31_19_L,							   

    FREE_REVERSE_ROTATION_SPEED31,				            
    FREE_REVERSE_ROTATION_ANGLE31_H,				          
    FREE_REVERSE_ROTATION_ANGLE31_L,				          
    REVERS_ANGLE_SETTING_SPEED31,				            
    REVERS_ANGLE_SETTING_ANGLE31,				            
    REVERS_ANGLE_SETTING_FW_REV31,		




    ANYBUS_ENABLE = 2900,
    ANYBUS_MODULE_TYPE,

    ANYBUS_IP_ADDRESS_1 = 2906,
    ANYBUS_IP_ADDRESS_2,
    ANYBUS_IP_ADDRESS_3,
    ANYBUS_IP_ADDRESS_4,
    ANYBUS_NETMASK_1,
    ANYBUS_NETMASK_2,
    ANYBUS_NETMASK_3,
    ANYBUS_NETMASK_4,
    ANYBUS_GATEWAY_1,
    ANYBUS_GATEWAY_2,
    ANYBUS_GATEWAY_3,
    ANYBUS_GATEWAY_4,

    ANYBUS_IO_INPUT_1 = 2926,
    ANYBUS_IO_INPUT_2,
    ANYBUS_IO_INPUT_3,
    ANYBUS_IO_INPUT_4,
    ANYBUS_IO_INPUT_5,
    ANYBUS_IO_INPUT_6,
    ANYBUS_IO_INPUT_7,
    ANYBUS_IO_INPUT_8,

    ANYBUS_IO_OUTPUT_1 = 2942,
    ANYBUS_IO_OUTPUT_2,
    ANYBUS_IO_OUTPUT_3,
    ANYBUS_IO_OUTPUT_4,
    ANYBUS_IO_OUTPUT_5,
    ANYBUS_IO_OUTPUT_6,
    ANYBUS_IO_OUTPUT_7,
    ANYBUS_IO_OUTPUT_8,

    ANYBUS_MONITORING_1 = 2958,
    ANYBUS_MONITORING_2,
    ANYBUS_MONITORING_3,
    ANYBUS_MONITORING_4,
    ANYBUS_MONITORING_5,
    ANYBUS_MONITORING_6,
    ANYBUS_MONITORING_7,
    ANYBUS_MONITORING_8,
    ANYBUS_MONITORING_9,
    ANYBUS_MONITORING_10,
    //...
    ANYBUS_MONITORING_29 = 2986,
    ANYBUS_MONITORING_30,





    /* MODEL ----------------------------------------------------------------------------- */

    MODEL1_1 = 3000,                                  // 3000 
    MODEL1_2,                                         // 3001 
    MODEL1_3,                                         // 3002 
    MODEL1_4,                                         // 3003 
    MODEL1_5,                                         // 3004 
    MODEL1_6,                                         // 3005 
    MODEL1_7,                                         // 3006 
    MODEL1_8,                                         // 3007 
    MODEL1_9,                                         // 3008 
    MODEL1_10,                                        // 3009		            
    MODEL1_11,                                        // 3010		            
    MODEL1_12,                                        // 3011 		            
    MODEL1_13,                                        // 3012 		            
    MODEL1_14,                                        // 3013 		            
    MODEL1_15,                                        // 3014 		            
    MODEL1_16,                                        // 3015 		            
    MODEL1_17,                                        // 3016 		            
    MODEL1_18,                                        // 3017 		            
    MODEL1_19,                                        // 3018		            
    MODEL1_20,                                        // 3019 		      

    MODEL2_1 = 3030,                                  // 3030 
    MODEL2_2,                                         
    MODEL2_3,                                         
    MODEL2_4,                                         
    MODEL2_5,                                         
    MODEL2_6,                                         
    MODEL2_7,                                         
    MODEL2_8,                                         
    MODEL2_9,                                         
    MODEL2_10,                                        
    MODEL2_11,                                        
    MODEL2_12,                                        
    MODEL2_13,                                        
    MODEL2_14,                                        
    MODEL2_15,                                        
    MODEL2_16,                                        
    MODEL2_17,                                        
    MODEL2_18,                                        
    MODEL2_19,                                        
    MODEL2_20,                                        

    MODEL3_1 = 3060,                                  // 3060 
    MODEL3_2,                                         
    MODEL3_3,                                         
    MODEL3_4,                                         
    MODEL3_5,                                         
    MODEL3_6,                                         
    MODEL3_7,                                         
    MODEL3_8,                                         
    MODEL3_9,                                         
    MODEL3_10,                                        
    MODEL3_11,                                        
    MODEL3_12,                                        
    MODEL3_13,                                        
    MODEL3_14,                                        
    MODEL3_15,                                        
    MODEL3_16,                                        
    MODEL3_17,                                        
    MODEL3_18,                                        
    MODEL3_19,                                        
    MODEL3_20,                                        

    MODEL4_1 = 3090,                                  // 3090 
    MODEL4_2,                                         
    MODEL4_3,                                         
    MODEL4_4,                                         
    MODEL4_5,                                         
    MODEL4_6,                                         
    MODEL4_7,                                         
    MODEL4_8,                                         
    MODEL4_9,                                         
    MODEL4_10,                                        
    MODEL4_11,                                        
    MODEL4_12,                                        
    MODEL4_13,                                        
    MODEL4_14,                                        
    MODEL4_15,                                        
    MODEL4_16,                                        
    MODEL4_17,                                        
    MODEL4_18,                                        
    MODEL4_19,                                        
    MODEL4_20,                                        

    MODEL5_1 = 3120,                                  // 3120 
    MODEL5_2,                                         
    MODEL5_3,                                         
    MODEL5_4,                                         
    MODEL5_5,                                         
    MODEL5_6,                                         
    MODEL5_7,                                         
    MODEL5_8,                                         
    MODEL5_9,                                         
    MODEL5_10,                                        
    MODEL5_11,                                        
    MODEL5_12,                                        
    MODEL5_13,                                        
    MODEL5_14,                                        
    MODEL5_15,                                        
    MODEL5_16,                                        
    MODEL5_17,                                        
    MODEL5_18,                                        
    MODEL5_19,                                        
    MODEL5_20,                                        

    MODEL6_1 = 3150,                                  // 3150 
    MODEL6_2,                                         
    MODEL6_3,                                         
    MODEL6_4,                                         
    MODEL6_5,                                         
    MODEL6_6,                                         
    MODEL6_7,                                         
    MODEL6_8,                                         
    MODEL6_9,                                         
    MODEL6_10,                                        
    MODEL6_11,                                        
    MODEL6_12,                                        
    MODEL6_13,                                        
    MODEL6_14,                                        
    MODEL6_15,                                        
    MODEL6_16,                                        
    MODEL6_17,                                        
    MODEL6_18,                                        
    MODEL6_19,                                        
    MODEL6_20,                                        

    MODEL7_1 = 3180,                                  // 3180 
    MODEL7_2,                                         
    MODEL7_3,                                         
    MODEL7_4,                                         
    MODEL7_5,                                         
    MODEL7_6,                                         
    MODEL7_7,                                         
    MODEL7_8,                                         
    MODEL7_9,                                         
    MODEL7_10,                                        
    MODEL7_11,                                        
    MODEL7_12,                                        
    MODEL7_13,                                        
    MODEL7_14,                                        
    MODEL7_15,                                        
    MODEL7_16,                                        
    MODEL7_17,                                        
    MODEL7_18,                                        
    MODEL7_19,                                        
    MODEL7_20,                                        

    MODEL8_1 = 3210,                                  // 3210 
    MODEL8_2,                                         
    MODEL8_3,                                         
    MODEL8_4,                                         
    MODEL8_5,                                         
    MODEL8_6,                                         
    MODEL8_7,                                         
    MODEL8_8,                                         
    MODEL8_9,                                         
    MODEL8_10,                                        
    MODEL8_11,                                        
    MODEL8_12,                                        
    MODEL8_13,                                        
    MODEL8_14,                                        
    MODEL8_15,                                        
    MODEL8_16,                                        
    MODEL8_17,                                        
    MODEL8_18,                                        
    MODEL8_19,                                        
    MODEL8_20,                                        

    MODEL9_1 = 3240,                                  // 3240 
    MODEL9_2,                                         
    MODEL9_3,                                         
    MODEL9_4,                                         
    MODEL9_5,                                         
    MODEL9_6,                                         
    MODEL9_7,                                         
    MODEL9_8,                                         
    MODEL9_9,                                         
    MODEL9_10,                                        
    MODEL9_11,                                        
    MODEL9_12,                                        
    MODEL9_13,                                        
    MODEL9_14,                                        
    MODEL9_15,                                        
    MODEL9_16,                                        
    MODEL9_17,                                        
    MODEL9_18,                                        
    MODEL9_19,                                        
    MODEL9_20,                                        

    MODEL10_1 = 3270,                                  // 3270 
    MODEL10_2,                                         
    MODEL10_3,                                         
    MODEL10_4,                                         
    MODEL10_5,                                         
    MODEL10_6,                                         
    MODEL10_7,                                         
    MODEL10_8,                                         
    MODEL10_9,                                         
    MODEL10_10,                                        
    MODEL10_11,                                        
    MODEL10_12,                                        
    MODEL10_13,                                        
    MODEL10_14,                                        
    MODEL10_15,                                        
    MODEL10_16,                                        
    MODEL10_17,                                        
    MODEL10_18,                                        
    MODEL10_19,                                        
    MODEL10_20,                                        

    MODEL11_1 = 3300,                                  // 3300 
    MODEL11_2,                                         
    MODEL11_3,                                         
    MODEL11_4,                                         
    MODEL11_5,                                         
    MODEL11_6,                                         
    MODEL11_7,                                         
    MODEL11_8,                                         
    MODEL11_9,                                         
    MODEL11_10,                                        
    MODEL11_11,                                        
    MODEL11_12,                                        
    MODEL11_13,                                        
    MODEL11_14,                                        
    MODEL11_15,                                        
    MODEL11_16,                                        
    MODEL11_17,                                        
    MODEL11_18,                                        
    MODEL11_19,                                        
    MODEL11_20,                                        

    MODEL12_1 = 3330,                                  // 3330 
    MODEL12_2,                                         
    MODEL12_3,                                         
    MODEL12_4,                                         
    MODEL12_5,                                         
    MODEL12_6,                                         
    MODEL12_7,                                         
    MODEL12_8,                                         
    MODEL12_9,                                         
    MODEL12_10,                                        
    MODEL12_11,                                        
    MODEL12_12,                                        
    MODEL12_13,                                        
    MODEL12_14,                                        
    MODEL12_15,                                        
    MODEL12_16,                                        
    MODEL12_17,                                        
    MODEL12_18,                                        
    MODEL12_19,                                        
    MODEL12_20,                                        

    MODEL13_1 = 3360,                                  // 3360 
    MODEL13_2,                                         
    MODEL13_3,                                         
    MODEL13_4,                                         
    MODEL13_5,                                         
    MODEL13_6,                                         
    MODEL13_7,                                         
    MODEL13_8,                                         
    MODEL13_9,                                         
    MODEL13_10,                                        
    MODEL13_11,                                        
    MODEL13_12,                                        
    MODEL13_13,                                        
    MODEL13_14,                                        
    MODEL13_15,                                        
    MODEL13_16,                                        
    MODEL13_17,                                        
    MODEL13_18,                                        
    MODEL13_19,                                        
    MODEL13_20,                                        

    MODEL14_1 = 3390,                                  // 3390 
    MODEL14_2,                                         
    MODEL14_3,                                         
    MODEL14_4,                                         
    MODEL14_5,                                         
    MODEL14_6,                                         
    MODEL14_7,                                         
    MODEL14_8,                                         
    MODEL14_9,                                         
    MODEL14_10,                                        
    MODEL14_11,                                        
    MODEL14_12,                                        
    MODEL14_13,                                        
    MODEL14_14,                                        
    MODEL14_15,                                        
    MODEL14_16,                                        
    MODEL14_17,                                        
    MODEL14_18,                                        
    MODEL14_19,                                        
    MODEL14_20,                                        

    MODEL15_1 = 3420,                                  // 3420 
    MODEL15_2,                                         
    MODEL15_3,                                         
    MODEL15_4,                                         
    MODEL15_5,                                         
    MODEL15_6,                                         
    MODEL15_7,                                         
    MODEL15_8,                                         
    MODEL15_9,                                         
    MODEL15_10,                                        
    MODEL15_11,                                        
    MODEL15_12,                                        
    MODEL15_13,                                        
    MODEL15_14,                                        
    MODEL15_15,                                        
    MODEL15_16,                                        
    MODEL15_17,                                        
    MODEL15_18,                                        
    MODEL15_19,                                        
    MODEL15_20,                                        






    /* MULTI SEQUENCE ----------------------------------------------------------------------------- */

    MS_PG1 = 3500,                                    // 3500     
    MS_PG2,                                           // 3501
    MS_PG3,                                           // 3502
    MS_PG4,                                           // 3503
    MS_PG5,                                           // 3504
    MS_PG6,                                           // 3505
    MS_PG7,                                           // 3506
    MS_PG8,                                           // 3507
    MS_PG9,                                           // 3508
    MS_PG10,                                          // 3509

    MS_PG11 = 3520,                                   // 3520
    MS_PG12,                                          // 3521
    MS_PG13,                                          // 3522
    MS_PG14,                                          // 3523
    MS_PG15,                                          // 3524
    MS_PG16,                                          // 3525
    MS_PG17,                                          // 3526
    MS_PG18,                                          // 3527
    MS_PG19,                                          // 3528
    MS_PG20,                                          // 3529

    




    /* SCREW COUNT ----------------------------------------------------------------------------- */

    COUNT_MODE = 3550,							                // 3550 
    COUNT_LIMIT_TIME_H,					                        // 3551
    COUNT_LIMIT_TIME_L,					                        // 3552
    COUNT_OUTPUT_SIGNAL_TYPE,			                        // 3553
    COUNT_MIDDLE_CNT_NUMBER,			                        // 3554
    COUNT_START_DELAY_TIME,				                        // 3555
    COUNT_SCREW,						                        // 3556    (BM: SCREW_COUNT)
    SCREW_CANCEL_LOOSENING,						                // 3557    





    /* I/O ------------------------------------------------------------------------------------- */

    INPUT_MAPPING1 = 3600,					                    // 3600
    INPUT_MAPPING2,					                            // 3601
    INPUT_MAPPING3,					                            // 3602
    INPUT_MAPPING4,					                            // 3603
    INPUT_MAPPING5,					                            // 3604
    INPUT_MAPPING6,					                            // 3605
    INPUT_MAPPING7,					                            // 3606
    INPUT_MAPPING8,					                            // 3607

    OUTPUT_MAPPING1 = 3620,				                        // 3620
    OUTPUT_MAPPING2,				                            // 3621
    OUTPUT_MAPPING3,				                            // 3622
    OUTPUT_MAPPING4,				                            // 3623
    OUTPUT_MAPPING5,				                            // 3624
    OUTPUT_MAPPING6,				                            // 3625
    OUTPUT_MAPPING7,				                            // 3626
    OUTPUT_MAPPING8,				                            // 3627





    /* CONTROLLER ------------------------------------------------------------------------------------ */

    DRIVER_ID = 3650,							                // 3650
    DRIVER_MODEL,                                               // 3651
                                   
    UNIT = 3657,                                                // 3657
    SCREW_TYPE_H,                                               // 3658
    SCREW_TYPE_L,                                               // 3659
    TORQUE_COMPENSATION_MAIN,		                            // 3660
    ACC_DEA_TIME,                                               // 3661
    FASTEN_TORQUE_MAINTAIN_TIME,                                // 3662
    LOOSENING_SPEED,                                            // 3663
    JUDGE_FASTEN_MIN_TURNS_H,                                   // 3664
    JUDGE_FASTEN_MIN_TURNS_L,                                   // 3665
    FREE_SPEED_MAX_TORQUE_H,				                    // 3666
    FREE_SPEED_MAX_TORQUE_L,				                    // 3667
    TOTAL_FASTENING_TIME_H,                                     // 3668
    TOTAL_FASTENING_TIME_L,                                     // 3669
    TOTAL_LOOSENING_TIME_H,                                     // 3670
    TOTAL_LOOSENING_TIME_L,                                     // 3671
    INIT_LOOSENING_TIME_LIMIT_H,                                // 3672
    INIT_LOOSENING_TIME_LIMIT_L,                                // 3673
    HOLDING_TIME_ANGLE_LIMIT,			                        // 3674
    AUTO_SELECT_SPEED,                                          // 3675
    USE_MAXTQ_FOR_LOOSENING,                                    // 3676

    SELECT_DATA_TO_SAVE = 3682,					                // 3682       EVENT_DATA_SELECT
    FASTENING_COMPLETE_SIGNAL_OUT_TIME,	              
    FASTENING_STOP_ALARM,                             
    AUTO_LOCK,                                        
    START_TRIGGER_ENABLE,				                      
    REVERSE_SWITCH_START_ENABLE,		                  
    REVERSE_LOCK,					                            

    FASTEN_NO_SAVE = 3694,					                          
    LCD_TOUCH_LOCK,						                        
    MODEL_SELECT,						                          
    SELECT_BY_TOUCH_MODEL_PRESET,		                  
    PRESET_CHANGE_BY_BARCODE,                                   // 1641  START_BY_BARCODE_MODEL				                    
    MODEL_AUTO_RESTART,					                      

    PASSWORD = 3705,							                            
    SOUND_SETTING,						                        
    ERROR_CLEAR_TIME_H,                               
    ERROR_CLEAR_TIME_L, 
    LAMP_ON_TIME,                                     


    PROTOCOL = 3715,							                            
    SELECT_RS232, 						                        
    SELECT_BAUDRATE,					                        
  	AUTO_UPDATE_EVENT,					                      
  	AUTO_UPDATE_PORT,	


    CROWFOOT_ENABLE = 3725,                                  
    CROWFOOT_RATIO_H,                                 
    CROWFOOT_RATIO_L,                                 
    CROWFOOT_EFFICIENCY,                              
    CROWFOOT_REVERSE_TORQUE_H,                          
    CROWFOOT_REVERSE_TORQUE_L,                          
    CROWFOOT_REVERSE_SPEED,   

    SLEEP_IN_TIME = 3737,                                    
    LCD_KEY_LOCK,                                     
    TRIGGER_START_DELAY_TIME,                         
    DISPLAY_PRESET_NUM_H,                               
    DISPLAY_PRESET_NUM_L,                               
    LOCK_WIFI_DICONNECT_TIME,                         
    LF_SWITCH_TYPE,                                   






    /* NETWORK ------------------------------------------------------------------------------------- */

    WIFI_SELECT = 3800,                               // 3800
    NETWORK_MODE,                                     // 3801    (새로 추가 - ANYBUS 또는 W5500 중 어느것을 사용할지)
    STATIC_DHCP,                                      // 3802

    IP_ADDRESS1 = 3810,                               // 3810 
    IP_ADDRESS2,                                      // 3811
    IP_ADDRESS3,                                      // 3812
    IP_ADDRESS4,                                      // 3813
    SUBNETMASK1,                                      // 3814
    SUBNETMASK2,                                      
    SUBNETMASK3,                                      
    SUBNETMASK4,                                      // 3817
    GATEWAY1,                                         // 3818
    GATEWAY2,                                         
    GATEWAY3,                                         
    GATEWAY4,                                         
    ETHERNET_PORT,                                    // 3822

    TARGET_IP1 = 3830,                                // 3830
    TARGET_IP2,                                       
    TARGET_IP3,                                       
    TARGET_IP4,                                       
    TARGET_PORT,                                      // 3854

    AP_SSID_CHAR1 = 3840,                             // 3840                      // AP SSID 32 char
    AP_SSID_CHAR2,                                    // 3841 
    AP_SSID_CHAR3,                                    // 3842
    AP_SSID_CHAR4,                                    // 3843
    AP_SSID_CHAR5,                                    // 3844
    AP_SSID_CHAR6,                                    // 3845
    AP_SSID_CHAR7,                                    // 3846
    AP_SSID_CHAR8,                                    // 3847
    AP_SSID_CHAR9,                                    // 3848
    AP_SSID_CHAR10,                                   // 3849
    AP_SSID_CHAR11,                                   // 3850
    AP_SSID_CHAR12,                                   // 3851
    AP_SSID_CHAR13,                                   // 3852
    AP_SSID_CHAR14,                                   // 3853
    AP_SSID_CHAR15,                                   // 3854
    AP_SSID_CHAR16,                                   // 3855

    AP_PW_CHAR1,                                      // 3856                        // AP PW 64 char  
    AP_PW_CHAR2,                                      // 3857 
    AP_PW_CHAR3,                                      // 3858 
    AP_PW_CHAR4,                                      // 3859 
    AP_PW_CHAR5,                                      // 3860 
    AP_PW_CHAR6,                                      // 3861 
    AP_PW_CHAR7,                                      // 3862 
    AP_PW_CHAR8,                                      // 3863 
    AP_PW_CHAR9,                                      // 3864 
    AP_PW_CHAR10,                                     // 3865 
    AP_PW_CHAR11,                                     // 3866 
    AP_PW_CHAR12,                                     // 3867 
    AP_PW_CHAR13,                                     // 3868 
    AP_PW_CHAR14,                                     // 3869 
    AP_PW_CHAR15,                                     // 3870 
    AP_PW_CHAR16,                                     // 3871 
    AP_PW_CHAR17,                                     // 3872 
    AP_PW_CHAR18,                                     // 3873 
    AP_PW_CHAR19,                                     // 3874 
    AP_PW_CHAR20,                                     // 3875 
    AP_PW_CHAR21,                                     // 3876 
    AP_PW_CHAR22,                                     // 3877 
    AP_PW_CHAR23,                                     // 3878 
    AP_PW_CHAR24,                                     // 3879 
    AP_PW_CHAR25,                                     // 3880 
    AP_PW_CHAR26,                                     // 3881 
    AP_PW_CHAR27,                                     // 3882 
    AP_PW_CHAR28,                                     // 3883 
    AP_PW_CHAR29,                                     // 3884 
    AP_PW_CHAR30,                                     // 3885 
    AP_PW_CHAR31,                                     // 3886 
    AP_PW_CHAR32,                                     // 3887 
    AP_COUNTRY,                                       // 3888    


    WIFI_EAP_ENABLE = 3900,                           // 3900
    WIFI_EAP_METHOD,                                  // 3901

    WIFI_EAP_USER_ID_1,                               // 3902
    WIFI_EAP_USER_ID_2,
    WIFI_EAP_USER_ID_3,
    WIFI_EAP_USER_ID_4,
    WIFI_EAP_USER_ID_5,
    WIFI_EAP_USER_ID_6,
    WIFI_EAP_USER_ID_7,
    WIFI_EAP_USER_ID_8,
    WIFI_EAP_USER_ID_9,
    WIFI_EAP_USER_ID_10,
    WIFI_EAP_USER_ID_11,
    WIFI_EAP_USER_ID_12,
    WIFI_EAP_USER_ID_13,
    WIFI_EAP_USER_ID_14,
    WIFI_EAP_USER_ID_15,
    WIFI_EAP_USER_ID_16,

    WIFI_EAP_USER_PASSWORD_1,                           // 3918
    WIFI_EAP_USER_PASSWORD_2,
    WIFI_EAP_USER_PASSWORD_3,
    WIFI_EAP_USER_PASSWORD_4,
    WIFI_EAP_USER_PASSWORD_5,
    WIFI_EAP_USER_PASSWORD_6,
    WIFI_EAP_USER_PASSWORD_7,
    WIFI_EAP_USER_PASSWORD_8,
    WIFI_EAP_USER_PASSWORD_9,
    WIFI_EAP_USER_PASSWORD_10,
    WIFI_EAP_USER_PASSWORD_11,
    WIFI_EAP_USER_PASSWORD_12,
    WIFI_EAP_USER_PASSWORD_13,
    WIFI_EAP_USER_PASSWORD_14,
    WIFI_EAP_USER_PASSWORD_15,
    WIFI_EAP_USER_PASSWORD_16,
    


    



    /* ETC ------------------------------------------------------------------------------------- */

    


}PARAMETER_ADDRESS_LIST;










#ifdef __cplusplus
    }
#endif
#endif //PARAMETER_H
