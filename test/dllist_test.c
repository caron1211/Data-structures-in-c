#include "dllist.h"
#include <stdio.h>

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
			(itr != DlistEnd(list) ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))			
			
#define TEST_IS_END(test_name, itr, list)\
			(itr == DlistEnd(list) ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))
			
static void PrintElements(dlist_t *list);
static int IntCmp(void *data, void *param);
static int printInt(void *data, void *param);


static void CreateAndDestroyTest();
static void PushAndPopFrontTest();
static void PushAndPopBackTest();
static void IsEmptyTest();
static void CountTest();
static void SpliceTest();
static void FindAndRemoveTest();
static void IsSameIterTest();
static void FindMultiTest();

int main()
{
	CreateAndDestroyTest();
	PushAndPopFrontTest();
	PushAndPopBackTest();
	IsEmptyTest();
	CountTest();
	SpliceTest();
	FindAndRemoveTest();
	IsSameIterTest();
	FindMultiTest();
	
	return 0;
}

static void CreateAndDestroyTest()
{
	dlist_t *list = NULL;
	
	printf(MAG"CreateAndDestroyTest\n"WHIT);
	list = DlistCreate();
	TEST_IS_TRUE("CreateAndDestroyTest", list);

	DlistDestroy(list); 
}

static void PushAndPopFrontTest()
{
	dlist_t *list = NULL;
	
	printf(MAG"PushAndPopFrontTest\n"WHIT);
	list = DlistCreate();
	
	DlistPushFront(list,(void *) 4);
	DlistPushFront(list, (void *) 3);
	
	TEST_IS_EQUAL("PushFront test", DlistGetData(DlistBegin(list)), (void *) 3);
	DlistPopFront(list);
	TEST_IS_EQUAL("PopFront test",DlistGetData(DlistBegin(list)), (void *) 4);

	DlistDestroy(list); 
	
}

static void PushAndPopBackTest()
{
	dlist_t *list = NULL;
	
	printf(MAG"PushAndPopBackTest\n"WHIT);
	list = DlistCreate();
	
	DlistPushBack(list,(void *) 4);
	DlistPushBack(list, (void *) 3);
	
	TEST_IS_EQUAL("PushBack test", DlistGetData(DlistBegin(list)), (void *) 4);
	DlistPopBack(list);
	TEST_IS_EQUAL("PopBack test",DlistGetData(DlistBegin(list)), (void *) 4);
	

	DlistDestroy(list); 
}

static void IsEmptyTest()
{
	dlist_t *list = NULL;
	
	printf(MAG"IsEmptyTest\n"WHIT);
	list = DlistCreate();
	
	TEST_IS_TRUE("empty test case 1", DlistIsEmpty(list));
	DlistPushBack(list,(void *) 4);
	TEST_IS_FALSE("empty test case 2", DlistIsEmpty(list));
	
	
	DlistDestroy(list); 
}

static void CountTest()
{
	dlist_t *list = NULL;
	
	printf(MAG"CountTest\n"WHIT);
	list = DlistCreate();
	
	TEST_IS_EQUAL("CountTest case 1", DlistSize(list), 0);
	DlistPushBack(list,(void *) 4);
	
	TEST_IS_EQUAL("CountTest case 2", DlistSize(list), 1);
	DlistPushFront(list,(void *) 2);
	
	TEST_IS_EQUAL("CountTest case 3", DlistSize(list), 2);
	
	DlistDestroy(list); 
}

static void SpliceTest()
{
	dlist_t *list1 = NULL;
	dlist_t *list2 = NULL;
	
	dlist_iter_t dest_itr = NULL;
	dlist_iter_t from_itr = NULL;
	dlist_iter_t to_itr = NULL;
	dlist_iter_t tmp_itr = NULL;
	
	printf(MAG"SpliceTest\n"WHIT);
	list1 = DlistCreate();
	list2 = DlistCreate();
	
	DlistPushBack(list1,(void *) 1);
	dest_itr = DlistPushBack(list1,(void *) 2);
	DlistPushBack(list1,(void *) 3);
	
	DlistPushBack(list2,(void *) 10);
	DlistPushBack(list2,(void *) 11);
	from_itr = DlistPushBack(list2,(void *) 12);
	DlistPushBack(list2,(void *) 13);
	to_itr = DlistPushBack(list2,(void *) 14);
	DlistPushBack(list2,(void *) 15);
	
	tmp_itr = DlistSplice(dest_itr, from_itr, to_itr);
	
	TEST_IS_EQUAL("DlistSplice", DlistSize(list1), 5);
	TEST_IS_EQUAL("DlistSplice", DlistSize(list2), 4);


	tmp_itr = DlistFindOne(DlistBegin(list1), DlistEnd(list1), IntCmp, (void *) 13);
	TEST_NOT_END("DlistSplice", tmp_itr, list1); /* found in list 1 */
	
	tmp_itr = DlistFindOne(DlistBegin(list2), DlistEnd(list2), IntCmp, (void *) 13);
	TEST_IS_END("DlistSplice", tmp_itr, list2); /* not found in list 2 */
	
	PrintElements(list1);
	PrintElements(list2);
	
	/* new case */
	dest_itr = DlistBegin(list1);
	from_itr = DlistBegin(list2);
	to_itr = DlistEnd(list2);
	
	tmp_itr = DlistSplice(dest_itr, from_itr, to_itr);
	
	TEST_IS_EQUAL("DlistSplice", DlistSize(list1), 9);
	TEST_IS_EQUAL("DlistSplice", DlistSize(list2), 0);

	DlistDestroy(list1); 
	DlistDestroy(list2); 
}

static void FindAndRemoveTest()
{
	dlist_t *list = NULL;
	dlist_iter_t itr = NULL;
	
	printf(MAG"FindAndRemoveTest\n"WHIT);
	list = DlistCreate();
	
	DlistPushBack(list,(void *) 10);
	DlistPushBack(list,(void *) 11);
	DlistPushBack(list,(void *) 5);
	
	itr = DlistFindOne(DlistBegin(list), DlistEnd(list), IntCmp, (void *) 10);
	TEST_NOT_END("FindTest", itr, list);
	TEST_IS_EQUAL("FindTest",DlistGetData(itr), (void *) 10);
	
	itr = DlistRemove(itr);
	TEST_IS_EQUAL("RemoveTest",DlistGetData(itr), (void *) 11);
	
	itr = DlistRemove(DlistBegin(list));
	itr = DlistRemove(DlistBegin(list));
	
	TEST_IS_END("RemoveTest", itr, list);
	
	DlistDestroy(list); 
}

static void IsSameIterTest()
{

	dlist_t *list1 = NULL;
	dlist_t *list2 = NULL;
	
	dlist_iter_t itr1 = NULL;
	dlist_iter_t itr2 = NULL;
	
	printf(MAG"IsSameIterTest\n"WHIT);
	list1 = DlistCreate();
	list2 = DlistCreate();
	
	itr1 = DlistBegin(list1);
	itr2 = DlistBegin(list2);
	
	TEST_IS_FALSE("IsSameIterTest",DlistIsSameIter(itr1, itr2));
	
	/* new case */
	DlistPushBack(list1,(void *) 11);
	itr1 = DlistPushBack(list1,(void *) 12);

	itr2 = DlistBegin(list1);
	itr2 = DlistNext(itr2);
	
	TEST_IS_TRUE("IsSameIterTest",DlistIsSameIter(itr1, itr2));
	
	itr2 = DlistFindOne(DlistBegin(list1), DlistEnd(list1), IntCmp, (void *) 12);
	
	TEST_IS_TRUE("IsSameIterTest",DlistIsSameIter(itr1, itr2));
	
	DlistDestroy(list1); 
	DlistDestroy(list2); 

}

static void FindMultiTest()
{

	dlist_t *list1 = NULL;
	dlist_t *list2 = NULL;
	int res = 0;
	
	
	printf(MAG"DlistFindMulti\n"WHIT);
	list1 = DlistCreate();
	list2 = DlistCreate();
	
	DlistPushBack(list1,(void *) 1);
	DlistPushBack(list1,(void *) 2);
	DlistPushBack(list1,(void *) 2);
	DlistPushBack(list1,(void *) 3);
	DlistPushBack(list1,(void *) 1);
	DlistPushBack(list1,(void *) 2);
	
	
	res = DlistFindMulti(DlistBegin(list1), DlistEnd(list1), IntCmp, (void *) 1, list2);
	TEST_IS_EQUAL("FindMultiTest",res , 2);
	DlistDestroy(list2);
	list2 = DlistCreate();
	
	res = DlistFindMulti(DlistBegin(list1), DlistEnd(list1), IntCmp, (void *) 2, list2);
	TEST_IS_EQUAL("FindMultiTest",res , 3);
	DlistDestroy(list2);
	list2 = DlistCreate();
	
	res = DlistFindMulti(DlistBegin(list1), DlistEnd(list1), IntCmp, (void *) 3, list2);
	TEST_IS_EQUAL("FindMultiTest",res , 1);
	DlistDestroy(list2);
	list2 = DlistCreate();
	
	res = DlistFindMulti(DlistBegin(list1), DlistEnd(list1), IntCmp, (void *) 4, list2);
	TEST_IS_EQUAL("FindMultiTest",res , 0);
	
	DlistDestroy(list1); 
	DlistDestroy(list2); 
	
}
static int IntCmp(void *data, void *param)
{

	return(data == param);
}

static int printInt(void *data, void *param) 
{ 
	param = param;
	printf(" %d",*(int *)&data);
	
	return 0;
} 

static void PrintElements(dlist_t *list)
{
	printf("print element using foreach: ");
	DlistForeach(DlistBegin(list), DlistEnd(list),printInt,(void *) 1);
	printf("\n");
}

