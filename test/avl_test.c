#include <stdio.h> /*printf*/
#include "avl.h"
#include "utilities.h"

int CmpInt(const void *new_data, const void *curr_data);

void CreateDestroyTest();
void InsertTest();
void TestAvlForEach();
void TestAvlSize();
void RemoveTest();


static int AvlPrintNode(void *data, void *param);
int Sum(void *data, void *param);

int main ()
{
    CreateDestroyTest();
    TestAvlSize();
    InsertTest();
    RemoveTest();
    TestAvlForEach();

    return 0;
}
void CreateDestroyTest()
{
    avl_t * avl = NULL;
    avl = AvlCreate(CmpInt);
    
    printf(YELLOW"CreateDestroyTest\n"WHITE);

    TEST_IS_TRUE("CreateDestroyTest", avl);
    AvlDestroy(avl);
}

void TestAvlSize()
{
    int a = 15;
    int b = 8;
    int c = 25;
    int d = 20;

    avl_t * avl = AvlCreate (CmpInt);

    printf(YELLOW"TestAvlSize\n"WHITE);

    TEST_IS_EQUAL("TestAvlSize", AvlSize(avl), 0);
    AvlInsert(avl, &a);
    TEST_IS_EQUAL("TestAvlSize", AvlSize(avl), 1);
    AvlInsert(avl, &b);
    AvlInsert(avl, &c);
    TEST_IS_EQUAL("TestAvlSize", AvlSize(avl), 3);
    
    TEST_IS_EQUAL("TestBstFind", *(int *) AvlFind(avl, &b), b);
	TEST_IS_EQUAL("TestBstFind", AvlFind(avl, &d), NULL);

    AvlDestroy(avl);

}

void RemoveTest()
{
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    avl_t * avl1 = AvlCreate (CmpInt);
    avl_t * avl2 = AvlCreate (CmpInt);
    avl_t * avl3 = AvlCreate (CmpInt);
    avl_t * avl4 = AvlCreate (CmpInt);

    printf(YELLOW"RemoveTest\n"WHITE);

    AvlInsert(avl1, &c);
    AvlInsert(avl1, &b);
    AvlInsert(avl1, &d);
    AvlInsert(avl1, &a);

    PrintTree(avl1);
    printf("Remove 4\n");
    AvlRemove(avl1, &d);
    PrintTree(avl1);

    TEST_IS_EQUAL("RemoveTest LL", AvlHeight(avl1), 2);
    /******************************/
    AvlInsert(avl2, &c);
    AvlInsert(avl2, &a);
    AvlInsert(avl2, &d);
    AvlInsert(avl2, &b);

    PrintTree(avl2);
    printf("Remove 2\n");
    AvlRemove(avl2, &d);
    PrintTree(avl2);
    TEST_IS_EQUAL("RemoveTest LR", AvlHeight(avl2), 2);
    /******************************/
    AvlInsert(avl3, &b);
    AvlInsert(avl3, &a);
    AvlInsert(avl3, &c);
    AvlInsert(avl3, &d);

    PrintTree(avl3);
    printf("Remove 3\n");
    AvlRemove(avl3, &c);
    PrintTree(avl3);
    TEST_IS_EQUAL("RemoveTest RR", AvlHeight(avl3), 2);
    /******************************/
    AvlInsert(avl4, &b);
    AvlInsert(avl4, &a);
    AvlInsert(avl4, &d);
    AvlInsert(avl4, &c);

    PrintTree(avl4);
    printf("Remove 1\n");
    AvlRemove(avl4, &a);
    PrintTree(avl4);
    TEST_IS_EQUAL("RemoveTest RR", AvlHeight(avl4), 2);
    /******************************/

    AvlDestroy(avl1);
    AvlDestroy(avl2);
    AvlDestroy(avl3);
    AvlDestroy(avl4);
}

void InsertTest()
{
    int a = 1;
    int b = 2;
    int c = 3;
    
    avl_t * avl1 = AvlCreate (CmpInt);
    avl_t * avl2 = AvlCreate (CmpInt);
    avl_t * avl3 = AvlCreate (CmpInt);
    avl_t * avl4 = AvlCreate (CmpInt);

    printf(YELLOW"InsertTest\n"WHITE);

    AvlInsert(avl1, &a);
    AvlInsert(avl1, &b);
    AvlInsert(avl1, &c);
  
    TEST_IS_EQUAL("InsertTest LL", AvlHeight(avl1), 2);   
    PrintTree(avl1);

    AvlInsert(avl2, &c);
    AvlInsert(avl2, &b);
    AvlInsert(avl2, &a);

    TEST_IS_EQUAL("InsertTest RR", AvlHeight(avl2), 2);   
    PrintTree(avl2);

    AvlInsert(avl3, &c);
    AvlInsert(avl3, &a);
    AvlInsert(avl3, &b);

    TEST_IS_EQUAL("InsertTest RL", AvlHeight(avl3), 2);   
    PrintTree(avl3);

    AvlInsert(avl4, &a);
    AvlInsert(avl4, &c);
    AvlInsert(avl4, &b);

    TEST_IS_EQUAL("InsertTest LR", AvlHeight(avl4), 2);   
    PrintTree(avl4);

    AvlDestroy(avl1);
    AvlDestroy(avl2);
    AvlDestroy(avl3);
    AvlDestroy(avl4);
}

void TestAvlForEach()
{
    avl_t *avl = NULL;
	static int sum = 0;
    
    int a = 6;
	int b = 3;
	int c = 8;
	int d = 1;
    int e = 4;

	avl = AvlCreate(CmpInt);

    printf(YELLOW"TestAvlForEach\n"WHITE);

	AvlInsert(avl, &a);
	AvlInsert(avl, &b);
	AvlInsert(avl, &c);
	AvlInsert(avl, &d);
    AvlInsert(avl, &e);

	AvlForeach(avl, Sum, (void *) &sum, INORDER);
    TEST_IS_EQUAL("TestAvlSize", sum, 22);

    AvlForeach(avl, AvlPrintNode, NULL, INORDER);
    printf("\n");
    AvlForeach(avl, AvlPrintNode, NULL, POSTORDER);
    printf("\n");
    AvlForeach(avl, AvlPrintNode, NULL, PREORDER);
    printf("\n");

    AvlDestroy(avl);
}

int CmpInt(const void *new_data, const void *curr_data)
{
    return ((*(int *)new_data) - (*(int *)curr_data));
}

int Sum(void *data, void *param)
{
	int *sum = param; 

	*sum += *(int *)data;

	return 0;

}

static int AvlPrintNode(void *data, void *param)
{
    (void)param;
    printf("%d, ", *(int *)(data));

    return SUCCESS;
}