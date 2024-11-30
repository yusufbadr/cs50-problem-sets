#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n, e, count = 0;

    do
    {
        n = get_int("Start size: ");
    }
    while (n < 9);

    do
    {
        e = get_int("End size: ");
    }
    while (e < n);

    while (n < e)
    {
        n = n + n / 3 - n / 4;
        count++;
    }

    printf("Years: %i\n", count);
}
