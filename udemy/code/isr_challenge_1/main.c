/*
 * Write a program to accomplish the following 5 tasks:
 *  1. Disable the WDT
 *  2. Uses an interrupt on Timer0 to toggle the red LED every second
 *  3. Monitor the status of the P1.1 pushbutton (do this in the main)
 *  4. When the button is pressed, the green LED is on (do this in main)
 *  5. When the button is not pressed, the green LED is off
 *
 *  In this program, we have eliminated our convention of explicitly defining things like
 *  RED_LED, GREEN_LED and BUTTON11. Instead, we are using BIT0, BIT7 and BIT1 respectively.
 *  These are already defined for us in the msp430.h file that we #include, and it just makes
 *  our programs just a little shorter to write.
 *
 * NOTES FOR THE PUSH BUTTON OPERATION:
 *
 * Remember:
 * BIT0             0x01                    // 0x01 = 0000 0001 in binary
 * BIT1             0x02                    // 0x02 = 0000 0010 in binary
 * BIT2             0x04                    // 0x04 = 0000 0100 in binary
 * BIT3             0x08                    // 0x08 = 0000 1000 in binary
 * BIT4             0x10                    // 0x10 = 0001 0000 in binary
 * BIT5             0x20                    // 0x20 = 0010 0000 in binary
 * BIT6             0x30                    // 0x40 = 0100 0000 in binary
 * BIT7             0x40                    // 0x80 = 1000 0000 in binary
 * - Therefore P3DIR = BIT7 + BIT4 + BIT1 + BIT0;   // P3.0, P3.1, P3.4, P3.7 are set as outputs
 *
 * The first instruction inside the While loop is checking to see of the P1.1 button has been pressed.
 * Note that this is being done inside a While loop. And like any While loop, it has a condition which is
 * being checked for. In this case, the condition is ((BIT1 & P1IN) == 0). We are testing something inside the curly
 * brackets. If the something is 0 the condition will be true, and the program will go into the inner loop.
 * If the something is non-zero, then the condition will be false, and the program will skip everything inside the
 * curly braces.
 *
 * When we look at our condition, we can see it is a bit-wise AND (&) on two 8-bit values: the first being BIT1
 * and another 8-bit box called P1IN, so we get: BIT1 & P1IN. Looking at the table
 * above, we can see BIT1 = 0x02 (= 0000 0010).
 * Next, P1IN is an 8-bit register that has 8 bins, each bin corresponding to one of the port 1 pins.
 * P1IN contains the INput values that the uC Port 1 pins presently are connected to. When we are writing our program
 * we will not know at any given time which inputs will be connected to a Hi (Logic 1)or Lo (Logic 0) value. This is where
 * the power of the condition comes into play. We are going to bit-wise AND the contents of BUTTON11 and the P1IN values.
 *
 * -----------------------------------------------------------------------
 * | BIT1                                                                |
 * | ------------------------------------------------------------------  |
 * | | Pin 7  | Pin 6 | Pin 5 | Pin 4 | Pin 3 | Pin 2 | Pin 1 | Pin 0 |  |
 * | |   0    |   0   |   0   |   0   |   0   |   0   |   1   |   0   |  |
 * | ------------------------------------------------------------------  |
 * |---------------------------------------------------------------------|
 *  *************************** Bit-Wise AND *****************************
 * -----------------------------------------------------------------------
 * | P1IN                                                                |
 * | ------------------------------------------------------------------  |
 * | | Pin 7  | Pin 6 | Pin 5 | Pin 4 | Pin 3 | Pin 2 | Pin 1 | Pin 0 |  |
 * | |   ?    |   ?   |   ?   |   ?   |   ?   |   ?   |   ?   |   ?   |  |
 * | ------------------------------------------------------------------  |
 * |---------------------------------------------------------------------|
 *
 * This operation will be the bit-wise AND of the two values. So, what is the truth table for AND:
 *
 * |-----------------|
 * | AND Truth Table |
 * |-----------------|
 * |  X  |  Y  |  Z  |
 * |-----------------|
 * |  0  |  0  |  0  |
 * |  0  |  1  |  0  |
 * |  1  |  0  |  0  |
 * |  1  |  1  |  1  |
 * |-----------------|
 *
 * So, from this we know that since 7 of the BIT1 bits are zero, seven of the results will be zero.
 * So we know that the following will always be the case
 * -----------------------------------------------------------------------
 * | Result                                                              |
 * | ------------------------------------------------------------------  |
 * | | Pin 7  | Pin 6 | Pin 5 | Pin 4 | Pin 3 | Pin 2 | Pin 1 | Pin 0 |  |
 * | |   0    |   0   |   0   |   0   |   0   |   0   |   ?   |   0   |  |
 * | ------------------------------------------------------------------  |
 * |---------------------------------------------------------------------|
 *
 * So, how do we find the value of pin 1 in the result? As with the other pins, we are looking at
 * the bit-wise AND operation. We already know the button is HI, due to the pull-up resistor, so
 *          BUTTON   AND P1IN.1 = Result.1
 *          HI       AND P1IN.1 = Result.1
 * And this results in the following truth table
 * |-----------------------------------|
 * |    BUTTON  |  P1IN.1  | Result.1  |
 * |-----------------------------------|
 * |      1     |    0     |     0     |
 * |      1     |    1     |     1     |
 * |-----------------------------------|
 * Therefore, if P1IN is HI, Result.1 will be HI, and P1IN will be non-zero
 * If P1IN is LO, Result.1 will be LO and P1IN will be zero.
 *
 * So, when will pin 1 of P1IN be high or low? When the S1 push-button is pressed, then P1.1
 * is pulled to ground. This means a logic 0 will be on the pin, therefore, P1.1 will be 0.
 * When P1IN.1 is zero, the result of the bit-wise AND will be zero. The condition for testing
 * if the bit-wise result is 0, so the result will be true, and the program will go into the loop
 * and execute the commands there. Obviously, the opposite happens when it is not true, therefore,
 * the condition will it be met when the button isn't pressed.
 *
 */


#include <msp430.h> 

#define STOP_WATCHDOG           0x5A80
#define ACLK                    0x0100              // Timer_A ACLK source
#define UP                      0x0010              // Timer_A UP mode
#define ENABLE_PINS             0xFFFE              // Required to use inputs and outputs

main()
{
    WDTCTL = STOP_WATCHDOG;

    PM5CTL0 = ENABLE_PINS;
    P9DIR = BIT7;                                   // Green LED is P9.7 (Port 9, Bit 7)

    P1DIR = BIT0;                                   // Ensure P1.1 is an input and
                                                    // P1.0 is an output

    P1OUT = BIT1;                                   // P1.1 needs a pull-up resistor
    P1REN = BIT1;

    TA0CCR0 = 40000;                                // (40000 * 25us) = 1sec
    TA0CTL = ACLK + UP;                             // Sets the ACLK in UP mode
    TA0CCTL0 = CCIE;                                // Enable interrupts for Timer0

    _BIS_SR(GIE);                                   // Activate previously enabled interrupts

    while(1)
    {
        while((BIT1 & P1IN) == 0)                   // Is the button pushed?
        {
            P9OUT = BIT7;                           // Turn on the green LED (P9.7)
        }
        P9OUT = 0x00;                               // Turn off green LED
    }
}


// *************************
// Timer0 ISR
// *************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR (void)
{
    P1OUT = P1OUT ^ BIT0;                           // Toggle the LED on P1.0
}
