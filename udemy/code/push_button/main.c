/*
 * Program turns on the red LED when button S1, connected to P1.1 is pressed. LED is connected to P1.0
 *
 * NOTES:
 * -----
 * - P1DIR is actually referring to Port 1 DIRection register. This is a special 8-bit memory location inside of the
 *   registers that determines if the pins in a port are going to be used as inputs or outputs. There are similar registers for
 *   port 2, port 3, part 4 etc.
 * - When a bit in the P1DIR register has a 0 in it, the corresponding pin will be an input
 *   When a bit in the P1DIR register has a 1 in it, the corresponding pin will be an output
 * - So when P1DIR = 00000001B, pin P1.0 will be an output, all the rest will be inputs
 * - To try and make this kind of thing easier, we can have the following definitions:
 * #define BIT0             0x01                    // 0x01 = 0000 0001 in binary
 * #define BIT1             0x02                    // 0x02 = 0000 0010 in binary
 * #define BIT2             0x04                    // 0x04 = 0000 0100 in binary
 * #define BIT3             0x08                    // 0x08 = 0000 1000 in binary
 * #define BIT4             0x10                    // 0x10 = 0001 0000 in binary
 * #define BIT5             0x20                    // 0x20 = 0010 0000 in binary
 * #define BIT6             0x30                    // 0x40 = 0100 0000 in binary
 * #define BIT7             0x40                    // 0x80 = 1000 0000 in binary
 * - Therefore P3DIR = BIT7 + BIT4 + BIT1 + BIT0;   // P3.0, P3.1, P3.4, P3.7 are set as outputs
 */

#include <msp430.h>                 // This file tells CCS how names (like P1OUT and P1DIR) are used by the uC

// The #defines does essentially the same thing as the <msp430.h> file, except we are defining what we want
#define RED_LED_OUT         0x0001                  // P1.0 is the red LED
#define CLEAR_RED_LED       0x00FE                  // Used to turn off the red LED
#define BUTTON11            0x0002                  // P1.1 is the button
#define DEVELOPMENT         0x5A80                  // Stops the WDT
#define ENABLE_PINS         0xFFFE                  // Required to use inputs and outputs

main()
{
    WDTCTL = DEVELOPMENT;

    PM5CTL0 = ENABLE_PINS;                  // Prepare pins for I/O usage

    /*
     * We define the RED_LED_OUT label to correspond to the hex number 0x01. If we write this number as
     * a binary number, we get: 0x01 = 0000 0001.
     * This could also be written as P1DIR = RED_LED; P1DIR = 0x01; P1DIR = 0b00000001;
     */
    P1DIR = RED_LED_OUT;                    // Pin connected to red LED will be an output

    /*
    * We already have 0x01 loaded into P1DIR which leaves pin 1 on port 1 as an input. So when a pin is configured
    * as an input (via P1DIR) AND its pin values are both 1 in the P1OUT and P1REN register, then the
    * uC will enable the pull-up resistor to stop the pin floating when the switch is not pushed.
    */
    P1OUT = BUTTON11;                       // Button needs a pull-up resistor
    P1REN = BUTTON11;                       // This is equivalent to P1OUT = 0x02;
                                            // and P1REN = 0x02;
                                            // were 0x02 = 0000 0010 in binary, so this puts pin 1 of both registers Hi.

     /* Notes for push button operation, as well as the loop operation:
     * The first instruction inside the While loop is checking to see of the P1.1 button has been pressed.
     * Note that this is being done inside a While loop. And like any While loop, it has a condition which is
     * being checked for. In this case, the condition is ((BUTTON11 & P1IN) == 0). We are testing something inside the curly
     * brackets. If the something is 0 the condition will be true, and the program will go into the inner loop.
     * If the something is non-zero, then the condition will be false, and the program will skip everything inside the
     * curly braces.
     *
     * When we look at our condition, we can see it is a bit-wise AND (&) on two 8-bit values: the contents of an
     * 8-bit box called BUTTON11 and another 8-bit box called P1IN, so we get: BUTTON11 & P1IN. Looking at the #define
     * statements, we can see BUTTON11 = 0x02 (= 0000 0010).
     * Next, P1IN is an 8-bit register that has 8 bins, each bin corresponding to one of the port 1 pins.
     * P1IN contains the INput values that the uC Port 1 pins presently are connected to. When we are writing our program
     * we will not know at any given time which inputs will be connected to a Hi (Logic 1)or Lo (Logic 0) value. This is where
     * the power of the condition comes into play. We are going to bit-wise AND the contents of BUTTON11 and the P1IN values.
     *
     * -----------------------------------------------------------------------
     * | BUTTON11                                                            |
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
     * This operation will be the bit-wise AND of the two registers. So, what is the truth table for AND:
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
     * So, from this we know that since 7 of the BUTTON11 bits are zero, seven of the results will be zero.
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
     * the bit-wise AND operation. We already know the BUTTON11 is Hi, due to the pull-up resistor, so
     *          BUTTON11 AND P1IN.1 = Result.1
     *          Hi       AND P1IN.1 = Result.1
     * And this results in the following truth table
     * |-----------------------------------|
     * |  BUTTON11  |  P1IN.1  | Result.1  |
     * |-----------------------------------|
     * |      1     |    0     |     0     |
     * |      1     |    1     |     1     |
     * |-----------------------------------|
     * Therefore, id P1IN is Hi, Result.1 will be Hi, and P1IN will be non-zero
     * If P1IN is Lo, Rrsult.1 will be Lo and P1IN will be zero.
     *
     * So, when will pin 1 of P1IN be high or low? When the S1 push-button is pressed, then P1.1
     * is pulled to ground. This means a logic 0 will be on the pin, therefore, P1.1 will be 0.
     * When P1IN.1 is zero, the result of the bit-wise AND will be zero. The condition for testing
     * if the bit-wise result is 0, so the result will be true, and the program will go into the loop
     * and execute the commands there. Obviously, the opposite happens when it is not true, therefore,
     * the condition will it be met when the button isn't pressed.
     *
     * Inside the loop, we use an 8-bitwise logic OR (|) operation between the contents of the 8-bit
     * P1OUT register box and the 8-bit RED_LED box.
     * We have to use the logic OR operation to set the P1OUT bit 0 (P1OUT.0) Hi to turn on the LED
     * without interfering with the previous instruction.
     *
     * Below we have shown the P1OUT and the RED_LED boxes.
     * Recall, P1OUT has a value of 0000 0010 binary from its previous instruction. RED_LED was setup
     * earlier to have a value of 0000 0001 binary
     *
     * -----------------------------------------------------------------------
     * | P1OUT                                                               |
     * | ------------------------------------------------------------------  |
     * | | Pin 7  | Pin 6 | Pin 5 | Pin 4 | Pin 3 | Pin 2 | Pin 1 | Pin 0 |  |
     * | |   0    |   0   |   0   |   0   |   0   |   0   |   1   |   0   |  |
     * | ------------------------------------------------------------------  |
     * |---------------------------------------------------------------------|
     *  *************************** Bit-Wise AND *****************************
     * -----------------------------------------------------------------------
     * | RED_LED                                                             |
     * | ------------------------------------------------------------------  |
     * | | Pin 7  | Pin 6 | Pin 5 | Pin 4 | Pin 3 | Pin 2 | Pin 1 | Pin 0 |  |
     * | |   0    |   0   |   0   |   0   |   0   |   0   |   0   |   1   |  |
     * | ------------------------------------------------------------------  |
     * |---------------------------------------------------------------------|
     *
     * Below is the logic OR truth table
     * |-----------------|
     * |  OR Truth Table |
     * |-----------------|
     * |  X  |  Y  |  Z  |
     * |-----------------|
     * |  0  |  0  |  0  |
     * |  0  |  1  |  1  |
     * |  1  |  0  |  1  |
     * |  1  |  1  |  1  |
     * |-----------------|
     *
     * Therefore the result of the logic operation will be:
     *    00000010  P1OUT
     *  | 00000001  RED_LED
     *  ----------
     *    00000011
     * This not only turns on the red LED connected to pin P1.0, but it also keeps the P1.1
     * pull-up resistor in place.
     */
    while(1)                                            // Keep looping forever
    {
        while((BUTTON11 & P1IN) == 0)                   // Is P1.1 button pushed?
        {
            P1OUT = P1OUT | RED_LED_OUT;                    // Turns on the red LED
        }
        /*
         * Now, for turning off the LED. This line uses a bit-wise AND to clear bit 0 of the
         * P1OUT register, without affecting the pull-up resistor associated with bit 1 of P1OUT
         *
         * Below is the pictorial representation of the P1OUT and CLEAR_RED_LED registers.
         *
         * NB: We cannot tell for sure what the contents of bit 0 in P1OUT will be at this input.
         * It may have already been turned on (P1OUT.0 will be Hi) or it could be turned off
         * (P1OUT.0 will be Lo).
         *
         * Regardless of the previous value of the P1OUT.0, this operation will make it Lo
         * to ensure the red LED is off.
         *
         * Again, let's look at the AND truth table:
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
         * This shows that no matter what value is in P1OUT.0, the result will always be 0.
         * -----------------------------------------------------------------------
         * | P1OUT                                                               |
         * | ------------------------------------------------------------------  |
         * | | Pin 7  | Pin 6 | Pin 5 | Pin 4 | Pin 3 | Pin 2 | Pin 1 | Pin 0 |  |
         * | |   0    |   0   |   0   |   0   |   0   |   0   |   1   |   ?   |  |
         * | ------------------------------------------------------------------  |
         * -----------------------------------------------------------------------
         *  *************************** Bit-Wise AND *****************************
         * -----------------------------------------------------------------------
         * | CLEAR_RED_LED                                                       |
         * | ------------------------------------------------------------------  |
         * | | Pin 7  | Pin 6 | Pin 5 | Pin 4 | Pin 3 | Pin 2 | Pin 1 | Pin 0 |  |
         * | |   1    |   1   |   1   |   1   |   1   |   1   |   1   |   0   |  |
         * | ------------------------------------------------------------------  |
         * -----------------------------------------------------------------------
         * Again, we are bit-wise AND'ing the two together. Because the first seven bits (or pins)
         * of CLEAR_RED_LED are Hi, the result will simply be the same values already stored
         * in the first seven bits of P1OUT.
         *
         * However, because bit 0 is Lo in CLEAR_RED_LED, the result will force P1OUT.0 to a Lo.
         *
         * This ensures that every time that the push button is not pressed, the red LED
         * will be forced off.
         */
        P1OUT = P1OUT & CLEAR_RED_LED;                  // Turns off the red LED
    }
}
