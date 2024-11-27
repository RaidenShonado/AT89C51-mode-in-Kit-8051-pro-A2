#include <AT89X52.h>
#include "delay/delay.h"

#define LED1    P3_0  // LED 7-segment 1
#define LED2    P3_1  // LED 7-segment 2
#define LED3    P3_2  // LED 7-segment 3
#define LED4    P3_3  // LED 7-segment 4

#define LED_PORT    P2  // C?ng di?u khi?n 7-segment

#define BT1        P1_6  // Switch 1
#define SPEAK      P1_7  // Switch 2

unsigned char index;
unsigned char led7_data[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90}; // Decimal to Led7seg

unsigned char key_value;
bit start = 0;
unsigned int num_product;
unsigned int num_count = 0;
unsigned char chuc_1, dv_1, chuc_2, dv_2;
unsigned char num_time = 0;
signed char time_led = 61;

#define R1    P1_0
#define Y1    P1_1
#define G1    P1_2
#define R2    P1_3
#define Y2    P1_4
#define G2    P1_5

//------------------------------------------------
void main()
{
    unsigned char j = 0;

    // Timer configuration
    TMOD = 0x11; // Timer 0 and 1 in mode 1 (16-bit)
    TL0 = -50000 % 256;
    TH0 = -50000 / 256;
    TF0 = 1;
    TR0 = 1;
    TH1 = -5000 / 256;
    TL1 = -5000 % 256;
    TF1 = 0;
    TR1 = 1;
    EA = 1; // Enable global interrupt
    ET1 = 1; // Enable Timer 1 interrupt
    IT1 = 1;
    ET0 = 1;

    while (1)
    {
        if (SPEAK == 0) // When P1.7 is low, switch to manual control
        {
            // Turn off all LEDs and counters
            R1 = 0; Y1 = 0; G1 = 0;
            R2 = 0; Y2 = 0; G2 = 0;
            LED1 = 0; LED2 = 0; LED3 = 0; LED4 = 0;

            // Check the state of P1.6 for LED control
            if (BT1 == 1) // When P1.6 is high
            {
                R1 = 1; G2 = 1; // Turn on R1 and G2
            }
            else // When P1.6 is low
            {
                R2 = 1; G1 = 1; // Turn on R2 and G1
            }
        }
        else
        {
            // Original LED and timer logic
            if (time_led > 33)
            {
                R1 = 1; Y1 = 0; G1 = 0;
                R2 = 0; Y2 = 0; G2 = 1;
                chuc_1 = (time_led - 31) / 10;
                dv_1 = (time_led - 31) % 10;
                chuc_2 = (time_led - 34) / 10;
                dv_2 = (time_led - 34) % 10;
            }
            else if ((time_led <= 33) && (time_led > 30))
            {
                R1 = 1; Y1 = 0; G1 = 0;
                R2 = 0; Y2 = 1; G2 = 0;
                chuc_1 = (time_led - 31) / 10;
                dv_1 = (time_led - 31) % 10;
                chuc_2 = (time_led - 31) / 10;
                dv_2 = (time_led - 31) % 10;
            }
            else if ((time_led <= 30) && (time_led >= 3))
            {
                R1 = 0; Y1 = 0; G1 = 1;
                R2 = 1; Y2 = 0; G2 = 0;
                chuc_1 = (time_led - 3) / 10;
                dv_1 = (time_led - 3) % 10;
                chuc_2 = (time_led - 0) / 10;
                dv_2 = (time_led - 0) % 10;
            }
            else if ((time_led < 3) && (time_led >= 0))
            {
                R1 = 0; Y1 = 1; G1 = 0;
                R2 = 1; Y2 = 0; G2 = 0;
                chuc_1 = (time_led) / 10;
                dv_1 = (time_led) % 10;
                chuc_2 = (time_led) / 10;
                dv_2 = (time_led) % 10;
            }
        }
    }
}

/* Timer 0 interrupt service routine */
void T0_ISR(void) interrupt 1
{
    TR0 = 0; // Stop Timer 0
    TF0 = 0; // Clear overflow flag
    TL0 = -50000 % 256; // Reload Timer 0
    TH0 = -50000 / 256;

    num_time++;
    if (num_time == 20)
    {
        num_time = 0;
        time_led--;
        if (time_led < 0)
            time_led = 61; // Reset time_led
    }

    TR0 = 1; // Restart Timer 0
}

/* Timer 1 interrupt service routine */
void T1_ISR(void) interrupt 3 // Using Timer 1 to scan LEDs
{
    TR1 = 0; // Stop Timer 1
    TF1 = 0; // Clear overflow flag
    TH1 = -5000 / 256; // Reload Timer 1
    TL1 = -5000 % 256;
    index++;
    LED1 = 0; LED2 = 0; LED3 = 0; LED4 = 0;

    if (index == 1)
    {
        LED1 = 1; // Activate first LED
        LED_PORT = led7_data[chuc_1]; // Display first digit
    }
    else if (index == 2)
    {
        LED2 = 1; // Activate second LED
        LED_PORT = led7_data[dv_1]; // Display second digit
    }
    else if (index == 3)
    {
        LED3 = 1; // Activate third LED
        LED_PORT = led7_data[chuc_2]; // Display third digit
    }
    else if (index == 4)
    {
        LED4 = 1; // Activate fourth LED
        LED_PORT = led7_data[dv_2]; // Display fourth digit
        index = 0; // Reset index
    }

    TR1 = 1; // Restart Timer 1
}

/* External interrupt 1 service routine */
void IT1_ISR(void) interrupt 2
{
    num_count++;
}