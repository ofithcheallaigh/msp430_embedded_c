#include <msp430.h>

#define RED_LED_ON          0x0001
#define RED_LED_OFF         0x0000
#define P1_DIR_OUT          0x0001
#define DEVELOPMENT         0x5A80
#define ENABLE_PINS         0xFFFE
#define ACLK                0x0100
#define UP                  0x0010
#define TAIFG               0x0001

main()
{
    WDTCTL = DEVELOPMENT;
    PM5CTL0 = ENABLE_PINS;

    TA0CCR0 = 40000;
    TA0CTL = ACLK | UP;

    P1DIR = P1_DIR_OUT;
    P1OUT = RED_LED_ON;

    unsigned char counter = 0;

    while(1)
    {
        if(TA0CTL & TAIFG)
        {
            counter = counter + 1;
            TA0CTL = TA0CTL & (~TAIFG);

            if(P1OUT & RED_LED_ON)
            {
                if(counter == 3)
                {
                    P1OUT = RED_LED_OFF;
                }
            }
            else
            {
                P1OUT = RED_LED_ON;
                counter = 0;
            }
        }
    }
}
