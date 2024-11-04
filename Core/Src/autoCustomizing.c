/**
  ******************************************************************************
  * @file    autoCusomizing.c
  * @author  jun
  * @brief   Source file of auto customizing.
  * @note    체결의 속도 등을 자동으로 customizing 해준다.
  ******************************************************************************
  */

/* Include ---------------------------------------------------------------------*/
#include "autoCustomizing.h"
#include "common.h"
#include "variable.h"
#include "main_bg.h"
#include "backup.h"
#include <stdlib.h>


/* Variable ---------------------------------------------------------------------*/
auto_setting AutoSetting;


/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief Auto customizing의 파라미터를 저장한다.
 */
void SaveAutoSettingParaValue(void)
{
	uint16_t j;
	
    // Check 저장 Flag
    if(AutoSetting.FlagSavePara)
    {
        if (AutoSetting.FlagSetting && AutoSetting.FlagFrSpeed)
        {
            if(AutoSetting.CurrnetSpeed == AutoSetting.CurrnetFSpeed)	// 같은면 target speed 만 수정..
            {
                AutoSetting.CurrnetFSpeed = 0;
                AutoSetting.CurrentFAngle = 0;
            }
        }

        // Preset 만큼 저장 index offset 찾기
        j = (Processing_number-1) * PRESET_SIZE;

        // Save Speed
        if (AutoSetting.CurrnetSpeed != 0)
        {
            ParameterWriteToMemory(FASTEN_SPEED1+j, AutoSetting.CurrnetSpeed);
            Para[FASTEN_SPEED1+j].val = AutoSetting.CurrnetSpeed;
        }

        // Save Seating point
        if (AutoSetting.CurrentSettingPoint != 0)
        {
            ParameterWriteToMemory(FASTEN_SEATTING_POINT_RATE1+j, AutoSetting.CurrentSettingPoint);
            Para[FASTEN_SEATTING_POINT_RATE1+j].val = AutoSetting.CurrentSettingPoint;
        }

        // Save Free speed
        if (AutoSetting.CurrnetFSpeed != 0)
        {
            ParameterWriteToMemory(FREE_FASTEN_SPEED1+j, AutoSetting.CurrnetFSpeed);
            Para[FREE_FASTEN_SPEED1+j].val = AutoSetting.CurrnetFSpeed;
        }

        // Save Free angle
        if (AutoSetting.CurrentFAngle != 0)
        {
            ParameterWriteToMemory(FREE_FASTEN_ANGLE1+j, AutoSetting.CurrentFAngle);
            Para[FREE_FASTEN_ANGLE1+j].val = AutoSetting.CurrentFAngle;
        }

        AutoSetting.FlagSavePara = OFF;
    }
}


/**
 * @brief Auto customizing 동작 모드를 선택
 * 
 * @param type : 0 = Soft joint, 1 = Hard joint, 2 = Start auto customizing, 3 = Save auto customizing setting
 * @param data : ON, OFF 데이터 
 */
void selectAutoCustomMode(uint8_t type, uint16_t data)
{
    switch (type)
    {
        case SOFT_JOINT_MODE:
            // Soft joint mode 설정
            if (!AutoSetting.FlagStart)
			{
				AutoSetting.FlagTgSpeed = data;
				AutoSetting.FlagSetting = data;
				AutoSetting.FlagFrSpeed = OFF;
			}
            break;

        case HARD_JOINT_MODE:
            // Hard joint mode 설정
            if (!AutoSetting.FlagStart)
			{
				AutoSetting.FlagTgSpeed = OFF;
				AutoSetting.FlagSetting = OFF;
				AutoSetting.FlagFrSpeed = data;
			}
            break;

        case START_AUTO_CUSTOMIZING:
            // Auto customizing 시작
            if (AutoSetting.FlagTgSpeed || AutoSetting.FlagFrSpeed)
				AutoSetting.FlagStart = data;

            // Auto customizing 정지
            if(data == OFF)
            {
				AutoSetting.FlagStart = data;
            }

			// Start 버튼 클릭시 초기화
			AutoSetting.CurrentA1 = 0;
			AutoSetting.CurrentA2 = 0;
			AutoSetting.CurrnetSpeed = 0;
			AutoSetting.CurrentSettingPoint = 0;
			AutoSetting.CurrnetFSpeed = 0;
			AutoSetting.CurrentFAngle = 0;

			Set_Torque_variable();
            break;

        case SAVE_AUTO_CUSTOMIZING:
            // Auto customizing 설정 값 저장 (0 이 아닌 것이 있으면 저장)
			if (AutoSetting.CurrnetSpeed || AutoSetting.CurrentSettingPoint || AutoSetting.CurrnetFSpeed || AutoSetting.CurrentFAngle)
			{
				AutoSetting.FlagSavePara = ON;
			}
            break;
        default:
            break;
    }
}


/**
 * @brief Auto customizing 계산 수행
 */
void AutoSettingSpeedAndSetting(void)
{
    // Check auto customizing 시작
    if (AutoSetting.FlagStart)
    {
        // A1, A2 각도 계산
        AutoSetting.CurrentA1 = (uint16_t)((float)(mPulse.After_Fastening_Start_ForA1) * MathConv.AngleConv_mPulseToDeg);
        AutoSetting.CurrentA2 = (uint16_t)((float)(mPulse.After_Fastening_Start_ForA2) * MathConv.AngleConv_mPulseToDeg);
        
        // test 결과 A2가 10 이하인 경우는 hard joint 이다.

        // Change Seating point - Soft joint
        if (AutoSetting.FlagSetting)
        {
            if (AutoSetting.CurrentA2 > 60)     // 60도 이상 이면.. 10% 씩 값을 올린다. 90% 이상이면.. 90%
            {
                FastenSeq.SeatingTQ_Nm = FastenSeq.SeatingTQ_Nm * 1.1;    // 10% up
                if ((FastenSeq.TargetTorque_Nm * 0.95) < FastenSeq.SeatingTQ_Nm)
                    FastenSeq.SeatingTQ_Nm = FastenSeq.TargetTorque_Nm * 0.95;
            }
            else if (AutoSetting.CurrentA2 < 10)    // if angle is under 15, reduce setting point
            {
                FastenSeq.SeatingTQ_Nm = FastenSeq.SeatingTQ_Nm * 0.95;   // 5% down
                if ((FastenSeq.TargetTorque_Nm * 0.4) > FastenSeq.SeatingTQ_Nm)
                    FastenSeq.SeatingTQ_Nm = FastenSeq.TargetTorque_Nm * 0.4;
            }
        }

        // Change Target speed - Soft, Hard joint
        if (AutoSetting.FlagTgSpeed)
        {
            if (AutoSetting.CurrentA2 > 30)     // 30도 이상 이면.. 10% 씩 값을 올린다. 95% 이상이면.. 95%
            {
                FastenSeq.TargetSpeed = FastenSeq.TargetSpeed * 1.1;    // 10% up
                if ((labs((long)(FastenSeq.MaxSpeed * 0.95))) < labs((long)(FastenSeq.TargetSpeed)))
                    FastenSeq.TargetSpeed = FastenSeq.MaxSpeed * 0.95;
            }
            else if (AutoSetting.CurrentA2 < 10)    // if angle is under 15, reduce speed
            {
                FastenSeq.TargetSpeed = FastenSeq.TargetSpeed * 0.95;   // 5% down
                if ((labs(AutoSetting.OldSpeed)) > labs((long)(FastenSeq.TargetSpeed)))
                    FastenSeq.TargetSpeed = AutoSetting.OldSpeed;
            }

            FastenSeq.SpeedAcc = fABS(FastenSeq.TargetSpeed) / (AutoSetting.OldSumAcc * 2);     // change acc
        }
        
        // Change Free speed - Hard joint
        if (AutoSetting.FlagFrSpeed)
        {
            // Angle setting
            AutoSetting.OldFreeAngle = (long)(AutoSetting.CurrentA1 * 0.7);
            FreeSpeed.Angle = (long)((float)AutoSetting.OldFreeAngle * (float)Motor.num_pulse * (float)deceleration_ratio / 360);   // Free angle = A1 70%

            // Speed setting
            FreeSpeed.Speed    = FastenSeq.MaxSpeed * 0.90;                                     // Free speed = 최대 속도의 90%
            FreeSpeed.SpeedAcc = fABS(FreeSpeed.Speed) / (AutoSetting.OldSumAcc * 2);           // change ACC
            FreeSpeed.SpeedDcc = fABS(FreeSpeed.Speed) / (Info_DrvModel[dset[4]].acc_dcc * 2);
        }

        // convert real for display and setting
        AutoSetting.CurrnetSpeed  = (uint16_t)(labs((long)(FastenSeq.TargetSpeed)) * MathConv.ConvSpeed);
        AutoSetting.CurrentSettingPoint = (uint16_t)(FastenSeq.SeatingTQ_Nm / FastenSeq.TargetTorque_Nm * 100.00005);
        AutoSetting.CurrnetFSpeed = (uint16_t)(labs((long)FreeSpeed.Speed) * MathConv.ConvSpeed);
        AutoSetting.CurrentFAngle = (uint16_t)(FreeSpeed.Angle / (float)Motor.num_pulse / (float)deceleration_ratio * 360.);
    }
}
