#include <msp430.h> 

#define STOP_WATCHDOG_TIMER             0x5A80
#define ACLK                            0x0100
#define UP                              0x0010
#define ENABLE_PINS                     0xFFFE

main()
{
    // Stop WDT
    WDTCTL = STOP_WATCHDOG_TIMER;

    // Enable Pins
    PM5CTL0 = ENABLE_PINS;

    // Set P1.0 to be an output, with LED off
    P1DIR = BIT0;
    P1OUT = 0x00;

    // Set P1.1 to be an input pushbutton switch (enable pull up)
    P1OUT = BIT1;
    P1REN = BIT1;

    // Set up timer for interrupt every 50ms (0.05s) - 50ms/25us = 2000
    TA0CCR0 = 2000;
    TA0CTL = ACLK | UP;                         // ACLK source in UP mode
    TA0CCTL0 = CCIE;                            // Interrupts enabled

    _BIS_SR(LPM0_bits | GIE);                   // Activate enabled interrupts

    while(1);
}


// ***********************
// Timer0 ISR
// ***********************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR (void)
{
    // _BIS_SR(LPM0_EXIT);
    // When here, check to see if P1.1 is pushed. If not, red LED is off & go back to main
    // If pushed, turn on red LED and end IRS and go back to main.
    // Repeat
    if(BIT1 & P1IN)                             // If button is NOT pushed
    {
        P1OUT = P1OUT & ~BIT0;                  //  BIT0 = 0000 0001 B
                                                // ~BIT0 = 1111 1110 B
                                                //  Bit-wise AND will clear P1OUT.0 and
                                                //  not change pull-up resistor
    }
    else                                        // If P1.1 is pushed
    {
        P1OUT = P1OUT | BIT0;                   // Turn on red LED
    }
}
