/*
 * This programs uses SLOW to slow the ACLK down by a factor of 8
 * TA0CCR0 has been changed from 1000 to 125 to accomadate this
 */

#include <msp430.h> 

#define STOP_WATCHDOG       0x5A80              // Stops the WDT
#define ACLK                0x0100              // TImer ACLK source
#define UP                  0x0010              // Timer UP mode
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define SLOW                0x00C0              // Slows down ACLK by factor of 8

main()
{
    WDTCTL = STOP_WATCHDOG;                     // Stop the WDT

    PM5CTL0 = ENABLE_PINS;                      // Required to use inputs and outputs
    P1DIR = BIT0;                               // P1.0 set as an output (P1.0 is the red LED)
    P1OUT = 0x00;                               // Makes sure red LED is off to start

    TA0CCR0 = 40000;                            // Sets a value for Timer0 - 40k * 25us ~ 1 sec
    TA0CTL = ACLK | UP | SLOW;                  // Sets ACLK in UP mode
    TA0CCTL0 = CCIE;                            // Enables interrupts for Timer0

    _BIS_SR(LPM0_bits | GIE);                   // Activate interrupts previously enabled

    while(1);                                   // Wait here for interrupt
}


// *********************
// Timer0 ISR
// *********************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR (void)
{
    if(TA0CCR0 == 40000)
    {
        TA0CCR0 = 125;                          // Turn the red LED on for a short bit of time
        P1OUT = BIT0;
    }
    else
    {
        TA0CCR0 = 40000;                        // Turn the red LED on for longer
        P1OUT = 0x00;
    }
}
