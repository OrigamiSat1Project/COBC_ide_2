/* 
 * File:            PIN_define
 * Author:          Britta
 * Comments:        defines/asigns all PINs of the PIC
 *                  see also COBC_pin_assignment file on google drive
 * Revision history: 
 */

#include <xc.h>  


#ifndef PIN_DEFINE_H
#define	PIN_DEFINE_H
//------------------------------------------------------------------------------
// All PIN Asignments:

// PINs that are already used in code
//-----------------------------------
//Original Source: UART.h
#define TXOBC_MULTI     PORTCbits.RC5 //Turn on 5R8G (high -> on)??
//Original Source: Init_MPU.h
#define LED_YELLOW      PORTCbits.RC2 //double
#define LED_WHITE       PORTBbits.RB2 //double
#define FX614_RXD       PORTBbits.RB5 //NOT USED!? //Receive radio data
//Original Source: encode_AX25.h
#define TXD             PORTBbits.RB2 //double definition with LED_WHITE, but code is NOT used!
//Original Source: FMCW.h
    /* PIN assignment for FMRX */
#define FMRX_CLK        PORTAbits.RA2
#define FMRX_DAT        PORTAbits.RA3
#define FMRX_STB        PORTBbits.RB0
    /* PIN assignment for FMTX */
#define FMTX_CLK        PORTEbits.RE2
#define FMTX_DAT        PORTEbits.RE1
#define FMTX_STB        PORTEbits.RE0
#define FMTX_PTT        PORTCbits.RC2 //double definition with LED_YELLOW
    /* PIN assignment for CWTX */
#define CWTX_CLK        PORTDbits.RD2
#define CWTX_DAT        PORTDbits.RD1
#define CWTX_STB        PORTDbits.RD0
#define CWTX_KEY        PORTDbits.RD3 //Turn on OBC sub power (high -> on)??
//Original Source: EPS.h
#define SEP_SW          PORTBbits.RB4 //Short SEP SW 1&2
#define RBF_SW          PORTAbits.RA1 //Short RBF SW 1&2

// PINs NOT used in code
//--------------------------
//PORTAbits.RA0 //BAT_POS voltage(10/43)
//PORTAbits.RA4 //Pull down to GND
//PORTAbits.RA5 //Pull down to GND
//PORTBbits.RB1 //Send pulse to WDT
//PORTBbits.RB3 //turn off WDT(high -> WDT off)
//PORTBbits.RB6 //PGC
//PORTBbits.RB7 //PGD
//PORTCbits.RC0 //TXCOBC-3(RA4)
//PORTCbits.RC1 //TXCOBC-22(RB4)
//PORTCbits.RC3 //I2C SCL
//PORTCbits.RC4 //I2C SCL
//PORTCbits.RC5 //Turn on 5R8G (high -> on)
//PORTCbits.RC6 //UART TX
//PORTCbits.RC7 //UART RX (to debug connector)
//PORTDbits.RD4 //12V status (Low -> over current 12V shut down)
//PORTDbits.RD5 //OBC H2-50 (49)
//PORTDbits.RD6 //Pull down to GND
//PORTDbits.RD7 //OBC H1-49 (55)
//------------------------------------------------------------------------------
#endif	/* PIN_DEFINE_H */

