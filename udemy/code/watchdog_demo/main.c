/*
 * Notes from Section 7, "How do I use the Watchdog Timer Peripheral?"
 *
 * In all previous programs, we have disabled the WDT. We did this with code similar to
 *
 *      WDTCLT = DEVELOPMENT;
 * Where
 *      #define DEVELOPMENT 0x5A80;
 *
 *  But why does moving 0x5A80 into the WDTCTL register disable the WDT? Just like a register like P1DIR, the WDT peripheral
 *  is controlled by a register, specifically the WatchDog Timer ConTroL register.
 *
 *  WDTCTL is a 16-bit register:
 *  |------------------------------------------------|
 *  | WDTCTL                                         |
 *  ||-----------------------------------------------|
 *  || Bin | Bin | Bin | Bin | Bin | Bin | Bin | Bin |
 *  ||  15 |  14 |  13 |  12 |  11 |  10 |  9  |  8  |
 *  ||-----------------------------------------------|
 *  ||-----------------------------------------------|
 *  || Bin | Bin | Bin | Bin | Bin | Bin | Bin | Bin |
 *  ||  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 *  ||-----------------------------------------------|
 *
 *  In the WDTCTL these bits serve different purposes. One of these is to enable/disable the peripheral. Another is to define how
 *  long the watchdog waits before resetting the uC. Another is to actually "pet" the watchdog.
 *
 *  Below is a list of the bits, their "code names" and associated functions.
 *  Bits    0-2     WatchDog Timer Interval Select (WDTIS)
 *                  Can be used to change how long the peripheral will count before resetting the uC.
 *  Bit     3       WatchDog Timer CouNTer CLear (WDTCNTCL)
 *                  This bit needs to be set HI to pet the watchdog and start counting again
 *  Bit     4       WatchDog Timer Mode SELect (WDTTMSEL)
 *                  Can be used to disconnect the watchdog timer from the uC's reset function. The
 *                  peripheral can still be used as a timer, but it loses its watchdog capability.
 *  Bits    5-6     WatchDog Timer Source SELect (WDTSSEL)
 *                  We need to make this bit HI to disable the watchdog. If it is LO, the watchdog will be
 *                  enabled
 *  Bits    8-15    WatchDog Timer PassWord (WDTPW)
 *                  These bits are reserved as the register password. You must store 0x5A (that is
 *                  0101 1010 B) in these eight bits every time you write to the register. Otherwise,
 *                  the watchdog peripheral will assume something is wrong and restart the program
 *
 *  So, recreating the above register drawing with the correct bit names gets us
 *  WDTCTL is a 16-bit register:
 *  |------------------------------------------------|
 *  | WDTCTL                                         |
 *  ||-----------------------------------------------|
 *  ||          Watchdog Timer PassWord              |
 *  ||-----------------------------------------------|
 *  || Bin | Bin | Bin | Bin | Bin | Bin | Bin | Bin |
 *  ||  15 |  14 |  13 |  12 |  11 |  10 |  9  |  8  |
 *  ||-----------------------------------------------|
 *  ||-----------------------------------------------|
 *  || HOLD|    SSEL   |TMSEL|CNTCL|       IS        |
 *  ||-----------------------------------------------|
 *  || Bin | Bin | Bin | Bin | Bin | Bin | Bin | Bin |
 *  ||  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
 *  ||-----------------------------------------------|
 *
 *  So now going back to the original instruction - WDTCTL = 0x5A80;
 *  This loads 0101 1010 1000 0000 into the register. The first 8-bits are the required password.
 *  The rest of the bits are zero, except for the HOLD bit, which disables the WDT.
 *
 *  |------------------------------------------------|
 *  | WDTCTL                                         |
 *  ||-----------------------------------------------|
 *  ||     Watchdog Timer PassWord (WDTPW)           |
 *  ||-----------------------------------------------|
 *  ||     |     |     |     |     |     |     |     |
 *  ||  0  |  1  |  0  |  1  |  1  |  0  |  1  |  0  |
 *  ||-----------------------------------------------|
 *  ||-----------------------------------------------|
 *  || HOLD|    SSEL   |TMSEL|CNTCL|       IS        |
 *  ||-----------------------------------------------|
 *  ||     |     |     |     |     |     |     |     |
 *  ||  1  |  0  |  0  |  0  |  0  |  0  |  0  |  0  |
 *  ||-----------------------------------------------|
 *
 *  To make our work easier, CCS allow us to use some abbreviations when working with registers. An example of
 *  this is the predefined terms WDTPW and WDTHOLD. So, we can stop the WDT as follows
 *
 *      WDTCTL = WDTPW | WDTHOLD;
 *
 *  If we look into the CCS files, we can see that
 *
 *      #define WDTPW       (0x5A00)
 *      #define WDTHOLD     (0x0080)
 *
 *  So when we preform that instruction, we are taking the bit-wise OR of 0x5A00 and 0x0080, and placing this
 *  in the WDTCTL register.
 *
 *  The WDT peripheral is enabled automatically when the program starts running. That is why we have to disable
 *  it for every program when it starts running. So, since the WDT is running automatically, we don't really need to
 *  enable it. However, if we want to re-enable it after it has been disabled, we need to we need to clear
 *  the WatchDog Timer HOLD bit. In other words, we need to change it from a 1 to a 0. To do this, we can simply
 *  use the following instruction
 *
 *      WDTCTL = WDTPW;         // Moves 0x5A00 into control register to enable the WDT
 *
 *  A list of definitions can be found within the programs files under
 *
 *      include > C:/ti/ccsvx/ccs_base/msp430/include > msp430fr6989.h
 *
 */

// More comments below the program

#include <msp430.h> 

#define     ENABLE_RED          0xFFFE              // Used to enable uC pins
#define     RED_LED             0x0001              // P1.0 is the red LED
#define     STOP_WDT            0x5A80              // Stops watchdog timer

main()
{
    // WDTCTL = STOP_WDT;

    PM5CTL0 = ENABLE_RED;                   // Use pins as inputs and outputs

    P1DIR = RED_LED;                        // Sets the red LED as output
    P1OUT = RED_LED;                        // Turns on the red LED

    while(1)
    {
        // WDTCTL = WDTPW | WDTCNTCL;          // Continuously pet the WDT by making the WDTCNTCL bit go HI
    }
}


/*
 * Notice the first instruction disables or stops the WDT peripheral has been commented out. Now when
 * the program runs the WDT will be counting. Since we are not petting the WDT, the watchdog timer's counter
 * will overflow, and the peripheral will reset the uC and start the program over from the beginning
 *
 * What is happening here is that the WDT counter is overflowing approximately every 0.032 seconds (or
 * every 32 milliseconds, 32ms).
 *
 * This can be seen by using a scope, and connecting to the P1.0 red LED.
 */
