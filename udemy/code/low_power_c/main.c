#include <msp430.h> 

#define STOP_WATCHDOG           0x5A80              // Stop WDT
#define ACLK                    0x0100              // Timer ACLK source
#define UP                      0x0010              // Timer UP mode
#define ENABLE_PINS             0xFFFE              // Required for inputs and outputs
#define SLOW                    0x00C0              // Slows down ACLK by a factor of 8

main()
{
    WDTCTL = STOP_WATCHDOG;                         // Stops the WDT

    PM5CTL0 = ENABLE_PINS;                          // Required for inputs and outputs

    P1DIR = BIT0;                                   // P1.0 set up as an output (red LED)
    P1OUT = 0x00;                                   // Turns P1.0 (red LED) off

    P9DIR = BIT7;                                   // P9.7 set up as an output (green LED)
    P9OUT = 0x00;                                   // P9.7 off

    TA0CCR0 = 50000;                                //
    TA0CTL = ACLK | UP | SLOW;                      // Sets up ACLK as source, and using UP mode
    TA0CCTL0 = CCIE;                                // Enable interrupts

    _BIS_SR(LPM0_bits | GIE);                       // Enter LPM0 and activate interrupts

    P9OUT = BIT7;                                   // Set P9.7 high (turn green LED on). This will happen
                                                    // if the uC executes any additional instructions in main

    while(1);
}


// *************
// Timer0 ISR
// *************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR (void)
{
    _BIS_SR(LPM0_EXIT);
    P1OUT = P1OUT ^ BIT0;                           // Toggle red LED
}
