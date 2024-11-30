#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    double l = count_letters(text) / (double) count_words(text) * 100;
    double s = count_sentences(text) / (double) count_words(text) * 100;
    double index = 0.0588 * l - 0.296 * s - 15.8;
    // printf("letters: %d", count_letters(text));
    // printf("sentences: %d", count_sentences(text));
    // printf("words: %d", count_words(text));

    int grade = round(index);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}

int count_words(string text)
{
    int size = strlen(text), count = 1;
    for (int i = 0; i < size; i++)
    {
        if (text[i] == ' ')
        {
            count++;
        }
    }

    return count;
}

int count_sentences(string text)
{
    int size = strlen(text), count = 0;
    for (int i = 0; i < size; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }
    return count;
}

int count_letters(string text)
{
    int size = strlen(text), count = 0;
    for (int i = 0; i < size; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'z'))
        {
            count++;
        }
    }
    return count;
}
