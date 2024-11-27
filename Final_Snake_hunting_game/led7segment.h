#ifndef LED7SEGMENT_H
#define LED7SEGMENT_H

#include <REG51.h>

// M� hi?n th? LED 7 do?n
unsigned char code LED7SEG[10] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
};

// Hi?n th? m?t ch? s?
void display_digit(unsigned char led_position, unsigned char number) {
    P2 &= ~(1 << 2); // T?t LED1 (P2.2)
    P2 &= ~(1 << 3); // T?t LED2 (P2.3)

    P0 = LED7SEG[number]; // G?i d? li?u l�n P0

    if (led_position == 1) {
        P2 |= (1 << 2); // B?t LED1 (P2.2)
    } else if (led_position == 2) {
        P2 |= (1 << 3); // B?t LED2 (P2.3)
    }

    delay(5); // Delay d? tr�nh nh?p nh�y
}

// Hi?n th? di?m tr�n hai LED 7 do?n
void display_score(unsigned int score) {
    unsigned char tens = (score / 10) % 10;  // H�ng ch?c
    unsigned char units = score % 10;       // H�ng don v?

    display_digit(2, tens);  // Hi?n th? h�ng ch?c
    display_digit(1, units); // Hi?n th? h�ng don v?
}

#endif
