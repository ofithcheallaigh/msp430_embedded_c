/*
 * This challenge references "loop_while_nested".
 * -----------------------------------------------
 * The challenge was to modify the program to also use 'hundreds', 'thousands' and 'tenthousands'
 * variables to allow the odometer to count up to 99999.
 *
 * Additionally, make it so that when the odometer rolls over from 99999, it starts over at 00000 and
 * begins counting again.
 */

#include <msp430.h>

#define         DEVELOPMENT         0x5A80                  // Stops WDT

main()
{
    unsigned long ones = 0;                                 // Ones digit
    unsigned long tens = 0;                                 // Tens digits
    unsigned long huns = 0;                                 // Hundreds digits
    unsigned long thou = 0;                                 // Thousands digit (1k)
    unsigned long tnth = 0;                                 // Ten thousands digits (10k)
    unsigned long km = 0;                                   // Total number of km travelled

    WDTCTL = DEVELOPMENT;                                   // We will learn more about this later
                                                            // It is not strictly needed for a general C
                                                            // program, but it is necessary for the MSP430

    while(1)
    {
        for(tnth = 0; tnth < 10; tnth = tnth + 1)                                       // Outer loop counts ten thousands
        {
            for(thou = 0; thou < 10; thou = thou + 1)                                   // Loop counts 1 thousands
            {
                for(huns = 0; huns < 10; huns = huns + 1)                               // Loop counts hundreds
                {
                    for(tens = 0; tens < 10; tens = tens + 1)                           // Loop counts tens
                    {
                        for(ones = 0; ones < 10; ones = ones + 1)                       // Loop counts ones
                        {
                            km = 10000*tnth + 1000*thou + 100*huns + 10*tens + ones;    // Total number of km
                        }   // ends ones loop

                    }   // ends tens loop

                }   // ends huns loop

            }   // ends thou loop

        }   // ends tnth loop

        km = 0;                                             // Used to reset

    }   // Ends while(1) loop

}   // ends main
