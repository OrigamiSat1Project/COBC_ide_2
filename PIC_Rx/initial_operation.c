#include <xc.h>
#include "UART.h"
#include "typeDefine.h"
#include "pinDefine.h"
#include "time.h"
//#include "ADC.h"
#include "I2C.h"
#include "MPU.h"
#include "EEPROM.h"
#include "initial_operation.h"
#include "OkError.h"
#include "WDT.h"
#include "SatMode.h"  

////for debug
void testInitialOpe(void){
    UBYTE temp;
//    /*--melting status---*/
    temp = 0b00000011;
    WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS,MeltingStatus_addressHigh, MeltingStatus_addressLow, temp);
    temp = 0b00000011;
    WriteOneByteToEEPROM(SUB_EEPROM_ADDRESS,MeltingStatus_addressHigh, MeltingStatus_addressLow, temp);
//    /*---sat mode---*/
//    temp = SATMODE_NOMINAL; //SATMODE_NOMINAL / SATMODE_SAVING / SATMODE_SURVIVAL
//    WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS,SatelliteMode_addressHigh, SatelliteMode_addressLow, temp);
//    temp = SATMODE_NOMINAL; //SATMODE_NOMINAL / SATMODE_SAVING / SATMODE_SURVIVAL
//    WriteOneByteToEEPROM(SUB_EEPROM_ADDRESS,SatelliteMode_addressHigh, SatelliteMode_addressLow, temp);    
    /*---melting counter---*/
    temp = 0;  //0-MELTING_COUNTER_LIMIT
    WriteOneByteToEEPROM(MAIN_EEPROM_ADDRESS,MeltingCounter_addressHigh, MeltingCounter_addressLow, temp);
    temp = 0;  //0-MELTING_COUNTER_LIMIT
    WriteOneByteToEEPROM(SUB_EEPROM_ADDRESS,MeltingCounter_addressHigh, MeltingCounter_addressLow, temp);
//    put_ok();
}

////for debug
void errorCheckInitialOpe(void){
//    put_error();
    /*---read error status---*/
    putChar(ReadEEPROM(MAIN_EEPROM_ADDRESS, InitialOpe_error_status_addressHigh,InitialOpe_error_status_addressLow));
    putChar(ReadEEPROM(SUB_EEPROM_ADDRESS, InitialOpe_error_status_addressHigh,InitialOpe_error_status_addressLow));
}


UBYTE InitialOperation(void){
    /*---start checking whether antenna are developed or not---*/
    /*---[antenna are not developed]+[OBC does not work]->[RXCOBC develops antenna]---*/
    /*--------------------------------------------------------------------------------*/
    UBYTE sat_mode = 0;
    UBYTE melting_counter = 0;
    UBYTE melting_status[2] = {0};

    /*---check OBC status---*/
    switch(OBC_STATUS){
        case OBC_ALIVE:
//            putChar(0x71);
//            put_lf();
//            putChar('1');
            return error_initialOpe_obcAlive;
        case OBC_DIED:
            
//            putChar(0x72);
//            put_lf();
//            putChar('2');
            
            /*---read melting status & bit cal---*/
            melting_status[0] = checkMeltingStatus(MAIN_EEPROM_ADDRESS);
            melting_status[1] = checkMeltingStatus(SUB_EEPROM_ADDRESS);
            
//            putChar(0x73);
//            putChar(melting_status[0]);
//            putChar(melting_status[1]);
//            put_lf();
            
            //cal_result>TBD: melting already finish   / cal_result=<TBD: not yet
            if((melting_status[0] < MELTING_FINISH)&&(melting_status[1] < MELTING_FINISH)){
                
                /*---check satellite mode---*/
                sat_mode = ReadEEPROM(MAIN_EEPROM_ADDRESS , SatelliteMode_addressHigh, SatelliteMode_addressLow);
                sat_mode &= 0xF0;

//                putChar(0x74);
//                putChar(sat_mode);
//                put_lf();                
                              
                //sat mode: NOMINAL->melting / SAVING or SURVIVAL ->break                
                if ((sat_mode != SATMODE_NOMINAL) && (sat_mode != SATMODE_SAVING) && (sat_mode != SATMODE_SURVIVAL)){
                    /*---read data from sub EEPROM---*/
                    sat_mode = ReadEEPROM(SUB_EEPROM_ADDRESS , SatelliteMode_addressHigh, SatelliteMode_addressLow);
                    sat_mode &= 0xF0;
                    
//                    putChar(0x75);
//                    putChar(sat_mode);
//                    put_lf();   
                    
                    if ((sat_mode!=SATMODE_NOMINAL) && (sat_mode!=SATMODE_SAVING) && (sat_mode!=SATMODE_SURVIVAL)){
//                        putChar(0x76);
//                        put_lf(); 
                        
                        return error_initialOpe_readSatMode;
                    }
                }
                
                switch(sat_mode){
                    case SATMODE_SAVING:
                    case SATMODE_SURVIVAL:
                        putChar(0x77);
//                        putChar(sat_mode);
//                        put_lf(); 
                        return error_initialOpe_powerShortage; 
                        
                    case SATMODE_NOMINAL:                 
//                        putChar(0x78);
//                        putChar(sat_mode);
//                        put_lf();
                        
                        /*---check melting counter---*/
                        melting_counter = ReadEEPROM(MAIN_EEPROM_ADDRESS, MeltingCounter_addressHigh, MeltingCounter_addressLow);
                        
                        if(melting_counter>MELTING_COUNTER_LIMIT){                            
                            /*---check melting counter again---*/
                            melting_counter = ReadEEPROM(SUB_EEPROM_ADDRESS, MeltingCounter_addressHigh, MeltingCounter_addressLow);
//                            putChar(0xc1);
                            
                            if(melting_counter>MELTING_COUNTER_LIMIT){
                                /*---reset counter---*/
                                melting_counter = 0;
//                                putChar(0xc2);
                            } 
                        }
                        
//                        putChar(0xc3);
//                        putChar('\r');
//                        putChar('\n');
//                        putChar(melting_counter);
//                        putChar('\r');
//                        putChar('\n');                        

                        // melting counter
                        //1.melting_counter==MELTING_COUNTER_LIMIT         -> riset counter
                        //2.7 < melting_counter <MELTING_COUNTER_LIMIT  -> ciunter++
                        //3.0 <= melting_counter <=7                    -> melting + counter++
                        if(melting_counter == MELTING_COUNTER_LIMIT){
                            putChar(0xa6);
//                            putChar('6');
                            melting_counter = 0;
                        } else if ((7 < melting_counter)&&(melting_counter<MELTING_COUNTER_LIMIT)){
                            putChar(0xa7);
//                            putChar('7');
                            melting_counter++;
                        } else {
//                            putChar(0xa8);
                            
//                            //*---wait for 200s---*/    ***FIXME need to delete comment out!!
                            for(UBYTE i=0; i<50; i++){
                                sendPulseWDT();
                                delay_s(WAIT_TIME_FOR_SETTING);
                            }
                            sendPulseWDT();
                            
                            if(melting_counter<4){
//                                putChar(0xa9);
//                                putChar('9');
//                                //***FIXME*** wire melting!! be careful!!
                                sendCommand('t','p','t', OnOff_forCutWIRE, CutWIRE_SHORT_highTime, CutWIRE_SHORT_lowTime, 0x01, 0x00);
                            } else {
//                                putChar(0xa0);
//                                putChar('0');
//                                //***FIXME***  wire melting!! be careful!!
                                sendCommand('t','p','t', OnOff_forCutWIRE, CutWIRE_LONG_highTime, CutWIRE_LONG_lowTime, 0x01, 0x00);
                            }
                            melting_counter++;
//                            putChar(0xb1);
                        }

    //                    putChar(0xaa);

                        /*---write melting counter to main and sub EEPROM---*/
                        WriteOneByteToMainAndSubB0EEPROM(MeltingCounter_addressHigh, MeltingCounter_addressLow, melting_counter);

//                        /*---for debug (check melting counter)---*/
//                        melting_counter = ReadEEPROM(MAIN_EEPROM_ADDRESS, MeltingCounter_addressHigh, MeltingCounter_addressHigh);
//                        putChar(melting_counter);
                        
                        return no_error;
                        break;
                    default:
                        return error_initialOpe_satMode;
                        break;
                }    
            } else {
                /*---melting is already finished---*/
//                putChar(0xab);
                return error_initialOpe_alreadyMelting;
            }              
            break;
        default:
//            putChar(0xac);
            return error_initialOpe_OBCstatus;
            break;    
    }
}

//TODO:need debug
//ex: 0b01101011 -> 0+1+1+0+1+0+1+1=5
UBYTE bitCalResult(UBYTE bit_input){
    UBYTE bit_cal_result = 0; 
    for(UBYTE cal_counter=0; cal_counter<8; cal_counter++){
        if((bit_input & 1)==1){
            bit_cal_result = bit_cal_result + 1;
        } else {
            bit_cal_result = bit_cal_result + 0;
        }
        bit_input = bit_input >>1;
    }
    return bit_cal_result;
}


UBYTE checkMeltingStatus(UBYTE e_address){
    /*---read melting status---*/
    UBYTE temp;
    temp = ReadEEPROM(e_address, MeltingStatus_addressHigh, MeltingStatus_addressLow);
        
    /*---0xff -> I2C error -> melting status is reset---*/
    if(temp==0xff){
        temp = 0x00;
    }
    
    /*---bit operation---*/
    //ex: 0b01101011 -> 0+1+1+0+1+0+1+1=5
    temp = bitCalResult(temp);
    return temp;
}