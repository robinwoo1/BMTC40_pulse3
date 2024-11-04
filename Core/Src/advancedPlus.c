/* Includes ------------------------------------------------------------------*/
#include "advancedPlus.h"
#include "variable.h"
#include "common.h"
#include "torque.h"
#include "backup.h"
#include "graphMonitoring.h"


/* Private variables ---------------------------------------------------------*/
atseating       AtSeating;
clamping        Clamping;
prevailingtq    Prevailing;
threadTap       ThreadTap;
engagingtq      EngagingTQ;
angleAfterTq    AngleAfterTqUp;
torque_cnv      Torque_cnv;
rundownTq 	    RundownTq;

// Advacned mode
uint16_t AdvanceMode;


/* Extern global variable ---------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/* BACK UP 관련 ---------------------------------------------------------------------------------------------------------*/

/**
 * @brief Advnaced에 관련된 파라미터 min, max 설정
 * 
 * @param adv_Pset_number : Advanced preset 번호
 */
void Init_Para_limit_forAdvence(uint8_t adv_Pset_number, float tq_upper_limit_unit, uint16_t speed_upper_limit)
{
    int i, j;
    uint16_t aMode;

    // Check preset limit
    if(adv_Pset_number > PRESET_INDEX_SIZE)
        adv_Pset_number = 1;

    // Preset number
    i = adv_Pset_number;
    aMode = Para[ADVANCED_MODE1 + ((i-1)*ADV_PARA_SIZE)].val;       // read setting value

    // Advanced 파라미터 Min, Max default 값으로 초기화
    setParaMax(ADVANCED_MODE1 + ((i-1)*ADV_PARA_SIZE), 10);

    // Advanced parameter 0으로 초기화
    for (j=0; j < ADVANCED_PARA1_14_L - ADVANCED_MODE1; j++)
    {
        setParaMin(ADVANCED_PARA1_1_H + j + ((i-1)*ADV_PARA_SIZE), 0);
        setParaMax(ADVANCED_PARA1_1_H + j + ((i-1)*ADV_PARA_SIZE), 0);
    }

    // Advanced 공통 파라미터 Max limit 설정
    Para[FREE_REVERSE_ROTATION_SPEED1 + ((i-1)*ADV_PARA_SIZE)].max = speed_upper_limit;
    Para[REVERS_ANGLE_SETTING_SPEED1 + ((i-1)*ADV_PARA_SIZE)].max  = speed_upper_limit;
    

    // Advanced 모드에 따라 Min, Max 초기화
    switch(aMode)
    {
        case ADV_CLAMP_TQ_CONTROL:     // auto seating point control (use target speed)
            // Seating point torque rate
            // Seating min torque
            // Seating max torque

            // Clamp torque replace target
            // Clamp torque after seating
            // Clamp torque min limit
            // Clamp torque max limit

            // A1 min angle
            // A1 max angle
            // A2 min angle
            // A2 max angle

            setParaMin(ADVANCED_PARA1_1_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_1_H + ((i-1)*ADV_PARA_SIZE), 1000);                       // Seating point torque rate
            setParaMin(ADVANCED_PARA1_2_H + ((i-1)*ADV_PARA_SIZE), 0);           
            setParaMax(ADVANCED_PARA1_2_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);        // Seating min torque
            setParaMin(ADVANCED_PARA1_3_H + ((i-1)*ADV_PARA_SIZE), 0);           
            setParaMax(ADVANCED_PARA1_3_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);        // Seating max torque 

            setParaMin(ADVANCED_PARA1_4_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_4_H + ((i-1)*ADV_PARA_SIZE), 1);                          // Clamp torque replace target
            setParaMin(ADVANCED_PARA1_5_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_5_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);        // Clamp torque after seating
            setParaMin(ADVANCED_PARA1_6_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_6_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);        // Clamp torque min limit
            setParaMin(ADVANCED_PARA1_7_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_7_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);        // Clamp torque max limit

            setParaMin(ADVANCED_PARA1_8_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_8_H + ((i-1)*ADV_PARA_SIZE), 20000);                      // A1 min angle         
            setParaMin(ADVANCED_PARA1_9_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_9_H + ((i-1)*ADV_PARA_SIZE), 20000);                      // A1 max angle         
            setParaMin(ADVANCED_PARA1_10_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_10_H + ((i-1)*ADV_PARA_SIZE), 20000);                     // A2 min angle     
            setParaMin(ADVANCED_PARA1_11_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_11_H + ((i-1)*ADV_PARA_SIZE), 20000);                     // A2 max angle

            setParaMin(ADVANCED_PARA1_12_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_12_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);       // Rundown min torque
            setParaMin(ADVANCED_PARA1_13_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_13_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);       // Rundown max torque
            setParaMin(ADVANCED_PARA1_14_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_14_H + ((i-1)*ADV_PARA_SIZE), 20000);                     // Rundown start angle
            setParaMin(ADVANCED_PARA1_15_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_15_H + ((i-1)*ADV_PARA_SIZE), 20000);                     // Rundown end angle
        break;

        case ADV_PREVAIL_TQ_CONTROL:     // add prevailing torque (use target speed)
            // Prevailing snug torque
            // Prevailing start angle after snug torque
            // Prevailing angle range
            // Min torque during prevailing
            // Max torque during prevailing
            // Min prevailing torque
            // Max prevailing torque
            // Prevailing compensation

            // Clamp torque after prevailing
            // Clamp torque min limit
            // Clamp torque max limit

            // Seating detection
            // Seating point torque rate
            // Seating min torque
            // Seating max torque

            // A1 min angle
            // A1 max angle
            // A2 min angle
            // A2 max angle


            setParaMin(ADVANCED_PARA1_1_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_1_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);            // Prevailing snug torque
            setParaMin(ADVANCED_PARA1_2_H + ((i-1)*ADV_PARA_SIZE), 0);           
            setParaMax(ADVANCED_PARA1_2_H + ((i-1)*ADV_PARA_SIZE), 20000);                          // Prevailing start angle after snug torque
            setParaMin(ADVANCED_PARA1_3_H + ((i-1)*ADV_PARA_SIZE), 0);           
            setParaMax(ADVANCED_PARA1_3_H + ((i-1)*ADV_PARA_SIZE), 20000);                          // Prevailing angle range
            setParaMin(ADVANCED_PARA1_4_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_4_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);            // Min torque during prevailing
            setParaMin(ADVANCED_PARA1_5_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_5_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);            // Max torque during prevailing
            setParaMin(ADVANCED_PARA1_6_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_6_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);            // Min prevailing torque
            setParaMin(ADVANCED_PARA1_7_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_7_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);            // Max prevailing torque
            setParaMin(ADVANCED_PARA1_8_H + ((i-1)*ADV_PARA_SIZE), 0);           
            setParaMax(ADVANCED_PARA1_8_H + ((i-1)*ADV_PARA_SIZE), 1);                              // Prevailing compensation   

            setParaMin(ADVANCED_PARA1_9_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_9_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);            // Clamp torque after prevailing
            setParaMin(ADVANCED_PARA1_10_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_10_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);           // Clamp torque min limit      
            setParaMin(ADVANCED_PARA1_11_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_11_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);           // Clamp torque max limit
                    
            setParaMin(ADVANCED_PARA1_12_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_12_H + ((i-1)*ADV_PARA_SIZE), 1000);                          // Seating point torque rate
            setParaMin(ADVANCED_PARA1_13_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_13_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);           // Seating min torque
            setParaMin(ADVANCED_PARA1_14_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_14_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);           // Seating max torque

            setParaMin(ADVANCED_PARA1_15_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_15_H + ((i-1)*ADV_PARA_SIZE), 20000);                         // A1 min angle         
            setParaMin(ADVANCED_PARA1_16_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_16_H + ((i-1)*ADV_PARA_SIZE), 20000);                         // A1 max angle         
            setParaMin(ADVANCED_PARA1_17_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_17_H + ((i-1)*ADV_PARA_SIZE), 20000);                         // A2 min angle     
            setParaMin(ADVANCED_PARA1_18_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_18_H + ((i-1)*ADV_PARA_SIZE), 20000);                         // A2 max angle
        break;

        case ADV_THREAD_TAPING:     // thread taping
            // - mix torque
            // - max torque
            // - finsh torque
            // - speed
            // - finshed angle from min torque
            // - angle start from thread tap            

            setParaMin(ADVANCED_PARA1_1_H + ((i-1)*ADV_PARA_SIZE), 0);           
            setParaMax(ADVANCED_PARA1_1_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);              
            setParaMin(ADVANCED_PARA1_2_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_2_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);   
            setParaMin(ADVANCED_PARA1_3_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_3_H + ((i-1)*ADV_PARA_SIZE), tq_upper_limit_unit);   

            setParaMin(ADVANCED_PARA1_4_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_4_H + ((i-1)*ADV_PARA_SIZE), speed_upper_limit); 
            setParaMin(ADVANCED_PARA1_5_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_5_H + ((i-1)*ADV_PARA_SIZE), 20000);                              // angle               
            setParaMin(ADVANCED_PARA1_6_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_6_H + ((i-1)*ADV_PARA_SIZE), 1);
        break;

        case ADV_FING_ENGAGING_TORQUE:     // find engaging torque
            // - torque
            // - speed
            // - angle limit
            // - time limit
            // - angle clear after engaging            

            setParaMin(ADVANCED_PARA1_1_H + ((i-1)*ADV_PARA_SIZE), 0);           
            setParaMax(ADVANCED_PARA1_1_H + ((i-1)*ADV_PARA_SIZE), 50.0);                               // Torque 50%
            setParaMin(ADVANCED_PARA1_2_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_2_H + ((i-1)*ADV_PARA_SIZE), speed_upper_limit);
            setParaMin(ADVANCED_PARA1_3_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_3_H + ((i-1)*ADV_PARA_SIZE), 20000);                              // degree

            setParaMin(ADVANCED_PARA1_4_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_4_H + ((i-1)*ADV_PARA_SIZE), 10);                                	// max: 10sec
            setParaMin(ADVANCED_PARA1_5_H + ((i-1)*ADV_PARA_SIZE), 0);
            setParaMax(ADVANCED_PARA1_5_H + ((i-1)*ADV_PARA_SIZE), 1);                
        break;
        default:
        break;
    }            
}


/**
 * @brief Advanced의 모드가 변경되었을 때 min, max 변경 및 파라미터 초기화 실행.
 * 
 * @param address : 변경하고자 하는 주소
 * @param newData : 변경하고자 하는 값
 * @param oldData : 원래 값
 */
void setAdvancedFromModeChange(uint16_t address, uint16_t newData, uint16_t oldData)
{
    uint16_t presetNum;

    float tq_upper_limit_unit = Info_DrvModel[Para[DRIVER_MODEL].val].tq_max * TorqueMetric[UNIT_Nm][Para[UNIT].val] * getCrowFootRatio();
    uint16_t speed_upper_limit = Info_DrvModel[Para[DRIVER_MODEL].val].speed_max / getCrowFootRatio();

    if(address >= ADVANCED_MODE1)
    {
        // 변경된 advence mode에 따라서 min/max 값 변경.
        if (((address - ADVANCED_MODE1) % PRESET_SIZE) == 0 && address <= ADVANCED_MODE1 + (PRESET_INDEX_SIZE - 1) * ADV_PARA_SIZE)
        {
            // 기존 데이타를 확인해서 변경이면 torque min/max 값을 초기화 한다.
            if (oldData != newData)
            {
                // Find preset no
                presetNum = address - ADVANCED_MODE1;
                presetNum = (presetNum / ADV_PARA_SIZE) + 1;    

                // Check preset
                if(presetNum > PRESET_INDEX_SIZE || presetNum == 0)
                    presetNum = 1;

                // Advanced 파라미터 Min, Max 설정
                Init_Para_limit_forAdvence(presetNum, tq_upper_limit_unit, speed_upper_limit);

                // Advanced 파라미터 0으로 초기화
                write_init_advanced_para_to_memory(presetNum);
            }
        }
    }
}


/**
 * @brief Advanced Mode 변경 시 파라미터의 값을 0 으로 초기화
 * 
 * @param pno : Preset 번호
 */
void write_init_advanced_para_to_memory(uint8_t pno)
{
    uint16_t i, base;

    // Check preset number
    if(pno >= 1)
    {
        // Advanced 시작 주소 계산
        base = ADVANCED_PARA1_1_H + ((pno - 1) * ADV_PARA_SIZE);

        // 시작 주소 부터 14개 파라미터 값 0으로 초기화 (401 ~ 428 : Free reverse 부분은 변경 X)
        for (i = 0; i < ADV_PARA_SIZE - 1; i++)
        {
            ParameterWriteToMemory(base + i, 0);      // 0으로 초기화.
        }
    }
}



/* Advanced 동작 세팅 관련 ---------------------------------------------------------------------------------------------------------*/

/**
 * @brief Advanced 동작 토크, Angle 설정 값을 setting
 * 
 * @param presetNum : 설정하려는 Preset 번호
 * @param FasteningTargetTorque : 목표 토크
 * @param DefaultAcc_time : 속도 가감속 만들기 위한 상수 (가감속 시간 * 1ms 주기)
 */
void setAdvancedParameter(uint8_t presetNum, float FasteningTargetTorque, float DefaultAcc_time)
{
    float crowfootGear = 1.0;

    // Get advencd mode
    AdvanceMode = Para[ADVANCED_MODE1 + ((presetNum-1)*ADV_PARA_SIZE)].val;      

    // Crow foot 기어비 반영
    crowfootGear = getCrowFootRatio();


    // Advanced 파라미터 초기화
    memset(&AtSeating,  0, sizeof(AtSeating));          // Auto seating detect
    memset(&Clamping,   0, sizeof(Clamping));           // Clamp control
    memset(&Prevailing, 0, sizeof(Prevailing));         // Prevailing control
    memset(&ThreadTap,  0, sizeof(ThreadTap));          // Thread tapping control
    memset(&EngagingTQ, 0, sizeof(EngagingTQ));         // Engaging torque control
    memset(&RundownTq,  0, sizeof(RundownTq));          // Rundown torque control

    switch(AdvanceMode)
    {
        case ADV_CLAMP_TQ_CONTROL:
            // Seating point torque rate
            // Seating min torque
            // Seating max torque

            // Clamp torque replace target
            // Clamp torque after seating
            // Clamp torque min limit
            // Clamp torque max limit

            // A1 min angle
            // A1 max angle
            // A2 min angle
            // A2 max angle

            // 파라피터 설정 반영            
            AtSeating.set_gradient_Nm = getParaVal(ADVANCED_PARA1_1_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;          // 10ms 동안 각도 1변화 했을 때 토크 변화
            AtSeating.min_seating_torque = getParaVal(ADVANCED_PARA1_2_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;
            AtSeating.max_seating_torque = getParaVal(ADVANCED_PARA1_3_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;
            
            Clamping.replace_target_active = getParaVal(ADVANCED_PARA1_4_H + ((presetNum-1)*ADV_PARA_SIZE));
            Clamping.clamp_torque_Nm = getParaVal(ADVANCED_PARA1_5_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            Clamping.min_clamp_torque = getParaVal(ADVANCED_PARA1_6_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;
            Clamping.max_clamp_torque = getParaVal(ADVANCED_PARA1_7_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;
            
            AtSeating.min_seating_angle = (getParaVal(ADVANCED_PARA1_8_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);
            AtSeating.max_seating_angle = (getParaVal(ADVANCED_PARA1_9_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);
            Clamping.min_angle_after_seating = (getParaVal(ADVANCED_PARA1_10_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);
            Clamping.max_angle_after_seating = (getParaVal(ADVANCED_PARA1_11_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);

            RundownTq.runDown_min_torque_Nm = getParaVal(ADVANCED_PARA1_12_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            RundownTq.runDown_max_torque_Nm = getParaVal(ADVANCED_PARA1_13_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            RundownTq.runDown_inspect_start_angle = (getParaVal(ADVANCED_PARA1_14_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);
            RundownTq.runDown_inspect_end_angle = (getParaVal(ADVANCED_PARA1_15_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);

            
            // Check advanced 기능 ON, OFF
            // Seating detect
            if(AtSeating.set_gradient_Nm)
                AtSeating.autoSeatDetectActive = ON;
            else
                AtSeating.autoSeatDetectActive = OFF;
        break;

        case ADV_PREVAIL_TQ_CONTROL:
            // Prevailing snug torque
            // Prevailing angle
            // Min torque during prevailing
            // Max torque during prevailing
            // Min prevailing torque
            // Max prevailing torque
            // Prevailing compensation

            // Clamp torque after prevailing
            // Clamp torque min limit
            // Clamp torque max limit

            // Seating detection
            // Seating point torque rate
            // Seating min torque
            // Seating max torque

            // 파라미터 설정 반영
            Prevailing.snugTorque_Nm = getParaVal(ADVANCED_PARA1_1_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            Prevailing.startAngle = (getParaVal(ADVANCED_PARA1_2_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);                      // Snug 토크 발생 이후 시작 각도
            Prevailing.targetAngle = (getParaVal(ADVANCED_PARA1_3_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);                     // Start angle 이후 측정할 각도
            Prevailing.minTorqueInSection_Nm = getParaVal(ADVANCED_PARA1_4_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            Prevailing.maxTorqueInSection_Nm = getParaVal(ADVANCED_PARA1_5_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            Prevailing.min_prevailing_torque = getParaVal(ADVANCED_PARA1_6_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;
            Prevailing.max_prevailing_torque = getParaVal(ADVANCED_PARA1_7_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;
            Prevailing.compensationActive = getParaVal(ADVANCED_PARA1_8_H + ((presetNum-1)*ADV_PARA_SIZE));

            Clamping.clamp_torque_Nm = getParaVal(ADVANCED_PARA1_9_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            Clamping.min_clamp_torque = getParaVal(ADVANCED_PARA1_10_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;  
            Clamping.max_clamp_torque = getParaVal(ADVANCED_PARA1_11_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;

            AtSeating.set_gradient_Nm = getParaVal(ADVANCED_PARA1_12_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;    // 10ms 동안 각도 1변화 했을 때 토크 변화
            AtSeating.min_seating_torque = getParaVal(ADVANCED_PARA1_13_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;
            AtSeating.max_seating_torque = getParaVal(ADVANCED_PARA1_14_H + ((presetNum-1)*ADV_PARA_SIZE)) / crowfootGear;

            AtSeating.min_seating_angle = (getParaVal(ADVANCED_PARA1_15_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);
            AtSeating.max_seating_angle = (getParaVal(ADVANCED_PARA1_16_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);
            Clamping.min_angle_after_seating = (getParaVal(ADVANCED_PARA1_17_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);
            Clamping.max_angle_after_seating = (getParaVal(ADVANCED_PARA1_18_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);


            // Check advanced 기능 ON, OFF
            // Seating detect
            if(AtSeating.set_gradient_Nm)
                AtSeating.autoSeatDetectActive = ON;
            else
                AtSeating.autoSeatDetectActive = OFF;
        break;

        case ADV_THREAD_TAPING:
            ThreadTap.Min_Torque_Nm = getParaVal(ADVANCED_PARA1_1_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            ThreadTap.Max_Torque_Nm = getParaVal(ADVANCED_PARA1_2_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            ThreadTap.Finish_Torque_Nm = getParaVal(ADVANCED_PARA1_3_H + ((presetNum-1)*ADV_PARA_SIZE)) * TorqueMetric[Para[UNIT].val][UNIT_Nm] / crowfootGear;
            ThreadTap.Speed = getParaVal(ADVANCED_PARA1_4_H + ((presetNum-1)*ADV_PARA_SIZE)) * deceleration_ratio * RPM_TO_RAD_SCALE * screw_type;
            ThreadTap.AngleLimitFromMinTorque = getParaVal(ADVANCED_PARA1_5_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360;
            ThreadTap.AngleStartFromThreadTap = getParaVal(ADVANCED_PARA1_6_H + ((presetNum-1)*ADV_PARA_SIZE));
            ThreadTap.SpeedAcc = fABS(ThreadTap.Speed) / (DefaultAcc_time);
        break;

        case ADV_FING_ENGAGING_TORQUE:
            EngagingTQ.Torque = FasteningTargetTorque * getParaVal(ADVANCED_PARA1_1_H + ((presetNum-1)*ADV_PARA_SIZE)) * 0.01;         // 0.01 => 값 10일 때 10 * 0.01 = 0.1 (10%)   소수점 2자리
            EngagingTQ.Speed  = getParaVal(ADVANCED_PARA1_2_H + ((presetNum-1)*ADV_PARA_SIZE)) * RPM_TO_RAD_SCALE * deceleration_ratio * screw_type;
            EngagingTQ.AngleLimit = (getParaVal(ADVANCED_PARA1_3_H + ((presetNum-1)*ADV_PARA_SIZE)) * Motor.num_pulse * deceleration_ratio / 360);   //degree
            EngagingTQ.TimeLimit  = (getParaVal(ADVANCED_PARA1_4_H + ((presetNum-1)*ADV_PARA_SIZE)) * INV_TS_VEL);
            EngagingTQ.EngagingAfterAngleClear = getParaVal(ADVANCED_PARA1_5_H + ((presetNum-1)*ADV_PARA_SIZE));
            EngagingTQ.SpeedAcc = fABS(EngagingTQ.Speed) / (DefaultAcc_time);    
        break;

        default:
        break;
    }
}


/**
 * @brief 해당 preset에 advanced의 value 값을 초기화 한다.
 * 
 * @param pNum : Preset 번호 (1 ~ PRESET_INDEX_SIZE)
 */
void initAdvancedParameter(uint8_t pNum)
{    
    // Check preset number
    if(pNum > PRESET_INDEX_SIZE && pNum < 1)
        return;

    // Init advanced value
    for(uint8_t paraNum = 0; paraNum < ADV_PARA_SIZE; paraNum++)
        setParaVal(ADVANCED_MODE1 + paraNum + (pNum - 1) * ADV_PARA_SIZE, 0);
}
    





/* Advanced 동작 관련 ---------------------------------------------------------------------------------------------------------*/

/**
 * @brief Prevailing의 Angle 구간에서 평균 토크를 구함
 * 
 * @param step : 현재 체결이 진행되고 있는 상태 step
 * @param currentAngle : 현재 회전된 각도 (pulse)
 * @param torqueNm : 현재 토크 (N.m 단위)
 */
void check_prevailing(uint8_t step, uint32_t currentAngle, float torqueNm)
{
    float tq_tmp = 0.0;

    // Check Prevailing mode 사용
    if(AdvanceMode == ADV_PREVAIL_TQ_CONTROL)
    {
        // Prevailing 파라미터 설정 확인
        if(Prevailing.targetAngle == 0)
            system_err = ADVACNED_PARA_ERROR;

        // Prevailing 파라미터 설정 확인 (Clamp 토크 관련)
        if(Prevailing.compensationActive)
        {
            if(Clamping.clamp_torque_Nm == 0)
                system_err = ADVACNED_PARA_ERROR;
        }        

        // Check Prevailing snug torque
        if((torqueNm >= Prevailing.snugTorque_Nm || Prevailing.snugTorque_Nm == 0) && Prevailing.detectAngle == 0)
        {
            // Prevailing 시작
            Prevailing.detectAngle = currentAngle;           // 시작 Angle 값
            Torque_cnv.snug = torqueNm * TorqueMetric[UNIT_Nm][Para[UNIT].val];                     // Snug 감지 시 토크
        }

        // Check Prevailing 구간 시작
        if(Prevailing.detectAngle != 0)
        {
            // Prevailing의 감지 각도 이후 시작 각도 확인
            if(currentAngle - Prevailing.detectAngle >= Prevailing.startAngle)
            {
                // Prevailing 완료 확인
                if(Prevailing.finished_prevailing == OFF && step < RAMPUP_TARGET_TORQUE)
                {
                    // Prevailing 완료 전 구간
                    // Check Prevailing 구간 Angle
                    if(currentAngle - Prevailing.detectAngle < Prevailing.targetAngle)
                    {
                        // Prevailing 구간 평균 토크 계산
                        Prevailing.sum_tq += RealTq_fromSensor_reflectedSettingUnit;
                        Prevailing.cnt_loop++;
                    }
                    else
                    {
                        // Prevailing 최종 평균 토크 계산
                        Prevailing.finished_prevailing = ON;
                        tq_tmp = Prevailing.sum_tq / (float)Prevailing.cnt_loop;
                        Torque_cnv.prevailing = tq_tmp;
                        Prevailing.result_of_prevailing_torque_Nm = tq_tmp * TorqueMetric[Para[UNIT].val][UNIT_Nm];     // change Nm
                    }

                    // Check Prevailing 구간 내에서 토크의 상,하한 값
                    check_tq_during_prevailing(torqueNm);

                    // 그래프 - Step 설정
                    setGraphStepQueue(GRAPH_STEP_PREVAILING_START);
                }
                else
                {
                    // Prevailing 종료된 구간 (평균 토크 계산 완료 -> Min, Max 확인)

                    // Prevailing 구간 이전에 seating
                    if (!Prevailing.finished_prevailing)
                        system_err = NO_FINSH_PREVAILING;
                    
                    // Prevailing 구간 이후
                    if (Prevailing.finished_prevailing)
                    {
                        if(Prevailing.min_prevailing_torque != 0 && (Prevailing.min_prevailing_torque > Torque_cnv.prevailing))
                            system_err = PREVAILING_TQ_ERROR;

                        if (Prevailing.max_prevailing_torque != 0 && (Prevailing.max_prevailing_torque < Torque_cnv.prevailing))
                            system_err = PREVAILING_TQ_ERROR;
                    }

                    // 최종 목표 토크 가 드라이버 최대 토크 보다 큰지 확인
                    if(Prevailing.result_of_prevailing_torque_Nm + Clamping.clamp_torque_Nm > FastenSeq.MaxTorque_Nm)
                        system_err = COMPENSATION_OVER_MAX_TQ;
                }
            }
        }
        else
        {
            // Check Prevailing passed (Snug을 감지 못하고 토크가 체결되버렸을 때 - Auto seating이 아닐 경우 발생 가능)
            if(step >= RAMPUP_TARGET_TORQUE && Prevailing.finished_prevailing == OFF)
                system_err = NO_FINSH_PREVAILING;
        }
    }
}


/**
 * @brief Prevailing 구간 내에서 토크가 변할 수 있는 상,하한 limit 확인
 * 
 * @param torqueNm : 현재 토크 값 (N.m)
 */
void check_tq_during_prevailing(float torqueNm)
{
    if(AdvanceMode == ADV_PREVAIL_TQ_CONTROL)
    {
        if(Prevailing.minTorqueInSection_Nm != 0 && (Prevailing.minTorqueInSection_Nm > torqueNm))
            system_err = PREVAILING_TQ_ERROR;

        if (Prevailing.maxTorqueInSection_Nm != 0 && (Prevailing.maxTorqueInSection_Nm < torqueNm))
            system_err = PREVAILING_TQ_ERROR;
    }
}


/**
 * @brief advanced에 있는 토크 관련 변수에 Compensation 반영
 * 
 * @param advancedMode : 현재 Advanced 모드
 * @param compensation : Compensation 값
 */
void setAdvancedCompensation(uint16_t advancedMode, float compensation)
{
    // Check compensation
    if(compensation < 30 || compensation > 200)
        compensation = 100;

    // Apply compensation to advanced
    switch (advancedMode)
    {
        case ADV_CLAMP_TQ_CONTROL:
            AtSeating.set_gradient_Nm *= compensation * 0.01;
            AtSeating.min_seating_torque *= compensation * 0.01;
            AtSeating.max_seating_torque *= compensation * 0.01;

            Clamping.clamp_torque_Nm *= compensation * 0.01;
            Clamping.min_clamp_torque *= compensation * 0.01;
            Clamping.max_clamp_torque *= compensation * 0.01;
            break;

        case ADV_PREVAIL_TQ_CONTROL:
            Prevailing.snugTorque_Nm *= compensation * 0.01;
            Prevailing.minTorqueInSection_Nm *= compensation * 0.01;
            Prevailing.maxTorqueInSection_Nm *= compensation * 0.01;
            Prevailing.min_prevailing_torque *= compensation * 0.01;
            Prevailing.max_prevailing_torque *= compensation * 0.01;

            Clamping.clamp_torque_Nm *= compensation * 0.01;
            Clamping.min_clamp_torque *= compensation * 0.01;
            Clamping.max_clamp_torque *= compensation * 0.01;

            AtSeating.set_gradient_Nm *= compensation * 0.01;
            AtSeating.min_seating_torque *= compensation * 0.01;
            AtSeating.max_seating_torque *= compensation * 0.01;
            break;

        case ADV_THREAD_TAPING:
            ThreadTap.Min_Torque_Nm *= compensation * 0.01;
            ThreadTap.Max_Torque_Nm *= compensation * 0.01;
            ThreadTap.Finish_Torque_Nm *= compensation * 0.01;
            break;

        case ADV_FING_ENGAGING_TORQUE:
            EngagingTQ.Torque *= compensation * 0.01;
            break;
        default:
            break;
    }
}


/**
 * @brief Seating torque, A1 angle의 min limit 확인
 * 
 * @param tq : seating torque (설정된 UNIT 단위)
 * @param angle : 회전 각도 (pulse)
 */
void check_seating_error_min(float tq, long angle)
{
    // check min torque
    if (AtSeating.min_seating_torque != 0)
    {
        if (tq < AtSeating.min_seating_torque)
            system_err = SEATING_TQ_ERROR;
    }

    // check min angle
    if (AtSeating.min_seating_angle != 0)
    {
        if (angle < AtSeating.min_seating_angle)
            system_err = SEATING_ANGLE_ERROR;
    }
}


/**
 * @brief Seating torque, A1 angle의 max limit 확인
 * 
 * @param tq : seating torque (설정된 UNIT 단위)
 * @param angle : 회전 각도
 */
void check_seating_error_max(float tq, long angle)
{
    // check max torque
    if (AtSeating.max_seating_torque != 0)
    {
        if (tq > AtSeating.max_seating_torque)
            system_err = SEATING_TQ_ERROR;
    }

    // check max angle
    if (AtSeating.max_seating_angle != 0)
    {
        if (angle > AtSeating.max_seating_angle)
            system_err = SEATING_ANGLE_ERROR;
    }
}


/**
 * @brief A2의 Min angle limit 확인
 * 
 * @param angle : A2 각도
 */
void check_afterSeating_error_min(long angle)
{
    // A2 Angle limit 관리
    if (Clamping.min_angle_after_seating != 0)
    {
        if (angle < Clamping.min_angle_after_seating)
        {
            system_err = A2_ANGLE_ERROR;
        }            
    }
}


/**
 * @brief A2의 Max angle limit 확인
 * 
 * @param angle : A2 각도
 */
void check_afterSeating_error_max(long angle)
{
    // A2 max angle check
    if ((Clamping.max_angle_after_seating != 0) && (fastening_step >= RAMPUP_TARGET_TORQUE))
    {
        if (angle > Clamping.max_angle_after_seating)
        {
            system_err = A2_ANGLE_ERROR;
        }            
    }
}


/**
 * @brief Advanced 모드에서 목표 토크 값을 반환
 * 
 * @param preset    : 현재 Preset 번호
 * @return uint16_t : 타켓 토크 값
 */
float advancedTargetTorque(uint8_t preset)
{
    float retval = 0;
    uint16_t advancedMode;

    // Check preset limit
    if(preset > PRESET_INDEX_SIZE)
        preset = 1;

    // Check Advanced 모드
    advancedMode = Para[ADVANCED_MODE1 + ((preset-1)*ADV_PARA_SIZE)].val;
    if(advancedMode != 0)
    {
        // Advanced 모드에 따라 타겟토크 계산 방법 선택
        switch (advancedMode)
        {
            case ADV_CLAMP_TQ_CONTROL:
                // Clamp torque 사용 여부
                if(Clamping.replace_target_active)
                    retval = getParaVal(ADVANCED_PARA1_5_H + ((preset-1)*ADV_PARA_SIZE));       // T: Clamp TQ
                else
                    retval = getParaVal(FASTEN_TORQUE1_H +((preset-1)*PRESET_SIZE));            // T: Preset Target TQ
                break;
            
            case ADV_PREVAIL_TQ_CONTROL:
                // Prevailing torque 사용 여부
                if(Prevailing.compensationActive)
                    retval = getParaVal(ADVANCED_PARA1_8_H + ((preset-1)*ADV_PARA_SIZE));       // T: Clamp TQ
                else
                    retval = getParaVal(FASTEN_TORQUE1_H +((preset-1)*PRESET_SIZE));            // T: Preset Target TQ
                break;

            default:
                // 이외 Advanced 모드에서는 기존 타켓 토크 사용
                retval = getParaVal(FASTEN_TORQUE1_H +((preset-1)*PRESET_SIZE));                // T: Preset Target TQ
                break;
        }
    }
    else
    {
        // Advanced 모드 아닐 때
        retval = getParaVal(FASTEN_TORQUE1_H +((preset-1)*PRESET_SIZE));
    }

    // Target 토크가 0일 경우 기존 Preset target torque 표시
    if(retval == 0)
        retval = getParaVal(FASTEN_TORQUE1_H +((preset-1)*PRESET_SIZE));

    return retval;
}




/* Gradient ---------------------------------------------------------------------------------------------------------*/
		
/**
 * @brief Auto seating을 위한 토크의 평균 값을 구한다.
 * 
 * @param input  : 토크 값
 * @return float : (현재는 평균 X)1.25ms 동안의 평균 토크 값
 */
float gradientAdcAverage(float input)
{
    static float    data[10];	                // 필터 값 보다 커야함 -> 새로운 data 저장하는 index와 예전 data 빼려는 index가 겹치면 안된다.
	static float    sumdata;					// Data의 합산	
    static uint16_t sumCount;					// 몇개의 Data가 더해졌는지 판단 (필터 개수까지 카운트)
	static uint16_t bufIndex;					// Buffer에 저장할 index
	static uint16_t subtractBufIndex;			// 뺄셈할 값의 index
	float average = 0;						    // 평균값

	// Data 반영
	data[bufIndex] = input;
	sumdata += data[bufIndex++];

	// 저장 Buffer index 관리
	if(bufIndex >= 10)
		bufIndex = 0;

	// 더한 개수 판단
	if(sumCount < 1)
		sumCount++;								// 개수 미달 -> count 증가
	else
		sumdata -= data[subtractBufIndex++];	// 개수 충족 -> 이전 값 1개 제거

	// 빼는 buffer Index 관리
	if(subtractBufIndex >= 10)
		subtractBufIndex = 0;

	// 평균 계산
	average = (float)sumdata / 1;

	return average;		
}


/**
 * @brief Auto seating을 위한 평균 각도를 구한다.
 * 
 * @param input  : 현재 회전한 각도
 * @return float : 1.25ms 동안 평균 회전 각도
 */
float gradientMpulseAverage(float input)
{
    static float    data[10];	                // 필터 값 보다 커야함 -> 새로운 data 저장하는 index와 예전 data 빼려는 index가 겹치면 안된다.
	static float    sumdata;					// Data의 합산	
    static uint16_t sumCount;					// 몇개의 Data가 더해졌는지 판단 (필터 개수까지 카운트)
	static uint16_t bufIndex;					// Buffer에 저장할 index
	static uint16_t subtractBufIndex;			// 뺄셈할 값의 index
	float average = 0;						    // 평균값

	// Data 반영
	data[bufIndex] = input;
	sumdata += data[bufIndex++];

	// 저장 Buffer index 관리
	if(bufIndex >= 10)
		bufIndex = 0;

	// 더한 개수 판단
	if(sumCount < 1)
		sumCount++;								// 개수 미달 -> count 증가
	else
		sumdata -= data[subtractBufIndex++];	// 개수 충족 -> 이전 값 1개 제거

	// 빼는 buffer Index 관리
	if(subtractBufIndex >= 10)
		subtractBufIndex = 0;

	// 평균 계산
	average = (float)sumdata / 1;

	return average;		
}


/**
 * @brief Gradient의 값을 계산한다.
 * 
 * @param step : 현재 체결 진행중인 상태 STEP
 * @param torqueNm : 현재 토크 (N.m)
 * @param angle : 현재 각도 (Degree)
 */
void calcGradient(uint8_t step, float torqueNm, float angle)
{
    float at_gTemp[2];
    float temp;

    // Auto Seating 감지 사용 여부
    if(AtSeating.autoSeatDetectActive)
    {
        // Check 파라미터 설정
        if(AtSeating.set_gradient_Nm == 0)
            system_err = ADVACNED_PARA_ERROR;


        // Seating 감지 이전
        if(fastening_step <= FIND_SEATING_POINT)
        {
            // (현재는 평균 X) 1.25ms 정도 평균을 구하고 그것의 토크 기울기를 구한다.
            AtSeating.averageAdc = gradientAdcAverage(torqueNm);        // 토크 (N.m)
            AtSeating.averageMpulse = gradientMpulseAverage(angle);     // 각도 (모터의 각도)

            AtSeating.adc[AtSeating.sdx] = AtSeating.averageAdc;
            AtSeating.mpulse[AtSeating.sdx] = AtSeating.averageMpulse;
            
            at_gTemp[0] = AtSeating.adc[AtSeating.sdx] - AtSeating.adc[AtSeating.sdxOld];
            at_gTemp[1] = AtSeating.mpulse[AtSeating.sdx] - AtSeating.mpulse[AtSeating.sdxOld];

            AtSeating.sdxOld = (AtSeating.sdxOld == 100-1) ? 0 : AtSeating.sdxOld+1;
            AtSeating.sdx = (AtSeating.sdx == 100-1) ? 0 : AtSeating.sdx+1;          // 10ms 동안 angle대 torque의 변화.

            // Gradient 계산
            if(at_gTemp[0] < 0)
            {
                // (-) 값이 나올 때는 이전 값을 비슷하게 유지
                AtSeating.currentGradient_Nm = AtSeating.currentGradient_Nm * 0.995;
            }
            else if(at_gTemp[1] != 0)
            {
                // 정상 : 0으로 나누는 현상만 제거
                temp = ((float)at_gTemp[0] / (float)at_gTemp[1]);        // 값 1 = 각도 1당 1 N.m 변화
                AtSeating.currentGradient_Nm = AtSeating.currentGradient_Nm * 0.05 + temp * 0.95;
            }

            // 현재 Gradient 값을 표시할 구간을 정한다.
            if(fastening_step > FIND_SEATING_POINT || AtSeating.currentGradient_Nm < 0 || fastening_step <= START_FASTENING)
                AtSeating.currentGradient_Nm = 0;


            // // FIXME: 최대값 테스트
            // if(AtSeating.currentGradient_Nm > AtSeating.finalGradient)
            //     AtSeating.finalGradient = AtSeating.currentGradient_Nm;
        }
    }
}


/**
 * @brief Auto seating 기울기 확인
 * 
 * @param torqueNm : 현재 토크 (N.m)
 * @param gradient : 계산된 Gradient 값
 * @param angle : 현재 각도
 */
void check_gradient_and_min(float torqueNm, float gradient, long angle)
{
    // Gradient 감지 확인
    if (gradient >= AtSeating.set_gradient_Nm)
    {
        AtSeating.finalGradient = gradient;           // 최종 기울기..

        AtSeating.finalGradientTq_Nm = fABS(torqueNm);
        Torque_cnv.seating = torqueNm * TorqueMetric[UNIT_Nm][Para[UNIT].val];

        FlagFasten.FindedSeationPointStep = ON;
    }
}     




/* Rundown ---------------------------------------------------------------------------------------------------------*/

/**
 * @brief Rundown 중 토크의 min, max limit 확인 기능
 * 
 * @param angle : 현재 각도
 * @param torque : 현재 토크
 */
void runDownTorqueMinMaxLimit(float angle, float torque)
{
    // Check rundown 중 체크할 토크 값이 있는 지
    if(RundownTq.runDown_min_torque_Nm == 0 && RundownTq.runDown_max_torque_Nm == 0)
        return;

    // Check rundown 중 체크할 angle 값이 있는 지
    if(RundownTq.runDown_inspect_end_angle == 0)
        return;
    
    // Check rundown 으로 설정한 angle 범위
    if(RundownTq.runDown_inspect_start_angle > fABS(angle) || RundownTq.runDown_inspect_end_angle < fABS(angle))
        return;

    // Check torque 값
    if(RundownTq.runDown_min_torque_Nm > fABS(torque) && RundownTq.runDown_min_torque_Nm)
    {
        system_err = RUNDOWN_TORQUE_ERROR;
    }

    if(RundownTq.runDown_max_torque_Nm < fABS(torque) && RundownTq.runDown_max_torque_Nm)
    {
        system_err = RUNDOWN_TORQUE_ERROR;
    }
}
