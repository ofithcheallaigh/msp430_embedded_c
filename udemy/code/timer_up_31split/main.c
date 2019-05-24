/*
 * This program will turn on the red LED for 3 seconds, and off for 1 second before repeating.
 *
 * What needs to be done:
 *      1. set up includes/defines
 *      2. Turn off WDT
 *      3. Enable pins
 *      4. put value into TA0CCR0 for 1 second
 *      5. set up ACLK in up mode
 *      6. set up P1DIR (output)
 *      7. infinity while loop
 *          a. need to check if TAIFG set
 *          b. turn one LED for 3 seconds
 *          c. off for 1 second
 *
 *  |------------------------------------------------|
 *  | TA0CTL                                         |
 *  ||-----------------------------------------------|
 *  ||               NOT USED            |  TASSEL   |
 *  ||-----------------------------------------------|
 *  ||     |     |     |     |     |     |  0  |  1  |
 *  ||-----------------------------------------------|
 *  ||-----------------------------------------------|
 *  || NOT USED  |    MC     |    NOT USED     |TAIFG|
 *  ||-----------------------------------------------|
 *  ||     |     |  0  |  1  |     |     |     |     |
 *  ||-----------------------------------------------|
 */

#include <msp430.h>

#define RED_LED_ON          0x0001
#define RED_LED_OFF         0x0000
#define P1_DIR_OUT          0x0001
#define DEVELOPMENT         0x5A80
#define ENABLE_PINS         0xFFFE
#define ACLK                0x0100
#define UP                  0x0010
#define TAIFG               0x0001

main()
{
    // Turn off WDT and enable pins
    WDTCTL = DEVELOPMENT;           // Turns off the WDT for development
    PM5CTL0 = ENABLE_PINS;          // Required for inputs and outputs

    // Put value into TA0CCR0 for 1 second.
    // ACLK counts in 25us, so 25us * 40000 = 1 second
    TA0CCR0 = 40000;

    // Set up ACLK in UP mode. Reference register diagram above
    // Takes the logic OR of 0x0100 and 0x0010, storing the result in TA0CTL
    // Therefore, TA0CTL = 0x0110
    TA0CTL = ACLK | UP;

    // Set up P1DIR as output
    P1DIR = P1_DIR_OUT;
    P1OUT = RED_LED_ON;

    unsigned char interval = 0;
    while(1)
    {

        // Check to see if the counter as reached 40000: TAIFG register changes from 0 -> 1
        if(TA0CTL & TAIFG)
        {
            interval = interval + 1;                // Increment
            TA0CTL = TA0CTL & (~TAIFG);             // Start counting again
            if(P1OUT & RED_LED_ON)                  // If the red LED is on
            {
                if(interval == 3)                   // Have we been on for 3 seconds?
                {
                    P1OUT = RED_LED_OFF;            // If yes, turn the LED off
                    interval = 0;                   // Reset interval
                }
            }
            else                                    // If we are off
            {
                P1OUT = RED_LED_ON;                 // Turn the LED back on
                interval = 0;
            }
        }
    }
}
