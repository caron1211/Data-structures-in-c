#include <stdio.h> /* printf */
#include <stdlib.h> /* system */

#include "avl.h"
/*#include "utilities.h"*/

/********************************* test utilities ******************************/


/*********************** color macros for printf *********************************/

#define RED "\033[1;91m"           /* print in red          */
#define GREEN "\033[1;38;5;47m"    /* print in green        */
#define MAGEN "\033[0;38;5;183m"   /* print in magenta      */
#define YELLOW "\033[1;38;5;220m"  /* print in light yellow */
#define CYAN "\033[0;96m"          /* print in cyan         */
#define LIGHT "\033[2;37m"         /* print in gray         */
#define PURPLE "\033[1;4;38;5;224m"/* print in purple       */
#define RESET "\033[1;0m"          /* reset print color     */


/********************************** Test MACRO's *********************************/

#define TEST(name, real, expected) printf("\n%s: %s", name, real == expected\
									? GREEN"SUCCESS\n"RESET\
									: RED"\aFAILURE\n"RESET)
									
#define NOT_TEST(name, real, expected) printf("\n%s: %s", name, real != expected\
									? GREEN"SUCCESS\n"RESET\
									: RED"\aFAILURE\n"RESET)									

/********************************* others: ******************************/
/***************     DECLERATIONS      ***************/
void Test(); 

int CmpInt(const void *new_data, const void *curr_data);
static int PrintData(void *data, void *param);

int main()
{
    system("clear");
    
    Test();
    return 0; 
}

void Test()
{
    avl_t *avl = NULL;
    size_t height = 0, size = 0;
    int res = 0;
    void *data = NULL;

    printf(CYAN"\tTest\n\n"RESET); 
    
    avl = AvlCreate(CmpInt);
    NOT_TEST("AvlCreate(CmpInt)", avl, NULL);

    res = AvlIsEmpty(avl);
    TEST("AvlIsEmpty(avl)", res, 1);

    AvlInsert(avl, (void *)50);
    AvlInsert(avl, (void *)55);
    AvlInsert(avl, (void *)60);
    AvlInsert(avl, (void *)40);
    AvlInsert(avl, (void *)30);
    AvlInsert(avl, (void *)45);
    AvlInsert(avl, (void *)10);
    
    printf("\nAvlInsert(avl, 50) \n");
    printf("AvlInsert(avl, 55) \n");
    printf("AvlInsert(avl, 60) \n");
    printf("AvlInsert(avl, 40) \n");
    printf("AvlInsert(avl, 30) \n");
    printf("AvlInsert(avl, 45) \n");
    printf("AvlInsert(avl, 10) \n");
    size = AvlSize(avl);
    TEST("AvlInsert: ", size, 7);

    TEST("AvlSize(avl)", size, 7);

    res = AvlIsEmpty(avl);
    TEST("AvlIsEmpty(avl)", res, 0);

    printf(CYAN"\n\n          AVL - foreach     \n\n"RESET);
    printf(CYAN"pre order\n"RESET);
    printf("expected: 50, 40, 30, 10, 45, 55, 60.\n");
    printf("real    : ");
    AvlForeach(avl, PrintData, NULL, PREORDER);
    printf("\b\b. \n\n");

    printf(CYAN"in order\n"RESET);
    printf("expected: 10, 30, 40, 45, 50, 55, 60.\n");
    printf("real    : ");
    AvlForeach(avl, PrintData, NULL, INORDER);
    printf("\b\b. \n\n");

    printf(CYAN"post order\n"RESET);
    printf("expected: 10, 30, 45, 40, 60, 55, 50.\n");
    printf("real    : ");
    AvlForeach(avl, PrintData, NULL, POSTORDER);
    printf("\b\b. \n\n");

    height = AvlHeight(avl);
    TEST("AvlHeight(avl): ", height, 4);

    data = AvlFind(avl, (void *)50);
    TEST("AvlFind(avl, 50): ", 50, *(int*)&data);

    data = AvlFind(avl, (void *)4); 
    TEST("AvlFind(avl, 4): ", data, NULL);

    AvlRemove(avl, (void*)60);
    size = AvlSize(avl);
    TEST("AvlRemove(avl, 60) ", size, 6);

    height = AvlHeight(avl);
    TEST("AvlHeight(avl): ", height, 3);

    TEST("AvlSize(avl)", size, 6);

    AvlDestroy(avl);

    printf(CYAN"\n\tEnd Test\n\n"RESET); 
}

/*************         USER FUNCS     ***************/

int CmpInt(const void *new_data, const void *curr_data)
{
    return (*(int *)&new_data - *(int *)&curr_data);
}

static int PrintData(void *data, void *param)
{
    (void)param;
    printf("%d, ", *(int*)&(data));

    return SUCCESS;
}