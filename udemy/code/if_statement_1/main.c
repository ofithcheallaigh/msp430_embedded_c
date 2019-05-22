#include <msp430.h> 

#define LED_ON              0x01                        // Used to turn on/enable P1.0 LED
#define LED_OFF             0xFE                        // Used to turn off the P1.0 LED
#define BUTTON11            0x02                        // P1.1 is the button
#define DEVELOPMENT         0x5A80                      // Needed for the WDT
#define ENABLE_IO            0xFFFE                      // Needed to enable I/O

main()
{
    WDTCTL = DEVELOPMENT;

    PM5CTL0 = ENABLE_IO;

    P1DIR = LED_ON;
    P1OUT = BUTTON11;                                   // P1.1 will be an input
    P1REN = BUTTON11;                                   // with a pull-up resistor

    while(1)
    {
        if((BUTTON11 & P1IN) == 0)                      // If P1.1 button pushed
        {
            P1OUT = P1OUT | LED_ON;                     // Turn on the red LED
        }

        else                                            // Else, if P1.1 is not pushed
        {
            P1OUT = P1OUT & LED_OFF;
        }
    }
}
