#include <stdio.h> /* printf */
#include "bitaray.h"

#define TEST_IS_EQUAL(test_name, expected, real)\
			(real == expected ?\
			printf("%s - PASS\n", test_name) :\
			printf("%s - FAIL\n", test_name))

#define TEST_IS_TRUE(test_name, result)\
			(result ?\
			printf("%s - PASS\n", test_name) :\
			printf("%s - FAIL\n", test_name))

#define TEST_IS_FALSE(test_name, result)\
			(!result ?\
			printf("%s - PASS\n", test_name) :\
			printf("%s - FAIL\n", test_name))		

static void BitArrSetBitOnTest();
static void BitArrSetBitOffTest();
static void BitArrSetBitTest();

static void BitArrFlipBitTest();
static void BitArrIsBitOnTest();
static void BitArrIsBitOffTest();

static void BitArrGetBitTest();
static void BitArrCountBitsOnTest();
static void BitArrCountBitsOffTest();

static void BitArrSetAllTest();
static void BitArrUnSetAllTest();
static void BitArrMirrorTest();

static void BitArrRotateRightTest();
static void BitArrRotateLeftTest();
static void BitArrArrToStringTest();


int main()
{


	BitArrSetBitOnTest();
	BitArrSetBitOffTest();
	BitArrSetBitTest();
	
	BitArrFlipBitTest();
	BitArrIsBitOnTest();
	BitArrIsBitOffTest();
	
	BitArrGetBitTest();
	BitArrCountBitsOnTest();
	BitArrCountBitsOffTest();
	
	BitArrSetAllTest();
	BitArrUnSetAllTest();
	BitArrMirrorTest();
	
	BitArrRotateRightTest();
	BitArrRotateLeftTest();
	BitArrArrToStringTest();

	
	return 0;
}

static void BitArrSetBitOnTest()
{
	bitarray_t my_arr = 0;
	
	my_arr = BitArrSetBitOn(my_arr, 0); /* ...0001 */
	TEST_IS_EQUAL("BitArrSetBitOnTest", my_arr, 1);
	
	my_arr = BitArrSetBitOn(my_arr, 6); /* ...1000001 */
	TEST_IS_EQUAL("BitArrSetBitOnTest", my_arr, 65);
	
	my_arr = BitArrSetBitOn(my_arr, 3); /* ...1001001 */
	TEST_IS_EQUAL("BitArrSetBitOnTest", my_arr, 73);

}

static void BitArrSetBitOffTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	
	my_arr = BitArrSetBitOff(my_arr, 0); /* ...100 */
	TEST_IS_EQUAL("BitArrSetBitOffTest", my_arr, 4);
	
	
	my_arr = BitArrSetBitOff(my_arr, 2); /* ...000 */
	TEST_IS_EQUAL("BitArrSetBitOffTest", my_arr, 0);
	
}

static void BitArrSetBitTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	
	my_arr = BitArrSetBit(my_arr, 0, 0); /* ...100 */
	TEST_IS_EQUAL("BitArrSetBitTest", my_arr, 4);
	
	my_arr = BitArrSetBit(my_arr, 3, 1); /* ...1100 */
	TEST_IS_EQUAL("BitArrSetBitTest", my_arr, 12);

	my_arr = BitArrSetBit(my_arr, 2, 0); /* ...1000 */
	TEST_IS_EQUAL("BitArrSetBitTest", my_arr, 8);
}

static void BitArrFlipBitTest() 
{
	bitarray_t my_arr = 5;               /* ...101 */
	
	my_arr = BitArrFlipBit(my_arr, 0); /* ...100 */
	TEST_IS_EQUAL("BitArrFlipBitTest", my_arr, 4);
	
	my_arr = BitArrFlipBit(my_arr, 0); /* ...101 */
	TEST_IS_EQUAL("BitArrFlipBitTest", my_arr, 5);
	
	my_arr = BitArrFlipBit(my_arr, 2); /* ...001 */
	TEST_IS_EQUAL("BitArrFlipBitTest", my_arr, 1);
}

static void BitArrIsBitOnTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	int res = 0;
	
	res = BitArrIsBitOn(my_arr, 0); 
	TEST_IS_TRUE("BitArrIsBitOnTest", res);
	
	res = BitArrIsBitOn(my_arr, 1); 
	TEST_IS_FALSE("BitArrIsBitOnTest", res);
	
	res = BitArrIsBitOn(my_arr, 25); 
	TEST_IS_FALSE("BitArrIsBitOnTest", res);
	
	res = BitArrIsBitOn(my_arr, 2); 
	TEST_IS_TRUE("BitArrIsBitOnTest", res);
	
}

static void BitArrIsBitOffTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	int res = 0;
	
	res = BitArrIsBitOff(my_arr, 0); 
	TEST_IS_FALSE("BitArrIsBitOffTest", res);
	
	res = BitArrIsBitOff(my_arr, 1); 
	TEST_IS_TRUE("BitArrIsBitOffTest", res);
	
	res = BitArrIsBitOff(my_arr, 25); 
	TEST_IS_TRUE("BitArrIsBitOffTest", res);
	
	res = BitArrIsBitOff(my_arr, 2); 
	TEST_IS_FALSE("BitArrIsBitOffTest", res);
}

static void BitArrGetBitTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	bitstate_t res = BIT_OFF;
	
	res = BitArrGetBit(my_arr, 0);
	TEST_IS_EQUAL("BitArrGetBitTest", res, BIT_ON);
	
	res = BitArrGetBit(my_arr, 1);
	TEST_IS_EQUAL("BitArrGetBitTest", res, BIT_OFF);
	
	res = BitArrGetBit(my_arr, 3);
	TEST_IS_EQUAL("BitArrGetBitTest", res, BIT_OFF);

}
static void BitArrCountBitsOnTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	unsigned res = 0;
	
	res = BitArrCountBitsOn(my_arr); 
	TEST_IS_EQUAL("BitArrCountBitsOnTest",res, 2);
	
	my_arr = 7;
	res = BitArrCountBitsOn(my_arr); 
	TEST_IS_EQUAL("BitArrCountBitsOnTest", res, 3);
		
}
static void BitArrCountBitsOffTest()
{

	bitarray_t my_arr = 5;               /* ...101 */
	unsigned res = 0;
	
	res = BitArrCountBitsOff(my_arr); 
	TEST_IS_EQUAL("BitArrCountBitsOff", res, 62);
	
	my_arr = 0;                          /* ...000 */
	res = BitArrCountBitsOff(my_arr); 
	TEST_IS_EQUAL("BitArrCountBitsOff", res, 64);
}

static void BitArrSetAllTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	
	my_arr = BitArrSetAll(my_arr); 
	TEST_IS_EQUAL("BitArrSetAllTest", my_arr, 0xffffffffffffffff);

}
static void BitArrUnSetAllTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	
	my_arr = BitArrUnSetAll(my_arr); 
	TEST_IS_EQUAL("BitArrUnSetAllTest", my_arr, 0);

}
static void BitArrMirrorTest()
{
	bitarray_t my_arr = 0;               /* ...000 */
	
	my_arr = BitArrMirror(my_arr); 
	TEST_IS_EQUAL("BitArrMirrorTest", my_arr, 0);
	
	my_arr = 0x000000000000000f;               /* ...1000 */
	
	my_arr = BitArrMirror(my_arr); 
	TEST_IS_EQUAL("BitArrMirrorTest", my_arr, 0xf000000000000000);
}

static void BitArrRotateRightTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	
	my_arr = BitArrRotateRight(my_arr, 1);  /* ..10 */
	TEST_IS_EQUAL("BitArrRotateRightTest", (unsigned int)my_arr, 2);
	
}

static void BitArrRotateLeftTest()
{
	bitarray_t my_arr = 5;               /* ...101 */
	
	my_arr = BitArrRotateLeft(my_arr, 1);  /* ..1010 */
	TEST_IS_EQUAL("BitArrRotateLeftTest", my_arr, 10);
	
}


static void BitArrArrToStringTest()
{
	char buffer[70];
	
	BitArrArrToString(10, buffer);
	printf("print 10: %s\n", buffer);
	
	BitArrArrToString(0xffffffffffffffff, buffer);
	printf("print 0xffffffffffffffff: %s\n", buffer);
	
}
