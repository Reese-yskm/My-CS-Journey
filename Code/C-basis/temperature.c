#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int lower = 0;
    int upper = 300;
    int step = 20;
    int c, fahr;
    fahr = lower;
    while (fahr <= upper)
    {
        int c = 5 * (fahr - 32) / 9;
        printf("%d\t%d\n", fahr, c);
        fahr += step;
    }
    

    return 0;
}