/**
  ******************************************************************************
  * @file           : parameter.c
  * @note           : Parameter manager source
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
/* Includes ------------------------------------------------------------------*/
#include "parameter.h"


#define INPUT_SIZE      21
#define OUTPUT_SIZE     17


/* Private variables ---------------------------------------------------------*/





para Para[PARA_END_VAL_ADDRESS + 1] = {  //val, dft, min, max, value type
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      //   ZERO_DUMMY},                       //o dummy



    /* FASTENING ----------------------------------------------------------------------------- */

    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM1         [1]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE1_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE1_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX1_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX1_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE1
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS1
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS1
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE1_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE1_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED1
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE1
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED1
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START1
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE1
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME1
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED1
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION1
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET1
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT1


    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    21
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      


    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM2         [31]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE2_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE2_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX2_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX2_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE2
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS2
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS2
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE2_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE2_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED2
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE2
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED2
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START2
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE2
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME2
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED2
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION2
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET2
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT2

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},



    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM3         [61]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE3_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE3_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX3_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX3_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE3
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS3
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS3
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE3_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE3_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED3
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE3
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED3
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START3
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE3
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME3
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED3
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION3
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET3
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT3
    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM4         [91]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE4_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE1_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX4_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX4_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE4
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS4
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS4
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE4_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE1_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED4
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE4
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED4
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START4
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE4
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME4
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED4
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION4
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET4
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT4

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM5         [121]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE5_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE5_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX5_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX5_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE5
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS5
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS5
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE5_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE5_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED5
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE5
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED5
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START5
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE5
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME5
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED5
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION5
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET5
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT5

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM6         [151]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE6_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE6_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX6_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX6_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE6
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS6
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS6
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE6_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE6_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED6
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE6
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED6
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START6
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE6
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME6
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED6
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION6
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET6
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT6
    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM7         [181]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE7_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE7_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX7_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX7_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE7
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS7
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS7
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE7_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE7_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED7
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE7
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED7
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START7
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE7
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME7
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED7
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION7
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET7
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT7
    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},

    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM8         [211]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE8_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE8_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX8_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX8_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE8
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS8
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS8
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE8_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE8_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED8
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE8
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED8
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START8
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE8
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME8
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED8
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION8
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET8
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT8

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM9         [241]                     
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE9_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE9_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX9_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX9_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE9
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS9
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS9
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE9_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE9_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED9
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE9
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED9
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START9
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE9
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME9
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED9
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION9
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET9
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT9

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM10             [271]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE10_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE10_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX10_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX10_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE10
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS10
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS10
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE10_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE10_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED10
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE10
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED10
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START10
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE10
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME10
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED10
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION10
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET10
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT10

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM11             [301]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE11_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE11_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX11_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX11_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE11
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS11
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS11
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE11_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE11_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED11
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE11
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED11
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START11
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE11
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME11
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED11
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION11
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET11
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT11

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM12             [331]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE12_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE12_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX12_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX12_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE12
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS12
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS12
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE12_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE12_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED12
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE12
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED12
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START12
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE12
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME12
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED12
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION12
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET12
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT12

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM13             [361]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE13_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE13_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX13_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX13_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE13
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS13
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS13
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE13_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE13_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED13
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE13
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED13
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START13
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE13
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME13
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED13
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION13
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET13
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT13

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM14             [391]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE14_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE14_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX14_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX14_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE14
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS14
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS14
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE14_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE14_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED14
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE14
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED14
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START14
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE14
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME14
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED14
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION14
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET14
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT14

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM15             [421]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE15_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE15_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX15_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX15_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE15
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS15
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS15
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE15_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE15_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED15
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE15
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED15
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START15
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE15
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME15
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED15
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION15
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET15
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT15

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM16             [451]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE16_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE16_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX16_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX16_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE16
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS16
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS16
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE16_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE16_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED16
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE16
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED16
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START16
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE16
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME16
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED16
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION16
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET16
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT16

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM17             [481]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE17_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE17_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX17_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX17_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE17
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS17
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS17
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE17_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE17_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED17
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE17
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED17
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START17
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE17
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME17
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED17
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION17
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET17
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT17

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM18             [511]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE18_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE18_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX18_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX18_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE18
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS18
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS18
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE18_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE18_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED18
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE18
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED18
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START18
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE18
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME18
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED18
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION18
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET18
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT18

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM19             [541]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE19_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE19_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX19_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX19_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE19
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS19
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS19
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE19_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE19_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED19
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE19
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED19
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START19
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE19
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME19
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED19
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION19
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET19
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT19

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM20             [571]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE20_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE20_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX20_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX20_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE20
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS20
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS20
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE20_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE20_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED20
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE20
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED20
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START20
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE20
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME20
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED20
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION20
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET20
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT20

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM21             [601]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE21_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE21_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX21_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX21_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE21
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS21
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS21
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE21_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE21_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED21
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE21
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED21
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START21
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE21
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME21
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED21
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION21
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET21
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT21

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM22             [631]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE22_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE22_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX22_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX22_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE22
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS22
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS22
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE22_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE22_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED22
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE22
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED22
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START22
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE22
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME22
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED22
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION22
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET22
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT22

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM23             [661]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE23_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE23_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX23_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX23_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE23
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS23
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS23
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE23_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE23_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED23
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE23
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED23
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START23
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE23
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME23
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED23
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION23
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET23
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT23

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM24             [691]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE24_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE24_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX24_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX24_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE24
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS24
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS24
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE24_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE24_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED24
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE24
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED24
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START24
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE24
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME24
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED24
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION24
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET24
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT24

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM25             [721]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE25_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE25_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX25_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX25_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE25
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS25
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS25
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE25_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE25_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED25
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE25
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED25
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START25
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE25
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME25
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED25
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION25
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET25
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT25

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM26             [751]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE26_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE26_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX26_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX26_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE26
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS26
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS26
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE26_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE26_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED26
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE26
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED26
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START26
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE26
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME26
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED26
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION26
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET26
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT26

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM27             [781]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE27_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE27_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX27_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX27_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE27
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS27
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS27
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE27_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE27_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED27
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE27
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED27
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START27
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE27
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME27
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED27
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION27
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET27
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT27

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM28             [811]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE28_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE28_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX28_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX28_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE28
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS28
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS28
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE28_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE28_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED28
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE28
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED28
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START28
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE28
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME28
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED28
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION28
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET28
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT28

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM29             [841]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE29_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE29_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX29_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX29_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE29
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS29
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS29
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE29_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE29_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED29
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE29
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED29
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START29
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE29
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME29
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED29
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION29
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET29
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT29

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM30             [871]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE30_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE30_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX30_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX30_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE30
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS30
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS30
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE30_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE30_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED30
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE30
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED30
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START30
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE30
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME30
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED30
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION30
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET30
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT30

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},


    
    {0,     0,      0,      1,        VAL_UINT16},                      //   TCAM_ACTM31             [901]                    
    {50,    50,     30,     500,      VAL_FLOAT_H},                     //   FASTEN_TORQUE31_H
    {50,    50,     30,     500,      VAL_FLOAT_L},                     //   FASTEN_TORQUE31_L
    {10,    10,     0,      100,      VAL_FLOAT_H},                     //   TORQUE_MIN_MAX31_H
    {10,    10,     0,      100,      VAL_FLOAT_L},                     //   TORQUE_MIN_MAX31_L
    {0,     0,      0,      20000,    VAL_UINT16},                      //   TARGET_ANGLE31
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MIN_TURNS31
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FASTEN_MAX_TURNS31
    {0,     0,      0,      100,      VAL_FLOAT_H},                     //   SNUG_TORQUE31_H
    {0,     0,      0,      100,      VAL_FLOAT_L},                     //   SNUG_TORQUE31_L
    {300,   300,    100,    2000,     VAL_UINT16},                      //   FASTEN_SPEED31
    {0,     0,      0,      20000,    VAL_UINT16},                      //   FREE_FASTEN_ANGLE31
    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_FASTEN_SPEED31
    {0,     0,      0,      300,      VAL_UINT16},                      //   SOFT_START31
    {40,    40,     10,     95,       VAL_UINT16},                      //   FASTEN_SEATTING_POINT_RATE31
    {50,    50,     50,     200,      VAL_UINT16},                      //   FASTEN_TQ_RISING_TIME31
    {100,   100,    20,     2000,     VAL_UINT16},                      //   RAMP_UP_SPEED31
    {100,   100,    70,     130,      VAL_UINT16},                      //   TORQUE_COMPENSATION31
    {100,   100,    50,     150,      VAL_UINT16},                      //   TORQUE_OFFSET31
    {20,    20,     0,      200,      VAL_UINT16},                      //   MAX_FASTEN_PULSE_COUNT31


    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    921
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    929
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    939
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    949
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    959
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    969
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    979
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    989
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // DUMMY    999

    





    /*ADVANCED --------------------------------------------------------------------------------------------*/

    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1		        1000  
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H	(MAX: 200)
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 		          



    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (2-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (2-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (2-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (2-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (2-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (2-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (2-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (2-1) 		        



    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (3-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (3-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (3-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (3-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (3-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (3-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (3-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (3-1) 		        
    



    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (4-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (4-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (4-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (4-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (4-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (4-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (4-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (4-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (5-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (5-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (5-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (5-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (5-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (5-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (5-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (5-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (6-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (6-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (6-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (6-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (6-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (6-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (6-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (6-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (7-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (7-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (7-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (7-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (7-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (7-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (7-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (7-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (8-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (8-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (8-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (8-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (8-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (8-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (8-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (8-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (9-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (9-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (9-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (9-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (9-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (9-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (9-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (9-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (10-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (10-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (10-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (10-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (10-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (10-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (10-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (10-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (11-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (11-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (11-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (11-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (11-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (11-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (11-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (11-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (12-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (12-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (12-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (12-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (12-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (12-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (12-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (12-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (13-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (13-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (13-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (13-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (13-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (13-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (13-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (13-1) 		        
        
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (14-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (14-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (14-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (14-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (14-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (14-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (14-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (14-1) 		        


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (15-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (15-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (15-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (15-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (15-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (15-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (15-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (15-1) 
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (16-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (16-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (16-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (16-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (16-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (16-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (16-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (16-1) 		        


    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (17-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (17-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (17-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (17-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (17-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (17-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (17-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (17-1) 		        

    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (18-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (18-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (18-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (18-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (18-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (18-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (18-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (18-1) 		        

    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (19-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (19-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (19-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (19-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (19-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (19-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (19-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (19-1) 		        


    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (20-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (20-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (20-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (20-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (20-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (20-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (20-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (20-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (21-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (21-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (21-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (21-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (21-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (21-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (21-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (21-1) 		        

    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (22-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (22-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (22-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (22-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (22-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (22-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (22-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (22-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (23-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (23-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (23-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (23-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (23-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (23-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (23-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (23-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (24-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (24-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (24-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (24-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (24-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (24-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (24-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (24-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (25-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (25-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (25-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (25-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (25-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (25-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (25-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (25-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (26-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (26-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (26-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (26-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (26-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (26-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (26-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (26-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (27-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (27-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (27-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (27-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (27-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (27-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (27-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (27-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (28-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (28-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (28-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (28-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (28-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (28-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (28-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (28-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (29-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (29-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (29-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (29-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (29-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (29-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (29-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (29-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (30-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (30-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (30-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (30-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (30-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (30-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (30-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (30-1) 		        
    
    


    {0,     0,      0,      10,       VAL_UINT16},                      //   ADVANCED_MODE1 + PARA_ADVANCED_SIZE * (31-1)		          
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_1_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_1_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_2_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_2_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_3_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_3_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_4_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_4_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_5_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_5_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_6_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_6_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_7_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_7_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_8_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_8_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_9_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_9_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_10_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_10_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_11_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_11_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_12_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_12_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_13_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_13_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_14_H + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_14_L + PARA_ADVANCED_SIZE * (31-1)	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_15_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_15_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_16_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_16_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_17_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_17_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_18_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_18_L	              
    {0,     0,      0,      0x477F,   VAL_FLOAT_H},                     //   ADVANCED_PARA1_19_H	              
    {0,     0,      0,      0xFF00,   VAL_FLOAT_L},                     //   ADVANCED_PARA1_19_L	              

    {0,     0,      0,      1000,     VAL_UINT16},                      //   FREE_REVERSE_ROTATION_SPEED1 + PARA_ADVANCED_SIZE * (31-1)	            
    {0,     0,      0,      0x4348,   VAL_FLOAT_H},                     //   FREE_REVERSE_ROTATION_ANGLE1_H + PARA_ADVANCED_SIZE * (31-1)		            
    {0,     0,      0,      0,        VAL_FLOAT_L},                     //   FREE_REVERSE_ROTATION_ANGLE1_L + PARA_ADVANCED_SIZE * (31-1)		            
    {0,     0,      0,      1000,     VAL_UINT16},                      //   REVERS_ANGLE_SETTING_SPEED1 + PARA_ADVANCED_SIZE * (31-1)	            
    {0,     0,      0,      30000,    VAL_UINT16},                      //   REVERS_ANGLE_SETTING_ANGLE1 + PARA_ADVANCED_SIZE * (31-1)   	          
    {0,     0,      0,      1,        VAL_UINT16},                      //   REVERS_ANGLE_SETTING_FW_REV1 + PARA_ADVANCED_SIZE * (31-1) 		        




    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2395
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2415
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2435
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2455
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2475
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2495
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2515
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2535
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2555
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2575
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2595
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2615
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2635
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2655
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2675
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2695
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2715
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2735
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2755
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2775
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2795
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2800
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2815
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2835
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2855
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2875
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2895
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           



    /* Fieldbus - ANYBUS ---------------------------------------------------------------------------------------------------- */

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // ANYBUS_ENABLE  2900
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // ANYBUS_MODULE_TYPE
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {192,   192,    0,      0xFF,     VAL_UINT16},                      // ANYBUS_IP_ADDRESS_1  2906
    {168,   168,    0,      0xFF,     VAL_UINT16},           
    {1,     1,      0,      0xFF,     VAL_UINT16},           
    {101,   101,    0,      0xFF,     VAL_UINT16},           
    {225,   225,    0,      0xFF,     VAL_UINT16},                      // ANYBUS_NETMASK_1  2910
    {225,   225,    0,      0xFF,     VAL_UINT16},           
    {225,   225,    0,      0xFF,     VAL_UINT16},           
    {0,     0,      0,      0xFF,     VAL_UINT16},           
    {192,   192,    0,      0xFF,     VAL_UINT16},                      // ANYBUS_GATEWAY_1  2914
    {168,   168,    0,      0xFF,     VAL_UINT16},                      
    {1,     1,      0,      0xFF,     VAL_UINT16},           
    {1,     1,      0,      0xFF,     VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2918
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      


    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // ANYBUS_IO_INPUT_1  2926
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                     
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2934
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                     

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // ANYBUS_IO_OUTPUT_1  2942
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2950
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                     

    {1,     1,      0,      0xFFFF,   VAL_UINT16},                      // ANYBUS_MONITORING_1  2958
    {2,     2,      0,      0xFFFF,   VAL_UINT16},           
    {3,     3,      0,      0xFFFF,   VAL_UINT16},                      // 2960
    {4,     4,      0,      0xFFFF,   VAL_UINT16},           
    {5,     5,      0,      0xFFFF,   VAL_UINT16},           
    {6,     6,      0,      0xFFFF,   VAL_UINT16},           
    {7,     7,      0,      0xFFFF,   VAL_UINT16},           
    {8,     8,      0,      0xFFFF,   VAL_UINT16},           
    {9,     9,      0,      0xFFFF,   VAL_UINT16},           
    {10,    10,     0,      0xFFFF,   VAL_UINT16},                      
    {11,    11,     0,      0xFFFF,   VAL_UINT16},           
    {12,    12,     0,      0xFFFF,   VAL_UINT16},           
    {13,    13,     0,      0xFFFF,   VAL_UINT16},           
    {14,    14,     0,      0xFFFF,   VAL_UINT16},           
    {15,    15,     0,      0xFFFF,   VAL_UINT16},           
    {16,    16,     0,      0xFFFF,   VAL_UINT16},           
    {17,    17,     0,      0xFFFF,   VAL_UINT16},           
    {18,    18,     0,      0xFFFF,   VAL_UINT16},           
    {19,    19,     0,      0xFFFF,   VAL_UINT16},           
    {20,    20,     0,      0xFFFF,   VAL_UINT16},                      // 2977
    {21,    21,     0,      0xFFFF,   VAL_UINT16},           

    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // ANYBUS_MONITORING_30  2987   

    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2988
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},                      // 2997
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    {0,     0,      0,      0xFFFF,   VAL_UINT16},           
    


    

    /* MODEL ---------------------------------------------------------------------------------------------------- */

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_1        3000
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL1_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3020
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_1        3030
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL2_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3050
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_1        3060
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL3_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3080
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_1        3090
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL4_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3110
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_1        3120
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL5_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3140
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_1        3150
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL6_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3170
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_1        3180
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL7_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3200
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_1        3210
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL8_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3230
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_1        3240
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL9_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3260
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_1        3270
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL10_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3290
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_1        3300
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL11_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3320
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_1        3330
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL12_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3350
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_1        3360
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL13_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3380
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_1        3390
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL14_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3410
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},

    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_1        3420
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_2
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_3
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_4
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_5
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_6
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_7
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_8
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_9
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_10
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_11
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_12
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_13
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_14
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_15
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_16
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_17
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_18
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_19
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      //  MODEL15_20

    
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3440
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3450
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3460
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3470
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3480
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},                      // 3490
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},           
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},
    {0,     0,      0,   0xFFFF,      VAL_UINT16},





    /* MULTI SEQUENCE ----------------------------------------------------------------------------- */

    {1001,  1001,   0,    32767,    VAL_UINT16},                        //  MS_PG1        3500
    {3200,  3200,   0,    32767,    VAL_UINT16},                        //  MS_PG2
    {5030,  5030,   0,    32767,    VAL_UINT16},                        //  MS_PG3
    {3200,  3200,   0,    32767,    VAL_UINT16},                        //  MS_PG4
    {6001,  6001,   0,    32767,    VAL_UINT16},                        //  MS_PG5
    {2000,  2000,   0,    32767,    VAL_UINT16},                        //  MS_PG6
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG7
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG8
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG9
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG10

    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3510
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           

    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG11         3520
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG12
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG13
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG14
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG15
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG16
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG17
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG18
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG19
    {0,     0,      0,    32767,    VAL_UINT16},                        //  MS_PG20

    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3530
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3540
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           






    /* SCREW COUNT ----------------------------------------------------------------------------- */

    {0,     0,      0,    3,        VAL_UINT16},                        // COUNT_MODE		                     3550
    {0,     0,      0,    0x4422,   VAL_FLOAT_H},                       // COUNT_LIMIT_TIME_H   (650.0)
    {0,     0,      0,    0x8000,   VAL_FLOAT_L},                       // COUNT_LIMIT_TIME_L
    {0,     0,      0,    3,        VAL_UINT16},                        // COUNT_OUTPUT_SIGNAL_TYPE		              
    {0,     0,      0,    99,       VAL_UINT16},                        // COUNT_MIDDLE_CNT_NUMBER		              
    {0,     0,      0,    100,      VAL_UINT16},                        // COUNT_START_DELAY_TIME		              
    {2,     2,      0,    255,      VAL_UINT16},                        // COUNT_SCREW	      
    {0,     0,      0,    100,      VAL_UINT16},                        // SCREW_CANCEL_LOOSENING	      



    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3558
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3560
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},            
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3570
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},            
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3580
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},            
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3590
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},            






    /* I/O ------------------------------------------------------------------------------------- */

    {1,     1,      0,    INPUT_SIZE, VAL_UINT16},                      // INPUT_MAPPING1        3600
    {2,     2,      0,    INPUT_SIZE, VAL_UINT16},                      // INPUT_MAPPING2	
    {3,     3,      0,    INPUT_SIZE, VAL_UINT16},                      // INPUT_MAPPING3	
    {10,    10,     0,    INPUT_SIZE, VAL_UINT16},                      // INPUT_MAPPING4	
    {11,    11,     0,    INPUT_SIZE, VAL_UINT16},                      // INPUT_MAPPING5	
    {12,    12,     0,    INPUT_SIZE, VAL_UINT16},                      // INPUT_MAPPING6	
    {13,    13,     0,    INPUT_SIZE, VAL_UINT16},                      // INPUT_MAPPING7	
    {14,    14,     0,    INPUT_SIZE, VAL_UINT16},                      // INPUT_MAPPING8	


    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX            3608
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // DUMMY_INDEX



    {1,     1,      0,    OUTPUT_SIZE, VAL_UINT16},                      // OUTPUT_MAPPING1	    3620
    {2,     2,      0,    OUTPUT_SIZE, VAL_UINT16},                      // OUTPUT_MAPPING2
    {3,     3,      0,    OUTPUT_SIZE, VAL_UINT16},                      // OUTPUT_MAPPING3
    {4,     4,      0,    OUTPUT_SIZE, VAL_UINT16},                      // OUTPUT_MAPPING4
    {6,     6,      0,    OUTPUT_SIZE, VAL_UINT16},                      // OUTPUT_MAPPING5
    {7,     7,      0,    OUTPUT_SIZE, VAL_UINT16},                      // OUTPUT_MAPPING6
    {8,     8,      0,    OUTPUT_SIZE, VAL_UINT16},                      // OUTPUT_MAPPING7
    {10,    10,     0,    OUTPUT_SIZE, VAL_UINT16},                      // OUTPUT_MAPPING8



    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3628
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3638
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},           
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3648
    {0,     0,      0,    0xFFFF,     VAL_UINT16},                      // 3649

 




    /* CONTROLLER ------------------------------------------------------------------------------------ */

    {1,     1,      1,    99,       VAL_UINT16},                        // DRIVER_ID                                  3650
    {0,     0,      1,    99,       VAL_UINT16},                        // DRIVER_MODEL


    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3652
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3656


    {2,     2,      0,    6,        VAL_UINT16},                        // UNIT
    {0,     0,      0,    0xFFFF,   VAL_UINT32_H},                      // SCREW_TYPE_H
    {0,     0,      0,    0xFFFF,   VAL_UINT32_L},                      // SCREW_TYPE_L
    {100,   100,    90,   110,      VAL_UINT16},                        // TORQUE_COMPENSATION_MAIN
    {100,   100,    10,   1000,     VAL_UINT16},                        // ACC_DEA_TIME
    {0,     0,      0,    20,       VAL_UINT16},                        // FASTEN_TORQUE_MAINTAIN_TIME                MD, BM에서는 필수 파라미터
    {500,   500,    100,  1000,     VAL_UINT16},                        // LOOSENING_SPEED
    {0,     0,      0,    0x40A0,   VAL_FLOAT_H},                       // JUDGE_FASTEN_MIN_TURNS_H                   1611 (D: 0,  MIN: 0, MAX: 5.0)
    {0,     0,      0,    0,        VAL_FLOAT_L},                       // JUDGE_FASTEN_MIN_TURNS_L
    {0,     0,      0,     1,       VAL_FLOAT_H},                       // FREE_SPEED_MAX_TORQUE_H                    1612 (설정 필요)
    {0,     0,      0,     1,       VAL_FLOAT_L},                       // FREE_SPEED_MAX_TORQUE_L
    {0x4120,0x4120, 0,    0x4270,   VAL_FLOAT_H},                       // TOTAL_FASTENING_TIME_H                     1614 (D: 10.0,  MIN: 0, MAX: 60.0)
    {0,     0,      0,    0,        VAL_FLOAT_L},                       // TOTAL_FASTENING_TIME_L
    {0x4120,0x4120, 0,    0x4270,   VAL_FLOAT_H},                       // TOTAL_LOOSENING_TIME_H                     1616 (D: 10.0,  MIN: 0, MAX: 60.0)
    {0,     0,      0,    0,        VAL_FLOAT_L},                       // TOTAL_LOOSENING_TIME_L
    {0x3E4C,0x3E4C, 0x3DCC,0x3F00,  VAL_FLOAT_H},                       // INIT_LOOSENING_TIME_LIMIT_H                1618 (D: 0.2,  MIN: 0.1, MAX: 0.5)
    {0xCCCD,0xCCCD, 0xCCCD,0xFFFF,  VAL_FLOAT_L},                       // INIT_LOOSENING_TIME_LIMIT_L
    {0,     0,      0,    360,      VAL_UINT16},                        // HOLDING_TIME_ANGLE_LIMIT
    {1,     1,      0,    1,        VAL_UINT16},                        // AUTO_SELECT_SPEED
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // USE_MAXTQ_FOR_LOOSENING


    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3677
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3681


    {0x3FF, 0x3FF,  0,    0x3FF,    VAL_UINT16},                        // SELECT_DATA_TO_SAVE                          EVENT_DATA_SELECT
    {200,   200,    0,    500,      VAL_UINT16},                        // FASTENING_COMPLETE_SIGNAL_OUT_TIME
    {0,     0,      0,    1,        VAL_UINT16},                        // FASTENING_STOP_ALARM
    {0,     0,      0,    1,        VAL_UINT16},                        // AUTO_LOCK                                    AUTO_LOCK_MODEL
    {0,     0,      0,    1,        VAL_UINT16},                        // START_TRIGGER_ENABLE
    {0,     0,      0,    1,        VAL_UINT16},                        // REVERSE_SWITCH_START_ENABLE
    {0,     0,      0,    2,        VAL_UINT16},                        // REVERSE_LOCK


    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3689
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3693


    {1,     1,      0,    33,       VAL_UINT16},                        // FASTEN_NO_SAVE
    {1,     1,      0,    1,        VAL_UINT16},                        // LCD_TOUCH_LOCK
    {0,     0,      0,    1,        VAL_UINT16},                        // MODEL_SELECT
    {0,     0,      0,    1,        VAL_UINT16},                        // SELECT_BY_TOUCH_MODEL_PRESET
    {0,     0,      0,    1,        VAL_UINT16},                        // PRESET_CHANGE_BY_BARCODE    START_BY_BARCODE_MODEL
    {0,     1,      0,    1,        VAL_UINT16},                        // MODEL_AUTO_RESTART


    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3700
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3704


    {0,     0,      0,    9999,     VAL_UINT16},                        // PASSWORD
    {1,     1,      0,    1,        VAL_UINT16},                        // SOUND_SETTING
    {0x4000,0x4000, 0,    0x40C0,   VAL_FLOAT_H},                       // ERROR_CLEAR_TIME_H                         1613 (D: 2.0,  MIN: 0, MAX: 6.0)
    {0,     0,      0,    0,        VAL_FLOAT_L},                       // ERROR_CLEAR_TIME_L
    {10,    10,     0,    60,       VAL_UINT16},                        // LAMP_ON_TIME


    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3710
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3714


    {0,     0,      0,    1,        VAL_UINT16},                        // PROTOCOL
    {0,     0,      0,    1,        VAL_UINT16},                        // SELECT_RS232
    {1,     1,      0,    5,        VAL_UINT16},                        // SELECT_BAUDRATE
    {0,     0,      0,    1,        VAL_UINT16},                        // AUTO_UPDATE_EVENT
    {0,     0,      0,    1,        VAL_UINT16},                        // AUTO_UPDATE_PORT


    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3720
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3724


    {0,     0,      0,    1,        VAL_UINT16},                        // CROWFOOT_ENABLE
    {0x4120,0x4120, 0,    0x4422,   VAL_FLOAT_H},                       // CROWFOOT_RATIO_H                           1640 (D: 10.00,  MIN: 0, MAX: 650.00)
    {0,     0,      0,    0x8000,   VAL_FLOAT_L},                       // CROWFOOT_RATIO_L
    {100,   100,    0,    300,      VAL_UINT16},                        // CROWFOOT_EFFICIENCY
    {0,     0,      0,    500,      VAL_FLOAT_H},                       // CROWFOOT_REVERSE_TORQUE_H
    {0,     0,      0,    500,      VAL_FLOAT_L},                       // CROWFOOT_REVERSE_TORQUE_L
    {50,    50,     0,    100,      VAL_UINT16},                        // CROWFOOT_REVERSE_SPEED


    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3732
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 
    {0,     0,      0,    0xFFFF,   VAL_UINT16},                        // 3736


    {0,     0,      0,     30,      VAL_UINT16},                        // SLEEP_IN_TIME
    {0,     0,      0,     3,       VAL_UINT16},                        // LCD_KEY_LOCK
    {0,     0,      0,     10000,   VAL_UINT16},                        // TRIGGER_START_DELAY_TIME
    {0,     0,      0,     0xFFFF,  VAL_UINT32_H},                      // DISPLAY_PRESET_NUM_H
    {0,     0,      0,     0xFFFF,  VAL_UINT32_L},                      // DISPLAY_PRESET_NUM_L
    {0,     0,      0,     60,      VAL_UINT16},                        // LOCK_WIFI_DICONNECT_TIME
    {0,     0,      0,     1,       VAL_UINT16},                        // LF_SWITCH_TYPE


    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3744
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3754
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3764
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3774
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3784
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3794
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    



    /* NETWORK ------------------------------------------------------------------------------------- */

    {0,     0,      0,      1,      VAL_UINT16},                        // WIFI_SELECT     3800
    {0,     0,      0,      1,      VAL_UINT16},                        // NETWORK_MODE
    {0,     0,      0,      1,      VAL_UINT16},                        // STATIC_DHCP


    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3803
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3809       


    {192,   192,    0,      255,    VAL_UINT16},                        // IP_ADDRESS1      3810
    {168,   168,    0,      255,    VAL_UINT16},                        // IP_ADDRESS2
    {1,     1,      0,      255,    VAL_UINT16},                        // IP_ADDRESS3
    {100,   100,    0,      255,    VAL_UINT16},                        // IP_ADDRESS4
    {255,   255,    0,      255,    VAL_UINT16},                        // SUBNETMASK1
    {255,   255,    0,      255,    VAL_UINT16},                        // SUBNETMASK2
    {255,   255,    0,      255,    VAL_UINT16},                        // SUBNETMASK3
    {0,     0,      0,      255,    VAL_UINT16},                        // SUBNETMASK4
    {192,   192,    0,      255,    VAL_UINT16},                        // GATEWAY1
    {168,   168,    0,      255,    VAL_UINT16},                        // GATEWAY2
    {1,     1,      0,      255,    VAL_UINT16},                        // GATEWAY3
    {1,     1,      0,      255,    VAL_UINT16},                        // GATEWAY4
    {5000,  5000,   0,      0xFFF0, VAL_UINT16},                        // ETHERNET_PORT

    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3823
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3829
    

    {0,     0,      0,      255,     VAL_UINT16},                       // TARGET_IP1       3830
    {0,     0,      0,      255,     VAL_UINT16},                       // TARGET_IP2
    {0,     0,      0,      255,     VAL_UINT16},                       // TARGET_IP3
    {0,     0,      0,      255,     VAL_UINT16},                       // TARGET_IP4
    {0,     0,      0,      0xFFF0,  VAL_UINT16},                       // TARGET_PORT


    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3835
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},            
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3839
    


    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR1    3840
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR2
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR3
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR4
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR5
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR6
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR7
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR8
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR9
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR10
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR11
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR12
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR13
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR14
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR15
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_SSID_CHAR16

    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR1     3856
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR2
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR3
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR4
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR5
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR6
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR7
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR8
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR9
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR10
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR11
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR12
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR13
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR14
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR15
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR16
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR17
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR18
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR19
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR20
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR21
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR22
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR23
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR24
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR25
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR26
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR27
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR28
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR29
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR30
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR31
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // AP_PW_CHAR32
    {0,     0,      0,      3,      VAL_UINT16},                        // AP_COUNTRY


    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3889
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       
    {0,     0,      0,      0xFFFF,  VAL_UINT16},                       // 3899



    {0,     0,      0,      1,      VAL_UINT16},                        // WIFI_EAP_ENABLE      3900
    {0,     0,      0,      4,      VAL_UINT16},                        // WIFI_EAP_METHOD      3901

    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_1    3902
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_2
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_3
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_4
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_5
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_6
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_7
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_8
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_9
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_10
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_11
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_12
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_13
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_14
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_15
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_ID_16

    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_1    3918
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_2
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_3
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_4
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_5
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_6
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_7
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_8
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_9
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_10
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_11
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_12
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_13
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_14
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_15
    {0,     0,      0,      0xFFFF, VAL_UINT16},                        // WIFI_EAP_USER_PASSWORD_16    3933


    /* ETC ------------------------------------------------------------------------------------- */                                   

    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    // 3934
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    
    
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    // 3944
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    
    
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    // 3954
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    
    
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    // 3964
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    
    
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    // 3974
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    
    
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    // 3984
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    

    {0,     0,      0,      0xFFFF,     VAL_UINT16},                    // 3994
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16},           
    {0,     0,      0,      0xFFFF,     VAL_UINT16}                     // 3999

};




/* Public functions -------------------------------------------------------------------------------------*/

/**
 * @brief Get 파라미터의 Value 값
 * 
 * @param address   : 파라미터 주소
 * @return float    : 리턴 값
 */
float getParaVal(uint16_t address)
{
    floatFrom16 convert;
    float retval = 0;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        // Convert
        switch (Para[address].type)
        {
            case VAL_UINT16:
                retval = (float)Para[address].val;
            break;

            case VAL_UINT32_H:
                retval = (Para[address].val << 16) + Para[address + 1].val;
            break;

            case VAL_UINT32_L:
                if(address > 0)
                {
                    retval = (Para[address - 1].val << 16) + Para[address].val;
                }
            break;

            case VAL_FLOAT_H:
            	convert.raw.integer_high = Para[address].val;
                convert.raw.integer_low = Para[address + 1].val;

                retval = convert.fVal;
            break;

            case VAL_FLOAT_L:
                if(address > 0)
                {
                    convert.raw.integer_high = Para[address - 1].val;
                    convert.raw.integer_low = Para[address].val;

                    retval = convert.fVal;
                }
            break;
        }
    }

    return retval;
}


/**
 * @brief Get 파라미터의 MIN 값
 * 
 * @param address   : 파라미터 주소
 * @return float    : 리턴 값
 */
float getParaMin(uint16_t address)
{
    floatFrom16 convert;
    float retval = 0;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        // Convert
        switch (Para[address].type)
        {
            case VAL_UINT16:
                retval = (float)Para[address].min;
            break;

            case VAL_UINT32_H:
                retval = (Para[address].min << 16) + Para[address + 1].min;
            break;

            case VAL_UINT32_L:
                if(address > 0)
                {
                    retval = (Para[address - 1].min << 16) + Para[address].min;
                }
            break;

            case VAL_FLOAT_H:
                convert.raw.integer_high = Para[address].min;
                convert.raw.integer_low = Para[address + 1].min;

                retval = convert.fVal;
            break;

            case VAL_FLOAT_L:
                if(address > 0)
                {
                    convert.raw.integer_high = Para[address - 1].min;
                    convert.raw.integer_low = Para[address].min;

                    retval = convert.fVal;
                }
            break;
        }
    }

    return retval;
}


/**
 * @brief Get 파라미터의 MAX 값
 * 
 * @param address   : 파라미터 주소
 * @return float    : 리턴 값
 */
float getParaMax(uint16_t address)
{
    floatFrom16 convert;
    float retval = 0;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        // Convert
        switch (Para[address].type)
        {
            case VAL_UINT16:
                retval = (float)Para[address].max;
            break;

            case VAL_UINT32_H:
                retval = (Para[address].max << 16) + Para[address + 1].max;
            break;

            case VAL_UINT32_L:
                if(address > 0)
                {
                    retval = (Para[address - 1].max << 16) + Para[address].max;
                }
            break;

            case VAL_FLOAT_H:
                convert.raw.integer_high = Para[address].max;
                convert.raw.integer_low = Para[address + 1].max;

                retval = convert.fVal;
            break;

            case VAL_FLOAT_L:
                if(address > 0)
                {
                    convert.raw.integer_high = Para[address - 1].max;
                    convert.raw.integer_low = Para[address].max;

                    retval = convert.fVal;
                }
            break;
        }
    }

    return retval;
}


/**
 * @brief Get 파라미터의 Default 값
 * 
 * @param address   : 파라미터 주소
 * @return float    : 리턴 값
 */
float getParaDef(uint16_t address)
{
    floatFrom16 convert;
    float retval = 0;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        // Convert

        switch (Para[address].type)
        {
            case VAL_UINT16:
                retval = (float)Para[address].dft;
            break;

            case VAL_UINT32_H:
                retval = (Para[address].dft << 16) + Para[address + 1].dft;
            break;

            case VAL_UINT32_L:
                if(address > 0)
                {
                    retval = (Para[address - 1].dft << 16) + Para[address].dft;
                }
            break;

            case VAL_FLOAT_H:
                convert.raw.integer_high = Para[address].dft;
                convert.raw.integer_low = Para[address + 1].dft;

                retval = convert.fVal;
            break;

            case VAL_FLOAT_L:
                if(address > 0)
                {
                    convert.raw.integer_high = Para[address - 1].dft;
                    convert.raw.integer_low = Para[address].dft;

                    retval = convert.fVal;
                }
            break;
        }
    }

    return retval;
}


/**
 * @brief Get the Para Value 16bit
 * 
 * @param address   : 파라미터 주소
 * @return uint16_t : 파라미터 값
 */
uint16_t getParaValSingle(uint16_t address)
{
    uint16_t retval = 0;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        retval = Para[address].val;
    }

    return retval;
}


/**
 * @brief Get the Para Min value 16bit
 * 
 * @param address   : 파라미터 주소
 * @return uint16_t : 파라미터 최소값
 */
uint16_t getParaMaxSingle(uint16_t address)
{
    uint16_t retval = 0;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        retval = Para[address].max;
    }

    return retval;
}


/**
 * @brief Get the Para Max value 16bit
 * 
 * @param address   : 파라미터 주소
 * @return uint16_t : 파라미터 최대값
 */
uint16_t getParaMinSingle(uint16_t address)
{
    uint16_t retval = 0;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        retval = Para[address].min;
    }

    return retval;
}


/**
 * @brief Get the Para default value 16bit
 * 
 * @param address   : 파라미터 주소
 * @return uint16_t : 파라미터 기본값
 */
uint16_t getParaDefSingle(uint16_t address)
{
    uint16_t retval = 0;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        retval = Para[address].dft;
    }

    return retval;
}


/**
 * @brief Set 파라미터의 Value 값
 * 
 * @param address   : 파라미터 주소
 * @param value     : 저장하고자 하는 값
 */
void setParaVal(uint16_t address, float value)
{
    floatFrom16 convert;
    uint32_t type32;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        // Save
        switch (Para[address].type)
        {
            case VAL_UINT16:
                Para[address].val = (uint16_t)value;
            break;

            case VAL_UINT32_H:
                type32 = (uint32_t)value;
                Para[address].val = (uint16_t)(type32 >> 16);
                Para[address + 1].val = (uint16_t)(type32 & 0xFFFF) ;
            break;

            case VAL_FLOAT_H:
                convert.fVal = value;
                Para[address].val = convert.raw.integer_high;
                Para[address + 1].val = convert.raw.integer_low;
            break;
            
            default:
                break;
        }
    }
}


/**
 * @brief Set 파라미터의 min Value 값
 * 
 * @param address   : 파라미터 주소
 * @param value     : 저장하고자 하는 값
 */
void setParaMin(uint16_t address, float value)
{
    floatFrom16 convert;
    uint32_t type32;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        // Save
        switch (Para[address].type)
        {
            case VAL_UINT16:
                Para[address].min = (uint16_t)value;
            break;

            case VAL_UINT32_H:
                type32 = (uint32_t)value;
                Para[address].min = (uint16_t)(type32 >> 16);
                Para[address + 1].min = (uint16_t)(type32 & 0xFFFF);
            break;

            case VAL_FLOAT_H:
                convert.fVal = value;
                Para[address].min = convert.raw.integer_high;
                Para[address + 1].min = convert.raw.integer_low;
            break;
            
            default:
                break;
        }
    }
}


/**
 * @brief Set 파라미터의 max value 값
 * 
 * @param address   : 파라미터 주소
 * @param value     : 저장하고자 하는 값
 */
void setParaMax(uint16_t address, float value)
{
    floatFrom16 convert;
    uint32_t type32;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        // Save
        switch (Para[address].type)
        {
            case VAL_UINT16:
                Para[address].max = (uint16_t)value;
            break;

            case VAL_UINT32_H:
                type32 = (uint32_t)value;
                Para[address].max = (uint16_t)(type32 >> 16);
                Para[address + 1].max = (uint16_t)(type32 & 0xFFFF) ;
            break;

            case VAL_FLOAT_H:
                convert.fVal = value;
                Para[address].max = convert.raw.integer_high;
                Para[address + 1].max = convert.raw.integer_low;
            break;
            
            default:
                break;
        }
    }
}


/**
 * @brief Set 파라미터의 default value 값
 * 
 * @param address   : 파라미터 주소
 * @param value     : 저장하고자 하는 값
 */
void setParaDef(uint16_t address, float value)
{
    floatFrom16 convert;
    uint32_t type32;

    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        // Save
        switch (Para[address].type)
        {
            case VAL_UINT16:
                Para[address].dft = (uint16_t)value;
            break;

            case VAL_UINT32_H:
                type32 = (uint32_t)value;
                Para[address].dft = (uint16_t)(type32 >> 16);
                Para[address + 1].dft = (uint16_t)(type32 & 0xFFFF) ;
            break;

            case VAL_FLOAT_H:
                convert.fVal = value;
                Para[address].dft = convert.raw.integer_high;
                Para[address + 1].dft = convert.raw.integer_low;
            break;
            
            default:
                break;
        }
    }
}


/**
 * @brief Set the Para Value 16bit
 * 
 * @param address   : 파라미터 주소
 * @param value     : 설정하려는 값
 */
void setParaValSingle(uint16_t address, uint16_t value)
{
    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        Para[address].val = value;
    }
}


/**
 * @brief Set the Para max value 16bit
 * 
 * @param address   : 파라미터 주소
 * @param value     : 설정하려는 값
 */
void setParaMaxSingle(uint16_t address, uint16_t value)
{
    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        Para[address].max = value;
    }
}


/**
 * @brief Set the Para min value 16bit
 * 
 * @param address   : 파라미터 주소
 * @param value     : 설정하려는 값
 */
void setParaMinSingle(uint16_t address, uint16_t value)
{
    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        Para[address].min = value;
    }
}


/**
 * @brief Set the Para default value 16bit
 * 
 * @param address   : 파라미터 주소
 * @param value     : 설정하려는 값
 */
void setParaDefSingle(uint16_t address, uint16_t value)
{
    // Check address
    if(address <= PARA_END_VAL_ADDRESS)
    {
        Para[address].dft = value;
    }
}


/**
 * @brief Parameter의 값의 Type 형태를 변경
 * 
 * @param address : 파라미터 주소
 * @param type : 변경할 타입 ( VAL_UINT16, VAL_UINT32_H, VAL_FLOAT_H )
 */
void changeParaType(uint16_t address, uint8_t type)
{
    if(address <= PARA_END_VAL_ADDRESS)
    {
        switch (type)
        {
            case VAL_UINT8:
                Para[address].type = type;
            break;

            case VAL_UINT16:
                Para[address].type = type;
            break;

            case VAL_UINT32_H:
                Para[address].type = type;
                Para[address + 1].type = VAL_UINT32_L;
            break;

            case VAL_FLOAT_H:
                Para[address].type = type;
                Para[address + 1].type = VAL_FLOAT_L;
            break;
        }
    }
}


/**
 * @brief Get the Float From 16bit 값
 * 
 * @param value1 : 상위 16bit 값
 * @param value2 : 하위 16bit 값
 * @return float : 두 값이 합쳐진 float 값
 */
float getFloatFrom16(uint16_t value1, uint16_t value2)
{
    floatFrom16 convert;

    convert.raw.integer_high = value1;
    convert.raw.integer_low = value2;

    return convert.fVal;
}


/**
 * @brief Set 파라미터의 Min 값
 * 
 * @param address   : 파라미터 주소
 * @param value     : 저장하고자 하는 값
 */
void setTypeMin(uint16_t address, float value, para* buf, uint8_t type)
{
    floatFrom16 convert;
    uint32_t type32;

    buf[address].type = type;

    // Save
    switch (type)
    {
        case VAL_UINT16:
            buf[address].min = (uint16_t)value;
        break;

        case VAL_UINT32_H:
            type32 = (uint32_t)value;
            buf[address].min = (uint16_t)(type32 >> 16);
            buf[address + 1].min = (uint16_t)(type32 & 0xFFFF);
        break;

        case VAL_FLOAT_H:
            convert.fVal = value;
            buf[address].min = convert.raw.integer_high;
            buf[address + 1].min = convert.raw.integer_low;
        break;
    }
}


/**
 * @brief Set 파라미터의 Max 값
 * 
 * @param address   : 파라미터 주소
 * @param value     : 저장하고자 하는 값
 */
void setTypeMax(uint16_t address, float value, para* buf, uint8_t type)
{
    floatFrom16 convert;
    uint32_t type32;

    buf[address].type = type;
    
    // Save
    switch (type)
    {
        case VAL_UINT16:
            buf[address].max = (uint16_t)value;
        break;

        case VAL_UINT32_H:
            type32 = (uint32_t)value;
            buf[address].max = (uint16_t)(type32 >> 16);
            buf[address + 1].max = (uint16_t)(type32 & 0xFFFF);
        break;

        case VAL_FLOAT_H:
            convert.fVal = value;
            buf[address].max = convert.raw.integer_high;
            buf[address + 1].max = convert.raw.integer_low;
        break;
    }
}

