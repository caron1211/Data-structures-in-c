#include <stdlib.h>  /* malloc, free */
#include <assert.h>  /* assert */
#include "cbuff.h"

ssize_t WriteOneByte(cbuff_t *cbuff, char *dest);
ssize_t ReadOneByte(cbuff_t *cbuff, char *dest);

struct cbuff
{
	size_t capacity;
	size_t write;
	size_t read;
	char buffer[1];
};

cbuff_t *CbuffCreate(size_t capacity)
{
	cbuff_t *cbuff = malloc(sizeof(cbuff_t) + capacity);
	if (NULL == cbuff)
	{
		return NULL;
	}
	cbuff->write = 0;
	cbuff->read = 0;
	cbuff->capacity = capacity + 1;
	
	return cbuff;
}

void CbuffDestroy(cbuff_t *cbuff)
{
	assert(cbuff);
	free(cbuff);
	cbuff = NULL;
}

ssize_t CbuffRead(cbuff_t *cbuff, void *dest, size_t num_of_bytes)
{
	size_t i = 0;
	ssize_t success_bytes = 0;
	char *ptr = NULL;
	
	assert(NULL != cbuff);
	assert(NULL != dest);
	
	ptr = (char *) dest; 
	for(i = 0; i < num_of_bytes ; ++i)
	{
		success_bytes += ReadOneByte(cbuff, ptr);
		++ptr;
	}
	
	return success_bytes;
}

ssize_t ReadOneByte(cbuff_t *cbuff, char *dest)
{
	size_t next = 0;
	if(CbuffIsEmpty(cbuff)) /* cbuff is empty */
	{
		return 0;
	}
	
	next = cbuff->read +1;
	
	if(next >= cbuff->capacity) 
	{
		next = 0;
	}
	
	*dest = cbuff->buffer[cbuff->read];
	cbuff->read = next;
	
	return 1;
}

ssize_t CbuffWrite(cbuff_t *cbuff, const void *src, size_t num_of_bytes)
{
	size_t i = 0;
	ssize_t success_bytes = 0;
	char *ptr = NULL;
	
	assert(NULL != cbuff);
	assert(NULL != src);
	
	ptr = (char *) src;
	for(i = 0; i < num_of_bytes ; ++i)
	{
		success_bytes += WriteOneByte(cbuff, ptr);
		++ptr;
	}
	
	return success_bytes;
}

ssize_t WriteOneByte(cbuff_t *cbuff, char *src)
{
	size_t next = 0;
	next = cbuff->write +1;
	
	if(next >= cbuff->capacity) 
	{
		next = 0;
	}
	
	if(next == cbuff->read) /* cbuff is full */
	{
		return 0;
	}
	
	cbuff->buffer[cbuff->write] = *src;
	cbuff->write = next;
	
	
	return 1;
}

size_t CbuffFreeSpace(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);
	return((cbuff->capacity -1 ) - CbuffCountFull(cbuff));
}

size_t CbuffCountFull(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);
	return(cbuff->write - cbuff->read);
}

int CbuffIsEmpty(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);
	
	return(cbuff->read == cbuff-> write);
	
}


