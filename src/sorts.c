#include <assert.h> /*assert */
#include <stdlib.h> /*malloc*/
#include <sys/types.h> /*ssize_t*/
#include <string.h> /*ssize_t*/

#include "sorts.h"

static void Swap(int *a, int * b); 
static int FindMax(int *arr, size_t size);
static int FindMin(int *arr, size_t size);
static void CopyArr(int *src, int *dest, size_t size);
static void CountSortDigit(int *arr, size_t size, size_t digit);
static size_t FindMaxDigits(int *arr, size_t size);
static size_t CountDigits (int num);
static int InternalBinarySearch(int *arr, int from, int to, int to_find);
int InternalMergeSort(int *arr_to_sort, int * left, int * right);
int Merge(int *start1, int *middle, int *end);
static void *Partition(void *arr, size_t nitems, size_t size, int (*compare)(const void *, const void*));
void GenericSwap(void *p1, void *p2, size_t size);
void GenericQSort(void *arr, int low, int high, int (*compare)(const void *, const void*),size_t size );
void Qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*));

void BubbleSort(int *arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	
	assert(NULL != arr);
	
	for (i = 0 ; i < size; i++)
	{
		for (j = 0 ; j < size - i -1 ; j++)
		{
			if (arr[j] > arr[j+1])
			{
				Swap(&arr[j], &arr[j+1]);
			}
		}
	}
}

void InsertionSort(int *arr, size_t size)
{
	size_t position = 0;
	size_t i = 0;
	int value = 0;
	
	assert(NULL != arr);
	for (i = 1; i < size; i++)
	{
		position = i;
		value = arr[i];
		
		while (position > 0 && value < arr[position - 1])
		{
			arr[position] = arr[position -1]; /* shift right */
			position = position -1;
		}
		
		arr[position] = value;
	}
}

void SelectionSort(int *arr, size_t size)
{
	size_t min_idx = 0;
	size_t i = 0;
	size_t j = 0;
	
	assert(NULL != arr);
	
	for (i = 0 ; i < size; i++)
	{
		min_idx = i;
		
		for (j = i ; j < size ; j++)
		{
			if (arr[j] < arr[min_idx])
			{
				min_idx = j;
			}
		}
		
		Swap(&arr[i], &arr[min_idx]);
	}	
}

void CountingSort(int *arr, size_t size)
{
	int max = 0;
	int min = 0;
	ssize_t i = 0;
	size_t size_of_count_arr = 0;
	int *count_arr = NULL;
	int *sorted_arr = NULL;
	 
	assert(NULL != arr);
	
	min = FindMin(arr, size);
	max = FindMax(arr, size);
	
	size_of_count_arr = max - min +1;
	count_arr = calloc(size_of_count_arr, sizeof(int));
	
	if (NULL == count_arr)
	{
		return;
	}
	
	for(i = 0; i < size; i++)
	{
		count_arr[arr[i] - min]++;
	}
	
	for(i = 1; i < size_of_count_arr; i++)
	{
		count_arr[i] = count_arr[i-1] + count_arr[i];
	}
	
	sorted_arr = malloc(size * sizeof(int));
	
	if (NULL == sorted_arr)
	{
		return;
	}
	
	for(i = size -1; i >= 0; i--)
	{
		 count_arr[arr[i] - min]--;
		 sorted_arr[count_arr[arr[i] - min]] = arr[i];
	}
	
	CopyArr(sorted_arr, arr, size);
	
	free(count_arr);
	free(sorted_arr);
}

void RadixSort(int *arr, size_t size)
{
	ssize_t max_digits = 0;
	size_t dig = 1;
	
	assert(NULL != arr);
	
	max_digits = FindMaxDigits(arr, size);
	
	for (dig = 1; max_digits >= 0; dig *= 10)
	{
		CountSortDigit(arr, size, dig);
		max_digits--;
	}

}
	
static void CountSortDigit(int *arr, size_t size, size_t digit) 
{
	ssize_t i = 0;
	int count_arr[10] = {0};
	int *sorted_arr = NULL;
	

	sorted_arr = malloc(size * sizeof(int));
	
	if (NULL == sorted_arr)
	{
		return;
	}
	
	for (i = 0; i < size; i++)
	{
		count_arr[(arr[i] / digit) % 10]++;
	}
	
	for (i = 1; i < 10; i++)
	{
		count_arr[i] = count_arr[i-1] + count_arr[i];
	}
	
	
	for (i = size -1; i >= 0; i--) 
	{
		count_arr[(arr[i] / digit) % 10]--;
		sorted_arr[count_arr[(arr[i] / digit) % 10]] = arr[i];
	}
	
	CopyArr(sorted_arr, arr, size);
	free(sorted_arr);
}

static size_t FindMaxDigits(int *arr, size_t size)
{
	int max = 0;
	size_t i = 0;
	size_t tmp = 0;
	
	for(i = 0; i < size; i++)
	{
		tmp = CountDigits(arr[i]);
		if (tmp > max)
		{
			max = tmp;
		}
	}
	
	return max;
}
static size_t CountDigits (int num)
{
	size_t count = 0;

	while (num != 0) 
	{
		num /= 10;     
		++count;
    }
    
    return count;
}

static void CopyArr(int *src, int *dest, size_t size)
{
	size_t i = 0;
	for(i = 0; i < size; i++)
    {
        dest[i] = src[i];
    }
}

static int FindMax(int *arr, size_t size)
{
	int max = 0;
	size_t i = 0;
	
	assert(NULL != arr);
	
	max = arr[0];
	
	for(i = 1; i < size; i++)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	
	return max;
}

static int FindMin(int *arr, size_t size)
{
	int min = 0;
	size_t i = 0;
	
	assert(NULL != arr);
	
	min = arr[0];
	
	for(i = 1; i < size; i++)
	{
		if (arr[i] < min)
		{
			min = arr[i];
		}
	}
	
	return min;
}

static void Swap(int *a, int *b)
{
	int tmp = 0;
	
	assert(NULL != a);
	assert(NULL != b);
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

int BinarySearch(int *arr, size_t size, int to_find)
{
	int from = 0;
	int middle = 0;
	int to = size;


	while (from <= to)
	{
		middle = (from + to) / 2;

		if(arr[middle] == to_find)
		{
			return middle;
		}

		if (to_find < arr[middle])
		{
			to = middle - 1;
		}

		else
		{
			from = middle + 1;
		}
	}

	return (-1);
	
}

int RecursiveBinarySearch(int *arr, size_t size, int to_find)
{
	return (InternalBinarySearch(arr, 0, size, to_find));

}

static int InternalBinarySearch(int *arr, int from, int to, int to_find)
{
	int middle = (from + to) / 2;
	if (from > to)
	{
		return -1;
	}

	if(arr[middle] == to_find)
	{
		return middle;
	}

	if (to_find < arr[middle])
	{
		return (InternalBinarySearch(arr, from, middle -1, to_find));
	}
	else
	{
		 return (InternalBinarySearch(arr, middle + 1, to, to_find));
	}
}

int MergeSort(int *arr_to_sort, size_t num_elements)
{
	int *right = arr_to_sort + (num_elements -1);
	InternalMergeSort(arr_to_sort, arr_to_sort, right);
}

int Merge(int *start1, int *middle, int *end)
{
	int * start2 = middle +1;
	ssize_t size = end - start1 +1;
	int * tmp = NULL;
	int * tmp_begin = NULL;
	int *tmp_begin1 = NULL;
	int * new_arr = start1;


	tmp = malloc(sizeof(int) * size);
	if (NULL == tmp)
	{
		return (-1);
	}
	tmp_begin = tmp;
	tmp_begin1 = tmp;

	while (start1 <= middle && start2 <= end)
	{
		if (*start1 <= *start2)
		{
			*tmp = *start1;
			start1++;
		}
		else
		{
			*tmp = *start2;
			start2++;
		}

		tmp++;
	}
	
	while (start1 <= middle)
	{
		*tmp = *start1;
		start1++;
		tmp++;
	}

	while (start2 <= end)
	{
		*tmp = *start2;
		start2++;
		tmp++;
	}

	while (size != 0)
	{
		*new_arr = *tmp_begin;
		new_arr++;
		tmp_begin++;
		size--;
	}

	free(tmp_begin1);
	
}

int InternalMergeSort(int *arr_to_sort, int * left, int * right)
{
	int * mid = NULL;
	int status = 0;
	if ( left >= right)
	{
		return 0;
	}

	mid = left + ((right - left) /2);

	InternalMergeSort(arr_to_sort, left, mid);
	InternalMergeSort(arr_to_sort, mid + 1, right);
	
	status = Merge(left, mid, right);
	return status;
}


void Qsort(void *base, size_t nitems, size_t size, int (*compare)(const void *, const void*))
{
	void *pivot = NULL;
	size_t sub_arr_size = 0 ;
	if (nitems < 2)
	{
		return;
	}

	pivot = Partition(base, nitems, size, compare);
	sub_arr_size = ((char *)pivot - (char *)base) / size;
	Qsort(base, sub_arr_size, size, compare);
	Qsort((char *)pivot + size, nitems - sub_arr_size - 1, size, compare);
}

static void *Partition(void *arr, size_t nitems, size_t size, int (*compare)(const void *, const void*))
{
    void *pivot = arr;
    int compare_l = 0, compare_r = 0;
    char *left = (char *)arr + size, *right = (char *)arr + ((nitems - 1) * size);

    while (right > left)
    {
        compare_r = compare(pivot, right);
        compare_l = compare(pivot, left);

        if (compare_r <= 0)
        {
            right -= size;
        }
        
        else if (compare_l > 0)
        {
            left += size;
        }
        
        else
        {
            GenericSwap(left, right, size);
        }
    }

    if (compare(pivot, right) < 0)
    {
        right -= size;
    }

    GenericSwap(pivot, right, size);

    return (right);
}

void GenericSwap(void *p1, void *p2, size_t size)
{
	char tmp = 0;
	char *ch_p1 = (char *)p1;
	char *ch_p2 = (char *)p2;
	
	while (size != 0)
	{
		memcpy(&tmp, ch_p1, 1);
		*ch_p1 = *ch_p2;
		*ch_p2 = tmp;
		++ch_p1;
		++ch_p2;
		--size;
	}
}
