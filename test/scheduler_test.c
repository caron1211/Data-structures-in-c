#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include <unistd.h> /* sleep */
#include "scheduler.h"

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
			


int IncreaseNum1(void * param);
int IncreaseNum2(void * param);
void Empty(ilrd_uid_t uid, void * param);
int ClearTask(void * param);

static void CreateAndDestroyTest();
static void IsEmpty();
static void AddRemoveTaskTest();
static void SizeTest();
static void RunTest();
static void RunTestCase2();
static void RunTestCase3();
int Stop(void * param);
int g_num1 = 0;
int g_num2 = 0;

int main()
{
	CreateAndDestroyTest();
	SizeTest();
	IsEmpty();
	AddRemoveTaskTest();
	RunTest();
	RunTestCase2();
	RunTestCase3();
	return 0;
}

static void CreateAndDestroyTest()
{
	scheduler_t *sched = NULL;

	
	printf(MAG"CreateAndDestroyTest\n"WHIT);
	sched = SchedulerCreate();
	TEST_IS_TRUE("CreateAndDestroyTest", sched);

	SchedulerDestroy(sched);

}

static void IsEmpty()
{
	scheduler_t *sched = NULL;

	printf(MAG"AddRemoveTaskTest\n"WHIT);
	sched = SchedulerCreate();
	
	TEST_IS_TRUE("CreateAndDestroyTest", SchedulerIsEmpty(sched));

	SchedulerAddTask(sched, IncreaseNum1, Empty, 3, NULL);
	
	TEST_IS_FALSE("CreateAndDestroyTest", SchedulerIsEmpty(sched));
	
	
	SchedulerDestroy(sched);
}

static void SizeTest()
{
	scheduler_t *sched = NULL;

	printf(MAG"AddRemoveTaskTest\n"WHIT);
	sched = SchedulerCreate();
	
	TEST_IS_EQUAL("SizeTest", SchedulerSize(sched), 0);

	SchedulerAddTask(sched, IncreaseNum1, Empty, 3, NULL);
	SchedulerAddTask(sched, IncreaseNum1, Empty, 3, NULL);
	
	TEST_IS_EQUAL("SizeTest", SchedulerSize(sched), 2);
	
	SchedulerDestroy(sched);
}

static void AddRemoveTaskTest()
{
	scheduler_t *sched = NULL;
	ilrd_uid_t uid = {0};

	printf(MAG"AddRemoveTaskTest\n"WHIT);
	sched = SchedulerCreate();
	
	SchedulerAddTask(sched, IncreaseNum1, Empty, 3, NULL);
	uid = SchedulerAddTask(sched, IncreaseNum1, Empty, 3, NULL);
	
	SchedulerRemoveTask(sched, uid);
	TEST_IS_EQUAL("SizeTest", SchedulerSize(sched), 1);
		
	SchedulerDestroy(sched);
}

static void RunTest()
{
	scheduler_t *sched = NULL;

	printf(MAG"RunTest\n"WHIT);
	sched = SchedulerCreate();
	
	TEST_IS_EQUAL("SizeTest", SchedulerSize(sched), 0);

	SchedulerAddTask(sched, IncreaseNum1, Empty, 1, NULL);
	SchedulerAddTask(sched, IncreaseNum2, Empty, 2, NULL);
	
	SchedulerRun(sched);
	TEST_IS_EQUAL("SizeTest", g_num1, 3);
	TEST_IS_EQUAL("SizeTest", g_num2, 3);

	SchedulerDestroy(sched);
}

static void RunTestCase2()
{
	scheduler_t *sched = NULL;
	ilrd_uid_t uid_stop = {0};

	g_num1 = 0;
	g_num2 = 0;
	printf(MAG"RunTest with stop\n"WHIT);
	sched = SchedulerCreate();
	
	TEST_IS_EQUAL("SizeTest", SchedulerSize(sched), 0);

	SchedulerAddTask(sched, IncreaseNum1, Empty, 1, NULL);
	SchedulerAddTask(sched, IncreaseNum2, Empty, 1, NULL);
	uid_stop = SchedulerAddTask(sched, Stop, Empty, 2, (void *) sched);

	SchedulerRun(sched);
	
	SchedulerRemoveTask(sched, uid_stop);
	
	TEST_IS_EQUAL("SizeTest", g_num1, 2);
	TEST_IS_EQUAL("SizeTest", g_num2, 2);
	
	printf(MAG"continue after stop\n"WHIT);
	SchedulerRun(sched);
	
	TEST_IS_EQUAL("SizeTest", g_num1, 3);
	TEST_IS_EQUAL("SizeTest", g_num2, 3);
	
	SchedulerDestroy(sched);
}

static void RunTestCase3()
{
	scheduler_t *sched = NULL;
	
	g_num1 = 0;
	g_num2 = 0;
	
	printf(MAG"RunTest with clear\n"WHIT);
	sched = SchedulerCreate();
	
	TEST_IS_EQUAL("SizeTest", SchedulerSize(sched), 0);

	SchedulerAddTask(sched, IncreaseNum1, Empty, 1, NULL);
	SchedulerAddTask(sched, IncreaseNum2, Empty, 1, NULL);
	SchedulerAddTask(sched, ClearTask, Empty, 2, (void *) sched);

	SchedulerRun(sched);
	
	TEST_IS_EQUAL("SizeTest", g_num1, 2);
	TEST_IS_EQUAL("SizeTest", g_num2, 2);
	
	SchedulerDestroy(sched);
}


int IncreaseNum1(void * param)
{
	param = param;
	g_num1++;
	printf("IncreaseNum1 %d\n", g_num1);
	
	
	return (g_num1 >= 3); /* REPEAT */
}

int IncreaseNum2(void * param)
{
	param = param;
	g_num2++;
	printf("IncreaseNum2 %d\n", g_num2);
	
	return (g_num2 >= 3); /* REPEAT==0 */
}

int ClearTask(void * param)
{
	SchedulerClear((scheduler_t *) param);
	return 1; 
}

int Stop(void * param)
{
	
	SchedulerStop((scheduler_t *) param);
	return 1; 
}

void Empty(ilrd_uid_t uid, void * param)
{
	uid = uid;
	param = param;
}


