/* Reviewed by Dor */
#include <assert.h> /* assert*/
#include <stdlib.h> /* malloc, free, realloc */
#include "d_vector.h"

#define GROWTH_FACTOR 2

struct d_vector    
{
	size_t num_of_elements;
	size_t capacity;
	void **elements;
};


d_vector_t *VectorCreate(size_t init_capacity)
{
	d_vector_t *vec = malloc(sizeof(d_vector_t));
	if (NULL == vec)
	{
		return NULL;
	}
	vec->num_of_elements = 0;
	vec->capacity = init_capacity;
	
	vec->elements = malloc(sizeof(void *) * init_capacity);
	if (NULL == vec->elements)
	{
		return NULL;
	}
	
	return vec;
}


void VectorDestroy(d_vector_t *vec)
{
	assert(NULL != vec);
	
	free(vec->elements);
	vec->elements =NULL;
	
	free(vec);
	vec = NULL;
}

void VectorPopBack(d_vector_t *vec)
{
	assert(NULL != vec);
	assert(vec->num_of_elements > 0);
	vec->num_of_elements--;
}

status_t VectorPushBack(d_vector_t *vec, const void *item)
{
	status_t status = SUCCESS;
	
	assert(NULL != vec);
	
	if ((vec->capacity ) == (vec->num_of_elements))
	{
		status = VectorExpand(vec, (vec->capacity) * GROWTH_FACTOR );
		
	}
	
	if (SUCCESS == status)
	{
		vec->elements[vec->num_of_elements] = (void *)item;
		vec->num_of_elements++;
	}
		
	return status;
}

void VectorSetElement(const d_vector_t *vec, size_t index, const void *item)
{
	assert(NULL != vec);
	assert(index < (vec->num_of_elements));
	
	vec->elements[index] = (void *)item;
}

void *VectorGetElement(const d_vector_t *vec, size_t index) 
{
	assert(NULL != vec);
	assert(index < (vec->capacity));
	
	return(vec->elements[index]);
}


size_t VectorGetSize(const d_vector_t *vec)
{
	assert(NULL != vec);
	
	return(vec->num_of_elements);
}

size_t VectorGetCapacity(const d_vector_t *vec)
{
	assert(NULL != vec);
	
	return(vec->capacity);
}

status_t VectorExpand(d_vector_t *vec, size_t total_size)
{
	void **tmp = NULL;
	
	assert(NULL != vec);
	assert(total_size > (vec->capacity));
	
	
	tmp = realloc(vec->elements, total_size * sizeof(void *));
	if (NULL == tmp)
	{
		return FAIL;
	}
	
	vec->elements = tmp;
	vec->capacity = total_size;
	
	return SUCCESS;
}

status_t VectorShrink(d_vector_t *vec)
{
	void **tmp = NULL;
	
	assert(NULL != vec);
	assert(vec->num_of_elements > 0);
	
	tmp = realloc(vec->elements, vec->num_of_elements * sizeof(void *));
	if (NULL == tmp)
	{
		return FAIL;
	}
	
	vec->elements = tmp;
	vec->capacity = vec->num_of_elements;
	
	return SUCCESS;
}


