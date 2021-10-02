#ifndef __SORTS_H__
#define __SORTS_H__

#include <stddef.h>  /* size_t */
#include <sys/types.h> /* ssize_t */

void BubbleSort(int *arr, size_t size);
void InsertionSort(int *arr, size_t size);
void SelectionSort(int *arr, size_t size);
void CountingSort(int *arr, size_t size);
void RadixSort(int *arr, size_t size);
int BinarySearch(int *arr, size_t size, int to_find);
int RecursiveBinarySearch(int *arr, size_t size, int to_find);
int MergeSort(int *arr_to_sort, size_t num_elements);
void Qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));

#endif /* __SORTS_H__ */
