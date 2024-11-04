/**
  ******************************************************************************
  * @file    lastPresetModel.c
  * @author  jun
  * @brief   Header file of last preset, model operation process.
  * @note    마지막으로 사용했던 preset, model 정보를 저장, 재부팅 시 이전 작업을 활성화 시킨다
  ******************************************************************************
  */


/* Include ---------------------------------------------------------------------*/
#include "lastPresetModel.h"
#include "variable.h"
#include "backup.h"

 

/* Variable ---------------------------------------------------------------------*/




/* Extern -----------------------------------------------------------------------*/
lastPresetModelStruct lastPreset;




/* Funcions ----------------------------------------------------------------------*/

/**
 * @brief Preset 또는 Model의 변화가 생기는 것을 저장한다.
 */
void lastPresetModelProcess(void)
{
    // Save preset number
    checkChangePreset();


    #if LAST_MODEL_ACTIVE

    // Save model number
    checkChangeModelStep();

    #endif
}



/**
 * @brief Get 초기 Last Preset Model의 값
 */
void getLastPresetModel(void)
{
    SPI_ReadMemoryData(LAST_PRESET_NUM, &lastPreset.preset, sizeof(lastPreset.preset));
    SPI_ReadMemoryData(LAST_MODEL_STEP, &lastPreset.modelStep, sizeof(lastPreset.modelStep));
    SPI_ReadMemoryData(LAST_MODEL_SCREW, (uint8_t*)&lastPreset.modelScrew, sizeof(lastPreset.modelScrew));
}




/**
 * @brief 부팅시 Preset을 저장하기 위해 Preset 변경이 있을 시 저장한다.
 * @note  3초마다 preset 확인
 * */
void checkChangePreset(void)
{
    static uint32_t loopTime;
    uint8_t processNum;             // 현재 모델, Preset 번호

    if(HAL_GetTick() - loopTime > 3000)
    {
        #if LAST_MODEL_ACTIVE

            // Check model
            if(Para[MODEL_SELECT].val)
                processNum = Model_Number;
            else
                processNum = Processing_number;

        #else

            processNum = Processing_number;

        #endif

        // Check preset 저장 모드
        if(Para[FASTEN_NO_SAVE].val == 0)
        {
            // Preset 변경되었으면 저장
            if(processNum != lastPreset.preset)
            {
                SPI_WriteMemoryData(LAST_PRESET_NUM, &processNum, sizeof(processNum));
                lastPreset.preset = processNum;
            }
        }

        loopTime = HAL_GetTick();
    }
}



#if LAST_MODEL_ACTIVE

/**
 * @brief Model 기능을 사용중일 때 재부팅 시 기존의 진행되었던 모델의 진행상황을 반영.
 * 
 * @note Model의 Step, 남은 screw 개수
 */
void checkChangeModelStep(void)
{
    static uint32_t loopTime;

    if(HAL_GetTick() - loopTime > 500 && HAL_GetTick() > 4000)
    {
        // Check preset 저장 모드, Model 사용 중
        if(Para[FASTEN_NO_SAVE].val == 0 && Para[MODEL_SELECT].val)
        {
            // Model step이 변경되었으면 저장
            if(lastPreset.modelStep != Model.CurrentStep)
            {
                lastPreset.modelStep = Model.CurrentStep;
                SPI_WriteMemoryData(LAST_MODEL_STEP, &lastPreset.modelStep, sizeof(lastPreset.modelStep));
            }

            // Screw 개수가 변경되었으면 저장
            if(lastPreset.modelScrew != ScrewCountValue)
            {
                lastPreset.modelScrew = ScrewCountValue;
                SPI_WriteMemoryData(LAST_MODEL_SCREW, (uint8_t*)&lastPreset.modelScrew, sizeof(lastPreset.modelScrew));
            }
        }

        loopTime = HAL_GetTick();
    }
}


/**
 * @brief 초기 부팅 시 Model의 백업된 데이터를 반영
 * 
 * @param function : Model의 function pointer
 * @param data1 : 해당 Model step의 data1 pointer
 * @param data2 : 해당 model step의 data2 pointer
 * @param screwCount : 해당 model step의 남아있던 체결 횟수
 */
void setBootModelStep(uint16_t* function, uint16_t* data1, uint16_t* data2, uint16_t* screwCount)
{
    static uint8_t init;

    // 초기 1회만 실행
    if(init)
        return;

    init = ON;

    // Check Model 백업 기능 사용중인지
    if(Para[FASTEN_NO_SAVE].val == 0 && Para[MODEL_SELECT].val)
    {
        SPI_ReadMemoryData(LAST_PRESET_NUM, &lastPreset.preset, sizeof(lastPreset.preset));
        SPI_ReadMemoryData(LAST_MODEL_STEP, &lastPreset.modelStep, sizeof(lastPreset.modelStep));
        SPI_ReadMemoryData(LAST_MODEL_SCREW, (uint8_t*)&lastPreset.modelScrew, sizeof(lastPreset.modelScrew));

        // Check saved value
        if(lastPreset.preset < 1 || lastPreset.preset > PRESET_INDEX_SIZE)
            lastPreset.preset = 0;
        
        if(lastPreset.modelStep < 1 || lastPreset.modelStep > 20)
            lastPreset.modelStep = 0;
        
        if(lastPreset.modelScrew < 1 || lastPreset.modelScrew > 999)
            lastPreset.modelScrew = 0;


        if(lastPreset.preset && lastPreset.modelStep)
        {
            // Model 저장되었던 값 반영
            Model_Number = lastPreset.preset;
            Model.CurrentStep = lastPreset.modelStep;
            Model.TotalCount = lastPreset.modelScrew;

            // 백업된 Model step 반영
            *function = (Para[MODEL1_1 + ((Model_Number-1)*MODEL_SIZE) + (Model.CurrentStep-1)].val >> 13);
            *data1 = (Para[MODEL1_1 + ((Model_Number-1)*MODEL_SIZE) + (Model.CurrentStep-1)].val & 0x1fff) >> 8;
            *data2 = (Para[MODEL1_1 + ((Model_Number-1)*MODEL_SIZE) + (Model.CurrentStep-1)].val & 0xff);

            // Model의 Fastening step에서 남아있던 screw count 개수 반영
            if(*function == FASTENING)
                *screwCount = lastPreset.modelScrew;
            else
                *screwCount = 0;
        }
    }
}


#endif
