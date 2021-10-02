#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include <unistd.h> /* sleep */
#include "task.h"

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
			


int foo (void * param);
int printTime (void * param);

static void CreateAndDestroyTest();
static void TaskRunTest();
static void UpdateTimeTest();
static void TaskIsMatchTest();
static void IsBeforeTest();

int main()
{
	CreateAndDestroyTest();
	TaskRunTest();
	UpdateTimeTest();
	TaskIsMatchTest();
	IsBeforeTest();
	return 0;
}

static void CreateAndDestroyTest()
{
	task_t *task1 = NULL;

	
	printf(MAG"CreateAndDestroyTest\n"WHIT);
	task1 = TaskCreate(foo, NULL, 3, NULL);
	TEST_IS_TRUE("CreateAndDestroyTest", task1);

	TaskDestroy(task1);

}

static void TaskRunTest()
{
	task_t *task1 = NULL;

	printf(MAG"Task Run\n"WHIT);
	
	task1 = TaskCreate(foo, NULL, 3, NULL);
	while(REPEAT == TaskRun(task1))
	{
	
	}

	TaskDestroy(task1);

}

static void UpdateTimeTest()
{
	task_t *task1 = NULL;
	task_t *task2 = NULL;
	
	printf(MAG"UpdateTimeTest\n"WHIT);
	
	task1 = TaskCreate(foo, NULL, 3, NULL);
	task2 = TaskCreate(printTime, NULL, 3, task1);
	TaskRun(task2);
	sleep(1);
	TaskUpdateTimeToRun(task1);
	TaskRun(task2);
	
	TaskDestroy(task1);
	TaskDestroy(task2); 

}

static void TaskIsMatchTest()
{
	task_t *task1 = NULL;
	task_t *task2 = NULL;
	
	printf(MAG"TaskIsMatchTest\n"WHIT);
	
	task1 = TaskCreate(foo, NULL, 3, NULL);
	task2 = TaskCreate(printTime, NULL, 3, task1);
	
	TEST_IS_FALSE("TaskIsMatch", TaskIsMatch(task1, TaskGetUid(task2)));
	TEST_IS_TRUE("TaskIsMatch", TaskIsMatch(task1, TaskGetUid(task1)));
	
	TaskDestroy(task1);
	TaskDestroy(task2); 

}

static void IsBeforeTest()
{
	task_t *task1 = NULL;
	task_t *task2 = NULL;
	
	printf(MAG"IsBeforeTest\n"WHIT);
	
	task1 = TaskCreate(foo, NULL, 3, NULL);
	task2 = TaskCreate(printTime, NULL, 3, task1);
	sleep(1);
	TaskUpdateTimeToRun(task1);
	
	TEST_IS_TRUE("before test", TaskIsBefore(task2, task1));
	
	TEST_IS_FALSE("before test", TaskIsBefore(task1, task1));
	
	TaskDestroy(task1);
	TaskDestroy(task2); 

}

int printTime (void * param)
{
	printf("time to run: %ld \n", TaskGetTimeToRun(param));
	return 0;
}

int foo (void * param)
{
	static int count = 0;
	printf("count %d\n", count);
	count++;
	
	return (count > 5); /* REPEAT */
}



