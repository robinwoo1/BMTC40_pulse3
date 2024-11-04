
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TORQUE_H
#define _TORQUE_H

#include "stm32f7xx_hal.h"



/* Define -----------------------------------------------------------------------*/

#define NO_TORQUE_UNIT              7




/* Struct -----------------------------------------------------------------------*/

typedef struct
{        
    float       ps[3];          // Positive Slope
    float       ns[3];          // Negative Slope
    float       base[3];
    uint8_t     sign;           /* direction (forward & reverse)   */        
}   STATE_T;




/* Extern -----------------------------------------------------------------------*/

extern const float  TorqueMetric[NO_TORQUE_UNIT][NO_TORQUE_UNIT];
extern float tqSlopeOffset;
extern float tqOffsetRange;




/* Functions --------------------------------------------------------------------*/

float convertToTorque(uint32_t adc);








#endif 
