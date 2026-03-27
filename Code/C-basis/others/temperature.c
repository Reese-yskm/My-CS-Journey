#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    float lower = 0;
    float upper = 300;
    float step = 20;
    float c, fahr;
    fahr = lower;
    while (fahr <= upper)
    {
        c = 5 * (fahr - 32) / 9;
        printf("%3.0f\t%6.1f\n", fahr, c);
        fahr += step;
    }

    return 0;
}