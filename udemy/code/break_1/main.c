// #include <msp430.h>

main()
{
    int x = 0;
    int alarm = 0;

    while(x < 10000)
    {
        x = x + 1;

        if(alarm == 4)
        {
            break;
        }
    }
    while(1);
}
