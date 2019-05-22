/*
 * LED and external button
 *
 * This program has an LED placed on a breadboard. The longer leg of the LED is the positive side, meaning the shorter leg is ground.
 * In this setup, we have a 100Ohm resistor connected between the shorter leg and ground on the breadboard, which will be connected to
 * ground on the uC via a jumper wire.
 *
 * Next, we will have a second jumper wire connected to P9.6 on the uC, and this will then be connected to the positive side of the LED
 * (i.e. the longer leg) on the breadboard.
 *
 * With the LED connected, now we placed a 4-pin push button on the breadboard. As with the LED, there needs to be a ground connection.
 * Connect the jumper wire to a second ground connection on the uC, and then connect the other side of the wire to a pin on the button.
 *
 * Next connect a jumper wire from P9.5 on the uC. The other side is connected to the other pin on the same side of the ground connection
 * (i.e. both connects on the left hand side)
 *
 * The program below will allow the button and the LED to be used.
 */

#include <msp430.h>                             // Allows us to use shortcut names

#define ENABLE_PINS     0xFFFE                  // Used to enable uC pins
#define DEVELOPMENT     0x5A80                  // Used to disable some of the security
                                                // features while we are still learning

main()
{
    WDTCTL = DEVELOPMENT;                       // Disables some security features
    PM5CTL0 = ENABLE_PINS;                      // Enables pins to the outside world
    P9DIR = BIT6;                               // P9.6 will be connected to the yellow LED

    P9REN = BIT5;                               // Enable all pull up resistors for pin 5
    P9OUT = BIT5;                               //

    long x = 0;                                 // Will be used to slow down blinking

    while(1)                                    // Continuously repeat everything below
    {
        if((BIT5 & P9IN) == 0)                  // If button connected to P9.5 is pushed
        {
            P9OUT = P9OUT | BIT6;               // Turn on the yellow LED
        }

        else
        {
            P9OUT = BIT5;                       // Turn off the yellow LED, but leave the
                                                //
        }
    }
}
