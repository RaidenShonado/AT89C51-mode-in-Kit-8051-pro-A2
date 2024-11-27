#ifndef _IF_READING_
#define _IF_READING_

#include "Base_Lib.h"
#include "REGX52.h"

// Reset timer at 0xFC67
#define RESET_TIMER_0() TH0=0xFC; TL0=0x67;
#define START_TIMER_0() TR0 = 1; 
#define STOP_TIMER() TR0 = 0; 
#define PUSH_BIT_1() buffer |=(uint32)1<<(31-negedge_count);
#define PUSH_BIT_0() /*do nothing*/;
#define RESET_BUFFER() buffer=0;
#define EXTRACT_FRAME() data_frame=buffer; buffer = 0; negedge_count = 0;

// Based on the diagram of "8051 Pro" Kit
sbit IR_RCV_PIN = P3^2;
sbit IndicatorLED = P2^7;
sbit DataRcv = P2^6;
sbit FrameExtracted = P2^5;
sbit L0 = P2^0;
sbit L1 = P2^1;
sbit L2 = P2^2;

sbit ROW4 = P1^4;
sbit ROW3 = P1^5;
sbit ROW2 = P1^6;
sbit ROW1 = P1^7;
sbit COL1 = P1^3;
sbit COL2 = P1^2;
sbit COL3 = P1^1;
sbit COL4 = P1^0;

// Final data_frame
uint32 data_frame = 0;
// Temporary storing unfinished  data-frame while receiving.
uint32 buffer = 0;
// Mili-second count
uint8 ms_count = 0;
// bit-count
int8 negedge_count = 0;

void IR_Reading_Initial(){
    EA  = 1;
    IT0 = 1;   
    EX0 = 1;
    ET0 = 1;
    TMOD = TMOD|0x01;
    IndicatorLED = 1;
    DataRcv = 1;
    buffer = 0;
    data_frame = 0;
    negedge_count = 0;
    START_TIMER_0();
}

void Initial(){
    IR_Reading_Initial();
    P0 = 0;
    P2 = 0xFF;
}

void LED_Show(uint32 CODE){
    switch (CODE) {
        case 0xFF30CF:
            L0 = ~L0;
            break;
        case 0xFF18E7:
            L1 = ~L1;
            break;
        case 0xFF7A85:
            L2 = ~L2;
            break;
        default:
            P2 = 0xFF;
    }
}

void Timer0_OverFlow_Interrupt() interrupt 1 {
    IndicatorLED = ~IndicatorLED;
    RESET_TIMER_0();
    if(ms_count < 50) ms_count += 1;
}

void External0_Interrupt() interrupt 0 {
    uint32 current_mscount = ms_count; 
    RESET_TIMER_0();
    ms_count = 0;
    negedge_count += 1;
    DataRcv = ~DataRcv;

    if(current_mscount >= 50){
        negedge_count = -2;
        RESET_BUFFER();
    } else {
        if(negedge_count < 0);
        if(0 <= negedge_count && negedge_count <= 31){
            if(current_mscount >= 2){
                PUSH_BIT_1();
            } else {
                PUSH_BIT_0();
            }
        } else if(negedge_count >= 32){
            EXTRACT_FRAME();
            FrameExtracted = 0;
            LED_Show(data_frame);
            FrameExtracted = 1;
        }
    }
}

// Function to scan matrix keys
void Matrix_Key_Scan() {
    ROW1 = 0; ROW2 = 1; ROW3 = 1; ROW4 = 1;
    if (COL1 == 0) { L0 = ~L0; while(COL1 == 0); } // S1
    if (COL2 == 0) { L1 = ~L1; while(COL2 == 0); } // S2
    if (COL3 == 0) { L2 = ~L2; while(COL3 == 0); } // S3
    ROW1 = 1; ROW2 = 0; ROW3 = 1; ROW4 = 1;
    // Other rows if needed
}

#endif
