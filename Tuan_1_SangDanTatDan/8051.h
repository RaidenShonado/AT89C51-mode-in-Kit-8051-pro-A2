#ifndef _8051_H_
#define _8051_H_

#include <intrins.h>

// Ð?nh nghia các c?ng
sfr P0 = 0x80; // C?ng 0
sfr P1 = 0x90; // C?ng 1
sfr P2 = 0xA0; // C?ng 2
sfr P3 = 0xB0; // C?ng 3

// Ð?nh nghia các thanh ghi di?u khi?n
sfr IE  = 0xA8; // Thanh ghi cho ng?t
sfr IP  = 0xB8; // Thanh ghi uu tiên ng?t
sfr TMOD = 0x89; // Thanh ghi ch? d? Timer
sfr TCON = 0x88; // Thanh ghi di?u khi?n Timer

// Ð?nh nghia các thanh ghi b? d?m
sfr TH0 = 0x8C; // Thanh ghi cao Timer 0
sfr TL0 = 0x8A; // Thanh ghi th?p Timer 0
sfr TH1 = 0x8D; // Thanh ghi cao Timer 1
sfr TL1 = 0x8B; // Thanh ghi th?p Timer 1

// Ð?nh nghia các thanh ghi b? nh?
sfr ACC = 0xE0; // Thanh ghi tích luy
sfr B   = 0xF0; // Thanh ghi B

// Ð?nh nghia các thanh ghi tr?ng thái
sfr PSW = 0xD0; // Thanh ghi tr?ng thái

// Ð?nh nghia các h?ng s?
#define SET_BIT(x, y) (x |= (1 << y))
#define CLR_BIT(x, y) (x &= ~(1 << y))
#define TOG_BIT(x, y) (x ^= (1 << y))

#endif // _8051_H_07:35/-strong/-heart:>:o:-((:-hÐã g?iXem tru?c khi g?iTh? Files vào dây d? xem l?i tru?c khi g?i