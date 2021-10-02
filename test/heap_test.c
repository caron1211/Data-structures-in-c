#include <stdio.h> /*printf*/
#include "heap.h"
#include "utilities.h"

int CmpInt(const void *new_data, const void *curr_data);
int IntIsMatch(const void *data1, void *data2);

void TestCreateDestroy();
void TestPush();
void TestSize();
void TestIsEmpty();
void TestPopPeek();
void TestRemove();



int main ()
{
    TestCreateDestroy();
    TestPush();
    TestSize();
    TestIsEmpty();
    TestPopPeek();
    TestRemove();
    return 0;
}

void TestCreateDestroy()
{
    heap_t *heap = HeapCreate(CmpInt);
    HeapDestroy(heap);
}

void TestPush()
{
    heap_t *heap = HeapCreate(CmpInt);
    size_t i = 0;

    for (i = 0; i <= 20; i++)
    {
       HeapPush(heap, (void *) i);
    }
    TEST_IS_EQUAL("TestPush", HeapPeek(heap), (void *)20);
    PrintHeap(heap);
    
    HeapDestroy(heap);
}

void TestIsEmpty()
{
    heap_t *heap = HeapCreate(CmpInt);

    TEST_IS_TRUE("TestIsEmpty", HeapIsEmpty(heap));
    HeapPush(heap, (void *) 8);
    TEST_IS_FALSE("TestIsEmpty", HeapIsEmpty(heap));
    HeapDestroy(heap);
}

void TestPopPeek()
{
    heap_t *heap = HeapCreate(CmpInt);
    size_t i = 0;

    for (i = 0; i <= 8; i++)
    {
       HeapPush(heap, (void *) i);
    }
       
   for (i = 8; i > 0; i--)
    {
        HeapPop(heap);
        TEST_IS_EQUAL("HeapPop",  HeapPeek(heap),(void *) (i-1));
    }

    HeapDestroy(heap);
}

void TestRemove()
{
    heap_t *heap = HeapCreate(CmpInt);
    size_t i = 0;

    for (i = 0; i <= 8; i++)
    {
       HeapPush(heap, (void *) i);
    }

    HeapRemove(heap, IntIsMatch, (void *) 3);

    PrintHeap(heap);

    HeapDestroy(heap);
}

void TestSize()
{
    heap_t *heap = HeapCreate(CmpInt);

    TEST_IS_EQUAL("TestSize", HeapSize(heap), 0);
    HeapPush(heap, (void *) 7);
    HeapPush(heap, (void *) 5);
    HeapPush(heap, (void *) 8);
    TEST_IS_EQUAL("TestSize", HeapSize(heap), 3);

    HeapDestroy(heap);
}
int CmpInt(const void *new_data, const void *curr_data)
{
    return ((*(int **)&new_data) - (*(int **)&curr_data));
}

int IntIsMatch(const void *data1, void *data2)
{
    return (*(int**)&(data1) == *(int**)&(data2));
}

