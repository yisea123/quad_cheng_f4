#include "Attitude.h"
#include "config.h"
#include "Parameters.h"


extern Parameters g;


void get_pilot_desired_lean_angles(int16_t roll_in,int16_t pitch_in,int16_t &roll_out,int16_t &pitch_out)
{
	static float _scaler = 1.0;
	static int16_t _angle_max = 0;

	//оч╥Ы
	roll_in = constrain_int16(roll_in, -RC_RPY_INPUT_MAX, RC_RPY_INPUT_MAX);
	pitch_in = constrain_int16(pitch_in, -RC_RPY_INPUT_MAX, RC_RPY_INPUT_MAX);

	if(g.angle_max == RC_RPY_INPUT_MAX){
		roll_out = roll_in;
		pitch_out = pitch_in;
		return;
	}

	if(g.angle_max != _angle_max){
		_angle_max = g.angle_max;
		_scaler = (float)g.angle_max / (float)RC_RPY_INPUT_MAX;
	}

	roll_out = (int16_t)((float)roll_in*_scaler);
	pitch_out = (int16_t)((float)pitch_in*_scaler);

}


float get_pilot_desired_yaw_rate(int16_t stick_angle)
{
	// convert pilot input to the desired yaw rate
	return stick_angle * g.acro_yaw_p;
}

int16_t get_pilot_desired_throttle(int16_t throttle_control)
{
	
	if (throttle_control == 0||g.throttle_mid == 500)
	{
		return throttle_control;
	}
	
	
	int16_t throttle_out;
	throttle_control = constrain_int16(throttle_control, 0, 1000);
	g.throttle_mid = constrain_int16(g.throttle_mid, 300, 700);

	if(throttle_control < 500)
	{
		throttle_out = g.throttle_min + ((float)(throttle_control - g.throttle_min))*((float)(g.throttle_mid - g.throttle_min)) / ((float)(500 - g.throttle_min));
	}else if (throttle_control > 500)
	{
		throttle_out = g.throttle_mid + ((float)(throttle_control - 500))*((float)(1000 - g.throttle_mid)) / 500;
	}
	else
	{
		throttle_out = throttle_control;
	}



	return throttle_out;
}

float get_smoothing_gain()
{
	return (2.0f + (float)g.rc_feel_rp / 10.0f);//
}

