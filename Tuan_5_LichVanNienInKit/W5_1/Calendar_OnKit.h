#include "LED7Seg_OnKit.h"
#include "DateTimeLimit.h"

#define A_DIGIT 0x77
#define P_DIGIT 0x73

#define VIEW_DATE 0x0
#define VIEW_TIME 0x1
#define SETTING_DATE 0x2
#define SETTING_TIME 0x3

sbit TRIGGER0 = P3^2;
sbit TRIGGER1 = P3^3;

UINT MODE = VIEW_TIME;
UINT EDIT_POS = 1;
UINT F_EXIT = 0;

DATETIME datetime;

static void delay(unsigned int mili_sec) {
  UINT i;
  for (i = 0; i < 12 * mili_sec; i++)
    if(F_EXIT) return;
}

void trigger_check(){
    if(TRIGGER0 == 0){
        MODE = ++MODE % 2;
    }
}

void calendar_disp(){
	UINT DATE_TIME_DISP = 5;
    UINT MAX_LOOP_DISP = 45;
    //trigger_check();
    increase_seccond(&datetime);
		//run in a seccond
    
    if( MODE == VIEW_DATE ){
        UINT i = 0;
        FOR(i, 1, MAX_LOOP_DISP){
            digit_disp(8, (((datetime.DAY)/10)%10)); 
            delay(DATE_TIME_DISP);
            digit_disp(7, ((datetime.DAY)%10));
            delay(DATE_TIME_DISP);

            digit_disp(6, (((datetime.MONTH)/10)%10)); 
            delay(DATE_TIME_DISP);
            digit_disp(5, ((datetime.MONTH)%10));
            delay(DATE_TIME_DISP);

            digit_disp(4, (((datetime.YEAR)/1000)%10)); 
            delay(DATE_TIME_DISP);
            digit_disp(3, (((datetime.YEAR)/100)%10)); 
            delay(DATE_TIME_DISP);
            digit_disp(2, (((datetime.YEAR)/10)%10)); 
            delay(DATE_TIME_DISP);
            digit_disp(1, ((datetime.YEAR)%10));
            delay(DATE_TIME_DISP);
        }
    }
    if( MODE == VIEW_TIME ){
        UINT i = 0;
        FOR(i, 1, MAX_LOOP_DISP){
            digit_disp(8, ((((datetime.HOUR)%12)/10)%10)); 
            delay(DATE_TIME_DISP);
            digit_disp(7, (((datetime.HOUR)%12)%10));
            delay(DATE_TIME_DISP);
            digit_disp(6, (((datetime.MINUTE)/10)%10)); 
            delay(DATE_TIME_DISP);
            digit_disp(5, ((datetime.MINUTE)%10));
            delay(DATE_TIME_DISP);

            digit_disp(4, (((datetime.SECOND)/10)%10)); 
            delay(DATE_TIME_DISP);
            digit_disp(3, ((datetime.SECOND)%10)); 
            delay(DATE_TIME_DISP);
            led7seg_disp(2, DIGIT_CODE[10]); 
            delay(DATE_TIME_DISP);
            led7seg_disp(1, ((datetime.HOUR)>12)?(P_DIGIT):(A_DIGIT));
            delay(DATE_TIME_DISP);
        }
    }
    EXIT:
        return;
}


static void Interrupt_Action(void) interrupt 0 {
    MODE = (MODE == VIEW_DATE)?(VIEW_TIME): (VIEW_DATE);
}

void calendar_initial(){
    EA = 1;
    EX0 = 1;
    IT0 = 1;
    datetime.YEAR = 2024;
    datetime.MONTH = 9;
    datetime.DAY = 24;
    datetime.HOUR = 10;
    datetime.MINUTE= 26;
    datetime.SECOND = 55;
}