



#include <msp430.h>

#define RED_LED             0X0001              // P1.0 is the red LED
#define GREEN_LED           0x0080              // P9.7 is the green LED
#define DEVELOPMENT         0x5A80
#define ENABLE_PINS         0xFFFE
#define ACLK                0x0100
#define UP                  0x0010
#define TAIFG               0x0001
#define PET_WDT             0x5A08              // WDT password and pet

main()
{
    unsigned char t0_count = 0;
    unsigned char t1_count = 0;

    PM5CTL0 = ENABLE_PINS;                      // Enable inputs and outputs

    TA0CCR0 = 400;                              // Counts from 0 to 400 (25us * 400 = ~10ms)
    TA0CTL = ACLK | UP;                           // Sets ACLK in UP mode
    TA1CCR0 = 40000;                            // Count up from 0 to 40000 (~1s)
    TA1CTL = ACLK | UP;                         // Sets ACLK in UP mode

    P1DIR = RED_LED;                            // Sets red LED as output
    P9DIR = GREEN_LED;                          // Sets green LED as output

    while(1)
    {
        if(TA0CTL & TAIFG)                      // If timer 0 has counted ~10ms
        {
            WDTCTL = PET_WDT;                   // Pet the WDT
            TA0CTL = TA0CTL & (~TAIFG);         // Restart count
            t0_count = t0_count + 1;            // Increment 10ms counts

            if(t0_count == 10)                  // If ~100ms has passed
            {
                t0_count = 0;                   // Reset 10ms counter
                P1OUT = P1OUT ^ RED_LED;        // Toggle red LED
            }
        } // End Timer0 if

        if(TA1CTL & TAIFG)                      // If timer 1 has counted to 5000
        {
            TA1CTL = TA1CTL & (~TAIFG);         // Restart counter
            t1_count = t1_count + 1;            // Increment 1s counts

            if(t1_count == 3)                   // if ~3s has passed
            {
                t1_count = 0;                   // Reset 1s counter
                P9OUT = P9OUT ^ GREEN_LED;      // Toggle green LED
            }
        } // End timer1 if
    } // End while(1)
} // End main
