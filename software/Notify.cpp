#include "Notify.h"
#include "HAL_F4.h"


struct Notify::notify_flags Notify::flags;

void Notify::init(void)
{

}


//10Hz
void Notify::update(void)
{

	static int8_t arm_counter = 0;

	_count++;

	//IMU校准，保持静止
	if (Notify::flags.initialising)
	{
		if(_count&1)//5Hz
		{
			hal.led1 = 1;
			hal.led2 = 0;
		}
		else
		{
			hal.led1 = 0;
			hal.led2 = 1;
		}
		return;
		
	}

	hal.led1 = 0;



	if(Notify::flags.armed)	//已解锁
	{
		hal.led2 = 1;
	}
	else//未解锁,显示是否可以解锁
	{
		if (_count & 0x2)arm_counter++;//4分频,2Hz

		if (Notify::flags.pre_arm_check)//可解锁，单闪
		{
			switch(arm_counter)
			{
			case 0:
			case 1:
			case 2:
				hal.led2 = 1;
				break;
			case 3:
			case 4:
			case 5:
				hal.led2 = 0;
				break;
			default:
				arm_counter = -1;
				break;


			}
		}
		else//不可解锁，双闪
		{
			switch (arm_counter)
			{
			case 0:
				hal.led2 = 1;
				break;
			case  1:
				hal.led2 = 0;
				break;
			case 2:
				hal.led2 = 1;
				break;
			case 3:
			case 4:
				hal.led2 = 0;
				break;
			default:
				arm_counter = -1;
				break;
			}
		}
	}
	
	
}
