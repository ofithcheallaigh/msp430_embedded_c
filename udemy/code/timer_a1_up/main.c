/*
 * Our uC actually has multiple general purpose timers that we can use in our programs. The default timer that we have used until
 * now was Timer_A0. The second timer we can use is Timer_A1.
 *
 * The default Timer_A0 and alternative Timer_A1 are almost identical, so we already know almost everything necessary to use
 * both timers in our programs. The only difference between the two are their register names.
 *
 * As an example
 *
 *      Timer_A0            Timer_A1
 *      ----------------------------
 *      TA0CCR0             TA1CCR0
 *      TA0CTL              TA1CTL
 *
 * Note: it is not TA1CCR1, this is a common mistake
 *
 * When looking at Timer_A1, the #define terms (ACLK, UP, TAIFG) are not changes. This is because the ACLK, UP and TAIFG bits
 * are located in the same position in both the TA0CTL and TA1CTL registers.
 */

#include <msp430.h>

#define RED_LED_ON          0x0001              // P1.0 is the red LED
#define DEVELOPMENT         0x5A80              // Stops the WDT
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define ACLK                0x0100              // Timer_A ACLK source
#define UP                  0x0010              // Timer_A UP mode
#define TAIFG               0x0001              // Used to look at Timer A Interrupt FlaG

main()
{
    WDTCTL = DEVELOPMENT;
    PM5CTL0 = ENABLE_PINS;

    TA1CCR0 = 5000;
    TA1CTL = ACLK | UP;

    P1DIR = RED_LED_ON;

    while(1)
    {
        if(TA1CTL & TAIFG)
        {
            P1OUT = P1OUT ^ RED_LED_ON;
            TA1CTL = TA1CTL & (~TAIFG);
        }
    }
}
