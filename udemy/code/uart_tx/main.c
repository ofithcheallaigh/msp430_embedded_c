/*
 * Universal Asynchronous Receiver Transmitter (UART) are one of the most common
 * serial communications interface used by the micro-controller to exchange information
 *
 * UARTs are Universal, meaning it is a standard used by everyone. Next, UART
 * peripherals are asynchronous in nature. This is, the peripheral does not have
 * a common clock signal to synchronise two uC that are communicating with each other.
 * To share information between two uC, we need two wires:
 *
 *             uC1                                  uC2
 *      ----------------                    --------------------
 *      |              |                    |                  |
 *      |              |                    |                  |
 *      |              |                    |                  |
 *      |              |                    |                  |
 *      |     TRANSMIT |------------------->| RECEIVE          |
 *      |              |                    |                  |
 *      |      RECEIVE |<-------------------| TRANSMIT         |
 *      |              |                    |                  |
 *      |              |                    |                  |
 *      ----------------                    --------------------
 *
 * Finally, the UART peripheral can used used for both transmitting and receiving information
 * to the outside world and receiving messages from the outside world.
 *
 * In these exercises, we will show you how to configure the MSP430FR6989 uC UART peripheral
 * to communicate with almost any other uC in the world. Since we are only using one uC here,
 * we will have it communicate with itself:
 *
 *      -----------------
 *      |               |
 *      |               |
 *      |               |
 *      |               |
 *      |       TRANSMIT|----
 *      |               |   |
 *      |               |   |
 *      |        RECEIVE|<---
 *      |               |
 *      |               |
 *      |---------------|
 */


#include <msp430.h> 

#define STOP_WATCHDOG           0x5A80
#define ENABLE_PINS             0xFFFE

/*
 * Below is the main() function we will use for out first UART program to transmit a byte of data
 *
 * The program begins by disabling the WDT and enabling the microcontroller pins
 */

main()
{
    WDTCTL = STOP_WATCHDOG;
    PM5CTL0 = ENABLE_PINS;

    /*
     * Next we have three functions whose names are self-explanatory
     */
    select_clock_signals();                 // Assign correct clock signals to UART
    assign_pins_to_uart();                  // P4.2 is for the TXD, P4.3 is for the RXD
    use_9600_baud();                        // UART operates at 9600 bits per second

    /*
     * Finally, we actually transmit the byte of data we want to send by storing it is a register
     * called the Universal serial Communication interface, type A, number 0, transmission (TX) BUFfer.
     * This is abbreviated to UCA0TXBUF, but this is often just called the 'transmission buffer' register
     * when developers are talking to one another.
     */
    UCA0TXBUF = 0x56;                       // This is our UART message to send out on P4.2

    while(1);
}

//**********************
// Select Clock Signals
//**********************
void select_clock_signals(void)
/*
 * This function tells the uC which of the different clock frequency signals it should
 * use to coordinate all the different parts of the uC.
 *
 * The first instruction in the function moves a special password code into the Clock
 * System ConTroL 0 register (CSCTL0). Without this PW, we cannot adjust the clock signals.
 *
 * In the next instruction, moving 0x0046 into Clock System ConTroL 1 register (CSCTL1) tells
 * the uC what frequency the Launchpad board was designed for.
 *
 * This is followed by moving 0x0133 into Clock System ConTroL 2 register (CSCTL2).
 * This specifies
 */
{

}
