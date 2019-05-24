/*
 * This program is similar to push_button, except it turns on the green LED (lower right corner) when push button P1.2
 * (also lower right corner) is pressed.
 *
 * Note, the green LED is connected to pin 7 of port 9
 *
 */

#include <msp430.h>

#define GREEN_ON            0x80                    // P9.7 is the green LED - 0x80 = 1000 0000 in binary
#define GREEN_OFF           0x7F                    // Used to turn off the green LED - 0x7F = 0111 1111 in binary
#define BUTTON12            0x04                    // P1.2 us the lower right push-button - 0x04 = 0000 0100
#define DEVELOPMENT         0x5A80                  // Stops WDT - 0x5A80 = 0101 1010 1000 0000 in binary
#define ENABLE_PINS         0xFFFE                  // Required to use inputs and outputs - FFFE = 1111 1111 1111 1110 in binary

main()
{
    WDTCTL = DEVELOPMENT;

    PM5CTL0 = ENABLE_PINS;                          // Prepare pins for I/O usage

    P9DIR = GREEN_ON;                               // Green LED connected to P9.7 as output
    P1OUT = BUTTON12;                               // Button needs pull-up resistor
    P1REN = BUTTON12;

    while(1)
    {
        while((BUTTON12 & P1IN) == 0)               // Is button pressed? If 'something' is zero, condition is true
        {
            P9OUT = P9OUT | GREEN_ON;               // Turn on the green LED
        }

        P9OUT = P9OUT & GREEN_OFF;                  // Turns off green LED
    }
}

