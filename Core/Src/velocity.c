/**
  ******************************************************************************
  * @file    velocity.c
  * @brief   Velocity Service Routines.
  ******************************************************************************
*/  

/* Includes ------------------------------------------------------------------*/
#include "velocity.h"
#include "common.h"
#include <stdlib.h>
#include "variable.h"
#include "main_bg.h"
#include "torque.h"
#include "advancedPlus.h"
#include "svpwm.h"
#include "BldcControl.h"
#include "eventGraphTask.h"
#include "autoCustomizing.h"
#include "modbus.h"
#include "backup.h"
#include "timer.h"
#include "realtimeMonitoring.h"
#include "current.h"

static uint8_t flagRealtimeMake;


/* Extern variables -----------------------------------------------------------*/

multiSequenceStruct multiStr;



/* Functions -------------------------------------------------------------------*/
void Velocity_loop(void)        // 100us
{
    static uint8_t runEdge[2];
    float acc;

    cnt_250us++;

    // M_cnt();

    // Servo Error , run singel
    if (
        ( !Flag.Stopping ) &&
        ( system_err == 0 ) && 
        ( !Flag.Lock) &&
        ( Flag.InternalRun || Flag.ExternalRun || Flag.RunByMult ) 
        )
    {
        Flag.Run      = ON;
        FlagFasten.FasteningErrorForLED = OFF;      // motor 구동시에 이전 상태 삭제..

        switch (Flag.ControlFL)
        {
            case FASTENING_CONTROL:     Fastening();            break;
            case LOOSENING_CONTROL:     Loosening();            break;
            default:    
                tq_cmd = 0.0;   w_cmd = 0.0;    
                break;
        }
    }
    else if(Flag.Stopping == ON)
    {
        acc = square_acc(0.0);

        if(w_rpm_abs > 50)
        {
            if (fastening_step >= RAMPUP_TARGET_TORQUE)
                velocity(0.);       // 바로 stop
            else
                velocity(acc);      // 가감속으로 멈추는 것으로 변경함..

            if (acc == 0)           // acc 값이 0이 되었으나, 이전 값이 남아 있어서 회전이 바로 정지하지 않아서 추가함.
            {
                w_pi_out = 0.0;
                i_pi_out = i_err = 0;   
                Tref = 0;
                Tref_i = 0;
                
                Vqse_ref_It = 0;
                Vqse_ref_fb = 0;
                Vqse_ref_ff = 0;
                Vqse_ref    = 0;
            }
        }
        else
        {
            Flag.InternalRun = Flag.ExternalRun = Flag.RunByMult = OFF;
            Flag.ReadMSData = OFF;

            if (system_err == 0)
            {
                StatusOutput = rNONE;

                // if mult mode and fastening complete is not... send data..
                // if (!FlagFasten.CompleteMultisequence)
                //     putRealtimeData(&realPollStruct);
            }

            Flag.Stopping = OFF;
            // Flag.Run = OFF;
        }
    }
    else
    {
        // Check Fasatening stop error
        if (Flag.FasteningStopAlarm)
        {
            if(system_err == 0)
            {
                if (FastenSeq.JudgeMinAngle != 0)
                {
                    if (FastenSeq.JudgeMinAngle < mPulse.After_Fastening_Start_abs)
                        system_err = FASTENING_STOP_ERROR;
                }
                else
                    system_err = FASTENING_STOP_ERROR;
            }
            
            Flag.FasteningStopAlarm = OFF;
        }

        // Loosening : Screw -1 동작
        screwCountLooseningD1();


        i_err               = 0.0;
        i_pi_out            = 0.0;
        i_ref_flt           = 0.0;

        Tref                = 0.0;
        Tref_i              = 0.0;
        tq_cmd              = 0.0;
        Vqse_ref_It         = 0.0;
        Vqse_ref_fb         = 0.0;
        Vqse_ref_ff         = 0.0;
        Vqse_ref            = 0.0;
        w_pi_out            = 0.0;
        w_cmd_flt           = 0.0;
        w_err               = 0.0;
        w_err_1             = 0.0;
        w_rad               = 0.0;
        Flag.Run            = OFF;

        w_cmd               = 0.0;
        vin_out             = 0.0;          
        Cnt_LooseningTime   = 0;
        Iref_fb = Iref_It   = 0.0;
        FlagFasten.ReachedInitialSpeed = OFF;
        fastening_step         = REVOLUTION_START;

        TotalLooseningTimeReal = 0;
        TotalFasteningTime     = 0;

        tq_cmd_increase = 0.0;
        Cnt_JumpModeChangeToP2 = 0;
    }

    // Over_Speed_Check();
    NoSpeedUp_ErrorCheck();

    // Make Graph monitoring data
    Collect_GraphData();

    // Graph event socket monitoring
    graphMonitoringEvent();

    // Make realtime data
    if(Flag.Multi_Mode && multiStr.flStatusOld == FASTENING_CONTROL)
    {
        // Multi sequence 사용시
        ms_MakeRealtimeAtStop();
    }
    else
    {
        runEdge[0] = runEdge[1];
        runEdge[1] = Flag.Run;
        if(runEdge[0] > runEdge[1])
        {
            if(system_err == 0)
                putRealtimeData(&realPollStruct);
        }
    }
}


float SelectDisplayData(long data_num)       // data를 취한다.
{
    float  Ltmp;

    switch(data_num)
    {
        case  0: // 선택 안함.
            Ltmp = 0;
        break;
        case  1: // fastening torque
            Ltmp = RealTq_fromSensor_reflectedSettingUnit * inv_Torque_ConvertValue * getCrowFootRatio() * screw_type;
        break;
        case  2: // current feedback 10mA  50A정도 표기 32767 이상 값이 표기가 안되서.. - 값..
            // Ltmp = Vqse_ref * screw_type;
            Ltmp = i_real * screw_type;
        break;
        case  3: // speed
            Ltmp = w_rpm_display * inv_deceleration_ratio;
        break;
        case  4: // angle 1도
            Ltmp = (float)(mPulse.After_Fastening_Start_abs) * MathConv.AngleConv_mPulseToDeg;
            if (Ltmp > 99999)   Ltmp = 99999;
        break;
        case  5: //  speed command
            Ltmp = w_cmd * MathConv.ConvSpeed_screwtype;
        break;
        case  6: // current command 10mA
            Ltmp = tq_cmd * 100.0 * screw_type;
            // Ltmp = PWM_Out; // TODO:
        break;
        case 7: // snug angle
            Ltmp = mPulse.ContinuSnugAngle * MathConv.AngleConv_mPulseToDeg;
        case 20:
            // Final torque
            Ltmp = (tq_real_avg_for_holding_Nm * TorqueMetric[UNIT_Nm][Para[UNIT].val] * inv_Torque_ConvertValue * getCrowFootRatio());
        break;
        default:
            Ltmp = 0;
        break;
    }

    return(Ltmp);
}

void Over_Speed_Check()			//  overspeed
{
	static float w_rpm_abs_OverSpeedCheck = 0.0;
	
	if (system_err != 0) return;

	w_rpm_abs_OverSpeedCheck = (w_rpm_abs * 0.2) + (w_rpm_abs_OverSpeedCheck * 0.8);

	if (w_rpm_abs_OverSpeedCheck > (Motor.max_motor_speed * 1.2))	// 20%이상..
	{
		if ((system_err == 0) && (Flag.Run))
			system_err = OVER_SPEED;

		Flag.InternalRun = OFF;
		Flag.ExternalRun = OFF;
		Flag.RunByMult   = OFF;
		tq_cmd = 0.0;
	}
}

void NoSpeedUp_ErrorCheck(void)
{
	long rpm_cmd;
	static int Cnt_NoSpeedUp_Error = 0;

	if (system_err != 0) return;

	if (Flag.Run && !Flag.Stopping)
	{
		rpm_cmd = labs((w_cmd * RAD_TO_RPM_SCALE) / 2.);

    // Speed command 100 이상일 때
		if (rpm_cmd > 100)
		{
      // 실제 속도 10rpm 이하, 0.2A 이하 면 에러  (대기 전류: 0.5, 1000RPM: 0.7A)
			if ((w_rpm_abs < 10) && fABS(i_real) < 0.2)
			{
				Cnt_NoSpeedUp_Error++;

				if ((Cnt_NoSpeedUp_Error > 5000) && (system_err == 0))	// 0.5sec
					system_err = NO_SPEED_ERROR;
			}
		}
		else
			Cnt_NoSpeedUp_Error = 0;
	}
	else
		Cnt_NoSpeedUp_Error = 0;
}

float square_acc(float Vin)
{
    float Vin_diff;

    Vin_diff = Vin - vin_out;

    if(Vin_diff >= 0)
    {
        if(Flag.ControlFL == FASTENING_CONTROL)
        {   
            if(vin_out >= 0 )   vin_out += Delta_acc;
            else                vin_out += Delta_dcc;
        }
        else
        {   
            if(vin_out >= 0 )   vin_out += Delta_acc_loosen;
            else                vin_out += Delta_dcc_loosen;
        }

        if(vin_out >= Vin)  vin_out = Vin;          /* limit */
    }
    else 
    {
        if(Flag.ControlFL == FASTENING_CONTROL)
        {   
            if(vin_out >= 0 )   vin_out -= Delta_dcc;
            else                vin_out -= Delta_acc;
        }
        else
        {   
            if(vin_out >= 0 )   vin_out -= Delta_dcc_loosen;
            else                vin_out -= Delta_acc_loosen;
        }
        
        if(vin_out <= Vin)  vin_out = Vin;          /* limit */
    }   

    return vin_out;

}

void velocity(float input)
{
  // input : speed(RPM)
  // output : tq_cmd = w_pi_out (Nm)
  
  float tmp;
  float ki_val;

  w_cmd = input;

  // set pulse mode
  if (pulse_on == 0 && fabs(tq_real_Nm) > 2.5)
  {
    pulse_on = 1;
    on_time_cnt = 0;
    off_time_cnt = 0;
    on_off_state = 0;

    vin_out       = 0.0;

    w_pi_out = 0;
    Tref = 0;
    Tref_i = 0;
    
    Vqse_ref_It  = 0;
    Vqse_ref_fb  = 0;
    Vqse_ref_ff  = 0;
    Vqse_ref     = 0;
    Vdse_ref_It  = 0;
    Vdse_ref_fb  = 0;
    Vdse_ref_ff  = 0;
    Vdse_ref     = 0;
  }

  // cancel pulse mode
  if (pulse_on == 1 && on_off_state == 1 && Flag.ControlFL == 1 && fABS(tq_cmd) > 3.5 && on_time_cnt > 100)
  {
    if (fabs(tq_real_Nm) < 2.0)
    {
      pulse_on = 0;
      on_off_state = 0;
      on_time_cnt = 0;
      off_time_cnt = 0;

      vin_out       = 0.0;

      w_pi_out = 0;
      Tref = 0;
      Tref_i = 0;
      
      Vqse_ref_It  = 0;
      Vqse_ref_fb  = 0;
      Vqse_ref_ff  = 0;
      Vqse_ref     = 0;
      Vdse_ref_It  = 0;
      Vdse_ref_fb  = 0;
      Vdse_ref_ff  = 0;
      Vdse_ref     = 0;
    }
  }
  
  if (pulse_on)
  {
    if (on_off_state == 1)
    {
      on_time_cnt++;
      if (on_time_cnt >= on_time || (on_time_cnt > 50 && fabs(tq_real_Nm) >= fabs(tq_cmd)))
      {
        on_off_state = 0;
        on_time_cnt = 0;
        off_time_cnt = 0;

        // previous_Out_tq = 0;
        
        w_pi_out = 0;
        Tref = 0;
        Tref_i = 0;

        Vqse_ref_It  = 0;
        Vqse_ref_fb  = 0;
        Vqse_ref_ff  = 0;
        Vqse_ref     = 0;
        Vdse_ref_It  = 0;
        Vdse_ref_fb  = 0;
        Vdse_ref_ff  = 0;
        Vdse_ref     = 0;
      }
    }
    else
    {
      off_time_cnt++;
      if (off_time_cnt >= off_time)// || ((on_time_cnt > 50) && (Flag.ControlFL == 0) && (w_rad >= 0)))
      {
        on_off_state = 1;
        on_time_cnt = 0;
        off_time_cnt = 0;

        w_pi_out = 0;
        Tref = 0;
        Tref_i = 0;

        Vqse_ref_It  = 0;
        Vqse_ref_fb  = 0;
        Vqse_ref_ff  = 0;
        Vqse_ref     = 0;
        Vdse_ref_It  = 0;
        Vdse_ref_fb  = 0;
        Vdse_ref_ff  = 0;
        Vdse_ref     = 0;
      }
    }
  }

  // Error
  if (Flag.Run || Flag.Stopping)
  {
    w_err = w_cmd - w_rad;
    
    // if ((Flag.ControlFL == 0 && pulse_on == 1 && fastening_step < REDUCE_SPEED))
    // {
    //   if (fabs(Tref_ramp) >= FastenSeq.SeatingTQ_Nm * 1.2)
    //     Tref_ramp = FastenSeq.SeatingTQ_Nm * (float)screw_type;
    //   else
    //   {
    //     Tref_slop = FastenSeq.SeatingTQ_Nm / 0.2;
    //     Tref_ramp += Tref_slop * TS_VEL * (float)screw_type;
    //   }
    //   Tref = Tref_ramp;
    // }
    // else if ((Flag.ControlFL == 0 && pulse_on == 1 && fastening_step >= RAMPUP_TARGET_TORQUE && fastening_step < LITTLE_REWIND))
    // {
    //   if (fabs(Tref_ramp) >= FastenSeq.TargetTorque_Nm * 1.05)
    //     Tref_ramp = FastenSeq.TargetTorque_Nm * (float)screw_type;
    //   else
    //   {
    //     Tref_slop = FastenSeq.TargetTorque_Nm / 0.2;
    //     Tref_ramp += Tref_slop * TS_VEL * (float)screw_type;
    //   }
    //   Tref = Tref_ramp;
    // }
    // else 
    // if ((Flag.ControlFL == 1) && pulse_on == 1)
    if (pulse_on == 1)
    {
      if (Flag.ControlFL == 1)
      {
        Tref_slop = -FastenSeq.TargetTorque_Nm / 0.35;
        Tref_ramp += Tref_slop * TS_VEL * (float)screw_type;
      }
      else
      {
        Tref_slop = FastenSeq.TargetTorque_Nm / 0.35;
        Tref_ramp += Tref_slop * TS_VEL * (float)screw_type;
      }
      Tref = Tref_ramp;

    }
    else
    {
      Tref_i += Gain.w_ki * (w_err - Gain.w_Ka * (Tref - w_pi_out));
      Tref_p = Gain.w_kp * w_err;
      Tref_ff = 0;
      Tref = Tref_p + Tref_i + Tref_ff;
    }
    

    tmp = 30;//Motor.torque_limit * deceleration_ratio;
    // tmp = FastenSeq.MaxTorque_Nm * 1.03;	// Speed PID 이후 Torque PID Max 값 제한
    if      (Tref >  tmp) w_pi_out =  tmp;
    else if (Tref < -tmp) w_pi_out = -tmp;
    else w_pi_out = Tref;
  }
  else
  {
    {
      pulse_on = 0;
      on_time_cnt = 0;
      off_time_cnt = 0;
      on_off_state = 0;

      Tref_slop = 0;
      Tref_ramp = 0;
    }
    Tref = 0;
    Tref_i = 0;
    w_pi_out = 0;
    tq_cmd = 0;
    w_cmd = 0;
  }
  

  // Limit
  // tmp = 100.;
  // if (Flag.ControlFL)
    tmp = 30;//Motor.torque_limit * deceleration_ratio;
  // else
  //   tmp = FastenSeq.TargetTorque_Nm*1.5;
  if      (w_pi_out >  tmp)       w_pi_out =  tmp;
  else if (w_pi_out < -tmp)       w_pi_out = -tmp;

  fasten_loosen_control();

  if      (w_pi_out >  tmp)       w_pi_out =  tmp;
  else if (w_pi_out < -tmp)       w_pi_out = -tmp;

  if (pulse_on)
  {
    if (on_off_state)
    {
      tq_cmd = w_pi_out;
    }
    else
    {
      tq_cmd = 0;//-w_pi_out;
    }
  }
  else
  {
    tq_cmd = w_pi_out;
  }  
}

// Encoder read
void M_cnt(void)
{
    int32_t  m_diff;
    static int32_t m_diffOld;

    m_cnt_m = bldcControl.absoluteAngle;
    m_diff = m_cnt_m - m_cnt_m_old;

    // Encoder 흔들림 보완 (정지 중 조금씩 흔들리는 것 제거)
    if(m_diff >= -1 && m_diff <= 1 && Flag.Run == OFF)
        m_diff = 0;
    
    // Check 4096 Pulse overflow
    if(m_diff > 3000)
    {
        if(m_cnt_m > m_cnt_m_old)
        {
            m_diff -= 4096;
        }
    }
    else if(m_diff < -3000)
    {
        if(m_cnt_m < m_cnt_m_old)
        {
            m_diff += 4096;
        }
    }

    m_pulse_diff = m_diff;

    // BISS 통신 시 Angle 값이 갑작스럽게 많이 변하는 경우 발생 - 속도가 튀는 것을 막기위해 제한 (8 pulse가 100us 안에 더 변하면 1171RPM이 변했을 경우 가능)
    if(Flag_L_F_control_current == 0 && m_diff - m_diffOld > 7)
        m_diff = m_diffOld;

    m_diffOld = m_diff;


    if (m_pulse_diff < 0)   m_pulse_diff_abs = -m_pulse_diff;
    else                    m_pulse_diff_abs =  m_pulse_diff;

    if (m_diff == 0)
    {
        if(w_rad > 10)
        {
            if(fastening_step <= FIND_SEATING_POINT)
                w_rad_tmp = w_rad * 0.98;               // 목표 속도가 낮을 때 해상도 문제로 속도가 0이 되는 것 방지
            else
                w_rad_tmp = w_rad * 0.7;
        }
        else
            w_rad_tmp = 0.;
    }
    else
    {
      // w_rad_tmp = -((float)m_pulse_diff * vel_time);
      w_rad_tmp = ((float)m_pulse_diff * vel_time);
    }

    // Low pass filter
    // w_rad = (0.003 * w_rad + TS_VEL * w_rad_tmp) / (TS_VEL + 0.003);
    w_rad = (1-2000.*TS_VEL)*w_rad + 2000.*TS_VEL*w_rad_tmp;

    w_rpm = w_rad * RAD_TO_RPM_SCALE;
    if( w_rpm < 0.0)        w_rpm_abs = -w_rpm;
    else                    w_rpm_abs =  w_rpm;

    // 화면 표시용 속도 - Low pass filter
    w_rpm_display = (0.020 * w_rpm_display + TS_VEL * w_rpm_abs) / (TS_VEL + 0.020);

    m_cnt_m_old = m_cnt_m;

}

long Check_counter(long cnt)
{
    if(cnt >= 32768)
        cnt -= 65536;
    else if(cnt <= -32768) 
        cnt += 65536;

    return cnt;
}

void fasten_loosen_control(void)
{
    float   tq_real_Nm_screw_type;
    float   tq_target;
    float   tq_real;
    float   tq_comp;
    static int Cnt_ReduceSpeed = 0;
    static uint32_t Cnt_HoldingTimeforError = 0;
    
    if (Flag.Stopping)  return;

    // loosening
    if( Flag.ControlFL == LOOSENING_CONTROL )
    {
        tq_real = RealTq_fromSensor_reflectedSettingUnit;
        tq_real_Nm_screw_type = RealTq_fromSensor_reflectedSettingUnit * TorqueMetric[Para[UNIT].val][UNIT_Nm];
        if (fABS(loosening_PickTorque) < fABS(tq_real))
        {
            loosening_PickTorque = tq_real;

            Torque_cnv.fastenComplete = fABS(RealTq_fromSensor_reflectedSettingUnit);
            Torque_cnv.fastenComplete_dsp_comp = Torque_cnv.fastenComplete * inv_Torque_ConvertValue;                
        }

        // Loosening max torque
        if (Para[USE_MAXTQ_FOR_LOOSENING].val == 1)
        {
            if (fABS(tq_real_Nm_screw_type) >= FastenSeq.MaxTorque_Nm * 0.9)
            {
                Cnt_LooseningTime++;

                if (tq_real_Nm_screw_type < 0)
                    tq_real_Nm_screw_type = -FastenSeq.MaxTorque_Nm;
                else
                    tq_real_Nm_screw_type = FastenSeq.MaxTorque_Nm;
            }
        }
        else
        {
            // 최대 토크까지 반영 (목표 토크가 최대 토크의 70% 이상일 때)
            if (FastenSeq.TargetTorque_Nm >= (FastenSeq.MaxTorque_Nm * 0.7))
            {
                if (fABS(tq_real_Nm_screw_type) >= FastenSeq.TargetTorque_Nm) 
                {
                    Cnt_LooseningTime++;
                    if (tq_real_Nm_screw_type < 0)
                        tq_real_Nm_screw_type = -FastenSeq.TargetTorque_Nm;
                    else
                        tq_real_Nm_screw_type = FastenSeq.TargetTorque_Nm;
                }	
            }
            else
            {
                // 목표 토크의 130% 까지 반영
                if (fABS(tq_real_Nm_screw_type) >= (FastenSeq.TargetTorque_Nm * 1.3))
                {
                    Cnt_LooseningTime++;
                    if (tq_real_Nm_screw_type < 0)
                        tq_real_Nm_screw_type = -FastenSeq.TargetTorque_Nm * 1.3;
                    else
                        tq_real_Nm_screw_type = FastenSeq.TargetTorque_Nm * 1.3;
                }
            }
        }
        

        // Crowfoot 사용중 Reverse torque 또는 Speed 설정이 안되어 있을 때 정지
        if(Para[CROWFOOT_ENABLE].val && (getParaVal(CROWFOOT_REVERSE_TORQUE_H) == 0 || Para[CROWFOOT_REVERSE_SPEED].val == 0))
        {
            Flag.Stopping = ON;
            system_err = CROWFOOT_REVERSE_TORQUE_ERROR;
        }

        // Crowfoot 역회전 토크 정지
        if ((Para[CROWFOOT_ENABLE].val) && (getParaVal(CROWFOOT_REVERSE_TORQUE_H)) && Para[CROWFOOT_REVERSE_SPEED].val)      // check crowfoot torque..
        {
            if (fABS(tq_real_Nm_screw_type) >= LooseningSeq.Crowfoot_torque_Nm)     // if high.
            {
                Flag.Stopping = ON;
            }
        }

        // Screw cancel during loosening
        if(Para[SCREW_CANCEL_LOOSENING].val)
        {
            // 목표 토크의 % 이상이 될 경우 Screw -1
            if(fABS(tq_real_Nm_screw_type) > FastenSeq.TargetTorque_Nm * (Para[SCREW_CANCEL_LOOSENING].val / 100.0))
            {
                Flag.ScrewDecrLoosening = ON;
            }
        }
    }

    if(Flag.ControlFL == FASTENING_CONTROL)
    {
        tq_real_Nm_screw_type = RealTq_fromSensor_reflectedSettingUnit * TorqueMetric[Para[UNIT].val][UNIT_Nm] * screw_type;

        // check snug torque
        if (fastening_step >= START_FASTENING)
        {
            if (FastenSeq.SnugTQ_Nm != 0)
            {
                if ((fABS(tq_real_Nm_screw_type) >= FastenSeq.SnugTQ_Nm) && !FlagFasten.SnugTorque)
                {
                    FlagFasten.SnugTorque = ON;
                    Torque_cnv.snug = tq_real_Nm_screw_type * TorqueMetric[UNIT_Nm][Para[UNIT].val];
                    mPulse.StartSnugTq = mPulse.After_Fastening_Start_abs;      // 이곳에서 한번만 값을 넣는다.

                    // 그래프 - Step 설정
                    setGraphStepQueue(GRAPH_STEP_SNUG_TQ);
                }
            }
        }

        // Set target torque
        if (fastening_step == THREADTAP)
            tq_target = ThreadTap.Max_Torque_Nm;                // Thread tapping torque
        else if(fastening_step == FIND_SEATING_POINT && FreeSpeed.FlagInStep && FreeSpeed.FlagOutStep == OFF)
            tq_target = FreeSpeed.Torque_Nm;                    // Free speed torque (Ramp up 구간이 아닐 때는 목표 토크 이상으로 토크 상승 X)
        else
            tq_target = FastenSeq.TargetTorque_Nm;              // Preset torque (해당값은 보상이 된 값임.)


        // Angle control
        if (FastenSeq.TcamActm && !Flag_JumpCompleteAngleControl)
        {
            if (fABS(tq_real_Nm_screw_type) > (tq_target * 1.03))        // max torque에 도착하면 error
            {
                Torque_cnv.fastenComplete = RealTq_fromSensor_reflectedSettingUnit;
                Torque_cnv.fastenComplete_dsp_comp = Torque_cnv.fastenComplete * inv_Torque_ConvertValue;
                Fasten_Real_Torque = RealTq_fromSensor_reflectedSettingUnit * screw_type;
                system_err = C_TORQUE_ERROR;
            }
        }


        // calculating gradient
        calcGradient(fastening_step, tq_real_Nm_screw_type, (float)mPulse.After_Fastening_Start_abs / (float)Motor.num_pulse);

        // Advacned - Prevailing 구간 angle 계산
        check_prevailing(fastening_step, mPulse.After_Fastening_Start_abs, tq_real_Nm_screw_type);


        switch (fastening_step)
        {
            case REVOLUTION_START:      
                tq_cmd_increase = w_pi_out; 

                // Init variable (Reduce speed)
                Cnt_ReduceSpeed = 0;
            break;
            case STOP_REVERSE_ANGLE :
                tq_cmd_increase = w_pi_out; 
            break;
            case THREADTAP:
                if (!FlagFasten.FinishedThreadTap)
                {
                    if (fABS(tq_real_Nm_screw_type) > ThreadTap.Max_Torque_Nm)
                        system_err = THREADTAP_MAX_TORQUE_ERROR;
                    else
                    {
                        if (ThreadTapTorque_real < fABS(tq_real_Nm_screw_type))
                            ThreadTapTorque_real = tq_real_Nm_screw_type;

                        if (fABS(tq_real_Nm_screw_type) >= ThreadTap.Min_Torque_Nm)        // min max torque 범위에 들어옴.
                            FlagFasten.WithinThreadTapTorque = ON;
                        // end condition.
                        // end 시에는 나사산을 내기 위해 힘이 많이 가해진 상태에서 갑자기 정상 조건으로 돌아오기 힘듬.
                        // 이를 해결하기 위해서 초기화 후에 처음부터 다시 시작함..
                        if ((fABS(tq_real_Nm_screw_type) <= ThreadTap.Finish_Torque_Nm) && FlagFasten.WithinThreadTapTorque)
                        {
                            FlagFasten.FinishedThreadTap = ON;
                            vin_out = 0.0;                              
                        }
                    }
                }
    
                tq_cmd_increase = w_pi_out;
            break;
            case FIND_ENGAGING_TORQUE:
                if (tq_real_Nm_screw_type >= EngagingTQ.Torque)  //&& !Flag_AngleClear_on)
                {
                    if (EngagingTQ.EngagingAfterAngleClear)
                        mPulse.After_Fastening_Start_abs = 0;
                    
                    Flag.FoundEngagingTorque = ON;
                }
                            
                tq_cmd_increase = w_pi_out; 
            break;
            case START_FASTENING:
                if (FreeSpeed.FlagInStep)       // 무조건 이동.
                    fastening_step = FIND_SEATING_POINT;
                else if (tq_real_Nm_screw_type > FastenSeq.SeatingTQ_Nm)
                {
                    Cnt_JumpModeChangeToP2++;
                    if (Cnt_JumpModeChangeToP2 >= 2)
                    {
                        fastening_step = FIND_SEATING_POINT;              // Seating 토크 이상 값을 바로 감지 했을 때
                    }

                    if (tq_real_Nm_screw_type > (tq_target * 0.6))        // P2일 경우 감속비가 높은 것에서는 계속 이곳에 걸림..
                    {
                        fastening_step = FIND_SEATING_POINT;
                    }
                }
                tq_cmd_increase = w_pi_out;
            break;
            case FIND_SEATING_POINT:            
                // free speed 구간에서는 seating check를 하지 않는다.
                // free speed 구간에서 over torque 발생시 error 처리..
                if (FreeSpeed.FlagInStep)
                {
                    if (!FreeSpeed.FlagOutStep)     // angle 완료가 아니면.. error check
                    {
                        if (tq_real_Nm_screw_type >= FreeSpeed.Torque_Nm)       // target를 초과하면 error
                            system_err = TORQUEUP_DURING_FREE_SPEED_ERROR;
                    }
                }

                // Advanced auto seating
                if (AtSeating.autoSeatDetectActive)
                {
                    // Free speed 구간 외에서 Gradient 확인
                    if((FreeSpeed.FlagInStep == OFF && FreeSpeed.FlagOutStep == OFF) || (FreeSpeed.FlagInStep == ON && FreeSpeed.FlagOutStep == ON))
                        check_gradient_and_min(tq_real_Nm_screw_type, AtSeating.currentGradient_Nm, mPulse.After_Fastening_Start_abs); 
                }
                else
                {
                    // Auto seating 아닐 경우 seating point 감지
                    if (FreeSpeed.FlagInStep)
                    {
                        if (FreeSpeed.FlagOutStep)      // if complete
                        {
                            // seating point check는 FlagOutStep이 on 이 되면 check 한다.
                            if (tq_real_Nm_screw_type >= FastenSeq.SeatingTQ_Nm)
                            {
                                w_pi_out = FastenSeq.SeatingTQ_Nm * screw_type;
                                FlagFasten.FindedSeationPointStep = ON;

                                Torque_cnv.seating = tq_real_Nm_screw_type * TorqueMetric[UNIT_Nm][Para[UNIT].val];
                            }
                        }
                    }
                    else if (tq_real_Nm_screw_type >= FastenSeq.SeatingTQ_Nm)
                    {
                        FlagFasten.FindedSeationPointStep = ON;
                        
                        Torque_cnv.seating = tq_real_Nm_screw_type * TorqueMetric[UNIT_Nm][Para[UNIT].val];
                    }                           
                }

                // Check TQ, A1 angle min limit over (Seating point를 찾았을 때)
                if(FlagFasten.FindedSeationPointStep)
                    check_seating_error_min(Torque_cnv.seating, mPulse.After_Fastening_Start_ForA1);

                // Check TQ, A1 angle max limit over
                check_seating_error_max(Torque_cnv.seating, mPulse.After_Fastening_Start_ForA1);

                Cnt_ReduceSpeed = 0;
                tq_cmd_increase = w_pi_out;
            break;
            case REDUCE_SPEED:
                Cnt_ReduceSpeed++;

                if (Cnt_ReduceSpeed > (100 / TS_VEL / 10000))         // 10ms
                {
                    Cnt_ReduceSpeed = 0;
                    FlagFasten.ReduceSpeedStep = ON;
                    Cnt_HoldingTimeforError = 0;

                    // 마지막 Step에서 PID 제어값 초기화
                    tq_cmd = 0.0;
                    w_pi_out = 0.0;
                    w_err = 0.0;
                    vin_out = 0.0;
                }

                // REDUCE SPEED 구간 중 토크가 목표 토크 근처로 발생했을 때 바로 넘어가는 동작
                if(tq_real_Nm_screw_type > tq_target * 0.80)
                {
                    FlagFasten.ReduceSpeedStep = ON;        
                }

                tq_cmd_increase = w_pi_out;
            break;
            case RAMPUP_TARGET_TORQUE:
                // Ramp-up step에서는 기울기를 만들기가 쉽지 않다.
                // 이유는 모터 전류와 토크와의 시간차가 많이 발생을 해서 이부분을 해결하기가 쉽지 않다.
                // tq-rising time은 모터의 speed를 올리는 기울기로 사용한다.

                // Advanced 모드 적용될 때 목표 토크 변경
                if ((AdvanceMode == ADV_CLAMP_TQ_CONTROL) && (Clamping.replace_target_active))   
                {
                    // Auto seating을 사용하지 않았을 때 seating 값 반영
                    if(AtSeating.finalGradientTq_Nm == 0)
                        AtSeating.finalGradientTq_Nm = Torque_cnv.seating * TorqueMetric[Para[UNIT].val][UNIT_Nm];

                    // Clamp mode - Target 토크 변경
                    Clamping.compensation_torque_Nm = AtSeating.finalGradientTq_Nm + Clamping.clamp_torque_Nm;        // seating tq + clamp tq. 
                    tq_target = Clamping.compensation_torque_Nm;

                    // Max torque check
                    if (tq_target> FastenSeq.MaxTorque_Nm)
                        system_err = COMPENSATION_OVER_MAX_TQ;

                    // Check Clamp 파라미터 설정
                    if(Clamping.clamp_torque_Nm == 0)
                        system_err = ADVACNED_PARA_ERROR;
                }    
                else if ((AdvanceMode == ADV_PREVAIL_TQ_CONTROL) && (Prevailing.compensationActive))       // 토크 변경.
                {
                    // Prevailing mode - Target 토크 변경
                    Prevailing.compensation_torque_Nm = Prevailing.result_of_prevailing_torque_Nm + Clamping.clamp_torque_Nm;       // Prevailing + clamp
                    tq_target = Prevailing.compensation_torque_Nm ;

                    // Max torque check
                    if(tq_target > FastenSeq.MaxTorque_Nm)
                        system_err = COMPENSATION_OVER_MAX_TQ;
                }

                // Torque, Speed 우선 제어 판단
                if(w_rpm_abs > FastenSeq.RampUpSpeed * deceleration_ratio * 1.2)
                {
                    // 속도 제어  (토크 상승이 멈추고 속도가 빨라질 때 폭주 제한) 
                    w_pi_out = w_pi_out;
                }
                else if(tq_real_Nm_screw_type > tq_target * 0.90)
                {
                    // 토크 제어 (목표 토크의 95% 도달 후)
                    w_pi_out = tq_target * 1.05;
                }
                else
                    w_pi_out = w_pi_out;


                // 목표 토크의 98% 이상일 때 정지 시작
                if (tq_real_Nm_screw_type > tq_target * 0.98)
                {
                    FlagFasten.FindedTargetTorqueStep = ON;                    
                    w_pi_out = tq_target * 0.2 * screw_type;

                    // 홀딩 중 토크 보상 (Target의 100%가 안될 때 - 외부 표시용) FIXME:
                    if(tq_real_avg_for_holding_Nm < tq_target)
                    {
                        tq_real_avg_for_holding_Nm = tq_real_avg_for_holding_Nm + (tq_target - tq_real_avg_for_holding_Nm) * 1.2;
                    }
                }

                if (tq_real_avg_for_holding_Nm < tq_real_Nm_screw_type)         // getting max torque value.
                    tq_real_avg_for_holding_Nm = tq_real_Nm_screw_type;

                // speed 가 100rpm보다 낮게 700ms 동안 유지가 되면 에러 발생..
                if (w_rpm_abs < 100)
                    Cnt_HoldingTimeforError++;
                
                // if (Cnt_HoldingTimeforError > (7000 / TS_VEL / 10000))     // 700ms
                // {
                //     system_err = NO_RAMPUP_TORQUE;
                //     Cnt_HoldingTimeforError = 0;
                // }

                tq_cmd_increase = w_pi_out;
            break;
            case MAINTAIN_TARGET_TORQUE:
                tq_cmd_increase = w_pi_out;

                // 가장 높게 올라간 토크를 표시
                if (tq_real_avg_for_holding_Nm < tq_real_Nm_screw_type)
                    tq_real_avg_for_holding_Nm = tq_real_Nm_screw_type;

            break;
            case LITTLE_REWIND: 
                tq_cmd_increase = w_pi_out;
            break;

            default : break;
        }


        if(Flag.ControlFL == FASTENING_CONTROL)
        {
            w_pi_out = tq_cmd_increase;

            // Seating 이전 체결 구간에서는 실시간으로 over torque를 check 한다
            if ((fastening_step >= START_FASTENING) && (fastening_step <= REDUCE_SPEED))
            {
                // 체결에서 실시간으로 Max torque limit 확인
                if(FreeSpeed.FlagInStep && FreeSpeed.FlagOutStep == OFF && Para[FREE_SPEED_MAX_TORQUE_H].val)
                    tq_comp = FreeSpeed.Torque_Nm * 1.1;                    // Free speed 구간 내에서는 Free speed - max torque의 110%
                else
                    tq_comp = FastenSeq.TargetTorque_Nm * 1.1;              // 목표 토크 110%
                
                // 드라이버 Max torque limit 확인
                if (tq_comp > FastenSeq.MaxTorque_Nm)
                    tq_comp = FastenSeq.MaxTorque_Nm * 1.03;
                
                // 현재 토크가 목표 토크 보다 높은지 확인
                if (fABS(tq_real_Nm_screw_type) > tq_comp)
                {
                    if(system_err == 0)
                    {
                        Torque_cnv.fastenComplete = RealTq_fromSensor_reflectedSettingUnit;
                        Fasten_Real_Torque = RealTq_fromSensor_reflectedSettingUnit * screw_type;
                        Torque_cnv.fastenComplete_dsp_comp = Torque_cnv.fastenComplete * inv_Torque_ConvertValue;
                        system_err = OVER_TORQUE_BEFORE_RAMPUP;
                    }
                }

                // 런다운 다운 토크의 최소/최대 검사
                runDownTorqueMinMaxLimit(mPulse.After_Fastening_Start_abs, tq_real_Nm_screw_type);
            }

            // rewind시에는 torque limit의 100%까지 사용한다.
            if ((fastening_step == LITTLE_REWIND) && FlagFasten.OverTorqueErrorCheck)
            {
                if (fABS(tq_real_Nm_screw_type) >= FastenSeq.MaxTorque_Nm) 
                {
                    if(system_err == 0)
                        system_err = FASTEN_OVER_TORQUE_ERROR;
                }
            }
            else
            {
                // ACTM에서는 limit control을 하지 않는다. 상단에서 error 처리 함.
                // if (!FastenSeq.TcamActm)    // if TCMA..
                // {
                //     if (w_pi_out > tq_target)
                //         w_pi_out = tq_target;
                //     else if (w_pi_out < -tq_target)
                //         w_pi_out = -tq_target;
                // }
            }
        }
    }
}

void Fastening(void)        // 500us
{
    long j = OFF, counter_diff;
    float w_jog;

    static float fasten_speed = 0.;
    static int   Cnt_delaytime = 0;       // ThreadTap

    if (Flag.Stopping == ON)    // 
    {   
    }
    else
    {
        // 초기 시작 시 파라미터 값들 초기화
        if (TotalFasteningTime < 1)
        {
            Initial_Fastening_Parameter();
        }

        ChangeSpeedAcc();   // step별 speed acc 적용..

        // alarm check 는 snug tq가 있으면 snug tq 이후에 확인하고, 없으면 바로 확인.
        if (Para[FASTENING_STOP_ALARM].val == ON)
        {
            if (FastenSeq.SnugTQ_Nm == 0)      // no setting
                Flag.FasteningStopAlarm = ON;
            else
            {
                if (FlagFasten.SnugTorque)  // snug tq 감지..
                    Flag.FasteningStopAlarm = ON;
            }
        }
            
        //  free speed 적용 rootin 초기 속도를 다르게 가져간다.
        if ((FreeSpeed.Speed != 0) && (FreeSpeed.Angle != 0) && ((fastening_step == START_FASTENING) || (fastening_step == FIND_SEATING_POINT)))
        {
            // first angle check 
            if (FreeSpeed.Angle < mPulse.After_Fastening_Start_abs)
            {
                fasten_speed = FastenSeq.TargetSpeed;
                FreeSpeed.FlagOutStep = ON;

                // 그래프 - Step 설정
                setGraphStepQueue(GRAPH_STEP_FASTENING_SPEED_START);
            }
            else
            {
                fasten_speed = FreeSpeed.Speed;
                FreeSpeed.FlagInStep = ON;                  // check free speed in sequence

                // 그래프 - Step 설정
                setGraphStepQueue(GRAPH_STEP_FREE_SPEED);
            }
        }
        else    
            fasten_speed = FastenSeq.TargetSpeed;


        // motor position cal
        if (mPulse.After_Fastening_Start_abs < 20000000)
        {
            mPulse.After_Fastening_Start_abs += m_pulse_diff_abs;
        }

        if (FastenSeq.TcamActm && !Flag_JumpCompleteAngleControl)   // angle control
        {
            if (FastenSeq.TargetAngle == 0)     // angle control은 반드시 0 이상이어야 함..
                system_err = TARGET_ANGLE_PARA_SET_ERROR;
            else
            {
                if(FastenSeq.SnugTQ_Nm == 0)   // 전체 angle check
                    mPulse.RealSnugTq = mPulse.After_Fastening_Start_abs;
                else
                {
                    if (FlagFasten.SnugTorque)  // snug tq 감지..
                        mPulse.RealSnugTq = mPulse.After_Fastening_Start_abs - mPulse.StartSnugTq;
                    else
                        mPulse.RealSnugTq = 0;
                }
                
                // Don't check Target Anlge in Advanced step (ACTM) 
                if(fastening_step <= THREADTAP)
                    mPulse.RealSnugTq = 0;

                if (FastenSeq.TargetAngle <= mPulse.RealSnugTq)         // 체결 완료..
                {
                    // Fastening을 끝내기 전 A1 각도 갱신
                    if(fastening_step <= FIND_SEATING_POINT)
                        mPulse.After_Fastening_Start_ForA1 = mPulse.RealSnugTq + 1;      

                    // angle control에서  LITTLE_REWIND step 사용..
                    fastening_step = MAINTAIN_TARGET_TORQUE;
                    Flag_JumpCompleteAngleControl = ON;
                    Fasten_Real_Torque = RealTq_fromSensor_reflectedSettingUnit * screw_type;
                }
            }
        }

        // Realtime, Graph monitoring snug torque
        if((FastenSeq.SnugTQ_Nm != 0) && (FlagFasten.SnugTorque))       // get the snug angle.
        {
            mPulse.ContinuSnugAngle = mPulse.After_Fastening_Start_abs - mPulse.StartSnugTq;    // Graph
            mPulse.RealSnugTq = mPulse.ContinuSnugAngle;                                        // Realtime
        } 
        else
            mPulse.ContinuSnugAngle = 0;

        Gain_Setting();     // 15% 이하에서는 gain을 달리 가져 간다.

        switch ( fastening_step )
        {
            case REVOLUTION_START:
                // 아래 조건이 맞은면 실행.
                if ((FreeReverse.Speed !=0) && (FreeReverse.Angle != 0))
                {
                    // 그래프 - Step 설정
                    setGraphStepQueue(GRAPH_STEP_FREE_REVERSE);

                    w_jog = square_acc(FreeReverse.Speed);
                    velocity(w_jog);
                }   
                else
                    fastening_step = THREADTAP;

                if( FreeReverse.Angle < mPulse.After_Fastening_Start_abs )
                {
                    fastening_step = STOP_REVERSE_ANGLE;
                    mPulse.After_Fastening_Start_abs = 0;       // 역회전에 대한 부분 삭제.
                }

                Cnt_delaytime = 0;
            break;
            case STOP_REVERSE_ANGLE :
                velocity(square_acc(0.0));

                if (w_rpm_abs < 10.0)       
                {
                    Cnt_JumpModeChangeToP2 = 0;
                    mPulse.After_Fastening_Start_abs = 0;       // 역회전에 대한 부분 삭제.
                    fastening_step = THREADTAP;
                }
            break; 
            case THREADTAP:
                if (AdvanceMode == ADV_THREAD_TAPING)
                {
                    // checking finshed ThreadTap.. check torque..
                    if (FlagFasten.FinishedThreadTap)
                    {
                        // checking torque range
                        if ((ThreadTapTorque_real < ThreadTap.Min_Torque_Nm) || (ThreadTapTorque_real > ThreadTap.Max_Torque_Nm))
                            system_err = THREADTAP_MIN_MAX_RANGE_ERROR;
                        else    // ok next step
                        {
                            if (ThreadTap.AngleStartFromThreadTap)
                            {
                                mPulse.After_Fastening_Start_abs = 0;       // clear
                                mPulse.After_Fastening_Start_ForA1 = 0;
                            }

                            // if on, waitting few times.. 
                            velocity(0.0);      // stop
                            Cnt_delaytime++;
                            if (Cnt_delaytime > 40)     // after 10ms restarting
                                fastening_step = FIND_ENGAGING_TORQUE;                            
                        }
                    }
                    else       // if off..
                    {
                        // checking setting
                        if ((ThreadTap.Speed != 0) && (ThreadTap.Finish_Torque_Nm != 0) && (ThreadTap.Max_Torque_Nm != 0))
                        {
                            // 그래프 - Step 설정
                            setGraphStepQueue(GRAPH_STEP_THREAD_TAP);

                            if (ThreadTap.Finish_Torque_Nm > ThreadTap.Min_Torque_Nm)       // error 발생.. 끝나는 토크는 무조건 작아야 함.
                                system_err = THREADTAP_MIN_MAX_RANGE_ERROR;

                            // Min torque 이후 Angle check
                            if(FlagFasten.WithinThreadTapTorque && ThreadTap.AngleLimitFromMinTorque != 0)
                            {
                                if(ThreadTap.ThreadStartAngle == 0)
                                    ThreadTap.ThreadStartAngle = mPulse.After_Fastening_Start_abs;

                                if(mPulse.After_Fastening_Start_abs - ThreadTap.ThreadStartAngle > ThreadTap.AngleLimitFromMinTorque)
                                    system_err = THREADTAP_MIN_MAX_RANGE_ERROR;
                            }
                            
                            velocity(square_acc(ThreadTap.Speed));
                        }
                        else    
                            fastening_step = FIND_ENGAGING_TORQUE;  
                    }
                }
                else
                    fastening_step = FIND_ENGAGING_TORQUE; 
                
                mPulse.After_Fastening_Start_ForA1 = mPulse.After_Fastening_Start_abs;
            break;
            case FIND_ENGAGING_TORQUE:
                if (AdvanceMode == ADV_FING_ENGAGING_TORQUE)
                {
                    if ((EngagingTQ.Speed != 0) && (EngagingTQ.Torque != 0) // check speed,torque setting
                        && ((EngagingTQ.AngleLimit != 0) || (EngagingTQ.TimeLimit != 0)))        // angle or time setting
                    {
                        // 그래프 - Step 설정
                        setGraphStepQueue(GRAPH_STEP_ENGAGING);

                        if (EngagingTQ.AngleLimit != 0)     // angle check
                        {
                            if (EngagingTQ.AngleLimit <= mPulse.After_Fastening_Start_abs)
                                if(system_err == 0)
                                    system_err = FIND_ENGAGING_TORQUE_ERROR;
                        }

                        if (EngagingTQ.TimeLimit != 0)      // time check
                        {
                            Cnt_EngagingTQTimeLimit++;
                            
                            if (EngagingTQ.TimeLimit <= Cnt_EngagingTQTimeLimit)
                                if(system_err == 0)
                                    system_err = FIND_ENGAGING_TORQUE_ERROR;
                        }
                        
                        w_jog = square_acc(EngagingTQ.Speed);
                        velocity(w_jog);

                        if (Flag.FoundEngagingTorque)
                            fastening_step = START_FASTENING;
                    }
                    else
                        fastening_step = START_FASTENING;
                }
                else
                    fastening_step = START_FASTENING;

                mPulse.After_Fastening_Start_ForA1 = mPulse.After_Fastening_Start_abs;
            break;
            case START_FASTENING:
                velocity(square_acc(fasten_speed));

                j = find_reach_initial_speed(); 
                if( j == ON )
                {
                    fastening_step = FIND_SEATING_POINT;
                }
                mPulse.After_Fastening_Start_ForA1 = mPulse.After_Fastening_Start_abs;      
                Flag.SendHostCTqNotComplete = ON;

                if(FreeSpeed.FlagInStep == 0 && (FreeSpeed.Speed == 0 || FreeSpeed.Angle == 0))
                {
                    // 그래프 - Step 설정
                    setGraphStepQueue(GRAPH_STEP_FASTENING_SPEED_START);
                }
            break;
            case FIND_SEATING_POINT:
                w_jog = square_acc(fasten_speed);   // square_acc는  항상 실행한다. 아님.. 정지시 overshoot 현상있음.
                velocity(w_jog);

                if (FlagFasten.FindedSeationPointStep == ON)
                {
                    // 그래프 - Step 설정
                    setGraphStepQueue(GRAPH_STEP_SEATING);

                    // 조건이 in step이면 out step이 on 되어야 다음 step으로 가고, in step이 아니면 바로 이동한다.
                    if (FreeSpeed.FlagInStep)
                    {
                        if (FreeSpeed.FlagOutStep)          // if complete out step
                            fastening_step = REDUCE_SPEED;
                    }
                    else                                    // if reached seating point
                        fastening_step = REDUCE_SPEED;
                }
                mPulse.After_Fastening_Start_ForA1 = mPulse.After_Fastening_Start_abs;
                Flag.SendHostCTqNotComplete = ON;
            break;
            case REDUCE_SPEED:
                velocity(0.0);

                // Reduce speed 구간에서 PWM 감소를 위해 실행
                if(Flag.SendHostCTqNotComplete)
                {
                    i_pi_out = i_pi_out * 0.85;
                    Flag.SendHostCTqNotComplete = OFF;
                }

                if (FlagFasten.ReduceSpeedStep == ON)
                    fastening_step = RAMPUP_TARGET_TORQUE;

                mPulse.After_Fastening_Start_ForA2 = mPulse.After_Fastening_Start_abs - mPulse.After_Fastening_Start_ForA1;
            break;          
            case RAMPUP_TARGET_TORQUE:
                fasten_speed = FastenSeq.RampUpSpeed;
                velocity(square_acc(fasten_speed));

                if (FlagFasten.FindedTargetTorqueStep == ON)
                {
                    // 그래프 - Step 설정
                    setGraphStepQueue(GRAPH_STEP_TQ_COMPLETE);

                    fastening_step = MAINTAIN_TARGET_TORQUE;
                    i_pi_out = i_pi_out * 0.7;
                }

                Flag.SendHostCTqNotComplete = ON;
                mPulse.After_Fastening_Start_ForA2 = mPulse.After_Fastening_Start_abs - mPulse.After_Fastening_Start_ForA1;
            break;
            case MAINTAIN_TARGET_TORQUE:
                // Flag.SendHostCTqNotComplete = OFF;
                fasten_speed = 0.0;
                velocity(0.0);

                // 속도가 너무 빠른 상태에서 가감속 없이 멈출 경우 반력 발생
                if(w_rpm_abs < 1600)
                {
                    // 토크 체결 시 - 속도가 이미 0에 가깝기 때문에 PWM 하락이 천천히 되는것 보완
                    if(Flag.SendHostCTqNotComplete)
                        i_pi_out = i_pi_out * 0.8;
                }
                
                Flag.SendHostCTqNotComplete = OFF;


                if (fABS(w_rad) < 50.0 || fABS(i_pi_out) < 400) 
                {
                    fastening_step      = LITTLE_REWIND;
                    Cnt_RewindAngle     = 0;
                    FlagFasten.ReverseAngleCompleteStep = OFF;

                    // Velocity Loop
                    w_err = 0.0;
                    // w_err_1 = 0.0;
                    tq_cmd = 0.0;
                    // w_cmd_flt = 0.0;
                    w_pi_out = 0.0;
                    vin_out = 0.0;
                    mPulse.After_Fastening_Start = mPulse.After_Fastening_Start_abs;
                }

                mPulse.After_Fastening_Start_ForA2 = mPulse.After_Fastening_Start_abs - mPulse.After_Fastening_Start_ForA1;
            break;
            case LITTLE_REWIND:
                Flag.Reached_LITTLE_REWIND = ON;        // 이곳이 이하 step에서 error 발생시 torque값을 올려 준다.

                // Angle after torque up 사용하지 않을 때 각도 반영
                if((AngleAfterTqUp.Speed == 0) || (AngleAfterTqUp.Angle == 0))
                {
                    mPulse.After_Fastening_Start_ForA2 = mPulse.After_Fastening_Start_abs - mPulse.After_Fastening_Start_ForA1;
                }

                if ((AngleAfterTqUp.Speed != 0) && (AngleAfterTqUp.Angle != 0) && !FlagFasten.ReverseAngleCompleteStep)
                {
                    // 그래프 - Step 설정
                    setGraphStepQueue(GRAPH_STEP_ANGLE_AFTER);

                    FlagFasten.OverTorqueErrorCheck = ON;   // max speed에서 angle contorl로 stop시 brack torque가 높아서 에러 발생을 방지 하기 위한 flag.
                    if (AngleAfterTqUp.Direction == 0)      // foward 
                        velocity(square_acc((float)(AngleAfterTqUp.Speed)));
                    else                                    // reverse
                        velocity(square_acc((float)(-AngleAfterTqUp.Speed)));

                    Cnt_RewindAngle += m_pulse_diff_abs;

                    if (labs(Cnt_RewindAngle) > AngleAfterTqUp.Angle)
                        FlagFasten.ReverseAngleCompleteStep = ON;
                }
                else
                {
                    FlagFasten.OverTorqueErrorCheck = OFF;
                    velocity(0.0);

                    // 최종 토크 값 반영
                    if (Flag_JumpCompleteAngleControl)
                    {
                        // ACTM
                        Torque_cnv.fastenComplete = Fasten_Real_Torque;
                    }
                    else
                    {
                        // TCAM - Nm를 각 unit으로 변환.
                        Torque_cnv.fastenComplete = tq_real_avg_for_holding_Nm * TorqueMetric[UNIT_Nm][Para[UNIT].val];       // 최종 토크

                        // Clamp 모드
                        if ((AdvanceMode == ADV_CLAMP_TQ_CONTROL))       
                        {
                            // 체결된 결과 토크 계산
                            if(Torque_cnv.seating && Clamping.replace_target_active)
                            {
                                Torque_cnv.clamp = Torque_cnv.fastenComplete - Torque_cnv.seating;          // 최종 토크에서 seating을 빼면 clemp..
                            }
                        }
                        // Prevailing 적용
                        else if ((AdvanceMode == ADV_PREVAIL_TQ_CONTROL))
                        {
                            // 체결된 결과 토크 계산
                            if (Clamping.clamp_torque_Nm && Prevailing.compensationActive)
                            {
                                Torque_cnv.clamp = Torque_cnv.fastenComplete - Torque_cnv.prevailing;       // 최종 토크에서 prevailing을 빼면 clemp..
                            }
                        }
                        else
                        {
                            Torque_cnv.clamp = 0.0;
                            Torque_cnv.prevailing = 0.0;
                        }
                    }

                    if (FastenSeq.TcamActm == 0)    // torque control
                    {
                        if (((AdvanceMode == ADV_CLAMP_TQ_CONTROL && Clamping.replace_target_active) || (AdvanceMode == ADV_PREVAIL_TQ_CONTROL && Prevailing.compensationActive)))       // at seating  이고 clamp 토크가 있으면 토크 변경.
                        {
                            if (Clamping.min_clamp_torque != 0)
                            {
                                if (fABS(Torque_cnv.clamp) < Clamping.min_clamp_torque)
                                    system_err = CLAMP_TQ_ERROR;
                            }

                            if (Clamping.max_clamp_torque != 0)
                            {
                                if (fABS(Torque_cnv.clamp) > Clamping.max_clamp_torque)
                                    system_err = CLAMP_TQ_ERROR;
                            }
                        }                        
                        else if (getParaVal(TORQUE_MIN_MAX1_H + ((Fastening_number-1)*PRESET_SIZE)) != 0)    // if 0 is not check.
                        {                   
                            if ((fABS(Torque_cnv.fastenComplete) > P_ConvertedTorqueMaxLimit) || (fABS(Torque_cnv.fastenComplete) < P_ConvertedTorqueMinLimit))
                            {
                                if(system_err == 0)
                                    system_err = C_TORQUE_ERROR;
                            }
                        }
                    }
                    else        // angle control
                    {
                        // Torque_cnv.fastenComplete value가 - 일때가 있다.. angle stop시..
                        if ((fABS(Torque_cnv.fastenComplete) > P_ConvertedTorqueMaxLimit) || (fABS(Torque_cnv.fastenComplete) < P_ConvertedTorqueMinLimit))
                        {
                            if(system_err == 0)
                                system_err = C_TORQUE_ERROR;
                        }              
                    }

                    Torque_cnv.fastenComplete_dsp_comp = Torque_cnv.fastenComplete * inv_Torque_ConvertValue;       // 최대 값을 얻는다.

                    Flag.TorqueUpCompleteOut = ON;
                    Flag.FasteningStopAlarm = OFF;
                    MinMax_Angle_process();             // angle check 
                    After_fasten_init_process();
                    AutoSettingSpeedAndSetting();       // Auto customizing
                }
            break;
            default:    tq_cmd = 0.0;
                break;
        }
    }

    // check max angle over limit
    if ((fastening_step > STOP_REVERSE_ANGLE) && (fastening_step != LITTLE_REWIND))
    {   
        if(FastenSeq.SnugTQ_Nm == 0)   // 전체 angle check
        {
            if ((FastenSeq.MaxAngle != 0) && (mPulse.After_Fastening_Start_ForA1 != 0))
            {
                counter_diff = mPulse.After_Fastening_Start_abs;
                if (FastenSeq.MaxAngle < counter_diff)
                {
                    if(system_err == 0)
                        system_err = FASTENING_TURN_MAX_ERROR;
                }
            }
        }
        else    // check after snug torque
        {
            if (FlagFasten.SnugTorque)
            {
                counter_diff = mPulse.After_Fastening_Start_abs - mPulse.StartSnugTq;
                if (FastenSeq.MaxAngle < counter_diff)
                {
                    if(system_err == 0)
                        system_err = FASTENING_TURN_MAX_ERROR;
                }
            }
        }

        // A2 max angle check
        check_afterSeating_error_max(mPulse.After_Fastening_Start_ForA2);
    }

    TotalFasteningTime++;
    Fastening_time = TotalFasteningTime / 10;    // change ms unit (/ (1.0 / TS_VEL) / 1000.0)
    DisplayAngle_forLCD = (uint16_t)((float)(mPulse.After_Fastening_Start_abs) * MathConv.AngleConv_mPulseToDeg);

    if(TotalFasteningTime > FastenSeq.FasteningTimeLimit)
    {
        Flag.Stopping = ON;
        
        if(system_err == 0)
            system_err = FASTENING_TIMEOUT;
    }

	// data 를 항상 올려 준다. maintain target step에서는 제외..
	if (Flag.SendHostCTqNotComplete && !Flag_JumpCompleteAngleControl) 	// torque 값 표기 step 정의.. 체결 이외에서는 skip
	{
		if ((fastening_step != MAINTAIN_TARGET_TORQUE) && (fastening_step != LITTLE_REWIND))
		{
            tq_real_avg = RealTq_fromSensor_reflectedSettingUnit;

            if (fABS(tq_real_avg_old) < fABS(tq_real_avg))
            {
                Fasten_Real_Torque = tq_real_avg * screw_type;
                Torque_cnv.fastenComplete = Fasten_Real_Torque;
                Torque_cnv.fastenComplete_dsp_comp = Torque_cnv.fastenComplete * inv_Torque_ConvertValue;
                tq_real_avg_old = tq_real_avg;
            }
		}	
	}

    // Make realtime data
    if(flagRealtimeMake)
    {
        flagRealtimeMake = OFF;
        // putRealtimeData(&realPollStruct);
    }
}

void Loosening(void) 
{   
    float w_jog;
    
    if(TotalLooseningTimeReal <= 0)
    {
        Initial_Fastening_Parameter();
        Gain_Setting();
    }

    TotalLooseningTimeReal++;
    Disp_Loosening_time = (uint16_t)((float)TotalLooseningTimeReal / ((1.0 / TS_VEL) / 1000.0));		// 1ms 단위

    mPulse.After_Fastening_Start_abs += m_pulse_diff_abs;
    mPulse.DisplayLoosening = mPulse.After_Fastening_Start_abs;
    DisplayAngle_forLCD = (uint16_t)((float)(mPulse.After_Fastening_Start_abs) * MathConv.AngleConv_mPulseToDeg);

    if (TotalLooseningTimeReal > LooseningSeq.TimeLimit) 
    {
        Flag.Stopping = ON;

        if(system_err == 0)
            system_err = LOOSENING_TIMEOUT;
    }

    if (Flag.Stopping == OFF)
    {
        if ((Para[CROWFOOT_ENABLE].val) && (getParaVal(CROWFOOT_REVERSE_TORQUE_H)) && Para[CROWFOOT_REVERSE_SPEED].val)     // if no setting torque, no activeted
        {
            w_jog = square_acc(LooseningSeq.Crowfoot_speed);
        }
        else
            w_jog = square_acc(LooseningSeq.Speed);
            
        velocity(w_jog);

        if( Cnt_LooseningTime > LooseningSeq.StallTimeLimit )
        {
            if(system_err == 0)
                system_err    = OVER_TIME_DURING_LOOSENING;
                    
            tq_cmd            = 0.0;
            w_cmd             = 0.0;
            Cnt_LooseningTime = 0;
            Flag.InternalRun  = OFF;
            Flag.ExternalRun  = OFF;
            Flag.RunByMult    = OFF;
            Flag.Stopping     = ON;
        }
    }

    if (Flag.Multi_Mode && Flag.RunningMultisequence)
    {
        if (ms.LooseningAngle <= mPulse.DisplayLoosening)
        {
            Flag.InternalRun = Flag.ExternalRun = Flag.RunByMult = OFF;
            Flag.ReadMSData  = OFF;

            StatusOutput = rNONE;
            // putRealtimeData(&realPollStruct);
        }
    }
}

int  find_reach_initial_speed(void)
{
    float tgSpeed;
    static int Flag_ReachedInitialSpeedDelay = 0;
    static int cnt_40 = 0;

    tgSpeed = (float)FastenSeq.RpmOfLoad * deceleration_ratio * 0.95;   
    
    if (FlagFasten.ReachedInitialSpeed == OFF)
    {
        if (w_rpm_abs > tgSpeed)    
            Flag_ReachedInitialSpeedDelay = ON;
    
        cnt_40++;

        if (Flag_ReachedInitialSpeedDelay == ON)
        {
            if (cnt_40 >= 40)       // 100ms
            { 
                FlagFasten.ReachedInitialSpeed = ON;                
                cnt_40 =    0;
                Flag_ReachedInitialSpeedDelay = OFF;

                return ON;
            }
        }
        
        return OFF;
    }
    else    
        return OFF;
}

void MinMax_Angle_process(void)
{
    long diff_count;

    if(FastenSeq.SnugTQ_Nm != 0)   // check snug angle
        diff_count =  mPulse.RealSnugTq;
    else
        diff_count =  mPulse.After_Fastening_Start_abs;         // 전체 각도

    if (FastenSeq.MinAngle != 0)    // min 값이 setting 되어 있으면.. if 0 is no check
    {
        if (FastenSeq.MinAngle > diff_count)
        {
            if(system_err == 0)
                system_err = FASTENING_TURN_MIN_ERROR;
        }
    }

    // A2 Angle limit 관리
    check_afterSeating_error_min(mPulse.After_Fastening_Start_ForA2);

	if (system_err == 0)
	{
		if (!Flag.Multi_Mode)	// multi mode에서는 count를 하지 않는다.
			ScrewCount_process();

		// fastening은 항상 출력을 준다.
        flagRealtimeMake = ON;
		StatusOutput = rFASTENING_OK;
        Flag.FasteningCompleteOut = ON;
        // putRealtimeData(&realPollStruct);     
        Flag.ReadMSData   = OFF;
        FlagFasten.BackCountForTouch = OFF;     // -1 Back count button
        //Flag.Buzzer = ON;   // for buzzer on
    }
}

void After_fasten_init_process(void)
{
    Flag.InternalRun = OFF;
    Flag.ExternalRun = OFF;
    Flag.RunByMult   = OFF;
}

void ChangeSpeedAcc(void)       // 각 mode에 맞는 acc 선택
{
    switch ( fastening_step )
    {
        case REVOLUTION_START:      Delta_dcc = Delta_acc = FreeReverse.SpeedAcc;
        break;
        case STOP_REVERSE_ANGLE :   Delta_dcc = Delta_acc = FreeReverse.SpeedAcc;
        break; 
        case THREADTAP:             Delta_dcc = Delta_acc = ThreadTap.SpeedAcc;
        break;
        case FIND_ENGAGING_TORQUE:  Delta_dcc = Delta_acc = EngagingTQ.SpeedAcc;
        break;
        case START_FASTENING:
        case FIND_SEATING_POINT:
            if (FreeSpeed.FlagInStep)   // if free speed
            {
                if(FreeSpeed.FlagOutStep)       // if outstep on, change fasteing step
                {
                    Delta_acc = FastenSeq.SpeedAcc;
                    Delta_dcc = FastenSeq.SpeedDcc;
                }
                else
                {
                    Delta_acc = FreeSpeed.SpeedAcc;
                    Delta_dcc = FreeSpeed.SpeedDcc;
                }
            }
            else
            {
                Delta_acc = FastenSeq.SpeedAcc;
                Delta_dcc = FastenSeq.SpeedDcc;
            }
        break;
        case REDUCE_SPEED:
                Delta_acc = FastenSeq.SpeedAcc;
                Delta_dcc = FastenSeq.SpeedDcc * 2;     
        break;          
        case RAMPUP_TARGET_TORQUE:
                Delta_acc = FastenSeq.SpeedAccForRampUp;
                Delta_dcc = FastenSeq.SpeedAccForRampUp;        
        break; 
        case MAINTAIN_TARGET_TORQUE:
                Delta_acc = FastenSeq.SpeedAccForRampUp;
                Delta_dcc = FastenSeq.SpeedAccForRampUp;        
        break;
        case LITTLE_REWIND:
            if (AngleAfterTqUp.SpeedAcc == 0)
            {
                Delta_acc = FastenSeq.SpeedAcc;
                Delta_dcc = FastenSeq.SpeedDcc;     
            }
            else
                Delta_dcc = Delta_acc = AngleAfterTqUp.SpeedAcc;        
        break;
        default:    
        break;
    }   
}

void Initial_Fastening_Parameter(void)
{
    mPulse.After_Fastening_Start_abs = 0;
    mPulse.After_Fastening_Start_ForA1 = mPulse.After_Fastening_Start_ForA2 = 0;
    mPulse.StartSnugTq = 0;
    mPulse.After_Fastening_Start = 0;
    mPulse.RealSnugTq = 0;
    
    FlagFasten.SnugTorque = OFF;
    FlagFasten.WithinPrevalingTorque = OFF;
    FlagFasten.FinishedThreadTap      = OFF;
    FlagFasten.WithinThreadTapTorque = OFF;
    FlagFasten.FinshedPrevaling = OFF;
    FlagFasten.ReachedMaxAngle = OFF;
    FlagFasten.ReverseAngleCompleteStep = OFF;
    FlagFasten.OverTorqueErrorCheck = OFF;
    FlagFasten.CompleteMultisequence = OFF;

    Torque_cnv.fastenComplete = 0.0;
    Torque_cnv.fastenComplete_dsp_comp = 0.;
    tq_real_avg = tq_real_avg_old = 0.0;
    Fastening_time = 0;
    TotalFasteningTime = 0;

    AtSeating.sdx = 0;
    AtSeating.sdxOld = 1;
    AtSeating.finalGradient = 0;
    AtSeating.currentGradient_Nm = 0;
    memset(AtSeating.mpulse, 0, sizeof(AtSeating.mpulse));
    memset(AtSeating.adc, 0, sizeof(AtSeating.adc));

    // Gradient 값 초기화
    for(uint8_t i=0; i < 10; i++)
    {
        gradientAdcAverage(0.0);
        gradientMpulseAverage(0.0);
    }

	Prevailing.detectAngle = 0;
    Prevailing.finished_prevailing = OFF;
    Prevailing.sum_tq = 0.0;
    Prevailing.cnt_loop = 0;
    Prevailing.result_of_prevailing_torque_Nm = 0.0;
    Prevailing.compensation_torque_Nm = 0;

    // Initilize advanced value
    Torque_cnv.clamp        = 0;
    Torque_cnv.prevailing   = 0;
    Torque_cnv.snug         = 0;
    Torque_cnv.seating      = 0;

    Flag.FoundEngagingTorque = OFF;
    Flag.Reached_LITTLE_REWIND = OFF;
    Flag.SendHostCTqNotComplete= OFF;
    Flag_JumpCompleteAngleControl = OFF;
    Flag_msStepFastenStepIN = OFF;
    flagRealtimeMake = OFF;

    FreeSpeed.FlagInStep = OFF;
    FreeSpeed.FlagOutStep = OFF;
    FreeSpeed.FlagInOut = OFF;

    Tq_SensorValue_Avg_max = 32767;
    DisplayAngle_forLCD = 0;
    loosening_PickTorque = 0.0;
    ThreadTapTorque_real = 0.0;
    ThreadTap.ThreadStartAngle = 0;
    
    Cnt_HoldingTime = 0;
    Cnt_EngagingTQTimeLimit = 0;
    
    tq_real_avg_for_holding_Nm = 0.0;

    // Graph
    clearGraphStepQueue();

    Initial_Fastening_Parameter_After_ReverseTq();
    return;
}

void Initial_Fastening_Parameter_After_ReverseTq(void)
{
    fastening_step = REVOLUTION_START;

    FlagFasten.ReduceSpeedStep = OFF;
    FlagFasten.FindedSeationPointStep = OFF;
    FlagFasten.FindedTargetTorqueStep = OFF;

    Cnt_JumpModeChangeToP2 = 0;
    Cnt_RewindAngle = 0;

    vin_out = 0.0;
    // w_err = w_err_1 = 0.0;
    w_err = 0.0;
}


float Reverse_TorqueConversion(float cc, int flag)
{
    float Real_tq=0.0;

    if (Para[UNIT].val == Kgfcm)
        Real_tq = cc * MathConv.TqConv_kfgcm; 
    else if (Para[UNIT].val == Kgfm)
        Real_tq = cc * MathConv.TqConv_kgfm;
    else if (Para[UNIT].val == Nm)          // Nm 0.01
        Real_tq = cc * MathConv.TqConv_Nm;
    else if (Para[UNIT].val == cNm)         // cNm 0.01
        Real_tq = cc * MathConv.TqConv_cNm;
    else if (Para[UNIT].val == lbfin)       // Ibf.in 0.01
        Real_tq = cc * MathConv.TqConv_Ibfin;   
    else if (Para[UNIT].val == Ozfin)       // ozf.in 0.01
        Real_tq = cc * MathConv.TqConv_ozfin;
    else if (Para[UNIT].val == lbfft)       // lbfft 0.01
        Real_tq = cc * MathConv.TqConv_Ibfft;

    return Real_tq * 1.000002;
}

void ScrewCount_process(void)
{
    if (ScrewCountValue != 0)
        --ScrewCountValue;			// screw counter
    
    if (ScrewCountValue <= 0)
    {
        if (Para[AUTO_LOCK].val)
        {
            Flag.LockCommand = 1;
            ScrewCountValue = 0;
        }
        // else
        //     ScrewCountValue = Para[COUNT_SCREW].val;
    }
}



/**
 * @brief 역방향 모드 : Screw count -1 동작 제어
 */
void screwCountLooseningD1(void)
{
    // Loosening : Screw -1 동작
    if(Flag.ScrewDecrLoosening)
    {
        if(FlagFasten.ScrewCountProcessStatus == PROGRESS && !FlagFasten.BackCountForTouch)
        {
            // if(Para[MODEL_SELECT].val)
            // {
            //     // Model 모드
            //     if(ScrewCountValue < Model.CurrentCountSetting)
            //     {
            //         StatusOutput = rScrewCount_D1;
            //         ScrewCountValue++;
            //     }
            // }
            // else
            // {
                // Preset 모드
                if(ScrewCountValue < Para[COUNT_SCREW].val)
                {
                    StatusOutput = rScrewCount_D1;
                    ScrewCountValue++;
                }
            // }

            FlagFasten.BackCountForTouch = ON;
        }
        
        Flag.ScrewDecrLoosening = OFF;
    }
}


void ms_process(void)
{
    int setdata = 0;
    static int ms_data=0, ms_command=0;

    if (Flag.Multi_Mode && Flag.Multi_Start)
    {
        if (ms.StepNumber > 10)
        {
            // Error
            ms.StepNumber = 0;
            if(system_err == 0)
                system_err = MS_PGM_ERROR;
        }
        else
        {
            if (Flag.ReadMSData == OFF)
            {
                ms.StepNumber++;
                Flag.ReadMSData = ON;   // step을 실행하기 전에 기초 data를 가져왔음을 알리는 flag..

                if (ms.PGNumber == 1)
                {
                    setdata = Para[MS_PG1 + ms.StepNumber -1].val;
                }
                else if (ms.PGNumber == 2)
                {
                    setdata = Para[MS_PG11 + ms.StepNumber -1].val;
                }
                    
                ms_command = setdata / 1000;
                ms_data    = setdata % 1000;    
                
                if (((ms_command > 20) || (ms_data > 999)) && (system_err == 0))
                    system_err = MS_PGM_ERROR;
                
                if (ms.StepNumber == 1)
                    ms_start();
            }
            else if (Flag.RunningMultisequence)     // 실제 step을 실행하고 있음을 알리는 flag..
            {
                switch (ms_command)
                {
                    case 1:     ms_fastening(ms_data);
                    break;
                    case 2:     ms_end();   
                    break;
                    case 3:     ms_delay(ms_data);  
                    break;
                    case 4:     ms_changeTorque(ms_data);   
                    break;
                    case 5:     ms_loosening(ms_data);  
                    break;
                    case 6:     ms_jump(ms_data);   
                    break;
                    case 7:     ms_countValueA(ms_data);    
                    break;
                    case 8:     ms_subIfA();    
                    break;
                    default:
                    break;
                }
            }
        }
    }
    else
    {
        ms.StepNumber   = 0;
        Flag.ReadMSData = OFF;
        Flag.RunningMultisequence = OFF;
        Flag.RunByMult  = OFF;              // motor run
        multiStr.flStatusOld = Flag.ControlFL;
    }
}

void ms_countValueA(int count)
{
    if (Flag.RunningMultisequence)
    {
        ms.LoopCount = count;

        if (ms.LoopCount < 0)   ms.LoopCount = 0;

        Flag.ReadMSData = OFF;
    }
}

void ms_subIfA(void)
{
    if (Flag.RunningMultisequence)
    {
        ms.LoopCount--;

        if (ms.LoopCount < 0)   ms.LoopCount = 0;

        if (ms.LoopCount == 0)          //
        {
            ms.StepNumber++;
        }
        else
        {
            
        }
        Flag.ReadMSData = OFF;
    }
}

void ms_jump(int step)
{
    if (Flag.RunningMultisequence)
    {
        if(step == 0)
        {
            system_err = MS_PGM_ERROR;
            return;
        }

        ms.StepNumber   = step - 1;
        Flag.ReadMSData = OFF;
    }
}

void ms_loosening(int angle)        // angle 0.1 * 10
{
    if (Flag.RunningMultisequence)
    {
        ms.LooseningAngle = (long)((float)angle * 0.1 * (float)Motor.num_pulse * deceleration_ratio);
        Flag.ControlFL = LOOSENING_CONTROL;
        Flag.RunByMult = ON;
    }
}

void ms_delay(int time)     // 10ms
{
    static int ms_Wait_Cnt=0;
    static uint32_t timeout;
    
    // Check delay step restart
    if(HAL_GetTick() - timeout > 50 && HAL_GetTick() > 50)
        ms_Wait_Cnt = 0;

    timeout = HAL_GetTick();


    if (Flag.RunningMultisequence)
    {
        if (ms_Wait_Cnt > time)
        {
            Flag.ReadMSData = OFF;
            ms_Wait_Cnt = 0;
        }
        else
            ms_Wait_Cnt++;
    }
}

void ms_start()
{
    Flag.RunningMultisequence = ON;
}

void ms_fastening(int no)
{
    if (Flag.RunningMultisequence)
    {
        if(Flag.Run == OFF)
        {
            if(no == 0)
            {
                system_err = MS_PGM_ERROR;
                return;
            }

            Fastening_number = no;
            Set_Torque_variable();

            Flag.ControlFL = FASTENING_CONTROL;
            Flag.RunByMult = ON;
            Flag_msStepFastenStepIN = ON;
        }
    }
}

void ms_changeTorque(int no)
{
    if (Flag.RunningMultisequence)
    {
        if(no == 0)
        {
            system_err = MS_PGM_ERROR;
            return;
        }

        Fastening_number = no;

        Set_Torque_variable();
        Flag.ReadMSData = OFF;
    }
}

void ms_end()
{
    if (Flag.RunningMultisequence && (system_err == 0))
    {
        // Make graph realtime data (Realtime과 같은 값이 나오기 위해 미리 Graph realtime을 만든다)

        // 체결 스크류 감소
        ScrewCount_process();

        // 최종 Multi 종료 Realtime 체결 결과 값
        StatusOutput = rFASTENING_OK;
        // ClearSendData_Init(ON);
        putRealtimeData(&realPollStruct);      

        Flag.Multi_Start = OFF;
        Flag.FasteningCompleteOut = ON;
        Flag.RunningMultisequence = OFF;
        FlagFasten.CompleteMultisequence = ON;
    }
}


/**
 * @brief Multi sequence - 동작 중 중간에 멈췄을 때 Realtime 데이터를 생성한다
 */
void ms_MakeRealtimeAtStop(void)
{
    static uint8_t runEdge[2];

    runEdge[0] = runEdge[1];
    runEdge[1] = Flag.Multi_Start;

    if(runEdge[0] > runEdge[1])
    {
        if(system_err)
        {
            // 기존의 에러 상태 코드 유지
        }
        else if(FlagFasten.CompleteMultisequence == OFF)
        {
            // Multi sequence 체결이 중간에 멈쳤을 경우 etc로 출력
            StatusOutput = rNONE;
        }
            
        // Multi sequence가 중간에 멈춘 경우 realtime 생성
        if(FlagFasten.CompleteMultisequence == OFF)
            putRealtimeData(&realPollStruct);

        // Reset F/L
        Flag.ControlFL = multiStr.flStatusOld;
    }
}

//=============================================================================

/* USER CODE END 1 */
