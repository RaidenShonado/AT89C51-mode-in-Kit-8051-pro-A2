#ifndef SNAKE_H
#define SNAKE_H

#include <matrix.h>
#include <INTRINS.h>
#include <REG51.h>

sbit Row0 = P1^7;
sbit Row1 = P1^6;
sbit Row2 = P1^5;
sbit Row3 = P1^4;

sbit Col0 = P1^3;
sbit Col1 = P1^2;
sbit Col2 = P1^1;
sbit Col3 = P1^0;

sbit SpeedUp = P3^2;
sbit SpeedDown = P3^3;
sbit K2 = P3^0;
sbit K1 = P3^1;

unsigned char snake[64];
unsigned char food;
unsigned char direction;
unsigned char last_direction;
unsigned char snake_length;
unsigned char display_buffer[8];
unsigned int Speed = 60;
bit game_paused = 0;

void generate_new_food();

void custom_delay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 120; j++);
}

void init_game() {
    unsigned char i;

    // Kh?i t?o m?ng r?n
    for (i = 0; i < 64; i++) {
        snake[i] = 0;
    }

    // T?o v? trí ng?u nhiên cho con r?n (2 ph?n t? d?u tiên)
    snake[0] = (unsigned char)((unsigned int)rand() % 64);  // V? trí ng?u nhiên cho ph?n d?u
    snake[1] = (unsigned char)((unsigned int)rand() % 64);  // V? trí ng?u nhiên cho ph?n thân

    // Ð?m b?o ph?n d?u và ph?n thân không trùng v? trí
    while (snake[1] == snake[0]) {
        snake[1] = (unsigned char)((unsigned int)rand() % 64);
    }

    // T?o v? trí ng?u nhiên cho th?c an
    generate_new_food();  // G?i hàm d? t?o th?c an

    // Kh?i t?o hu?ng và d? dài r?n
    direction = 3;
    last_direction = 3;
    snake_length = 2;
}

// Ð?nh nghia hàm generate_new_food
void generate_new_food() {
    unsigned char i, is_valid;
    do {
        is_valid = 1;
        food = (food + 17) % 64;  // T?o ra v? trí th?c an ng?u nhiên

        // Ki?m tra xem th?c an có trùng v?i ph?n nào c?a con r?n không
        for (i = 0; i < snake_length; i++) {
            if (food == snake[i]) {
                is_valid = 0;  // N?u trùng, ti?p t?c t?o l?i v? trí m?i
                break;
            }
        }
    } while (is_valid == 0);  // Ti?p t?c cho d?n khi tìm du?c v? trí h?p l?
}

void check_direction() {
    unsigned char new_direction = last_direction;

    Row1 = 0;
    if (Col0 == 0) {
        if (last_direction != 1) {
            new_direction = 0;
        }
    }
    Row1 = 1;

    Row1 = 0;
    if (Col2 == 0) {
        if (last_direction != 0) {
            new_direction = 1;
        }
    }
    Row1 = 1;

    Row2 = 0;
    if (Col1 == 0) {
        if (last_direction != 3) {
            new_direction = 2;
        }
    }
    Row2 = 1;

    Row0 = 0;
    if (Col1 == 0) {
        if (last_direction != 2) {
            new_direction = 3;
        }
    }
    Row0 = 1;

    if (new_direction != last_direction) {
        direction = new_direction;
        last_direction = new_direction;
    }
}

void update_snake() {
    unsigned char i;
    for (i = snake_length; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    switch (direction) {
        case 0:
            snake[0] = (snake[0] - 8 + 64) % 64;
            break;
        case 1:
            snake[0] = (snake[0] + 8) % 64;
            break;
        case 2:
            if (snake[0] % 8 == 0)
                snake[0] += 7;
            else
                snake[0] -= 1;
            break;
        case 3:
            if (snake[0] % 8 == 7)
                snake[0] -= 7;
            else
                snake[0] += 1;
            break;
    }
}

void check_collision() {
    unsigned char i;
    for (i = 1; i < snake_length; i++) {
        if (snake[0] == snake[i]) {
            init_game();
            return;
        }
    }

    if (snake[0] == food) {
        if (snake_length < 63) {
            snake_length++;
        }
        generate_new_food();
    }
}

void update_display_buffer() {
    unsigned char i, row, col;
    for (i = 0; i < 8; i++) {
        display_buffer[i] = 0x00;
    }

    for (i = 0; i < snake_length; i++) {
        row = snake[i] / 8;
        col = snake[i] % 8;
        display_buffer[row] |= (1 << col);
    }

    row = food / 8;
    col = food % 8;
    display_buffer[row] |= (1 << col);
}

void check_speed() {
    if (SpeedUp == 0) {
        if (Speed > 10) {
            Speed -= 10;
        }
    }

    if (SpeedDown == 0) {
        if (Speed < 60) {
            Speed += 10;
        }
    }
}

void toggle_game_pause() {
    static bit K2_last_state = 1;

    if (K2 == 0 && K2_last_state == 1) {
        game_paused = !game_paused;
        K2_last_state = 0;
    }

    if (K2 == 1) {
        K2_last_state = 1;
    }
}

void check_reset() {
    if (K1 == 0) {
        init_game();
        game_paused = 0;
    }
}

void Running_Game() {
    toggle_game_pause();

    check_reset();

    if (game_paused) {
        return;
    }

    check_direction();
    check_speed();
    update_snake();
    check_collision();
    update_display_buffer();
}

#endif
