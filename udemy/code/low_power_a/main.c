#include <msp430.h> 

#define STOP_WATCHDOG       0x5A80              // Stops the WDT
#define ACLK                0x0100              // TImer ACLK source
#define UP                  0x0010              // Timer UP mode
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs

main()
{
    WDTCTL = STOP_WATCHDOG;                     // Stop the WDT

    PM5CTL0 = ENABLE_PINS;                      // Required to use inputs and outputs
    P1DIR = BIT0;                               // P1.0 set as an output (P1.0 is the red LED)

    TA0CCR0 = 20000;                            // Sets a value for Timer0
    TA0CTL = ACLK + UP;                         // Sets ACLK in UP mode
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
    P1OUT = P1OUT ^ BIT0;                       // Toggle the red LED on P1.0
}
