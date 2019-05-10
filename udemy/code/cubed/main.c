#include <msp430.h>                     // Need this to easily stop watchdog

// **************************************************************************
// Function prototype
// **************************************************************************
signed int cubed(signed char);          // Has an input of type signed char
                                        // Has an output type signed int

// **************************************************************************
// Main program is here
// **************************************************************************
main()
{
    WDTCTL = WDTPW | WDTHOLD;          // Stop watchdog timer

    signed char q = 6;                  // This will be the function input

    signed int answer = 0;              // We will store the result of the
                                        // cubed function in this variable

    answer = cubed(q);                  // This is the function call. it
                                        // sends the value of 6 to the function
                                        // and will put the functions output
                                        // into the variable called answer

    while(1);
} // End main


signed int cubed(signed char abc)         // Has an input of type signed char that
{                                       // will be called "abc" in the function
                                        // It also has an output of type signed int

    return abc * abc * abc;             // Send contents of result back to answer
}
