#include <stdio.h> /*printf*/
#include "knights_tour.h"
#include "utilities.h"

void PrintPath(unsigned char res[64]);

unsigned char arr[SIZE * SIZE] = {0};
int main ()
{
    FindKnightsHeuristic(arr, 17);
    PrintPath(arr);
    return 0;
}

void PrintPath(unsigned char res[64])
{
    size_t i = 0;
    size_t j = 0;

    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            printf("%2d, ", (int)(res[i*8 + j]));
        }
        printf("\b\b.\n");
    }
}