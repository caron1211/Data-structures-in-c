#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <unistd.h> /* sleep */
#include "scheduler.h" 
#include "pqueue.h" /* pq_t */
#include "task.h" 

struct scheduler
{
    pq_t *tasks;
    task_t *current_task;
    int remove_current_task;	
    int to_stop;
};

static int WrapperIsMatch(void *id1, void *id2);

scheduler_t *SchedulerCreate()
{
	scheduler_t *sched = NULL;
	pq_t *tasks = NULL;
	sched = (scheduler_t *) malloc(sizeof(scheduler_t));
	tasks = PQCreate((pq_cmp_t)TaskIsBefore);
	if (NULL == tasks)
	{
		free(sched);
		sched = NULL;
		return (NULL);
	}
	
	sched->current_task = NULL;
	sched->remove_current_task = 0;
	sched->to_stop = 0;
	sched->tasks = tasks;
	
	return sched;
}


void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	SchedulerClear(scheduler);
	
	PQDestroy(scheduler->tasks);
	free(scheduler);
	scheduler = NULL;
}


ilrd_uid_t SchedulerAddTask(scheduler_t *scheduler, operation_func_t operation_func, destroy_func_t destroy_func, size_t interval, void *param)
{
	task_t *task = NULL;
	
	assert(NULL != scheduler);
	
	task = TaskCreate(operation_func, destroy_func, interval, param);
	if (NULL == task)
	{
		return GetBadUid();
	}
	
	if (1 == PQEnqueue(scheduler->tasks, task)) 
	{
		return GetBadUid();
	}
	
	return TaskGetUid(task);
}

int SchedulerRemoveTask(scheduler_t *scheduler, ilrd_uid_t task_id)
{
	task_t *task = NULL;
	
	assert(NULL != scheduler);
	
	task = (task_t *) PQErase(scheduler->tasks, WrapperIsMatch, (void *) &task_id);
	if (NULL == task)
	{
		return 1;
	}
	
	TaskDestroy(task);
	
	return 0;
}


void SchedulerClear(scheduler_t *scheduler)
{
	size_t count = SchedulerSize(scheduler);
	assert(NULL != scheduler);
	
	if (scheduler->current_task != NULL)
	{
		--count;
	}    
    while(count > 0)
    {
        TaskDestroy((task_t *)PQDequeue(scheduler->tasks));
        --count;
    }
    
    scheduler->remove_current_task = 1;
}


size_t SchedulerSize(const scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	return (PQCount(scheduler->tasks) + (scheduler->current_task != NULL));
}


int SchedulerIsEmpty(const scheduler_t *scheduler)
{
	return (SchedulerSize(scheduler) == 0);
}


sch_status_t SchedulerRun(scheduler_t *scheduler)
{
	sch_status_t status = EMPTY;
	task_status_t repeat = 0;
	
	assert(NULL != scheduler);
	
	scheduler->to_stop = 0;
	
	while (!SchedulerIsEmpty(scheduler))
	{
	
		if (scheduler->to_stop)
		{
			return (STOPPED);
		}
		scheduler->current_task = PQDequeue(scheduler->tasks);
		/* to_sleep = TaskGetTimeToRun(scheduler->current_task) - time(NULL);
		to_sleep = (to_sleep < 0 ? 0 : to_sleep);
		sleep(to_sleep); */

		while (TaskGetTimeToRun(scheduler->current_task) > time(NULL))
        {
            sleep(TaskGetTimeToRun(scheduler->current_task) - time(NULL));
        }

		
		repeat = TaskRun(scheduler->current_task);
		if (DONT_REPEAT == repeat)
		{
			scheduler->remove_current_task = 1;
		}
		
		if (scheduler->remove_current_task)
		{
			TaskDestroy(scheduler->current_task);
			scheduler->remove_current_task = 0;
		}
		
		else /* not remove */
		{
			TaskUpdateTimeToRun(scheduler->current_task);
			status = PQEnqueue(scheduler->tasks, scheduler->current_task);
			if (MEMORY_FAIL == status)
			{
				TaskDestroy(scheduler->current_task);
				scheduler->current_task = NULL;
				return MEMORY_FAIL;
			}
		}
		
		scheduler->current_task = NULL;
	}
	
	return (EMPTY);
}


void SchedulerStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	scheduler->to_stop = 1;
}

static int WrapperIsMatch(void *id1, void *id2)
{
	return (TaskIsMatch((task_t *)id1, *(ilrd_uid_t *)id2));
}

