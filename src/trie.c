#include <stdlib.h> /* malloc, free */
#include <assert.h> /*assert*/
#include "trie.h"


#define RIGHT_CHILD(NODE) (NODE->children[RIGHT])
#define LEFT_CHILD(NODE) (NODE->children[LEFT]) 

static int const TRUE = 1;
static int const FALSE = 0;


typedef struct trie_node trie_node_t;

typedef enum children
{
	LEFT = 0,
	RIGHT ,
	NUM_OF_CHILDREN 
}children_t;

struct trie
{
    trie_node_t *root;
    size_t height;
    size_t num_of_elements;
};

struct trie_node
{
	trie_node_t *parent;  
    int is_full;
	trie_node_t *children[NUM_OF_CHILDREN];
};

static int IsLeaf(trie_node_t * node);
static int IsLeftRightSon(trie_node_t * son, children_t direction);
static trie_node_t *DeleteNode(trie_node_t * curr_node);
static children_t GetDirection( size_t data, size_t height);
static trie_node_t *CreateNode();
static children_t GetNextBit(size_t data, size_t index);
static void GoMostLeft(trie_node_t *current, size_t *data, int index);
static void GoMostLeft(trie_node_t *current, size_t *data, int index);


trie_t *TrieCreate(size_t height)
{
    trie_t * trie = NULL;
    trie_node_t *root = NULL;

    trie = malloc(sizeof(trie_t));
    if (trie == NULL)
    {
        return NULL;
    }

    root = calloc(1, sizeof(trie_node_t));
    /* return NULL */
    root->is_full = 0;

    trie->root = root;
    trie->height = height;
    trie->num_of_elements = 0;
    
    return trie;
}

void TrieDestroy(trie_t *trie)
{
   trie_node_t *curr = NULL;

	assert(NULL != trie);

    curr = trie->root;
	
	while (!TrieIsEmpty(trie))
	{
		if (curr->children[LEFT] != NULL)
		{
			curr = curr->children[LEFT];
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

    free(trie->root);
    free(trie);
}

static trie_node_t *DeleteNode(trie_node_t * curr_node)
{
	trie_node_t *tmp = NULL;
    tmp = curr_node->parent;
	if (IsLeftRightSon(curr_node, LEFT))
	{
        tmp->children[LEFT] = NULL;
	}

	else
	{
		tmp->children[RIGHT] = NULL;
	}

	free(curr_node);
	curr_node = tmp;

	return curr_node;
}

static int IsLeftRightSon(trie_node_t * son, children_t direction)
{
	return (son->parent->children[direction] == son);
}

int TrieIsEmpty(trie_t *trie)
{
    return (RIGHT_CHILD(trie->root) == NULL && LEFT_CHILD(trie->root) == NULL);
}

static int IsLeaf(trie_node_t * node)
{
	return (node->children[LEFT] == NULL && node->children[RIGHT] == NULL);
}

static children_t GetDirection( size_t data, size_t height)
{
    size_t mask = 0x01;
    size_t res = 0;

    mask = mask << (height - 1);
    res = data & mask;

    return (res == 0 ? LEFT : RIGHT);
}

static trie_node_t *CreateNode()
{
    trie_node_t *new_node = (trie_node_t*)malloc(sizeof(trie_node_t));
    if (NULL == new_node)
    {
        return NULL;
    }
    new_node->parent = NULL;
    new_node->children[0] = NULL;
    new_node->children[1] = NULL;
    new_node->is_full = 0;

    return new_node;    
}

static void UpdateIsFull(trie_node_t * leaf)
{
    trie_node_t * curr_node = leaf;
    while(curr_node != NULL &&  curr_node->parent != NULL )
    {
        curr_node = curr_node->parent;

        if (curr_node->children[LEFT] == NULL || curr_node->children[RIGHT] == NULL)
        {
            curr_node->is_full = 0;
            break;
        }
        if (curr_node->children[LEFT]->is_full && curr_node->children[RIGHT]->is_full)
        {
            curr_node->is_full = 1;
        }
        else 
        {
            curr_node->is_full = 0;
        }

    }
}

trie_status_t TrieInsert(trie_t *trie, size_t data)
{
    size_t curr_height = 0;
    children_t direction = LEFT;
    trie_node_t * curr_node = NULL;
    trie_node_t * tmp = NULL;

    assert(NULL != trie);
    
    curr_node = trie->root;
    curr_height = trie->height;

    if (curr_node->is_full == 1)
    {
        return TRIE_FULL;
    }

    while(curr_height > 0)
    {
        direction = GetDirection(data, curr_height);

       
        if (curr_node->children[direction] == NULL)
        {
            tmp = CreateNode();
            if (tmp == NULL)
            {
                return MALLOC_FAIL;
            }
            tmp->parent = curr_node;
            curr_node->children[direction] = tmp;
        }

        curr_node = curr_node->children[direction];

        if (curr_node->is_full == 1)
        {
            return PATH_OCCUPIED;
        }
        curr_height--;
    }

    trie->num_of_elements++;
    curr_node->is_full = 1;
    UpdateIsFull(curr_node);

    return (SUCCESS);
}

int TrieIsFound(trie_t *trie, size_t data)
{
    size_t curr_height = 0;
    trie_node_t * curr_node = NULL;
    children_t direction = LEFT;

    curr_node = trie->root;
    curr_height = trie->height;

    while (curr_height > 0)
    {
        direction = GetDirection(data, curr_height);
        if (curr_node->children[direction] == NULL || curr_node->is_full == 1)
        {
            return FALSE;
        }

        curr_node = curr_node->children[direction];
        curr_height--;
    }

    return TRUE;
}

trie_status_t TrieRemove(trie_t *trie, size_t data)
{
    size_t curr_height = 0;
    trie_node_t * curr_node = NULL;
    trie_node_t * tmp = NULL;
    children_t direction = LEFT;

    curr_node = trie->root;
    curr_height = trie->height;

    while (curr_height > 0)
    {
        direction = GetDirection(data, curr_height);
        if (curr_node->children[direction] == NULL)
        {
            return PATH_INVALID;
        }

        curr_node = curr_node->children[direction];
        curr_height--;
    }

    trie->num_of_elements--;

    tmp = curr_node->parent;
    DeleteNode(curr_node);

    /* delete all single children in path */ 
    while( tmp != trie->root)
    {
        if (tmp->parent->children[!direction] == NULL)
        {
            tmp = tmp->parent;
            DeleteNode(tmp);
        }

        else 
        {
            break;
        }
    }
   
    tmp->is_full = 0;
    UpdateIsFull(tmp);

    return (SUCCESS);
}

size_t TrieCount(trie_t *trie)
{
    return (trie->num_of_elements);
}

trie_status_t TrieNextAvailable(const trie_t *trie, size_t *data)
{
    int i = 0; 
    int direction = 0; 
    trie_node_t *current = NULL; 

    assert(trie); 

    current = trie->root; 

    for (i = trie->height - 1; i >= 0; i--)
    {
        direction = GetNextBit(*data, i);

        if (current->children[direction] == NULL)
        {
            return SUCCESS; 
        }

        if (current->is_full)
        {
            break;
        }

        current = current->children[direction];
    }

    
    while (current->parent != NULL)
    {
       if (current->parent->children[0] == current && current->parent->is_full == 0)  
        {
            *data |= 1; 
            i--; 
            GoMostLeft(current->parent->children[1], data, (int)i); 
            return SUCCESS; 
        }
        
        *data >>= 1;  
        i++; 
        current = current->parent;
    }

    return TRIE_FULL;
}


static void GoMostLeft(trie_node_t *current, size_t *data, int index)
{
    for ( ;index >= 0; index--)
    {
        if (current == NULL)
        {
            *data <<= 1;
        }
        
        else if (current->children[LEFT]== NULL)
        {
            *data <<= 1;
        }

        else if(current->children[LEFT]->is_full != 1)
        {
            current = current->children[LEFT];
            *data <<= 1;
        }
        
        else
        {
            current = current->children[RIGHT];
            *data <<= 1;
            *data |= 1; 
        }
    }
}

static children_t GetNextBit(size_t data, size_t index)
{
    size_t mask = 1;
    data >>= index;
    return (data & mask);
}


void Print2DInt(trie_node_t *root, int space)
{
    int i = 0;
        
    space += 10;
    if (root == NULL)
    {
        return;
    }
    Print2DInt(root->children[1], space);
    
    printf("\n");
    for(i = 10; i < space; ++i)
         printf(" ");
    printf("d:%d", (root->is_full));    
    Print2DInt(root->children[0], space);
}

void PrintTrie(trie_t *trie)
{
    Print2DInt(trie->root, 5);
    printf("\n\n");

}