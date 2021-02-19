#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>
#include <PIC16LF877A.h>
#include "ADC.h"
#include "typeDefine.h"
#include "pinDefine.h"
#include "MPU.h"
#include "time.h"
#include "UART.h"
#include "decodeAX25.h"
#include "I2C.h"
#include "initial_operation.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "EPS.h"
#include "WDT.h"
#include "CRC16.h"
#include "OkError.h"
#include "timer.h"
#include "SatMode.h"



// PIC16F887 Configuration Bit Settings
#pragma config FOSC     = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE     = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE    = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN    = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP      = OFF       // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD      = OFF       // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT      = OFF       // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP       = OFF       // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define MELTING_FINISH_FLAG 0b01111110
UBYTE lastCommandID=0xff;        //ID of uplink command
UBYTE commandData[DATA_SIZE];
UBYTE commandID;            //ID of uplink command
UBYTE mainControlByte;      //control byte of main EEPROM
UBYTE subControlByte;       //control byte of sub EEPROM
//UWORD SatMode_error_status;
UBYTE SatMode_error_status;

//TODO:add interrupt finction?
void main(void) {

    /*---Initialization---*/
    /*----------------------------------------------------------------------*/
    InitSerial();
    InitMPU();
    InitWDT();
    InitI2CMaster(I2C_baud_rate_def);
    initTimer();
    InitSatMode();

    __delay_ms(500);           //wait until 5V is generated by EPS

    /*---PLL setting (common Phase-Locked Loop)---*/
    /*----------------------------------------------------------------------*/
    setPLL();
    __delay_ms(500);           //wait for circuit of PLL
    
#define flight
//#define debug
#ifdef debug
    while(1){
        sendPulseWDT();
        putChar(0xAA);
        putChar(0xAA);
        putChar(0xAA);
        putChar(0xAA);
        putChar(0xAA);
        
         set_receive_command_counter(0,0);
        while(RCIF != 1){
            if(get_receive_command_counter_sec() >= 5)   break;
        }
        UBYTE test;
        test = getChar();
        putChar(test);
//        for(int i=0; i<32;i ++){
//            putChar(0xCC);
//            putChar(0xCC);
//            putChar(0xCC);
//            set_receive_command_counter(0,0);
//            while(RCIF != 1){
//                if(get_receive_command_counter_sec() >= 1)   break;
//            }
//            commandData[i] = getChar();
//            putChar(commandData[i]);
//        }
    }
#endif
#ifdef flight
    sendPulseWDT();

    /*----------------------------------------------------------------------*/
    // for debug initial ope
//    testInitialOpe();
    /*----------------------------------------------------------------------*/

    UBYTE error_status;

    UBYTE melting_status[2];
    melting_status[0] = checkMeltingStatus(MAIN_EEPROM_ADDRESS);
    melting_status[1] = checkMeltingStatus(SUB_EEPROM_ADDRESS);
//    if((melting_status[0] >= MELTING_FINISH)||(melting_status[1] >= MELTING_FINISH)) {  //after melting
//        if(MRLTING_FLAG_FOR_OBC == LOW){
//            MRLTING_FLAG_FOR_OBC = HIGH;
//        }
//    } else {                                                                            //before melting
//        /*---200s ( 50s * 4times)---*/
//        for(UBYTE i=0; i<4; i++){
//            /*---wait 50s---*/
//            sendPulseWDT();
//            for(UBYTE j=0; j<10; j++){
//                delay_s(5);
//                sendPulseWDT();
//            }
//            /*---measure voltage & change Sat Mode---*/
//            SatMode_error_status = MeasureBatVoltageAndChangeSatMode();
//            if (SatMode_error_status != 0){
//                SatMode_error_status = MeasureBatVoltageAndChangeSatMode();
//            }
//            WriteOneByteToMainAndSubB0EEPROM(SatMode_error_status1_addresshigh, SatMode_error_status1_addresslow, SatMode_error_status);
//        }
//    }
    putChar(0x88);
    putChar(0x88);
    putChar(0x88);
    putChar(0x88);
    putChar(0x88);
    putChar(0x88);
    while(1){
        putChar(0xAA);
        putChar(0xAA);
        putChar(0xAA);
        putChar(0xAA);
        putChar(0xAA);

        /*---timer interrupt---*/
        /*----------------------------------------------------------------------------*/
        /*----------------------------------------------------------------------------*/
        /*---timer process for EPS reset (1week)---*/
        if(get_timer_counter('w') >= 1){  
            resetEPS();
            setPLL();
//            // Execute 1week reset
            reset_timer();
            set_eps_reset_counter(0,0);  
        }
//
        /*---timer process for NTRX PLL setting(every day) & EPS reset (if initial Ope / everyday)---*/
        if(get_NTRX_pll_setting_counter_day() >= NTRX_PLL_INTERVAL){   
            melting_status[0] = checkMeltingStatus(MAIN_EEPROM_ADDRESS);
            melting_status[1] = checkMeltingStatus(SUB_EEPROM_ADDRESS);
            if((melting_status[0] < MELTING_FINISH)&&(melting_status[1] < MELTING_FINISH)) {
                resetEPS();  //only initial ope -> reset EPS
            }
            setPLL();  // set PLL every day
            set_NTRX_pll_setting_counter(0,0,0,0);
        }

        //*---timer process for initial operation (22.5min)---*/
        if(get_init_ope_counter_min() >= INITIAL_OPE_INTERVAL){  
            error_status = InitialOperation();
            WriteOneByteToMainAndSubB0EEPROM(InitialOpe_error_status_addressHigh,InitialOpe_error_status_addressLow,error_status);
            set_init_ope_counter(0,0);
         }

        /*---timer process for measure EPS BATTERY---*/
        if(get_bat_meas_counter_min() >= EPS_MEASURE_INTERVAL){  
            SatMode_error_status = MeasureBatVoltageAndChangeSatMode();
            if (SatMode_error_status != 0){
               SatMode_error_status = MeasureBatVoltageAndChangeSatMode();
           }
           WriteOneByteToMainAndSubB0EEPROM(SatMode_error_status1_addresshigh, SatMode_error_status1_addresslow, SatMode_error_status);
           set_bat_meas_counter(0,0);
        }
        /*----------------------------------------------------------------------------*/
        /*----------------------------------------------------------------------------*/
        sendPulseWDT();

//
//        /*---Receive command data---*/
//        /*------------------------------------------------------------------*/

        //for information on EEPROM see data sheet: 24LC1025

        /*---COMMAND RESET----*/
        commandData[0] = 0;
        for(int i=0; i<32;i ++){
            commandData[i] = 0;
        }
//        receiveDataPacket(commandData);
        set_receive_command_counter(0,0);
        while(RCIF != 1){
//            if(get_receive_command_counter_sec() >= 1) sendPulseWDT();
            __delay_ms(1);
            sendPulseWDT();
            if(get_receive_command_counter_sec() >= 5)   break;
        }
        for(int i=0; i<32;i ++){
            commandData[i] = getChar();
            putChar(commandData[i]);
            sendPulseWDT();
        }
        putChar(0xBB);
        putChar(0xBB);
        putChar(0xBB);
        putChar(0xBB);
        putChar(0xBB);

        //XXX if () continue, IF COMMAND IS STILL RESET
        if(commandData[0] == 0) continue;      //not receive command-->continue
        
        mainControlByte = (UBYTE)(MAIN_EEPROM_ADDRESS | commandData[19]);
        subControlByte = (UBYTE)(SUB_EEPROM_ADDRESS | commandData[19]);

        /*---CRC check for command from Grand Station---*/
        /*------------------------------------------------------------------*/

        /*---update CRC---*/
        if(crc16(0,commandData,29) == checkCRC(commandData,29)){
            commandData[31] = 0xb0;
            /*---check command ID---*/
            lastCommandID = ReadEEPROMmainAndSub(B0select_EEPROM, HighAddress_for_LastCommandID, LowAddress_for_LastCommandID);
            commandID = commandData[1];
            if (commandID == lastCommandID) {
                putChar(0xAF);
                putChar(0xAF);
                putChar(0xAF);
                putChar(0xAF);
                putChar(0xAF);
                putChar(0xAF);
                continue;       //same uplink command-->continue
            }
            WriteOneByteToMainAndSubB0EEPROM(HighAddress_for_LastCommandID, LowAddress_for_LastCommandID, commandID);
//            putChar(commandID);
        }
        UBYTE read_ID;
        read_ID = ReadEEPROMmainAndSub(B0select_EEPROM, HighAddress_for_LastCommandID, LowAddress_for_LastCommandID);
        putChar(lastCommandID);
        putChar(commandID);
        putChar(read_ID);
        putChar(0xCC);
        putChar(0xCC);
        putChar(0xCC);
        for(int i=0; i<32;i ++){
            putChar(commandData[i]);
        }

        /*---Write uplink command in EEPROM---*/
        /*------------------------------------------------------------------*/
        WriteToEEPROMWithDataSize(mainControlByte,commandData[20],commandData[21],commandData,DATA_SIZE);
        WriteToEEPROMWithDataSize(subControlByte,commandData[20],commandData[21],commandData,DATA_SIZE);

        /*---Send address using UART to OBC and TXCOBC---*/
        sendCommand('g','u',commandData[19], commandData[20], commandData[21], commandData[22], 0x00, 0x00);
        
        /*---Define if command target is RXCOBC 'R' and read in task target ---*/
        /*------------------------------------------------------------------*/
        if(commandData[0] != 'R') continue;              //command target = PIC_RX        
        
            //Task target
        if(commandData[2] != 'r') continue;          //task target =  PIC_RX
                // Command type
        switch(commandData[3]){         //Process command type
            case 'm': /*change sattelite mode*/
                commandSwitchSatMode(commandData[4], commandData[5], commandData[6]);
    //                    commandSwitchSatMode(&commandData[4]);
                break;
            case 'p': /*power supply*/
                commandSwitchPowerSupply(commandData[4], commandData[5], commandData[6], commandData[7]);
                break;
            case 'n': /*radio unit*/
                commandSwitchFMCW(commandData[4]);
                break;
            case 'i':/*I2C*/
                commandSwitchI2C(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8]);
                break;
            case 'e': /*EEPROM*/
    //                    commandSwitchEEPROM(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], &commandData[9]);
                commandSwitchEEPROM(&commandData[4]);
                break;
            case 'u':/*UART*/
                commandSwitchUART(commandData[4], commandData[5], commandData[6], commandData[7]);
                break;
            case 'w':/*WDT (watch dog timer)*/
                WDTwait();
                break;
            case 'h':/*update HK data (BAT_POS V) (HK = house keeping)*/
                //TODO: write function directly here or in MPU.c
                break;
            case 'r':/*internal processing*/
                commandSwitchIntProcess(commandData[4], commandData[5], commandData[6]);
                break;
            default:
                break;
        }
        WriteOneByteToMainAndSubB0EEPROM(HighAddress_for_RXCOBCLastCommandID,LowAddress_for_RXCOBCLastCommandID,lastCommandID);
    }
    putChar(0xEE);
    putChar(0xEE);
    putChar(0xEE);
    putChar(0xEE);
    putChar(0xEE);
    putChar(0xEE);
#endif
}