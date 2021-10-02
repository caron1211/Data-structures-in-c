#include <stdio.h> /*printf*/
#include <string.h>
#include <stdlib.h> /*malloc */

#include "srtlist.h"
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
			
#define TEST_NOT_END(test_name, itr, list)\
			(itr != SrtListEnd(list) ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))			
			
#define TEST_IS_END(test_name, itr, list)\
			(itr == SrtListEnd(list) ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))
			
			
#define PRINT_ELEMENTS(list, print_func){\
			SrtListForeach(SrtListBegin(list), SrtListEnd(list),print_func,(void *) 1);\
			printf("\n");}
			
			
static int IntCmp(void *num1, void *num2);
static int printInt(void *data, void *param);
static int StrCmp(void *str1, void *str2);
static int printStr(void *data, void *param);
static int IntIsMatch(void *data, void *param);
static int StrIsMatch(void *data, void *param);


static void CreateAndDestroyTest();
static void InsertTest();
static void IsEmptyTest();
static void CountTest();
static void FindAndRemoveTest();
static void FindTest();
static void MergeTest1();
static void MergeTest2();

int main()
{
	CreateAndDestroyTest();
	InsertTest();
	IsEmptyTest();
	CountTest();
	FindAndRemoveTest();
	FindTest();
	MergeTest1();
	MergeTest2();
	
	return 0;
}

static void CreateAndDestroyTest()
{
	srtlist_t *list = NULL;
	
	printf(MAG"CreateAndDestroyTest\n"WHIT);
	list = SrtListCreate(IntCmp);
	TEST_IS_TRUE("CreateAndDestroyTest", list);

	SrtListDestroy(list); 
}

static void InsertTest()
{
	srtlist_t *list_int = NULL;
	srtlist_t *list_str = NULL;
	int a = 100;
	int b = 200;
	int c = 50;
	
	char * str1 = "bb";
	char * str2 = "aa";
	char * str3 = "ab";
	
	printf(MAG"InsertTest\n"WHIT);
	list_int = SrtListCreate(IntCmp);
	SrtListInsert(list_int, &a);
	SrtListInsert(list_int, &b);
	SrtListInsert(list_int, &c);
	PRINT_ELEMENTS(list_int, printInt);
	
	TEST_IS_EQUAL("test",*(int *)SrtListPopFront(list_int), c);
	TEST_IS_EQUAL("test",*(int *)SrtListPopFront(list_int), a);

	SrtListDestroy(list_int); 
	
	list_str = SrtListCreate(StrCmp);
	SrtListInsert(list_str, str1);
	SrtListInsert(list_str, str2);
	SrtListInsert(list_str, str3);
	PRINT_ELEMENTS(list_str, printStr);
	
	TEST_IS_EQUAL("test",(char *)SrtListPopFront(list_str), str2);
	TEST_IS_EQUAL("test",(char *)SrtListPopFront(list_str), str3);

	SrtListDestroy(list_str); 
}

static void IsEmptyTest()
{
	srtlist_t *list = NULL;
	int a = 100;
	
	printf(MAG"IsEmptyTest\n"WHIT);
	list = SrtListCreate(IntCmp);
	
	TEST_IS_TRUE("empty test case 1", SrtListIsEmpty(list));
	SrtListInsert(list, &a);
	TEST_IS_FALSE("empty test case 2", SrtListIsEmpty(list));
	
	
	SrtListDestroy(list); 
}

static void CountTest()
{
	srtlist_t *list = NULL;
	
	int a = 100;
	int b = 200;
	
	printf(MAG"CountTest\n"WHIT);
	list = SrtListCreate(IntCmp);
	
	TEST_IS_EQUAL("CountTest case 1", SrtListSize(list), 0);
	
	SrtListInsert(list, &a);
	TEST_IS_EQUAL("CountTest case 2", SrtListSize(list), 1);
	
	SrtListInsert(list, &b);	
	TEST_IS_EQUAL("CountTest case 3", SrtListSize(list), 2);
	
	SrtListDestroy(list); 
}

static void FindAndRemoveTest()
{
	srtlist_t *list = NULL;
	srtlist_iter_t itr = NULL;
	
	int a = 100;
	int b = 200;
	int c = 50;
	
	printf(MAG"FindAndRemoveTest\n"WHIT);
	list = SrtListCreate(IntCmp);
	
	SrtListInsert(list, &a);
	SrtListInsert(list, &b);
	SrtListInsert(list, &c);
	
	itr = SrtListFindIf(SrtListBegin(list), SrtListEnd(list), IntIsMatch, &b);
	TEST_NOT_END("FindTest", itr, list);
	TEST_IS_EQUAL("FindTest",*(int*)SrtListGetData(itr), b);
	
	itr = SrtListRemove(itr);
	TEST_IS_END("FindTest", itr, list);
	
	itr = SrtListRemove(SrtListBegin(list));
	itr = SrtListRemove(SrtListBegin(list));
	
	TEST_IS_END("RemoveTest", itr, list);
	
	SrtListDestroy(list); 
}

static void FindTest()
{
	srtlist_t *list = NULL;
	srtlist_iter_t itr = NULL;
	char *to_find = NULL;
	
	char *a = "aa";
	char *b = "bb";
	char *c = "cc";
	
	to_find = (char *) malloc(sizeof(char *) * 3);
	to_find[0] = 'b';
	to_find[1] = 'b';
	to_find[2] = '\0';
	

	printf(MAG"FindTest\n"WHIT);
	list = SrtListCreate(StrCmp);
	
	SrtListInsert(list, a);
	SrtListInsert(list, b);
	SrtListInsert(list, c);
	
	itr = SrtListFindIf(SrtListBegin(list), SrtListEnd(list), StrIsMatch, to_find);
	TEST_NOT_END("FindTest", itr, list);
	
	itr = SrtListFind(SrtListBegin(list), SrtListEnd(list), to_find);
	TEST_IS_END("FindTest", itr, list);
	
	
	SrtListDestroy(list); 
	
}

static void MergeTest1()
{
	srtlist_t *list1 = NULL;
	srtlist_t *list2 = NULL;
	
	int a = 1;
	int b = 7;
	int c = 8;
	int d = 12;
	
	int e = 0;
	int f = 1;
	int g = 12;
	int h = 13;
	int i = 14;
	
	printf(MAG"MergeTest\n"WHIT);
	list1 = SrtListCreate(IntCmp);
	list2 = SrtListCreate(IntCmp);
	
	SrtListInsert(list1, &a);
	SrtListInsert(list1, &b);
	SrtListInsert(list1, &c);
	SrtListInsert(list1, &d);
	
	
	SrtListInsert(list2, &e);
	SrtListInsert(list2, &f);
	SrtListInsert(list2, &g);
	SrtListInsert(list2, &h);
	SrtListInsert(list2, &i);
	
	SrtListMerge(list1, list2);
	
	PRINT_ELEMENTS(list1, printInt);
	TEST_IS_TRUE("MergeTest", SrtListIsEmpty(list2));
	TEST_IS_EQUAL("MergeTest", SrtListSize(list1), 9);

	
	TEST_IS_EQUAL("test",*(int *)SrtListPopFront(list1), e);
	TEST_IS_EQUAL("test",*(int *)SrtListPopBack(list1), i);
	
	SrtListDestroy(list1);
	SrtListDestroy(list2);  
}

static void MergeTest2()
{
	srtlist_t *list1 = NULL;
	srtlist_t *list2 = NULL;
	
	char * a = "a";
	char * c = "c";
	char * d = "d";
	
	char * b = "b";
	char * e = "e";
	
	
	printf(MAG"MergeTest\n"WHIT);
	list1 = SrtListCreate(StrCmp);
	list2 = SrtListCreate(StrCmp);

	SrtListInsert(list1, a);
	SrtListInsert(list1, c);
	SrtListInsert(list1, d);
	
	PRINT_ELEMENTS(list1, printStr);
	
	SrtListInsert(list2, b);
	SrtListInsert(list2, e);
	
	PRINT_ELEMENTS(list2, printStr);
	
	SrtListMerge(list1, list2);
	
	PRINT_ELEMENTS(list1, printStr);
	TEST_IS_TRUE("MergeTest", SrtListIsEmpty(list2));
	TEST_IS_EQUAL("MergeTest", SrtListSize(list1), 5);

	
	
}
static int IntIsMatch(void *data, void *param)
{

	return(*(int *)data == *(int *)param);
}

static int StrIsMatch(void *data, void *param)
{
	int res = 0;
	res = StrCmp(data, param);
	return (res == 0);
}

static int IntCmp(void *num1, void *num2)
{

	return (*(int *)num1 - *(int *)num2);
}

static int StrCmp(void *str1, void *str2)
{

	return (strcmp((char*)str1, (char*)str2));
}


static int printInt(void *data, void *param) 
{ 
	param = param;
	printf(" %d",*(int *)data);
	
	return 0;
} 

static int printStr(void *data, void *param) 
{ 
	param = param;
	printf(" %s",(char *)data);
	
	return 0;
}
