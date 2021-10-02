#include <stdio.h> /*printf*/
#include <stdlib.h> /* rand */
#include <time.h> /* time */
#include <string.h> /* strcmp */

#include "sorts.h"

#define RED "\033[5;91m"           /* print in red          */
#define GREEN "\033[1;38;5;47m"    /* print in green        */
#define MAGEN "\033[0;38;5;183m"   /* print in magenta      */
#define YELLOW "\033[1;38;5;220m"  /* print in light yellow */
#define CYAN "\033[0;96m"          /* print in cyan         */
#define LIGHT "\033[2;37m"         /* print in gray         */
#define PURPLE "\033[1;4;38;5;224m"/* print in purple       */
#define RESET "\033[1;0m"          /* reset print color     */
#define WHITE "\033[0m"

#define TEST(name, actual, expected)\
    printf("%s: %s\n\n", name, actual == expected ? GREEN"Pass"WHITE : RED"Fail"WHITE)

#define TEST_FIND(expected)\
    (expected == -1)? printf("Element is not present in array\n")\
                 : printf("Element is present at index %d\n", expected);
    
#define SIZE 5000
    
void TestBubbleSort(int *arr, size_t size);
void TestSelectionSort(int *arr, size_t size);
void TestInsertionSort(int *arr, size_t size);
void TestCountingSort(int *arr, size_t size);
void TestRadixSort(int *arr, size_t size);
void TestQsort(int *arr, size_t size);

void PrintArr(int *arr, size_t size);
int cmpfunc (const void * a, const void * b);
void TestMergeSort();
void TestBinarySearch();
void TestParttion ();
int CmpInt(const void *new_data, const void *curr_data);
int cmpstr(const void* v1, const void* v2);
void TestQuickSort();

int main()
{
  
	/* size_t i = 0;
	int tmp = 0;
	
	int arr[] = {383, 886, 777, 915, 793};

    int random_arr1[SIZE] = {0};
    int random_arr2[SIZE] = {0};
    int random_arr3[SIZE] = {0};
    int random_arr4[SIZE] = {0};
    int random_arr5[SIZE] = {0};
    int random_arr6[SIZE] = {0};
    
    
    for(i = 0; i < SIZE; i++)
    {
    	tmp = rand() % 1000;
    	random_arr1[i] = tmp;
    	random_arr2[i] = tmp;
    	random_arr3[i] = tmp;
    	random_arr4[i] = tmp;
    	random_arr5[i] = tmp;
    	random_arr6[i] = tmp;
    }
    
    TestBubbleSort(random_arr1, SIZE);
    TestSelectionSort(random_arr2, SIZE);
    TestInsertionSort(random_arr3, SIZE);
    TestQsort(random_arr4, SIZE);
	TestCountingSort(random_arr5, SIZE);
	TestRadixSort(random_arr6, SIZE); */

	/* TestBinarySearch(); */
	/* int arr2[] = {1, 4, 5, 2, 3, 6, 7};
	int arr1[] = {0, 1, 5, 2, 3};
	Merge(arr1, arr1, arr1+2, arr1 +4);
	PrintArr(arr1, 5); 
	
	Merge(arr2, arr2, arr2+2, arr2 +6);
	PrintArr(arr2, 7); */
 
	/* TestMergeSort(); */
	/* TestParttion (); */
	 TestQuickSort();
    
    return 0; 
}

void TestRadixSort(int *arr, size_t size)
{
	clock_t start_t, end_t = 0;
	size_t i = 0;
	int g_flag = 0;
	
	printf(YELLOW"\n**********RadixSort**********\n\n"RESET);
	
	start_t = clock();
	RadixSort(arr, size);
	end_t = clock();
	
	for (i = 1;i < size -1; ++i)
	{
		if( arr[i-1] > arr[i])
		{
			g_flag = 1;
		}
	}
	
	TEST("sorting status", g_flag, 0);
	
	printf("End of the sorting, end_t = %ld\n", end_t - start_t);
	
	printf(YELLOW"\n********** END Test**********\n\n"RESET);


}
void TestCountingSort(int *arr, size_t size)
{
	clock_t start_t, end_t = 0;
	size_t i = 0;
	int g_flag = 0;
	
	printf(YELLOW"\n**********CountingSort**********\n\n"RESET);
	
	start_t = clock();
	CountingSort(arr, size);
	end_t = clock();
	
	for (i = 1;i < size -1; ++i)
	{
		if( arr[i-1] > arr[i])
		{
			g_flag = 1;
		}
	}
	
	TEST("sorting status", g_flag, 0);
	
	printf("End of the sorting, end_t = %ld\n", end_t - start_t);
	
	printf(YELLOW"\n********** END Test**********\n\n"RESET);

}

void TestBubbleSort(int *arr, size_t size)
{
	clock_t start_t, end_t = 0;
	size_t i = 0;
	int g_flag = 0;
	
	printf(YELLOW"\n**********Testing BubbleSort**********\n\n"RESET);
	
	start_t = clock();
	BubbleSort(arr,size);
	end_t = clock();
	
	for (i = 1;i < size -1; ++i)
	{
		if( arr[i-1] > arr[i])
		{
			g_flag = 1;
		}
	}
	
	TEST("sorting status", g_flag, 0);
	
	printf("End of the sorting, end_t = %ld\n", end_t - start_t);
	
	printf(YELLOW"\n********** END Test**********\n\n"RESET);
}

void TestSelectionSort(int *arr, size_t size)
{
	clock_t start_t, end_t = 0;
	size_t i = 0;
	int g_flag = 0;
	
		printf(YELLOW"\n**********Testing SelectionSort**********\n\n"RESET);
	
	start_t = clock();
	SelectionSort(arr,size);
	end_t = clock();
	
		for (i = 1;i < size -1; ++i)
	{
		if( arr[i-1] > arr[i])
		{
			g_flag = 1;
		}
	}
	TEST("sorting status", g_flag, 0);
	
	printf("End of the sorting, end_t = %ld\n", end_t - start_t);
	
		printf(YELLOW"\n********** END Test**********\n\n"RESET);
}

void TestInsertionSort(int *arr, size_t size)
{
	clock_t start_t, end_t = 0;
	size_t i = 0;
	int g_flag = 0;
	
	printf(YELLOW"\n**********Testing InsertionSort**********\n\n"RESET);
	
	start_t = clock();
	InsertionSort(arr, size);
	end_t = clock();
	
		
	for (i = 1;i < size -1; ++i)
	{
		if( arr[i-1] > arr[i])
		{
			g_flag = 1;
		}
	}
	TEST("sorting status", g_flag, 0);
	
	
	printf("End of the sorting, end_t = %ld\n", end_t - start_t);
	
	printf(YELLOW"\n********** END Test**********\n\n"RESET);
}

void TestQsort(int *arr, size_t size)
{
	clock_t start_t, end_t = 0;
	
	printf(YELLOW"\n**********Testing quick sort **********\n\n"RESET);
	
	start_t = clock();
	qsort(arr, size, sizeof(int), cmpfunc);
	end_t = clock();
	
	printf("End of the sorting, end_t = %ld\n", end_t - start_t);
	
	printf(YELLOW"\n********** END Test**********\n\n"RESET);

}

int cmpfunc (const void * a, const void * b) 
{
	return ( *(int*)a - *(int*)b );
}

void PrintArr(int *arr, size_t size)
{
    size_t i = 0;
    
    for(i = 0; i < size; i++)
    {
        printf("%d, ", arr[i]);
    }
    printf("\b\b.\n");
}

void TestBinarySearch()
{
	int arr1[] = {2, 3, 4, 10, 40};
	int arr2[] = {-8, 0, 4, 10, 40, 100};
	int res1 = 0;
	int res2 = 0;

	res1 = BinarySearch(arr1, 5, 1);
	TEST_FIND(res1);
	res2 = RecursiveBinarySearch(arr1, 5, 1);
	TEST_FIND(res2);

	res1 = BinarySearch(arr1, 5, 2);
	TEST_FIND(res1);
	res2 = RecursiveBinarySearch(arr1, 5, 2);
	TEST_FIND(res2);

	res1 = BinarySearch(arr1, 5, 40);
	TEST_FIND(res1);
	res2 = RecursiveBinarySearch(arr1, 5, 40);
	TEST_FIND(res2);

	res1 = BinarySearch(arr2, 6, -8);
	TEST_FIND(res1);
	res2 = RecursiveBinarySearch(arr2, 6, -8);
	TEST_FIND(res2);

	res1 = BinarySearch(arr2, 6, -9);
	TEST_FIND(res1);
	res2 = RecursiveBinarySearch(arr2, 6, -9);
	TEST_FIND(res2);
}

void TestMergeSort()
{
	int arr1[] = {8, 6, 1, 5};
	PrintArr(arr1, 4);
	MergeSort(arr1, 4);
	PrintArr(arr1, 4);
}

void TestParttion ()
{
	/* int arr1[] = {8, 2, 4, 1, 7, 5};
	Partition(arr1, arr1, arr1 +5);
	PrintArr(arr1, 6); */
}

void TestQuickSort()
{
    int a[] = {8, 2, 4, 1, 7, 5};
  
    Qsort(a, 6, sizeof(int), CmpInt);
   /*  Qsort(p, 7, sizeof(int), cmpnum); */
  
    PrintArr(a, 6);
   
}

int cmpstr(const void* v1, const void* v2)
{
    char *a1 = *(char**)v1;
    char *a2 = *(char**)v2;
    return strcmp(a1, a2);
}
  
int CmpInt(const void *new_data, const void *curr_data)
{
    return ((*(int *)new_data) - (*(int *)curr_data));
}

