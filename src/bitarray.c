#include <assert.h> /* assert */
#include <limits.h> /* CHAR_BIT */

#include "bitarray.h"

#define TRUE 1
#define FALSE 0

#define BIT_ARRAY_SIZE (sizeof(size_t) * CHAR_BIT)


bitarray_t BitArrSetBitOn(bitarray_t bitarray, size_t bit_index);
bitarray_t BitArrSetBitOff(bitarray_t bitarray, size_t bit_index);
bitarray_t BitArrSetBit(bitarray_t bitarray, size_t bit_index, int set_to);
bitarray_t BitArrFlipBit(bitarray_t bitarray, size_t bit_index);
int BitArrIsBitOn(bitarray_t bitarray, size_t bit_index);
int BitArrIsBitOff(bitarray_t bitarray, size_t bit_index);
bitstate_t BitArrGetBit(bitarray_t bitarray, size_t bit_index);
size_t BitArrCountBitsOn(bitarray_t bitarray);
size_t BitArrCountBitsOff(bitarray_t bitarray);
bitarray_t BitArrSetAll();
bitarray_t BitArrUnSetAll();
bitarray_t BitArrMirror(bitarray_t bitarray);
bitarray_t BitArrRotateRight(bitarray_t bitarray, size_t n);
bitarray_t BitArrRotateLeft(bitarray_t bitarray, size_t n);
char *BitArrArrToString(bitarray_t bitarray, char *buffer);

bitarray_t BitArrSetBitOn(bitarray_t bitarray, size_t bit_index)
{
	size_t mask = 0x01;
	
	assert(bit_index < BIT_ARRAY_SIZE);
	
	mask <<= bit_index;
	bitarray |= mask;
	
	return bitarray;
}

bitarray_t BitArrSetBitOff(bitarray_t bitarray, size_t bit_index)
{
	size_t mask = 0x01; 
	
	assert(bit_index < BIT_ARRAY_SIZE);
	
	mask <<= bit_index;
	mask = ~mask; /* all bits are set to 1, except for bit_index */ 
	bitarray &= mask;
	
	return bitarray;
}

bitarray_t BitArrSetBit(bitarray_t bitarray, size_t bit_index, int set_to)
{
	assert(bit_index < BIT_ARRAY_SIZE);
	assert(set_to == 1 || set_to == 0);
	
	return ((set_to == 0) ? 
	BitArrSetBitOff(bitarray, bit_index) : 
	BitArrSetBitOn(bitarray, bit_index));
}

bitarray_t BitArrFlipBit(bitarray_t bitarray, size_t bit_index)
{
	bitstate_t bit_state = BitArrGetBit(bitarray, bit_index);
	
	assert(bit_index < BIT_ARRAY_SIZE);
	
	return (BIT_ON == bit_state ? 
			BitArrSetBit(bitarray, bit_index, BIT_OFF) :
			BitArrSetBit(bitarray, bit_index, BIT_ON)) ;
}

int BitArrIsBitOn(bitarray_t bitarray, size_t bit_index)
{
	size_t mask = 0x01;
	size_t tmp = 0x00;
	
	assert(bit_index < BIT_ARRAY_SIZE);
	
	mask <<= bit_index;
	tmp = mask & bitarray;
	
	return (tmp > 0 ? TRUE : FALSE);
}

int BitArrIsBitOff(bitarray_t bitarray, size_t bit_index)
{
	return !BitArrIsBitOn(bitarray, bit_index);
}

bitstate_t BitArrGetBit(bitarray_t bitarray, size_t bit_index)
{
	size_t mask = 0x01;
	size_t tmp = 0x00;
	
	assert(bit_index < BIT_ARRAY_SIZE);
	
	mask <<= bit_index;
	tmp = mask & bitarray;
	
	return (tmp > 0x00 ? BIT_ON : BIT_OFF);
}

size_t BitArrCountBitsOn(bitarray_t bitarray)
{
	size_t count = 0;
	
    while (bitarray) 
    {
        bitarray &= (bitarray - 1);
        count++;
    }
    
	return count;
}

size_t BitArrCountBitsOff(bitarray_t bitarray)
{
	return BIT_ARRAY_SIZE - BitArrCountBitsOn(bitarray);
}

bitarray_t BitArrSetAll()
{
	return ~(0x00);
}

bitarray_t BitArrUnSetAll()
{
	return 0x00;
}

/*bitarray_t BitArrMirror(bitarray_t bitarray)
{
	bitarray_t res = 0x00;
	size_t mask = 0x01;
	size_t lsb_bit = 0x00;
	size_t i = 0;
	
	for(i = 0; i < BIT_ARRAY_SIZE; i++)
	{
		lsb_bit = mask & bitarray;
		res <<= 1;
		res |= lsb_bit;
		bitarray >>= 1;
	}
	
	return res; 
}*/
bitarray_t BitArrMirror(bitarray_t bitarray)
{    

    bitarray = (bitarray & 0x00000000FFFFFFFF) << 32 | (bitarray & 0xFFFFFFFF00000000) >> 32;
    bitarray = (bitarray & 0x0000FFFF0000FFFF) << 16 | (bitarray & 0xFFFF0000FFFF0000) >> 16;
    bitarray = (bitarray & 0x00FF00FF00FF00FF) << 8  | (bitarray & 0xFF00FF00FF00FF00) >>  8;
    bitarray = (bitarray & 0x0F0F0F0F0F0F0F0F) << 4  | (bitarray & 0xF0F0F0F0F0F0F0F0) >>  4;
    bitarray = (bitarray & 0x3333333333333333) << 2  | (bitarray & 0xCCCCCCCCCCCCCCCC) >>  2;
    bitarray = (bitarray & 0x5555555555555555) << 1  | (bitarray & 0xAAAAAAAAAAAAAAAA) >>  1;
    
    return (bitarray); 
}

bitarray_t BitArrRotateRight(bitarray_t bitarray, size_t n)
{
	size_t lsb_bit = 0x00; 
	size_t i = 0;
	
	/* in case of n >= 64 were back to where we started.. redundant.*/
	for(i = 0; i < n % BIT_ARRAY_SIZE; i++)
	{	
		lsb_bit = 0x01 & bitarray;	
		bitarray >>= 1;
		lsb_bit <<= (BIT_ARRAY_SIZE - 1);
		bitarray |= lsb_bit;
	}
	
	return bitarray;
}

bitarray_t BitArrRotateLeft(bitarray_t bitarray, size_t n)
{
	size_t i = 0;
	size_t msb_bit = 0x00;
	
	size_t msb_mask = 0x01;
	msb_mask <<=  (BIT_ARRAY_SIZE - 1);
	
	/* in case of n >= 64 were back to where we started.. redundant.*/
	for(i = 0; i < n % BIT_ARRAY_SIZE; i++)
	{	
		msb_bit = msb_mask & bitarray;	
		msb_bit >>= (BIT_ARRAY_SIZE - 1);
		bitarray <<= 1;
		bitarray |= msb_bit;
	}
	
	return bitarray;
}

char *BitArrArrToString(bitarray_t bitarray, char *buffer)
{
	size_t i = 0;
	bitstate_t curr_state = BIT_OFF;
	
	assert(NULL != buffer);
	
	buffer[BIT_ARRAY_SIZE] = '\0';
	for(i = 0; i < BIT_ARRAY_SIZE; i++)
	{
		curr_state = BitArrGetBit(bitarray, i);
		if (BIT_ON == curr_state)
		{
			buffer[BIT_ARRAY_SIZE - 1 - i] = '1';
		}
		else if(BIT_OFF == curr_state)
		{
			buffer[BIT_ARRAY_SIZE - 1 - i] = '0';
		}
	}
	
	return buffer;
}

bitarray_t BitArrShiftLeft(bitarray_t bitarray, size_t n)
{
	return bitarray <<= n;
}

bitarray_t BitArrShiftRight(bitarray_t bitarray, size_t n)
{
	return bitarray >>= n;
}


















