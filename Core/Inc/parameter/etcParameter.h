/**
  ******************************************************************************
  * @file    etcParameter.h
  * @author  jun
  * @brief   Header file of parameter 중 저장하지 않고 외부 통신용 명령 주소.
  ******************************************************************************
  * @version
  * 
  * 
  * @note 사용법
  * - .h 파일
  * 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ETC_PARAMETER_H
#define __ETC_PARAMETER_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"



/* Define ------------------------------------------------------------------*/





/* Enums ---------------------------------------------------------------------*/


/* Struct ---------------------------------------------------------------------*/



typedef enum {
    DRIVER_RUN_STOP = 5000,                           // 5000      
    F_L_CONTROL,                                      // 5001
    DRIVER_LOCK,                                      // 5002
    PRESET_CHANGE,                                    // 5003
    MODEL_CHANGE,                                     // 5004
    ALARM_RESET,                                      // 5005
    PARA_INIT,                                        // 5006      
    DRIVER_PARA_INIT,                                 // 5007    
    ERROR_LIST_INIT,                                  // 5008   
    IO_REMOTE_ACTIVE,                                 // 5009    
    IO_REMOTE_STATE,                                  // 5010   
    FIRMWARE_UPDATE_START,                            // 5011         
                                        
    
    RTC_YEAR = 5050,                                     
    RTC_MONTH,                                     
    RTC_DAY,                                     
    RTC_HOUR,                                     
    RTC_MINUTE,                                     
    RTC_SEC,                                     
   
}REMOTE_ADDRESS_LIST;


typedef enum {
    AUTO_CUSTOMIZE_SORT_HART_JOINT = 5100,            // 5100             
    AUTO_CUSTOMIZE_ACTIVE,                                      
    AUTO_CUSTOMIZE_SAVE,                                      
    AUTO_CUSTOMIZE_ANGLE1,                                    
    AUTO_CUSTOMIZE_ANGLE2,                                    
    AUTO_CUSTOMIZE_SPEED,                                     
    AUTO_CUSTOMIZE_FREE_SPEED,                                     
    AUTO_CUSTOMIZE_FREE_ANGLE,                                     
    AUTO_CUSTOMIZE_SEATING_POINT,                                     
   
}AUTO_CUSTOMIZE_ADDRESS_LIST;


typedef enum {
    SENSOR_CAL_SEN_CALC_START = 5150,                   // 5150             
    SENSOR_CAL_SENSOR_VAL,                                      
    SENSOR_CAL_OFFSET_VAL,                                      
    SENSOR_CAL_CALIB_VAL,                                    
   
}SENSOR_CALIBRATION_ADDRESS_LIST;


typedef enum {
    BIT_SOCKET_TRAY_COMM_ENABLE = 5180,                  // 5180             
   
}BIT_SOCKET_TRAY_ADDRESS_LIST;


typedef enum {
    VIRTAUL_PRESET_TCAM_ACTM1 = 5200,                           // 5200             
    VIRTAUL_PRESET_FASTEN_TORQUE1_H,                                      
    VIRTAUL_PRESET_FASTEN_TORQUE1_L,                                      
    VIRTAUL_PRESET_TORQUE_MIN_MAX1_H,                                    
    VIRTAUL_PRESET_TORQUE_MIN_MAX1_L,                                    
    VIRTAUL_PRESET_TARGET_ANGLE1,                                    
    VIRTAUL_PRESET_FASTEN_MIN_TURNS1,                                    
    VIRTAUL_PRESET_FASTEN_MAX_TURNS1,                                    
    VIRTAUL_PRESET_SNUG_TORQUE1_H,                                    
    VIRTAUL_PRESET_SNUG_TORQUE1_L,     
    VIRTAUL_PRESET_FASTEN_SPEED1,                                    
    VIRTAUL_PRESET_FREE_FASTEN_ANGLE1,                                    
    VIRTAUL_PRESET_FREE_FASTEN_SPEED1,                                    
    VIRTAUL_PRESET_SOFT_START1,                                    
    VIRTAUL_PRESET_FASTEN_SEATTING_POINT_RATE1,                                    
    VIRTAUL_PRESET_FASTEN_TQ_RISING_TIME1,                                    
    VIRTAUL_PRESET_RAMP_UP_SPEED1,                                    
    VIRTAUL_PRESET_TORQUE_COMPENSATION1,                                    

    VIRTAUL_PRESET_ADVANCED_MODE1 = 5230,                       // 5230       
    VIRTAUL_PRESET_ADVANCED_PARA1_1_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_1_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_2_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_2_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_3_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_3_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_4_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_4_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_5_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_5_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_6_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_6_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_7_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_7_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_8_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_8_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_9_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_9_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_10_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_10_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_11_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_11_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_12_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_12_L,                                    
    VIRTAUL_PRESET_ADVANCED_PARA1_13_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_13_L,
    VIRTAUL_PRESET_ADVANCED_PARA1_14_H,
    VIRTAUL_PRESET_ADVANCED_PARA1_14_L,

    VIRTAUL_PRESET_FREE_REVERSE_ROTATION_SPEED1 = 5269,                                    
    VIRTAUL_PRESET_FREE_REVERSE_ROTATION_ANGLE1_H,                                    
    VIRTAUL_PRESET_FREE_REVERSE_ROTATION_ANGLE1_L,                                    
    VIRTAUL_PRESET_REVERS_ANGLE_SETTING_SPEED1,                                    
    VIRTAUL_PRESET_REVERS_ANGLE_SETTING_ANGLE1,                                    
    VIRTAUL_PRESET_REVERS_ANGLE_SETTING_FW_REV1,                                    

    VIRTAUL_PRESET_MODEL_1_1 = 5280,                            // 5280
    VIRTAUL_PRESET_MODEL_1_2,
    VIRTAUL_PRESET_MODEL_1_3,
    VIRTAUL_PRESET_MODEL_1_4,
    VIRTAUL_PRESET_MODEL_1_5,
    VIRTAUL_PRESET_MODEL_1_6,
    VIRTAUL_PRESET_MODEL_1_7,
    VIRTAUL_PRESET_MODEL_1_8,
    VIRTAUL_PRESET_MODEL_1_9,
    VIRTAUL_PRESET_MODEL_1_10,
    VIRTAUL_PRESET_MODEL_1_11,
    VIRTAUL_PRESET_MODEL_1_12,
    VIRTAUL_PRESET_MODEL_1_13,
    VIRTAUL_PRESET_MODEL_1_14,
    VIRTAUL_PRESET_MODEL_1_15,
    VIRTAUL_PRESET_MODEL_1_16,
    VIRTAUL_PRESET_MODEL_1_17,
    VIRTAUL_PRESET_MODEL_1_18,
    VIRTAUL_PRESET_MODEL_1_19,
    VIRTAUL_PRESET_MODEL_1_20,
   
}VIRTAUL_PRESET_ADDRESS_LIST;


typedef enum {
    REALTIME_TRIGGER_MONITORING_ENABLE = 5350,                     // 5350             
    
    REALTIME_USB_ENABLE,                                      
    REALTIME_USB_START_INDEX,                                    
    REALTIME_USB_END_INDEX,                                    
    REALTIME_USB_1CYCLE_OVER,                                    
   
}REALTIME_MONITORING_ADDRESS_LIST;


typedef enum {
    GRAPH_TRIGGER_MONITORING_ENABLE = 5370,                         // 5370             
    GRAPH_MONITOINRG_CHANNEL1,
    GRAPH_MONITOINRG_CHANNEL2,
    GRAPH_MONITOINRG_SAMPLING,
    GRAPH_MONITOINRG_DIRECTION,
                             
}GRAPH_MONITORING_SETTING_ADDRESS_LIST;


typedef enum {
    GRAPH_EVENT_SETTING_H = 5390,                                   // 5390     
    GRAPH_EVENT_SETTING_L,      
    GRAPH_EVENT_RECEIVE_ACK,
    GRAPH_EVENT_SAVED_DATA_NUM,
    GRAPH_EVENT_REVISION,
    GRAPH_EVENT_KEEP_ALIVE,
                             
}GRAPH_EVENT_ADDRESS_LIST;


typedef enum {
    BARCODE_READ_ACTIVE = 6000,                                     // 6000             
    BARCODE_INDEX_CLEAR,
    BARCODE_SETTING_SAVE,
    
    BARCODE_PRESET1 = 6012,
    BARCODE_MASK_BIT1_1,
    BARCODE_MASK_BIT1_2,
    BARCODE_MASK_BIT1_3,
    BARCODE_MASK_BIT1_4,

    BARCODE_DATA_1_1 = 6020,
    BARCODE_DATA_1_2,
    BARCODE_DATA_1_3,
    BARCODE_DATA_1_4,
    BARCODE_DATA_1_5,
    BARCODE_DATA_1_6,
    BARCODE_DATA_1_7,
    BARCODE_DATA_1_8,
    BARCODE_DATA_1_9,
    BARCODE_DATA_1_10,
    BARCODE_DATA_1_11,
    BARCODE_DATA_1_12,
    BARCODE_DATA_1_13,
    BARCODE_DATA_1_14,
    BARCODE_DATA_1_15,
    BARCODE_DATA_1_16,
    BARCODE_DATA_1_17,
    BARCODE_DATA_1_18,
    BARCODE_DATA_1_19,
    BARCODE_DATA_1_20,
    BARCODE_DATA_1_21,
    BARCODE_DATA_1_22,
    BARCODE_DATA_1_23,
    BARCODE_DATA_1_24,
    BARCODE_DATA_1_25,
    BARCODE_DATA_1_26,
    BARCODE_DATA_1_27,
    BARCODE_DATA_1_28,
    BARCODE_DATA_1_29,
    BARCODE_DATA_1_30,
    BARCODE_DATA_1_31,
    BARCODE_DATA_1_32,


    BARCODE_PRESET2 = 6052,




    BARCODE_PRESET20 = 6772,
    BARCODE_MASK_BIT20_1,
    BARCODE_MASK_BIT20_2,
    BARCODE_MASK_BIT20_3,
    BARCODE_MASK_BIT20_4,

    BARCODE_DATA_20_1 = 6780,
    BARCODE_DATA_20_2,
    BARCODE_DATA_20_3,
    BARCODE_DATA_20_4,
    BARCODE_DATA_20_5,
    BARCODE_DATA_20_6,
    BARCODE_DATA_20_7,
    BARCODE_DATA_20_8,
    BARCODE_DATA_20_9,
    BARCODE_DATA_20_10,
    BARCODE_DATA_20_11,
    BARCODE_DATA_20_12,
    BARCODE_DATA_20_13,
    BARCODE_DATA_20_14,
    BARCODE_DATA_20_15,
    BARCODE_DATA_20_16,
    BARCODE_DATA_20_17,
    BARCODE_DATA_20_18,
    BARCODE_DATA_20_19,
    BARCODE_DATA_20_20,
    BARCODE_DATA_20_21,
    BARCODE_DATA_20_22,
    BARCODE_DATA_20_23,
    BARCODE_DATA_20_24,
    BARCODE_DATA_20_25,
    BARCODE_DATA_20_26,
    BARCODE_DATA_20_27,
    BARCODE_DATA_20_28,
    BARCODE_DATA_20_29,
    BARCODE_DATA_20_30,
    BARCODE_DATA_20_31,
    BARCODE_DATA_20_32,    
   
   
}BARCODE_ADDRESS_LIST;


typedef enum {
    MODEL_BARCODE_READ_ACTIVE = 7000,                                 // 7000             
    MODEL_BARCODE_INDEX_CLEAR,
    MODEL_BARCODE_SETTING_SAVE,
    
    MODEL_BARCODE_PRESET1 = 7012,
    MODEL_BARCODE_MASK_BIT1_1,
    MODEL_BARCODE_MASK_BIT1_2,
    MODEL_BARCODE_MASK_BIT1_3,
    MODEL_BARCODE_MASK_BIT1_4,

    MODEL_BARCODE_DATA_1_1 = 7020,
    MODEL_BARCODE_DATA_1_2,
    MODEL_BARCODE_DATA_1_3,
    MODEL_BARCODE_DATA_1_4,
    MODEL_BARCODE_DATA_1_5,
    MODEL_BARCODE_DATA_1_6,
    MODEL_BARCODE_DATA_1_7,
    MODEL_BARCODE_DATA_1_8,
    MODEL_BARCODE_DATA_1_9,
    MODEL_BARCODE_DATA_1_10,
    MODEL_BARCODE_DATA_1_11,
    MODEL_BARCODE_DATA_1_12,
    MODEL_BARCODE_DATA_1_13,
    MODEL_BARCODE_DATA_1_14,
    MODEL_BARCODE_DATA_1_15,
    MODEL_BARCODE_DATA_1_16,
    MODEL_BARCODE_DATA_1_17,
    MODEL_BARCODE_DATA_1_18,
    MODEL_BARCODE_DATA_1_19,
    MODEL_BARCODE_DATA_1_20,
    MODEL_BARCODE_DATA_1_21,
    MODEL_BARCODE_DATA_1_22,
    MODEL_BARCODE_DATA_1_23,
    MODEL_BARCODE_DATA_1_24,
    MODEL_BARCODE_DATA_1_25,
    MODEL_BARCODE_DATA_1_26,
    MODEL_BARCODE_DATA_1_27,
    MODEL_BARCODE_DATA_1_28,
    MODEL_BARCODE_DATA_1_29,
    MODEL_BARCODE_DATA_1_30,
    MODEL_BARCODE_DATA_1_31,
    MODEL_BARCODE_DATA_1_32,


    MODEL_BARCODE_PRESET2 = 7052,




    MODEL_BARCODE_PRESET20 = 7772,
    MODEL_BARCODE_MASK_BIT20_1,
    MODEL_BARCODE_MASK_BIT20_2,
    MODEL_BARCODE_MASK_BIT20_3,
    MODEL_BARCODE_MASK_BIT20_4,

    MODEL_BARCODE_DATA_20_1 = 7780,
    MODEL_BARCODE_DATA_20_2,
    MODEL_BARCODE_DATA_20_3,
    MODEL_BARCODE_DATA_20_4,
    MODEL_BARCODE_DATA_20_5,
    MODEL_BARCODE_DATA_20_6,
    MODEL_BARCODE_DATA_20_7,
    MODEL_BARCODE_DATA_20_8,
    MODEL_BARCODE_DATA_20_9,
    MODEL_BARCODE_DATA_20_10,
    MODEL_BARCODE_DATA_20_11,
    MODEL_BARCODE_DATA_20_12,
    MODEL_BARCODE_DATA_20_13,
    MODEL_BARCODE_DATA_20_14,
    MODEL_BARCODE_DATA_20_15,
    MODEL_BARCODE_DATA_20_16,
    MODEL_BARCODE_DATA_20_17,
    MODEL_BARCODE_DATA_20_18,
    MODEL_BARCODE_DATA_20_19,
    MODEL_BARCODE_DATA_20_20,
    MODEL_BARCODE_DATA_20_21,
    MODEL_BARCODE_DATA_20_22,
    MODEL_BARCODE_DATA_20_23,
    MODEL_BARCODE_DATA_20_24,
    MODEL_BARCODE_DATA_20_25,
    MODEL_BARCODE_DATA_20_26,
    MODEL_BARCODE_DATA_20_27,
    MODEL_BARCODE_DATA_20_28,
    MODEL_BARCODE_DATA_20_29,
    MODEL_BARCODE_DATA_20_30,
    MODEL_BARCODE_DATA_20_31,
    MODEL_BARCODE_DATA_20_32,    
   
   
}MODEL_BARCODE_ADDRESS_LIST;


typedef enum {
    MAINTENANCE_ENABLE = 8000,                          // 8000
    MAINTENANCE_ALARM_COUNT_1,
    MAINTENANCE_ALARM_COUNT_2,
    MAINTENANCE_START_COUNT_1,
    MAINTENANCE_START_COUNT_2, 
    MAINTENANCE_USED_COUNT_1,
    MAINTENANCE_USED_COUNT_2, 

} MAINTENANCE_ADDRESS_LIST;


typedef enum {
    DRIVER_PARA1 = 9000,                              // 9000 - NOT USE            
    DRIVER_PARA2,                                      
    DRIVER_PARA3,                                      
    DRIVER_PARA4,                                     // factory compensation
    DRIVER_PARA5,                                     // User compensation 
    DRIVER_PARA6,                                     // Driver model
    DRIVER_PARA7,                                     // Serial number 1 2
    DRIVER_PARA8,                                     // Serial number 3 4
    DRIVER_PARA9,                                     // Product code 
    DRIVER_PARA10,                                    // Serial number 7 8
    DRIVER_PARA11,                                    // Serial number 9 10
    DRIVER_PARA12,                                    // Fasctory compensation
    DRIVER_PARA13,                                    // Sensor offset high                
    DRIVER_PARA14,                                    // Sensor offset low
    DRIVER_PARA15,                                      
   
}DRIVER_ADDRESS_LIST;


typedef enum {
    CONTROLLER_MODEL = 9100,
    CONTROLLER_SERIAL_NUMBER1,                                     
    CONTROLLER_SERIAL_NUMBER2,                                     
    CONTROLLER_SERIAL_NUMBER3,                                     
    CONTROLLER_SERIAL_NUMBER4,                                     
    CONTROLLER_SERIAL_NUMBER5,                                     
    CONTROLLER_PRODUCT_DATE_1,                                     
    CONTROLLER_PRODUCT_DATE_2,                                     
    CONTROLLER_MAC_ADDRESS_1,                                     
    CONTROLLER_MAC_ADDRESS_2,                                     
    CONTROLLER_MAC_ADDRESS_3,   

} CONTROLLER_PARA_ADDRESS_LIST;







#ifdef __cplusplus
    }
#endif

#endif





