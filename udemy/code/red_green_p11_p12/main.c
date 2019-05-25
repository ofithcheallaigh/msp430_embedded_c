/*
 * What do we do if we want to use two (or more) digital inputs as interrupts?
 *
 * If the digital inputs are on different ports (for example P1.1 and P2.3) everything
 * is fairly straightforward - just like 'digital_input_isr_p11_debounce'.
 *
 * However, if we want to use more than one digital input on the same port
 * (for example. the P1.1 and P1.2 push buttons on the launchpad), things can get a bit
 * trickier.
 *
 * To work with multiple interrupt sources on a single port, we need to use a particular
 * register - the Port 1 Interrupt Vector (P1IV) register.
 */


#include <msp430.h> 

#define ENABLE_PINS             0xFFFE

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                       // Stops the WDT
    PM5CTL0 = ENABLE_PINS;                          // Required for inputs and outputs

    // LEDs
    P1DIR = BIT0;                                   // P1.0 will be an output (red LED)
    P9DIR = BIT7;                                   // P9.7 will be an output (green LED)

    // Buttons
    P1OUT = BIT1 | BIT2;
    P1REN = BIT1 | BIT2;                            // Pull up resistors

    // Interrupts for pins
    P1IE = BIT1 | BIT2;                             // Interrupts enabled for P1.1 and P1.2
    P1IES = BIT1 | BIT2;                            // For transition from HI --> LO

    P1IFG = 0x00;                                   // Ensures no interrupts are pending

    _BIS_SR(GIE);                                   // Activate previously enabled interrupts

    while(1);
}

// ******************
// Port 1 ISR
// ******************
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    unsigned long delay;                            // required for de-bounce
    for(delay = 0; delay < 12345; delay = delay + 1);

    /*
     * After the delay, we need to determine which button was the source of the interrupt.
     * To do this, we will use the C instructions 'switch' and 'case'.
     *
     * The switch statement tells the CPU to look at the contents of the input (in this case
     * the P1IV register.
     *
     * Inside the switch statements are cases. These cases are executed conditionally based
     * on the contents of P1IV.
     *
     * Note, with the use of switch on the P1IV register, we no longer need to clear the digital
     * interrupt flag
     *
     * It might seem strange that P1IV has a value of 4 if P1.1 caused the interrupt, or a value
     * of 6 if P1.2 caused the interrupt
     *
     * However, this is the way the MSP430FR6989 was designed. Below, we list the different
     * values that can be stored in P1IV due to Port 1 interrupts:
     *
     *  - P1.0 generates interrupts, P1IV = 0x02
     *  - P1.1 generates interrupts, P1IV = 0x04
     *  - P1.2 generates interrupts, P1IV = 0x06
     *  - P1.3 generates interrupts, P1IV = 0x08
     *  - P1.4 generates interrupts, P1IV = 0x0A
     *  - P1.5 generates interrupts, P1IV = 0x0C
     *  - P1.6 generates interrupts, P1IV = 0x0E
     *  - P1.7 generates interrupts, P1IV = 0x10
     */
    switch(P1IV)                                    // What is stored in P1IV?
    {
        case 4:                                     // Come here if P1.1 interrupt
        {
            P1OUT = P1OUT ^ BIT0;                   // Toggle P1.0 for P1.1 push
            break;
        }

        case 6:                                     // Come here if P1.2 interrupt
        {
            P9OUT = P9OUT ^ BIT7;                   // Toggle  P9.7 for P1.2 push
            break;
        }
    }
}
