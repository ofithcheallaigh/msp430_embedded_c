/*
 * This program will have the green LED on when the button isn't pressed.
 * When the button is pressed, the green LED will go out, and the red LED will
 * come on.
 */

#include <msp430.h> 

#define LED_ON              0x01                        // Used to turn on/enable P1.0 LED
#define LED_OFF             0xFE                        // Used to turn off the P1.0 LED
#define BUTTON11            0x02                        // P1.1 is the button
#define DEVELOPMENT         0x5A80                      // Needed for the WDT
#define ENABLE_IO           0xFFFE                      // Needed to enable I/O
#define GREEN_ON            0x80                        // P9.7 is the green LED - 0x80 = 1000 0000 in binary
#define GREEN_OFF           0x7F                    // Used to turn off the green LED - 0x7F = 0111 1111 in binary

main()
{
    WDTCTL = DEVELOPMENT;

    PM5CTL0 = ENABLE_IO;

    P1DIR = LED_ON;
    P1OUT = BUTTON11;                                   // P1.1 will be an input
    P1REN = BUTTON11;                                   // with a pull-up resistor

    P9DIR = 0X80;                                       // Green LED connected to P9.7 as output, 0x80 = 1000 0000 in binary

    while(1)
    {
        if((BUTTON11 & P1IN) == 0)                      // If P1.1 button pushed
        {
            P1OUT = P1OUT | LED_ON;                     // Turn on the red LED
            P9OUT = P9OUT & GREEN_OFF;                  // Turns off green LED
        }

        else                                            // Else, if P1.1 is not pushed
        {
            P1OUT = P1OUT & LED_OFF;                    // Turns off red LED
            P9OUT = P9OUT | GREEN_ON;                   // Green LED comes on when button is not pressed
        }
    }
}
