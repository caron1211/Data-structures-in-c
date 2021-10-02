#include <stdio.h>/*printf*/
#include <stdlib.h>/*sysytem*/
#include <time.h> /* clock_t, CLOCKS_PER_SEC, clock */

#include "avl.h"
/*****************************************************************************/
#define RED "\033[5;91m"           /* print in red          */
#define GREEN "\033[1;38;5;47m"    /* print in green        */
#define MAGEN "\033[0;38;5;183m"   /* print in magenta      */
#define YELLOW "\033[1;38;5;220m"  /* print in light yellow */
#define CYAN "\033[0;96m"          /* print in cyan         */
#define LIGHT "\033[2;37m"         /* print in gray         */
#define PURPLE "\033[1;4;38;5;224m"/* print in purple       */
#define RESET "\033[1;0m"          /* reset print color     */

/*****************************************************************************/
#define TEST(name, actual, expected)\
    				printf("%s: %s\n\n", name, actual == expected ?\
     				GREEN"Pass"RESET : RED"Fail"RESET)

#define NOT_TEST(name, actual, expected)\
    				printf("%s: %s\n\n", name, actual != expected ?\
     				GREEN"Pass"RESET : RED"Fail"RESET)			
/****************************************************************************/
int CmpInt(const void *new_data, const void *curr_data);
static int AvlPrintNode(void *data, void *param);

void TestAvlFuncs();
void CompareAvlBst();
/****************************************************************************/
int main()
{
    system("clear");
    TestAvlFuncs();
    return(0);
}

void TestAvlFuncs()
{
    int arr[8] = {0};
    int i = 0;
    status_t status = SUCCESS;
    avl_t *tree = AvlCreate(CmpInt);

    printf(YELLOW"\n***************Testing BST Functions ***************\n\n"RESET);

	NOT_TEST("test AVL create", tree, NULL);

    TEST("Test IsEmpty = 1",AvlIsEmpty(tree), 1);

    TEST("Test AvlSize = 0", AvlSize(tree), 0);

    for(i = 0; i < 8; i++)
    {
        arr[i] = i + 1;
        status = AvlInsert(tree, &(arr[i]));
        if(status != SUCCESS)
        {
            break;
        }
    }
    TEST("insert 1 to 8 status = SUCCESS", status, SUCCESS);

    TEST("Test IsEmpty = 0",AvlIsEmpty(tree), 0);

    TEST("Test AvlSize = 8", AvlSize(tree), 8);

    TEST("Test avl Height = 4", AvlHeight(tree), 4);

    printf(YELLOW"\n***************Print Elements with Foreach***************\n\n"RESET);

    printf("Preorder traversal : \n");
    AvlForeach(tree, AvlPrintNode, NULL, PREORDER);
    printf("\nInorder traversal : \n");
    AvlForeach(tree, AvlPrintNode, NULL, INORDER);
    printf("\nPostorder traversal : \n");
    AvlForeach(tree, AvlPrintNode, NULL, POSTORDER);

    printf(YELLOW"\n***************Print tree***************\n\n"RESET);

    PrintTree(tree);
    printf(YELLOW"\n***************Testing Remove ***************\n\n"RESET);

    status = AvlRemove(tree, &arr[1]); 

    TEST("Test remove elemnet from tree", status, SUCCESS);

    AvlRemove(tree, &(arr[0]) );

    status = AvlRemove(tree, &(arr[0]) );

    TEST("Test remove elemnet not from tree", status, FAIL);

    TEST("Test AvlFind element from tree", AvlFind(tree, &arr[6]), &arr[6]);

    TEST("Test AvlFind element not from tree", AvlFind(tree, &arr[0]), NULL);

    TEST("Test avl Height = 3", AvlHeight(tree), 3);

    printf(YELLOW"\n***************Print tree***************\n\n"RESET);

    PrintTree(tree);

    AvlDestroy(tree);

    printf(YELLOW"\n***************END Test***************\n\n"RESET);

}
/******************************************************************************/
int CmpInt(const void *new_data, const void *curr_data)
{
    return (*(int *)new_data - *(int *)curr_data );
}
/******************************************************************************/
static int AvlPrintNode(void *data, void *param)
{
    (void)param;
    printf("%d, ", *(int*)(data));

    return SUCCESS;
}
