/*
 * Section 8 - How do I use the MSP430FR6989 General Purpose Timer
 *
 * The first thing to know about the general purpose timers in a uC is that they are very, very, flexible
 * peripherals and can often be used in up to a dozen different ways. However, all timers do is count.
 *
 * A lot of time could be spend understanding timer peripherals, especially the finer points. However our
 * goal is to learn how to count up, which is the most common mode of operation. UP mode is probably sufficient
 * for 95% of the developers, 95% of the time.
 *
 * Timer_A is a 16-bit general purpose timer on the MSP430FR6989. This means that it can count from
 * 0x0000 up to 0xFFFF (or 0 to 65,535 decimal).
 *
 * There are actually several Timer_1 peripherals on the uC, but the first one we will look at will be
 * Tinmer_A0. We will look at some of the other Timer_A peripherals later.
 *
 * Timer_A0 is primarily controlled by a register called Timer A0 ConTroL (or TA0CTL) register. Again, this is a
 * 16-bit register
 *
 *  |------------------------------------------------|
 *  | TA0CTL                                         |
 *  ||-----------------------------------------------|
 *  || Bin | Bin | Bin | Bin | Bin | Bin | Bin | Bin |
 *  ||  15 |  14 |  13 |  12 |  11 |  10 |  9  |  8  |
 *  ||-----------------------------------------------|
 *  ||-----------------------------------------------|
 *  || Bin | Bin | Bin | Bin | Bin | Bin | Bin | Bin |
 *  ||  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 *  ||-----------------------------------------------|
 *
 *  By manipulating the bits in the Timer A0 ConTroL register, we can put the timer into UP mode
 *  and specify how long we want it to count for.
 *
 *  The bits are broken out as follows:
 *  Bit     0       Timer_A Interrupt FlaG (TAIFG)
 *                  This bit will go HI when the timer has counted up to its specified value. It will
 *                  remain LO if the timer has not finished counting yet
 *  Bits    4-5     Mode Control (MC)
 *                  These two bits are used to put the peripheral into UP mode. This allows the
 *                  peripheral to count up from 0 to a value you will choose
 *  Bits    8-9     Timer A Source SELect (TASSEL)
 *                  These two bits are used to specify how fast we want the time to count
 *
 *  |------------------------------------------------|
 *  | TA0CTL                                         |
 *  ||-----------------------------------------------|
 *  ||               NOT USED            |  TASSEL   |
 *  ||-----------------------------------------------|
 *  || Bin | Bin | Bin | Bin | Bin | Bin | Bin | Bin |
 *  ||  15 |  14 |  13 |  12 |  11 |  10 |  9  |  8  |
 *  ||-----------------------------------------------|
 *  ||-----------------------------------------------|
 *  || NOT USED  |    MC     |    NOT USED     |TAIFG|
 *  ||-----------------------------------------------|
 *  || Bin | Bin | Bin | Bin | Bin | Bin | Bin | Bin |
 *  ||  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 *  ||-----------------------------------------------|
 *
 *  To put the Timer_A0 peripheral into UP mode, we make the MC bits 01B. This can be done with the following #define
 *  statement
 *
 *      #define     UP      0x0010          // This is: 0000 0000 0001 0000 binary
 *
 *  |------------------------------------------------|
 *  | TA0CTL                                         |
 *  ||-----------------------------------------------|
 *  ||               NOT USED            |  TASSEL   |
 *  ||-----------------------------------------------|
 *  ||     |     |     |     |     |     |     |     |
 *  ||-----------------------------------------------|
 *  ||-----------------------------------------------|
 *  || NOT USED  |    MC     |    NOT USED     |TAIFG|
 *  ||-----------------------------------------------|
 *  ||     |     |  0  |  1  |     |     |     |     |
 *  ||-----------------------------------------------|
 *
 *  To specify how fast the timer will count, we will also make TASSEL bits 01B. This tells the counter to use
 *  something called the Auxiliary CLocK (ACLK) as its timing source.
 *
 *  Think of the ACLK like this: if you are measuring time with a normal clock, it will increment every second. With
 *  the ACLK timing source, the Timer_A0 peripheral will be incrementing approximately every 25us. (It should be noted
 *  there are other clock sources available, but the ACLK will do for now). This is done as follows
 *
 *      #define     ACLK        0x0100          // That is: 0000 1000 0000 0000 binary
 *
 *  Our register now looks like
 *  |------------------------------------------------|
 *  | TA0CTL                                         |
 *  ||-----------------------------------------------|
 *  ||               NOT USED            |  TASSEL   |
 *  ||-----------------------------------------------|
 *  ||     |     |     |     |     |     |  0  |  1  |
 *  ||-----------------------------------------------|
 *  ||-----------------------------------------------|
 *  || NOT USED  |    MC     |    NOT USED     |TAIFG|
 *  ||-----------------------------------------------|
 *  ||     |     |  0  |  1  |     |     |     |     |
 *  ||-----------------------------------------------|
 *
 * To achieve this in one instruction, we can write the following
 *
 *      TA0CTL = ACLK | UP;         // Takes the logic OR of 0x0100 and 0x0010, storing the result in TA0CTL
 *                                  // Therefore, TA0CTL = 0x0110
 *
 * Now we have told our counter how fast to count, and that we want it to count in the UP mode. The last thing to tell it
 * is what to count to. This is done with the   Timer_A0 Capture/Compare Register 0 (or TA0CCR0).
 *
 * There are a lot of different ways this register can be set up, but in the UP mode, we will compare its count value to
 * the number stored we store in TA0CCR0. For now, we will add a value of 20,000. This will cause our counter to count for
 * approximately 0.5 seconds -> 25E-6 * 20000 = 0.5.
 *
 * When we count up to TA0CCR0, the peripheral will alert you that the task is completed.
 *
 *      TA0CCR0 = 20000;        // Count for 20000 * 25us = 0.5 seconds
 *
 */

// Now that we know how to configure or general purpose timer peripheral, let us write a program with instructions
// to stop the WDT and configure P1.0

#include <msp430.h>

#define     RED_LED         0x0001          // P1.0 is the red LED
#define     DEVELOPMENT     0x5A80          // Stops the WDT
#define     ENABLE_PINS     0xFFFE          // Required to use inputs and outputs
#define     ACLK            0x0100          // Timer_A ACLK source
#define     UP              0x0010          // Timer_A UP mode
#define     TAIFG           0x0001          // Used to look at the Timer A Interrupt FlaG

main()
{
    WDTCTL = DEVELOPMENT;
    PM5CTL0 = ENABLE_PINS;

    TA0CCR0 = 65535;                        // We will count from 0 to 20000, it will count to a max of 65535
    TA0CTL = ACLK | UP;                     // Use the ACLK in UP mode

    P1DIR = RED_LED;                        // Sets up the red LED as an output

    while(1)
    {
        /*
         * Once our program gets to the infinite loop, we will have to check to see if the general purpose timer
         * as reached 20000. To do this we need to check to see if the Timer_A0 Interrupt FlaG (TAIFG) has moved from LO to HI.
         * This is Bit0 of the TA0CTL register, detailed in the notes above
         *
         * The in instruction begins by taking the bit-wise logic-AND of the contents of the TA0CTL register and
         * TAIFG (which we defined as 0x0001).
         *
         * Now, since anything we AND with a 0 is also 0, the first 15 bits of the result (bits 1-15) will be LO. The only bit
         * in the result which may not be LO is bit 0. It all depends on the value of the TAFIG bit in the TA0CTL register.
         *
         * If the TAIFG bit in the TA0CTL register is LO, the peripheral HAS NOT counted to 20000 yet, and the result of the bit-wise
         * AND operation will be 0x0000. Therefore the if-statement will NOT be true.
         *
         * If the TAIFG bit in the TA0CTL register is HI, the peripheral HAS counted to 20000, and the result of the bit-wise
         * AND operation will be 0x0001. The if-statement will be TRUE, and execution will continue into the if statement.
         */
        if(TA0CTL & TAIFG)          // Will be TRUE if the if the TAIFG bit in TA0CTL is HI
        {
            /*
             * Once we have determined the peripheral has counted to 20000, we want to toggle the P1.0 red LEDand ensure the
             * timer keeps counting. This is done with two separate instructions. The following toggles the LED
             */
            P1OUT = P1OUT ^ RED_LED;    // This is taking the bit-wise XOR of the contents of the P1OUT register and the
                                        // value defined as RED_LED (0x01). Anything XOR'd with a 0 stays the same. Anything
                                        // XOR'd with a 1 changes. So, since RED_LED = 0000 0001B, the instruction will toggle
                                        // Bit 0 of P1OUT

            /*
             * Next we need to make sure that the general purpose timer keeps on counting. This instruction does two things.
             * First, it takes the bit-wise inverse of our TAIFG value. Since TAIFG was 0x0001, the result in the inversion becomes
             * 0xFFFE (or 1111 1111 1111 1110B).
             *
             * Next, the program takes the bitwise-AND of the contents of TA0CTL register with the inverted TAIFG value.
             * The result is stored back in TA0CTL.
             */
            TA0CTL = TA0CTL & (~TAIFG); // Count again


        }
    }
}
