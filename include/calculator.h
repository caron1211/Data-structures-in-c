#ifndef __A_H__
#define __A_H__


typedef enum
{
    CALC_SUCCESS,
    CALC_MATH_ERROR,
    CALC_MEMORY_ALLOCATION_ERROR,
    CALC_SYNTAX_ERROR
} calculator_status_t;

calculator_status_t Calculate(const char *expression, double *res);

#endif /* __A_H__ */
