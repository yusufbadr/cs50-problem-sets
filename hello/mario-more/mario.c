#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (!(n >= 1 && n <= 8));

    int s = n - 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < s; j++)
        {
            printf(" ");
        }

        for (int k = 0; k < n - s; k++)
        {
            printf("#");
        }

        printf("  ");

        for (int k = 0; k < n - s; k++)
        {
            printf("#");
        }

        s--;

        printf("\n");
    }
}