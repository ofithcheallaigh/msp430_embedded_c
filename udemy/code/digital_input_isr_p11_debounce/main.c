/*
 * Debouncing program for pushbutton
 *
 * Delay added in to allow the switch to settle.
 * Decision has to be made on how long to wait (i.e. how long is the delay).
 * If the delay = 100000 for example, you can press the switch a few times
 * with nothing happening, while with a delay of 2000, you maye still get some
 * unusual behaviour.
 *
 * A delay of 20000 seems to be a good middle ground
 */

#include <msp430.h> 

#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   // Stops WDT

    PM5CTL0 = ENABLE_PINS;                      // Enables inputs and outputs

    P1DIR = BIT0;                               // P1.0 set as an output (P1.0 is the red LED)

    P1OUT = BIT1;                               // P1.1 will be an input with a pull up resistor.
    P1REN = BIT1;                               // Additionally, the red LED will be turned off.

    P1IE = BIT1;                                // Enable interrupts for P1.1
    P1IES = BIT1;                               // Interrupts transitions from HI to LO

    P1IFG = 0x00;                               // Ensures no ISRs are pending

    _BIS_SR(LPM0_bits | GIE);                               // Activate all interrupts

    while(1);
}

// ***********
// Port 1 ISR
// ***********
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    unsigned long delay;                        // Wait for debouncing to end
    for(delay = 0; delay < 20000; delay = delay + 1);

    P1OUT = P1OUT ^ BIT0;                       // Toggle LED after delay
    P1IFG &= ~(BIT1);                           // Clear P1.1 interrupt flag
}
