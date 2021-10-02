#include <stdio.h> /* printf*/
#include <string.h> /* strlen*/
#include <stdlib.h> /* fopen */

#include "hasht.h"
#include "utilities.h"

#define SIZE 10
#define ASCI_SIZE 256
#define PATH "/usr/share/dict/words"


typedef struct student 
{
    int id;
    char * name;
}student_t;

size_t StudentHashFunc(const void *key);
int StudentIsMatch(void *data1, void *data2);
int StudentPrint(void *data, const void *param);
size_t StringHashFunc(const void *key);
void TestCreateDestroy();
int StrIsMatch(void *data1, void *data2);
int StrtPrint(void *data, const void *param);
size_t IntHashFunc(const void *key);
int IntPrint(void *data, const void *param);
int IntIsMatch(void *data1, void *data2);


void TestInsert();
void TestRemove();
void TestFind();
void TestForEach();
void TestSpellChecker();
size_t GetDictSize();
void TestCaching();
void FreeLinuxDict(char **buffer, size_t size, hashtable_t *hash_table);


static student_t students[] = {{1, "carmel"}, {2, "eden"}, {3, "naor"}, {4, "noam"}, {5, "moshe"}};

int main ()
{
    TestCreateDestroy();
    TestInsert();
    TestFind();
    TestRemove();
    TestForEach();
    TestSpellChecker();
    TestCaching();
    return 0;
}

void TestCreateDestroy()
{
    hashtable_t * hash = HashtableCreate(SIZE, StudentHashFunc, StudentIsMatch);
    printf(YELLOW"TestCreateDestroy\n"WHITE);
    TEST_IS_TRUE("TestCreateDestroy", hash);
    HashtableDestroy(hash);
}

void TestInsert()
{
    int res = 0;
    hashtable_t * hash = HashtableCreate(SIZE, StudentHashFunc, StudentIsMatch);

    printf(YELLOW"TestInsert\n"WHITE);
    res = HashtableIsEmpty(hash);
    TEST_IS_TRUE("HashtableIsEmpty(table)", res);

    res = HashtableCount(hash);
    TEST_IS_EQUAL("HashtableCount(table)", res, 0);

    HashtableInsert(hash, &students[0]);
    HashtableInsert(hash, &students[1]);

    res = HashtableCount(hash);
    TEST_IS_EQUAL("HashtableCount(table)", res, 2);
    HashtableDestroy(hash);

}

void TestRemove()
{
    int res = 0;
    hashtable_t * hash = HashtableCreate(SIZE, StudentHashFunc, StudentIsMatch);

    printf(YELLOW"TestRemove\n"WHITE);

    HashtableInsert(hash, &students[0]);
    HashtableInsert(hash, &students[1]);

    HashtableRemove(hash,  &students[0]);
    HashtableRemove(hash,  &students[2]);

    res = HashtableCount(hash);
    TEST_IS_EQUAL("HashtableCount(table)", res, 1);
    HashtableDestroy(hash);
}

void TestForEach()
{
    hashtable_t * hash = HashtableCreate(SIZE, StudentHashFunc, StudentIsMatch);

    printf(YELLOW"TestForEach : print dict\n"WHITE);

    HashtableInsert(hash, &students[0]);
    HashtableInsert(hash, &students[1]);
    HashtableInsert(hash, &students[2]);
    HashtableInsert(hash, &students[3]);
    HashtableInsert(hash, &students[4]);

    HashtableForeach(hash, StudentPrint, NULL);
    HashtableDestroy(hash);
}

void TestFind()
{
    void * res = NULL;

    hashtable_t * hash = HashtableCreate(SIZE, StudentHashFunc, StudentIsMatch);

    printf(YELLOW"TestFind\n"WHITE);

    HashtableInsert(hash, &students[0]);
    HashtableInsert(hash, &students[1]);

    res = HashtableFind(hash, &students[0]);
   
    TEST_IS_EQUAL("TestFind",res,  &students[0] );

    res = HashtableFind(hash, &students[2]);
    TEST_IS_EQUAL("TestFind", res, NULL);
    HashtableDestroy(hash);
}

void TestCaching()
{
    size_t i = 0;

    hashtable_t * hash = HashtableCreate(SIZE, IntHashFunc, IntIsMatch);

    printf(PURPLE"TestCaching\n"WHITE);

    for (i = 0; i < 100; i++)
    {
        HashtableInsert(hash, (void *) i);
    }

    HashtableForeach(hash, IntPrint, NULL);
    HashtableFind(hash, (void *) 0);
    HashtableFind(hash, (void *) 51);
  
    printf(LIGHT"\nfind 0 and 51 \n"WHITE);

    HashtableForeach(hash, IntPrint, NULL);

    HashtableDestroy(hash);
}

void FreeLinuxDict(char **buffer, size_t size, hashtable_t *hash_table)
{
    size_t i = 0;

    for (i = 0; i < size; ++i)
    {
        free(buffer[i]);
    }

    free(buffer);
    HashtableDestroy(hash_table);
}

void TestSpellChecker()
{
    char input[100];
    char *res = NULL;
    FILE * fp;
    char str[100];
    size_t size = 0;
    char **buff = NULL;
    hashtable_t * hash_table = NULL;
    char **temp = NULL;
    status_t status = SUCCESS;
    printf(YELLOW"TestLinuxDict: spell checker (enter a word or !q to quit)\n"WHITE);

    size = GetDictSize();
   
    fp = fopen (PATH, "r");
   
    hash_table = HashtableCreate(size, StringHashFunc, StrIsMatch);
    if (hash_table == NULL)
    {
        return;
    }

    buff = malloc(sizeof(char*) * size * 1.3);

    if (buff == NULL)
    {
        return;
    }
    temp = buff; 
    while (fgets(str, 100, fp) != NULL)
    {
        *buff = malloc((strlen(str)+ 1) * sizeof(char));
        if (*buff == NULL)
        {
            return;
        }
        strcpy(*buff, str);
        HashtableInsert(hash_table, *buff);
        if (status == FAIL)
        {
            return;
        }
        
        buff++;
    }
    
    fclose(fp);

     while (1)
    {
        fgets(input, 100, stdin);
        if (strcmp(input, "!q\n") == 0)
        {
            break;
        }
        res = HashtableFind(hash_table,(void *) input);
        TEST_IS_TRUE("HashtableFind", res);
    }

    FreeLinuxDict(temp, size, hash_table);
}

size_t GetDictSize()
{
    FILE * fp;
    char str[100];
    size_t count = 0;
   
    fp = fopen (PATH, "r");
    while (fgets(str, 100, fp) != NULL)
    {
       count++;
    }

    fclose(fp);

    return count;
}

size_t StudentHashFunc(const void *key)
{
    student_t *student = (student_t *) key;
    return (student->id % SIZE);
}

size_t IntHashFunc(const void *key)
{
    return (*(int *)&key);
}

size_t StringHashFunc(const void *key)
{
    size_t sum = 0;
    char *str = (char *) key;
    
    while (*str != '\0')
    {
       sum+= (sum * ASCI_SIZE) +*str;
       str++;
    }
    
    return (sum);
}

int StudentIsMatch(void *data1, void *data2)
{
    student_t *student1 = (student_t *) data1;
    student_t *student2 = (student_t *) data2;

    return (student1->id == student2->id);
}

int IntIsMatch(void *data1, void *data2)
{
    return (*(int*)&(data1) == *(int*)&(data2));
}

int StrIsMatch(void *data1, void *data2)
{
    char *str1 = (char *)data1;
    char *str2 = (char *)data2;
    int is_match = strcmp(str1, str2);

    return (is_match == 0 ? 1 : 0);
}

int StudentPrint(void *data, const void *param)
{
    student_t *student = (student_t *)data;
    (void)param;
    
    printf("name: %s\n  id: %d\n", student->name, student->id);
    return (0);
}

int StrtPrint(void *data, const void *param)
{
    char *str = (char *)data;
    (void)param;
    
    printf("%s\n", str);
    return (0);
}

int IntPrint(void *data, const void *param)
{
    (void)param;
    printf("%d, ", *(int*)&(data));

    return (0);
}