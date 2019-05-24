/*
 * This is the first challenge for the section on timers
 *
 * We need to turn on the LED for 20 seconds, then turn it off, and keep it off
 *
 * ACLK sounds for 25us, and can be a max of 65535
 *      25us * 40000 = 1 second
 *      So need to count to 40000 twenty times
 */

#include <msp430.h>

#define RED_LED             0x0001              // P1.0 is the red LED
#define PIN_DIR_OUT         0x0001              // Sets the port direction
#define DEVELOPMENT         0x5A80              // Turns off the WDT
#define ENABLE_PINS         0xFFFE              // Required for inputs and outputs
#define ACLK                0x0100              // Timer A ACLK source
#define UP                  0x0010              // Timer A UP mode
#define TAIFG               0x0001              // Used to look at the Timer A Interrput FlaG
#define LED_OFF             0x0000

main()
{
    unsigned char intervals = 0;                // Used to count the number of 40000's (i.e. 20)

    WDTCTL = DEVELOPMENT;
    PM5CTL0 = ENABLE_PINS;                      // Required for inputs and outputs

    TA0CCR0 = 40000;                            // The number we want to count to
    TA0CTL = ACLK | UP;                         // Use ACLK in UP mode

    P1DIR = PIN_DIR_OUT;                        // Sets P1 as output
    P1OUT = PIN_DIR_OUT;                        // Turns on the red LED

    while(1)
    {
        // Check to see if the counter has reached 40,000 - TAIFG register changes from 0 -> 1
        if(TA0CTL & TAIFG)
        {
            intervals = intervals + 1;          // Increment the intervals count
            TA0CTL = TA0CTL & (~TAIFG);         // Restart counter

            if(intervals == 20)                 // We need 20 times 40000 to get us to 20 seconds
            {
                // intervals = 0;                  // Reset interval counter
                P1OUT = LED_OFF;                // Turns the LED off
                while(2);                       // Stays here forever
            }
        }
    }
}
