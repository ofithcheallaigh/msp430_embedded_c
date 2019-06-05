/*
 * Write a program that uses the UART to transmit a count-down at 9600 baud.
 * In the main program, set up the peripheral and enable the transmit interrupt.
 * Then transmit 0x0A (that is 10 decimal).
 *
 * In your ISR, continue the count-down by sending 0x09, 0x08, 0x07, 0x06, 0x05, 0x04
 * 0x03, 0x02, 0x01 and finally, 0x00. When we transmit 0x00, light the red LED.
 *
 * The program below shows this program.
 *
 * After setting the up the UART peripheral, the program transmits the first count (10 or 0x0A)
 * and then put the uC into an infinite loop.
 *
 * After the UART completes transmission of the stop bit, an interrupt is generated and the
 * program jumps to the ISR.
 *
 * The ISR begins by determining if the count has been decremented to zero yet. If it has,
 * it transmits the final 0x00 data byte and launches the rocket. If the count is still above
 * zero, the updated count-down is transmitted.
 *
 * The ISR ends by clearing the TX ComPleTe Interrupt FlaG, and the uC then returns to the
 * infinite loop in the main()
 *
 */

#include <msp430.h>

#define ENABLE_PINS             0xFFFE
#define UART_CLK_SEL            0x0080                          // Specifies accurate clock for UART peripheral
#define BR0_FOR_9600            0x34                            // Value required to use 9600 baud
#define BR1_FOR_9600            0x00                            // Value required to use 9600 baud
#define CLK_MOD                 0x4911                          // uC will "clean up" clock signal

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

    UCA0IE = UCTXCPTIE;                                         // Interrupt when TX stop bit complete
    _BIS_SR(GIE);                                               // Activate enabled UART TXD interrupt

    UCA0TXBUF = 10;                                             // Send the UART message 0x0A out on pin P4.2

    while(1);


}

// ********************
// UART Interrupt
// ********************
#pragma vector=USCI_A0_VECTOR
__interrupt void UART_ISR(void)
{
    static unsigned char countdown = 10;                        // Countdown state to transmit
    countdown = countdown - 1;                                  // Decrement countdown each time

    // What to do when the countdown is at certain points
    if(countdown == 0)                                          // If the countdown is over
    {
        P1OUT = BIT0;                                           // Turn on the red LED
        UCA0TXBUF = 0x00;                                       // Countdown is complete
        UCA0IE = UCA0IE & (~UCTXCPTIE);                         // Disable future UART interrupts
    }
    else                                                        // Countdown isn't over
    {
        UCA0TXBUF = countdown;                                  // Transmit the next count
    }
    UCA0IFG = UCA0IFG & (~UCTXCPTIFG);                          // Clear TX ComPleTe Interrupt FlaG
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
