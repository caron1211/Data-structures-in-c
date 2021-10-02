#include "calculator.h"
#include <stdio.h> /*printf*/

#define RED "\033[5;91m"           /* print in red          */
#define GREEN "\033[1;38;5;47m"    /* print in green        */
#define MAGEN "\033[0;38;5;183m"   /* print in magenta      */
#define YELLOW "\033[1;38;5;220m"  /* print in light yellow */
#define CYAN "\033[0;96m"          /* print in cyan         */
#define LIGHT "\033[2;37m"         /* print in gray         */
#define PURPLE "\033[1;4;38;5;224m"/* print in purple       */
#define RESET "\033[1;0m"          /* reset print color     */
#define WHITE "\033[0m"

static void PrintStatus(char *expression, double * res);

int main()
{
    double res = 0;
   
   
    char str1[100] = "7+8";                  /* Result = 15 status = SUCCESS       */
    char str2[100] = "8+8 *3+-2^5";         /* Result =  0 status = SUCCESS      */
    char str3[100] = "8+8 * 3-2^";         /* Result =  0 status = SYNTAX_ERROR  */
    char str4[100] = "2/0";                 /* Result =  0 status = MATH_ERROR   */
    char str5[100] = "8++8 * ((3-2) * 5)";     /* Result = 48 status = SUCCESS       */
    char str6[100] = "3-2) * 5";             /* Result =  0 status = SYNTAX_ERROR */
    char str7[100] = "";
    char str8[100] = "()";
    char str9[100] = "(-2)^4";
    char str10[100] = "8+90/9";

    PrintStatus(str1, &res);
    PrintStatus(str2, &res);
    PrintStatus(str3, &res);
    PrintStatus(str4, &res);
    PrintStatus(str5, &res);
    PrintStatus(str6, &res);
    PrintStatus(str7, &res);
    PrintStatus(str8, &res);
    PrintStatus(str9, &res);
    PrintStatus(str10, &res);

    return 0;
}

static void PrintStatus(char *expression, double * res)
{
    calculator_status_t status =  Calculate(expression, res);
    
    printf(CYAN"\n%s\n", expression);
    if (status == CALC_SUCCESS)
    {
        printf(GREEN"CALC_SUCCESS\n"RESET);
    }
    else if (status == CALC_MATH_ERROR)
    {
        printf(RED"CALC_MATH_ERROR\n"RESET);
    }
    else if (status == CALC_MEMORY_ALLOCATION_ERROR)
    {
        printf(RED"CALC_MEMORY_ALLOCATION_ERROR\n"RESET);
    }
    else if (status == CALC_SYNTAX_ERROR)
    {
        printf(RED"CALC_SYNTAX_ERROR\n"RESET);
    }

    printf("res is %f \n", *res);
}