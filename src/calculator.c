#include <math.h> /* pow */
#include <stdlib.h> /*malloc, free */
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include "calculator.h"
#include "cal_util.h"
#include "stack.h"

#define ASCI_SIZE 256

static operation_t g_lut_op[ASCI_SIZE] = {0};
static transition_t g_lut_transition[ASCI_SIZE][NUM_STATES] = {0};

calculator_status_t Calculate(const char *expression, double *res)
{
	void *data = NULL;
    int curr_chr = 0;
    calculator_t *calc = NULL;
	calculator_status_t status = CALC_SUCCESS;

    assert(NULL != expression);
    assert(NULL != res);


	calc = CalCreate(expression);
    if (NULL == calc)
    {
        return (CALC_MEMORY_ALLOCATION_ERROR);
    }

	while (calc->curr_state != END && calc->curr_state != ERROR)
	{
        curr_chr = *(calc->expression);
		status = g_lut_transition[curr_chr][calc->curr_state].action_func(calc);
		calc->curr_state = g_lut_transition[curr_chr][calc->curr_state].next_state;
	}

    data = StackPeek(calc->num_stack);
	*res = *(double *)&data;
	StackPop(calc->num_stack);
   
	CalDestroy(calc);

	return (status);
}

static calculator_t *CalCreate(const char *expression)
{
    stack_t *op_stack = NULL;
    stack_t *num_stack = NULL;
    calculator_t *calc = NULL;

    op_stack = StackCreate(20);
    if (NULL == op_stack)
    {
        return (NULL);

    }
    num_stack = StackCreate(20);
    if (NULL == num_stack)
    {
        StackDestroy(op_stack);
        return (NULL);

    }

    InitLutOp();
    InitLutTransition();
    calc = (calculator_t *) malloc(sizeof(calculator_t));
    if (NULL == calc)
    {
        StackDestroy(op_stack);
        StackDestroy(num_stack);
        return (NULL);

    }

    calc->expression = (char *) expression;
    calc->num_stack = num_stack;
    calc->op_stack = op_stack;
    calc->curr_state = WAIT_FOR_NUM;

    return (calc);

}

static void CalDestroy(calculator_t *calc)
{
    StackDestroy(calc->num_stack);
	StackDestroy(calc->op_stack);
	free(calc);
}

static void InitLutOp()
{
    g_lut_op['+'].priority = ADD_P;
    g_lut_op['+'].operation_func = Add;

    g_lut_op['-'].priority = SUB_P;
    g_lut_op['-'].operation_func = Sub;

    g_lut_op['/'].priority = DIVIDE_P;
    g_lut_op['/'].operation_func = Divide;

    g_lut_op['*'].priority = MULTI_P;
    g_lut_op['*'].operation_func = Multi;

    g_lut_op['^'].priority = POWER_P;
    g_lut_op['^'].operation_func = Power;
}

static void InitLutTransition()
{

    size_t i = 0;
    size_t j = 0;

    for (i = 0; i < ASCI_SIZE ; i++)
    {
        for  (j = 0; j < NUM_STATES; j++)
        {
           g_lut_transition[i][j].action_func = EmptyFunc;
            g_lut_transition[i][j].next_state = ERROR;
        }
    }

    for (i = '0'; i <= '9'; ++i)
    {
        g_lut_transition[i][WAIT_FOR_NUM].next_state = WAIT_FOR_OP;
        g_lut_transition[i][WAIT_FOR_NUM].action_func = NumberHandler;
    }

    g_lut_transition['+'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
	g_lut_transition['+'][WAIT_FOR_OP].action_func = OperationHandler;

    g_lut_transition['+'][WAIT_FOR_NUM].next_state = WAIT_FOR_OP;
	g_lut_transition['+'][WAIT_FOR_NUM].action_func = NumberHandler;

	g_lut_transition['-'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
	g_lut_transition['-'][WAIT_FOR_OP].action_func = OperationHandler;

    g_lut_transition['-'][WAIT_FOR_NUM].next_state = WAIT_FOR_OP;
	g_lut_transition['-'][WAIT_FOR_NUM].action_func = NumberHandler;

	g_lut_transition['*'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
	g_lut_transition['*'][WAIT_FOR_OP].action_func = OperationHandler;

	g_lut_transition['/'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
	g_lut_transition['/'][WAIT_FOR_OP].action_func = OperationHandler;

	g_lut_transition['^'][WAIT_FOR_OP].next_state = WAIT_FOR_NUM;
	g_lut_transition['^'][WAIT_FOR_OP].action_func = OperationHandler;

	g_lut_transition['('][WAIT_FOR_NUM].next_state = WAIT_FOR_NUM;
	g_lut_transition['('][WAIT_FOR_NUM].action_func = OpenParenthesisHandler; 

	g_lut_transition[')'][WAIT_FOR_OP].next_state = WAIT_FOR_OP;
	g_lut_transition[')'][WAIT_FOR_OP].action_func = CloseParenthesisHandler; 

	g_lut_transition['\0'][WAIT_FOR_OP].next_state= END;
	g_lut_transition['\0'][WAIT_FOR_OP].action_func = LastOp;
    
    g_lut_transition[' '][WAIT_FOR_OP].next_state= WAIT_FOR_OP;
	g_lut_transition[' '][WAIT_FOR_OP].action_func = SpaceHandler;

    g_lut_transition[' '][WAIT_FOR_NUM].next_state= WAIT_FOR_NUM;
	g_lut_transition[' '][WAIT_FOR_NUM].action_func = SpaceHandler;
}

static void SetStateError(calculator_t * calc)
{
    calc->curr_state = ERROR;
    StackPush(calc->num_stack, (void*) 0);
}

static calculator_status_t EmptyFunc(calculator_t * calc)
{
    assert(NULL != calc);
    SetStateError(calc);
    return (CALC_SYNTAX_ERROR);
}

static calculator_status_t SpaceHandler(calculator_t * calc)
{
    calc = calc;
    calc->expression++;
    return (CALC_SUCCESS);
}

static calculator_status_t NumberHandler(calculator_t * calc)
{
    char *str = NULL;
    double num = strtod(calc->expression, &str);

    if (str == calc->expression)
    {
        SetStateError(calc);
        return (CALC_SYNTAX_ERROR);
    }

    StackPush(calc->num_stack, *(void **)&num);
    calc->expression += (str - calc->expression );

    return (CALC_SUCCESS);
}

static calculator_status_t OperationHandler(calculator_t * calc)
{
    void *tmp = NULL;
    char curr_operator = *(calc->expression);
    char stack_operator = 0;
    int to_stop = 0;
    calculator_status_t status = CALC_SUCCESS;
    
    while (!StackIsEmpty(calc->op_stack) && !to_stop && calc->curr_state!= ERROR)
    {
        tmp = StackPeek(calc->op_stack);
        stack_operator = *(char*)&tmp;
        
        if (g_lut_op[(int)stack_operator].priority >= g_lut_op[(int)curr_operator].priority)
        {
            status = ExecuteOperation(calc);
        }
        else
        {
             to_stop = 1;
        }
    }

    StackPush(calc->op_stack, *(void **)&curr_operator);
    calc->expression++;
	
	return (status);
   
}

static calculator_status_t ExecuteOperation(calculator_t *calc)
{
	double res = 0;
    void *num1 = NULL;
    void *num2 = NULL;
    void *op = NULL;
    char stack_operator = 0;

	num1 = StackPeek(calc->num_stack);
    StackPop(calc->num_stack);

	num2 = StackPeek(calc->num_stack);
    StackPop(calc->num_stack);

	op = StackPeek(calc->op_stack);
    StackPop(calc->op_stack);

	stack_operator = *(char *)&op;

    if (stack_operator == '(' )
	{
        SetStateError(calc);
		return (CALC_SYNTAX_ERROR);
	}

	if (stack_operator == '/' && *(double *)&num1 == 0)
	{
        SetStateError(calc);
		return (CALC_MATH_ERROR);
	}

	res = g_lut_op[(int)stack_operator].operation_func(*(double *)&num2, *(double *)&num1);
	StackPush(calc->num_stack, *(void **)&res);

	return (CALC_SUCCESS);
}

static calculator_status_t OpenParenthesisHandler(calculator_t * calc)
{
    char curr_operator = *(calc->expression);
    StackPush(calc->op_stack, *(void **)&curr_operator);
    calc->expression++;

    return (CALC_SUCCESS);
}

static calculator_status_t CloseParenthesisHandler(calculator_t * calc)
{
    void *op = NULL;
    calculator_status_t status = CALC_SUCCESS;
    int open_par_found = 0;
    while (!StackIsEmpty(calc->op_stack) && calc->curr_state != ERROR && !open_par_found)
    {
        op = StackPeek(calc->op_stack);
        if (*(char *)&op == '(')
        {
            open_par_found = 1;
            StackPop(calc->op_stack);
        }
        else 
        {
            status = ExecuteOperation(calc);
        }
    }

    if(open_par_found == 0)
    {
        status = CALC_SYNTAX_ERROR;
        SetStateError(calc);
    }

    calc->expression++;
    return status;
    
}

static calculator_status_t LastOp(calculator_t *calc)
{
    calculator_status_t status = CALC_SUCCESS;
	while (!StackIsEmpty(calc->op_stack) && calc->curr_state != ERROR)
	{
		status = ExecuteOperation(calc);
	}

    return (status);
}

/*****************************************************************************/
static double Multi(double a, double b)
{
    return (a * b);
}

static double Add(double a, double b)
{
    return (a + b);
}

static double Divide(double a, double b)
{
    return (a / b);
}

static double Sub(double a, double b)
{
    return (a - b);
}

static double Power(double a, double b)
{
    return pow(a, b);
}