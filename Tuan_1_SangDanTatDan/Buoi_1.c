#include <reg51.h>

void delay_ms(unsigned int time) {
    unsigned int i, j;
    for(i = 0; i < time; i++)
        for(j = 0; j < 123; j++);
}

void main() {
    unsigned char i;
    while(1) {
        // Sáng d?n t? P1.0 d?n P1.7
        for(i = 0; i < 8; i++) {
            P1 = (1 << (i + 1)) - 1; // B?t t?t c? LED t? P1.0 d?n P1.i
						
            delay_ms(100);
        }

        // T?t d?n t? P1.0 d?n P1.7
        for(i = 0; i < 8; i++) {
            P1 = 0xFF ^ ((1 << (i + 1)) - 1); // T?t LED t? P1.0 d?n P1.i
            delay_ms(100);
        }

        // T?t t?t c? LED
        P1 = 0x00;
				P3 = 0x00;// T?t t?t c? LED
        delay_ms(100);
    }
}
