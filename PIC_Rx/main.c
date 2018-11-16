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

//TODO:add interrupt finction?
void main(void) {

    /*---Initialization---*/
    /*----------------------------------------------------------------------*/ 
    InitSerial();
    InitMPU();
//    InitWDT();
    InitI2CMaster(I2C_baud_rate_def);
    initTimer();
   
//    LED_WHITE = 1;              //for debugging of init
    __delay_ms(500);           //wait until 5V is generated by EPS

    /*---PLL setting (common Phase-Locked Loop)---*/
    /*----------------------------------------------------------------------*/
    setPLL();

    
//    LED_WHITE = 0;              //for debugging of PLL setting
    __delay_ms(500);           //wait for circuit of PLL
    
//   putChar('S');

    /*----------------------------------------------------------------------*/
    //for debug BatVoltage measure
    // WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS,SatelliteMode_addressHigh,SatelliteMode_addressLow,SATMODE_SURVIVAL_SEPOFF_RBFON);
    // WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, BatVol_nominal_saving_datahigh_addresshigh, BatVol_nominal_saving_datahigh_addressLow,0x02);
    // WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, BatVol_nominal_saving_datalow_addresshigh, BatVol_nominal_saving_datalow_addressLow,0x1D);
    //
    // for debug initial ope
    // testInitialOpe(); 
    /*----------------------------------------------------------------------*/
       
    
    UBYTE error_status = 0x00;
    
    while(1){
//        UBYTE DataLengthHigh = 0x01;
//        UBYTE DataLengthLow = 0x00;
//        UBYTE addressHigh = 0x00;
//        UBYTE addressLow = 0x80;
//        UBYTE downlinkTimes = 0x01;
//            UBYTE *readData;
//            UINT DataLength = (UINT)((DataLengthHigh << 8) + DataLengthLow);
//            UINT address = (UINT)((addressHigh << 8) + addressLow);
//            UBYTE flag = 0;
//
//            for(UBYTE sendCounter = 0; sendCounter < downlinkTimes; sendCounter++){
//                while(!flag){
//                    __delay_ms(300);
////                    SendPacket(readData,32);
//                    putChar(addressHigh);
//                    putChar(addressLow);
//                    putChar(DataLengthHigh);
//                    putChar(DataLengthLow);
//                    
//                    __delay_ms(300);
//                    address += 0x0020;
//                    addressHigh = (UBYTE)(address >> 8);
//                    addressLow = (UBYTE)address;
//                    if(DataLength < 32){
//                        flag = 1;
//                    }else{
//                    DataLength -= 32;
//                    DataLengthHigh = (UBYTE)(DataLength >> 8);
//                    DataLengthLow = (UBYTE)DataLength;
//                    }
//                    __delay_ms(500);
//                }
//            }
//        
//            while(1){
//                putChar(0xFF);
//            }
//              
        /*---timer interrupt---*/
        /*----------------------------------------------------------------------------*/
        /*----------------------------------------------------------------------------*/
//        /*---timer process for EPS reset (1week)---*/       
//        if(get_timer_counter('w') >= 1){  //for FM
//        if(get_eps_reset_counter_sec() >= EPS_RSET_INTERVAL_SHORT){   //for debug
////            putChar('E');
////            putChar('E');
////            putChar('E');
////            Reset_EPS();
//            sendPulseWDT();
//            Reset_EPS();
//            sendPulseWDT();
//            delay_ms(5000);
//            sendPulseWDT();
//            setPLL();
////            // Execute 1week reset
//            reset_timer();
//            set_eps_reset_counter(0,0);  //for debug
//        }
//
//        /*---timer process for NTRX PLL setting(every day) & EPS reset (if initial Ope / everyday)---*/
////        if(get_NTRX_pll_setting_counter_day() >= NTRX_PLL_INTERVAL){   //FM
//        if(get_NTRX_pll_setting_counter_sec() >= 5){   //for debug
////            putChar('N');
////            putChar('N');
////            putChar('N');            
//            UBYTE melting_status[2] = {0x00};
//            melting_status[0] = checkMeltingStatus(MAIN_EEPROM_ADDRESS);
//            melting_status[1] = checkMeltingStatus(SUB_EEPROM_ADDRESS);
//            if((melting_status[0] < MELTING_FINISH)&&(melting_status[1] < MELTING_FINISH)) {
//                sendPulseWDT();
//                Reset_EPS();  //only initial ope -> reset EPS
//                sendPulseWDT();
//                delay_ms(5000);
//                sendPulseWDT();
//            }
//            setPLL();  // set PLL every day
//            set_NTRX_pll_setting_counter(0,0,0,0);
//        }        
//        
//        /*---timer process for initial operation (22.5min)---*/
//        // //       if(get_init_ope_counter_min() >= INITIAL_OPE_INTERVAL){  //for FM
//        // if(get_init_ope_counter_sec() >= INITIAL_OPE_INTERVAL){   //for debug[sec]
//        //     //error_status = InitialOperation();
//        //     WriteOneByteToMainAndSubB0EEPROM(InitialOpe_error_status_addressHigh,InitialOpe_error_status_addressLow,error_status);
//        //     //errorCheckInitialOpe();  //*******for debug (initial ope) ************
//        //     set_init_ope_counter(0,0);
//        // }
//
//        /*---timer process for measure EPS BATTERY---*/
//        //       if(get_bat_meas_counter_min() >= EPS_MEASURE_INTERVAL){  //for FM
//        if(get_bat_meas_counter_sec() >= EPS_MEASURE_INTERVAL){   //for debug[sec]
////            putChar('B');
////            putChar('B');
////            putChar('B');           
//            
//            //TODO:debug function to measure EPS Battery
//           UBYTE SatMode_error_status = MeasureBatVoltageAndChangeSatMode();
//                  
//           if (SatMode_error_status != 0){
//               SatMode_error_status = MeasureBatVoltageAndChangeSatMode();
//           }
//           WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS, SatMode_error_status_addresshigh, SatMode_error_status_addresslow, SatMode_error_status);
//           set_bat_meas_counter(0,0);
//        }
        /*----------------------------------------------------------------------------*/
        /*----------------------------------------------------------------------------*/
        
//        putChar(0xff);
        sendPulseWDT();
        delay_ms(2000);
        
        /*---Receive command data---*/ 
        /*------------------------------------------------------------------*/
        UBYTE commandData[DATA_SIZE];         //data of uplink command
        UBYTE commandID;            //ID of uplink command

        //for information on EEPROM see data sheet: 24LC1025        
        UBYTE B0select;             //control byte B0 of EEPROM
        UBYTE wHighAddress;         //address high byte of EEPROM
        UBYTE wLowAddress;          //address low byte of EEPROM
        UBYTE mainControlByte;      //control byte of main EEPROM
        UBYTE subControlByte;       //control byte of sub EEPROM       
        UBYTE downlinkTimes;       //downlink times of received command 
        
        /*---COMMAND RESET----*/
        for(UBYTE i=0; i<DATA_SIZE; i++){
            commandData[i] = 0;
        }
        
        UBYTE writedata[32];
        for(int i = 0 ; i < 32 ; i++){
            writedata[i] = i + 0x30;
        } 
//        WriteToEEPROM(0x50,0x01,0x80,writedata);
//        WriteToEEPROM(0x50,0x01,0xa0,writedata);
//        WriteToEEPROM(0x50,0x01,0xc0,writedata);
//        WriteToEEPROM(0x50,0x01,0xe0,writedata);
//        WriteToEEPROM(0x50,0x02,0x00,writedata);
//        WriteToEEPROM(0x50,0x02,0x20,writedata);
//        WriteToEEPROM(0x50,0x02,0x40,writedata);
//        WriteToEEPROM(0x50,0x02,0x60,writedata);
//        WriteToEEPROM(0x50,0x02,0x80,writedata);
        
        receiveDataPacket(commandData);
        delay_ms(5000);
        
        
        //XXX if () continue, IF COMMAND IS STILL RESET
//        if(commandData[0]==0) {
//            continue;      //not receive command-->continue
//        }
        
//        /*---check command ID---*/
//        lastCommandID = ReadEEPROMmainAndSub(B0select_for_RXCOBCLastCommand,HighAddress_for_LastCommandID,LowAddress_for_LastCommandID);
//        commandID = commandData[1];     
//        if (commandID == lastCommandID) {
//            putChar(commandID);
//            continue;       //same uplink command-->continue
//        }
//        lastCommandID = commandID;                      //update command ID
        
        
        /*-------------Substitute EEPROM ADDRESS DATA-------------------*/
//        B0select = commandData[19];
//        wHighAddress = commandData[20];
//        wLowAddress = commandData[21];
//        downlinkTimes = commandData[22];
        B0select = 0x00;
        wHighAddress = 0x00;
        wLowAddress = 0x80;
        downlinkTimes = 0x05;
        mainControlByte = MAIN_EEPROM_ADDRESS | B0select;
        subControlByte = SUB_EEPROM_ADDRESS | B0select;
        
        /*---CRC check for command from Grand Station---*/ 
        /*------------------------------------------------------------------*/
        UWORD crcResult, crcValue;
        crcResult = crc16(0,commandData,29);
        crcValue =  checkCRC(commandData,29);
        
        /*------------------------------------*/
        //for debug (check CRC)
//        UBYTE crcResultHigh,crcResultLow,crcValueHigh,crcValueLow;
//        crcResultHigh = crcResult>>8;
//        crcResultLow = crcResult & 0x00FF;
//        crcValueHigh = crcValue>>8;
//        crcValueLow = crcValue & 0x00FF;
        /*------------------------------------*/
        
        /*---update CRC---*/
        if(crcResult != crcValue){
            commandData[31] = commandData[31] & 0b01111111; 
//            switchError(error_main_crcCheck);
        }else{
            commandData[31] = commandData[31] | 0b10000000;
//            switchOk(ok_main_crcCheck);           
        }  
        
        /*-----------for debug------------*/
        commandData[0] = 'T';
        commandData[1] = 1;
        commandData[2] = 't';
        commandData[3] = 'f';
        commandData[4] = 0xAA;
        commandData[5] = 0x50;
        commandData[6] = 0x01;
        commandData[7] = 0x80;
        commandData[8] = 0x01;
        commandData[9] = 0x01;
        commandData[10] = 0x10;
        
        
        /*---Write uplink command in EEPROM---*/
        /*------------------------------------------------------------------*/
        WriteToEEPROM(mainControlByte,wHighAddress,wLowAddress,commandData);
        WriteToEEPROM(subControlByte,wHighAddress,wLowAddress,commandData);
        WriteLastCommandIdToEEPROM(commandData[1]);

        /*---Send address using UART to OBC and TXCOBC---*/
        /*------------------------------------------------------------------*/
//        UBYTE send_command[8];
//        send_command[0] = 'g';
//        send_command[1] = 'u';
//        send_command[2] = B0select;
//        send_command[3] = wHighAddress;
//        send_command[4] = wLowAddress;
//        send_command[5] = downlinkTimes;
//        send_command[6] = 0x00;
//        send_command[7] = 0x00;
//        sendCommandByPointer(send_command);
        sendCommand('g','u',B0select, wHighAddress, wLowAddress, downlinkTimes, 0x00, 0x00);
//        putChar('G');
        
        for(int i=0; i<DATA_SIZE; i++){
//            putChar(commandData[i]);
        }
        
//        putChar('H');
        /*---Define if command target is RXCOBC 'R' and read in task target ---*/
        /*------------------------------------------------------------------*/
        if(commandData[0]=='R'){                //command target = PIC_RX
            //Task target
            if(commandData[2] == 'r'){          //task target =  PIC_RX
                // Command type
                switch(commandData[3]){         //Process command type
                case 'm': /*change sattelite mode*/
                    commandSwitchSatMode(commandData[4], commandData[5], commandData[6]);
                    break;
                case 'p': /*power supply*/
                    commandSwitchPowerSupply(commandData[4], commandData[5], commandData[6], commandData[7]);
                    break;
                case 'n': /*radio unit*/
                    commandSwitchFMCW(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'i':/*I2C*/
                    commandSwitchI2C(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8]);
                    break;
                case 'e': /*EEPROM*/
                    commandSwitchEEPROM(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'u':/*UART*/
                    commandSwitchUART(commandData[4], commandData[5], commandData[6], commandData[7], commandData[8], commandData[9]);
                    break;
                case 'w':/*WDT (watch dog timer)*/
                    commandWDT(commandData[4]);
                    break;
                case 'h':/*update HK data (BAT_POS V) (HK = house keeping)*/
                    //TODO: write function directly here or in MPU.c                   
                    break;
                case 'r':/*internal processing*/
                    commandSwitchIntProcess(commandData[4], commandData[5], commandData[6]);                   
                    break;
                default:
//                    switchError(error_main_reveiveCommand);
                    break;
                }

            // }else if(commandData[2] == 't'){      //task target =  PIC_TX       

            // }else if(commandData[2] == 'o'){      //task target =  OBC       

            // }else if(commandData[2] == '5'){      //task target =  5R8G       

            }
        }else{
            //debugging if coomand target is not RXCOBC
            LED_WHITE = 1;  
            __delay_ms(1000);
            LED_WHITE = 0;
        }
    }
    //return;
}
