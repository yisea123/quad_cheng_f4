#include "Scheduler.h"




void Scheduler::init(const scheduler_tasks_t *tasks, uint16_t num_tasks)
{
	//清除所有timer事件
	uint16_t i;
	for (i = 0; i < SCHEDULER_MAX_TIMER_PROCS; i++)
	{
		_timer_proc[i] = NULL;
	}
	_timer_suspended = false;
	_in_timer_proc = false;
	_timer_event_missed = false;
	
	
	//加载task
	_tasks = tasks;
	_num_tasks = num_tasks;
	printf("task num=%d\r\n", _num_tasks);

	uint32_t tnow = micros();
	for (i = 0; i < SCHEDULER_MAX_TASK; i++)
	{
		_last_time[i] = tnow;
	}
	_current_task = -1;
}



void Scheduler::suspend_timer_procs() 
{
    _timer_suspended = true;
}

void Scheduler::resume_timer_procs() 
{
    _timer_suspended = false;
	
    if (_timer_event_missed == true) 	//有任务漏掉了
	{
//        _run_timer_procs(false);
        _timer_event_missed = false;
    }
}

void Scheduler::register_timer_process(timer_process_t proc)
{
	uint16_t i;
	for (i = 0; i < _num_timer_procs;i++)	//重复注册
	{
		if (_timer_proc[i] == proc)
			return;
	}
	if (_num_timer_procs < SCHEDULER_MAX_TIMER_PROCS)
	{
		//printf("timer register\n");
		_timer_proc[_num_timer_procs] = proc;
		_num_timer_procs++;
	}
}
//由定时器调用
void Scheduler::timer_event()
{
	_in_timer_proc = true;
	if(!_timer_suspended)
	{
		uint16_t i;
		for(i=0;i<_num_timer_procs;i++)
		{
			if(_timer_proc[i] != NULL)
			{
				_timer_proc[i]();
			}	
		}
	}
	_in_timer_proc = 0;
}

/***************************************************************/
void Scheduler::run()
{
	
	uint32_t tnow;
	uint32_t dt;
	uint16_t i;
	
	while(1)
	{
		tnow = micros();
		for(i=0;i<_num_tasks;i++)
		{
			dt = tnow - _last_time[i];
			if(dt >= _tasks[i].interval_micros)		
			{		
				_current_task = i;
				_last_time[i] =  tnow;
				_tasks[i].function();					//运行任务
				_current_task = -1;
				
				tnow = micros();							//更新当前时间
				_time_taken[i] = tnow - _last_time[i];		//实际调用消耗时间
				
				if(_time_taken[i] > _tasks[i].max_time_micros)		//运行超时
				{
						//
				}
				
			}
		}
	
		
	}

}
