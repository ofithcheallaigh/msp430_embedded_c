#include <msp430.h> 

#define RED_LED             0x0001
#define GREEN_LED           0x0080
#define OUTPUT_DIRECTION    0x0001
#define STOP_WDT            0x5A80
#define ACLK                0x0100
#define UP                  0x0010
#define ENABLE_PINS         0xFFFE

main()
{
    WDTCTL = STOP_WDT;

    PM5CTL0 = ENABLE_PINS;          // Required for inputs and outputs
    P1DIR = RED_LED;                // Sets red LED as output
    P9DIR = GREEN_LED;              // Sets the green LED as output

    TA0CCR0 = 40000;                // Sets value for Timer_0
    TA0CTL = ACLK + UP;             // Set ACLK, UP mode for Timer0
    TA0CCTL0 = CCIE;                // Enable interrupts for Timer_0

    TA1CCR0 = 20000;                // Sets value for Timer_1
    TA1CTL = ACLK + UP;             // Set ACLK, UP mode for Timer1
    TA1CCTL0 = CCIE;                // Enable interrupts for Timer_1

    _BIS_SR(GIE);                   // Activate previously enabled interrupts

    while(1);                       // Wait here for interrupts
}

// *************************************************************************
// Timer0 ISR
// *************************************************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR (void)
{
    P1OUT = P1OUT ^ RED_LED;        // Toggles red LED
}

// ************************************************************************
// Timer1 ISR
// ***********************************************************************
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_ISR (void)
{
    P9OUT = P9OUT ^ GREEN_LED;      // Toggles red LED
}
