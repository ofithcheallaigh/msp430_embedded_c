// ******************************************************
// Function prototype
// ******************************************************
signed int max_of(signed int, signed int);


main()
{
    signed int a, b, first, second, output;

    a = 5;
    b = -2;

    first = -4;
    second = 13219;

    output = max_of(first,second);

    while(1);
}

// *****************************************************
// Function definition
// *****************************************************
signed int max_of(signed int a, signed int b)
{
    if(a > b)               // If a > b
    {
        return a;           // Then return a, and immediate
                            // leave the max_of function
    }

    return b;               // You only get here if a > b is false
                            // so return b and leave the max_of function
}
