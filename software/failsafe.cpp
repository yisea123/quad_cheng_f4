#include "failsafe.h"
#include "Parameters.h"
#include "Motors.h"

failsafe_t failsafe;
extern Parameters g;
extern Motors motors;



void failsafe_radio_on_event(void);		//关闭失控保护
void failsafe_radio_off_event(void);	//开启失控保护
void failsafe_set_radio(uint8_t b)		//失控保护
{
	if (failsafe.radio != b)
	{
		failsafe.radio = b;
		if (failsafe.radio == 0)
		{
			failsafe_radio_off_event();//关闭失控保护
		}
		else
		{
			failsafe_radio_on_event();//启动失控保护失控保护
		}
	}
}



void failsafe_radio_on_event()
{
	
}

void failsafe_radio_off_event()
{
}


#define FS_COUNTER 3  
void set_throttle_failsafe(uint16_t throttle_pwm)
{
	if (g.failsafe_throttle == FS_THR_DISABLED)		////禁止油门保护,直接设置
	{
		return;
	}

	//check for low throttle value
	if (throttle_pwm < g.failsafe_throttle_value)			//失控油门
	{
		if (failsafe.radio || !motors.armed())			//未解锁或已经在失控保护模式下则忽略
		{
			return;
		}

		failsafe.radio_counter++;
		if (failsafe.radio_counter >= FS_COUNTER)	//确定为失控油门
		{
			failsafe.radio_counter = FS_COUNTER;  // check to ensure we don't overflow the counter
			failsafe_set_radio(true);
		}
	}
	else if (failsafe.radio_counter)
	{		//油门正常,减小失控保护记录
			// we have a good throttle so reduce failsafe counter
		failsafe.radio_counter--;
		if (failsafe.radio_counter <= 0)
		{
			failsafe.radio_counter = 0;   // check to ensure we don't underflow the counter
			if (failsafe.radio)
			{
				failsafe_set_radio(false);
			}
		}
	}
}



void failsafe_enable()
{

}

void failsafe_disable()
{

}

