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
 * This specifies the different clock signals in the uC that the various peripherals have
 * access to.
 *
 * Finally, by clearing the Clock System ConTroL 3 register (CSCTL3), we ensure that
 * the clock signals we have specified are all running at their expected frequency. (optionally,
 * developers have the option of slowing the clock down. While slowing things down can reduce power
 * consumption, it can be troublesome for coordinating communications between devices.)
 *
 * NB: To understand more where the numbers below come from, please see Note 1 below the program
 *
 */

{
    CSCTL0 = 0xA500;                    // "Password" to access clock calibration registers
    CSCTL1 = 0x0046;                    // Specifies frequency of the main clock
    CSCTL2 = 0x0133;                    // Assigns additional clock signals
    CSCTL3 = 0x0000;                    // Use clocks at intended freq, do not slow them down
}


/*
 * Note 1
 * ------
 * First we have CSCTL0:
 * The information below is taken from the Family User's Guide
 *
 * CSCTL0 Register
 * |=======================================================
 * |   15  |  14  |  13  |  12  |  11 |  10  |  9  |  8   |
 * |-------------------------- CSKEY ---------------------|
 * |  rw-1 | rw-0 | rw-0 | rw-1 | rw-0| rw-1 | rw-1| rw-1 |
 * |======================================================|
 * |    7  |   6  |  5   |   4  |  3  |  2   |  1  |  0   |
 * |------------------------- RESERVED -------------------|
 * |    r0 |   r0 |  r0  |  r0  | r0  |  r0  |  r0 |  r0  |
 * |======================================================|
 *
 * CSCTL0 Register Description:
 * Bit      Field      Type    Reset   Description
 * ------------------------------------------------------------------------------------------------------------------
 * 15-8     CSKEY      RW       96h    CSKEY password. Must be written with A5h; A PCU is generated if any other
 *                                     value is written. Always reads as 96h. After the correct password is written
 *                                     all CS registers are available for writing
 * 7-0      Reserved   R        0h     Reserved. Always reads as 0
 * ------------------------------------------------------------------------------------------------------------------
 *
 * Next, we have CSCTL1, where 0x0046 has a binary equivalent of 0000 0000 0100 0110 B. These values have been placed
 * is the register diagram below. To get our board to work, we need to make DCORSEL bit (bit 6) HI, and the 3-bits of DCOFSEL 011.
 * The table below hows us that this is the correct combination to choose an internal uC frequency (called DC0) of 8MHz
 *
 * CSCTL1
 * |=======================================================================|
 * |   15   |   14   |   13   |   12   |   11   |   10   |    9   |    8   |
 * |                                Reserved                               |
 * |    0   |    0   |    0   |    0   |    0   |    0   |    0   |    0   |
 * |=======================================================================|
 * |    7   |    6   |    5   |    4   |    3   |    2   |    1   |    0   |
 * |Reserved| DCORSEL|     Reserved    |           DCOFSEL        |Reserved|
 * |    0   |    1   |    0   |    0   |    0   |    1   |    1   |    0   |
 * |=======================================================================|
 *
 * CSCTL1 Register Description:
 * Bit      Field      Type     Reset   Description
 * ------------------------------------------------------------------------------------------------------------------
 * 15-7     Reserved    R       0h      Reserved. Always reads as 0.
 * 6        DCORSEL     RW      0h      DCO range select. For high speed devices, this can be written by the user. For
 *                                      low-speed devices, it is always 0. See the description of DCOFSEL bit for details.
 * 5-4      Reserved    R       0h      Reserved. Always reads as 0.
 * 3-1      DCOFSEL     RW      6h      DCO frequency select. Selects frequency settings for the DCO. Values shown below
 *                                      are approximate. Refer to the device specific data sheet.
 *                                          000b = if DCORSEL = 0: 1 MHz; If DCORSEL = 1: 1MHz
 *                                          001b = if DCORSEL = 0: 2.67MHz; if DCORSEL = 1: 5.33MHz
 *                                          010b = if DCORSEL = 0: 3.33MHz; if DCORSEL = 1: 6.67MHz
 *                                          011b = if DCORSEL = 0: 4MHz; if DCORSEL = 1: 8MHz
 *                                          100b = if DCORSEL = 0: 5.33MHz; if DCORSEL = 1: 16MHz
 *                                          101b = if DCORSEL = 0: 6.67MHz; if DCORSEL = 1: 21MHz
 *                                          110b = if DCORSEL = 0: 8MHz; if DCORSEL = 1: 24MHz
 *                                          111b = if DCORSEL = 0: Reserved. Defaults to 8MHz. It is not recommended to use
 *                                          this setting: If DCORSEL = 1: Reserved. Defaults to 24MHz. It is not recommended
 *                                          to use this setting.
 * 0        Reserved    R       0h      Reserved. Always reads as 0
 *
 * How do we figure this out? Doing so is not easy without some help. To correctly decode all of this, you would actually need
 * to read the Clock System Module chapter and the UART chapter of the Family User's Guide. To help out sometimes, you can get sample code
 * from a uC manufacturer to get you started. TI makes this available online. too, but again, most of the sample code was written
 * by experts, for experts. But the relavent function here gives an idea of what is going on.
 *
 */
