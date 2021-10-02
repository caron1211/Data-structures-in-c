/* Reviewed by Dor */
#include <stdio.h> /* printf */
#include "d_vector.h"

#define RED "\33[1;31m"
#define GREEN "\33[1;32m"
#define WHIT "\33[0m"
#define MAG "\33[1;96m"

#define TEST_IS_EQUAL(test_name, expected, real)\
			(real == expected ?\
			printf("%s:" GREEN" PASS\n"WHIT, test_name) :\
			printf("%s:"  RED" FAIL\n"WHIT, test_name))


static status_t TestIntVector();
static status_t TestStringVector();

int main()
{
	status_t status = SUCCESS;
	
	printf(RED"Test vector with integers:\n\n"WHIT);
	status = TestIntVector();
	if ( FAIL == status)
	{
		return status;
	}

	printf(RED"Test vector with strings:\n\n"WHIT);
	status = TestStringVector();
	
	return status;
	
}

static status_t TestIntVector() 
{
	void *ptr1 = (void*)5;
	void *ptr2 = (void*)10;
	void *ptr3 = (void*)15;
	void *ptr4 = (void*)20;
	
	d_vector_t *vec = NULL;
	printf(MAG"Create vector with capacity 3:\n"WHIT);
	
	vec = VectorCreate(3);
	if (NULL == vec)
	{
		return FAIL;
	}
	TEST_IS_EQUAL("Check the capacity is 3", VectorGetCapacity(vec), 3);
	TEST_IS_EQUAL("Check the num of elements is 0", VectorGetSize(vec), 0);
	
	printf(MAG"Push 3 elements:\n"WHIT);
	VectorPushBack(vec, ptr1);
	VectorPushBack(vec, ptr2);
	VectorPushBack(vec, ptr3);
	
	TEST_IS_EQUAL("Check the capacity is 3", VectorGetCapacity(vec), 3);
	TEST_IS_EQUAL("Check the num of elements is 3", VectorGetSize(vec), 3);
	
	printf(MAG"Push element:\n"WHIT);
	VectorPushBack(vec, ptr4);
	TEST_IS_EQUAL("Check the capacity is 6", VectorGetCapacity(vec), 6);
	TEST_IS_EQUAL("Check the num of elements is 4", VectorGetSize(vec), 4);
	
	TEST_IS_EQUAL("Check the num in index 0 ", VectorGetElement(vec, 0), ptr1);
	TEST_IS_EQUAL("Check the num in index 1", VectorGetElement(vec, 1), ptr2);
	
	printf(MAG"set element in index 0:\n"WHIT);
	VectorSetElement(vec, 0 ,ptr2);
	TEST_IS_EQUAL("Check the num index 0 ", VectorGetElement(vec, 0), ptr2);
	
	printf(MAG"pop element:\n"WHIT);
	VectorPopBack(vec);
	TEST_IS_EQUAL("Check the num of elements is 3", VectorGetSize(vec), 3);
	
	printf(MAG"pop and shrink:\n"WHIT);
	VectorPopBack(vec);
	VectorShrink(vec);
	TEST_IS_EQUAL("Check the capacity is 2", VectorGetCapacity(vec), 2);
	TEST_IS_EQUAL("Check the num of elements is 2", VectorGetSize(vec), 2);
	
	VectorDestroy(vec);
	vec = NULL;
	
	return SUCCESS;
}


static status_t TestStringVector()
{
	int i = 0;
	char *str1 = "hello";
	char *str2 = "hey";

	d_vector_t *vec = NULL;
	printf(MAG"Create vector with capacity 2:\n"WHIT);
	
	vec = VectorCreate(2);
	if (NULL == vec)
	{
		return FAIL;
	}
	
	printf(MAG"expand vector to 100:\n"WHIT);
	VectorExpand(vec, 100);
	TEST_IS_EQUAL("Check the capacity is 100", VectorGetCapacity(vec), 100);
	
	printf(MAG"Push 30 elements:\n"WHIT);
	for (i =0 ; i < 30; ++i) 
	{
		VectorPushBack(vec, (void*) str1);
		VectorSetElement(vec, i, (void*) str2);
	}
	
	TEST_IS_EQUAL("Check the string in index 5 ", VectorGetElement(vec, 5), str2);
	TEST_IS_EQUAL("Check the num of elements is 30", VectorGetSize(vec), 30);
	VectorDestroy(vec);
	vec = NULL;
	
	return SUCCESS;

}

