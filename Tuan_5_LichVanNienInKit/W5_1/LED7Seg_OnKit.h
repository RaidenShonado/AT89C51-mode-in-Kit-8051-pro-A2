//---------- Include -----------
#include <REGX52.h>
#include <stdio.h>


//---------- Macros -----------
#define elif else if
#define  DECREASE_ONE(VAR) VAR = (VAR>0?(VAR-1):VAR)
#define  FOR(i, a, b) for(i = (a); i <= (b); ++i)
#define  FOR_reverse(i, a, b) for(i = (a); i >= (b); --i)

typedef unsigned int  UINT;

sbit GND0 = P2^2;
sbit GND1 = P2^3;
sbit GND2 = P2^4;

#define LED_7SEG P0

UINT DISP_TIME = 10;

const UINT DIGIT_CODE[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D,
                           0X7D, 0X07, 0X7F, 0X6F, 0X0, 0X1, 0x40, 0x8};

void led7seg_disp(UINT  POS, UINT  CODE){
    if( POS == 0x1){ GND0 = 0; GND1 = 0; GND2 = 0; LED_7SEG = CODE;}
    if( POS == 0x2){ GND0 = 1; GND1 = 0; GND2 = 0; LED_7SEG = CODE;}
    if( POS == 0x3){ GND0 = 0; GND1 = 1; GND2 = 0; LED_7SEG = CODE;}
    if( POS == 0x4){ GND0 = 1; GND1 = 1; GND2 = 0; LED_7SEG = CODE;}
    if( POS == 0x5){ GND0 = 0; GND1 = 0; GND2 = 1; LED_7SEG = CODE;}
    if( POS == 0x6){ GND0 = 1; GND1 = 0; GND2 = 1; LED_7SEG = CODE;}
    if( POS == 0x7){ GND0 = 0; GND1 = 1; GND2 = 1; LED_7SEG = CODE;}
    if( POS == 0x8){ GND0 = 1; GND1 = 1; GND2 = 1; LED_7SEG = CODE;}
}

void digit_disp	(UINT POS, UINT DIGIT){
    led7seg_disp(POS, DIGIT_CODE[DIGIT]);
}

void digit_select_disp(UINT LED_POS, UINT SEL_POS, UINT DIGIT){
    if( LED_POS == 0x1){ GND0 = 0; GND1 = 0; GND2 = 0; LED_7SEG = DIGIT_CODE[DIGIT] + (LED_POS==SEL_POS)?(0x80):(0x0);}
    if( LED_POS == 0x2){ GND0 = 1; GND1 = 0; GND2 = 0; LED_7SEG = DIGIT_CODE[DIGIT] + (LED_POS==SEL_POS)?(0x80):(0x0);;}
    if( LED_POS == 0x3){ GND0 = 0; GND1 = 1; GND2 = 0; LED_7SEG = DIGIT_CODE[DIGIT] + (LED_POS==SEL_POS)?(0x80):(0x0);;}
    if( LED_POS == 0x4){ GND0 = 1; GND1 = 1; GND2 = 0; LED_7SEG = DIGIT_CODE[DIGIT] + (LED_POS==SEL_POS)?(0x80):(0x0);;}
    if( LED_POS == 0x5){ GND0 = 0; GND1 = 0; GND2 = 1; LED_7SEG = DIGIT_CODE[DIGIT] + (LED_POS==SEL_POS)?(0x80):(0x0);;}
    if( LED_POS == 0x6){ GND0 = 1; GND1 = 0; GND2 = 1; LED_7SEG = DIGIT_CODE[DIGIT] + (LED_POS==SEL_POS)?(0x80):(0x0);;}
    if( LED_POS == 0x7){ GND0 = 0; GND1 = 1; GND2 = 1; LED_7SEG = DIGIT_CODE[DIGIT] + (LED_POS==SEL_POS)?(0x80):(0x0);;}
    if( LED_POS == 0x8){ GND0 = 1; GND1 = 1; GND2 = 1; LED_7SEG = DIGIT_CODE[DIGIT] + (LED_POS==SEL_POS)?(0x80):(0x0);;}
}