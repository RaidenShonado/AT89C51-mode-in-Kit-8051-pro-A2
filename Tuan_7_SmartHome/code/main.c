#include "IR_Reading.h" // Thu vi?n d?c IR
#include "reg51.h"     // Thu vi?n d?nh nghia vi di?u khi?n 8051
#include "XPT2046.h"   // Thu vi?n cho XPT2046 (n?u c?n)

sbit K1 = P3^1; // Nút K1
sbit K2 = P3^0; // Nút K2
sbit K3 = P3^2; // Nút K3
sbit LED3 = P2^2; // Ðèn LED 3
sbit LED2 = P2^1; // Ðèn LED 2
sbit LED7 = P2^6;
sbit LED8 = P2^7;


unsigned char code smgduan[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
                                   0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; // Mã hi?n th? cho 0-F
unsigned char countdown = 15; // Th?i gian d?m ngu?c b?t d?u t? 15s (F)
unsigned char timer_running = 0; // Bi?n ki?m tra tr?ng thái h?n gi?
unsigned int temp; // Bi?n t?m d? luu giá tr? d?c t? c?m bi?n

void Timer1Init() {
    TMOD |= 0X10; // Ch? d? 1 cho Timer1
    TH1 = 0xFC;   // Giá tr? kh?i t?o
    TL1 = 0x18;   
    ET1 = 1;      // Cho phép ng?t Timer1
    EA = 1;       // Cho phép ng?t toàn c?c
    TR1 = 1;      // Kh?i d?ng Timer1
}

void display(unsigned char value) {
    if (value > 15) value = 15; // Ð?m b?o không vu?t quá 15
    P0 = smgduan[value]; // Hi?n th? giá tr? countdown
}

void main() {
    unsigned char count = 0; // Bi?n d?m d? di?u khi?n th?i gian d?c
    Initial(); // Kh?i t?o c?m bi?n
    LED2 = 1; // T?t LED2 ban d?u
		LED7 = 1;
		LED8 = 1;
    Timer1Init(); // Kh?i d?ng Timer1
    display(countdown); // Hi?n th? giá tr? ban d?u

    while (1) {
        Button_Matrix_Check(); // Ki?m tra tr?ng thái bàn phím

        // Ki?m tra tr?ng thái nút K1
        if (K1 == 0) { // Nút K1 du?c nh?n
            while (K1 == 0); // Ð?i nút nh? ra
            timer_running = !timer_running; // Ð?o tr?ng thái c?a timer_running
        }

        // Ki?m tra tr?ng thái nút K2
        if (K2 == 0) { // Nút K2 du?c nh?n
            while (K2 == 0); // Ð?i nút nh? ra
            if (countdown < 15) {
                countdown++; // Tang th?i gian
                display(countdown); // C?p nh?t hi?n th?
            }
        }

        // Ki?m tra tr?ng thái nút K3
        if (K3 == 0) { // Nút K3 du?c nh?n
            while (K3 == 0); // Ð?i nút nh? ra
            if (countdown > 0) {
                countdown--; // Gi?m th?i gian
                display(countdown); // C?p nh?t hi?n th?
            }
        }

        if (count == 50) { // Ð?c d? li?u m?i 50 l?n l?p
            count = 0;
            temp = Read_AD_Data(0xA4); // Ð?c d? li?u t? c?m bi?n
        }
        count++;

        // Ði?u khi?n LED2 d?a trên giá tr? d?c t? c?m bi?n
        if (temp < 100) {
            LED2 = 0; // B?t LED2 n?u giá tr? d?c du?i 100
        } else if (temp > 100) {
            LED2 = 1; // T?t LED2 n?u giá tr? d?c trên 100
        }
    }
}

void Timer1() interrupt 3 {
    static unsigned int i;
    TH1 = 0xFC; // Thi?t l?p l?i giá tr? cho Timer1
    TL1 = 0x18;
    i++;
    if (i == 1000) { // Sau 1 giây
        i = 0;
        if (timer_running && countdown > 0) {
            countdown--; // Gi?m th?i gian h?n gi?
            display(countdown); // C?p nh?t giá tr? hi?n th?
            if (countdown == 0) {
                LED3 = ~LED3; // Ð?o tr?ng thái c?a LED3 khi h?t th?i gianac
                timer_running = 0; // D?ng h?n gi?
            }
        }
    }
}
