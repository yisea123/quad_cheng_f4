#include "PosControl.h"
#include "QMath.h"



PosControl::PosControl(const AHRS& ahrs, const Motors& motors,
	AttitudeControl& attitude_control,
	InertialNav& inav,
	PID& p_alt_pos, PID& p_alt_rate, PID& pid_alt_accel,
	PID& p_pos_xy, PID& pid_rate_x, PID& pid_rate_y) :
	_ahrs(ahrs),
	_motors(motors),
	_attitude_control(attitude_control),
	_inav(inav),
	_p_alt_pos(p_alt_pos),
	_p_alt_rate(p_alt_rate),
	_pid_alt_accel(pid_alt_accel),
	_p_pos_xy(p_pos_xy),
	_pid_rate_x(pid_rate_x),
	_pid_rate_y(pid_rate_y),

	_throttle_hover(POSCONTROL_THROTTLE_HOVER),

	_last_update_xy_ms(0),
	_last_update_z_ms(0),
	_last_update_vel_xyz_ms(0),

	_speed_down_cms(POSCONTROL_SPEED_DOWN),	//最大下降速度,-150
	_speed_up_cms(POSCONTROL_SPEED_UP),		//最大上升速度,250
	_speed_cms(POSCONTROL_SPEED),			//最大水平速度
	_accel_z_cms(POSCONTROL_ACCEL_Z),		//最大垂直加速度
	_accel_cms(POSCONTROL_ACCEL_XY),		//最大水平加速度
	_leash(POSCONTROL_LEASH_LENGTH_MIN),	//水平最大误差
	_leash_down_z(POSCONTROL_LEASH_LENGTH_MIN),	//下降位置最大误差
	_leash_up_z(POSCONTROL_LEASH_LENGTH_MIN),	//上升位置最大误差

	_roll_target(0.0f),
	_pitch_target(0.0f),

	_alt_max(0),		//无高度限制

	_distance_to_target(0),
	_xy_step(0)
{

	_flags.force_recalc_xy = false;

	_flags.recalc_leash_xy = true;
	_flags.recalc_leash_z = true;

	_flags.keep_xy_I_terms = false;
	_flags.reset_desired_vel_to_pos = true;
	_flags.reset_rate_to_accel_xy = true;
	_flags.reset_rate_to_accel_z = true;
	_flags.reset_accel_to_throttle = true;



}
	
/// set_dt - sets time delta in seconds for all controllers (i.e. 100hz = 0.01, 400hz = 0.0025)
void PosControl::set_dt(float delta_sec)
{
	_dt = delta_sec;

	// update rate controller's d filter
	_pid_alt_accel.set_d_lpf_alpha(POSCONTROL_ACCEL_Z_DTERM_FILTER, _dt);

	// update rate z-axis velocity error and accel error filters
	_vel_error_filter.set_cutoff_frequency(_dt, POSCONTROL_VEL_ERROR_CUTOFF_FREQ);
	_accel_error_filter.set_cutoff_frequency(_dt, POSCONTROL_ACCEL_ERROR_CUTOFF_FREQ);
}


void PosControl::init_takeoff()
{
	const Vector3f& curr_pos = _inav.get_position();

	_pos_target.z = curr_pos.z + POSCONTROL_TAKEOFF_JUMP_CM;

	// freeze feedforward to avoid jump
	freeze_ff_z();

	// shift difference between last motor out and hover throttle into accelerometer I
	_pid_alt_accel.set_integrator(_motors.get_throttle_out() - _throttle_hover);
}

void PosControl::set_alt_target_from_climb_rate(float climb_rate_cms, float dt, bool force_descend)
{
	// adjust desired alt if motors have not hit their limits
	// To-Do: add check of _limit.pos_down?
	//根据油门状态确定是否继续积分
	if ((climb_rate_cms < 0 && (!_motors.limit.throttle_lower || force_descend)) || (climb_rate_cms > 0 && !_motors.limit.throttle_upper && !_limit.pos_up)) {
		_pos_target.z += climb_rate_cms * dt;
	}

	// do not let target alt get above limit
	//高度限制
	if (_alt_max > 0 && _pos_target.z > _alt_max) {
		_pos_target.z = _alt_max;

		_limit.pos_up = true;
	}

	_vel_desired.z = climb_rate_cms;		//前馈速率?
}

// update_z_controller - fly to altitude in cm above home
void PosControl::update_z_controller(float dt)
{
	// check time since last cast

// 	uint32_t now = millis();
// 	if (now - _last_update_z_ms > POSCONTROL_ACTIVE_TIMEOUT_MS) {
// 		_flags.reset_rate_to_accel_z = true;
// 		_flags.reset_accel_to_throttle = true;
// 	}
//	_last_update_z_ms = now;

	// check if leash lengths need to be recalculated
	calc_leash_length_z();		//更新_leash_up_z,_leash_down_z

	// call position controller
	pos_to_rate_z(dt);
}



// calc_leash_length - calculates the vertical leash lengths from maximum speed, acceleration
//     called by pos_to_rate_z if z-axis speed or accelerations are changed
void PosControl::calc_leash_length_z()
{
	if (_flags.recalc_leash_z)
	{
		_leash_up_z = calc_leash_length(_speed_up_cms, _accel_z_cms, _p_alt_pos.kP());
		_leash_down_z = calc_leash_length(-_speed_down_cms, _accel_z_cms, _p_alt_pos.kP());
		_flags.recalc_leash_z = false;
	}
}

// calc_leash_length - calculates the horizontal leash length given a maximum speed, acceleration and position kP gain
float PosControl::calc_leash_length(float speed_cms, float accel_cms, float kP) const
{
	float leash_length;

	// sanity check acceleration and avoid divide by zero
	if (accel_cms <= 0.0f) {
		accel_cms = POSCONTROL_ACCELERATION_MIN;
	}

	// avoid divide by zero
	if (kP <= 0.0f) {
		return POSCONTROL_LEASH_LENGTH_MIN;
	}

	// calculate leash length
	if (speed_cms <= accel_cms / kP) {
		// linear leash length based on speed close in
		leash_length = speed_cms / kP;
	}
	else {
		// leash length grows at sqrt of speed further out
		leash_length = (accel_cms / (2.0f*kP*kP)) + (speed_cms*speed_cms / (2.0f*accel_cms));
	}

	// ensure leash is at least 1m long
	if (leash_length < POSCONTROL_LEASH_LENGTH_MIN) {
		leash_length = POSCONTROL_LEASH_LENGTH_MIN;
	}

	return leash_length;
}


// pos_to_rate_z - position to rate controller for Z axis
// calculates desired rate in earth-frame z axis and passes to rate controller
// vel_up_max, vel_down_max should have already been set before calling this method
void PosControl::pos_to_rate_z(float dt)
{
	float curr_alt = _inav.get_altitude();		//当前高度
	// half the distance we swap between linear and sqrt and the distance we offset sqrt.
	float linear_distance;  
							// clear position limit flags
	_limit.pos_up = false;
	_limit.pos_down = false;

	// calculate altitude error
	_pos_error.z = _pos_target.z - curr_alt;		//高度误差

	// do not let target altitude get too far from current altitude
	if (_pos_error.z > _leash_up_z)			//大于最大误差
	{
		_pos_target.z = curr_alt + _leash_up_z;
		_pos_error.z = _leash_up_z;
		_limit.pos_up = true;
	}
	if (_pos_error.z < -_leash_down_z) {
		_pos_target.z = curr_alt - _leash_down_z;
		_pos_error.z = -_leash_down_z;
		_limit.pos_down = true;
	}



	// check kP to avoid division by zero
	//计算期望速度
	if (_p_alt_pos.kP() != 0.0f) 
	{
		linear_distance = _accel_z_cms / (2.0f*_p_alt_pos.kP()*_p_alt_pos.kP());

		if (_pos_error.z > 2 * linear_distance) {
			_vel_target.z = safe_sqrt(2.0f*_accel_z_cms*(_pos_error.z - linear_distance));
		}
		else if (_pos_error.z < -2.0f*linear_distance) {
			_vel_target.z = -safe_sqrt(2.0f*_accel_z_cms*(-_pos_error.z - linear_distance));
		}
		else {
			_vel_target.z = _p_alt_pos.get_p(_pos_error.z);
		}
	}
	else 
	{
		_vel_target.z = 0;
	}

	// call rate based throttle controller which will update accel based throttle controller targets
	rate_to_accel_z(dt);
}

// rate_to_accel_z - calculates desired accel required to achieve the velocity target
// calculates desired acceleration and calls accel throttle controller
void PosControl::rate_to_accel_z(float dt)
{
	const Vector3f& curr_vel = _inav.get_velocity();
	float p;                                // used to capture pid values for logging
	float desired_accel;                    // the target acceleration if the accel based throttle is enabled, otherwise the output to be sent to the motors

											// check speed limits
											// To-Do: check these speed limits here or in the pos->rate controller
	_limit.vel_up = false;
	_limit.vel_down = false;

	//上升下降速度限制
	if (_vel_target.z < _speed_down_cms)
	{
		_vel_target.z = _speed_down_cms;
		_limit.vel_down = true;
	}
	if (_vel_target.z > _speed_up_cms) 
	{
		_vel_target.z = _speed_up_cms;
		_limit.vel_up = true;
	}

	// reset last velocity target to current target
	if (_flags.reset_rate_to_accel_z)
	{
		_vel_last.z = _vel_target.z;
		_flags.reset_rate_to_accel_z = false;
	}

	// feed forward desired acceleration calculation
	if (dt > 0.0f) 
	{
		if (!_flags.freeze_ff_z) 
		{
			_accel_feedforward.z = (_vel_target.z - _vel_last.z) / dt;
		}
		else 
		{
			// stop the feed forward being calculated during a known discontinuity
			_flags.freeze_ff_z = false;
		}
	}
	else 
	{
		_accel_feedforward.z = 0.0f;
	}

	// store this iteration's velocities for the next iteration
	_vel_last.z = _vel_target.z;

	// reset velocity error and filter if this controller has just been engaged
	if (_flags.reset_rate_to_accel_z) 
	{
		// Reset Filter
		_vel_error.z = 0;
		_vel_error_filter.reset(0);
		desired_accel = 0;
		_flags.reset_rate_to_accel_z = false;
	}
	else 
	{
		// calculate rate error and filter with cut off frequency of 2 Hz
		_vel_error.z = _vel_error_filter.apply(_vel_target.z - curr_vel.z);
	}

	// calculate p
	p = _p_alt_rate.kP() * _vel_error.z;

	// consolidate and constrain target acceleration
	desired_accel = _accel_feedforward.z + p;
	desired_accel = constrain_int32(desired_accel, -32000, 32000);

	// set target for accel based throttle controller
	accel_to_throttle(desired_accel,dt);
}

// accel_to_throttle - alt hold's acceleration controller
// calculates a desired throttle which is sent directly to the motors
void PosControl::accel_to_throttle(float accel_target_z,float dt)
{
	float z_accel_meas;         // actual acceleration
	int32_t p, i, d;              // used to capture pid values for logging

								  // Calculate Earth Frame Z acceleration
	//当前加速度
	z_accel_meas = -(_ahrs.get_accel_ef().z + GRAVITY_MSS) * 100.0f;

	// reset target altitude if this controller has just been engaged
	if (_flags.reset_accel_to_throttle) 
	{
		// Reset Filter
		_accel_error.z = 0;
		_accel_error_filter.reset(0);
		_flags.reset_accel_to_throttle = false;
	}
	else 
	{
		// calculate accel error and Filter with fc = 2 Hz
		_accel_error.z = _accel_error_filter.apply(constrain_float(accel_target_z - z_accel_meas, -32000, 32000));
	}

	// separately calculate p, i, d values for logging
	p = _pid_alt_accel.get_p(_accel_error.z);

	// get i term
	i = _pid_alt_accel.get_integrator();

	// update i term as long as we haven't breached the limits or the I term will certainly reduce
	// To-Do: should this be replaced with limits check from attitude_controller?
	if ((!_motors.limit.throttle_lower && !_motors.limit.throttle_upper) || (i > 0 && _accel_error.z < 0) || (i < 0 && _accel_error.z>0)) {
		i = _pid_alt_accel.get_i(_accel_error.z, dt);
	}

	// get d term
	d = _pid_alt_accel.get_d(_accel_error.z, dt);

	// To-Do: pull min/max throttle from motors
	// To-Do: we had a contraint here but it's now removed, is this ok?  with the motors library handle it ok?
	_attitude_control.set_throttle_out((int16_t)p + i + d + _throttle_hover, true);

	// to-do add back in PID logging?
}


void PosControl::set_speed_z(float speed_down, float speed_up)
{
	// ensure speed_down is always negative
	speed_down = (float)-fabs(speed_down);

	if (((float)fabs(_speed_down_cms - speed_down) > 1.0f) || ((float)fabs(_speed_up_cms - speed_up) > 1.0f)) {
		_speed_down_cms = speed_down;
		_speed_up_cms = speed_up;
		_flags.recalc_leash_z = true;
	}
}
// set_accel_z - set vertical acceleration in cm/s/s
void PosControl::set_accel_z(float accel_cmss)
{
	if ((float)fabs(_accel_z_cms - accel_cmss) > 1.0f) {
		_accel_z_cms = accel_cmss;
		_flags.recalc_leash_z = true;
	}
}

// set_target_to_stopping_point_z - returns reasonable stopping altitude in cm above home
void PosControl::set_target_to_stopping_point_z()
{
	// check if z leash needs to be recalculated
	calc_leash_length_z();
	get_stopping_point_z(_pos_target);
}


// get_stopping_point_z - sets stopping_point.z to a reasonable stopping altitude in cm above home
void PosControl::get_stopping_point_z(Vector3f& stopping_point) const
{
	const float curr_pos_z = _inav.get_altitude();	//当前位置
	float curr_vel_z = _inav.get_velocity_z();		//当前速度

	float linear_distance;  // half the distance we swap between linear and sqrt and the distance we offset sqrt
	float linear_velocity;  // the velocity we swap between linear and sqrt

							// if position controller is active add current velocity error to avoid sudden jump in acceleration
// 	if (is_active_z()) {
// 		curr_vel_z += _vel_error.z;
// 	}

	// calculate the velocity at which we switch from calculating the stopping point using a linear function to a sqrt function
	linear_velocity = _accel_z_cms / _p_alt_pos.kP();

	if ((float)fabs(curr_vel_z) < linear_velocity)
	{
		// if our current velocity is below the cross-over point we use a linear function
		stopping_point.z = curr_pos_z + curr_vel_z / _p_alt_pos.kP();
	}
	else 
	{
		linear_distance = _accel_z_cms / (2.0f*_p_alt_pos.kP()*_p_alt_pos.kP());

		if (curr_vel_z > 0)
		{
			stopping_point.z = curr_pos_z + (linear_distance + curr_vel_z*curr_vel_z / (2.0f*_accel_z_cms));
		}
		else 
		{
			stopping_point.z = curr_pos_z - (linear_distance + curr_vel_z*curr_vel_z / (2.0f*_accel_z_cms));
		}
	}

	stopping_point.z = constrain_float(stopping_point.z, curr_pos_z - POSCONTROL_STOPPING_DIST_Z_MAX, curr_pos_z + POSCONTROL_STOPPING_DIST_Z_MAX);
}

