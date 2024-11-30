#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sum = image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue;
            double greyscaleValue = round((double) sum / 3);
            image[i][j].rgbtRed = greyscaleValue;
            image[i][j].rgbtGreen = greyscaleValue;
            image[i][j].rgbtBlue = greyscaleValue;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double r = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            double g = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            double b = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);

            if (r > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = r;
            }

            if (g > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = g;
            }

            if (b > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = b;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // swap image[i][j], image[i][width-j]
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // corners
    int sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            sumR += copy[i][j].rgbtRed;
            sumG += copy[i][j].rgbtGreen;
            sumB += copy[i][j].rgbtBlue;
        }
    }
    image[0][0].rgbtRed = round((double) sumR / 4);
    image[0][0].rgbtGreen = round((double) sumG / 4);
    image[0][0].rgbtBlue = round((double) sumB / 4);

    // --
    sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 1; j < 3; j++)
        {
            sumR += copy[i][width - j].rgbtRed;
            sumG += copy[i][width - j].rgbtGreen;
            sumB += copy[i][width - j].rgbtBlue;
        }
    }
    image[0][width - 1].rgbtRed = round((double) sumR / 4);
    image[0][width - 1].rgbtGreen = round((double) sumG / 4);
    image[0][width - 1].rgbtBlue = round((double) sumB / 4);

    //--
    sumR = 0, sumG = 0, sumB = 0;
    for (int i = 1; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            sumR += copy[height - i][j].rgbtRed;
            sumG += copy[height - i][j].rgbtGreen;
            sumB += copy[height - i][j].rgbtBlue;
        }
    }
    image[height - 1][0].rgbtRed = round((double) sumR / 4);
    image[height - 1][0].rgbtGreen = round((double) sumG / 4);
    image[height - 1][0].rgbtBlue = round((double) sumB / 4);

    // --
    sumR = 0, sumG = 0, sumB = 0;
    for (int i = 1; i < 3; i++)
    {
        for (int j = 1; j < 3; j++)
        {
            sumR += copy[height - i][width - j].rgbtRed;
            sumG += copy[height - i][width - j].rgbtGreen;
            sumB += copy[height - i][width - j].rgbtBlue;
        }
    }
    image[height - 1][width - 1].rgbtRed = round((double) sumR / 4);
    image[height - 1][width - 1].rgbtGreen = round((double) sumG / 4);
    image[height - 1][width - 1].rgbtBlue = round((double) sumB / 4);
    // -- END OF CORNERS --

    // processing edge rows/col

    // left
    sumR = 0, sumG = 0, sumB = 0;
    for (int i = 1; i < height - 1; i++)
    {

        sumR = copy[i - 1][0].rgbtRed + copy[i - 1][1].rgbtRed + copy[i][0].rgbtRed + copy[i][1].rgbtRed + copy[i + 1][0].rgbtRed +
               copy[i + 1][1].rgbtRed;

        sumG = copy[i - 1][0].rgbtGreen + copy[i - 1][1].rgbtGreen + copy[i][0].rgbtGreen + copy[i][1].rgbtGreen +
               copy[i + 1][0].rgbtGreen + copy[i + 1][1].rgbtGreen;

        sumB = copy[i - 1][0].rgbtBlue + copy[i - 1][1].rgbtBlue + copy[i][0].rgbtBlue + copy[i][1].rgbtBlue +
               copy[i + 1][0].rgbtBlue + copy[i + 1][1].rgbtBlue;

        image[i][0].rgbtRed = round((double) sumR / 6);
        image[i][0].rgbtGreen = round((double) sumG / 6);
        image[i][0].rgbtBlue = round((double) sumB / 6);
    }

    // right
    sumR = 0, sumG = 0, sumB = 0;
    for (int i = 1; i < height - 1; i++)
    {

        sumR = copy[i - 1][width - 1].rgbtRed + copy[i - 1][width - 2].rgbtRed + copy[i][width - 1].rgbtRed +
               copy[i][width - 2].rgbtRed + copy[i + 1][width - 1].rgbtRed + copy[i + 1][width - 2].rgbtRed;

        sumG = copy[i - 1][width - 1].rgbtGreen + copy[i - 1][width - 2].rgbtGreen + copy[i][width - 1].rgbtGreen +
               copy[i][width - 2].rgbtGreen + copy[i + 1][width - 1].rgbtGreen + copy[i + 1][width - 2].rgbtGreen;

        sumB = copy[i - 1][width - 1].rgbtBlue + copy[i - 1][width - 2].rgbtBlue + copy[i][width - 1].rgbtBlue +
               copy[i][width - 2].rgbtBlue + copy[i + 1][width - 1].rgbtBlue + copy[i + 1][width - 2].rgbtBlue;

        image[i][width - 1].rgbtRed = round((double) sumR / 6);
        image[i][width - 1].rgbtGreen = round((double) sumG / 6);
        image[i][width - 1].rgbtBlue = round((double) sumB / 6);
    }

    // top
    sumR = 0, sumG = 0, sumB = 0;
    for (int i = 1; i < height - 1; i++)
    {

        sumR = copy[0][i - 1].rgbtRed + copy[0][i].rgbtRed + copy[0][i + 1].rgbtRed + copy[1][i - 1].rgbtRed + copy[1][i].rgbtRed +
               copy[1][i + 1].rgbtRed;

        sumG = copy[0][i - 1].rgbtGreen + copy[0][i].rgbtGreen + copy[0][i + 1].rgbtGreen + copy[1][i - 1].rgbtGreen +
               copy[1][i].rgbtGreen + copy[1][i + 1].rgbtGreen;

        sumB = copy[0][i - 1].rgbtBlue + copy[0][i].rgbtBlue + copy[0][i + 1].rgbtBlue + copy[1][i - 1].rgbtBlue +
               copy[1][i].rgbtBlue + copy[1][i + 1].rgbtBlue;

        image[0][i].rgbtRed = round((double) sumR / 6);
        image[0][i].rgbtGreen = round((double) sumG / 6);
        image[0][i].rgbtBlue = round((double) sumB / 6);
    }

    // bottom
    sumR = 0, sumG = 0, sumB = 0;
    for (int i = 1; i < height - 1; i++)
    {

        sumR = copy[height - 1][i - 1].rgbtRed + copy[height - 1][i].rgbtRed + copy[height - 1][i + 1].rgbtRed +
               copy[height - 2][i - 1].rgbtRed + copy[height - 2][i].rgbtRed + copy[height - 2][i + 1].rgbtRed;

        sumG = copy[height - 1][i - 1].rgbtGreen + copy[height - 1][i].rgbtGreen + copy[height - 1][i + 1].rgbtGreen +
               copy[height - 2][i - 1].rgbtGreen + copy[height - 2][i].rgbtGreen + copy[height - 2][i + 1].rgbtGreen;

        sumB = copy[height - 1][i - 1].rgbtBlue + copy[height - 1][i].rgbtBlue + copy[height - 1][i + 1].rgbtBlue +
               copy[height - 2][i - 1].rgbtBlue + copy[height - 2][i].rgbtBlue + copy[height - 2][i + 1].rgbtBlue;

        image[height - 1][i].rgbtRed = round((double) sumR / 6);
        image[height - 1][i].rgbtGreen = round((double) sumG / 6);
        image[height - 1][i].rgbtBlue = round((double) sumB / 6);
    }

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            sumR = copy[i - 1][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j - 1].rgbtRed +
                   copy[i][j].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j - 1].rgbtRed + copy[i + 1][j].rgbtRed +
                   copy[i + 1][j + 1].rgbtRed;

            sumG = copy[i - 1][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen +
                   copy[i][j - 1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i + 1][j - 1].rgbtGreen +
                   copy[i + 1][j].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;

            sumB = copy[i - 1][j - 1].rgbtBlue + copy[i - 1][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j - 1].rgbtBlue +
                   copy[i][j].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i + 1][j - 1].rgbtBlue + copy[i + 1][j].rgbtBlue +
                   copy[i + 1][j + 1].rgbtBlue;

            image[i][j].rgbtRed = round((double) sumR / 9);
            image[i][j].rgbtGreen = round((double) sumG / 9);
            image[i][j].rgbtBlue = round((double) sumB / 9);
        }
    }

    return;
}
