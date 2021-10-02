
#include <stdio.h>/*printf*/
#include <stdlib.h>/*sysytem, qsort*/
#include <time.h>/*clock*/
#include "bst.h"
/****************************************************************************/
#define RED "\033[5;91m"           /* print in red          */
#define GREEN "\033[1;38;5;47m"    /* print in green        */
#define MAGEN "\033[0;38;5;183m"   /* print in magenta      */
#define YELLOW "\033[1;38;5;220m"  /* print in light yellow */
#define CYAN "\033[0;96m"          /* print in cyan         */
#define LIGHT "\033[2;37m"         /* print in gray         */
#define PURPLE "\033[1;4;38;5;224m"/* print in purple       */
#define RESET "\033[1;0m"          /* reset print color     */

#define TEST(name, actual, expected)\
    printf("%s: %s\n\n", name, actual == expected ? GREEN"SUCCESS"RESET : RED"FAILURE"RESET)
#define TEST_ARE_NOT_EQUAL(name, real, expected) printf("%s: %s", name, real != expected\
									? GREEN"SUCCESS\n"RESET\
									: RED"\aFAILURE\n"RESET)	
    

/****************************************************************************/
int CompareIntFunction(const void *data1, const void *data2, const void *param);
int PrintTreePlus2(void *data, void *param);
/****************************************************************************/
int main()
{
	int one = 1;
	int two = 2;
	int three = 3;
	int four = 4;
	int five = 5;
	int six = 6;
	int nine = 9;
	int ten = 10;
	bst_iter_t start = NULL;
	bst_iter_t iter = NULL;
	bst_iter_t find_iter = NULL;
	
	bst_t *bst = NULL;

	system("clear");

	printf(YELLOW"\n***************Testing BST Create ***************\n\n"RESET);


	bst = BstCreate(CompareIntFunction, NULL);

	TEST("test BST create", (bst != NULL), 1 );

	TEST("test BST IsEmpty = 1", BstIsEmpty(bst), 1 );

	TEST("test BST size = 0  ", BstSize(bst), 0 );

		printf(YELLOW"\n***************Testing BST insert ***************\n\n"RESET);


	start = BstInsert(bst, &five);
	
	BstInsert(bst, &one);
	
	BstInsert(bst, &nine);

	BstInsert(bst, &six);

	BstInsert(bst, &three);

	BstInsert(bst, &ten);

	BstInsert(bst, &two);

	TEST("test BST IsEMpty = 0", BstIsEmpty(bst), 0 );
	TEST("test BST size = 7  ", BstSize(bst), 7 );

	printf(YELLOW"\n***************Testing BST next prev ***************\n\n"RESET);

	printf("\n\tTest Next\n");
	iter = BstBegin(bst);
	while(iter != BstEnd(bst))
	{
		printf("%d-->", *(int *)BstGetData(iter));
		iter = BstNext(iter);
	}

	printf("\n\n\tTest Prev\n");
	iter = BstEnd(bst);
	while(iter != BstBegin(bst))
	{
		iter = BstPrev(iter);
		printf("%d-->", *(int *)BstGetData(iter));
	}
	print2DUtil(start, 10);
	

	printf(YELLOW"\n***************Testing BST Find ***************\n\n"RESET);

	TEST_ARE_NOT_EQUAL("test BstFind(bst, &nine);", BstFind(bst, &nine) , BstEnd(bst));
	TEST("test BstFind(bst, &four);", BstFind(bst, &four) ,BstEnd(bst));


	printf(YELLOW"\n***************Testing BST foreach ***************\n\n"RESET);

	printf("test : BstForeach(BstBegin(bst),BstEnd(bst), PrintTreePluspParam,&four);\n");
	BstForeach(BstBegin(bst),BstEnd(bst), PrintTreePlus2,&four);


	printf(YELLOW"\n***************Testing BST Remove ***************\n\n"RESET);

	BstRemove(start);

	TEST("BstRemove(start);", BstFind(bst, &five) , BstEnd(bst) );
	TEST("test BST size = 6  ", BstSize(bst), 6 );

	BstRemove(iter);
	TEST("BstRemove(start);", BstFind(bst, &five) , BstEnd(bst) );
	TEST("test BST size = 5", BstSize(bst), 5 );

	print2DUtil(start, 10);

	BstDestroy(bst);

	return(0);
}
/******************************************************************************/
int CompareIntFunction(const void *data1, const void *data2, const void *param)
{
	param = param;
	return (*(int *)&data2 - *(int *)&data1);
}

/******************************************************************************/
int PrintTreePlus2(void *data, void *param)
{
	printf("%d-->", (*(int *)data + *(int*)param));
	return(0);
}
/******************************************************************************/