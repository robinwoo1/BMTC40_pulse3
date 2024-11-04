/**
  ******************************************************************************
  * File Name          : tdc.c
  * Description        : tdc program body
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main_bg.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "variable.h"
#include "backup.h"
#include "display.h"
#include "timer.h"
#include "wifiTask.h"
#include "backup.h"
#include "modbus.h"
#include "advancedPlus.h"
#include "virtualPreset.h"
#include "realtimeMonitoring.h"
#include "totalScrewCount.h"


/* Extern ------------------------------------------------------------------*/

extern WifiAPTypeDef     WifiAPSet;              // AP Config: SSID, Password, Security of AP
extern NetworkTypeDef    WifiEthSet;             // Ethernet Config: IP, Gateway, Netmask



/* Variables ------------------------------------------------------------------*/

char* const Menu_Error[] = {    // max 20 character
    "DUMMY",
    "NO CONCT SENSOR",          //[101]
    "NO CALIBRATION",           //[102]
    "TORQUE OVER",              //[103]
    "UNDER VOLTAGE",            //[104]
    "CHECK SENSOR",             //[105]
    "ENCODER OPEN",             //[106]
    "NONE",                     //[107]
    "BACKUP DATA R/W",          //[108]
    "OVER CURRENT",             //[109]
    "CURRENT OFFSET",           //[110]
    "BAT UNDER VOLTAGE",        //[111]
    "OVER SPEED",               //[112]
    "DRIVER PARAMETER",         //[113]
    "UNKNOWN DRIVER",           //[114]
    "NOT RECOGNIZE CTL",        //[115]
    "IO READ ERROR",            //[116]
    "NO SPEED",                 //[118]
    "BARCODE R/W ERR",          //[119]
    "WIFI COMM FAIL",           //[120]
    "SD CARD FAIL",             //[121]
    "USB COMM FAIL",            //[122]
    "WIFI INIT FAIL",           //[123]
    "NO RAMPUP TQ",             //[125]
    "CHECK BATTERY",            //[127]
    "PARAMETER R/W",            //[200]
    "PARAMETER CHKSUM",         //[201]
    "MULTI SEQUEN PGM",         //[220]
    "FASTENING TIMOUT",         //[300]
    "LOOSENING TIMOUT",         //[301]
    "MODEL CANCEL",             //[303]
    "STALL TIME OVER",          //[304]
    "OVER TQ BEFO RAMPUP",      //[305]
    "BIT SOCKET TRAY",          //[309]     // 현재 없음
    "TIME OVER COUNT",          //[310]
    "MISSING COUNT",            //[311]
    "CROWFOOT TQ ERR",          //[329]
    "MIN ANGLE",                //[330]
    "TARGET ANGLE SET",         //[331]
    "MAX ANGLE",                //[332]
    "FASTENING STOP",           //[333]
    "FIND ENGAGING TQ",         //[334]
    "C_TORQUE LIMIT",           //[335]
    "FASTEN OVER TQ",           //[336]
    "TqUP DURING F_SPEE",       //[337]
    "THREADTAP OVER MAX",       //[338]      // TORQUE 
    "THREADTAP MIN MAX",        //[339]      // RANGE
    "SEATING TQ ERROR",         //[350]
    "SEATING ANGLE ERR",        //[351]
    "COMPENSATION OVER",        //[352]
    "CLAMP TQ ERROR",           //[353]
    "A2 ANGLE ERROR",           //[354]
    "NO FINISH PREVAIL",        //[355]
    "PREVAILING TQ ERR",        //[356]
    "ADVANCED SET ERR",         //[358]
    "NETWORK INIT FAIL",        //[400]
    "SOCKET ERROR",             //[401]
    "OVER TEMP MOTOR",          //[500]
    "OVER TEMP BAT",            //[501]
    "OVER TEMP PCB",            //[503]
	"Total screw over"          //[505]

};

char* const Menu_Unit[] = {
    "Kgf.cm",
    "Kgf.m ",
    "N.m   ",
    "cN.m  ",
    "lbf.in",
    "ozf.in",
    "lbf.ft"
};


/* Functions ------------------------------------------------------------------*/

void key_process(void)
{
    static uint8_t Cnt_resetOrder    = 0;
    static uint8_t inputdata_edg0[2] = {1,1};
    static uint8_t inputdata_edg1[2] = {1,1};
    static uint8_t inputdata_edg2[2] = {1,1};
    static uint8_t inputdata_edg3[2] = {1,1};

    int32_t tmp=0;
    uint32_t paraU32Val;


    // Key F1 Lock
    if(Para[LCD_KEY_LOCK].val == 1 && current_mode == _OPERATION)
    {
        ReceiveKeyData[0] = 0;
    }
    // Key All lock (Alarm일 때는 가능)
    else if(Para[LCD_KEY_LOCK].val == 2 && current_mode != _ALARM)
    {
        ReceiveKeyData[0] = 0;
        ReceiveKeyData[1] = 0;
        ReceiveKeyData[2] = 0;
        ReceiveKeyData[3] = 0;
    }
    // Key All lock (무조건)
    else if(Para[LCD_KEY_LOCK].val == 3)
    {
        ReceiveKeyData[0] = 0;
        ReceiveKeyData[1] = 0;
        ReceiveKeyData[2] = 0;
        ReceiveKeyData[3] = 0;
    }

    inputdata_edg0[0] = inputdata_edg0[1];
    inputdata_edg0[1] = ReceiveKeyData[0];
    inputdata_edg1[0] = inputdata_edg1[1];
    inputdata_edg1[1] = ReceiveKeyData[1];
    inputdata_edg2[0] = inputdata_edg2[1];
    inputdata_edg2[1] = ReceiveKeyData[2];
    inputdata_edg3[0] = inputdata_edg3[1];
    inputdata_edg3[1] = ReceiveKeyData[3];


    switch(current_mode) 
    {
        case _OPERATION:
            // mode key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
                current_mode = _MONITORING_MODE;
            // Disp key
            if (inputdata_edg1[0] < inputdata_edg1[1])      // high edg
                current_mode = _MONITORING_NET;
            // down key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg
            {
                if(Flag.Run == OFF)
                {
                    Processing_number--;
                    if (Processing_number < 1)
                        Processing_number = PRESET_INDEX_SIZE + 2;

                    paraU32Val = (getParaValSingle(DISPLAY_PRESET_NUM_H) << 16) + getParaValSingle(DISPLAY_PRESET_NUM_L);

                    if(paraU32Val != 0)
                    {
                        for(uint8_t i=0; i < PRESET_INDEX_SIZE + 1; i++)
                        {
                            // BIT 단위로 preset 가능 체크 (Multi A까지)
                            if((paraU32Val >> (Processing_number - 1)) & 0x01)
                                break;
                            else
                            {
                                Processing_number--;
                                if (Processing_number < 1)
                                    Processing_number = PRESET_INDEX_SIZE + 2;
                            }
                        }
                    }

                    Set_Torque_variable();

                    StatusOutput = rPRESET_CHANGE;
                    ClearSendData_Init(ON);
                    putRealtimeData(&realPollStruct);
                }
            }
            // up key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                if(Flag.Run == OFF)
                {
                    Processing_number++;
                    if (Processing_number > PRESET_INDEX_SIZE + 2)
                        Processing_number = 1;

                    paraU32Val = (getParaValSingle(DISPLAY_PRESET_NUM_H) << 16) + getParaValSingle(DISPLAY_PRESET_NUM_L);

                    if(paraU32Val != 0)
                    {
                        for(uint8_t i=0; i < PRESET_INDEX_SIZE + 1; i++)
                        {
                            // BIT 단위로 preset 가능 체크
                            if((paraU32Val >> (Processing_number - 1)) & 0x01)
                                break;
                            else
                            {
                                Processing_number++;
                                if (Processing_number > PRESET_INDEX_SIZE + 2)
                                    Processing_number = 1;
                            }
                        }
                    }

                    Set_Torque_variable();
                    
                    StatusOutput = rPRESET_CHANGE;
                    ClearSendData_Init(ON);
                    putRealtimeData(&realPollStruct);
                }
            }
        break;
        case _MONITORING_NET:
            // mode key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
                current_mode = _OPERATION;
            // down key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg
            {
                current_mode--;
                if (current_mode < _MONITORING_NET)
                    current_mode = _MONITORING_NET;
            }
            // up key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                current_mode++;
                if (current_mode > _MONITORING_VER)
                    current_mode = _MONITORING_VER;
            } 

            Cnt_resetOrder = 0;     // Init parameter 초기화 카운트

        break;
        case _MONITORING_SER:
            // mode key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
                current_mode = _OPERATION;
            // down key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg
            {
                current_mode--;
                if (current_mode < _MONITORING_NET)
                    current_mode = _MONITORING_NET;
            }
            // up key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                current_mode++;
                if (current_mode > _MONITORING_VER)
                    current_mode = _MONITORING_VER;
            }        
        break;
        case _MONITORING_SSID:
            // mode key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
                current_mode = _OPERATION;
            // down key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg
            {
                current_mode--;
                if (current_mode < _MONITORING_NET)
                    current_mode = _MONITORING_NET;
            }
            // up key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                current_mode++;
                if (current_mode > _MONITORING_VER)
                    current_mode = _MONITORING_VER;                
            }        
        break;        
        case _MONITORING_DRIVER:
            // mode key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
                current_mode = _OPERATION;
            // Disp key
            if (inputdata_edg1[0] < inputdata_edg1[1])      // high edg
            {
            }
            // down key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg
            {
                current_mode--;
                if (current_mode < _MONITORING_NET)
                    current_mode = _MONITORING_NET;
            }
            // up key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                current_mode++;
                if (current_mode > _MONITORING_VER)
                    current_mode = _MONITORING_VER;
            }        
        break;   
        case _MONITORING_VER:
            // mode key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
                current_mode = _OPERATION;
            // Disp key
            if (inputdata_edg1[0] < inputdata_edg1[1])      // high edg
            {
                Cnt_resetOrder++;

                // Hiden key (Para reset)
                if (Cnt_resetOrder > 15)
                {
                    Cnt_resetOrder      = 0;
                    SystemResetValue    = RESET_ALL_PARA;
                    Flag.ParaInitialize = ON;
                }
            }
            // down key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg
            {                
                current_mode--;
                if (current_mode < _MONITORING_NET)
                    current_mode = _MONITORING_NET;
            }
            // up key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                current_mode++;
                if (current_mode > _MONITORING_VER)
                    current_mode = _MONITORING_VER;
            }        
        break; 
        case _ALARM:
            // reset key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
                Flag.Reset = ON;
        break;
        case _SLEEP:
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
            {
                Flag_sleep_mode = OFF;
            }        
        break;
        case _MONITORING_MODE:
            // mode key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
            {
                current_mode = _OPERATION;

                if(Processing_number > PRESET_INDEX_SIZE)
                    Processing_number = 1;

                key_input_value = getParaVal(FASTEN_TORQUE1_H + PRESET_SIZE*(Processing_number -1));
            }
            // Speed key
            if (inputdata_edg1[0] < inputdata_edg1[1])      // high edg
            {
                current_mode = _SET_SPEED;

                if(Processing_number > PRESET_INDEX_SIZE)
                    Processing_number = 1;

                key_input_value = Para[FASTEN_SPEED1 + PRESET_SIZE*(Processing_number -1)].val;
                cursor_position = 1;
            }
            // Torq key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg  torque change
            {
                current_mode = _SET_TORQUE;

                if(Processing_number > PRESET_INDEX_SIZE)
                    Processing_number = 1;

                key_input_value = getParaVal(FASTEN_TORQUE1_H + PRESET_SIZE*(Processing_number -1));
                cursor_position = 0.001;
            }
            // -1 count key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                current_mode = _YES_NO;
            }                    
        break;
        case _YES_NO:
            // yes key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
            {
                current_mode = _OPERATION;
                if (!FlagFasten.BackCountForTouch)
                {
                    if (ScrewCountValue < Para[COUNT_SCREW].val)
                    {
                        ScrewCountValue++;			// screw counter
                        
                        StatusOutput = rScrewCount_D1;
                        ClearSendData_Init(ON);
                        putRealtimeData(&realPollStruct);
                        
                        FlagFasten.BackCountForTouch = ON;
                    }
                }
            }
            // no key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                current_mode = _OPERATION;
            }                   
        break;
        case _SET_TORQUE:
            // set key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
            {
                current_mode = _OPERATION;
                
                floatFrom16 floatData;
                floatData.fVal = key_input_value;
                SendSaveParameter(FASTEN_TORQUE1_H + PRESET_SIZE*(Processing_number - 1), floatData.raw.integer_high);
                SendSaveParameter(FASTEN_TORQUE1_L + PRESET_SIZE*(Processing_number - 1), floatData.raw.integer_low);
            }
            // shift key
            if (inputdata_edg1[0] < inputdata_edg1[1])      // high edg
            {
                // 1자리 씩 증가
                cursor_position *= 10;

                // 최대 토크 보다 자리수가 커질 경우 초기화
                if (cursor_position > setRange.maxTorqueUnitConvert)
                {
                    cursor_position = 0.001;
                }
            }
            // down key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg
            { 
                // decrese para value
                key_input_value -= cursor_position;

                tmp = key_input_value;
                if (tmp < setRange.minTorqueUnitConvert)                // min max check
                    key_input_value = setRange.minTorqueUnitConvert;    // Para[FASTEN_TORQUE1_H].min
                else if(tmp > setRange.maxTorqueUnitConvert) 
                    key_input_value = setRange.maxTorqueUnitConvert;                      
            }
            // up key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                // Increse parameter value
                key_input_value += cursor_position;

                tmp = key_input_value;
                if (tmp < setRange.minTorqueUnitConvert)                    // min max check
                    key_input_value = setRange.minTorqueUnitConvert;        // Para[FASTEN_TORQUE1_H].min
                else if(tmp > setRange.maxTorqueUnitConvert) 
                    key_input_value = setRange.maxTorqueUnitConvert;                  
            }                    
        break;
        case _SET_SPEED:
            // set key
            if (inputdata_edg0[0] < inputdata_edg0[1])      // high edg   memu
            {
                current_mode = _OPERATION;
                // CntHighButton = 0;

                if(Para[AUTO_SELECT_SPEED].val == OFF)
                    SendSaveParameter(FASTEN_SPEED1 + PRESET_SIZE*(Processing_number - 1), key_input_value);
            }
            // shift key
            if (inputdata_edg1[0] < inputdata_edg1[1])      // high edg
            {
                // 최대 Shift 자리수 결정
                cursor_position *= 10;

                if (cursor_position > Para[FASTEN_SPEED1].max)
                {
                    cursor_position = 1;
                }
            }
            // down key
            if (inputdata_edg2[0] < inputdata_edg2[1])      // high edg
            {       
                // decrese para value
                key_input_value -= cursor_position;

                tmp = key_input_value;
                if (tmp < Para[FASTEN_SPEED1].min)       // min max check
                    key_input_value = Para[FASTEN_SPEED1].min;
                else if(tmp > Para[FASTEN_SPEED1].max) 
                    key_input_value = Para[FASTEN_SPEED1].max;                
            }
            // up key
            if (inputdata_edg3[0] < inputdata_edg3[1])      // high edg
            {
                // Increse parameter value
                key_input_value += cursor_position;

                tmp = key_input_value;
                if (tmp < Para[FASTEN_SPEED1].min)       // min max check
                    key_input_value = Para[FASTEN_SPEED1].min;
                else if(tmp > Para[FASTEN_SPEED1].max) 
                    key_input_value = Para[FASTEN_SPEED1].max;                  
            }                    
        break;
        default:
        break;
    }
    
    // event가 다시 발생하는 것을 막는다.
    ReceiveKeyData[0] = 0;
    ReceiveKeyData[1] = 0;
    ReceiveKeyData[2] = 0;
    ReceiveKeyData[3] = 0;
}

void display_process(void)      // 100ms
{
    static uint16_t current_mode_edg[2];
    static uint8_t lcd_off_edg[2];
    static uint8_t cnt_inprocess;
    static uint8_t flag_change_mode;
    static uint32_t ReflashLCD;
    char make_packet[100];
    int len;
    uint16_t serial_tmp;

    Cnt_Oled_Packet = 0;

    current_mode_edg[0] = current_mode_edg[1];
    current_mode_edg[1] = current_mode;

    lcd_off_edg[0] = lcd_off_edg[1];
    lcd_off_edg[1] = Flag_LCD_off;      // Not use

    if (current_mode_edg[0] != current_mode_edg[1])
    {
        ReflashLCD = 0;
        flag_change_mode = ON;
    }
    else
        flag_change_mode = OFF;

    // Reflash lcd display
    if(current_mode == _OPERATION)
    {
        // 180초 마다 relash (Operation 화면)
        if(HAL_GetTick() - ReflashLCD > 180000)
        {
            flag_change_mode = ON;
            ReflashLCD = HAL_GetTick();
        }
    }


    // OLED LED에 색 변경
    Control_LED();

    if (lcd_off_edg[0] < lcd_off_edg[1])  // on.. lcd off
    {
        fillOLED(BLACK);
    }
    else
    {
        if (lcd_off_edg[0] > lcd_off_edg[1])  // off
            flag_change_mode = ON;

        switch(current_mode) 
        {
            case _OPERATION:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(28, 0,  FONT_10PT, RED,  BLACK, 9, (char*)"OPERATION");
                    drawStringXY(3,  82, FONT_8PT, GREEN, BLACK, 4, (char*)"MODE");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DISP");
                    drawStringXY(67, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DOWN");
                    drawStringXY(106, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"UP");

                    drawFillRectXY(90, 55, 126, 64, 1, BLACK, BLACK);   // clear block
                    sprintf(make_packet, "%6s", Menu_Unit[Para[UNIT].val]);
                    drawStringXY(93, 51, FONT_8PT, WHITE, BLACK, 6, (char*)make_packet);
                }

                Display_SetTorque(flag_change_mode);                                                						// 목표 토크
                DisPlay_CurrentTorque(flag_change_mode, fABS(Torque_cnv.fastenComplete_dsp_comp * getCrowFootRatio()));    	// 현재 토크 

                Display_PresetNo(flag_change_mode);                                                 						// 프리셋
                Display_CurrentRPM(flag_change_mode, w_rpm_display, deceleration_ratio);            						// 현재 RPM
                Display_Angle(flag_change_mode);                                                    						// 현재 Angle
                Display_Count(flag_change_mode);                                                    						// Screw count
            break;
            case _ALARM:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(28, 0, FONT_10PT, RED, BLACK, 5, (char*)"ALARM");

                    memset((char*)make_packet, 0, sizeof(make_packet));    // memory init
                    sprintf(make_packet, "[%3d]", system_err);
                    drawStringXY(1, 20, FONT_10PT, RED, BLACK, 5, (char*)make_packet);

                    len = sprintf(make_packet, "%s", Menu_Error[ErrorNo_Conversion(system_err)]);
                    drawStringXY(1, 40, FONT_10PT, RED, BLACK, len, (char*)make_packet);

                    // 2 Line text
                    if(system_err == THREADTAP_MAX_TORQUE_ERROR)            // 338
                        drawStringXY(1, 57, FONT_10PT, RED, BLACK, 6, (char*)"TORQUE");
                    else if(system_err == THREADTAP_MIN_MAX_RANGE_ERROR)    // 339
                        drawStringXY(1, 57, FONT_10PT, RED, BLACK, 5, (char*)"RANGE");
                    
                    drawStringXY(102, 82, FONT_8PT, GREEN, BLACK, 3, (char*)"RST");
                }
            break;
            case _YES_NO:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(28, 0, FONT_10PT, RED, BLACK, 9, (char*)"Question?");
                    drawStringXY(5, 82, FONT_8PT, GREEN, BLACK, 3, (char*)"YES");
                    drawStringXY(105, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"NO");

                    drawStringXY(20, 40, FONT_10PT, YELLOW, BLACK, 13, (char*)"Are you sure?");
                }
            break;
            case _SLEEP:
                if (flag_change_mode)      // 한번 실행
                {
                    //if (Flag_sleep_mode)
                        //Sleep(ON);
                    //else
                        //Sleep(OFF);
                    flag_change_mode = OFF;
                }

                if (!Flag_sleep_mode)
                {
                    //Sleep(OFF);
                    //initSSD1351();
                    current_mode = _OPERATION;
                }
            break;
            case _MONITORING_MODE:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(22, 0, FONT_10PT, RED, BLACK, 11, (char*)"INFORMATION");
                    drawStringXY(3, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"MODE");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"SPEE");
                    drawStringXY(66, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"TORQ");
                    drawStringXY(105, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"-1");

                    if (BatteryType == BATTERY_18V)
                    {
                        drawStringXY(20, 40, FONT_16PT, YELLOW, BLACK, 9, (char*)"Low power");
                    }
                    else
                    {
                        drawStringXY(15, 40, FONT_16PT, YELLOW, BLACK, 10, (char*)"High power");
                    }
                }
            break;            
            case _MONITORING_NET:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(20, 0, FONT_10PT, YELLOW, BLACK, 13, (char*)"Network  Info");
                    drawStringXY(3, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"MODE");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"    ");
                    drawStringXY(67, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DOWN");
                    drawStringXY(106, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"UP");

                    drawFillRectXY(90, 55, 126, 64, 1, BLACK, BLACK);   // clear block
                }

                if(HAL_GetTick() - ReflashLCD > 5000)
                {
                    ReflashLCD = HAL_GetTick();

                    if (WifiEthSet.DHCP == RSI_STATIC_IP_MODE)
                        drawStringXY(5, 20, FONT_8PT, WHITE, BLACK, 12, (char*)"MODE: STATIC");
                    else
                        drawStringXY(5, 20, FONT_8PT, WHITE, BLACK, 10, (char*)"MODE: DHCP");

                    // Gateway, Netmask, IP
                    len = sprintf(make_packet, "GW: %d.%d.%d.%d", WifiEthSet.GATEWAY[0],WifiEthSet.GATEWAY[1],WifiEthSet.GATEWAY[2],WifiEthSet.GATEWAY[3]);
                    drawStringXY(5, 46, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                    len = sprintf(make_packet, "NM: %d.%d.%d.%d", WifiEthSet.NETMASK[0],WifiEthSet.NETMASK[1],WifiEthSet.NETMASK[2],WifiEthSet.NETMASK[3]);
                    drawStringXY(5, 59, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                    len = sprintf(make_packet, "IP: %d.%d.%d.%d", WifiEthSet.DEVICE_IP[0],WifiEthSet.DEVICE_IP[1],WifiEthSet.DEVICE_IP[2],WifiEthSet.DEVICE_IP[3]);
                    drawStringXY(5, 33, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                }
            break;
            case _MONITORING_SER:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(20, 0, FONT_10PT, YELLOW, BLACK, 13, (char*)"Network  Info");
                    drawStringXY(3, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"MODE");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"    ");
                    drawStringXY(67, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DOWN");
                    drawStringXY(106, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"UP");

                    drawFillRectXY(90, 55, 126, 64, 1, BLACK, BLACK);   // clear block
                }

                if(HAL_GetTick() - ReflashLCD > 5000)
                {
                    ReflashLCD = HAL_GetTick();

                    len = sprintf(make_packet, "Port: %d", WifiEthSet.DEVICE_PORT);         // PORT
                    drawStringXY(5, 20, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                    len = sprintf(make_packet, "Mac address:");                             // MAC ADDRESS
                    drawStringXY(5, 34, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                    len = sprintf(make_packet, "%x.%x.%x.%x.%x.%x", WifiEthSet.MACADDRESS[0],WifiEthSet.MACADDRESS[1],WifiEthSet.MACADDRESS[2]
                                                                  , WifiEthSet.MACADDRESS[3],WifiEthSet.MACADDRESS[4],WifiEthSet.MACADDRESS[5]);
                    drawStringXY(5, 47, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                }
            break;
            case _MONITORING_SSID:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(13, 0, FONT_10PT, YELLOW, BLACK, 15, (char*)"Network AP Info");
                    drawStringXY(3, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"MODE");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"    ");
                    drawStringXY(67, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DOWN");
                    drawStringXY(106, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"UP");

                    drawFillRectXY(90, 55, 126, 64, 1, BLACK, BLACK);   // clear block

                    drawStringXY(5, 20, FONT_10PT, WHITE, BLACK, 5, (char*)"SSID:");
                    len = sprintf(make_packet, "%-16.16s", WifiAPSet.SSID);
                    drawStringXY(5, 33, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                    if(strlen(WifiAPSet.SSID) > 16)
                    {
                        len = sprintf(make_packet, "%-16.16s", WifiAPSet.SSID + 16);
                        drawStringXY(5, 45, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                    }
                }
            break;
            case _MONITORING_DRIVER:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(20, 0, FONT_10PT, YELLOW, BLACK, 12, (char*)"Driver State");
                    drawStringXY(3, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"MODE");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"    ");
                    drawStringXY(67, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DOWN");
                    drawStringXY(106, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"UP");

                    drawFillRectXY(90, 55, 126, 64, 1, BLACK, BLACK);   // clear block
                }

                if(HAL_GetTick() - ReflashLCD > 200)
                {
                    ReflashLCD = HAL_GetTick();

                    // Temperature
                    len = sprintf(make_packet, "Temperature:  %0.1f", Temperature_real_Flt_motor);
                    drawStringXY(5, 20, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                    
                    // display sensor value
                    if(Tq_SensorValue_Avg - SensorOffset - AD_OFFSET_VALUE > 0)
                        len = sprintf(make_packet, "TS: +%d   ", Tq_SensorValue_Avg - SensorOffset - AD_OFFSET_VALUE);      // display sensor value
                    else
                        len = sprintf(make_packet, "TS: %d   ", Tq_SensorValue_Avg - SensorOffset - AD_OFFSET_VALUE);       // display sensor value

                    drawStringXY(5, 35, FONT_10PT, WHITE, BLACK, len, (char*)make_packet); 

                    // 비트소켓트레이 연결 중이라는 것 표시
                    if(Flag_bitsocket_mode)
                        len = sprintf(make_packet, "Bit socket:  ON  ");
                    else
                        len = sprintf(make_packet, "Bit socket:  OFF ");

                    drawStringXY(5, 48, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);


                    len = sprintf(make_packet, "Cycle:  %lu ", screwCount.totalScrew);
                    drawStringXY(5, 61, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                }
            break;
            case _MONITORING_VER:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(6, 0, FONT_10PT, YELLOW, BLACK, 18, (char*)"Driver Information");
                    drawStringXY(3, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"MODE");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"    ");
                    drawStringXY(67, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DOWN");
                    drawStringXY(106, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"UP");

                    drawFillRectXY(90, 55, 126, 64, 1, BLACK, BLACK);   // clear block

                    len = sprintf(make_packet, "FW Ver: %d.%02d.%d", controllerPara.firmwareVersion[0], controllerPara.firmwareVersion[1], controllerPara.firmwareVersion[2]);     // version
                    drawStringXY(5, 20, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);

                    serial_tmp = ((uint16_t)dset[5] * 100) + ((uint16_t)dset[6]);
                    len = sprintf(make_packet, "S/N: %04d%d%04d", serial_tmp, PRODUCT_CODE, (uint16_t)(dset[7] * 100) + (uint16_t)dset[8]);     // serial number
                    drawStringXY(5, 35, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);

                    len = sprintf(make_packet, "Model: %s", Info_DrvModel[dset[4]].name);     // model name
                    drawStringXY(5, 48, FONT_10PT, WHITE, BLACK, len, (char*)make_packet);
                }
            break;
            case _SET_TORQUE:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(15, 0, FONT_10PT, WHITE, BLACK, 14, (char*)"Torque Setting");
                    drawStringXY(5, 82, FONT_8PT, GREEN, BLACK, 3, (char*)"SET");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"SHIF");
                    drawStringXY(67, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DOWN");
                    drawStringXY(106, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"UP");
                    drawFillRectXY(90, 55, 126, 64, 1, BLACK, BLACK);   // clear block
                }

                Display_SetTorque(flag_change_mode);                                                // 기존값
                DisPlay_CurrentTorque(flag_change_mode, key_input_value);                           // 변경값
                Display_PresetNo(flag_change_mode);                                                 // Preset number
                Display_CurrentShift(flag_change_mode, cursor_position, DISPLAY_SHIFT_FLOAT);       // 현재 shift
            break;
            case _SET_SPEED:
                if (flag_change_mode)      // 한번 실행
                {
                    fillOLED(BLACK);
                    drawStringXY(19, 0, FONT_10PT, WHITE, BLACK, 13, (char*)"Speed Setting");
                    drawStringXY(5, 82, FONT_8PT, GREEN, BLACK, 3, (char*)"SET");
                    drawStringXY(36, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"SHIF");
                    drawStringXY(67, 82, FONT_8PT, GREEN, BLACK, 4, (char*)"DOWN");
                    drawStringXY(106, 82, FONT_8PT, GREEN, BLACK, 2, (char*)"UP");
                    drawFillRectXY(90, 55, 126, 64, 1, BLACK, BLACK);   // clear block
                }

                if(Para[AUTO_SELECT_SPEED].val)
                    drawStringXY(20, 40, FONT_12PT, YELLOW, BLACK, 10, (char*)"AUTO SPEED");
                else
                    DisPlay_CurrentSpeed(flag_change_mode, key_input_value);

                Display_SetSpeed(flag_change_mode);
                Display_PresetNo(flag_change_mode);
                Display_CurrentShift(flag_change_mode, cursor_position, DISPLAY_SHIFT_INT);
            break;

            default:
            break;
        }
    }

    // 상단에 있으면 삭제이전에 실행이 되어서 표기가 않됨.
    if (current_mode == _OPERATION)
    {
        if ((cnt_inprocess++ > 4) || flag_change_mode)          // about 1sec
        {
            displayBattery(battery_level, flag_change_mode);
            displayAntena(WifiAPSet.RSSI, flag_change_mode);    // 0 : red - not connection, xx : white - connection
            cnt_inprocess = 0;
        }
    }

    Send_Oled_Data();       // send data
}

void Display_Angle(uint8_t disp)
{
    static float tmp_value_angle[2];
    char make_packet[25];
    int len;

    // 각도 표시 결정 (소수점 단위 계산으로 달라지는 것 보정)
    if(mPulse.After_Fastening_Start_abs == mPulse.After_Fastening_Start_ForA1 + mPulse.After_Fastening_Start_ForA2)
        DisplayAngle_forLCD = (uint16_t)(mPulse.After_Fastening_Start_ForA1 * MathConv.AngleConv_mPulseToDeg) + (uint16_t)(mPulse.After_Fastening_Start_ForA2 * MathConv.AngleConv_mPulseToDeg);
    else
        DisplayAngle_forLCD = mPulse.After_Fastening_Start_abs * MathConv.AngleConv_mPulseToDeg;


    tmp_value_angle[1] = DisplayAngle_forLCD;  // angle 


    if ((tmp_value_angle[0] != tmp_value_angle[1]) || disp)  // 값이 다를 때 반영한다.
    {
        drawFillRectXY(55, 64, 100, 75, 1, BLACK, BLACK);    // clear block..
        memset((char*)make_packet, 0, sizeof(make_packet));  // memory init
        len = sprintf(make_packet, "%.0f", tmp_value_angle[1]);
        drawStringXY(55, 64, FONT_8PT, YELLOW, BLACK, len, (char*)make_packet);
        tmp_value_angle[0] = tmp_value_angle[1];
    }
}

void Display_CurrentRPM(uint8_t disp, float rpm, float ratio)
{
    static uint16_t tmp_value_cRPM[2];
    char make_packet[25];
    int len;

    tmp_value_cRPM[1] = (uint16_t)rpm;  // current rpm

    if ((tmp_value_cRPM[0] != tmp_value_cRPM[1]) || disp)   // 값이 다를 때 반영한다.
    {
        drawFillRectXY(5, 64, 50, 75, 1, BLACK, BLACK);     // clear block..
        memset((char*)make_packet, 0, sizeof(make_packet)); // memory init
        len = sprintf(make_packet, "%d", (uint16_t)(rpm / ratio));
        drawStringXY(5, 64, FONT_8PT, YELLOW, BLACK, len, (char*)make_packet);
        tmp_value_cRPM[0] = tmp_value_cRPM[1];
    }
}


void Display_CurrentShift(uint8_t disp, float shift, uint8_t type)
{
    static float tmp_value_shift[2];
    char make_packet[25];
    int len;

    tmp_value_shift[1] = shift;  // current shift

    if ((tmp_value_shift[0] != tmp_value_shift[1]) || disp)   // 값이 다를 때 반영한다.
    {
        drawFillRectXY(5, 64, 50, 75, 1, BLACK, BLACK);     // clear block..
        memset((char*)make_packet, 0, sizeof(make_packet)); // memory init


        // Type 별 구분
        if(type)
            len = sprintf(make_packet, "%.3f", shift);      // Float - 소수점 3자리
        else
            len = sprintf(make_packet, "%.0f", shift);      // int - 소수점 X
        
        drawStringXY(5, 64, FONT_8PT, YELLOW, BLACK, len, (char*)make_packet);
        tmp_value_shift[0] = tmp_value_shift[1];
    }
}


void Display_Count(uint8_t disp)
{
    static uint16_t tmp_value_count[2];
    char make_packet[25];
    int len;

    tmp_value_count[1] = ScrewCountValue;  // current rpm

    if ((tmp_value_count[0] != tmp_value_count[1]) || disp)   // if value is different, change lcd value
    {
        drawFillRectXY(110, 64, 126, 75, 1, BLACK, BLACK);    // clear block..
        memset((char*)make_packet, 0, sizeof(make_packet));   // memory init
        len = sprintf(make_packet, "%2d ", (uint16_t)ScrewCountValue);
        drawStringXY(110, 64, FONT_8PT, YELLOW, BLACK, len, (char*)make_packet);
        tmp_value_count[0] = tmp_value_count[1];
    }
}

void Display_PresetNo(uint8_t disp)
{
    static uint16_t tmp_value_pNO[2];
    static uint8_t vPreset[2];
    char make_packet[25];

    tmp_value_pNO[1] = Processing_number;  // process no

    // Virtual preset
    vPreset[0] = vPreset[1];
    vPreset[1] = virtualPrsetState();
    
    if(vPreset[0] != vPreset[1])
        disp = ON;

    if ((tmp_value_pNO[0] != tmp_value_pNO[1]) || disp)   // 값이 다를 때 반영한다.
    {
        if (Processing_number == PRESET_INDEX_SIZE + 1)
            drawStringXY(5, 20, FONT_12PT, WHITE, BLACK, 5, (char*)"MA   ");
        else if (Processing_number == PRESET_INDEX_SIZE + 2)
            drawStringXY(5, 20, FONT_12PT, WHITE, BLACK, 5, (char*)"MB   ");
        else if (virtualPrsetState())
            drawStringXY(5, 20, FONT_12PT, WHITE, BLACK, 5, (char*)"VP   ");
        else
        {
            sprintf(make_packet, "P%2d  ", Processing_number);
            drawStringXY(5, 20, FONT_12PT, WHITE, BLACK, 5, (char*)make_packet);                
        }
        tmp_value_pNO[0] = tmp_value_pNO[1];
    }
}

void DisPlay_CurrentTorque(uint8_t disp, float value)
{
    static float tmp_value_rTQ[2];
    float temp_value_f;
    char make_packet[25];
    uint8_t xPosition, len = 0;

    tmp_value_rTQ[1] = value;  // real torque
    // tmp_value_rTQ[1] = (uint16_t)(Tq_SensorValue_Avg_max);  // real torque   // FIXME: 임시 - 토크 기울기 측정용

    // 이전과 같이 다를 경우 반영
    if ((tmp_value_rTQ[0] != tmp_value_rTQ[1]) || disp)   // if change value, 
    {
        drawFillRectXY(10, 40, 92, 61, 1, BLACK, BLACK);   // clear block..44
        temp_value_f = value;
        // temp_value_f = Tq_SensorValue_Avg_max;  // FIXME: 임시 - 토크 기울기 측정용
        

        // Make string torque data
        len = sprintf(make_packet, "%.3f", temp_value_f);

        // 토크 자리수 별로 표시 위치 변경
        if(make_packet[len - 1] == '0')
            len--;

        xPosition = 42 - 7*(len - 4);
        if(xPosition > 100 || xPosition < 10)
            xPosition = 42;
        drawStringXY(xPosition, 40, FONT_16PT, YELLOW, BLACK, len, (char*)make_packet);

        tmp_value_rTQ[0] = tmp_value_rTQ[1];
    }    
}

void Display_SetTorque(uint8_t disp)
{
    static float tmp_value_setTq[2];
    float temp_value_f;
    char make_packet[25];
    uint8_t xPosition, len = 0;

    // Advanced target torque 값을 화면에 표시
    tmp_value_setTq[1] = advancedTargetTorque(Fastening_number);

    if ((tmp_value_setTq[0] != tmp_value_setTq[1]) || disp)
    {
        drawFillRectXY(60, 20, 126, 33, 1, BLACK, BLACK);   // clear block
        temp_value_f = tmp_value_setTq[1];

        // Make string torque data
        len = sprintf(make_packet, "%.3f", temp_value_f);

        // 토크 자리수 별로 표시 위치 변경
        if(make_packet[len - 1] == '0')
            len--;

        xPosition = 98 - 8*(len - 4);
        drawStringXY(xPosition, 20, FONT_12PT, WHITE, BLACK, len, (char*)make_packet);

        tmp_value_setTq[0] = tmp_value_setTq[1];
    }
}

void DisPlay_CurrentSpeed(uint8_t disp, float value)
{
    static uint16_t tmp_value_rTQ[2];
    uint16_t temp_value_f;
    char make_packet[25];

    tmp_value_rTQ[1] = (uint16_t)(value);  // real torque

    if ((tmp_value_rTQ[0] != tmp_value_rTQ[1]) || disp)   // if change value, 
    {
        drawFillRectXY(30, 40, 90, 61, 1, BLACK, BLACK);   // clear block..44
        temp_value_f = (uint16_t)value;
        if (tmp_value_rTQ[1] > 999)
        {
            sprintf(make_packet, "%d", temp_value_f);
            drawStringXY(30, 40, FONT_16PT, YELLOW, BLACK, 4, (char*)make_packet);
        }
        else if (tmp_value_rTQ[1] > 99)
        {
            sprintf(make_packet, "%d", temp_value_f);
            drawStringXY(40, 40, FONT_16PT, YELLOW, BLACK, 3, (char*)make_packet);
        }
        else
        {
            sprintf(make_packet, "%d", temp_value_f);
            drawStringXY(50, 40, FONT_16PT, YELLOW, BLACK, 2, (char*)make_packet);
        }
        tmp_value_rTQ[0] = tmp_value_rTQ[1];
    }    
}

void Display_SetSpeed(uint8_t disp)
{
    static uint16_t tmp_value_setTq[2];
    uint16_t temp_value_f;
    char make_packet[25];

    tmp_value_setTq[1] = Para[FASTEN_SPEED1 + ((Processing_number - 1)*PRESET_SIZE)].val;  // setting torque

    if ((tmp_value_setTq[0] != tmp_value_setTq[1]) || disp)
    {
        drawFillRectXY(80, 20, 126, 33, 1, BLACK, BLACK);   // clear block
        temp_value_f = (uint16_t)(tmp_value_setTq[1]);
        if (tmp_value_setTq[1] > 999)
        {
            sprintf(make_packet, "%d", temp_value_f);
            drawStringXY(80, 20, FONT_12PT, WHITE, BLACK, 4, (char*)make_packet);
        }
        else if (tmp_value_setTq[1] > 99)
        {
            sprintf(make_packet, "%d", temp_value_f);
            drawStringXY(89, 20, FONT_12PT, WHITE, BLACK, 3, (char*)make_packet);
        }
        else
        {
            sprintf(make_packet, "%d", temp_value_f);
            drawStringXY(96, 20, FONT_12PT, WHITE, BLACK, 2, (char*)make_packet);
        }
        tmp_value_setTq[0] = tmp_value_setTq[1];
    }
}


// Error code to Error string number (에러코드로 에러문자 목록 선택)
int  ErrorNo_Conversion(int err)
{
    int retvalue;
    
    if (err)
    {
        switch (err)
        {
            case 101: retvalue = 1; break;
            case 102: retvalue = 2; break;
            case 103: retvalue = 3; break;
            case 104: retvalue = 4; break;
            case 105: retvalue = 5; break;
            case 106: retvalue = 6; break;
            case 107: retvalue = 7; break;
            case 108: retvalue = 8; break;
            case 109: retvalue = 9; break;
            case 110: retvalue = 10; break;
            case 111: retvalue = 11; break;
            case 112: retvalue = 12; break;
            case 113: retvalue = 13; break;
            case 114: retvalue = 14; break;
            case 115: retvalue = 15; break;
            case 116: retvalue = 16; break;
            case 118: retvalue = 17; break;
            case 119: retvalue = 18; break;
            case 120: retvalue = 19; break;
            case 121: retvalue = 20; break;
            case 122: retvalue = 21; break;
            case 123: retvalue = 22; break;
            case 125: retvalue = 23; break;
            case 127: retvalue = 24; break;
            case 200: retvalue = 25; break;
            case 201: retvalue = 26; break;
            case 220: retvalue = 27; break;
            case 300: retvalue = 28; break;
            case 301: retvalue = 29; break;
            case 303: retvalue = 30; break;
            case 304: retvalue = 31; break;
            case 305: retvalue = 32; break;
            case 309: retvalue = 33; break;
            case 310: retvalue = 34; break;
            case 311: retvalue = 35; break;
            case 329: retvalue = 36; break;
            case 330: retvalue = 37; break;
            case 331: retvalue = 38; break;
            case 332: retvalue = 39; break;
            case 333: retvalue = 40; break;
            case 334: retvalue = 41; break;
            case 335: retvalue = 42; break;
            case 336: retvalue = 43; break;
            case 337: retvalue = 44; break;
            case 338: retvalue = 45; break;
            case 339: retvalue = 46; break;
            case 350: retvalue = 47; break;
            case 351: retvalue = 48; break;
            case 352: retvalue = 49; break;
            case 353: retvalue = 50; break;
            case 354: retvalue = 51; break;
            case 355: retvalue = 52; break;
            case 356: retvalue = 53; break;
			case 358: retvalue = 54; break;
            case 400: retvalue = 55; break;
            case 401: retvalue = 56; break;
            case 500: retvalue = 57; break;
            case 501: retvalue = 58; break;
            case 503: retvalue = 59; break;
			case 505: retvalue = 60; break;
			
            default:  retvalue = 0; break;
        }
    }
    else
        retvalue = 0;
    
    return retvalue;
}

void displayBattery(uint8_t level, uint8_t mode)
{
    static uint16_t tmp_value_bat[2];
    // uint16_t pCnt;

    tmp_value_bat[1] = level;
    // pCnt = 0;

    if ((tmp_value_bat[0] != tmp_value_bat[1]) || mode)  // if change, display level
    {
        MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD0;       // start packet
        MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD1;

        MakePacket_Oled[Cnt_Oled_Packet++] = 2;        // lenght .. function + color
        MakePacket_Oled[Cnt_Oled_Packet++] = DisplayBattery;
        MakePacket_Oled[Cnt_Oled_Packet++] = level;

        // SerialPutChar_Oled(pCnt, (uint8_t*)MakePacket_Oled);
        tmp_value_bat[0] = tmp_value_bat[1];
    }
}

void displayAntena(uint8_t level, uint8_t mode)
{
    static uint16_t tmp_value_ant[2];
    // uint16_t pCnt;

    tmp_value_ant[1] = level;
    // pCnt = 0;

    if ((tmp_value_ant[0] != tmp_value_ant[1]) || mode)  // if change, display level
    {
        MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD0;       // start packet
        MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD1;

        MakePacket_Oled[Cnt_Oled_Packet++] = 2;        // lenght .. function + color
        MakePacket_Oled[Cnt_Oled_Packet++] = DisplayAntena;
        MakePacket_Oled[Cnt_Oled_Packet++] = level;

        // SerialPutChar_Oled(pCnt, (uint8_t*)MakePacket_Oled);
        tmp_value_ant[0] = tmp_value_ant[1];
    }
}

void fillOLED(uint8_t color)
{
    // uint16_t pCnt;

    // pCnt = 0;

    MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD0;       // start packet
    MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD1;

    MakePacket_Oled[Cnt_Oled_Packet++] = 2;        // lenght .. function + color
    MakePacket_Oled[Cnt_Oled_Packet++] = Fill_OLED;
    MakePacket_Oled[Cnt_Oled_Packet++] = color;


    // SerialPutChar_Oled(pCnt, (uint8_t*)MakePacket_Oled);
}

void drawStringXY(uint8_t x, uint8_t y, uint8_t size, uint8_t fontCol, uint8_t bgCol, uint8_t length, char* str)
{
    // uint16_t pCnt;
    uint16_t i;

    // pCnt = 0;

    MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD0;       // start packet
    MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD1;

    MakePacket_Oled[Cnt_Oled_Packet++] = 7 + length;        // lenght .. function + ..
    MakePacket_Oled[Cnt_Oled_Packet++] = DrawStringXY;
    MakePacket_Oled[Cnt_Oled_Packet++] = x;
    MakePacket_Oled[Cnt_Oled_Packet++] = y;        // 5
    MakePacket_Oled[Cnt_Oled_Packet++] = size;
    MakePacket_Oled[Cnt_Oled_Packet++] = fontCol;
    MakePacket_Oled[Cnt_Oled_Packet++] = bgCol;
    MakePacket_Oled[Cnt_Oled_Packet++] = length;   // 9

    for(i=0; i<=length-1; i++)
    {
        MakePacket_Oled[Cnt_Oled_Packet++] = str[i];
    }   

    // SerialPutChar_Oled(pCnt, (uint8_t*)MakePacket_Oled);
}

void drawFillRectXY(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey, uint8_t thick, uint8_t lineCol, uint8_t fillCol)
{
    // uint16_t pCnt;

    // pCnt = 0;

    MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD0;       // start packet
    MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD1;

    MakePacket_Oled[Cnt_Oled_Packet++] = 8;        // lenght .. function + ..
    MakePacket_Oled[Cnt_Oled_Packet++] = DrawFillRectXY;
    MakePacket_Oled[Cnt_Oled_Packet++] = sx;
    MakePacket_Oled[Cnt_Oled_Packet++] = sy;
    MakePacket_Oled[Cnt_Oled_Packet++] = ex;
    MakePacket_Oled[Cnt_Oled_Packet++] = ey;
    MakePacket_Oled[Cnt_Oled_Packet++] = thick;
    MakePacket_Oled[Cnt_Oled_Packet++] = lineCol;
    MakePacket_Oled[Cnt_Oled_Packet++] = fillCol;

    // SerialPutChar_Oled(pCnt, (uint8_t*)MakePacket_Oled);
}

void drawRectXY(uint8_t sx, uint8_t sy, uint8_t ex, uint8_t ey, uint8_t thick, uint8_t color)
{
    // uint16_t pCnt;

    // pCnt = 0;

    MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD0;       // start packet
    MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD1;

    MakePacket_Oled[Cnt_Oled_Packet++] = 7;        // lenght .. function + ..
    MakePacket_Oled[Cnt_Oled_Packet++] = DrawRectXY;
    MakePacket_Oled[Cnt_Oled_Packet++] = sx;
    MakePacket_Oled[Cnt_Oled_Packet++] = sy;
    MakePacket_Oled[Cnt_Oled_Packet++] = ex;
    MakePacket_Oled[Cnt_Oled_Packet++] = ey;
    MakePacket_Oled[Cnt_Oled_Packet++] = thick;
    MakePacket_Oled[Cnt_Oled_Packet++] = color;

    // SerialPutChar_Oled(pCnt, (uint8_t*)MakePacket_Oled);
}

void Control_LED(void)
{
    static uint16_t tmp_value_color_g[2];
    static uint16_t tmp_value_color_r[2];

    tmp_value_color_g[1] = Flag_GreenLED;

    if (tmp_value_color_g[0] != tmp_value_color_g[1])   // 값이 다를 때 반영한다.
    {
        MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD0;   // start packet
        MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD1;

        MakePacket_Oled[Cnt_Oled_Packet++] = 3;            // lenght .. function + ..
        MakePacket_Oled[Cnt_Oled_Packet++] = ControlLED;
        MakePacket_Oled[Cnt_Oled_Packet++] = 1;            // 1: green, 2: red
        MakePacket_Oled[Cnt_Oled_Packet++] = Flag_GreenLED;

        // SerialPutChar_Oled(pCnt, (uint8_t*)MakePacket_Oled);
        tmp_value_color_g[0] = tmp_value_color_g[1];
    }

    tmp_value_color_r[1] = Flag_RedLED;

    if (tmp_value_color_r[0] != tmp_value_color_r[1])  // 값이 다를 때 반영한다.
    {
        MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD0;   // start packet
        MakePacket_Oled[Cnt_Oled_Packet++] = OLED_HEAD1;

        MakePacket_Oled[Cnt_Oled_Packet++] = 3;            // lenght .. function + ..
        MakePacket_Oled[Cnt_Oled_Packet++] = ControlLED;
        MakePacket_Oled[Cnt_Oled_Packet++] = 2;            // 1: green, 2: red
        MakePacket_Oled[Cnt_Oled_Packet++] = Flag_RedLED;

        // SerialPutChar_Oled(pCnt, (uint8_t*)MakePacket_Oled);
        tmp_value_color_r[0] = tmp_value_color_r[1];        
    }
}

void Send_Oled_Data(void)   // send data all.
{
    SerialPutChar_Oled(Cnt_Oled_Packet, (uint8_t*)MakePacket_Oled);
}



// 1 : fillOLED(color)
//        1:BLACK
//        2:WHITE
//        3:RED
//        4:GREEN
//        5:BLUE
//        6:YELLOW

// 2 : drawStringXY(x, y, font_size, font_color, bg_color, length, *font)  7
        // 0:FONT_8PT, 1:FONT_10PT, 2:FONT_12PT, 3:FONT_16PT

// 3 : drawFillRectXY((sx, sy, ex, ey, thick, lineCol, fillCol) 7

// 4 : drawRectXY(sx, sy, ex, ey, thick, color) 6

// 5 : Control LED ( red/green, on/off)
