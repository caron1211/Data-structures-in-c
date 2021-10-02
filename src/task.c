#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "task.h" /* time_t */


struct task
{
    ilrd_uid_t uid;
    task_func_t task_func;
    task_destroy_func_t destroy_func;
    size_t interval;
    time_t time_to_run;
    void *param;
};


task_t *TaskCreate(task_func_t task_func, task_destroy_func_t destroy_func, size_t interval, void *param)
{
	task_t *task = NULL;
	
	assert(NULL != task_func); 

	
	task = (task_t *) malloc(sizeof(task_t));
	if (NULL == task)
	{
		return (NULL);
	}
	task->uid = UidCreate();
	task->task_func = task_func;
	task->destroy_func = destroy_func;
	task->interval = interval;
	
	task->param = param;
	task->time_to_run = time(NULL) + task->interval;
	
	return (task);	
}


void TaskDestroy(task_t *task)
{
	assert(NULL !=task);
	
	if (NULL != task->destroy_func)
	{
		task->destroy_func(task->uid, task->param);
	}
	
	free(task);
	task = NULL;
}


time_t TaskGetTimeToRun(task_t *task)
{
	assert(NULL !=task);
	
	return (task->time_to_run);
}


task_status_t TaskRun(task_t *task)
{
	assert(NULL !=task);
	
	return (task->task_func(task->param));
}


int TaskIsMatch(const task_t *task, ilrd_uid_t task_id)
{

	assert(NULL !=task);
	
	return (UidIsSame(TaskGetUid(task), task_id));
}


ilrd_uid_t TaskGetUid(const task_t *task)
{
	assert(NULL !=task);
	
	return (task->uid);
}


void TaskUpdateTimeToRun(task_t *task)
{
	assert(NULL !=task);
	
	task->time_to_run = time(NULL) + task->interval;
}


int TaskIsBefore(const void *task1, const void *task2)
{
	assert(NULL !=task1);
	assert(NULL !=task2);
	
	return (TaskGetTimeToRun ((task_t *) task1) - TaskGetTimeToRun ((task_t *) task2));
}



