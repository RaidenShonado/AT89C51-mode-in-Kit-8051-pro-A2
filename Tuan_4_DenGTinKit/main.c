#include <reg52.h>

typedef unsigned int u16;
typedef unsigned char u8;

// Pin definitions for the 7-segment display
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

// Pin definitions for the 74HC595
sbit P41 = P3^4;  // SER of 74HC595
sbit P42 = P3^5;  // RCLK of 74HC595
sbit P43 = P3^6;  // SCLK of 74HC595

// Pin definitions for buttons
sbit button_1 = P3^1; // Mode toggle button
sbit button_2 = P3^0; // Pattern toggle button

// 7-segment display code for digits 0-9 and letters A-F
u8 code smgduan[16] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 
    0x6d, 0x7d, 0x07, 0x7f, 0x6f, 
    0x77, 0x7c, 0x39, 0x5e, 0x79, 
    0x71
};

u8 DisplayData[8];
u16 count = 0;
u8 time_led = 21;
bit led_on = 1; // Flag to indicate if LED matrix should be on
bit control_mode = 0; // 0 = Automatic, 1 = Manual
u8 matrix_state = 0x81; // Initial matrix state
bit button_2_last_state = 1; // Last state of button_2

// Function to create a delay
void delay(u16 i) {
    while (i--);
}

// Timer 0 initialization
void Timer0Init() {
    TMOD |= 0x01; // Set Timer 0 mode 1
    TH0 = 0xFC;   // Load timer value for 1ms delay
    TL0 = 0x18;
    ET0 = 1;      // Enable Timer 0 interrupt
    EA = 1;       // Enable global interrupts
    TR0 = 1;      // Start Timer 0
}

// Function to send a byte to the 74HC595
void sendByteTo74HC595(u8 j) {
    u8 i;
    for (i = 0; i < 8; i++) {
        P41 = (j & (0x80 >> i)) ? 1 : 0;
        P43 = 1;
        P43 = 0;
    }
    P42 = 1;
    P42 = 0;
}

// Function to display segments
void DigDisplay() {
    u8 i;
    for (i = 0; i < 8; i++) {
        switch (i) {
            case 0: LSA = 1; LSB = 1; LSC = 1; break; // 7th digit
            case 1: LSA = 0; LSB = 1; LSC = 1; break; // 6th digit
            case 2: LSA = 1; LSB = 0; LSC = 1; break; // 5th digit
            case 3: LSA = 0; LSB = 0; LSC = 1; break; // 4th digit
            case 4: LSA = 1; LSB = 1; LSC = 0; break; // 3rd digit
            case 5: LSA = 0; LSB = 1; LSC = 0; break; // 2nd digit
            case 6: LSA = 1; LSB = 0; LSC = 0; break; // 1st digit
            case 7: LSA = 0; LSB = 0; LSC = 0; break; // 0th digit
        }
        P0 = DisplayData[i];
        delay(100);
        P0 = 0x00; // Clear the display after each digit
    }
}

// Function to update the matrix display
void updateMatrix() {
    P42 = 0;
    P41 = 0;
    P43 = 0;

    if (control_mode) {
        // Manual mode: display the matrix state selected by button_2
        sendByteTo74HC595(matrix_state);
    } else {
        // Automatic mode: change pattern based on `time_led`
        if (time_led > 13) {
            sendByteTo74HC595(0x81);
        } else if (time_led > 10) {
            sendByteTo74HC595(0x41);
        } else if (time_led >= 3) {
            sendByteTo74HC595(0x24);
        } else {
            sendByteTo74HC595(0x22);
        }
    }
    P42 = 1;
}

// Function to process time and update DisplayData
void datapros() {
    u8 chuc_1, dv_1, chuc_2, dv_2;

    if (time_led > 13) {
        chuc_1 = (time_led - 11) / 10;
        dv_1 = (time_led - 11) % 10;
        chuc_2 = (time_led - 14) / 10;
        dv_2 = (time_led - 14) % 10;
    } else if (time_led > 10) {
        chuc_1 = (time_led - 11) / 10;
        dv_1 = (time_led - 11) % 10;
        chuc_2 = chuc_1;
        dv_2 = dv_1;
    } else if (time_led >= 3) {
        chuc_1 = (time_led - 3) / 10;
        dv_1 = (time_led - 3) % 10;
        chuc_2 = time_led / 10;
        dv_2 = time_led % 10;
    } else {
        chuc_1 = 0;
        dv_1 = time_led;
        chuc_2 = 0;
        dv_2 = time_led;
    }

    DisplayData[2] = smgduan[chuc_1];
    DisplayData[3] = smgduan[dv_1];
    DisplayData[1] = 0x00; // Clear unused segments
    DisplayData[0] = 0x00; // Clear unused segments
    DisplayData[6] = smgduan[chuc_2];
    DisplayData[7] = smgduan[dv_2];
    DisplayData[4] = 0x00; // Clear unused segments
    DisplayData[5] = 0x00; // Clear unused segments
}

// Main function
void main() {
    Timer0Init();

    while (1) {
        // Toggle between automatic and manual mode with button_1
        if (!button_1) {
            delay(10000); // Debounce delay
            if (!button_1) {
                control_mode = !control_mode; // Toggle control mode
                if (control_mode) {
                    // When switching to manual mode, turn off 7-segment display
                    u8 i; // Declare the loop variable here
                    for (i = 0; i < 8; i++) {
                        DisplayData[i] = 0x00; // Turn off all segments
                    }
                }
            }
        }

        // Check button_2 for toggling matrix state in manual mode
        if (control_mode) {
            bit button_2_current_state = !button_2; // Current state of button_2
            if (button_2_current_state && !button_2_last_state) { // Button pressed
                matrix_state = (matrix_state == 0x81) ? 0x24 : 0x81; // Swap matrix state
            }
            button_2_last_state = button_2_current_state; // Store last state
        }

        // In automatic mode, update both displays
        if (!control_mode) {
            datapros();
        }

        DigDisplay();
        updateMatrix();
    }
}

// Timer 0 interrupt service routine
void Timer0(void) interrupt 1 {
    if (!control_mode) { // Only update in automatic mode
        TH0 = 0xFC;
        TL0 = 0x18;

        count++;
        
        if (count >= 1000) { // 1 second
            count = 0;
            if (time_led > 0) {
                time_led--;
            } else {
                time_led = 21;
                led_on = !led_on;
            }
        }
    }
}
