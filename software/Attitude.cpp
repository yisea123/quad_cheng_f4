#include "Attitude.h"
#include "config.h"
#include "Parameters.h"
#include "PosControl.h"

#include "failsafe.h"

extern Parameters g;
extern PosControl pos_control;
extern Motors motors;

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


#define THROTTLE_IN_MIDDLE 500          // the throttle mid point
#define THROTTLE_IN_DEADBAND_TOP (THROTTLE_IN_MIDDLE+g.throttle_deadzone)  // top of the deadband
#define THROTTLE_IN_DEADBAND_BOTTOM (THROTTLE_IN_MIDDLE-g.throttle_deadzone)  // bottom of the deadband



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

int16_t get_pilot_desired_climb_rate(int16_t throttle_control)
{
	int16_t desired_rate = 0;

	// throttle failsafe check
	if (failsafe.radio) {
		return 0;
	}

	// ensure a reasonable throttle value
	throttle_control = constrain_int16(throttle_control, 0, 1000);

	// ensure a reasonable deadzone
	g.throttle_deadzone = constrain_int16(g.throttle_deadzone, 0, 400);

	// check throttle is above, below or in the deadband
	if (throttle_control < THROTTLE_IN_DEADBAND_BOTTOM) {
		// below the deadband
		desired_rate = (int32_t)g.pilot_velocity_z_max * (throttle_control - THROTTLE_IN_DEADBAND_BOTTOM) / (THROTTLE_IN_MIDDLE - g.throttle_deadzone);
	}
	else if (throttle_control > THROTTLE_IN_DEADBAND_TOP) {
		// above the deadband
		desired_rate = (int32_t)g.pilot_velocity_z_max * (throttle_control - THROTTLE_IN_DEADBAND_TOP) / (THROTTLE_IN_MIDDLE - g.throttle_deadzone);
	}
	else {
		// must be in the deadband
		desired_rate = 0;
	}

	return desired_rate;
}

void set_throttle_takeoff()
{
	// tell position controller to reset alt target and reset I terms
	pos_control.init_takeoff();

	// tell motors to do a slow start
	motors.slow_start();
}
static int16_t get_non_takeoff_throttle()
{
	return (g.throttle_mid / 2.0f);
}
int16_t get_throttle_pre_takeoff(int16_t throttle_control)
{
	int16_t throttle_out;

	// exit immediately if throttle_control is zero
	if (throttle_control <= 0) {
		return 0;
	}

	// sanity check throttle input
	throttle_control = constrain_int16(throttle_control, 0, 1000);

	// sanity check throttle_mid
	g.throttle_mid = constrain_int16(g.throttle_mid, 300, 700);

	// sanity check throttle_min vs throttle_mid
	if (g.throttle_min > get_non_takeoff_throttle()) {
		return g.throttle_min;
	}

	// check throttle is below top of deadband
	if (throttle_control < THROTTLE_IN_DEADBAND_TOP) 
	{
		throttle_out = g.throttle_min + ((float)(throttle_control - g.throttle_min))*((float)(get_non_takeoff_throttle() - g.throttle_min)) / ((float)(THROTTLE_IN_DEADBAND_TOP - g.throttle_min));
	}
	else 
	{
		// must be in the deadband
		throttle_out = get_non_takeoff_throttle();
	}

	return throttle_out;
}

float get_smoothing_gain()
{
	return (2.0f + (float)g.rc_feel_rp / 10.0f);//
}

