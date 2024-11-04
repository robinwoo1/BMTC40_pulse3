
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CURRENT_H
#define _CURRENT_H

#include "stm32f7xx_hal.h"

#ifndef __CURRENT_H
#define TQ_EXT extern //volatile
#else
#define TQ_EXT
#endif

TQ_EXT float Motor_mech_t_back;

extern float sinTBL[585];

extern float Vref_max;
extern float PWM_gain;
extern int16_t adc_current_u_value, adc_current_v_value;
extern float i_u_offset, i_v_offset, i_u_offset_back, i_v_offset_back;
extern long PWM_Offset, PWM_HalfMax;

extern float Vuss_ref, Vvss_ref, Vwss_ref, Vofs;
extern float Idse_ref, Iqse_ref, Idse, Iqse, Idse_flt, Iqse_flt, EIdse, EIqse;
extern float Iqse_sensor, Iqse_sensor_flt;
extern float Idse_ref_flt, Iqse_ref_flt;
extern float Vdse_ref_It, Vdse_ref_fb, Vdse_ref_ff, Vdse_ref, Vdse_ref_out;
extern float Vqse_ref_It, Vqse_ref_fb, Vqse_ref_ff, Vqse_ref, Vqse_ref_out;
extern float iu_real, iv_real, iw_real;

extern float Tref, Tref_p, Tref_i, Tref_ff;
extern float Tref_ramp, Tref_slop;
// float tq_cmd, tq_cmd1;

extern float sin_theta_e, cos_theta_e;
extern long Fu_ref, Fv_ref, Fw_ref, Pwm_ref;

extern int32_t on_time_cnt, on_time;
extern int32_t off_time_cnt, off_time;
extern int32_t on_off_state, pulse_on;
// extern float previous_Out_tq;
extern float previous_Tref;
extern float previous_Tref_i;
extern float previous_w_cmd;
extern float tq_cmd_pulse_on;
extern float preVqse_ref_It, preVqse_ref_fb, preVqse_ref_ff, preVqse_ref;

void Pwm_L_On(uint32_t Channel);
void Pwm_Off(uint32_t Channel);
void current_loop(void);

int32_t MovingAverage(int32_t input);

void MakeTable(void);

#undef TQ_EXT
#endif 
