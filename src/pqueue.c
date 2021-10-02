#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */
#include "srtlist.h" /* srtlist_t */
#include "pqueue.h"

struct pq
{
	srtlist_t *srtlist;
};

pq_t *PQCreate(pq_cmp_t pq_cmp)
{

	srtlist_t *list = NULL;
	pq_t *pqueue = NULL;
	
	list = SrtListCreate(pq_cmp);
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
	
	SrtListDestroy(pq->srtlist);
	free(pq);
	pq = NULL;
}


void PQClear(pq_t *pq)
{
	assert(NULL != pq);

	while (!SrtListIsEmpty(pq->srtlist))
	{
		SrtListPopFront(pq->srtlist);
	}
}


int PQEnqueue(pq_t *pq, void *data)
{
	srtlist_iter_t itr = NULL;
	
	assert(NULL != pq);
	
	itr = SrtListInsert(pq->srtlist, data);
	return (itr == SrtListEnd(pq->srtlist));
}


void *PQDequeue(pq_t *pq)
{
	assert(NULL != pq);
	
	return (SrtListPopFront(pq->srtlist));
}


void *PQPeek(const pq_t *pq)
{
	srtlist_iter_t itr = NULL;
	
	assert(NULL != pq);
	
	itr = SrtListBegin(pq->srtlist);
	return (SrtListGetData(itr));
}


size_t PQCount(const pq_t *pq)
{
	assert(NULL != pq);
	
	return (SrtListSize(pq->srtlist));
}


int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);
	
	return (SrtListIsEmpty(pq->srtlist));
}


void *PQErase(pq_t *pq, pq_match_t pq_match, void *param)
{
	srtlist_iter_t itr = NULL;
	void *data = NULL;
	
	assert(NULL != pq);
	assert(NULL != pq_match);
	
	itr = SrtListFindIf(SrtListBegin(pq->srtlist), SrtListEnd(pq->srtlist), pq_match, param);
	if (itr ==  SrtListEnd(pq->srtlist))
	{
		return (NULL);
	}
	
	data = SrtListGetData(itr);
	SrtListRemove(itr);
	
	return (data);
}

#ifndef NDEBUG

void PQPrint(pq_t *pq, print_func_t print_func, void *param)
{
	assert(NULL != pq);
	SrtListForeach(SrtListBegin(pq->srtlist), SrtListEnd(pq->srtlist),print_func, param);
}

#endif
