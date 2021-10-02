#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "s_l_list.h"

struct node 
{
	void *data;
	node_t *next;
};

struct list
{
	node_t *head;
	node_t *tail; 
}; 

static int Count(void *data, void *param);

slist_t *SlistCreate(void) 
{
	node_t *dummy = NULL;
	slist_t *list = (slist_t*)malloc(sizeof(slist_t));
	if(NULL == list)
	{
		return NULL;
	}
	dummy = (node_t*)malloc(sizeof(node_t));
	if(NULL == dummy)
	{
		free(list);
		list = NULL;
		return NULL;
	}
	
	dummy->data = (void *)list;
	dummy->next = NULL;
	
	list->head = dummy;
	list->tail = dummy;
	
	return list;
}

void SlistDestroy(slist_t *list)
{
	node_t *p = NULL;
	node_t *tmp = NULL;
	assert(NULL != list);
	
	p = list->head;
	while (NULL != p->next)
	{
		tmp = p;
		p = p->next;
		free(tmp);
	}
	
	free(p); /* free dummy */
	free(list);
	p = NULL;
	list = NULL;
}


slist_iter_t SlistInsert(slist_iter_t iterator, const void *data)
{
	node_t *new_node = NULL;
	slist_t *list = NULL;
	
	
	assert(NULL != iterator);
	
	new_node = (node_t*)malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		while(NULL != iterator->next)
		{
			iterator = iterator->next;
		}
		return iterator;
	}
	
	new_node->data = iterator->data;
	new_node->next = iterator->next;
	
	iterator->data = (void *)data;
	iterator->next = new_node;
	
	if (NULL == new_node->next) 
    {
        list = (slist_t *)new_node->data;
        list->tail = new_node;
    }
	
	return iterator;
}


slist_iter_t SlistRemove(slist_iter_t iterator)
{
	slist_t *list = NULL;
	slist_iter_t next_itr = NULL;
	
	assert(NULL != iterator);
	assert(NULL != iterator->next);
	
	next_itr = iterator->next;
	
	iterator->data = next_itr->data;
	iterator->next = next_itr->next;
	
	free(next_itr);
	next_itr = NULL;
	
	if (NULL == iterator->next)
	{
		list = (slist_t *) iterator->data;
		list->tail = iterator;
	}
	
	return iterator;
}


void *SlistGetData(const slist_iter_t iterator)
{
	assert(NULL != iterator);
	assert(NULL != iterator->next);
	
	return (iterator->data);
}


void SlistSetData(const slist_iter_t iterator, const void *data)
{
	assert(NULL != iterator);
	assert(NULL != iterator->next);
	
	iterator->data = (void *)data;
}

size_t SlistCount(const slist_t *list)
{
	size_t count = 0;
	void *count_ptr =&count;
	
	assert(NULL != list);
	
	SlistForeach(list->head, list->tail, Count, count_ptr);
	
	return count;
}

slist_iter_t SlistFind(const slist_iter_t from, const slist_iter_t to, cmp_func_t func, void *param)
{
	slist_iter_t itr = NULL;
	
	assert(NULL != param);
	assert(NULL != from);
	assert(NULL != to);
	
	itr = (slist_iter_t) from;
	while (itr != to && 1 != func(SlistGetData(itr), param))
	{
		itr = SlistNext(itr);
	}
	
	return itr;
}

slist_iter_t SlistBegin(const slist_t *list)
{
	assert(NULL != list);
	return(list->head);
}

slist_iter_t SlistEnd(const slist_t *list)
{
	assert(NULL != list);
	return(list->tail);
}

slist_iter_t SlistNext(const slist_iter_t iterator)
{
	assert(NULL != iterator);
	return(iterator->next);
}


int SlistIsEmpty(slist_t *list)
{
	assert(NULL != list);
	
	return(list->head == list->tail);
}


status_t SlistForeach(slist_iter_t from, slist_iter_t to, action_func_t func, void *param)
{
	status_t status = SUCCESS;
	 
	assert(NULL != param);
	assert(NULL != from);
	assert(NULL != to);
	
	while (from != to && SUCCESS == status)
	{
		status = func(SlistGetData(from), param);
		from = SlistNext(from);
	}
	
	return status;	
}

static int Count(void *data, void *param)
{
	size_t *count = NULL;
	data = data;
	
	assert(param);
	
	count = (size_t *)param;
	++(*count);
	
	return SUCCESS;
}

void SlistAppend(slist_t *dest, slist_t *src)
{
	slist_iter_t itr_end_dest = NULL;
	slist_iter_t itr_begin_src = NULL;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	itr_end_dest = SlistEnd(dest);
	itr_begin_src =  SlistBegin(src);
	
	itr_end_dest->next = itr_begin_src;
	
	dest->tail = SlistEnd(src);
	dest->tail->data = itr_end_dest->data;
	
	SlistRemove(itr_end_dest);
	free(src);
	src = NULL;
	
}
