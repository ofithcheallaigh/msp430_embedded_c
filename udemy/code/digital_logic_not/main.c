/*
 * The NOT truth table is:
 *  | X | Z |
 *  |-------|
 *  | 1 | 0 |
 *  | 0 | 1 |
 *  |-------|
 *
 *  For byte-wise operations, we have:
 *  The result of the ! byte-wise NOT will be either 0B or 1B
 *  If the ! input is zero, the ! output will be 1B
 *  if the ! input is non-zero, the ! output will be 0B
 */

#include <msp430.h> 

main()
{
    char a = 0b10101101;
    char b = 0b01111111;
    char c = 0b00000000;

    char u,v,w,x,y,z;               // Answers will go here


                                    //   Bit-wise         Byte-wise

    u = ~a;                         // ~ 10101101        ! 10101101
    v = !a;                         // ----------        ----------
                                    //   01010010          00000000

    w = ~b;                         // ~ 01111111        ! 01111111
    x = !b;                         // ----------        ----------
                                    //   10000000          00000000

    y = ~c;                         // ~ 00000000        ! 00000000
    z = !c;                         // ----------        ----------
                                    //   11111111          00000001

    while(1);
}

