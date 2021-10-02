#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */
#include "cbuff.h"

#define RED "\33[1;31m"
#define GREEN "\33[1;32m"
#define WHIT "\33[0m"
#define MAG "\33[1;96m"

#define TEST_IS_EQUAL(test_name, expected, real)\
			(real == expected ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))

#define TEST_IS_TRUE(test_name, result)\
			(result ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))

#define TEST_IS_FALSE(test_name, result)\
			(!result ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))		

static void TestCreateDestroy();
static void TestWrite();
static void TestRead();
static void TestReadFromEmptyCbuff();
static void TestWriteToFullCbuff();

static cbuff_t *FillCbuff();
static cbuff_t *CreateEmptyCbuff();
static cbuff_t *FullCbuff();
int main()
{
	TestCreateDestroy();
	TestWrite();
	TestRead();
	TestReadFromEmptyCbuff();
	TestWriteToFullCbuff();
	return 0;
}

static void TestCreateDestroy()
{
	cbuff_t *cbuff = CreateEmptyCbuff();
	TEST_IS_TRUE("check the buffer is empty", CbuffIsEmpty(cbuff));
	CbuffDestroy(cbuff);
}

static void TestWrite()
{
	ssize_t res = 0;
	cbuff_t *cbuff = NULL;
	char arr[5] = {'a'};
	
	cbuff = CreateEmptyCbuff();
	res = CbuffWrite(cbuff, arr, 2);
	TEST_IS_EQUAL("check write 2", res, 2);
	
	TEST_IS_FALSE("check the buffer is not empty", CbuffIsEmpty(cbuff));
	TEST_IS_EQUAL("check the free space is 23", CbuffFreeSpace(cbuff), 23);
	 /* printf(" free space %d \n", CbuffFreeSpace(cbuff)); */
	TEST_IS_EQUAL("check the count is 2", CbuffCountFull(cbuff), 2);
	 /* printf("count full %d \n", CbuffCountFull(cbuff)); */
	 
	 CbuffDestroy(cbuff);
}


static void TestRead()
{
	int res = 0;
	cbuff_t *cbuff = NULL;
	int arr[4] = {1};
	cbuff = FillCbuff();
	
	res += CbuffRead(cbuff, arr, sizeof(int));
	
	TEST_IS_EQUAL("read int from cbuff", arr[0], 11);
	
	res += CbuffRead(cbuff, arr + 1 ,  2 * sizeof(int));

	TEST_IS_EQUAL("read 2 int from cbuff", arr[2], 13);
	
	CbuffDestroy(cbuff);
}

static void TestReadFromEmptyCbuff()
{
	cbuff_t *cbuff = NULL;
	int byte_read = 0;
	int arr[4] = {1};
	cbuff = CreateEmptyCbuff();
	
	byte_read = CbuffRead(cbuff, arr, sizeof(int));
	
	TEST_IS_EQUAL("read from empty cbuff", byte_read, 0);
	
	CbuffDestroy(cbuff);
}

static void TestWriteToFullCbuff()
{
	cbuff_t *cbuff = NULL;
	char *arr = NULL;
	int byte_write = 0;
	cbuff = FullCbuff();

	arr = malloc(sizeof(char));
	
	arr[0] = 'e';
	
	byte_write = CbuffWrite(cbuff, arr, sizeof(char));
	TEST_IS_EQUAL("write to full cbuff", byte_write, 0);
	free(arr);
	
	CbuffDestroy(cbuff);
}

static cbuff_t *FillCbuff()
{
	cbuff_t *cbuff = NULL;
	int *arr = malloc(sizeof(int) * 3);
	if (NULL == arr)
	{
		return NULL;
	}
	
	arr[0] = 11;
	arr[1] = 12;
	arr[2] = 13;
	
	cbuff = CbuffCreate(25);
	CbuffWrite(cbuff, arr, 3 * sizeof(int));
	
	free(arr);
	return cbuff;
}

static cbuff_t *CreateEmptyCbuff()
{
	cbuff_t *cbuff = CbuffCreate(25);
	return cbuff;
}

static cbuff_t *FullCbuff()
{
	char *arr = NULL;
	cbuff_t *cbuff = CbuffCreate(5);
	
	arr = malloc(sizeof(char) * 5);
	if (NULL == arr)
	{
		return NULL;
	}
	
	arr[0] = 'a';
	arr[1] = 'b';
	arr[2] = 'c';
	arr[3] = 'd';
	arr[4] = 'e';
	
	CbuffWrite(cbuff, arr, 5 * sizeof(char));
	free(arr);
	
	return cbuff;
}

