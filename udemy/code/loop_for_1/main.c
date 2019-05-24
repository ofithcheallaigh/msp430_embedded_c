/*
 * XOR:
 * The output will be 1 if exactly one input  is 1.
 * The output will be 0 if both inputs are 0.
 * The output will be 0 if both inputs are 1.
 *
 * Truth table:
 *
 *  | X  Y | Z |
 *  |----------|
 *  | 0  0 | 0 |
 *  | 0  1 | 1 |
 *  | 1  0 | 1 |
 *  | 1  1 | 0 |
 *  |----------|
 *
 */


#include <msp430.h> 

#define RED_ON          0x0001                  // Enable and turn on the red LED
#define RED_OFF         0x0000                  // Turn off the LED
#define DEVELOPMENT     0x5A80                  // Stop the WDT
#define ENABLE_IO       0xFFFE                  // Used to ensure outputs are ready

main()
{
    WDTCTL = DEVELOPMENT;                       // Stop the WDT

    // The next three instructions prepare the uC to use one of its pins to turn on and off
    // the red LED. We will learn more about these instructions later on
    PM5CTL0 = ENABLE_IO;                        // Enable outputs
    P1DIR = RED_ON;                             // Red LED pin will be an output, remember RED_ON = 0x0001
    P1OUT = RED_ON;                            // Start with the red LED being off

    long x;                                     // Creates variable, x. No value has been assigned

    for(x = 0; x < 30000; x = x + 1)            // Loop 30,000 times
    {
        // Here, we will use the XOR instruction. (See above for Truth Table).
        // In this next line, if the red LED is on, it will be turned off
        // and if it is off, it will be turned on
        // P1OUT = P1OUT ^ RED_ON;                 // Toggles LED when x < 30,000
    }

    P1OUT = RED_OFF;                            // Turns off the LED

    while(1);
}
