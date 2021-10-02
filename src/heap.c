#include <assert.h> /*assert */
#include <stdlib.h> /*malloc */

#include "heap.h"

#define PARENT(child_index) ((child_index - 1) / 2)
#define CHILDREN_LEFT(parent_index) ((parent_index * 2) + 1)
#define CHILDREN_RIGHT(parent_index) ((parent_index * 2) + 2)
#define LAST_ELEMENT(vec) (VectorGetSize(vec) - 1)


struct heap
{
	d_vector_t *d_vector;
    heap_cmp_t cmp_func;
};

static int const SIZE = 10;
static int const FIRST_ELEMENT = 0;


static void HeapifyUp(heap_t *heap, size_t child_idx);
static void HeapifyDown(heap_t *heap, size_t parent_idx, size_t size);
static void SwapElement(d_vector_t *vec, int idx1, int idx2);
static void RemoveElement(heap_t *heap, size_t to_remove);

heap_t *HeapCreate(heap_cmp_t heap_cmp)
{
    heap_t * heap = NULL;
    d_vector_t * vector = NULL;

    assert(heap_cmp != NULL);

    heap = (heap_t *) malloc(sizeof(heap_t));
    if (NULL == heap)
    {
        return NULL;
    }

    vector = VectorCreate(SIZE);
    if (NULL == vector)
    {
        free(heap);
        return NULL;
    }
    heap->cmp_func = heap_cmp;
    heap->d_vector = vector;

    return heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);

    VectorDestroy(heap->d_vector);
    heap->d_vector = NULL;
    free(heap);
}

size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);
    return (VectorGetSize(heap->d_vector));
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);
    return (VectorGetSize(heap->d_vector) == 0);
}

void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);
    if (HeapIsEmpty(heap))
    {
        return (NULL);
    }
    return (VectorGetElement(heap->d_vector, FIRST_ELEMENT));
}

int HeapPush(heap_t *heap, void *data)
{
    status_t status = SUCCESS;

    assert(NULL != heap);

    status = VectorPushBack(heap->d_vector, data);
    if(status == SUCCESS)
    {
        HeapifyUp(heap, LAST_ELEMENT(heap->d_vector));
    }
    
    return status;
}

static void HeapifyUp(heap_t *heap, size_t child_idx)
{
    size_t parent_idx = PARENT(child_idx);
    void *parent_value = NULL;
    void *child_value = NULL;

    if (child_idx == 0)
    {
        return;
    }

    parent_value = VectorGetElement(heap->d_vector, parent_idx);
    child_value = VectorGetElement(heap->d_vector, child_idx);
    if (heap->cmp_func(parent_value, child_value) < 0)
    {
        SwapElement(heap->d_vector, parent_idx, child_idx);
        HeapifyUp(heap, parent_idx);
    }
}

void HeapPop(heap_t *heap)
{
    assert(NULL != heap);

    RemoveElement(heap, 0);
}

static void RemoveElement(heap_t *heap, size_t to_remove)
{
    void *data = NULL;

    data = VectorGetElement(heap->d_vector, LAST_ELEMENT(heap->d_vector));

    VectorSetElement(heap->d_vector, to_remove, data);
    VectorPopBack(heap->d_vector);
    HeapifyDown(heap, to_remove, VectorGetSize(heap->d_vector));
}
static void HeapifyDown(heap_t *heap, size_t parent_idx, size_t size)
{
    size_t max_idx = parent_idx;
    size_t left_child_idx = CHILDREN_LEFT(parent_idx);
    size_t right_child_idx = CHILDREN_RIGHT(parent_idx);
    void *left_child_value = NULL;
    void *right_child_value =  NULL;
    void *max_value =   NULL;

    if (left_child_idx >= size)
    {
        return;
    }

    left_child_value = VectorGetElement(heap->d_vector, left_child_idx);
    right_child_value = VectorGetElement(heap->d_vector, right_child_idx);
    max_value = VectorGetElement(heap->d_vector, max_idx);

    if ((heap->cmp_func(max_value, left_child_value) < 0) && left_child_idx <size)
    {
       max_idx = left_child_idx;
    }

    max_value = VectorGetElement(heap->d_vector, max_idx);

    if ((heap->cmp_func(max_value, right_child_value) < 0) && right_child_idx <size)
    {
       max_idx = right_child_idx;
    }

    if (parent_idx != max_idx)
    {
        SwapElement(heap->d_vector, parent_idx, max_idx);
        HeapifyDown(heap, max_idx, size);
    }
}

static void SwapElement(d_vector_t *vec, int idx1, int idx2)
{
    void *data1 = VectorGetElement(vec, idx1);
    void *data2 = VectorGetElement(vec, idx2);
    VectorSetElement(vec, idx1, data2);
    VectorSetElement(vec, idx2, data1);
}

void *HeapRemove(heap_t *heap, heap_is_match is_match, void * param)
{
    size_t i = 0;
    void *data = NULL;
    size_t size = VectorGetSize(heap->d_vector);

    assert(NULL != heap);
    assert(NULL != is_match);

    for(i = 0; i < size; i++)
    {
        data = VectorGetElement(heap->d_vector, i);
        if (is_match(data, param) == 1)
        {
            RemoveElement(heap, i);
            return data;
        }
    }

    return NULL;
}
void PrintHeap(heap_t *heap)
{
    size_t arr_size = VectorGetSize(heap->d_vector);
    size_t i = 0;
    void *data = NULL;

    for(i = 0; i < arr_size; i++)
    {
        data = VectorGetElement(heap->d_vector, i);
        printf("%d-->", *(int *)&data);
    }
    printf("\n");
}