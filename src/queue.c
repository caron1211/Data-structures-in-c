#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */
#include "s_l_list.h" /* slist_t */
#include "queue.h"

struct queue
{
	slist_t *list;
}; 

queue_t *QueueCreate()
{
	slist_t *list = NULL;
	queue_t *queue = NULL;
	
	list = SlistCreate();
	if(NULL == list)
	{
		return NULL;
	}
	queue = malloc(sizeof(queue));
	if(NULL == queue)
	{
		return NULL;
	}
	queue->list = list;
	
	return queue;
}

void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SlistDestroy(queue->list);
	free(queue);
	queue = NULL;
}


int Enqueue(queue_t *queue, const void *data)
{
	slist_iter_t end_itr = NULL;
	
	assert(NULL != queue);
	
	end_itr = SlistEnd(queue->list);
	end_itr = SlistInsert(end_itr, data);
	if(SlistEnd(queue->list) == end_itr) 
	{
		return FAIL;
	}
	return SUCCESS;
}

void Dequeue(queue_t *queue)
{
	slist_iter_t begin_itr = NULL;
	assert(NULL != queue);
	
	begin_itr = SlistBegin(queue->list);
	SlistRemove(begin_itr);
}

void *QueuePeek(const queue_t *queue)
{
	slist_iter_t begin_itr = NULL;
	void * data = NULL;
	
	assert(NULL != queue);
	
	begin_itr = SlistBegin(queue->list);
	data = SlistGetData(begin_itr);
	
	return(data);
}

size_t QueueCount(const queue_t *queue)
{
	assert(NULL != queue);
	
	return(SlistCount(queue->list));
}

int QueueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	
	return(SlistIsEmpty(queue->list));
}

void QueueAppend(queue_t *dest, queue_t *src)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	SlistAppend(dest->list, src->list);
	free(src);
	src = NULL;
}

