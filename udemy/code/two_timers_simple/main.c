/*
 * Here, we will look at using the two general purpose timers simultaneously.
 *
 * Timer_A0 will count from 0 to 33,000. This will take approximately:
 *
 *      (33,000)*(25us) = 0.825 seconds
 *
 * Timer_A1 will count from 0 to 5000. This will take approximately:
 *
 *      (5000)*(25us) = 0.125 seconds
 *
 */

#include <msp430.h>

#define RED_LED             0X0001              // P1.0 is the red LED
#define GREEN_LED           0x0080              // P9.7 is the green LED
#define DEVELOPMENT         0x5A80
#define ENABLE_PINS         0xFFFE
#define ACLK                0x0100
#define UP                  0x0010
#define TAIFG               0x0001

main()
{
    WDTCTL = DEVELOPMENT;
    PM5CTL0 = ENABLE_PINS;

    TA0CCR0 = 33000;
    TA0CTL = ACLK | UP;

    TA1CCR0 = 5000;
    TA1CTL = ACLK | UP;

    P1DIR = RED_LED;
    P9DIR = GREEN_LED;

    while(1)
    {
        if(TA0CTL & TAIFG)
        {
            P1OUT = P1OUT ^ RED_LED;
            TA0CTL = TA0CTL & (~TAIFG);
        }

        if(TA1CTL & TAIFG)
        {
            P9OUT = P9OUT ^ GREEN_LED;
            TA1CTL = TA1CTL & (~TAIFG);
        }
    }

}

