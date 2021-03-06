/*
 * After we have loaded 0x56 into the UCA0TXBUF register, the program begins trying to receive
 * a UART message.
 *
 * When a UART message has been successfully received, the UART peripheral will set the
 * Universal Communication receive (RX) Interrupt FlaG (UCRXIFG) HI in the Universal
 * Communication interface, type A, number 0, Interrupt FlaG register (UCA0IFG). Therefore,
 * the program will continuously poll the UXRXIFG bit in the UCA0IFG register using
 * an if-statement
 *
 * With the long register names, this relatively simple operation can seem more complex.
 * Here it is with just the register names
 *      When the UART message has been successfully received, the UART peripheral
 *      will set the UCRXIFG bit HI in the UCA0IFG register. Therefore, the program
 *      will continuously poll the UXRXIFG bit in the UCA0IFG register using an
 *      if-statement
 */


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

    UCA0TXBUF = 0x56;                       // This is our UART message to send out on P4.2

    /*
     * The code below shows what the program will do when a UART message is received (by having
     * the UCRXIFG bit in the UCA0IFG register).
     *
     * When a UART byte is successfully received, the peripheral automatically stores the received
     * byte in the UCA0RXBUF register. The program checks to see what the message is. If the message is
     * 0x56, the program will turn on the red LED. If the message is anything else, the program
     * turns off the red LED.
     *
     * Finally, the program clears the UCEXIFG bit in the UCA0IFG register so the program can begin looking
     * for another UART message.
     */
    while(1)                                // Check for incoming message
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
    } // End while(1)
} // End main()

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
