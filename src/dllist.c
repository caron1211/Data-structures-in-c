#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc, free*/
#include "dllist.h"

typedef struct node node_t;

static int CountElements(void *data, void *param);
static dlist_iter_t GoToTail(dlist_iter_t iterator);

struct node
{
	void *data;
	node_t *next;
	node_t *prev;

};

struct dlist
{
	node_t head;
	node_t tail; 
};

dlist_t *DlistCreate(void)
{
	dlist_t *list = (dlist_t*)malloc(sizeof(dlist_t));
	if(NULL == list)
	{
		return NULL;
	}
	
	list->head.data = list;
	list->head.next = &(list->tail);
	list->head.prev = NULL;
	
	list->tail.data = list;
	list->tail.next = NULL;
	list->tail.prev = &(list->head);
	
	return list;
}


void DlistDestroy(dlist_t *list)
{
	node_t *p = NULL;
	node_t *tmp = NULL;
	assert(NULL != list);
	
	p = DlistBegin(list);
	while (NULL != p->next)
	{
		tmp = p;
		p = p->next;
		free(tmp);
	}
	
	free(list);
	list = NULL;
}


dlist_iter_t DlistInsert(dlist_iter_t iterator, void *data)
{
	node_t *new_node = NULL;
	node_t *prev_node = NULL;
	
	assert(NULL != iterator);
	assert(NULL != iterator->prev);
	
	new_node = (node_t*)malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		while(NULL != iterator->next)
		{
			iterator = iterator->next;
		}
		return iterator;
	}
	
	prev_node = iterator->prev;
	
	new_node->next = iterator;
	new_node->prev = prev_node;
	new_node->data = data;
	
	prev_node->next = new_node;
	iterator->prev = new_node;
	
	
	return new_node;

}


dlist_iter_t DlistPushFront(dlist_t *dlist, void *data)
{
	node_t *begin_node = NULL;
	
	assert(NULL != dlist);
	
	begin_node = DlistBegin(dlist);
	return(DlistInsert(begin_node, data));
	
}


dlist_iter_t DlistPushBack(dlist_t *dlist, void *data)
{
	node_t *end_node = NULL;
	
	assert(NULL != dlist);
	
	end_node = DlistEnd(dlist);
	return(DlistInsert(end_node, data));
}


dlist_iter_t DlistRemove(dlist_iter_t iterator)
{
	node_t *next_node = NULL;
	node_t *prev_node = NULL;
	
	assert(iterator != NULL);
	assert(NULL != iterator->next);
	assert(NULL != iterator->prev);
	
	prev_node = iterator->prev;
	next_node = iterator->next;
	
	next_node->prev = prev_node;
	prev_node->next = next_node;
	
	free(iterator);
	iterator = NULL;
	
	return(next_node);
}


void *DlistPopFront(dlist_t *dlist)
{
	node_t *begin_node = NULL;
	void * data = NULL;
	
	assert(NULL != dlist);
	
	begin_node = DlistBegin(dlist);
	data =  DlistGetData(begin_node);
	DlistRemove(begin_node);
	
	return(data);
}


void *DlistPopBack(dlist_t *dlist)
{
	node_t *end_node = NULL;
	void * data = NULL;
	
	assert(NULL != dlist);
	
	end_node = DlistEnd(dlist)->prev;
	data =  DlistGetData(end_node);
	DlistRemove(end_node);
	
	return(data);
}


void *DlistGetData(const dlist_iter_t iterator)
{
	assert(NULL != iterator);
	assert(NULL != iterator->next);
	
	return (iterator->data);
}


size_t DlistSize(const dlist_t *list)
{
	size_t count = 0;
	void *count_ptr =&count;
	
	assert(NULL != list);
	
	DlistForeach(DlistBegin(list), DlistEnd(list), CountElements, count_ptr);
	
	return count;
}


dlist_iter_t DlistFindOne(dlist_iter_t from, dlist_iter_t to, is_equal_func_t is_equal_func, const void *param) 
{
	dlist_iter_t itr = NULL;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != is_equal_func);
	
	itr = (dlist_iter_t) from;
	while (itr != to && 1 != is_equal_func(DlistGetData(itr), (void *)param))
	{
		itr = DlistNext(itr);
	}
	
	if(itr == to)
	{
		return(GoToTail(itr));
	}
	
	return itr;
}


int DlistFindMulti(dlist_iter_t from, dlist_iter_t to, is_equal_func_t is_equal_func, const void *param, dlist_t *dest) 
{
	int count = 0;
	dlist_iter_t itr = NULL;
	
	assert(NULL != from);
	assert(NULL != to);
	assert(DlistIsEmpty(dest));
	
	from = DlistFindOne(from, to, is_equal_func, param);
	while (NULL != from->next) 
	{
		
		from = DlistFindOne(DlistNext(from), to, is_equal_func, param);
		itr = DlistPushBack(dest, &from);
		if (DlistEnd(dest) == itr)
		{
			return -1;
		}
		++count;
	}
	
	return count;
}


dlist_iter_t DlistBegin(const dlist_t *list)
{
	assert(NULL != list);
	return(list->head.next);
}


dlist_iter_t DlistEnd(const dlist_t *list)
{
	assert(NULL != list);
	return((dlist_iter_t) &(list->tail));
}


dlist_iter_t DlistNext(dlist_iter_t iterator)
{
	assert(NULL != iterator);
	assert(NULL != iterator->next);
	
	return(iterator->next);
}

dlist_iter_t DlistPrev(dlist_iter_t iterator)
{
	assert(NULL != iterator);
	return(iterator->prev);
}


int DlistIsEmpty(const dlist_t *list)
{
	return(list->head.next == &(list->tail));
}


int DlistForeach(dlist_iter_t from, dlist_iter_t to , action_func_t action_func, void *param)
{

	int status = 0;
	 
	assert(NULL != from);
	assert(NULL != to);
	
	while (from != to && 0 == status)
	{
		status = action_func(DlistGetData(from), param);
		from = DlistNext(from);
		if (1 == status)
		{
			return(status);
		}
	}
	
	return 0;	
}


dlist_iter_t DlistSplice(dlist_iter_t dest, dlist_iter_t from, dlist_iter_t to)
{
	 dlist_iter_t tmp = from->prev;
    
    assert(NULL != dest && NULL != dest->prev);
    assert(NULL != from && NULL != from->prev && NULL != from->next);
    assert(NULL != to);

    from->prev->next = to;
    
    to->prev->next = dest;
    dest->prev->next = from;
    from->prev = dest->prev;
    dest->prev = to->prev;
    
    to->prev = tmp;
    
    
    return dest;
}


int DlistIsSameIter(dlist_iter_t iterator1, dlist_iter_t iterator2)
{
	assert(NULL != iterator1);
	assert(NULL != iterator2);
	
	
	return(iterator1 == iterator2);
}

static dlist_iter_t GoToTail(dlist_iter_t iterator)
{
	while(NULL != iterator->next)
		{
			iterator = iterator->next;
		}
		
		return iterator;

}
static int CountElements(void *data, void *param)
{
	size_t *count = NULL;
	data = data;
	
	assert(param);
	
	count = (size_t *)param;
	++(*count);
	
	return 0;
}

