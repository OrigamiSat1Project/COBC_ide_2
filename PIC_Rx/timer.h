

#ifndef TIMER_H
#define	TIMER_H

#include "typeDefine.h"

#define _XTAL_FREQ       10000000       // Clock frequency
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

#define one_second 38
#define one_minute 60
#define one_hour 60
#define one_day 24
#define one_week 7

#define WDT_INTERVAL             4
#define EPS_RSET_INTERVAL_LONG   10
#define EPS_RSET_INTERVAL_SHORT  5
#define INITIAL_OPE_INTERVAL     5
#define COMMAND_COUNTER_INTERVAL 1      //for debug 1min
#define EPS_MEASURE_INTERVAL 1          //for debug 1min



void initTimer(void);
void interrupt TimerCheck(void);
void InitialOperation(void);
UBYTE checkMeltingStatus(UBYTE);

void set_receive_command_counter(UINT, UINT);

UINT get_receive_command_counter_min(void);

void set_init_ope_counter(UINT, UINT);

UINT get_init_ope_counter_min(void);

void set_bat_meas_counter(UINT, UINT);

UINT get_bat_meas_counter_sec_min(void);

UINT get_timer_counter(UBYTE unit);

void reset_timer(void);

//void interrupt TimerReset(void);

#endif	/* TIME_H */