main()
{
    int x,y;                            // Create two variables x and y

    y = 0;                              // Set y to be 0

    for(x = 0; x < 10; x = x + 1)       // The FOR loop has 3 parts
    {                                   // 1) Set the control variable
                                        // 2) Loop as long as x < 10
                                        // 3) After every iteration, add 1 to x

        y = y + x;                      // In each iteration, add the value of x to y
    }

    while(1);                           // When the for loop ends, stay here
}
