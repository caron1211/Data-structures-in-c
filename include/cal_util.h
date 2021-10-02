#ifndef __CAL_UTIL_H__
#define __CAL_UTIL_H__

#include "stack.h"
#include "calculator.h"

typedef enum
{
    WAIT_FOR_NUM = 0,
    WAIT_FOR_OP,
    END,
    ERROR,
    NUM_STATES
} state_t;

typedef enum
{
    OPEN_PAR_P = 0,
    ADD_P = 1,
    SUB_P = 1,
    MULTI_P = 2,
    DIVIDE_P = 2,
    POWER_P = 3,
    CLOSE_PAR_P = 4
} priority_t;

typedef struct
{
    char *expression;
    stack_t *num_stack;
    stack_t *op_stack;
    state_t curr_state;
} calculator_t;

typedef calculator_status_t (*action_func_t) (calculator_t * cal);
typedef double (*operation_func_t) (double, double);

typedef struct 
{
    priority_t priority;
    operation_func_t operation_func;
}operation_t;

typedef struct 
{
    state_t next_state;
    action_func_t action_func;
}transition_t;

static calculator_t *CalCreate(const char *expression);
static void CalDestroy(calculator_t *calculator);

static calculator_status_t OperationHandler(calculator_t * calc);
static calculator_status_t EmptyFunc(calculator_t * calc);
static calculator_status_t NumberHandler(calculator_t * calc);
static calculator_status_t LastOp(calculator_t *calculator);
static calculator_status_t ExecuteOperation(calculator_t *calculator);
static calculator_status_t OperationHandler(calculator_t * calc);
static calculator_status_t OpenParenthesisHandler(calculator_t * calc);
static calculator_status_t CloseParenthesisHandler(calculator_t * calc);
static calculator_status_t SpaceHandler(calculator_t * calc);
static void SetStateError(calculator_t * calc);
static void InitLutOp();
static void InitLutTransition();

static double Multi(double a, double b);
static double Add(double a, double b);
static double Divide(double a, double b);
static double Sub(double a, double b);
static double Power(double a, double b);
#endif /* __CAL_UTIL_H__ */
