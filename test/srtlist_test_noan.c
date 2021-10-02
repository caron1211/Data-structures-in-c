#include <stdio.h> /* printf */
#include <stdlib.h> /* system */
#include <string.h> /* strcmp */

#include "srtlist.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define WHITE "\033[0m"
#define YELLOW "\033[1;33m"
#define PURPLE "\033[1;35m"

/*****************************************************************************/

#define TEST(name, actual, expected)\
    				printf("%s: %s\n\n", name, actual == expected ?\
     				GREEN"Pass"WHITE : RED"Fail"WHITE)
    
/*****************************************************************************/
						/* Function Declaration */

int CmpInt(void *data, void *param);
int CmpString(void *data, void *param);
static void TestOne();
static void TestTwo();
static void TestThree();
static void TestFour();
static void StringTest();
static void MergeIntTest();
static srtlist_t *CreateTest(srtlist_t *list);
static void FillListInt(srtlist_t *list);
int IntPrint(void *data, void *param);
int StringPrint(void *data, void *param);
static void PrintNodes(srtlist_t *list);
int IsMatchStr(void *data, void *param);

/*****************************************************************************/

int main()
{
    TestOne();
    TestTwo();
    TestThree();
    TestFour();
    StringTest();
    MergeIntTest();
    
    return 0; 
}

/*****************************************************************************/
int CmpInt(void *data, void *param)
{
	if (*(int *)&data == *(int *)&param)
	{
		return 0;
	}
	else if (*(int *)&data > *(int *)&param)
	{
		return 1;
	}
	return -1;
}
/*****************************************************************************/
int CmpString(void *data, void *param)
{
	int res = strcmp(data, param);
	if (res < 0)
		return -1;
	else if (res > 0)
		return 1;
	return 0;
}
/*****************************************************************************/

static void TestOne()
{
	srtlist_t *list = NULL;
    void *data = NULL;
    
    printf(YELLOW"\n\tFirst Test"WHITE);
    list = CreateTest(list);
    TEST("SrtListIsEmpty(list) == 1", SrtListIsEmpty(list), 1);
    TEST("SrtListSize(list) == 0", SrtListSize(list), 0);
    
    FillListInt(list);
    PrintNodes(list);
    TEST("SrtListIsEmpty(list) == 0", SrtListIsEmpty(list), 0);
    TEST("SrtListSize(list) == 5", SrtListSize(list), 5);
    
    data = SrtListPopFront(list);
    printf("The data is: %d\n", *(int *)&data);
    TEST("SrtListPopFront(list) == 1",data, (void *)1); /* casting?? */
    
    data = SrtListPopBack(list);
    printf("The data is: %d\n", *(int *)&data);
    TEST("SrtListPopFront(list) == 19",data, (void *)19); /* casting?? */
	
    PrintNodes(list);
	
    SrtListDestroy(list);
}

/*****************************************************************************/

static void TestTwo()
{
	srtlist_t *list = NULL;
    srtlist_iter_t iter = 0;
    
    printf(YELLOW"\n\tSecond Test"WHITE);
    list = CreateTest(list);
    TEST("SrtListIsEmpty(list) == 1", SrtListIsEmpty(list), 1);
    TEST("SrtListSize(list) == 0", SrtListSize(list), 0);
    
    FillListInt(list);
    PrintNodes(list);
    TEST("SrtListIsEmpty(list) == 0", SrtListIsEmpty(list), 0);
    TEST("SrtListSize(list) == 5", SrtListSize(list), 5);
    
    iter = SrtListBegin(list);
    printf(CYAN"\nRemove the first element from the list"WHITE);
    iter = SrtListRemove(iter);
    PrintNodes(list);
    
    printf(CYAN"\nNext->Next->Next->Remove"WHITE);
    iter = SrtListNext(iter);
    iter = SrtListNext(iter);
    iter = SrtListNext(iter);
    iter = SrtListRemove(iter);
    PrintNodes(list);
    
    SrtListDestroy(list);
}

/*****************************************************************************/

static void TestThree()
{
	srtlist_t *list = NULL;
    srtlist_iter_t iter = 0;
    
    printf(YELLOW"\n\tThird Test"WHITE);
    list = CreateTest(list);
   
    FillListInt(list);
    PrintNodes(list);
    
    iter = SrtListEnd(list);
    printf(CYAN"\nRemove the last element from the list"WHITE);
    iter = SrtListPrev(iter);
    iter = SrtListRemove(iter);
    PrintNodes(list);
    
    printf(CYAN"\nPrev->Remove"WHITE);
    iter = SrtListPrev(iter);
    iter = SrtListRemove(iter);
    PrintNodes(list);
    
    SrtListDestroy(list);
}

/*****************************************************************************/

static void TestFour()
{
	srtlist_t *list = NULL;
    srtlist_iter_t iter = 0;
    
    printf(YELLOW"\n\tTest Find Function"WHITE);
    list = CreateTest(list);
   
    SrtListInsert(list, (void *)50);
	SrtListInsert(list, (void *)99);
	SrtListInsert(list, (void *)450);
	SrtListInsert(list, (void *)202);
    PrintNodes(list);
	
	iter = SrtListFind(SrtListBegin(list), SrtListEnd(list), (void *)99);
	TEST("\nFind 99 in the list using SrtListFind",SrtListGetData(iter), (void *)99);
	
	iter = SrtListFind(SrtListBegin(list), SrtListEnd(list), (void *)99);
	TEST("Find 300 in the list using SrtListFind (NEED TO FAIL)"
	,SrtListGetData(iter), (void *)300);
	
	SrtListDestroy(list);
}

/*****************************************************************************/

static void StringTest()
{
	srtlist_t *list = NULL;
    srtlist_iter_t iter = 0;
    srtlist_t *list2 = NULL;
    
    printf(YELLOW"\n\tStrings Test"WHITE);
	list = SrtListCreate(CmpString);
    
    printf(CYAN"\nInsert string to the list\n"WHITE);
    SrtListInsert(list, (void *)"Eden");
	SrtListInsert(list, (void *)"Naor");
	SrtListInsert(list, (void *)"Carmel");
	SrtListInsert(list, (void *)"Liran");
	SrtListInsert(list, (void *)"Noam");
    SrtListForeach(SrtListBegin(list), SrtListEnd(list), StringPrint, NULL);
    printf("\n");
    
    iter = SrtListFindIf(SrtListBegin(list), SrtListEnd(list), IsMatchStr, (void *)"Liran");
    TEST("Find \"Liran\" in the list using SrtListFindIf"
	,SrtListGetData(iter), (void *)"Liran");
	
	iter = SrtListFindIf(SrtListBegin(list), SrtListEnd(list), IsMatchStr, (void *)"Angie");
    TEST("Find \"Angie\" in the list using SrtListFindIf"
	,SrtListNext(iter), NULL);
	
	list2 = SrtListCreate(CmpString);
    printf(CYAN"\nInsert string to the list\n"WHITE);
    SrtListInsert(list2, (void *)"Inbar");
	SrtListInsert(list2, (void *)"Eyal");
	SrtListInsert(list2, (void *)"Idan");
	SrtListInsert(list2, (void *)"Liran");
	SrtListForeach(SrtListBegin(list2), SrtListEnd(list2), StringPrint, NULL);
    printf("\n");
	printf("List for Merge Test Created\n\n");
	
	SrtListMerge(list, list2);
	SrtListForeach(SrtListBegin(list), SrtListEnd(list), StringPrint, NULL);
    printf("\n");
    
	SrtListDestroy(list);
	SrtListDestroy(list2);
}

/*****************************************************************************/

static void MergeIntTest()
{
	srtlist_t *list = NULL;
    srtlist_t *list2 = NULL;
    
    printf(YELLOW"\n\tTest Find Function"WHITE);
    list = CreateTest(list);
	
    SrtListInsert(list, (void *)1);
	SrtListInsert(list, (void *)7);
	SrtListInsert(list, (void *)8);
	SrtListInsert(list, (void *)12);
    PrintNodes(list);
    
    list2 = CreateTest(list2);
    SrtListInsert(list2, (void *)12);
	SrtListInsert(list2, (void *)1);
	SrtListInsert(list2, (void *)13);
	SrtListInsert(list2, (void *)14);
	SrtListInsert(list2, (void *)0);
	PrintNodes(list2);
    
    printf("\n");
    printf("\n");
    SrtListMerge(list, list2);
	SrtListForeach(SrtListBegin(list), SrtListEnd(list), IntPrint, NULL);
    printf("\n");
    
    SrtListDestroy(list);
	SrtListDestroy(list2);
}

/*****************************************************************************/
int IsMatchStr(void *data, void *param)
{
	int res = strcmp(data, param);
	return (res == 0? 1 : 0);
}
/*****************************************************************************/
static srtlist_t *CreateTest(srtlist_t *list)
{
	list = SrtListCreate(CmpInt);
	
	printf("\nTest - Sorted List Create - "); 
	if (NULL == list)
	{
		printf(RED"FAIL\n"WHITE);
	}
	else
	{
		printf(GREEN"SUCCESS!\n"WHITE);
	}
	return (list);
}
/*****************************************************************************/
static void FillListInt(srtlist_t *list)
{
	int num1 = 1;
	int num2 = 8;
	int num3 = 4;
	int num4 = 19;
	int num5 = 3;
	
	printf(CYAN"\nFilling List wite: {1,8,4,19,3}\n"WHITE);
	
	SrtListInsert(list, *(void **)&num1);
	SrtListInsert(list, *(void **)&num2);
	SrtListInsert(list, *(void **)&num3);
	SrtListInsert(list, *(void **)&num4);
	SrtListInsert(list, *(void **)&num5);
}
/*****************************************************************************/
int IntPrint(void *data, void *param)
{
	param = param;
	
	printf("%d <-> ", *(int *)&data);
	return (0);
}
/*****************************************************************************/
int StringPrint(void *data, void *param)
{
	param = param;
	
	printf("%s <-> ", (char *)data);
	return (0);
}
/*****************************************************************************/
static void PrintNodes(srtlist_t *list)
{
	void *param = NULL;
	printf(CYAN"\nPrints all elements using ForEach\n"WHITE); 
	SrtListForeach(SrtListBegin(list), SrtListEnd(list), IntPrint, param);
	printf("\n");
}

/*****************************************************************************/
