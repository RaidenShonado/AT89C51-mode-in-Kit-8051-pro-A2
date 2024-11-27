#ifndef LED7SEGMENT_H
#define LED7SEGMENT_H

#include <REG51.h>

// Mã hi?n th? LED 7 do?n
unsigned char code LED7SEG[10] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
};

// Hi?n th? m?t ch? s?
void display_digit(unsigned char led_position, unsigned char number) {
    P2 &= ~(1 << 2); // T?t LED1 (P2.2)
    P2 &= ~(1 << 3); // T?t LED2 (P2.3)

    P0 = LED7SEG[number]; // G?i d? li?u lên P0

    if (led_position == 1) {
        P2 |= (1 << 2); // B?t LED1 (P2.2)
    } else if (led_position == 2) {
        P2 |= (1 << 3); // B?t LED2 (P2.3)
    }

    delay(5); // Delay d? tránh nh?p nháy
}

// Hi?n th? di?m trên hai LED 7 do?n
void display_score(unsigned int score) {
    unsigned char tens = (score / 10) % 10;  // Hàng ch?c
    unsigned char units = score % 10;       // Hàng don v?

    display_digit(2, tens);  // Hi?n th? hàng ch?c
    display_digit(1, units); // Hi?n th? hàng don v?
}

#endif
