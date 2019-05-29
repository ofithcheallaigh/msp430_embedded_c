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

    while(1)                                // Check for incoming message
    {
        if(UCA0IFG & UCRXIFG)               // Received any new messages?
        {
            if(UCA0IFG == 0x56)             // If the message is 0x56
            {
                P1OUT = BIT0;               // Turn the red LED on
            }
        }
    }
