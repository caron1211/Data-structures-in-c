#include <stdio.h> /*printf*/
#include "bst.h"

#define RED "\033[5;91m"           /* print in red          */
#define GREEN "\033[1;38;5;47m"    /* print in green        */
#define MAGEN "\033[0;38;5;183m"   /* print in magenta      */
#define YELLOW "\033[1;38;5;220m"  /* print in light yellow */
#define CYAN "\033[0;96m"          /* print in cyan         */
#define LIGHT "\033[2;37m"         /* print in gray         */
#define PURPLE "\033[1;4;38;5;224m"/* print in purple       */
#define RESET "\033[1;0m"          /* reset print color     */
#define WHITE "\033[0m"

#define TEST_IS_EQUAL(test_name, expected, real)\
			(real == expected ?\
			printf("%s:" GREEN" PASS\n"WHITE, test_name) :\
			printf("%s:"  RED" FAIL\n"WHITE, test_name))

#define TEST_IS_TRUE(test_name, result)\
			(result ?\
			printf("%s:" GREEN" PASS\n"WHITE, test_name) :\
			printf("%s:"  RED" FAIL\n"WHITE, test_name))

#define TEST_IS_FALSE(test_name, result)\
			(!result ?\
			printf("%s:" GREEN" PASS\n"WHITE, test_name) :\
			printf("%s:"  RED" FAIL\n"WHITE, test_name))		

int CmpInt(const void *data1, const void *data2, const void *param);
int Sum(void *data, void *param);


void TestBstCreateDestroy();
void TestBstInsert();
void TestBstEndBegin();
void TestBstNext();
void TestBstPrev();
void TestBstSize();
void TestBstFind();
void TestBstForEach();
void TestBstRemove();



int main()
{
    TestBstCreateDestroy();
	TestBstInsert();
	TestBstEndBegin();
	TestBstNext();
    TestBstPrev();
	TestBstSize();
	TestBstFind();
	TestBstForEach();
	TestBstRemove();

    return 0; 
}

void TestBstCreateDestroy()
{
	bst_t *bst = NULL;
	
    printf(YELLOW"\n**********Testing CreateDestroy**********\n\n"RESET);
	
	bst = BstCreate(CmpInt, NULL);
	TEST_IS_TRUE("TestCreate", bst);
	BstDestroy(bst);
}

void TestBstInsert()
{
	bst_t *bst = NULL;
	bst_iter_t itr1 = NULL;
	bst_iter_t itr2 = NULL;
	bst_iter_t itr3 = NULL;

	int a = 5;
	int b = 3;
	int c = 8;

	printf(YELLOW"\n**********Testing TestBstInsert**********\n\n"RESET);

	bst = BstCreate(CmpInt, NULL);

	itr1 = BstInsert(bst, &a);
	itr2 = BstInsert(bst, &b);
	itr3 = BstInsert(bst, &c);

	TEST_IS_EQUAL("TestBstInsert", *(int *) BstGetData(itr1), a);
	TEST_IS_EQUAL("TestBstInsert", *(int *) BstGetData(itr2), b);
	TEST_IS_EQUAL("TestBstInsert", *(int *) BstGetData(itr3), c);

	BstDestroy(bst);
}

void TestBstEndBegin()
{
	bst_t *bst = NULL;
	bst_iter_t itr = NULL;

	int a = 3;
	int b = 2;
	int c = 1;
	int d = 10;

	bst = BstCreate(CmpInt, NULL);
	BstInsert(bst, &a);
	BstInsert(bst, &b);
	BstInsert(bst, &c);
	BstInsert(bst, &d);


	itr = BstBegin(bst);
	TEST_IS_EQUAL("BstBegin", *(int *) BstGetData(itr), c);
	printf("aa%d\n", (int *) BstGetData(itr));
	BstDestroy(bst);
}

void TestBstNext()
{
	bst_t *bst = NULL;
	bst_iter_t itr1 = NULL;
	bst_iter_t itr2 = NULL;
	bst_iter_t itr3 = NULL;

	int a = 6;
	int b = 3;
	int c = 8;
	int d = 0;
	int e = 5;
	int f = 7;
	int g = 9;

	bst = BstCreate(CmpInt, NULL);
	

	BstInsert(bst, &a);
	BstInsert(bst, &b);
	BstInsert(bst, &c);
	itr2 = BstInsert(bst, &d);
	itr1 = BstInsert(bst, &e);
	BstInsert(bst, &f);
	itr3 = BstInsert(bst, &g);

	itr1 = BstNext(itr1);
	itr2 = BstNext(itr2);
	itr3 = BstNext(itr3);

	TEST_IS_EQUAL("BstNext", *(int *) BstGetData(itr1), a);
	TEST_IS_EQUAL("BstNext", *(int *) BstGetData(itr2), b);
	TEST_IS_EQUAL("BstNext", BstEnd(bst), itr3);
	BstDestroy(bst);
}

void TestBstPrev()
{
	bst_t *bst = NULL;
	bst_iter_t itr1 = NULL;
	bst_iter_t itr2 = NULL;
	bst_iter_t itr3 = NULL;

	int a = 6;
	int b = 3;
	int c = 8;
	int d = 0;
	int e = 5;
	int f = 7;
	int g = 9;

	bst = BstCreate(CmpInt, NULL);
	

	itr2 = BstInsert(bst, &a);
	BstInsert(bst, &b);
	BstInsert(bst, &c);
	itr3 = BstInsert(bst, &d);
	BstInsert(bst, &e);
	itr1 = BstInsert(bst, &f);
	BstInsert(bst, &g);

	itr1 = BstPrev(itr1);
	itr2 = BstPrev(itr2);
	/* itr3 = BstPrev(itr3); */

	TEST_IS_EQUAL("BstPrev", *(int *) BstGetData(itr1), a);
	TEST_IS_EQUAL("BstPrev", *(int *) BstGetData(itr2), e);
	/* TEST_IS_EQUAL("BstPrev", BstEnd(bst), itr3); */

	printf("%d \n", *(int *) BstGetData(itr1));
	printf("%d \n", *(int *) BstGetData(itr2));
	BstDestroy(bst);
}

void TestBstSize()
{
    bst_t *bst = NULL;
    int a = 6;
	int b = 3;

	bst = BstCreate(CmpInt, NULL);
	TEST_IS_EQUAL("TestBstSize", BstSize(bst), 0);
	BstInsert(bst, &a);
	BstInsert(bst, &b);

    TEST_IS_EQUAL("TestBstSize", BstSize(bst), 2);
	BstDestroy(bst);
}

void TestBstFind()
{
	bst_t *bst = NULL;
    int a = 6;
	int b = 3;
	int c = 8;
	int d = 0;

	bst = BstCreate(CmpInt, NULL);

	BstInsert(bst, &a);
	BstInsert(bst, &b);
	BstInsert(bst, &c);

    TEST_IS_EQUAL("TestBstFind", *(int *) BstGetData(BstFind(bst, &b)), b);
	TEST_IS_EQUAL("TestBstFind", BstFind(bst, &d), BstEnd(bst));
	BstDestroy(bst);
}

void TestBstForEach()
{
    bst_t *bst = NULL;
	static int sum = 0;
    int a = 6;
	int b = 3;
	int c = 8;
	int d = 0;

	bst = BstCreate(CmpInt, NULL);

	BstInsert(bst, &a);
	BstInsert(bst, &b);
	BstInsert(bst, &c);
	BstInsert(bst, &d);

	BstForeach(BstBegin(bst), BstEnd(bst), Sum, &sum);
	printf(" aaaaa %d \n", sum);
	BstDestroy(bst);
}

void TestBstRemove()
{
	bst_t *bst = NULL;
	bst_iter_t itr1 = NULL;
    int a = 6;
	int b = 3;
	int c = 8;
	int d = 0;

	bst = BstCreate(CmpInt, NULL);

	itr1 = BstInsert(bst, &a);
	BstInsert(bst, &b);
	BstInsert(bst, &c);
	BstInsert(bst, &d);

	BstRemove(itr1);

	print2DUtil(itr1, 10);

	BstDestroy(bst);
}
int CmpInt(const void *data1, const void *data2, const void *param)
{
	param = param;
	return (*(int *)data1 - *(int *)data2);
}

int Sum(void *data, void *param)
{
	int *sum = param; 

	*sum += *(int *)data;

	return 0;

}