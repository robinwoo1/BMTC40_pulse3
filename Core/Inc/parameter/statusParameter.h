/**
  ******************************************************************************
  * @file    statusParameter.h
  * @author  jun
  * @brief   Header file of status parameter.
  ******************************************************************************
  * @version
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STATUS_PARAMETER_H
#define __STATUS_PARAMETER_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/

/**
 * EEPROM 메모리 영역 중 Status 영역 설정
 * - 0x1F40 번지 부터 시작 ~ 0x2000
 * 
 * 0x1F40: Graph Monitoring - graph start index (2byte)         GRAPH_EVENT_CYCLE_INDEX_ADDR
 * 0x1F42: Graph Monitoring - graph save index  (2byte)         GRAPH_EVENT_GRAPH_SAVE_INDEX_ADDR
 * 0x1F44: Graph monitoring index 1 turn over   (1byte)         REALTIME_EVENT_SAVE_1TURN_OVER_ADDRESS
 * 0x1F45: Graph Monitoring setting             (4byte)         GRAPH_EVENT_EEPROM_SETTING_ADDR
 * 0x1F49: Graph Monitoring realtime save index (2byte)         REALTIME_EVENT_SAVE_INDEX_ADDRESS
 * 0x1F4B: Graph Monitoring realtime send index (2byte)         REALTIME_EVENT_SEND_INDEX_ADDRESS
 * 
 * 0x1F4F: Total screw - enable                 (1byte)         TOTAL_SCREW_ALARM_ACTIVE_ADDRESS
 * 0x1F50: Total screw - total count            (4byte)         TOTAL_SCREW_ADDRESS
 * 0x1F54: Total screw - reset count            (4byte)         RESET_SCREW_ADDRESS
 * 0x1F58: Total screw - limit count            (4byte)         LIMIT_SCREW_ADDRESS
 * 
 * 0x1F60: last preset number                   (1byte)         LAST_PRESET_NUM
 * 0x1F61: last model step                      (1byte)         LAST_MODEL_STEP
 * 0x1F62: last model screw                     (2byte)         LAST_MODEL_SCREW
 * 
 * 0x1F6A: sleep screw count                    (2byte)         SLEEP_SCREW_COUNT
 * 0x1F6C: webpage load state                   (1byte)         WEBPAGE_LOAD
 * 
 * 0x1F70: Controller serial number             (10byte)        CONTROLLER_PARA_SERIAL_NUMBER_ADDRESS
 * 0x1F7A: Controller product date              (4byte)         CONTROLLER_PARA_PRODUCT_DATE_ADDRESS
 * 
 */


#define LAST_PRESET_NUM                 0x1F60
#define LAST_MODEL_STEP                 0x1F61
#define LAST_MODEL_SCREW                0x1F62

#define SLEEP_SCREW_COUNT               0x1F6A
#define WEBPAGE_LOAD                    0x1F6C

#define CONTROLLER_PARA_SERIAL_NUMBER_ADDRESS       0x1F70
#define CONTROLLER_PARA_PRODUCT_DATE_ADDRESS        0x1F7A



/* Enums ---------------------------------------------------------------------*/


/* Function code 4 ---------------------------------------------------------------------------------*/

typedef enum {
    INFORMATION_DRIVER_ID = 1,                             // 1     (Function code 4)
    INFORMATION_DRIVER_MODEL_NUMBER,
    INFORMATION_DRIVER_MODEL_NAME_1,
    INFORMATION_DRIVER_MODEL_NAME_2,
    INFORMATION_DRIVER_MODEL_NAME_3,
    INFORMATION_DRIVER_MODEL_NAME_4,
    // ... //
    INFORMATION_DRIVER_MODEL_NAME_16 = 18,
    INFORMATION_DRIVER_SERIAL_NUMBER_1,
    INFORMATION_DRIVER_SERIAL_NUMBER_2,
    INFORMATION_DRIVER_SERIAL_NUMBER_3,
    INFORMATION_DRIVER_SERIAL_NUMBER_4,
    INFORMATION_DRIVER_SERIAL_NUMBER_5,

    INFORMATION_CONTROLLER_MODEL_NUMBER,
    INFORMATION_CONTROLLER_MODEL_NAME_1,
    INFORMATION_CONTROLLER_MODEL_NAME_2,
    INFORMATION_CONTROLLER_MODEL_NAME_3,
    INFORMATION_CONTROLLER_MODEL_NAME_4,
    // ... //
    INFORMATION_CONTROLLER_MODEL_NAME_16 = 40,
    INFORMATION_CONTROLLER_SERIAL_NUMBER_1,
    INFORMATION_CONTROLLER_SERIAL_NUMBER_2,
    INFORMATION_CONTROLLER_SERIAL_NUMBER_3,
    INFORMATION_CONTROLLER_SERIAL_NUMBER_4,
    INFORMATION_CONTROLLER_SERIAL_NUMBER_5,

    INFORMATION_CONTROLLER_FIRMWARE_1,
    INFORMATION_CONTROLLER_FIRMWARE_2,
    INFORMATION_CONTROLLER_FIRMWARE_3,
    INFORMATION_CONTROLLER_PRODUCT_DATE_1,
    INFORMATION_CONTROLLER_PRODUCT_DATE_2,

    INFORMATION_CONTROLLER_ADVANCED_TYPE,

    INFORMATION_MAC_ADDRESS_1,
    INFORMATION_MAC_ADDRESS_2,
    INFORMATION_MAC_ADDRESS_3,


    INFORMATION_END_ADDRESS = 99,
          
}INFORMATION_ADDRESS_LIST;


typedef enum {
    CURRENT_TORQUE_H = 100,                             // 100  (Function code 4)
    CURRENT_TORQUE_L,
    CURRENT_SPEED,
    CURRENT_CURRENT_H,
    CURRENT_CURRENT_L,
    CURRENT_PRESET,
    CURRENT_MODEL,
    CURRENT_TORQUE_UP_IO,
    CURRENT_FASTENING_OK_IO,
    CURRENT_READY,
    CURRENT_RUN,
    CURRENT_ALARM,
    CURRENT_FL,
    CURRENT_SCREW_COUNT,
    CURRENT_INPUT_IO,
    CURRENT_OUTPUT_IO,
    CURRENT_TEMPERATURE_H,
    CURRENT_TEMPERATURE_L,
    CURRENT_LOCK,


    CURRENT_STATE_END_ADDRESS = 199,
          
}CURRENT_STATE_ADDRESS_LIST;


typedef enum {
    GRAPH_REALTIME_START_ADDRESS = 200,


    GRAPH_REVISION = 200,                             // 200 (Function code 4)
    GRAPH_EVENT_ID,
    GRAPH_RTC1,
    GRAPH_RTC2,
    GRAPH_RTC3,
    GRAPH_RTC4,

    GRAPH_FASTENING_TIME,
    GRAPH_PRESET,
    GRAPH_UNIT,
    GRAPH_SCREW_COUNT,
    GRAPH_FL,
    GRAPH_ERROR_CODE,
    GRAPH_STATUS,

    GRAPH_TARGET_TORQUE_H,
    GRAPH_TARGET_TORQUE_L,
    GRAPH_CONVERTED_TORQUE_H,
    GRAPH_CONVERTED_TORQUE_L,
    GRAPH_SEATING_TORQUE_H,
    GRAPH_SEATING_TORQUE_L,
    GRAPH_CLAMP_TORQUE_H,
    GRAPH_CLAMP_TORQUE_L,
    GRAPH_PREVAILING_TORQUE_H,
    GRAPH_PREVAILING_TORQUE_L,
    GRAPH_SNUG_TORQUE_H,
    GRAPH_SNUG_TORQUE_L,

    GRAPH_SPEED,
    GRAPH_ANGLE1,
    GRAPH_ANGLE2,
    GRAPH_ANGLE3,
    GRAPH_SNUG_ANGLE,

    GRAPH_RESERVED1,
    // ... //

    GRAPH_BARCODE1 = 238,
    GRAPH_BARCODE2,
    GRAPH_BARCODE3,
    GRAPH_BARCODE4,
    // ... //
    GRAPH_BARCODE32 = 269,

    GRAPH_CHANNEL_TYPE_1,
    GRAPH_CHANNEL_TYPE_2,
    GRAPH_CHANNEL_COUNT_1,
    GRAPH_CHANNEL_COUNT_2,
    GRAPH_SAMPLING_TIME,
    
    GRAPH_STEP_INFO_1_1,
    GRAPH_STEP_INFO_1_2,
    GRAPH_STEP_INFO_2_1,
    GRAPH_STEP_INFO_2_2,
    // ... //
    GRAPH_STEP_INFO_16_1 = 304,
    GRAPH_STEP_INFO_16_2,

    GRAPH_REALTIME_END_ADDRESS = 306,               // Realtime END
    

    GRAPH_DATA_CAHNNEL1_1 = 2000,
    GRAPH_DATA_CAHNNEL1_2,
    //  ...
    GRAPH_DATA_CAHNNEL1_1999 = 3999,

    GRAPH_DATA_CAHNNEL2_1 = 4000,
    GRAPH_DATA_CAHNNEL2_2,
    //  ...
    GRAPH_DATA_CAHNNEL2_1999 = 5999,


    GRAPH_MONI_END_ADDRESS = 6999,

          
}GRAPH_MONITORING_ADDRESS_LIST;


typedef enum {
    ERROR1_CODE = 7000,                               // 7000
    ERROR1_DUMMY1,                                    // 7001
    ERROR1_DUMMY2,                                    // 7002
    ERROR1_DATE1,                                     // 7003
    ERROR1_DATE2,                                     // 7004
    ERROR1_TIME1,                                     // 7005
    ERROR1_TIME2,                                     // 7006

    ERROR2_CODE,                                      // 7007
    ERROR2_DUMMY1,                                    
    ERROR2_DUMMY2,                                    
    ERROR2_DATE1,                                     
    ERROR2_DATE2,                                     
    ERROR2_TIME1,                                     
    ERROR2_TIME2,                                     
    
    // ... //

    ERROR32_CODE = 7217,                              // 7217
    ERROR32_DUMMY1,                                    
    ERROR32_DUMMY2,                                    
    ERROR32_DATE1,                                     
    ERROR32_DATE2,                                     
    ERROR32_TIME1,                                     
    ERROR32_TIME2,                                     


    ERROR_END_ADDRESS = 7999,

}ERROR_ADDRESS_LIST;


typedef enum {
    FIELDBUS_DIGITAL_INPUT = 8000,                    // 8000  (Function code 4)
    FIELDBUS_DIGITAL_OUTPUT = 8002,

    FIELDBUS_MONITOR_DATA_1 = 8010,
    FIELDBUS_MONITOR_DATA_2 = 8012,

    // ... //

    FIELDBUS_MONITOR_DATA_29 = 8066,
    FIELDBUS_MONITOR_DATA_30 = 8068,
    

    FIELDBUS_STATE_END_ADDRESS = 8068,
          
}FIELDBUS_STATE_ADDRESS_LIST;




/* Extern -----------------------------------------------------------------------*/



/* Functions --------------------------------------------------------------------*/






#ifdef __cplusplus
    }
#endif

#endif





