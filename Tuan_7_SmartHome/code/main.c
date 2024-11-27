#include "IR_Reading.h" // Thu vi?n d?c IR
#include "reg51.h"     // Thu vi?n d?nh nghia vi di?u khi?n 8051
#include "XPT2046.h"   // Thu vi?n cho XPT2046 (n?u c?n)

sbit K1 = P3^1; // N�t K1
sbit K2 = P3^0; // N�t K2
sbit K3 = P3^2; // N�t K3
sbit LED3 = P2^2; // ��n LED 3
sbit LED2 = P2^1; // ��n LED 2
sbit LED7 = P2^6;
sbit LED8 = P2^7;


unsigned char code smgduan[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
                                   0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; // M� hi?n th? cho 0-F
unsigned char countdown = 15; // Th?i gian d?m ngu?c b?t d?u t? 15s (F)
unsigned char timer_running = 0; // Bi?n ki?m tra tr?ng th�i h?n gi?
unsigned int temp; // Bi?n t?m d? luu gi� tr? d?c t? c?m bi?n

void Timer1Init() {
    TMOD |= 0X10; // Ch? d? 1 cho Timer1
    TH1 = 0xFC;   // Gi� tr? kh?i t?o
    TL1 = 0x18;   
    ET1 = 1;      // Cho ph�p ng?t Timer1
    EA = 1;       // Cho ph�p ng?t to�n c?c
    TR1 = 1;      // Kh?i d?ng Timer1
}

void display(unsigned char value) {
    if (value > 15) value = 15; // �?m b?o kh�ng vu?t qu� 15
    P0 = smgduan[value]; // Hi?n th? gi� tr? countdown
}

void main() {
    unsigned char count = 0; // Bi?n d?m d? di?u khi?n th?i gian d?c
    Initial(); // Kh?i t?o c?m bi?n
    LED2 = 1; // T?t LED2 ban d?u
		LED7 = 1;
		LED8 = 1;
    Timer1Init(); // Kh?i d?ng Timer1
    display(countdown); // Hi?n th? gi� tr? ban d?u

    while (1) {
        Button_Matrix_Check(); // Ki?m tra tr?ng th�i b�n ph�m

        // Ki?m tra tr?ng th�i n�t K1
        if (K1 == 0) { // N�t K1 du?c nh?n
            while (K1 == 0); // �?i n�t nh? ra
            timer_running = !timer_running; // �?o tr?ng th�i c?a timer_running
        }

        // Ki?m tra tr?ng th�i n�t K2
        if (K2 == 0) { // N�t K2 du?c nh?n
            while (K2 == 0); // �?i n�t nh? ra
            if (countdown < 15) {
                countdown++; // Tang th?i gian
                display(countdown); // C?p nh?t hi?n th?
            }
        }

        // Ki?m tra tr?ng th�i n�t K3
        if (K3 == 0) { // N�t K3 du?c nh?n
            while (K3 == 0); // �?i n�t nh? ra
            if (countdown > 0) {
                countdown--; // Gi?m th?i gian
                display(countdown); // C?p nh?t hi?n th?
            }
        }

        if (count == 50) { // �?c d? li?u m?i 50 l?n l?p
            count = 0;
            temp = Read_AD_Data(0xA4); // �?c d? li?u t? c?m bi?n
        }
        count++;

        // �i?u khi?n LED2 d?a tr�n gi� tr? d?c t? c?m bi?n
        if (temp < 100) {
            LED2 = 0; // B?t LED2 n?u gi� tr? d?c du?i 100
        } else if (temp > 100) {
            LED2 = 1; // T?t LED2 n?u gi� tr? d?c tr�n 100
        }
    }
}

void Timer1() interrupt 3 {
    static unsigned int i;
    TH1 = 0xFC; // Thi?t l?p l?i gi� tr? cho Timer1
    TL1 = 0x18;
    i++;
    if (i == 1000) { // Sau 1 gi�y
        i = 0;
        if (timer_running && countdown > 0) {
            countdown--; // Gi?m th?i gian h?n gi?
            display(countdown); // C?p nh?t gi� tr? hi?n th?
            if (countdown == 0) {
                LED3 = ~LED3; // �?o tr?ng th�i c?a LED3 khi h?t th?i gianac
                timer_running = 0; // D?ng h?n gi?
            }
        }
    }
}
