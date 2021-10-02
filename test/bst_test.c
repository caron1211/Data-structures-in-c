
	/* Reviewd by  */

#include <stdio.h>  /* printf */
#include <stdlib.h> /* system */
#include <assert.h> /* assert */
#include "bst.h"

#define WHITE "\033[0m" 			/*  print in white   	   */
#define RED "\033[0;31m"  			/*  print in red 	  	   */
#define GREEN "\033[0;32m"  		/*  print in green   	   */
#define GOLD "\033[0;33m" 			/*  print in gold  		   */
#define BLUE "\033[0;34m"    		/*  print in blue 	  	   */
#define PURPLE "\033[0;35m"  		/*  print in purple  	   */
#define CYAN "\033[1;36m" 	 		/*  print in cyan  	  	   */
#define YELLOW "\033[1;38;5;220m"   /*  print in light yellow  */
#define MAGEN "\033[0;38;5;183m"    /*  print in magenta       */


#define TEST(name, actual, expected)\
    		 printf("\n%s: %s\n", name, actual == expected \
    		 ? GREEN"Pass"WHITE \
    		 : RED"Fail"WHITE)
    
    
static void TestBst (); 
static void *CreateNDestroy(bst_t *bst, cmp_func_t cmp_func, void *param);
int CmpFunc(const void *data1, const void *data2, const void *param);
int ActionFunc(void *data, void *param);


int main()
{
	system("clear");

	TestBst ();

	return 0; 
}


static void TestBst ()
{
	bst_t *tree = NULL;
	bst_iter_t iter_arr[10] = {NULL};
	int *param = NULL;
	int num_arr[] = {1, -3, 25, 17, -8, 0, 11, 27, 26, 14};
	int i = 0;

	printf(CYAN"\n\n\tTest Bst\n\n"WHITE); 

	tree = CreateNDestroy(tree, CmpFunc, (void *) param);
	
	TEST("BstIsEmpty, 1", BstIsEmpty(tree), 1);
	
	for (i = 0; i < 10; ++ i)
	{
		iter_arr[i] = BstInsert(tree, (void *) (num_arr + i));
	}
	
	TEST("BstSize(), 10", BstSize(tree), 10);
	
	TEST("BstGetData(), 1", *((int *) BstGetData(iter_arr[0])), num_arr[0]);
	TEST("BstGetData(), -3", *((int *) BstGetData(iter_arr[1])), num_arr[1]);
	TEST("BstPrev(), -3, -8", *((int *) BstGetData(BstPrev(iter_arr[1]))), -8);
	TEST("BstNext(), -3, 0", *((int *) BstGetData(BstNext(iter_arr[1]))), 0);
	
	TEST("NULL == BstPrev(min_num), true", (NULL == BstPrev(iter_arr[4])), 1);
	
	TEST("BstEnd() == BstNext(max_num), true", \
		  (BstNext(iter_arr[7]) == BstEnd(tree)), 1);

	TEST("max_num == BstPrev(BstEnd()), true", \
		  (BstPrev(BstEnd(tree)) == iter_arr[7]), 1);

		
	printf(BLUE"\nPrinting all tree elements using BstForeach\n"WHITE);
	BstForeach(BstBegin(tree), BstPrev(BstEnd(tree)), ActionFunc, (void *) param);

	TEST("BstFind(17)", BstFind(tree, (void *) (num_arr + 3)), iter_arr[3]);

	printf(BLUE"\nPrinting all tree elements in revese using BstForeach\n"WHITE);

	for (i = 0; i < 10; ++ i)
	{
		BstRemove(iter_arr[i]);
		TEST("BstFind(), return root", BstFind(tree, (void *) &num_arr[i]), BstEnd(tree));
	}

		
	tree = CreateNDestroy(tree, CmpFunc, (void *) param);
	printf(CYAN"\n\n\tEnd Test Bst\n\n"WHITE);
}
/*	TEST("receive a pointer", (Bst(5)? 1:0) , 1); */


static void *CreateNDestroy(bst_t *ds, cmp_func_t cmp_func, void *param)
{
	if (NULL == ds)
	{
		return (BstCreate(cmp_func, param));
	}
	
	else
	{
		BstDestroy(ds);
		return (NULL);
	}
}



/* data1 = current_node, data2 = new_node */ 
int CmpFunc(const void *data1, const void *data2, const void *param)
{
	assert (NULL != data1);
	assert (NULL != data2);
	assert (param == param);
	
	return (*((int *) data1) - *((int *) data2));
}


int ActionFunc(void *data, void *param)
{
	assert (NULL != data);
	assert (param == param);
	
	printf("%d ", *((int *) data));
	
	return (0);
}




