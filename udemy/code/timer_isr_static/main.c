#include <msp430.h>
#define RED_LED 0x0001                  // P1.0 is the red LED
#define STOP_WATCHDOG 0x5A80            // Stop the watchdog timer
#define ACLK 0x0100                     // Timer_A ACLK source
#define UP 0x0010                       // Timer_A UP mode
#define ENABLE_PINS 0xFFFE              // Required to use inputs and outputs
main()
{
    WDTCTL = STOP_WATCHDOG;             // Stop the watchdog timer
    PM5CTL0 = ENABLE_PINS;              // Required to use inputs and outputs
    P1DIR = RED_LED;                    // Set Red LED as an output
    TA0CCR0 = 20000;                    // Sets value of Timer_0
    TA0CTL = ACLK + UP;                 // Set ACLK, UP MODE
    TA0CCTL0 = CCIE;                    // Enable interrupt for Timer_0
    _BIS_SR(GIE);                       // Activate interrupts previously enabled
    while (1);                          // Wait here for interrupt
}
//************************************************************************
// Timer0 Interrupt Service Routine
//************************************************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR(void)
{
    unsigned char x = 0;         // Used to count number of elapses
    x = x + 1;                          // Increment the elapse count
    if (x == 15)                        // If count 15*20,000 = 300,000
    {
        P1OUT = P1OUT ^ RED_LED;        // Toggle red LED
        x = 0;                          // Reset master count
    }
}
