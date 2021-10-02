#include <time.h> /* time */
#include <stdlib.h> /* qsort */ 

#include "knights_tour.h"
#include "bitarray.h"

#define N 64
#define TIMEOUT 300

static int cx[SIZE] = { 2, 1, -1, -2, -2, -1, 1, 2 };
static int cy[SIZE] = { 1, 2, 2, 1, -1, -2, -2, -1 };

static int g_move_lut[N][SIZE] = {0};

time_t g_start_time;

typedef struct knight
{
   int index; 
   int degree;
} knight_t;


static void InitLut();
static int IsOutOfBound(int x, int y);
static int GetIndex(int i, int j);
static int RFind(int curr, bitarray_t board, unsigned char *arr, int step);
static void InitAdjacentArray(knight_t arr[SIZE], bitarray_t bitarray, int index);
static int GetDegree(bitarray_t bitarray, int index);
static int FindHeuristic(int curr, bitarray_t board, unsigned char *arr, int step);
static int KnightCmp(const void *data1, const void *data2);
/* static void PrintLut(); */


void FindKnightsPath(unsigned char knight_path[SIZE * SIZE], int first)
{
    bitarray_t board = 0;
    InitLut();

    g_start_time = time(NULL);
    RFind(first, board, knight_path, 0);
}

void FindKnightsHeuristic(unsigned char knight_path[SIZE * SIZE], int first)
{
    bitarray_t board = 0;
    InitLut();

    FindHeuristic(first, board, knight_path, 0);
}

static int RFind(int curr_idx, bitarray_t board, unsigned char *knight_path, int curr_step)
{
    size_t i = 0;
    int status = 0;
    time_t new_time = time(NULL);

    if (board == 0xffffffffffffffff)
    {
        return (1);
    }

    if (BitArrIsBitOn(board, curr_idx))
    {
        return (0);
    }

    if (new_time - g_start_time >= TIMEOUT)
    {
        return (1);
    }

    knight_path[curr_idx] = curr_step;
   
    board = BitArrSetBitOn(board, curr_idx);

    for (i = 0; i < SIZE  && !status; i++)
    {
    
        if (g_move_lut[curr_idx][i] != -1)
        {
           status = RFind( g_move_lut[curr_idx][i], board, knight_path, curr_step +1);
        }
    }

     return (status);
}

static int IsOutOfBound(int x, int y)
{
    return ((x < 0 || y < 0) || (x >= SIZE || y >= SIZE));
}

static int GetIndex( int i, int j)
{
    return ((i * SIZE) + j);
}

static void InitLut()
{
    size_t index = 0;
    size_t i = 0;
    size_t j = 0;
    size_t option = 0;

    for (index = 0; index < N; index++)
    {
       i = index / SIZE;
       j = index % SIZE;

       for (option= 0; option < SIZE; option++)
       {
        if (!IsOutOfBound(i + cx[option], j + cy[option]))
        {
           g_move_lut[index][option] =  GetIndex(i + cx[option], j + cy[option]);
        }
        else
        {
            g_move_lut[index][option] = -1;
        }
        
       }
    }
    
}

static int FindHeuristic(int curr_idx, bitarray_t board, unsigned char *knight_path, int curr_step)
{
    size_t i = 0;
    int status = 0;
    knight_t knights[SIZE] = {0};

    if (board == 0xffffffffffffffff)
    {
        return (1);
    }

    if (BitArrIsBitOn(board, curr_idx))
    {
        return (0);
    }

    knight_path[curr_idx] = curr_step;
   
    board = BitArrSetBitOn(board, curr_idx);

    
    InitAdjacentArray(knights, board, curr_idx);

    for (i = 0; i < SIZE  && !status; i++)
    {
        if (knights[i].index != -1)
        {
           status = FindHeuristic(knights[i].index, board, knight_path, curr_step +1);
        }
    }

     return (status);
}

static void InitAdjacentArray(knight_t knights[SIZE], bitarray_t bitarray, int index)
{
    size_t i = 0;
    size_t degree = 0;
    int adj = 0;

    for (i = 0; i < SIZE; i++)
    {
        adj = g_move_lut[index][i];
        degree = GetDegree(bitarray, adj);
        knights[i].degree = degree;
        knights[i].index = adj;
    }

    qsort(knights, SIZE, sizeof(knight_t), KnightCmp);
    
}
static int GetDegree(bitarray_t bitarray, int index)
{
    size_t i = 0;
    size_t degree = 0;
    int adj = 0;

    if (index == -1)
    {
        return (SIZE +1); 
    }

    for (i = 0; i < SIZE; i++)
    {
        adj = g_move_lut[index][i];
        if (adj != -1 && BitArrIsBitOff(bitarray, adj))
        {
            degree++;
        }
    }

    return (degree);
}


static int KnightCmp(const void *data1, const void *data2)
{
    knight_t *knight1 = (knight_t *) data1;
    knight_t *knight2 = (knight_t *) data2;

    return (knight1->degree - knight2->degree);
}

/* void PrintLut()
{
    int i = 0;
    int j = 0;

    InitLut();
    for(i = 0; i < 64; i++)
    {
        for(j = 0; j < 8; j++)
        {
            printf("  %d  ", g_move_lut[i][j]);
        }
        printf("\n");
    }
} */