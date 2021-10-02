#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <stdio.h> /* printf */
#include <sys/types.h> /*ssize_t*/

#include "avl.h"

typedef struct avl_node avl_node_t;

typedef enum children
{
	LEFT = 0,
	RIGHT,
	NUM_OF_CHILDREN
}children_t;


struct avl_node 
{
	void *data;
	size_t height;
	avl_node_t *children[NUM_OF_CHILDREN];
};

struct avl
{
	cmp_func_t cmp_func;
	avl_node_t *root;
};

void print2DUtil(avl_node_t *tree, int space);
void PrintTree(avl_t *tree);
void PrintHeight(avl_node_t *root, int space);


static void InOrder(avl_node_t *node, action_func_t action_func, void *param, int* status);
static void PostOrder(avl_node_t *node, action_func_t action_func, void *param, int *status);
static void PreOrder(avl_node_t *node, action_func_t action_func, void *param, int *status);
static void FreeTree(avl_node_t *node);

static size_t GetMaxHeight(avl_node_t *parent);
static void *GetLeftestNode(avl_node_t * node);
static avl_node_t *RemoveNode(avl_node_t * node, cmp_func_t cmp_func, const void * data);
static int CountNodes(void *data, void *count);
static avl_node_t *InsertNode(avl_node_t *node, cmp_func_t cmp_func, avl_node_t *new_node);
static size_t GetNodeHeight(avl_node_t *node);
static avl_node_t *Rotate(avl_node_t *root, children_t side);
static avl_node_t *BalanceTree(avl_node_t *root);

static children_t GetHeighestChild(avl_node_t *node);
static int IsUnbBalance(avl_node_t *node);

avl_t *AvlCreate(cmp_func_t cmp_func)
{
    avl_t *avl = NULL;

    assert(NULL != cmp_func);

    avl = malloc(sizeof(avl_t));
    if (NULL == avl)
    {
        return NULL;
    }

    avl->cmp_func = cmp_func;
    avl->root = NULL;

    return (avl);
}

void AvlDestroy(avl_t *tree)
{
    assert(NULL != tree);
    FreeTree(tree->root);
    free(tree);
}

static void FreeTree(avl_node_t *node)
{
    if ( node == NULL)
    {
        return;
    }

    FreeTree(node->children[LEFT]);
    FreeTree(node->children[RIGHT]);
    free(node);
}

status_t AvlInsert(avl_t *tree, void *data)
{
    avl_node_t *new_node = NULL;

    assert(NULL != tree);
    
    new_node = malloc(sizeof(avl_node_t));
    if (new_node == NULL)
    {
        return FAIL;
    }
    new_node->data = data;
    new_node->height = 1;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;

    tree->root = InsertNode(tree->root, tree->cmp_func, new_node);
  
    return SUCCESS;
}

static avl_node_t *InsertNode(avl_node_t *root, cmp_func_t cmp_func, avl_node_t *new_node)
{
    int cmp_res = 0;
    children_t side = 0;

    if (root == NULL)
    {
        return(new_node);
    }

    cmp_res = cmp_func(new_node->data , root->data);
    side = cmp_res < 0 ? LEFT : RIGHT;
    
    root->children[side] = InsertNode(root->children[side], cmp_func, new_node);
    root->height = GetMaxHeight(root) +1;
    
    if(IsUnbBalance(root))
    {
        return (BalanceTree(root));
    }
    
    return root;
}

status_t AvlRemove(avl_t *tree, const void *data)
{
    assert(NULL != tree);
    tree->root = RemoveNode(tree->root, tree->cmp_func, data);
   
    return SUCCESS;
}

static avl_node_t *RemoveNode(avl_node_t * node, cmp_func_t cmp_func, const void * data)
{
    int cmp_res = 0;
    children_t side = 0;
    avl_node_t * tmp = NULL;

    if (node == NULL)
    {
        return NULL;
    }

    cmp_res = cmp_func(data , node->data);
    if (cmp_res != 0)
    {
        side = cmp_res < 0 ? LEFT : RIGHT;
        node->children[side] =  RemoveNode(node->children[side],cmp_func, data);
    }

    else 
    {
        if (node->children[LEFT] == NULL && node->children[RIGHT] == NULL)
        {
            free(node);
            return NULL;
        }
        else if (node->children[LEFT] == NULL || node->children[RIGHT] == NULL)
        {
            side = cmp_res < 0 ? LEFT : RIGHT;
            tmp = node->children[side];
            free(node);
            return (tmp);
        }
        
        else
        {
            node->data = GetLeftestNode(node->children[RIGHT]);
            node->children[RIGHT] = RemoveNode(node->children[RIGHT],cmp_func, node->data);
        }
    }
    node->height = GetMaxHeight(node) +1 ;
    if(IsUnbBalance(node))
    {
        return (BalanceTree(node));
    }

    return node;
}

static void *GetLeftestNode(avl_node_t * node)
{
    avl_node_t *tmp = node;

    while(tmp->children[LEFT] != NULL)
    { 
        tmp = node->children[LEFT];
    }

    return (tmp->data);
}

static avl_node_t *FindNode(avl_node_t * node, cmp_func_t cmp_func, const void * data)
{
    int cmp_res = 0;
    children_t side = 0;
    if (node == NULL)
    {
        return node;
    }

    cmp_res = cmp_func(data , node->data);
    side = cmp_res < 0 ? LEFT : RIGHT;

    if (cmp_res == 0)
    {
        return node;
    }

    return (FindNode(node->children[side], cmp_func, data));
}

void *AvlFind(const avl_t *tree, const void *data)
{
    avl_node_t *avl = NULL;

    assert(NULL != tree);

    avl = FindNode(tree->root, tree->cmp_func, data);
    if (avl == NULL)
    {
        return avl;
    }
    else 
    {
         return (avl->data);
    }
}

static size_t GetNodeHeight(avl_node_t *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return (node->height);
}

static size_t GetMaxHeight(avl_node_t *parent)
{
    
    avl_node_t *left_node = parent->children[LEFT];
    avl_node_t *right_node = parent->children[RIGHT];

    size_t h_left = GetNodeHeight(left_node);
    size_t h_right = GetNodeHeight(right_node);
    return (h_left > h_right ? h_left : h_right);
}

size_t AvlHeight(const avl_t *tree)
{
    assert(NULL != tree);
    return (GetNodeHeight(tree->root));
}

int AvlForeach(avl_t *tree, action_func_t action_func, void *param, order_t order)
{
    int status = SUCCESS;

    assert(NULL != tree);

    switch (order)
    {
        case INORDER:
        {
            InOrder(tree->root, action_func, param, &status);
            break;
        }

        case PREORDER:
        {
            PreOrder(tree->root, action_func, param, &status);
            break;
        }
        case POSTORDER:
        {
            PostOrder(tree->root, action_func, param, &status);
            break;
        }

        default:
            break;
    }

    return status;
}

static void InOrder(avl_node_t *node, action_func_t action_func, void *param, int *status)
{
    if (node == NULL || *status != SUCCESS)
    {
        return;
    }
    InOrder(node->children[LEFT], action_func, param, status);
    *status = action_func(node->data, param);
    InOrder(node->children[RIGHT], action_func, param, status);
}

static void PostOrder(avl_node_t *node, action_func_t action_func, void *param, int *status)
{
    if (node == NULL || *status != SUCCESS)
    {
        return;
    }
    PostOrder(node->children[LEFT], action_func, param, status);
    PostOrder(node->children[RIGHT], action_func, param, status);
    *status = action_func(node->data, param);
}

static void PreOrder(avl_node_t *node, action_func_t action_func, void *param, int *status)
{
    if (node == NULL || *status != SUCCESS)
    {
        return;
    }
    *status = action_func(node->data, param);
    PreOrder(node->children[LEFT], action_func, param, status);
    PreOrder(node->children[RIGHT], action_func, param, status);
}

size_t AvlSize(const avl_t *tree)
{
    size_t count = 0;
    int status = SUCCESS;

    assert(NULL != tree);

    PreOrder(tree->root, CountNodes, &count, &status);

    return (count);
}

static int CountNodes(void *node, void *count)
{
	(void)node;
	++*(size_t*)count;
	
	return (SUCCESS);
}

int AvlIsEmpty(const avl_t *tree)
{
    assert(NULL != tree);
    return (tree->root == NULL);
}

void PrintTree(avl_t *tree)
{
    print2DUtil(tree->root, 10);
    printf("-----\n\n");
}

void print2DUtil(avl_node_t *root, int space)
{
    int i = 0;

    if (root == NULL)
        return;
        
    space += 10;
    
    print2DUtil(root->children[RIGHT], space);
    
    printf("\n");
    for(i = 10; i < space; ++i)
        printf(" ");
    printf("%d (%ld)\n", *(int *)(root->data), root->height);
    
    print2DUtil(root->children[LEFT], space);
}

static avl_node_t *Rotate(avl_node_t *root, children_t side)
{
    avl_node_t *new_root = NULL;
    avl_node_t *sub_tree = NULL;

    new_root = root->children[!side];
    sub_tree = new_root->children[side];

    new_root->children[side] = root;
    root->children[!side] = sub_tree;

    root->height = GetMaxHeight(root) + 1;
    new_root->height = GetMaxHeight(new_root) + 1;

    return (new_root);
}

static children_t GetHeighestChild(avl_node_t *node)
{
    avl_node_t *left_child = node->children[LEFT];
    avl_node_t *right_child = node->children[RIGHT];

    size_t h_left = GetNodeHeight(left_child);
    size_t h_right = GetNodeHeight(right_child);
    return (h_left > h_right ? LEFT : RIGHT);
}

static avl_node_t *BalanceTree(avl_node_t *sub_tree)
{
    children_t first = GetHeighestChild(sub_tree);
    children_t second = GetHeighestChild(sub_tree->children[first]);

    if (first == LEFT && second == LEFT)
    {
        return (Rotate(sub_tree, RIGHT));
    }

    else if (first == RIGHT && second == RIGHT)
    {
       return (Rotate(sub_tree, LEFT));
    }

     else if (first == RIGHT && second == LEFT)
    {
        sub_tree->children[RIGHT] = Rotate(sub_tree->children[RIGHT], RIGHT);
        return (Rotate(sub_tree, LEFT));
    
    }

    else 
    {
        sub_tree->children[LEFT] = Rotate(sub_tree->children[LEFT], LEFT);
        return (Rotate(sub_tree, RIGHT));
    }
}

static int IsUnbBalance(avl_node_t *node)
{
    avl_node_t *left_child = node->children[LEFT];
    avl_node_t *right_child = node->children[RIGHT];
    ssize_t balance = 0;

    balance = GetNodeHeight(right_child) -  GetNodeHeight(left_child);
    if (balance > 1 || balance <-1 )
    {
        return 1;
    }

    return 0;
}