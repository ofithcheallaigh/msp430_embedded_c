#include <msp430.h> 

#define RED_LED             0x0001              // P1.0 is the red LED
#define SET_OUTPUT          0x0001              // Used to set port direction
#define STOP_WDT            0x5A80
#define ACLK                0x0100              // Timer ACLK source
#define UP                  0x0010              // Timer UP mode
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs

main()
{
    WDTCTL = STOP_WDT;

    PM5CTL0 = ENABLE_PINS;                      // Required to use inputs and outputs
    P1DIR = SET_OUTPUT;                         // Sets the red LED as an output

    TA0CCR0 = 20000;                            // Sets the value for Timer_0
    TA0CTL = ACLK + UP;                         //Set ACLK, UP mode
    TA0CCTL0 = CCIE;                            // Enable interrupt for Timer_0

    _BIS_SR(GIE);                               // Activate interrupts previously enabled

    while(1);                                   // Wait here for interrupt
}

// ****************************************************************************************
// Timer0 ISR
// ****************************************************************************************
/*
 * The ISR must be put into a special place in the uC program memory. That's what this line (below) does.
 * While you do not need this comment, the code line itself must always appear exactly like this in
 * the program
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_ISR (void)              // This officially names the ISR as "Timer0_ISR"
{
    // Like other functions, everything happens inside the curly braces
    P1OUT = P1OUT ^ RED_LED;                    // Toggles the red LED
                                                // When the code here is done, the ISR ends and the
                                                // program jumps back to whatever it was doing before
}
