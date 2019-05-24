/*
 * As was stated in the timer_up_mode program, the maximum number a 16-bit register can hold is 65535. But what happens if we
 * load a larger number into the TA0CCR0 register?
 *
 * As an example, if we load 70000, the program will no display an error. So if we use the program where an LED toggles every time the
 * counter reaches a specific value, if we put 70,000 into the register, we would be expecting quite a long time between toggles.
 * Instead, the LED toggles very quickly, almost as if we put 5000 into the register. Why is this?
 *
 * The program will do its best to carry out the instruction. So, when asked to load 70000, it will start loading. When it reaches 65535
 * the register is full, and then rolls over. However, there is still 4465 left of the 70000 (70000 - 65535 = 4465). So the register
 * over again and is loaded with the remaining 4465.
 *
 * To reinforce this point, if you loaded 135535 into the register, it would roll over twice:
 *
 *      135535 - 65535 - 65535 = 4465
 *
 * So, if we can only count up to 65535 with a 16-bit timer, how do we count for a longer period of time? The answer is that we
 * write a program to count to 65535 multiple times.
 *
 * The program below counts up to 500,000 by counting up to 50,000 ten times (using a variable called intervals). Every time TAIFG
 * goes HI we add one to intervals. When intervals reach 10, we toggle the red LED and resets intervals back to 0 to start the
 * process over.
 *
 */

#include <msp430.h>

#define RED_LED             0x0001              // P1.0 is the red LED
#define PIN_DIR_OUT         0x0001              // Sets a pin to be an output
#define DEVELOPMENT         0x5A80              // Stops the WDT
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define ACLK                0x0100              // Time_A ACLK source
#define UP                  0x0010              // Time_A UP mode
#define TAIFG               0x0001              // Used to look at Timer A Interrupt FlaG

main()
{
    unsigned char intervals = 0;                // Counts the number of 50000 counts

    WDTCTL = DEVELOPMENT;                       // Turns off the WDT
    PM5CTL0 = ENABLE_PINS;                      // Enables inputs and outputs

    TA0CCR0 = 50000;                            // Counts up from 0 to 50000
    TA0CTL = ACLK | UP;                         // Use ACLK source in UP mode

    P1DIR = PIN_DIR_OUT;                        // Sets the P1 direction as output, setting red LED as output

    while(1)
    {
        if(TA0CTL & TAIFG)                      // Checks if counter has counted to 50000
        {
            intervals = intervals + 1;         // Updates the number of 50000 counts

            TA0CTL = TA0CTL & (~TAIFG);         // Count again

            if(intervals == 20)                 // If counted 10 * 50000 = 500000
            {
                intervals = 0;                  // Reset intervals counter
                P1OUT = P1OUT ^ RED_LED;         // Toggle red LED
            }
        }
    }
}
