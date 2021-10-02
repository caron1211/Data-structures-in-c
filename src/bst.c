#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include "bst.h"

typedef enum children
{
	LEFT = 0,
	RIGHT ,
	NUM_OF_CHILDREN 
}children_t;

struct bst_node 
{
	void *data;
	bst_node_t *parent;
	bst_node_t *children[NUM_OF_CHILDREN];
};

struct bst
{
	cmp_func_t cmp_func;
	void *param;
	bst_node_t root_stub;
};

void Print2DInt(bst_iter_t root, int space);
void Print2DStr(bst_iter_t root, int space);

static int CountNodes(void *data, void *count);
static bst_iter_t MostRightLeftInSubTree(bst_iter_t itr, children_t direction);
static bst_iter_t BstPrevNext(bst_iter_t itr, children_t direction);
static int IsLeftRightSon(bst_iter_t son, children_t direction);
static int IsLeaf(bst_iter_t itr);
static bst_iter_t DeleteNode(bst_iter_t itr);
static bst_iter_t SwapNodeData(bst_iter_t itr1, bst_iter_t itr2);

bst_t *BstCreate(cmp_func_t cmp_func, void *param) 
{ 
	bst_t *bst = (bst_t *) malloc(sizeof(bst_t));
	bst_node_t stub = {NULL};
	
	if (NULL == bst)
	{
		return NULL;
	}
	
	bst->root_stub = stub;
	
	bst->cmp_func = cmp_func;
	bst->param = param;

	return bst;
}


void BstDestroy(bst_t *tree) 
{ 
	bst_node_t *curr = NULL;

	assert(NULL != tree);

    curr = tree->root_stub.children[LEFT];
	
	while (!BstIsEmpty(tree))
	{
		if (curr->children[LEFT] != NULL)
		{
			curr = MostRightLeftInSubTree(curr, LEFT);
		}
		
		if (curr->children[RIGHT] != NULL)
		{
            curr = curr->children[RIGHT];
		}

		if (IsLeaf(curr))
		{
			curr = DeleteNode(curr);
		}
	}

	free(tree);
}

bst_iter_t BstInsert(bst_t *tree, void *data) 
{ 
	bst_node_t *curr = NULL;
	bst_node_t *parent = NULL;
	bst_node_t *new_node = NULL;
	children_t to_insert = LEFT;
	
	assert(NULL != tree);
	
	new_node = (bst_node_t *) malloc(sizeof(bst_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	
	new_node->data = data;
	new_node->children[LEFT] = NULL;
	new_node->children[RIGHT] = NULL;
	
	curr = tree->root_stub.children[LEFT];
	parent = &(tree->root_stub);
	
	while (NULL != curr)
	{
		parent = curr;
		
		if (tree->cmp_func(curr->data, data, tree->param) < 0)
		{
			curr = curr->children[LEFT];
			to_insert = LEFT;
		}
		else
		{
			curr = curr->children[RIGHT];
			to_insert = RIGHT;
		}
	}
	
	parent->children[to_insert] = new_node;
	new_node->parent = parent;
	
	return (new_node);
}

void BstRemove(bst_iter_t itr) 
{ 
	bst_node_t *pred_node = NULL;

	assert(NULL != itr);
	assert(itr->parent != NULL);
	
    while (!IsLeaf(itr))
	{
	    if (itr->children[LEFT] != NULL && itr->children[RIGHT] != NULL)
		{
			pred_node = BstPrev(itr);
			itr = SwapNodeData(itr, pred_node);
		}

		else if (itr->children[LEFT] != NULL)
		{
             itr = SwapNodeData(itr, itr->children[LEFT]);
		}

		else 
		{
			itr = SwapNodeData(itr, itr->children[RIGHT]);
		}
	}
	DeleteNode(itr);
}

void *BstGetData(const bst_iter_t itr) 
{  
	assert(NULL != itr); 
	return (itr->data);
}


size_t BstSize(const bst_t *tree) 
{ 
	size_t count = 0;
	
	assert(NULL != tree); 

	BstForeach(BstBegin(tree), BstEnd(tree) , CountNodes, &count);
	
	return (count);
}

bst_iter_t BstFind(const bst_t *tree, void *data) 
{ 
    bst_node_t *curr = NULL;
	int found = 0;
	int res_of_cmp = 0;

	assert(NULL != tree);

	curr = tree->root_stub.children[LEFT];

	while (NULL != curr && !found)
	{
        res_of_cmp = tree->cmp_func(curr->data, data, tree->param);
		if (res_of_cmp < 0)
		{
			curr = curr->children[LEFT];
		}
		else if (res_of_cmp > 0)
		{
			curr = curr->children[RIGHT];
		}
		else
		{
			found = 1;
		}
	}

	if (curr == NULL)
	{
        return BstEnd(tree);
	}

	return curr;

} 

bst_iter_t BstBegin(const bst_t *tree) 
{
	bst_node_t *root = NULL;

	assert(NULL != tree);

	root = (bst_node_t *) &(tree->root_stub);

	return (MostRightLeftInSubTree(root, LEFT));
}

bst_iter_t BstEnd(const bst_t *tree) 
{ 
	bst_node_t *root = NULL;

	assert(NULL != tree);

	root = (bst_node_t *) &(tree->root_stub);

	return (root);
}

bst_iter_t BstNext(bst_iter_t itr) 
{
	assert(NULL != itr);
	
	return (BstPrevNext(itr, RIGHT));
}

bst_iter_t BstPrev(bst_iter_t itr) 
{
	assert(NULL != itr);

	return (BstPrevNext(itr, LEFT));
}


int BstIsEmpty(const bst_t *tree) 
{
	assert(NULL != tree); 
 	return (tree->root_stub.children[LEFT] == NULL);
}


int BstForeach(bst_iter_t from, bst_iter_t to , action_func_t action_func, void *param) 
{ 
	int status = 0;
	
	assert(NULL != from);
	assert(NULL != to);

    while (!BstIterIsEqual(from, to) && status == 0)
	{
		status = action_func(BstGetData(from), param);
		from = BstNext(from);
	}
	
	return status;
}


int BstIterIsEqual(bst_iter_t iter1, bst_iter_t iter2) 
{ 
	return (iter1 == iter2);
}

static bst_iter_t BstPrevNext(bst_iter_t itr, children_t direction) 
{
	if (itr->children[direction] != NULL)
	{
		 itr = itr->children[direction];
		return (MostRightLeftInSubTree(itr, !direction));
	}

	else 
	{
		while (!IsLeftRightSon(itr, !direction))
		{
			itr = itr->parent;
		}
			
		return (itr->parent);
	}
}

static bst_iter_t DeleteNode(bst_iter_t itr)
{
	bst_node_t *tmp = NULL;
    tmp = itr->parent;
	if (IsLeftRightSon(itr, LEFT))
	{
        tmp->children[LEFT] = NULL;
	}

	else
	{
		tmp->children[RIGHT] = NULL;
	}
			
	free(itr);
	itr = tmp;

	return itr;
}

static int IsLeftRightSon(bst_iter_t son, children_t direction)
{
	return (son->parent->children[direction] == son);
}

static int IsLeaf(bst_iter_t itr)
{
	return (itr->children[LEFT] == NULL && itr->children[RIGHT] == NULL);
}

static bst_iter_t MostRightLeftInSubTree(bst_iter_t itr, children_t direction)  
{ 
	assert(NULL != itr);

	while (itr->children[direction] != NULL)
	{
		itr = itr->children[direction];
	}
	
	return (itr);
}

static bst_iter_t SwapNodeData(bst_iter_t itr1, bst_iter_t itr2)
{
	void *tmp = NULL;

	tmp = itr2->data;
	itr2->data = itr1->data;
	itr1->data = tmp;

	return itr2;
}

static int CountNodes(void *data, void *count)
{
	(void)data;
	++*(size_t*)count;
	
	return (0);
}

void Print2DInt(bst_iter_t root, int space)
{
    int i = 0;
    if (root == NULL)
        return;
        
    space += 10;
    
    Print2DInt(root->children[RIGHT], space);
    
    printf("\n");
    for(i = 10; i < space; ++i)
        printf(" ");
    printf("%d\n", *(int *)(root->data));
    
    Print2DInt(root->children[LEFT], space);
}

void Print2DStr(bst_iter_t root, int space)
{
    int i = 0;
    if (root == NULL)
        return;
        
    space += 10;
    
    Print2DStr(root->children[RIGHT], space);
    
    printf("\n");
    for(i = 10; i < space; ++i)
        printf(" ");
    printf("%s\n", (char *)(root->data));
    
    Print2DStr(root->children[LEFT], space);
}

void print2DUtil(bst_node_t *tree, int space)
{
    int i = 0;
    if (tree == NULL || tree->data == NULL)
    {
        return;
    }

    space += 10;

    print2DUtil(tree->children[RIGHT], space);
    

    printf("\n");
    for (i = 10; i < space; i++)
    {
    printf(" ");
    }


    if(tree->data != NULL)
    printf("%d\n", *(int *)tree->data);
    
    print2DUtil(tree->children[LEFT], space);
}