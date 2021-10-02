#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include "pqueue.h"

#define RED "\33[1;31m"
#define GREEN "\33[1;32m"
#define WHIT "\33[0m"
#define MAG "\33[1;96m"

#define TEST_IS_EQUAL(test_name, expected, real)\
			(real == expected ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))

#define TEST_IS_TRUE(test_name, result)\
			(result ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))

#define TEST_IS_FALSE(test_name, result)\
			(!result ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))		


static int IntCmp(void *num1, void *num2);
static int StudentCmp(void *data1, void *data2);
static int StudentPrint(void *data, void *param);
static int StudentIsMatch(void *data1, void *data2);

static void CreateAndDestroyTest();
static void IsEmptyTest();
static void CountTest();
static void EnQueueTest();
static void EnQueueTest();
static void EraseTest();
static void ClearTest();
typedef struct students
{
	char *name;
	int age;
} students_t;

int main()
{
	CreateAndDestroyTest();
	IsEmptyTest();
	CountTest();
	EnQueueTest();
	EraseTest();
	ClearTest();
	
	return 0;
}

static void CreateAndDestroyTest()
{
	pq_t *pq = NULL;
	
	printf(MAG"CreateAndDestroyTest\n"WHIT);
	pq = PQCreate(IntCmp);
	TEST_IS_TRUE("CreateAndDestroyTest", pq);

	PQDestroy(pq); 
}

static void IsEmptyTest()
{
	pq_t *pq = NULL;
	int a = 100;
	
	printf(MAG"IsEmptyTest\n"WHIT);
	pq = PQCreate(IntCmp);
	
	TEST_IS_TRUE("empty test case 1",PQIsEmpty(pq));
	PQEnqueue(pq, &a);
	TEST_IS_FALSE("empty test case 2", PQIsEmpty(pq));
	
	
	PQDestroy(pq);  
}

static void CountTest()
{
	pq_t *pq = NULL;
	
	int a = 100;
	int b = 500;
	
	printf(MAG"CountTest\n"WHIT);
	pq = PQCreate(IntCmp);
	
	TEST_IS_EQUAL("CountTest case 1", PQCount(pq), 0);
	
	PQEnqueue(pq, &a);
	TEST_IS_EQUAL("CountTest case 2", PQCount(pq), 1);
	
	PQEnqueue(pq, &b);	
	TEST_IS_EQUAL("CountTest case 3", PQCount(pq), 2);
	
	PQDequeue(pq);
	PQDequeue(pq);
	
	TEST_IS_EQUAL("CountTest case 4", PQCount(pq), 0);
	
	PQDestroy(pq);  
}

static void EnQueueTest()
{
	pq_t *pq = NULL;
	
	int a = 300;
	int b = 500;
	int c = 100;
	int d = 200;
	
	printf(MAG"EnQueue/DeQueue Test\n"WHIT);
	pq = PQCreate(IntCmp);
	
	PQEnqueue(pq, &a);
	TEST_IS_EQUAL("EnQueueTest", *(int *)PQPeek(pq), a);
	
	PQEnqueue(pq, &b);
	TEST_IS_EQUAL("EnQueueTest", *(int *)PQPeek(pq), a);
	
	PQEnqueue(pq, &c);
	TEST_IS_EQUAL("EnQueueTest", *(int *)PQPeek(pq), c);
	
	PQEnqueue(pq, &d);
	TEST_IS_EQUAL("DeQueueTest", *(int *)PQDequeue(pq), c);
	
	TEST_IS_EQUAL("DeQueueTest", *(int *)PQDequeue(pq), d);

	
	PQDestroy(pq);  
}

static void EraseTest()
{
	pq_t *pq = NULL;
	void *res = NULL;
	
	students_t s1 = {"alice", 7};
	students_t s2 = {"bob", 9};
	students_t s3 = {"crab", 5};
	students_t s4 = {"dan", 8};
	
	char *to_find = "bob";
	
	printf(MAG"EnQueueTest\n"WHIT);
	pq = PQCreate(StudentCmp);
	
	PQEnqueue(pq, &s1);
	PQEnqueue(pq, &s2);
	PQEnqueue(pq, &s3);
	PQEnqueue(pq, &s4);
	
	PQPrint(pq, StudentPrint, NULL);
	
	printf(MAG"erase bob\n"WHIT);
	res = PQErase(pq, StudentIsMatch, to_find);
	TEST_IS_TRUE("EraseTest",res);
	PQPrint(pq, StudentPrint, NULL);
	

	res = PQErase(pq, StudentIsMatch, to_find);
	TEST_IS_FALSE("EraseTest", res);
	
	
	PQDestroy(pq);
}

static void ClearTest()
{
	pq_t *pq = NULL;
	
	students_t s1 = {"alice", 7};
	students_t s4 = {"dan", 8};
	
	printf(MAG"PQClearTest\n"WHIT);
	pq = PQCreate(StudentCmp);
	
	PQClear(pq);
	
	TEST_IS_EQUAL("ClearTest", PQCount(pq), 0);
	TEST_IS_TRUE("ClearTest",PQIsEmpty(pq));
	
	PQPrint(pq, StudentPrint, NULL);
	
	PQEnqueue(pq, &s1);
	PQEnqueue(pq, &s4);
	TEST_IS_FALSE("ClearTest",PQIsEmpty(pq));
	
	
	PQDestroy(pq);

}

static int IntCmp(void *num1, void *num2)
{

	return (*(int *)num1 - *(int *)num2);
}

static int StudentIsMatch(void *data1, void *data2)
{
	int res = 0;
	students_t *s1 = (students_t *) data1;

	res = strcmp(s1->name,(char *)data2);
	return (res == 0);
}

static int StudentCmp(void *data1, void *data2)
{
	students_t *s1 = (students_t *) data1;
	students_t *s2 = (students_t *) data2;

	return (s1->age - s2->age);
}

static int StudentPrint(void *data, void *param)
{
	students_t *s = (students_t *) data;
	param = param;
	printf("name: %s    age: %d \n",s->name, s->age);

	return 0;
}
