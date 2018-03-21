#include "flight_mode.h"
#include "Parameters.h"
#include "defines.h"
#include "PosControl.h"


uint8_t control_mode = STABILIZE;
extern PosControl pos_control;



static bool stabilize_init(bool ignore_checks)
{
	//设置期望高度为0,自稳模式下无用


	return true;
}

static bool althold_init(bool ignore_checks)
{
	//设置期望z轴高度,
	//设置当前加速度?速度??
	pos_control.set_speed_z(-g.pilot_velocity_z_max, g.pilot_velocity_z_max);
	pos_control.set_accel_z(g.pilot_accel_z);
	pos_control.set_target_to_stopping_point_z();		//将期望高度设置为当前高度

	return true;

}

static bool arco_init(bool ignore_checks)
{
	return true;
}



static void exit_mode(uint8_t old_control_mode,uint8_t new_control_mode)
{

}


bool set_mode(uint8_t mode,bool is_arm)
{
	bool success = false;
	bool ignore_checks = !is_arm;

	if(mode == control_mode){
		return true;
	}


	switch (mode)
	{
	case STABILIZE:
		success = stabilize_init(ignore_checks);
		break;
	case ALT_HOLD:
		success = althold_init(ignore_checks);
		break;
	case ACRO:
		success = arco_init(ignore_checks);
		break;
	default:
		break;
	}

	if(success)
	{
		exit_mode(control_mode, mode);
		control_mode = mode;
	}

	return success;

}






