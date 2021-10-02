#ifndef __UTILITIES_H__
#define __UTILITIES_H__

/********************************* test utilities ******************************/

/*********************** color macros for printf *********************************/

#define RED "\033[5;91m"           /* print in red          */
#define GREEN "\033[1;38;5;47m"    /* print in green        */
#define MAGEN "\033[0;38;5;183m"   /* print in magenta      */
#define YELLOW "\033[1;38;5;220m"  /* print in light yellow */
#define CYAN "\033[0;96m"          /* print in cyan         */
#define LIGHT "\033[2;37m"         /* print in gray         */
#define PURPLE "\033[1;4;38;5;224m"/* print in purple       */
#define RESET "\033[1;0m"          /* reset print color     */
#define WHITE "\033[0m"


/*****************************************************************************/

#define TEST_IS_EQUAL(test_name, expected, real)\
			(real == expected ?\
			printf("%s:" GREEN" PASS\n"WHITE, test_name) :\
			printf("%s:"  RED" FAIL\n"WHITE, test_name))

#define TEST_IS_TRUE(test_name, result)\
			(result ?\
			printf("%s:" GREEN" PASS\n"WHITE, test_name) :\
			printf("%s:"  RED" FAIL\n"WHITE, test_name))

#define TEST_IS_FALSE(test_name, result)\
			(!result ?\
			printf("%s:" GREEN" PASS\n"WHITE, test_name) :\
			printf("%s:"  RED" FAIL\n"WHITE, test_name))

#define TEST_IS_GREATER_ZERO(test_name, result)\
			(return > 0 ?\
			printf("%s:" GREEN" PASS\n"WHITE, test_name) :\
			printf("%s:"  RED" FAIL\n"WHITE, test_name))		
     				
/********************************* others: ******************************/




#endif /* __UTILITIES_H__ */


