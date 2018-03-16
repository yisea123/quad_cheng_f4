#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "sys.h"
#include "delay.h"




const uint8_t SCHEDULER_MAX_TIMER_PROCS	= 4;		//定时器任务
const uint8_t SCHEDULER_MAX_TASK	= 20;			//轮询任务

typedef void (*timer_process_t)(void);
typedef void (*task_fn_t)(void);

typedef struct 
{
	task_fn_t function;
	uint32_t interval_micros;				//调用间隔
	uint32_t max_time_micros;				//最大占用时间	
}scheduler_tasks_t;




class Scheduler
{
public:
	Scheduler(){};
	~Scheduler(){};
	
	
	void init(const scheduler_tasks_t *tasks, uint16_t num_tasks);
	void run();
	

	void register_timer_process(timer_process_t proc);
	void suspend_timer_procs(void);
	void resume_timer_procs(void);
	void timer_event(void);

		
private:
	timer_process_t _timer_proc[SCHEDULER_MAX_TIMER_PROCS];
	uint16_t _num_timer_procs;
	bool _timer_suspended;			//暂停定时器任务
	bool _in_timer_proc;			//正在运行
	bool _timer_event_missed;

	const scheduler_tasks_t *_tasks;			//任务表
	uint16_t _num_tasks;						//任务数
	uint32_t _last_time[SCHEDULER_MAX_TASK];	//上次调用时间
	uint32_t _time_taken[SCHEDULER_MAX_TASK];	//任务消耗时间
	int16_t _current_task;						//当前运行任务
	
};







#endif

