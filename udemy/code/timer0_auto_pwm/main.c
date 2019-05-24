/*
 * The program below uses Timer0 in fully automatic mode to generate a pulse width
 * modulated output.
 *
 */

#include <msp430.h> 

#define ENABLE_PINS          0xFFFE
#define ACLK                0x0100              // TimerA ACLK source
#define UP                  0x0010              // TimerA UP mode

main()
{
    WDTCTL = WDTPW | WDTHOLD;                   // Stops WDT

    PM5CTL0 = ENABLE_PINS;                      // Enables inputs and outputs

    P1DIR = BIT0;                               // P1.0 will be an output for the red LED

    /*
     *  Below is a new instruction. It gives Timer0 control over to P1.0. As long as bit 0
     *  is set in the P1SEL0 register, the CPU cannot turn on or off the P1.0 output with
     *  the P1OUT register. Only Timer0 peripheral can now turn on or off the P1.0 output
     */
    P1SEL0 = BIT0;

    /*
     * This next instruction places Timer0 into the automatic pulse width modulation mode.
     * For the MSP430FR6989 Timer0, this is called OUTput MODe 3. In mode 3, Timer0 will
     * automatically generate a pulse width modulated output without the need for an ISR.
     *
     * Before Timer0 starts counting, the timer will ensure the assigned output (P1.0) is
     * LO. The timer will start counting up from 0. When the timer reaches the value we load
     * into TA0CCR0, Timer0 will automatically make P1.0 output HI.
     */
    TA0CCTL1 = OUTMOD_3;                        // Uses Timer mode 3
                                                // Timer starts at 0, P1.0 starts LO
                                                // When timer reaches TA0CCR1, P1.0 goes HI
                                                // When timer reaches TA0CCR0, P1.0 goes LO
                                                // Count starts over at 0 with P1.0 LO

    /*
     * Next we load the values into TA0CCR1 and TA0CCR0.
     * With the values we select, P1.0 will be LO as the timer counts from 0 to 45000.
     * In mode 3, Timer0 will then automatically make the P1.0 output HI. P1.0 will then remain
     * HI as Timer0 counts from 45000 to 50000.
     * When Timer0 reaches 50000, Timer0 will automatically make the P1.0 output go LO and the
     * counting will begin again at 0
     */
    TA0CCR1 = 45000;                            // P1.0 LO from 0 to 45000
    TA0CCR0 = 50000;                            // P1.0 HI from 45000 to 50000

    /*
     * Finally, the program starts the Timer0 counting by placing it in UP mode.
     * At this point, the counter is working as explain in the previous steps
     */
    TA0CTL = ACLK | UP;                         // Count up to TA0CCR0 at 25us/count

    /*
     * Next we enter an infinite while loop.
     * The program will continue to stay in this loop until it is stopped by the CCS debugger.
     * Timer0 will automatically count up to TA0CCR1, set P1.0 HI, count to TA0CCR0, set
     * P1.0 LO, and then begin to count over again at 0
     */
    while(1);                                   // Program stays here and never leaves
                                                // Timer0 handles everything without needing
                                                // an ISR
}
