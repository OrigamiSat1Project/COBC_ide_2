/* 
 * File:   EEPROM.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 2:17
 */

#include <xc.h>


#ifndef EEPROM_H
#define	EEPROM_H

#include "typeDefine.h"

/*******************************************************************************
*EEPROM address
******************************************************************************/
#define MAIN_EEPROM_ADDRESS     0x50
#define SUB_EEPROM_ADDRESS      0x52
#define MAIN_EEPROM_ADDRESS_B1  0x54
#define SUB_EEPROM_ADDRESS_B1   0x56

//for test EEPROM
//TODO:change address
#define datalength_for_test         4
#define B0select_for_testEEPROM     0x00
#define HighAddress_for_testEEPROM  0xE0
#define LowAddress_for_testEEPROM   0x00


/*******************************************************************************
*MPU.c
******************************************************************************/
//address for satellite mode
//TODO:change address
#define deviceOnOff_addressHigh 0x00
#define deviceOnOff_addressLow 0x00
//7:CIB / 6:EPS / 5:OBC / 4:Radio(TX) / 3:Radio(RX)
//                    0b76543210
#define all_device_ON 0b11111000
#define CIB_TX_RX_ON  0b10011000
#define CIB_ON        0b10000000

/*-----------in/out status-----------------*/
//TODO:change address
#define TRISA_addressHigh 0x00
#define TRISA_addressLow  0x00
#define TRISB_addressHigh 0x00
#define TRISB_addressLow  0x00
#define TRISC_addressHigh 0x00
#define TRISC_addressLow  0x00
#define TRISD_addressHigh 0x00
#define TRISD_addressLow  0x00
#define TRISE_addressHigh 0x00
#define TRISE_addressLow  0x00

/*-----------high/low status-----------------*/
//TODO:change address
#define PORTA_addressHigh 0x00
#define PORTA_addressLow  0x00
#define PORTB_addressHigh 0x00
#define PORTB_addressLow  0x00
#define PORTC_addressHigh 0x00
#define PORTC_addressLow  0x00
#define PORTD_addressHigh 0x00
#define PORTD_addressLow  0x00
#define PORTE_addressHigh 0x00
#define PORTE_addressLow  0x00


/*******************************************************************************
*OK and ERROR
******************************************************************************/
#define B0select_for_commandID     0x00
#define HighAddress_for_commandID  0xE0
#define LowAddress_for_commandID   0x00

#define B0select_for_Ok       0x00
#define HighAddress_for_Ok    0xE0
#define LowAddress_for_Ok     0x00

#define B0select_for_Error     0x00
#define HighAddress_for_Error  0x97
#define LowAddress_for_Error   0x09

#define InitialOpe_error_status_addressHigh     0x81
#define InitialOpe_error_status_addressLow      0x0c


#define SatMode_error_status1_addresshigh 0x81
#define SatMode_error_status1_addresslow 0x14
#define SatMode_error_status2_addresshigh 0x81
#define SatMode_error_status2_addresslow 0x15
#define SW_Change_SavingMode_ADC_addresshigh 0x81
#define SW_Change_SavingMode_ADC_addresslow 0x13

/*******************************************************************************
*Last Command ID
******************************************************************************/
#define B0select_for_RXCOBCLastCommand     0x00
#define HighAddress_for_RXCOBCLastCommand  0x97
#define LowAddress_for_RXCOBCLastCommand   0x08


/*******************************************************************************
*DATA
******************************************************************************/
/*---initial operation---*/
#define MeltingStatus_addressHigh      0x97
#define MeltingStatus_addressLow       0x07
#define BatteryVoltage_addressHigh     0x81
#define BatteryVoltage_addressLow      0x87
#define MeltingCounter_addressHigh     0x81
#define MeltingCounter_addressLow      0x0b
#define SatelliteMode_addressHigh       0x81//include SEP(3,2bit) and RBF(1,0bit)
#define SatelliteMode_addressLow        0x80//include SEP(3,2bit) and RBF(1,0bit)

#define BatVol_nominal_saving_datahigh_addressHigh  0x81
#define BatVol_nominal_saving_datahigh_addressLow 0x01
#define BatVol_nominal_saving_datalow_addressHigh  0x81
#define BatVol_nominal_saving_datalow_addressLow 0x02
#define BatVol_saving_survival_datahigh_addressHigh  0x81
#define BatVol_saving_survival_datahigh_addressLow 0x0D
#define BatVol_saving_survival_datalow_addressHigh  0x81
#define BatVol_saving_survival_datalow_addressLow 0x0E

#define BatVol_nominal_revival_datahigh_addressHigh 0x81
#define BatVol_nominal_revival_datahigh_addressLow 0x0F
#define BatVol_nominal_revival_datalow_addressHigh 0x81
#define BatVol_nominal_revival_datalow_addressLow 0x10
#define BatVol_saving_revival_datahigh_addressHigh 0x81
#define BatVol_saving_revival_datahigh_addressLow 0x11
#define BatVol_saving_revival_datalow_addressHigh 0x81
#define BatVol_saving_revival_datalow_addressLow 0x12

#endif	/* EEPROM_H */

