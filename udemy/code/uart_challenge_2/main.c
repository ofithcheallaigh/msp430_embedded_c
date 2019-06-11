#include <msp430.h>

#define ENABLE_PINS             0xFFFE
#define UART_CLK_SEL            0x0080                          // Specifies accurate clock for UART peripheral
#define BR0_FOR_9600            0x34                            // Value required to use 9600 baud
#define BR1_FOR_9600            0x00                            // Value required to use 9600 baud
#define CLK_MOD                 0x4911                          // uC will "clean up" clock signal
#define ONE_SECOND              40000                           // 40000 *25us = 1 sec
#define ACLK                    0x0100                          // Timer_A ACLK source
#define UP                      0x0010                          // Timer_A UP mode
// #define TAIFG                   0x0001                          // Used to look at the Timer A Interrupt FlaG

// ********************
// Function Definitions
// ********************
void select_clock_signals(void);                                // Assigns uC clock signals
void assign_pins_to_uart(void);                                 // P4.2 is for TXD, P4.3 is for RXD
void use_9600_baud(void);                                       // UART operates at 9600 bits per second

//*********************
// Main function
//*********************
main()
{
    WDTCTL = WDTPW | WDTHOLD;                                   // Stops WDT
    PM5CTL0 = ENABLE_PINS;                                      // Enable pins

    P1DIR = BIT0;                                               // Males P1.0 an output for red LED
    P1OUT = 0x00;                                               // Red LED initially off

    // Function calls here
    select_clock_signals();                                     // Assigns uC clock signals
    assign_pins_to_uart();                                      // P4.2 is for TXD, P4.3 is for RXD
    use_9600_baud();                                            // UART operates at 9600 bits per second

    TA0CCR0 = ONE_SECOND;                                       // Defined above
    TA0CTL = ACLK | UP;                                         // ACLK source in UP mode
    TA0CCTL0 = CCIE;                                            // Enable interrupts for Timer0
    _BIS_SR(GIE);                                               // Activate enabled UART TXD interrupt

    UCA0TXBUF = 10;                                             // Send the UART message 0x0A out on pin P4.2

    while(1);


}

// ********************
// UART Interrupt
// ********************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR(void)
{
    static unsigned char countdown = 9;                         // Countdown state to transmit
    countdown = countdown - 1;                                  // Decrement countdown each time

    // What to do when the countdown is at certain points
    if(countdown == 0)                                          // If the countdown is over
    {
        UCA0TXBUF = countdown;                                  // Transmit "zero"
        P1OUT = BIT0;                                           // Turn on the red LED
        TA0CCTL0 = TA0CCTL0 & (~CCIE);                          // Disable future timer interrupts
    }
    else                                                        // Countdown isn't over
    {
        UCA0TXBUF = countdown;                                  // Transmit the next count
        countdown = countdown - 1;
    }
}

// *********************
// Functions
// *********************
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
