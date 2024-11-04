/**
  ******************************************************************************
  * @file    barcode.c
  * @author  jun
  * @brief   Header file of barcode operation process.
  * @note    바코드 스캐너에서 바코드를 읽고 처리하는 기능.
  *          바코드 (LV3096 모듈)
  ******************************************************************************
  */


/* Include ---------------------------------------------------------------------*/
#include "barcode.h"
#include "ringBuffer.h"
#include "main.h"
#include "variable.h"
#include "modbus.h"
#include "realtimeMonitoring.h"
#include "timer.h"
#include "main_bg.h"
#include "memoryQspiControl.h"
 

/* Variable ---------------------------------------------------------------------*/
barcodeStateStruct      barcode;

t_barcode_settings      presetBarcode;
t_barcode_settings      modelStepBarcode;



/* Extern -----------------------------------------------------------------------*/
extern UART_HandleTypeDef huart3;
extern SPI_HandleTypeDef  hspi3;


/* Funcions ----------------------------------------------------------------------*/


/**
 * @brief 바코드 관련 처리
 * 
 * @note 동작
 * 
 * 1. 바코드 스캐너 동작 실행 판단 - Start 레버 확인
 * 2. 바코드 받은 데이터 추출
 * 3. 읽은 바코드 값 과 저장된 바코드 값 비교 (Preset 변경을 위해)
 * 4. 바코드 값 저장 및 수정
 */
void barcodeProcess(void)
{
    // Barcode의 동작 process 상태에 따라 실행
    switch (barcode.state)
    {
        case BAROCDE_STATE_INIT:
            // 바코드 관련 항목 초기화
            initBarcode();

            barcode.state = BARCODE_STATE_RUN;
        break;

        case BARCODE_STATE_RUN:
            // 바코드 동작 및 데이터 확인
            runBarcode();
        break;

        case BARCODE_STATE_END:
        break;
        
        default:
        break;
    }
}


/**
 * @brief 바코드 관련 GPIO, Memory, UART 초기화
 */
void initBarcode(void)
{
    #if PRESET_BARCODE_ACTIVE

    // 모든 저장된 Preset 바코드 값 읽기
    ReadBarcodeDataAll(&presetBarcode, MEM_PRESET_BARCODE_START_ADDRESS);

    #endif

    #if MODEL_BARCODE_ACTIVE

    // 모든 저장된 Model 바코드 값 읽기
    ReadBarcodeDataAll(&modelStepBarcode, MEM_MODEL_BARCODE_START_ADDRESS);

    #endif

    
    // 바코드 읽기 UART 실행
    HAL_UART_Receive_DMA(&huart3, (uint8_t*) barcode.BarRx_data, 1);         // Read Barcode 

    // 바코드 스캐너 동작 OFF
    HAL_GPIO_WritePin(BAR_START_GPIO_Port, BAR_START_Pin, 1);
}


/**
 * @brief 바코드 동작 실행
 */
void runBarcode(void)
{
    static uint32_t barcodeScanTimeout;
    static uint32_t barcodeTriggerTimeout;
    static uint32_t presetSaveTimeout;
    static uint32_t modelSaveTimeout;

    // 바코드 스캐너 동작 판단 - Start 레버 동작 확인 (10ms loop)
    if(HAL_GetTick() - barcodeTriggerTimeout > 9 && HAL_GetTick() > 5)
    {
        BarcodeReadTrigger();

        barcodeTriggerTimeout = HAL_GetTick();
    }


    // Barcode 데이터 받았을 때 - 받은 바코드 메시지 분석하여 바코드 값만 추출
    checkBarcodeMessage();

    // 바코드 통신 상태 확인
    checkBarcodeComm();


    // Barcode - preset 기능 제어
    #if PRESET_BARCODE_ACTIVE

    // Barcode를 읽기 Complete 했을 때
    if(presetBarcode.flag_barcode_read_complete)
    {
        if(presetBarcode.flag_scan_barcode_index)
        {
            barcodeReadForMem(&presetBarcode);              // 바코드 저장용 읽기 분석
        }
        else
            barcodeChangePreset();                          // Preset 변경용 바코드 분석

        presetBarcode.flag_barcode_read_complete = 0;
    }

    // Barcode scan 값 timeout 10s (스캔 통신이 완료가 되지 않을 경우 OFF)
    if(presetBarcode.flag_scan_barcode_index)
    {
        if(HAL_GetTick() - barcodeScanTimeout > BARCODE_SCAN_TIMEOUT && HAL_GetTick() > 10000)
            presetBarcode.flag_scan_barcode_index = 0;
    }
    else
        barcodeScanTimeout = HAL_GetTick();


    // Barocde list initalize
    if (presetBarcode.flag_reset_all)
    {
        presetBarcode.flag_reset_all = 0;
        ClearAll_BarcodeData(&presetBarcode, MEM_PRESET_BARCODE_START_ADDRESS);
    }

    // Save Barcode list 
    if (presetBarcode.save_data_no != 0)
    {
        if(HAL_GetTick() - presetSaveTimeout > 100)
        {
            WriteBarcodeData(MEM_PRESET_BARCODE_START_ADDRESS, presetBarcode.save_data_no - 1, &presetBarcode);
            presetBarcode.save_data_no = 0;

            presetSaveTimeout = HAL_GetTick();
        }
    }

    #endif



    // Barcode - Model 기능 제어
    #if MODEL_BARCODE_ACTIVE


    // Barcode를 읽기 Complete 했을 때
    if(modelStepBarcode.flag_barcode_read_complete)
    {
       if(modelStepBarcode.flag_scan_barcode_index)
            barcodeReadForMem(&modelStepBarcode);                    // 바코드 저장용 읽기 분석
        else
            barcodeChangePreset();                                   // Model number 변경용 바코드 분석

        modelStepBarcode.flag_barcode_read_complete = 0;
    }

    // Barcode scan 값 timeout 10s (스캔 통신이 완료가 되지 않을 경우 OFF)
    if(modelStepBarcode.flag_scan_barcode_index)
    {
        if(HAL_GetTick() - barcodeScanTimeout > BARCODE_SCAN_TIMEOUT && HAL_GetTick() > 10000)
            modelStepBarcode.flag_scan_barcode_index = 0;
    }
    else
        barcodeScanTimeout = HAL_GetTick();


    // Barocde list initalize
    if (modelStepBarcode.flag_reset_all)
    {
        modelStepBarcode.flag_reset_all = 0;
        ClearAll_BarcodeData(&modelStepBarcode, MEM_MODEL_BARCODE_START_ADDRESS);
    }

    // Save Barcode list 
    if (modelStepBarcode.save_data_no != 0)
    {
        if(HAL_GetTick() - modelSaveTimeout > 100)
        {
            WriteBarcodeData(MEM_MODEL_BARCODE_START_ADDRESS, modelStepBarcode.save_data_no - 1, &modelStepBarcode);
            modelStepBarcode.save_data_no = 0;

            modelSaveTimeout = HAL_GetTick();
        }
    }

    #endif
}




/* Communication state -------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief 바코드 읽기 모드가 해제되었을 때 다시 실행 시켜준다.
 */
void checkBarcodeComm(void)
{
    // UART 상태 확인 - Barcode UART state 확인
    if(huart3.RxState != HAL_UART_STATE_BUSY_RX)
        HAL_UART_Receive_DMA(&huart3, barcode.BarRx_data , 1);
}


/**
 * @brief Barcode read 명령 실행 - DMA Error callback 등이 발생했을 때 실행한다.
 */
void barcodeRxStart(void)
{
    HAL_UART_Receive_DMA(&huart3, barcode.BarRx_data , 1);
}


/**
 * @brief Barcode 데이터가 DMA Callback이 발생했을 때 Ringbuffer에 데이터를 쌓는다
 * 
 * @note  HAL_UART_RxCpltCallback 함수에서 실행한다
 */
void barcodeRxCallback(void)
{
    rb_put(&BarcodeRecvBuf, barcode.BarRx_data[0]);
}




/* Message analay -------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief 바코드 메시지 받은 것을 분석하여 실제 바코드 값만 추출한다.
 */
void checkBarcodeMessage(void)
{
    static uint8_t  BarcodeTempBuf[NUMBER_OF_BARCODE_CHARACTERS];
    static uint32_t BarcodeInitTime;
    static uint8_t  BarcodeCount;
    uint8_t err = FALSE;
    uint8_t data;
    uint16_t cnt;

    // Barcode process
    cnt = BarcodeRecvBuf.head - BarcodeRecvBuf.tail;
    for (int i=0; i < cnt; i++)
    {
        data = rb_get(&BarcodeRecvBuf, &err);
        BarcodeInitTime = HAL_GetTick();

        if(err == TRUE)     // error
            break;

        // 바코드 길이 제한
        if(BarcodeCount > NUMBER_OF_BARCODE_CHARACTERS)
            data = 0;

        // Check ASCII Char
        if(BARCODE_START_ASCII <= data && BARCODE_END_ASCII >= data)
        {
            // Barcode read 시작 시간
            if(BarcodeCount == 0)
                barcode.readWaitTime = HAL_GetTick();

            BarcodeTempBuf[BarcodeCount++] = data;
        }
    }


    // Barcode receive complete (After 100ms)
    if(HAL_GetTick() - barcode.readWaitTime > BARCODE_RX_COMPLETE_WAIT_TIMEOUT && barcode.readWaitTime != 0 && HAL_GetTick() > 30)
    {
        barcode.readWaitTime = 0;
        BarcodeCount = 0;
        memcpy(barcode.data, BarcodeTempBuf, sizeof(barcode.data));     // Copy
        memset(BarcodeTempBuf, 0, sizeof(BarcodeTempBuf));              // Init


        #if BARCODE_REALTIME_EVENT_ACTIVE

        StatusOutput = rBarcode;
        putRealtimeData(&realPollStruct);
        
        #endif
        
        presetBarcode.flag_barcode_read_complete = 1;    // Barcode read complete flag
    }

    // Clear temp buf after 300ms
    if(BarcodeTempBuf[0] != 0 && HAL_GetTick() - BarcodeInitTime > 300 && HAL_GetTick() > 300)
    {
        BarcodeCount = 0;
        memset(BarcodeTempBuf, 0, sizeof(BarcodeTempBuf));          // Init
    }
}


/**
 * @brief Barcode에 따라 Preset 변경
 */
void barcodeChangePreset(void)
{
	uint8_t judge = FALSE;
    uint64_t maskBit;
	t_barcode_settings* BarcodeStruct;
    char barcodeData[65] = {0, };

	// Assgin Barcode pointer (Preset, Model barcode)
    BarcodeStruct = &presetBarcode;
    
    if(Para[PRESET_CHANGE_BY_BARCODE].val)
	{
		// Barcode index 별로 분석
		for (uint8_t index = 0; index < NUMBER_OF_BARCODE_INDEX; index++)
		{
            judge = FALSE;
            memcpy(barcodeData, BarcodeStruct->barcode[index].d_barcode, sizeof(BarcodeStruct->barcode[index].d_barcode));

			// 길이 확인
			if(strlen(barcodeData))
			{
                // Mask bit 확인 (Mask bit가 1인 경우 확인하지 않는 자리수)
                maskBit = BarcodeStruct->barcode[index].mask;

                // Mask bit가 모두 0일 때 모두 활성화
                if(maskBit == 0)
                    maskBit = 0xFFFFFFFFFFFFFFFF;

                for(uint8_t bit = 0; bit < NUMBER_OF_BARCODE_CHARACTERS; bit++)
                {
                    // 확인해야할 Bit인 경우
                    if(maskBit & (((uint64_t)0x01) << bit))
                    {
                        // 다를 경우 Break
                        if(BarcodeStruct->barcode[index].d_barcode[bit] != barcode.data[bit])
                        {
                            judge = FALSE;
                            break;
                        }
                        else
                        {
                            judge = TRUE;
                        }
                    }
                }

				// Correct data
				if(judge == TRUE)
				{
                    // Preset change
                    if(BarcodeStruct->barcode[index].pm >= 1 && BarcodeStruct->barcode[index].pm <= PRESET_INDEX_SIZE + 2)
                        Processing_number = BarcodeStruct->barcode[index].pm;

                    Set_Torque_variable();						// 변경된 torque 반영..
                    
                    StatusOutput = rPRESET_CHANGE;
                    ClearSendData_Init(1);
                    putRealtimeData(&realPollStruct);

                    break;
				}
			}
		}
	}

	presetBarcode.flag_barcode_read_complete = 0;
}


/**
 * @brief Barcode 저장용 모드에서 데이터를 읽고 저장
 */
void barcodeReadForMem(t_barcode_settings* barcodeStr)
{
    // Barcode 저장용 읽기 시작
    if (barcodeStr->flag_scan_barcode_index)
    {
        // 이전 바코드 데이터 임시 저장
        memcpy(&barcodeStr->barcodeTemp, &barcodeStr->barcode[barcodeStr->flag_scan_barcode_index - 1], sizeof(t_barcode));

        // Init
        memset(&barcodeStr->barcode[barcodeStr->flag_scan_barcode_index - 1].d_barcode, 0, NUMBER_OF_BARCODE_CHARACTERS);
        barcodeStr->barcode[barcodeStr->flag_scan_barcode_index - 1].mask = 0;

        // Copy barcode
        memcpy(&barcodeStr->barcode[barcodeStr->flag_scan_barcode_index - 1].d_barcode, barcode.data, NUMBER_OF_BARCODE_CHARACTERS);

        // Barcode index
        barcodeStr->barcedTempIndex = barcodeStr->flag_scan_barcode_index;
    }
}







/* Operation -------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief 바코드 트리거로 읽는 것 실행 판단
 */
void BarcodeReadTrigger(void)
{
    static uint8_t StartEdg[2] = {0, };
    static uint32_t BarData[2];
    static uint8_t TrigerCount;
    static uint8_t BarcodeStartOn;          // Barcode 사용 상태
    static uint32_t BarcodePushTime;        // 첫번째 레버 동작 시간
    static uint32_t BarcodeGpioTime;        // Barcode 시작 시간

    StartEdg[0] = StartEdg[1];
    StartEdg[1] = !HAL_GPIO_ReadPin(DR_START_IN_GPIO_Port, DR_START_IN_Pin);

    // Check 레버 Rising edge
    if(StartEdg[0] < StartEdg[1] && BarcodeStartOn == 0)
    {
        TrigerCount++;

        if(TrigerCount == 1)
            BarcodePushTime = HAL_GetTick();

        // Lock + start 레버 동작 시 바로 Trigger 동작
        if(Flag.Lock && TrigerCount)
            TrigerCount = 2;
    }

    // Read Barcode (Limit 300ms)
    if(TrigerCount == 2 && HAL_GetTick() - BarcodePushTime < 300)
    {
        BarcodeStartOn = 1;
        BarcodeGpioTime = HAL_GetTick();
        HAL_GPIO_WritePin(BAR_START_GPIO_Port, BAR_START_Pin, 0);

        TrigerCount = 0;
    }

    // Read off (2500ms)
    if(BarcodeStartOn && HAL_GetTick() - BarcodeGpioTime > 2500)
    {
        BarcodeStartOn = 0;
        HAL_GPIO_WritePin(BAR_START_GPIO_Port, BAR_START_Pin, 1);
    }

    // Init Triger count
    if(TrigerCount != 0 && HAL_GetTick() - BarcodePushTime > 400)
        TrigerCount = 0;


    // Check Barcode Good read (Barcode start GPIO)
    BarData[0] = BarData[1];
    BarData[1] = HAL_GPIO_ReadPin(BAR_GOODREAD_GPIO_Port, BAR_GOODREAD_Pin);
    if(BarData[0] < BarData[1])
        barcode.readWaitTime = HAL_GetTick();
}






/* Memory -------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief Read all Barcode list data 
 * */
void ReadBarcodeDataAll(t_barcode_settings* BarcodeStruct, uint32_t base_address)
{
    // 바코드 데이터 전체 읽기
    readMemoryData(base_address, 0, sizeof(t_barcode), (uint8_t*)&BarcodeStruct->barcode[0], sizeof(t_barcode) * NUMBER_OF_BARCODE_INDEX);


    // Check barcode data
    if(BarcodeStruct->barcode[0].pm == 65535 || BarcodeStruct->barcode[1].pm == 65535)
    {
        // Clear barcode data
        memset(&BarcodeStruct->barcode[0], 0, sizeof(t_barcode) * NUMBER_OF_BARCODE_INDEX);
    }
}


// Barcode save in memory
void WriteBarcodeData(uint32_t baseAddress, uint8_t index, t_barcode_settings* BarcodeStruct)
{
    // 바코드 데이터 전체 Write
    saveMemoryData(baseAddress, 0, sizeof(t_barcode), (uint8_t*)&BarcodeStruct->barcode[0], sizeof(t_barcode) * NUMBER_OF_BARCODE_INDEX);
}


// 1개의 List만 초기화
void Clear_BarcodeData(int no, t_barcode_settings *barcodeStruct)
{
    // Check index limit
    if(no <= NUMBER_OF_BARCODE_INDEX)
    {
        // 바코드 정보 초기화
        memset(&barcodeStruct->barcode[no - 1], 0, sizeof(t_barcode));

        barcodeStruct->save_data_no = no;
    }
}


// 모든 Barocdee List 초기화
void ClearAll_BarcodeData(t_barcode_settings *barcodeStruct, uint32_t baseAddress)
{
    // Init Barcode data
    memset(&barcodeStruct->barcode[0], 0, sizeof(t_barcode) * NUMBER_OF_BARCODE_INDEX);

    // 바코드 정보 메모리에 저장
    saveMemoryData(baseAddress, 0, sizeof(t_barcode), (uint8_t*)&barcodeStruct->barcode[0], sizeof(t_barcode) * NUMBER_OF_BARCODE_INDEX);

    barcodeStruct->flag_scan_barcode_index = 0;
    barcodeStruct->flag_reset_all = 0;        
}












/* External communication -------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  Write barcode command data by communication
 * 
 * @param  address: Barcode address
 * @param  data: Barcode data
 * @return retval: -1: Error,  0: OK
 * 
 * */
int8_t presetBarcodeSetWrite(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
    uint16_t fixAddress = 0;
    uint8_t  index = 0;
    uint8_t  num;
    uint8_t  barcodeParaSize;


	// Check address
    if(address >= BARCODE_READ_ACTIVE)
    {
        // Barcode address arrange
        if(address >= BARCODE_PRESET1)
        {
            barcodeParaSize = BARCODE_PRESET2 - BARCODE_PRESET1;                // 바코드 인덱스 1개 파라미터 크기
            index = (address - BARCODE_PRESET1) / barcodeParaSize;              // 바코드의 인덱스를 구한다
            num = address - BARCODE_PRESET1 - index * barcodeParaSize;          // 바코드의 읽고자 하는 파라미터 종류를 구한다.
            
            fixAddress = BARCODE_PRESET1 + num;                                 // 바코드 인덱스 1의 주소
        }

        // Set barcode data
        // Barcode read active
        if(address == BARCODE_READ_ACTIVE)
        {
            if(data <= NUMBER_OF_BARCODE_INDEX)
                presetBarcode.flag_scan_barcode_index = data;               // Barcode 값을 변경하고자하는 preset 번호 활성화
        }

        // 바코드 이전 데이터 저장 결정
        if(address == BARCODE_SETTING_SAVE)
        {
            // 바코드 설정 변경 취소 - 이전 바코드 데이터 설정
            if(data == 0 && presetBarcode.barcedTempIndex)
            {
                memcpy(&presetBarcode.barcode[presetBarcode.barcedTempIndex - 1], &presetBarcode.barcodeTemp, sizeof(t_barcode));
                presetBarcode.barcedTempIndex = 0;
            }
            else
            {
                memset(&presetBarcode.barcodeTemp, 0, sizeof(t_barcode));
                presetBarcode.barcedTempIndex = 0;
            }
        }

        // Barcode index clear
        if(address == BARCODE_INDEX_CLEAR)
        {
            if(data == 0)
                presetBarcode.flag_reset_all = 1;                       // Barcode list clear ALL
            else
				Clear_BarcodeData(data, &presetBarcode);			    // delete current barcode list
        }


        // Barcode preset
        if(fixAddress == BARCODE_PRESET1)
        {
            if(data <= PRESET_INDEX_SIZE + 2)
            {
                presetBarcode.barcode[index].pm = data;

                // 해당 Index 저장
                presetBarcode.save_data_no = index + 1;
            }
        }

        // Barcode mask bit
        switch (fixAddress)
        {
            case BARCODE_MASK_BIT1_1:
                presetBarcode.barcode[index].mask &= 0x0000FFFFFFFFFFFF;
                presetBarcode.barcode[index].mask |= ((uint64_t)data << 48);

                presetBarcode.save_data_no = index + 1;
            break;

            case BARCODE_MASK_BIT1_2:
                presetBarcode.barcode[index].mask &= 0xFFFF0000FFFFFFFF;
                presetBarcode.barcode[index].mask |= ((uint64_t)data << 32);

                presetBarcode.save_data_no = index + 1;
            break;

            case BARCODE_MASK_BIT1_3:
                presetBarcode.barcode[index].mask &= 0xFFFFFFFF0000FFFF;
                presetBarcode.barcode[index].mask |= ((uint64_t)data << 16);
                
                presetBarcode.save_data_no = index + 1;
            break;

            case BARCODE_MASK_BIT1_4:
                presetBarcode.barcode[index].mask &= 0xFFFFFFFFFFFF0000;
                presetBarcode.barcode[index].mask |= ((uint64_t)data << 0);

                presetBarcode.save_data_no = index + 1;
            break;
        }


        // Barcode data
        if(fixAddress >= BARCODE_DATA_1_1 && fixAddress <= BARCODE_DATA_1_32)
        {
            // Check data
            if(data < BARCODE_START_ASCII && data > BARCODE_END_ASCII)
                data = 0;

            fixAddress = fixAddress - BARCODE_DATA_1_1;
            presetBarcode.barcode[index].d_barcode[fixAddress * 2] = data >> 8;
            presetBarcode.barcode[index].d_barcode[fixAddress * 2 + 1] = data & 0xFF;

            // 해당 Index 저장
            presetBarcode.save_data_no = index + 1;
        }
    }

	return retval;
}


/**
 * @brief  Read barcode command data by communication
 * 
 * @param  address: Barcode address
 * @return retval: barcode data
 * 
 * */
uint16_t presetBarcodeSetRead(uint16_t address)
{
	uint16_t retval = 0;
    uint16_t fixAddress = 0;
    uint8_t  index = 0;
    uint8_t  num;
    uint8_t  barcodeParaSize;

    // Check address
    if(address >= BARCODE_READ_ACTIVE)
    {
        // Barcode address arrange
        if(address >= BARCODE_PRESET1)
        {
            barcodeParaSize = BARCODE_PRESET2 - BARCODE_PRESET1;                // 바코드 인덱스 1개 파라미터 크기
            index = (address - BARCODE_PRESET1) / barcodeParaSize;              // 바코드의 인덱스를 구한다
            num = address - BARCODE_PRESET1 - index * barcodeParaSize;          // 바코드의 읽고자 하는 파라미터 종류를 구한다.
            
            fixAddress = BARCODE_PRESET1 + num;                                 // 바코드 인덱스 1의 주소로 변환
        }

        // Get barcode data
        // Barcode read active
        if(address == BARCODE_READ_ACTIVE)
        {
            retval = presetBarcode.flag_scan_barcode_index;
        }

        // Barcode preset
        if(fixAddress == BARCODE_PRESET1)
        {
            retval = presetBarcode.barcode[index].pm;
        }

        // Barcode mask bit
        switch (fixAddress)
        {
            case BARCODE_MASK_BIT1_1:
                retval = (presetBarcode.barcode[index].mask >> 48) & 0xFFFF;
            break;

            case BARCODE_MASK_BIT1_2:
                retval = (presetBarcode.barcode[index].mask >> 32) & 0xFFFF;
            break;

            case BARCODE_MASK_BIT1_3:
                retval = (presetBarcode.barcode[index].mask >> 16) & 0xFFFF;
            break;

            case BARCODE_MASK_BIT1_4:
                retval = (presetBarcode.barcode[index].mask >> 0) & 0xFFFF;
            break;
        }

        // Barcode data
        if(fixAddress >= BARCODE_DATA_1_1 && fixAddress <= BARCODE_DATA_1_32)
        {
            fixAddress = fixAddress - BARCODE_DATA_1_1;
            retval = ((uint16_t)(presetBarcode.barcode[index].d_barcode[fixAddress * 2] << 8)) + presetBarcode.barcode[index].d_barcode[fixAddress * 2 + 1];
        }
    }

	return retval;
}


/**
 * @brief  Write barcode command data by communication
 * 
 * @param  address: Barcode address
 * @param  data: Barcode data
 * @return retval: -1: Error,  0: OK
 * 
 * */
int8_t modelBarcodeSetWrite(uint16_t address, uint16_t data)
{
	int8_t retval = 0;
    uint16_t fixAddress = 0;
    uint8_t  index = 0;
    uint8_t  num;
    uint8_t  barcodeParaSize;


	// Check address
    if(address >= MODEL_BARCODE_READ_ACTIVE)
    {
        // Barcode address arrange
        if(address >= MODEL_BARCODE_PRESET1)
        {
            barcodeParaSize = MODEL_BARCODE_PRESET2 - MODEL_BARCODE_PRESET1;          // 바코드 인덱스 1개 파라미터 크기
            index = (address - MODEL_BARCODE_PRESET1) / barcodeParaSize;              // 바코드의 인덱스를 구한다
            num = address - MODEL_BARCODE_PRESET1 - index * barcodeParaSize;          // 바코드의 읽고자 하는 파라미터 종류를 구한다.
            
            fixAddress = MODEL_BARCODE_PRESET1 + num;                                 // 바코드 인덱스 1의 주소
        }


        // Set barcode data
        // Barcode read active
        if(address == MODEL_BARCODE_READ_ACTIVE)
        {
            if(data <= NUMBER_OF_BARCODE_INDEX)
                modelStepBarcode.flag_scan_barcode_index = data;
        }

        // 바코드 이전 데이터 저장 결정
        if(address == MODEL_BARCODE_SETTING_SAVE)
        {
            // 바코드 설정 변경 취소 - 이전 바코드 데이터 설정
            if(data == 0 && modelStepBarcode.barcedTempIndex)
            {
                memcpy(&modelStepBarcode.barcode[modelStepBarcode.barcedTempIndex - 1], &modelStepBarcode.barcodeTemp, sizeof(t_barcode));
                modelStepBarcode.barcedTempIndex = 0;
            }
            else
            {
                memset(&modelStepBarcode.barcodeTemp, 0, sizeof(t_barcode));
                modelStepBarcode.barcedTempIndex = 0;
            }
        }

        // Barcode index clear
        if(address == MODEL_BARCODE_INDEX_CLEAR)
        {
            if(data == 0)
                modelStepBarcode.flag_reset_all = 1;                     // Barcode list clear ALL
            else
				Clear_BarcodeData(data, &modelStepBarcode);			    // delete current barcode list
        }


        // Barcode preset
        if(fixAddress == MODEL_BARCODE_PRESET1)
        {
            if(data <= PRESET_INDEX_SIZE + 2)
            {
                modelStepBarcode.barcode[index].pm = data;
                modelStepBarcode.save_data_no = index + 1;
            }
        }

        // Barcode mask bit
        switch (fixAddress)
        {
            case MODEL_BARCODE_MASK_BIT1_1:
                modelStepBarcode.barcode[index].mask &= 0x0000FFFFFFFFFFFF;
                modelStepBarcode.barcode[index].mask |= ((uint64_t)data << 48);

                modelStepBarcode.save_data_no = index + 1;
            break;

            case MODEL_BARCODE_MASK_BIT1_2:
                modelStepBarcode.barcode[index].mask &= 0xFFFF0000FFFFFFFF;
                modelStepBarcode.barcode[index].mask |= ((uint64_t)data << 32);
                
                modelStepBarcode.save_data_no = index + 1;
            break;

            case MODEL_BARCODE_MASK_BIT1_3:
                modelStepBarcode.barcode[index].mask &= 0xFFFFFFFF0000FFFF;
                modelStepBarcode.barcode[index].mask |= ((uint64_t)data << 16);

                modelStepBarcode.save_data_no = index + 1;
            break;

            case MODEL_BARCODE_MASK_BIT1_4:
                modelStepBarcode.barcode[index].mask &= 0xFFFFFFFFFFFF0000;
                modelStepBarcode.barcode[index].mask |= ((uint64_t)data << 0);

                modelStepBarcode.save_data_no = index + 1;
            break;
        }

        // Barcode data
        if(fixAddress >= MODEL_BARCODE_DATA_1_1 && fixAddress <= MODEL_BARCODE_DATA_1_32)
        {
            // Check data
            if(data < BARCODE_START_ASCII && data > BARCODE_END_ASCII)
                data = 0;

            fixAddress = fixAddress - MODEL_BARCODE_DATA_1_1;
            modelStepBarcode.barcode[index].d_barcode[fixAddress * 2] = data >> 8;
            modelStepBarcode.barcode[index].d_barcode[fixAddress * 2 + 1] = data & 0xFF;

            modelStepBarcode.save_data_no = index + 1;
        }

        // 해당 Index 저장
        modelStepBarcode.save_data_no = index + 1;
    }

	return retval;
}


/**
 * @brief  Read barcode command data by communication
 * 
 * @param  address: Barcode address
 * @return retval: barcode data
 * 
 * */
uint16_t modelBarcodeSetRead(uint16_t address)
{
	uint16_t retval = 0;
    uint16_t fixAddress = 0;
    uint8_t  index = 0;
    uint8_t  num;
    uint8_t  barcodeParaSize;

    // Check address
    if(address >= MODEL_BARCODE_READ_ACTIVE)
    {
        // Barcode address arrange
        if(address >= MODEL_BARCODE_PRESET1)
        {
            barcodeParaSize = MODEL_BARCODE_PRESET2 - MODEL_BARCODE_PRESET1;          // 바코드 인덱스 1개 파라미터 크기
            index = (address - MODEL_BARCODE_PRESET1) / barcodeParaSize;              // 바코드의 인덱스를 구한다
            num = address - MODEL_BARCODE_PRESET1 - index * barcodeParaSize;          // 바코드의 읽고자 하는 파라미터 종류를 구한다.
            
            fixAddress = MODEL_BARCODE_PRESET1 + num;                                 // 바코드 인덱스 1의 주소로 변환
        }

        // Get barcode data
        // Barcode read active
        if(address == MODEL_BARCODE_READ_ACTIVE)
        {
            retval = modelStepBarcode.flag_scan_barcode_index;
        }

        // Barcode preset
        if(fixAddress == MODEL_BARCODE_PRESET1)
        {
            retval = modelStepBarcode.barcode[index].pm;
        }

        // Barcode mask bit
        switch (fixAddress)
        {
            case MODEL_BARCODE_MASK_BIT1_1:
                retval = (modelStepBarcode.barcode[index].mask >> 48) & 0xFFFF;
            break;

            case MODEL_BARCODE_MASK_BIT1_2:
                retval = (modelStepBarcode.barcode[index].mask >> 32) & 0xFFFF;
            break;

            case MODEL_BARCODE_MASK_BIT1_3:
                retval = (modelStepBarcode.barcode[index].mask >> 16) & 0xFFFF;
            break;

            case MODEL_BARCODE_MASK_BIT1_4:
                retval = (modelStepBarcode.barcode[index].mask >> 0) & 0xFFFF;
            break;
        }

        // Barcode data
        if(fixAddress >= MODEL_BARCODE_DATA_1_1 && fixAddress <= MODEL_BARCODE_DATA_1_32)
        {
            fixAddress = fixAddress - MODEL_BARCODE_DATA_1_1;
            retval = ((uint16_t)(modelStepBarcode.barcode[index].d_barcode[fixAddress * 2] << 8)) + modelStepBarcode.barcode[index].d_barcode[fixAddress * 2 + 1];
        }
    }

	return retval;
}
