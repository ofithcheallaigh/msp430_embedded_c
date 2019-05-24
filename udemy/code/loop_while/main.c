#include <msp430.h> 

main()
{
    char x = 0;                 // Creates a variable and initialises it
    char y = 0;                 // Creates a variable and initialises it

    while(x < 10)               // Keeps looping as long as x < 10
    {
        y = y + 1;              // y will sum the numbers while inside the loop (i.e. 0 - 9)
        x = x + 1;              // This will get us out of the loop - updates the variable condition to test
    }
    while(1);                   // Stay here when finished
}
