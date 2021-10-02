#ifndef __TRIE_H__
#define __TRIE_H__

#include <stddef.h>  /* size_t */

typedef enum status
{
    SUCCESS = 0,
    MALLOC_FAIL,
    PATH_OCCUPIED,
    PATH_INVALID,
    TRIE_FULL
}trie_status_t;


typedef struct trie trie_t;

/**********************************************************************/

trie_t *TrieCreate(size_t height);

void TrieDestroy(trie_t *trie);


trie_status_t TrieInsert(trie_t *trie, size_t data);

trie_status_t TrieRemove(trie_t *trie, size_t data);

int TrieIsFound(trie_t *tree, size_t data); 


size_t TrieCount(trie_t *trie);

int TrieIsEmpty(trie_t *trie);

trie_status_t TrieNextAvailable(const trie_t *trie, size_t *data);


#endif /* __TRIE_H__ */