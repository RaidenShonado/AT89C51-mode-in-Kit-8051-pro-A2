#ifndef _IF_READING_
#define _IF_READING_

#include "Base_Lib.h"
#include "reg51.h"

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
sbit L1 = P2^0; // Chỉ giữ lại LED 0

// Button matrix pins
sbit Row = P1^7;  // P17 kết nối chung đầu của các nút nhấn
sbit Col1 = P1^3; // P13 nối với Button1

// Final data_frame
uint32 data_frame = 0;
// Temporary storing unfinished  data-frame while receiving.
uint32 buffer = 0;
// Mili-second count
uint8 ms_count = 0;
// bit-count
int8 negedge_count = 0;

void IR_Reading_Initial(){
    // Enable the Global Interrupt bit
    EA  = 1;
    // Configure INT0 falling edge interrupt
    IT0 = 1;   
    // Enable the INT0 External Interrupt    
    EX0 = 1;
    // Enable Timer 1
    ET0 = 1;
    // Set mode Timer 1: 16 bits
    TMOD = TMOD|0x01;
    // Start timer1
    IndicatorLED = 1;
    DataRcv = 1;
    buffer = 0;
    data_frame = 0;
    negedge_count = 0;
    START_TIMER_0();
}

void Initial(){
    // Run initial 
    IR_Reading_Initial();
    // SET all LED-7seg OFF by set a->g = L.
    P0 = 0;
    // SET all LED OFF by set Port 2 = H.
    P2 = 0xFF;
}

void LED_Show(uint32 CODE){
    switch (CODE) {
        case 0xFF30CF:
            L1 = ~L1; // Chỉ điều khiển LED 0
            break;
        default:
            P2 = 0xFF; // Tắt tất cả LED
    }
}

// Hàm kiểm tra ma trận nút nhấn 4x4 (ở đây chỉ sử dụng 1 hàng và 3 cột)
void Button_Matrix_Check(){
    Row = 0;  // Kéo dòng P17 xuống mức thấp để bắt đầu quét

    // Kiểm tra trạng thái của Button1 (P17 và P13)
    if (Col1 == 0) {
        delay_ms(20);  // Tránh rung phím (debouncing)
        if (Col1 == 0) {
            L1 = ~L1;  // Đảo trạng thái LED L1
        }
        while (Col1 == 0);  // Chờ nút nhấn được nhả
    }
    Row = 1;  // Kết thúc quét, trả dòng P17 về mức cao
}

void Timer0_OverFlow_Interrupt() interrupt 1 {
    IndicatorLED = ~IndicatorLED;
    RESET_TIMER_0();
    // A data-frame isn't longer than 67.5 mili-sec.
    if(ms_count < 50) ms_count = ms_count + 1;
}

void External0_Interrupt() interrupt 0 {
    uint32 current_mscount = 0; 
    current_mscount = ms_count;
    RESET_TIMER_0();
    ms_count = 0;
    negedge_count += 1;
    DataRcv = ~DataRcv;
    // this neg-edge by SOF (start of frame)?
    if(current_mscount >= 50){
        negedge_count = -2;
        RESET_BUFFER();
    } else {
        if(negedge_count < 0)
            /*Do nothing, skip this neg-edge*/;
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
#endif
