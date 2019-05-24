/*
 * This program demonstrates a manual setting of pulse width modulation
 * PWM is the practice of turning on and off an output at various duty cycles.
 * In this case:
 *  Time LED On     = 0.125 seconds
 *  Time LED Off    = 1.125 seconds
 *
 *  Graphically, the output will look as follows:
 *
 *      5000 Counts   On 10%
 *          --          --          --              --> LED ON
 *          ||          ||          ||
 *          ||          ||          ||
 *  45000   ||    Off   ||          ||
 *  Counts  ||    90%   ||          ||
 * ---------  ----------  ----------  -----------   --> LED OFF
 *
 * Duty cycle = Time LED On/ (Time LED On + Time LED Off)
 *
 * For On time of 0.4 seconds, and off time of 0.1 seconds:
 * Duty Cycle = 0.4seonds / (0.4 seconds + 0.1 seconds)
 * Duty Cycle = 0.4 seconds / (0.5 seconds)
 * Duty Cycle = 80%
 */


#include <msp430.h> 

#define STOP_WATCHDOG               0x5A80
#define ACLK                        0x0100      // ACLK timer source
#define UP                          0x0010      // ACLK, UP mode
#define ENABLE_PINS                 0xFFFE      // Required to use inputs and outputs

main()
{
    WDTCTL = STOP_WATCHDOG;

    PM5CTL0 = ENABLE_PINS;                      // Required to use inputs and outputs

    P1DIR = BIT0;                               // Sets P1.0 as output - P1.0 is the red LED
    P1OUT = 0x00;                               // Switches the red LED off

    TA0CCR0 = 4000;                            // (9000 * 25us) = 0.1seconds
    TA0CTL = ACLK | UP;                         // Sets ACLK in UP mode
    TA0CCTL0 = CCIE;                            // Enables interrupts for timer 0

    _BIS_SR(GIE);                               // Activates previously enabled interrupts

    while(1)
    {
        ;                                       // Wait here for interrupts
    }
}


// ********************************************************************************
// Timer0 Interrupt Service Routine
// ********************************************************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR (void)
{
    if(TA0CCR0 == 4000)                        // If counted to 45000
    {
        P1OUT = BIT0;                           // Turns on red LED
        TA0CCR0 = 16000;                        // Count to 16000 next time ((16000 * 25us) = 0.4seconds)
    }
    else
    {
        P1OUT = 0x00;                           // Turns red LED off
        TA0CCR0 = 4000;                        // Count to 4000
    }
}
