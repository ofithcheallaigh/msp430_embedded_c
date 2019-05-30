#include <msp430.h> 

#define ENABLE_PINS             0xFFFE
#define UART_CLK_SEL            0x0080                  // Specifies accurate SMCLK clock for UART
#define BR0_FOR_9600            0x34                    // Value required to use 9600 baud
#define BR1_FOR_9600            0x00                    // Value required to use 9600 baud
#define CLK_MOD                 0x4911                  // uC will "clean-up" clock signal

// Function prototypes
void select_clock_signals(void);
void assign_pins_to_uart(void);
void use_9600_baud(void);

main()
{
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 = ENABLE_PINS;

    P1DIR = BIT0;
    P1OUT = 0x00;

    select_clock_signals();                 // Assign correct clock signals to UART
    assign_pins_to_uart();                  // P4.2 is for the TXD, P4.3 is for the RXD
    use_9600_baud();                        // UART operates at 9600 bits per second

    /*
     * Now we will look at using an ISR with the UART so the program so the program does not have to
     * keep polling, or checking, to see if the UCRXIFG has gone HI
     *
     * To do this, we need to add two lines, shown below. First we need to enable the UART
     * receive interrupt. Second, we activate the enabled interrupt
     *
     * After we send the message, we immediately put the program into an infinite loop to
     * wait for the interrupt service routine
     */
    UCA0IE = UCRXIE;                        // Enable UART RXD interrupt
    _BIS_SR(GIE);                           // Activate UART RXD interrupt

    UCA0TXBUF = 0x56;                       // This is our UART message to send out on P4.2

    while(1);                               // Check for incoming message

} // End main()

/*
 * ISR
 */
#pragma vector=USCI_A0_VECTOR
__interrupt void UART_ISR(void)
{
    if(UCA0IFG & UCRXIFG)               // Received any new messages?
    {
        if(UCA0RXBUF == 0x56)             // If the message is 0x56
        {
            P1OUT = BIT0;               // Turn the red LED on
        }
        else                            // Else, if the message is not 0x56
        {
            P1OUT = 0x00;               // Turn off the red LED
        }
        UCA0IFG = UCA0IFG & (~UCRXIFG); // Reset the UART receive flag
    }
}

void select_clock_signals(void)
{
    CSCTL0 = 0xA500;                    // "Password" to access clock calibration registers
    CSCTL1 = 0x0046;                    // Specifies frequency of the main clock
    CSCTL2 = 0x0133;                    // Assigns additional clock signals
    CSCTL3 = 0x0000;                    // Use clocks at intended freq, do not slow them down
}


void assign_pins_to_uart(void)
{
    P4SEL1 = 0x00;                      // 0000 0000
    P4SEL0 = BIT3 | BIT2;               // 0000 1100
                                        //      ^^
                                        //      ||
                                        //      |+----- 01 assigns P4.2 to UART Transmit (TXD)
                                        //      |
                                        //      +------ 01 assigns P4.3 to UART Receive (RXD)
}


void use_9600_baud(void)
{
    UCA0CTLW0 = UCSWRST;                            // Puts UART into SoftWare ReSeT
    UCA0CTLW0 = UCA0CTLW0 | UART_CLK_SEL;           // Specifies clock source for UART
    UCA0BR0 = BR0_FOR_9600;                         // Specifies bit rate of 9600
    UCA0BR1 = BR1_FOR_9600;                         // Specifies bit rate of 9600
    UCA0MCTLW = CLK_MOD;                            // "Cleans" clock signal
    UCA0CTLW0 = UCA0CTLW0 & (~UCSWRST);             // Takes UART out of SoftWare ReSeT
}
