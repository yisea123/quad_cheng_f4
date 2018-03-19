#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_
#include "sys.h"
#include "config.h"
#include "Parameter.h"
#include "PID.h"
#include "RCIn.h"
#include "RCOut.h"
#include "defines.h"
#include "config.h"
class Parameters
{
public:
	Parameters() :

		//PID
		pid_rate_roll(RATE_ROLL_P, RATE_ROLL_I, RATE_ROLL_D, RATE_ROLL_IMAX),
		pid_rate_pitch(RATE_PITCH_P, RATE_PITCH_I, RATE_PITCH_D, RATE_PITCH_IMAX),
		pid_rate_yaw(RATE_YAW_P, RATE_YAW_I, RATE_YAW_D, RATE_YAW_IMAX),

		p_angle_roll(STABILIZE_ROLL_P),
		p_angle_pitch(STABILIZE_PITCH_P),
		p_angle_yaw(STABILIZE_YAW_P),

		p_alt_pos(ALT_HOLD_P),
		p_alt_rate(THROTTLE_RATE_P),
		pid_alt_accel(THROTTLE_ACCEL_P,THROTTLE_ACCEL_I,THROTTLE_ACCEL_D,THROTTLE_ACCEL_IMAX),



		land_speed(LAND_SPEED),
		pilot_velocity_z_max(PILOT_VELZ_MAX),
		pilot_accel_z(PILOT_ACCEL_Z_DEFAULT),
		//
		throttle_min(THR_MIN_DEFAULT),
		throttle_max(THR_MAX_DEFAULT),
		throttle_mid(THR_MID_DEFAULT),
		throttle_deadzone(THR_DZ_DEFAULT),

		failsafe_throttle(FS_THR_ENABLED),
		failsafe_throttle_value(FS_THR_VALUE_DEFAULT),

		use_compass(false),
		arming_check(ARMING_CHECK_INS),
		esc_cali(false),


		acro_yaw_p(ACRO_YAW_P),
		rc_feel_rp(RC_FEEL_RP_VERY_CRISP),
		angle_max(DEFAULT_ANGLE_MAX)
	{

	}

	RCOut rcout_roll;
	RCOut rcout_pitch;
	RCOut rcout_yaw;
	RCOut rcout_throttle;

	//RCIN
	RCIn rcin_roll;
	RCIn rcin_pitch;
	RCIn rcin_throttle;
	RCIn rcin_yaw;
 	RCIn rcin_ch5;
 	RCIn rcin_ch6;
 	RCIn rcin_ch7;
 	RCIn rcin_ch8;
	RCIn rcin_ch9;

	PID pid_rate_roll;
	PID pid_rate_pitch;
	PID pid_rate_yaw;

	PID p_angle_roll;
	PID p_angle_pitch;
	PID p_angle_yaw;

	PID p_alt_pos;
	PID p_alt_rate;
	PID pid_alt_accel;

	PID p_pos_xy;
	PID pid_rate_x;
	PID pid_rate_y;



	Param_Int16 land_speed;
	Param_Int16 pilot_velocity_z_max;	// maximum vertical velocity the pilot may request
	Param_Int16 pilot_accel_z;			//  acceleration the pilot may request

	//throtltle
	Param_Int16 throttle_min;
	Param_Int16 throttle_max;
	Param_Int16 throttle_mid;

	Param_UInt16 throttle_deadzone;
	Param_Int8 failsafe_throttle;
	Param_UInt16 failsafe_throttle_value;
	


	Param_UInt8 use_compass;
	Param_Int8 arming_check;
	Param_UInt8 esc_cali;


	//Param_UInt8 radio_type;
	Param_Float acro_yaw_p;	//期望转为速率
	Param_Int8 rc_feel_rp;
	Param_Int16 angle_max;


};


extern Parameters g;

#endif


