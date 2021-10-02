#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc, free*/
#include "srtlist.h"

			
static int AddressIsMatch(void *data, void *param);

struct srtlist
{
	dlist_t *list;
	cmp_func_t cmp_func;
};

srtlist_t *SrtListCreate(cmp_func_t cmp_func) 
{ 
	srtlist_t *strlist = NULL;
	dlist_t *dlist = NULL;
	
	assert(NULL != cmp_func);
	
	strlist = (srtlist_t *) malloc(sizeof(srtlist_t));
	if(NULL == strlist)
	{
		return NULL;
	}
	dlist = DlistCreate();
	if(NULL == dlist)
	{
		free(strlist);
		strlist = NULL;
		return NULL;
	}
	
	strlist->list = dlist;
	strlist->cmp_func = cmp_func;
	
	return (strlist);
}



void SrtListDestroy(srtlist_t *slist) 
{ 
	assert(NULL != slist);
	
	DlistDestroy(slist->list);
	
	free(slist);
	slist = NULL;
}


srtlist_iter_t SrtListNext(srtlist_iter_t iterator) 
{ 
	return (DlistNext(iterator));
}


srtlist_iter_t SrtListPrev(srtlist_iter_t iterator) 
{ 
	return (DlistPrev(iterator));
}



void *SrtListGetData(const srtlist_iter_t iterator) 
{ 
	return (DlistGetData(iterator));
}


srtlist_iter_t SrtListInsert(srtlist_t *slist, void *data) 
{ 
	srtlist_iter_t itr = NULL;
	int found = 0;
	
	assert(NULL != slist);
	
	itr = SrtListBegin(slist);

	
	while (!found && itr != SrtListEnd(slist))
	{
		if(slist->cmp_func(SrtListGetData(itr), data) < 0) /* new data is bigger */
		{
			itr = SrtListNext(itr);
		}
		
		else 
		{
			found = 1;
		}
		
	}
	
	itr = DlistInsert(itr, data);
	
	return (itr);
	
}


srtlist_iter_t SrtListRemove(srtlist_iter_t iterator) 
{ 
	return (DlistRemove(iterator));
}


void *SrtListPopFront(srtlist_t *slist) 
{ 
	assert(NULL != slist);
	
	return (DlistPopFront(slist->list));
}

void *SrtListPopBack(srtlist_t *slist) 
{ 
	assert(NULL != slist);
	
	return (DlistPopBack(slist->list));
}


size_t SrtListSize(const srtlist_t *slist) 
{
	assert(NULL != slist);
	
 	return (DlistSize(slist->list));
}


int SrtListIsEmpty(const srtlist_t *slist) 
{ 
	assert(NULL != slist);
	
	return (DlistIsEmpty(slist->list));
}


srtlist_iter_t SrtListBegin(const srtlist_t *slist) 
{ 
	assert(NULL != slist);
	
	return (DlistBegin(slist->list));
}


srtlist_iter_t SrtListEnd(const srtlist_t *slist) 
{ 
	assert(NULL != slist);
	
	return (DlistEnd(slist->list));
}

int SrtListForeach(srtlist_iter_t from, srtlist_iter_t to , int(*action_func)(void *data, void *param), void *param) 
{ 
	return (DlistForeach(from, to, action_func, param));
}


srtlist_iter_t SrtListFind(srtlist_iter_t from, srtlist_iter_t to, const void *to_find) 
{ 
	return (DlistFindOne(from, to, AddressIsMatch, to_find));
} 



srtlist_iter_t SrtListFindIf(srtlist_iter_t from, srtlist_iter_t to, is_match_t is_match, void *param) 
{ 

	return (DlistFindOne(from, to, is_match, param));
} 


void SrtListMerge(srtlist_t *dest, srtlist_t *src) 
{ 
	
	srtlist_iter_t dest_node = NULL;
    srtlist_iter_t from_node = NULL;
    srtlist_iter_t to_node = NULL;
    cmp_func_t cmp_f = NULL;
    
    cmp_f = dest->cmp_func;
    assert(NULL != dest);
    assert(NULL != src);
    assert(dest->cmp_func == src->cmp_func);
    
    dest_node = SrtListBegin(dest);
    
    while (0 == SrtListIsEmpty(src))
    {
        from_node = SrtListBegin(src);
        to_node = SrtListNext(from_node);
    
            while (SrtListEnd(dest) != dest_node && 0 < cmp_f(SrtListGetData(from_node), SrtListGetData(dest_node)))
        {
            dest_node = SrtListNext(dest_node);
        }
        

        while (SrtListEnd(dest) != dest_node && SrtListEnd(src) != to_node && 0 > cmp_f(SrtListGetData(to_node), SrtListGetData(dest_node)))
        {
            to_node = SrtListNext(to_node);
        }
        DlistSplice(dest_node, from_node, to_node);
    }
	
}

static int AddressIsMatch(void *data, void *param)
{

	return(data == param);
}

