// Program to look at counting in binary and learn new features of CCS

#include <msp430.h>                         // Used to make code easier to read

#define         DEVELOPMENT     0x5A80      // Used to disable WDT for development

main()
{
    WDTCTL = DEVELOPMENT;               // Disable WDT for development

    long count = 0;                     // Create variable named count, set equal to 0

    while(count < 20)                   // count for 0 to 19 (code is less than 20))
    {
        count = count + 1;              // Add 1 to variable
    }

    while(1);                           // Stay here after counting
}
