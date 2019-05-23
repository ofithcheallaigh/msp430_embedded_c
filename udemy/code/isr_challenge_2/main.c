/*
 * Challenge instruction
 * 1. Do not disable the WDT, instead set up Timer1 to use an interrupt every 0.01seconds to pet the watchdog
 * 2. Use an interrupt on Timer0 to toggle the red LED every second
 * 3. Monitor the status of the P1.1 push button (do this in main)
 *      a. When the button is pressed, the green LED comes on (do this in main)
 *      b. When the button is not pressed, the green LED is off (do this in main)
*  4. Create a function (not an ISR) to set up the inputs and outputs
*  5. Create a function (not an ISR) to set up and start Timer0 counting
*  6. Create a function (not an ISR) to set up and start Timer1 counting
 */

#include <msp430.h> 

#define PET_WATCHDOG            0x5A08
#define ACLK                    0x0100
#define UP                      0x0010
#define ENABLE_PINS             0xFFFE

// Function prototypes
void init_pins (void);
void setup_timer0 (void);
void setup_timer1 (void);

main()
{
    // Setup the pins
    init_pins();

    // Set up the two timers
    setup_timer0();
    setup_timer1();

    // Activate all enabled interrupts
    _BIS_SR(GIE);

    // Set up a while loop to monitor the push button, and turn red LED on if pressed, off if not
    while(1)
    {
        if((BIT1 & P1IN) == 0)
        {
            P9OUT = BIT7;                   // Turns on the green LED
        }
        P9OUT = 0x0000;                     // Turns off the green LED
    }
}

// ******************************************
// init_pins function
// ******************************************
void init_pins(void)
{
    PM5CTL0 = ENABLE_PINS;

    P9DIR = BIT7;                           // The green LED is on P9.7

    P1DIR = BIT0;                           // Sets Port 1 as on output
    P1OUT = BIT1;
    P1REN = BIT1;                           // Pull up resistor enabled
}
// *****************************************
// Timer0 ISR - Toggle red LED
// *****************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR (void)
{
    P1OUT = P1OUT ^ BIT0;                   // Toggles the red LED
}
// *****************************************
// Timer1 ISR - Pet WDT every 10ms
// *****************************************
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_ISR (void)
{
    WDTCTL = PET_WATCHDOG;
}
// *****************************************
// setup_timer0() function definition
// *****************************************
void setup_timer0 (void)
{
    TA0CCR0 = 40000;                        // (40000 * 25us) = 1sec
    TA0CTL = ACLK + UP;                     // ACLK in UP mode
    TA0CCTL0 = CCIE;                        // Enable interrupts for timer0
}
// *****************************************
// setup_timer1() function definition
// *****************************************
void setup_timer1 (void)
{
    TA1CCR0 = 400;                          // (400 * 25us) = 0.01sec
    TA1CTL = ACLK + UP;                     // ACLK in UP mode
    TA1CCTL1 = CCIE;                        // Enable interrupts for timer1
}
