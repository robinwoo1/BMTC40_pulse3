
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TIMER_SYSTICK_H
#define _TIMER_SYSTICK_H

#include "stm32f7xx_hal.h"



void SysTick_loop(void);
void Process_10ms(void);
void Calibration_process(void);

void Error_process(void);
void screwCountReset(void);

//void FineDirection(uint32_t ad);

/* Battery */
void temperature_check_battery(void);
void temperature_check_motor(void);
void temperature_check_pcb(void);
void motor_current_check(void);
void battery_voltage_check(void);
void battery_voltage_check_with_run(void);
void calcDisplayBattery(void);
float adcVoltageAverage(float input);

void FastenLoosenControl(void);
void ClearSendData_Init(uint8_t flagResetTorque);

void led_status_control(uint8_t red_led, uint8_t green_led);
void CheckControllerModel(void);
void CheckDriverModel(void);
void get_tq_sensor_value(void);
void DriverLED_Control(void);	// 10ms
void Light_Control(void);
void Save_errorCode(void);
void Lock_Command_process(void);
void Handling_FlagExtOutput(void);
uint8_t driverFilter(uint8_t* buf, uint8_t newSignal, uint8_t oldSignal, uint16_t limitCnt);

void monitoringTimeout(void);

#endif 
