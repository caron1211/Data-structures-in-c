#include <stdio.h> /* printf */
#include "stack.h"

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


typedef enum {

	SUCCESS,
	StackCreate_FAILED
} status_t;

static status_t TestIntStack();
static status_t TestStringStack();

int main()
{
	status_t status = SUCCESS;
	printf(RED"Test stack with integers:\n\n"WHIT);
	status = TestIntStack();
	if ( StackCreate_FAILED == status)
	{
		return status;
	}

	printf(RED"Test stack with strings:\n\n"WHIT);
	status = TestStringStack();
	
	return status;
	
}

static status_t TestIntStack()
{

	void *ptr = (void*)5;
	stack_t *stack = NULL;
	printf(MAG"Create stack with capacity 5:\n"WHIT);
	stack = StackCreate(5);
	if (NULL == stack)
	{
		return StackCreate_FAILED;
	}
	TEST_IS_TRUE("Check IsEmpty", StackIsEmpty(stack));
	
	printf(MAG"Insert element:\n"WHIT);
	/* ptr = *(int *)&x; */ 
	StackPush(stack, ptr);
	TEST_IS_FALSE("Check is not Empty ", StackIsEmpty(stack));
	TEST_IS_EQUAL("Check the size is one", StackSize(stack), 1);
	TEST_IS_EQUAL("Check the capacity is five", StackCapacity(stack), 5);
	
	TEST_IS_EQUAL("Peek the first element and compare", StackPeek(stack), ptr);
	ptr = (void*)17;
	
	printf(MAG"Insert element:\n"WHIT);
	StackPush(stack, ptr);
	TEST_IS_EQUAL("Check the size is two", StackSize(stack), 2);
	TEST_IS_EQUAL("Peek the second element and compare", StackPeek(stack), ptr);
	TEST_IS_EQUAL("Check the capacity is five", StackCapacity(stack), 5);
	
	printf(MAG"Pop element:\n"WHIT);
	StackPop(stack);
	TEST_IS_EQUAL("Check the size is one", StackSize(stack), 1);
	
	printf(MAG"Pop element:\n"WHIT);
	StackPop(stack);
	TEST_IS_TRUE("Check IsEmpty", StackIsEmpty(stack));
	TEST_IS_EQUAL("Check the capacity is five", StackCapacity(stack), 5);
	
	StackDestroy(stack);
	
	return SUCCESS;
}

static status_t TestStringStack()
{
	void *str1 = (void*)"my test1";
	void *str2 = (void*)"my test2";
	void *str3 = (void*)"my test3";
	void *str4 = (void*)"my test4";
	
	stack_t *stack = NULL;
	printf(MAG"Create stack with capacity 10:\n"WHIT);
	stack = StackCreate(10);
	if (NULL == stack)
	{
		return StackCreate_FAILED;
	}
	printf(MAG"Insert 4 elements:\n"WHIT);

	StackPush(stack, str1);
	StackPush(stack, str2);
	StackPush(stack, str3);
	StackPush(stack, str4);
	
	TEST_IS_EQUAL("Peek and compare", StackPeek(stack), str4);
	StackPop(stack);
	
	TEST_IS_EQUAL("Peek and compare", StackPeek(stack), str3);
	StackPop(stack);
	
	TEST_IS_EQUAL("Peek and compare", StackPeek(stack), str2);
	StackPop(stack);
	
	TEST_IS_EQUAL("Peek and compare", StackPeek(stack), str1);
	StackPop(stack);
	
	StackDestroy(stack);

	return SUCCESS;
}



