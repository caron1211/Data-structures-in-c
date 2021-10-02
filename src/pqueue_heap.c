#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */
#include "heap.h" 
#include "pqueue.h"

struct pq
{
	heap_t *srtlist;
};

pq_t *PQCreate(pq_cmp_t pq_cmp)
{

	heap_t *list = NULL;
	pq_t *pqueue = NULL;
	
	list = HeapCreate((int (*)(const void *, const void *)) pq_cmp);
	if(NULL == list)
	{
		return (NULL);
	}
	pqueue = (pq_t *) malloc(sizeof(pq_t));
	if(NULL == pqueue)
	{
		free(pqueue);
		pqueue = NULL;
		return (NULL);
	}
	pqueue->srtlist = list;
	
	return (pqueue);
}


void PQDestroy(pq_t *pq)
{
	assert(NULL != pq);
	
	HeapDestroy(pq->srtlist);
	free(pq);
	pq = NULL;
}


void PQClear(pq_t *pq)
{
	assert(NULL != pq);

	while (!HeapIsEmpty(pq->srtlist))
	{
		HeapPop(pq->srtlist);
	}
}


int PQEnqueue(pq_t *pq, void *data)
{
	
	assert(NULL != pq);
	
	return(HeapPush(pq->srtlist, data));
}


void *PQDequeue(pq_t *pq)
{
    void * data = NULL;
	assert(NULL != pq);
	
    data = HeapPeek(pq->srtlist);
    HeapPop(pq->srtlist);
	return (data);
}


void *PQPeek(const pq_t *pq)
{

	assert(NULL != pq);
	
	return (HeapPeek(pq->srtlist));
}


size_t PQCount(const pq_t *pq)
{
	assert(NULL != pq);
	
	return (HeapSize(pq->srtlist));
}


int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);
	
	return (HeapIsEmpty(pq->srtlist));
}


void *PQErase(pq_t *pq, pq_match_t pq_match, void *param)
{
	void *data = NULL;
	
	assert(NULL != pq);
	assert(NULL != pq_match);
	
	HeapRemove(pq->srtlist,(int (*)(const void *, void *))pq_match, param);
	
	return (data);
}

#ifndef NDEBUG

void PQPrint(pq_t *pq, print_func_t print_func, void *param)
{
	PrintHeap(pq->srtlist);

}

#endif
