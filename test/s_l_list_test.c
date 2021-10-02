#include <stdio.h> /*printf*/
#include <assert.h> /* assert */
#include "s_l_list.h"

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
    
static void SListTest();
static void AppendListTest();
static void PrintElements();
int main()
{

    SListTest();
    AppendListTest();
    
    return 0; 
}


int IntCmp(const void *data, void *param)
{
	int a = 0;
	int b = 0;
	 
	assert(NULL !=data);
	assert(NULL !=param);
	
	a = *(int *)&data;
	b = *(int *)&param;

	return(a == b);
}

int printInt(void *data, void *param) 
{ 
	assert(data);
	assert(param);
	printf(" %d",*(int *)&data);
	
	return 0;
} 

static void SListTest()
{
	slist_iter_t itr = NULL;
	slist_t *list = NULL;
	
	printf(MAG"Create list\n"WHIT);
	list = SlistCreate();
	itr = SlistBegin(list);
	
	TEST_IS_TRUE("check if empty", SlistIsEmpty(list));
	TEST_IS_EQUAL("check count equal to 0", SlistCount(list), 0);
	
	printf(MAG"Insert to Begin (11)\n"WHIT);
	itr = SlistInsert(itr, (void *)11);
	PrintElements(list);
	TEST_IS_FALSE("check if is not empty", SlistIsEmpty(list));
	TEST_IS_EQUAL("check count equal to 1", SlistCount(list), 1);

	printf(MAG"Insert to Begin(12)\n"WHIT);
	itr = SlistInsert(itr, (void *)12);
	PrintElements(list);
	
	printf(MAG"set value(-12)\n"WHIT);
	SlistSetData(itr, (void *)-12);
	PrintElements(list);
	
	printf(MAG"Insert to end (13)\n"WHIT);
	itr = SlistInsert(SlistEnd(list), (void *)13);
	PrintElements(list);
	TEST_IS_EQUAL("check count equal to 3", SlistCount(list), 3);
	
	printf(MAG"find '13' and remove\n"WHIT);
	itr = SlistFind(SlistBegin(list), SlistEnd(list), IntCmp, (void *) 13 );
	itr = SlistRemove(itr);
	PrintElements(list);
	
	TEST_IS_EQUAL("check count equal to 2", SlistCount(list), 2);
	
	SlistDestroy(list);
	list = NULL;	
}

static void AppendListTest()
{
	slist_t *list1 = NULL;
	slist_t *list2 = NULL;
	
	slist_iter_t itr1 = NULL;
	slist_iter_t itr2 = NULL;
	
	list1 = SlistCreate();
	list2 = SlistCreate();
	
	itr1 = SlistBegin(list1);
	itr2 = SlistBegin(list2);
	
	itr1 = SlistInsert(itr1, (void *)1);
	itr1 = SlistInsert(itr1, (void *)2);
	itr1 = SlistInsert(itr1, (void *)3);
	
	itr2 = SlistInsert(itr2, (void *)4);
	itr2 = SlistInsert(itr2, (void *)5);
	itr2 = SlistInsert(itr2, (void *)6);
	
	printf(MAG"list 1\n"WHIT);
	PrintElements(list1);
	
	printf(MAG"list 2\n"WHIT);
	PrintElements(list2);
	printf(MAG"Append 2 list\n"WHIT);
	
	SlistAppend(list1, list2);
	PrintElements(list1);
	
	SlistDestroy(list1);
	

}

static void PrintElements(slist_t *list)
{
	printf("print element using foreach: ");
	SlistForeach(SlistBegin(list), SlistEnd(list),printInt,(void *) 1);
	printf("\n");
}
