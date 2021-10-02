#include <stdio.h> /*printf*/
#include "trie.h"
#include "utilities.h"

int CmpInt(const void *new_data, const void *curr_data);
int IntIsMatch(const void *data1, void *data2);

void InsertTest();
void IsFoundTest();
void RemoveTest();
void NextAvailableTest1();
void NextAvailableTest2();


int main ()
{
    /* InsertTest();
    IsFoundTest();
    RemoveTest(); */
    NextAvailableTest1();
    NextAvailableTest2();

    return 0;
}

void InsertTest()
{
    trie_t * trie = TrieCreate(4);
    TEST_IS_EQUAL("InsertTest", TrieInsert(trie, 15), SUCCESS);
    TEST_IS_EQUAL("InsertTest", TrieInsert(trie, 8), SUCCESS);
    TEST_IS_EQUAL("InsertTest", TrieInsert(trie, 0), SUCCESS);
    TEST_IS_EQUAL("InsertTest", TrieInsert(trie, 0), PATH_OCCUPIED);
    TrieDestroy(trie);
}

void IsFoundTest()
{
    trie_t * trie = TrieCreate(3);
    TrieInsert(trie, 1);
    TrieInsert(trie, 2);

    TEST_IS_TRUE("IsFoundTest", TrieIsFound(trie, 2));
    TEST_IS_FALSE("IsFoundTest", TrieIsFound(trie, 3));

    TrieDestroy(trie);
}

void RemoveTest()
{
    trie_t * trie = TrieCreate(3);
    TrieInsert(trie, 0);
    TrieInsert(trie, 1);
    TrieInsert(trie, 2);
    TrieInsert(trie, 3);
    TrieInsert(trie, 4);
    TrieInsert(trie, 5);
    TrieInsert(trie, 6);
    TrieInsert(trie, 7);

    PrintTrie(trie);

    TEST_IS_EQUAL("RemoveTest",  TrieRemove(trie, 1), SUCCESS);
    TEST_IS_EQUAL("RemoveTest",  TrieRemove(trie, 1), PATH_INVALID);
    TEST_IS_EQUAL("RemoveTest",  TrieRemove(trie, 7), SUCCESS);
    TEST_IS_EQUAL("RemoveTest",  TrieRemove(trie,3), SUCCESS);

    PrintTrie(trie);

    TrieDestroy(trie);
}


void NextAvailableTest1()
{
    size_t a = 1;
    trie_t * trie = TrieCreate(2);

    TrieInsert(trie, 1);

    TrieNextAvailable(trie, &a);
    TEST_IS_EQUAL("NextAvailableTest", a, 2);

    a = 1;
    TrieInsert(trie, 2);
    TrieNextAvailable(trie, &a);
    TEST_IS_EQUAL("NextAvailableTest", a, 3);

    TrieDestroy(trie);
}

void NextAvailableTest2()
{
    size_t a = 1;
    size_t i = 0;
    trie_t * trie = TrieCreate(3);

    for (i = 0; i < 8; i++)
    {
        TrieInsert(trie, i);
    }
    
    TEST_IS_EQUAL("NextAvailableTest",TrieNextAvailable(trie, &a), TRIE_FULL);

    TrieRemove(trie, 1);
   
   TEST_IS_EQUAL("NextAvailableTest",TrieNextAvailable(trie, &a), SUCCESS);

    TrieDestroy(trie);
}
