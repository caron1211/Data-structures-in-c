  #include <assert.h> /* assert*/
#include <stdlib.h> /* malloc, free */
#include "stack.h"


struct stack
{
	size_t top;
	size_t capacity;
	void **array;
};

stack_t *StackCreate(size_t capacity)
{
	stack_t *stack = malloc(sizeof(stack_t));
	if(NULL == stack)
	{
		return NULL;
	}
	stack->top = 0;
	stack->capacity = capacity;
	
	stack->array = malloc(capacity * sizeof(size_t));
	if (NULL == stack->array)
	{
		return NULL;
	}
	
	return stack;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	free(stack->array);
	free(stack);
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);
	assert(0 != stack->top );
	stack->top--;
}

void StackPush(stack_t *stack, void *item)
{
	assert(NULL != stack);
	assert((stack->top) < (stack->capacity));
	stack->top++;
	stack->array[stack->top] = item;
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	assert(0 != stack->top );
	return(stack->array[stack->top]);
}

size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);
	return(stack->top);
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	return(0 == stack->top);

}

size_t StackCapacity(const stack_t *stack)
{
	assert(NULL != stack);
	return (stack->capacity);
}

