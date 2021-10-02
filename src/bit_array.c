#include <assert.h> /* assert*/
#include <limits.h> /* CHAR_BIT */
#include "bitaray.h"

#define WORD_SIZE sizeof(bitarray_t) * CHAR_BIT

bitarray_t BitArrSetBitOn(bitarray_t bitarray, size_t bit_index)
{
	bitarray_t bit_arr = bitarray;
	assert(bit_index < WORD_SIZE);
	bit_arr = bit_arr | (1UL << bit_index);
	
	return bit_arr;
}

bitarray_t BitArrSetBitOff(bitarray_t bitarray, size_t bit_index)
{
	bitarray_t bit_arr = bitarray;
	assert(bit_index < WORD_SIZE);
	bit_arr =  bit_arr & ~(1UL << bit_index);
	
	return bit_arr;
}

bitarray_t BitArrSetBit(bitarray_t bitarray, size_t bit_index, int set_to)
{
	assert(0 == set_to || 1 == set_to);
	assert(bit_index < WORD_SIZE);
	
	if (0 == set_to)
	{
		return BitArrSetBitOff(bitarray, bit_index);
	}
	else 
	{
		return BitArrSetBitOn(bitarray, bit_index);
	}
}

bitarray_t BitArrFlipBit(bitarray_t bitarray, size_t bit_index)
{
	bitarray_t bit_arr = bitarray;
	assert(bit_index < WORD_SIZE);
	
	bit_arr = bit_arr ^ (1UL << bit_index);
	
	return bit_arr;
}

int BitArrIsBitOn(bitarray_t bitarray, size_t bit_index)
{
	bitarray_t bit_arr = bitarray;
	assert(bit_index < WORD_SIZE);
	return(bit_arr & (1UL << bit_index));
}

int BitArrIsBitOff(bitarray_t bitarray, size_t bit_index)
{
	assert(bit_index < WORD_SIZE);
	return(!BitArrIsBitOn(bitarray, bit_index));
}

bitstate_t BitArrGetBit(bitarray_t bitarray, size_t bit_index)
{
	assert(bit_index < WORD_SIZE);
	if (BitArrIsBitOn(bitarray, bit_index))
	{
		return BIT_ON;
	}
	
	else 
	{
		return BIT_OFF;
	}
}

size_t BitArrCountBitsOn(bitarray_t bitarray)
{
	unsigned int count = 0;
    while (bitarray) 
    {
        bitarray &= (bitarray - 1);
        count++;
    }
    return count;
}

size_t BitArrCountBitsOff(bitarray_t bitarray)
{
	return(WORD_SIZE - BitArrCountBitsOn(bitarray));
}

bitarray_t BitArrSetAll()
{
	return 0xffffffffffffffff;
}

bitarray_t BitArrUnSetAll()
{
	return 0;
}

bitarray_t BitArrMirror(bitarray_t bitarray)
{
	assert( 64 == WORD_SIZE);
	bitarray = (bitarray & 0xFFFFFFFF00000000) >> 32 | (bitarray & 0x00000000FFFFFFFF) << 32;
    bitarray = (bitarray & 0xFFFF0000FFFF0000) >> 16 | (bitarray & 0x0000FFFF0000FFFF) << 16;
    bitarray = (bitarray & 0xFF00FF00FF00FF00) >> 8 | (bitarray & 0x00FF00FF00FF00FF) << 8;
    bitarray = (bitarray & 0xF0F0F0F0F0F0F0F0) >> 4 | (bitarray & 0x0F0F0F0F0F0F0F0F) << 4;
    bitarray = (bitarray & 0xCCCCCCCCCCCCCCCC) >> 2 | (bitarray & 0x3333333333333333) << 2;
    bitarray = (bitarray & 0xAAAAAAAAAAAAAAAA) >> 1 | (bitarray & 0x5555555555555555) << 1;
    return bitarray;
}

bitarray_t BitArrRotateRight(bitarray_t bitarray, size_t n)
{
	bitarray = (bitarray >> n)|(bitarray << (WORD_SIZE - n));
	return bitarray;

}

bitarray_t BitArrRotateLeft(bitarray_t bitarray, size_t n)
{
	bitarray = (bitarray << n)|(bitarray >> (WORD_SIZE - n));
	return bitarray;
}

char *BitArrArrToString(bitarray_t bitarray, char *buffer)
{

	int i = 0;

	unsigned char c = 0;
	
	
	assert(buffer);
	for ( i = WORD_SIZE -1; i >= 0; i--)
	{ 
		c = (bitarray >> i);
		if (1 & c)
		{
			buffer[WORD_SIZE - i - 1] = '1';
		}
		else
		{
			buffer[WORD_SIZE - i - 1] = '0';
		}

	}
	
	buffer[WORD_SIZE] = '\0';
	return buffer;
}

bitarray_t BitArrShiftLeft(bitarray_t bitarray, size_t n)
{
	return(bitarray << n);
}

bitarray_t BitArrShiftRight(bitarray_t bitarray, size_t n)
{
	return(bitarray >> n);
}
