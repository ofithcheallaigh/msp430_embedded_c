// This program simulates a vehicle odometer

/*
 * When the program first begins, it gets to the outer loop and tens = 0.
 * The program then goes to the inner loop, and the number of km starts to increase by 1 each time around the inner loop
 * When the inner loop reaches 9, the program returns to the outer loop, since the inner loop condition calles for ones < 10
 * The outer loop will be incremented by 1 (i.e. tens increases by 1) and the program then goes back into the inner loop
 * This continues until we reach 99.
 */

#include <msp430.h>

main()
{
    int ones = 0;                                   // Set up variables
    int tens = 0;                                   // for odometer
    int km = 0;

    for(tens = 0; tens < 10; tens = tens + 1)       // Outer loop to count tens of km
    {
        for(ones = 0; ones < 10; ones = ones + 1)   // Inner loop counts km
        {
            km = 10*tens + ones;                    // Total number of km travelled
        }
    }
    while(1);                                       // Will stop here when 99km reached
}
