#include "./headers/coloredPrints.h"

void changePrintColor(int color)
{
    if (color == -1)
    {
        printf("\033[0m");
    }
    else
    {
        printf("\033[1;3%dm", color);
    }
}
