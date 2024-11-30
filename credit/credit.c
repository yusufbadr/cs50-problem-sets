#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    long n = get_long("Number: ");
    char str[20];
    sprintf(str, "%ld", n);

    // str now contains the number as a string

    int size = (int) strlen(str);
    if (!(size == 13 || size == 15 || size == 16))
    {
        printf("%s", "INVALID\n");
        return 0;
    }

    {
        bool c = true;

        int multipliedDigit, totalSum = 0;
        for (int i = size - 2; i >= 0; i -= 2)
        {
            multipliedDigit = ((int) str[i] - 48) * 2;

            while (multipliedDigit > 0)
            {
                totalSum += multipliedDigit % 10;
                multipliedDigit /= 10;
            }
        }

        for (int i = size - 1; i >= 0; i -= 2)
        {
            totalSum += (int) str[i] - 48;
        }

        if (totalSum % 10 != 0)
        {
            printf("%s", "INVALID\n");
            return 0;
        }
    }

    if (str[0] == '5' && (str[1] == '1' || str[1] == '2' || str[1] == '3' || str[1] == '4' || str[1] == '5'))
    {
        printf("%s", "MASTERCARD\n");
    }
    else if (str[0] == '3' && (str[1] == '4' || str[1] == '7'))
    {
        printf("%s", "AMEX\n");
    }
    else if (str[0] == '4')
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}