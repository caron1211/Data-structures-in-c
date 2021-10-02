#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "hasht.h"
#include "dllist.h"

struct hashtable 
{
    dlist_t **hashtable_items; 
    hash_func_t hash_func;
  	is_match_t is_match;
    size_t table_size;
};

static void Destroy(hashtable_t *hash_table, size_t idx);
static dlist_t *GetEntry(const hashtable_t *hash_table,const void * data);
static void MoveDataToBegin(hashtable_t *hash_table, dlist_iter_t itr);


hashtable_t *HashtableCreate(size_t table_size, hash_func_t hash_func, is_match_t is_match)
{
    hashtable_t *hash_table = NULL;
    dlist_t **hashtable_items = NULL;
    size_t i = 0;

    assert(NULL != hash_func);
    assert(NULL != is_match);

    hash_table = malloc(sizeof(hashtable_t));
    if (NULL == hash_table)
    {
        return (NULL);
    }

    hashtable_items = malloc(sizeof(dlist_t *) * table_size);
    if (NULL == hashtable_items)
    {
        free(hash_table);
        return (NULL);
    }
    for (i = 0; i < table_size; i++)
    {
        hashtable_items[i] = DlistCreate();
        if (NULL == hashtable_items[i])
        {
            Destroy(hash_table, i);
            return (NULL);
        }
    }
    
    hash_table->hashtable_items = hashtable_items;
    hash_table->hash_func = hash_func;
    hash_table->is_match = is_match;
    hash_table->table_size = table_size;

    return (hash_table);
}

void HashtableDestroy(hashtable_t *hash_table)
{
    assert(NULL != hash_table);

    Destroy(hash_table, hash_table->table_size);
}

static void Destroy(hashtable_t *hash_table, size_t idx)
{
    size_t i = 0;

    assert(NULL != hash_table);

    for (i = 0; i < idx; i++)
    {
        DlistDestroy(hash_table->hashtable_items[i]);
    }

    free(hash_table->hashtable_items);
    hash_table->hashtable_items = NULL;
    free(hash_table);
}

static dlist_t *GetEntry(const hashtable_t *hash_table, const void * data)
{
    size_t idx = 0;

    idx = hash_table->hash_func(data);
    idx = idx % hash_table->table_size;

    return (hash_table->hashtable_items[idx]);

}

status_t HashtableInsert(hashtable_t *hash_table, void *data)
{
    dlist_iter_t itr = NULL;
    dlist_t *entry = NULL;

    assert(NULL != hash_table);

    entry = GetEntry(hash_table, data);

    itr =DlistPushFront(entry, data);
    if (DlistEnd(entry) == itr)
    {
        return FAIL;
    }
    
    return (SUCCESS);
}

void HashtableRemove(hashtable_t *hash_table, void *data)
{
    
    dlist_iter_t itr = NULL;
    dlist_t *entry = NULL;
    dlist_iter_t dummy = NULL;

    assert(NULL != hash_table);

    entry = GetEntry(hash_table, data);
    dummy = DlistEnd(entry);
    
    itr = DlistFindOne(DlistBegin(entry), dummy, hash_table->is_match, data);
    if (dummy != itr)
    {
        DlistRemove(itr);
    }
}

size_t HashtableCount(const hashtable_t *hash_table)
{
    size_t i = 0;
    size_t count = 0;

    assert(NULL != hash_table);

    for (i = 0; i < hash_table->table_size; i++)
    {
        count+= DlistSize(hash_table->hashtable_items[i]);
    }
    return (count);
}

int HashtableIsEmpty(const hashtable_t *hash_table)
{
     assert(NULL != hash_table);
     
    return (HashtableCount(hash_table) ==  0);
}


void *HashtableFind(const hashtable_t *hash_table, const void *data)
{
    dlist_iter_t itr = NULL;
    dlist_t *entry = NULL;
    dlist_iter_t dummy = NULL;

    assert(NULL != hash_table);

    entry = GetEntry(hash_table, data);
    dummy = DlistEnd(entry);

    itr = DlistFindOne(DlistBegin(entry), dummy, hash_table->is_match, data);

    if (itr != dummy)
    {   
        MoveDataToBegin((hashtable_t *)hash_table, itr);
        return (DlistGetData(itr));
    }

    return (NULL);
}

int HashtableForeach(const hashtable_t *hash_table, action_func_hash_t action_func, void *param)
{
    size_t i = 0;
    dlist_t *entry = NULL;
    int status = SUCCESS;

    assert(NULL != hash_table);

    for (i = 0; i < hash_table->table_size && status == SUCCESS; i++)
    {
        entry = hash_table->hashtable_items[i];
        status = DlistForeach(DlistBegin(entry), DlistEnd(entry), (action_func_t) action_func, param);
    }

    return (status);
}

static void MoveDataToBegin(hashtable_t *hash_table, dlist_iter_t itr)
{
    void *data = NULL;
    
    assert(NULL != hash_table);

    data = DlistGetData(itr);
    DlistRemove(itr);

   HashtableInsert(hash_table, data);
}