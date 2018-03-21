#include "AttitudeControl.h"
#include "Parameters.h"
#include "QMath.h"

extern Parameters g;





void AttitudeControl::set_dt(float delta_sec)
{
	_dt = delta_sec;
	_pid_rate_roll.set_d_lpf_alpha(20, _dt);
	_pid_rate_pitch.set_d_lpf_alpha(20, _dt);
	_pid_rate_yaw.set_d_lpf_alpha(20, _dt);

}

void AttitudeControl::relax_bf_rate_controller()
{
	_rate_bf_target = _ahrs.get_gyro()*ATTITUDE_CONTROL_DEGX100;
}

void AttitudeControl::update_ef_yaw_angle_and_error(float yaw_rate_ef, Vector3f &angle_ef_error, float overshoot_max,float dt)
{
	// calculate angle error with maximum of +- max angle overshoot
	angle_ef_error.z = wrap_180_cd(_angle_ef_target.z - _ahrs.yaw_sensor);
	angle_ef_error.z = constrain_float(angle_ef_error.z, -overshoot_max, overshoot_max);

	// update yaw angle target to be within max angle overshoot of our current heading
	_angle_ef_target.z = _ahrs.yaw_sensor + angle_ef_error.z;

	// increment the yaw angle target
	_angle_ef_target.z += yaw_rate_ef * dt;
	_angle_ef_target.z = wrap_180_cd(_angle_ef_target.z);
}

void AttitudeControl::frame_conversion_ef_to_bf(const Vector3f& ef_vector, Vector3f& bf_vector)
{
	// convert earth frame rates to body frame rates,欧拉角导数转为载体角速率
	bf_vector.x = ef_vector.x - _ahrs.sin_pitch() * ef_vector.z;
	bf_vector.y = _ahrs.cos_roll()  * ef_vector.y + _ahrs.sin_roll() * _ahrs.cos_pitch() * ef_vector.z;
	bf_vector.z = -_ahrs.sin_roll() * ef_vector.y + _ahrs.cos_pitch() * _ahrs.cos_roll() * ef_vector.z;
}

void AttitudeControl::update_rate_bf_targets()
{
	// stab roll calculation
	_rate_bf_target.x = _p_angle_roll.kP() * _angle_bf_error.x;
	// constrain roll rate request
	if (_flags.limit_angle_to_rate_request) {
		_rate_bf_target.x = constrain_float(_rate_bf_target.x, -_angle_rate_rp_max, _angle_rate_rp_max);
	}

	// stab pitch calculation
	_rate_bf_target.y = _p_angle_pitch.kP() * _angle_bf_error.y;
	// constrain pitch rate request
	if (_flags.limit_angle_to_rate_request) {
		_rate_bf_target.y = constrain_float(_rate_bf_target.y, -_angle_rate_rp_max, _angle_rate_rp_max);
	}

	// stab yaw calculation
	_rate_bf_target.z = _p_angle_yaw.kP() * _angle_bf_error.z;
	// constrain yaw rate request
	if (_flags.limit_angle_to_rate_request) {
		_rate_bf_target.z = constrain_float(_rate_bf_target.z, -_angle_rate_y_max, _angle_rate_y_max);
	}

	//????
// 	_rate_bf_target.x += _angle_bf_error.y * _ahrs.get_gyro().z;
// 	_rate_bf_target.y += -_angle_bf_error.x * _ahrs.get_gyro().z;
}

void AttitudeControl::angle_ef_roll_pitch_rate_ef_yaw_smooth(float roll_angle_ef, float pitch_angle_ef, float yaw_rate_ef, float dt,float smoothing_gain)
{
	Vector3f angle_ef_error;

	
// 	smoothing_gain = constrain_float(smoothing_gain, 1.0f, 50.0f);
// 	float linear_angle = _accel_rp_max / (smoothing_gain*smoothing_gain);
// 	float rate_ef_desired;
// 	float angle_to_target;

	_angle_ef_target.x = constrain_float(roll_angle_ef, -g.angle_max, g.angle_max);
	angle_ef_error.x = wrap_180_cd_float(_angle_ef_target.x - _ahrs.roll_sensor);


	_angle_ef_target.y = constrain_float(pitch_angle_ef, -g.angle_max, g.angle_max);
	angle_ef_error.y = wrap_180_cd_float(_angle_ef_target.y - _ahrs.pitch_sensor);

	_rate_ef_desired.x = 0;		//取消前馈
	_rate_ef_desired.y = 0;


	_rate_ef_desired.z = yaw_rate_ef;	//yaw前馈角速率?
	//计算误差,yaw角速率积分
	update_ef_yaw_angle_and_error(_rate_ef_desired.z, angle_ef_error, ATTITUDE_RATE_STAB_YAW_OVERSHOOT_ANGLE_MAX,dt);


	frame_conversion_ef_to_bf(angle_ef_error, _angle_bf_error);			//误差转到bf
	update_rate_bf_targets();
	frame_conversion_ef_to_bf(_rate_ef_desired, _rate_bf_desired);		//前馈转到bf

	_rate_bf_target += _rate_bf_desired;

}




//////////////////////////////////////////////////////////////////////////
//throttle
//////////////////////////////////////////////////////////////////////////
void AttitudeControl::set_throttle_out(int16_t throttle_out, bool apply_angle_boost)
{
	if (apply_angle_boost)
	{
		_motors.set_throttle(get_angle_boost(throttle_out));
	}
	else 
	{
		_motors.set_throttle(throttle_out);
		_angle_boost = 0;
	}
}
int16_t AttitudeControl::get_angle_boost(int16_t throttle_pwm)
{
	float temp = _ahrs.cos_pitch() * _ahrs.cos_roll();	//_ahrs._dcm_matrix.c.z;
	int16_t throttle_out;
	temp = constrain_float(temp, 0.5f, 1.0f);
	//角度在60度以内的值都不变,大于60度时按比例减小
	temp = constrain_float(9000 - max(fabsf(_ahrs.roll_sensor), fabsf(_ahrs.pitch_sensor)), 0, 3000) / (3000 * temp);
	// apply scale and constrain throttle
	throttle_out = constrain_float((float)(throttle_pwm - _motors.throttle_min()) * temp + _motors.throttle_min(), _motors.throttle_min(), _motors.throttle_max());

	// record angle boost for logging
	_angle_boost = throttle_out - throttle_pwm;

	return throttle_out;
}



//////////////////////////////////////////////////////////////////////////
//rate control
//////////////////////////////////////////////////////////////////////////
float AttitudeControl::_rate_bf_to_motor_roll(float rate_roll_target_cds, float dt)
{
	float p, i=0.0f, d;           // used to capture pid values for logging
	float current_rate;     // this iteration's rate
	float rate_error;       // simply target_rate - current_rate
	float integ;

	current_rate = _ahrs.get_gyro().x * ATTITUDE_CONTROL_DEGX100;	//当前速度

	rate_error = rate_roll_target_cds - current_rate;		//误差

	p = _pid_rate_roll.get_p(rate_error);		//比例

	integ = _pid_rate_roll.get_integrator();	//积分
	//解锁时,无限制时进行积分
	if (_motors.armed() && (!_motors.limit.roll_pitch || (integ > 0 && rate_error < 0) || (integ < 0 && rate_error>0)))
	{
		i = _pid_rate_roll.get_i(rate_error, dt);
	}

	d = _pid_rate_roll.get_d(rate_error, dt);		//微分

	return constrain_float((p + i + d), -ATTITUDE_RATE_RP_CONTROLLER_OUT_MAX, ATTITUDE_RATE_RP_CONTROLLER_OUT_MAX);

}
float AttitudeControl::_rate_bf_to_motor_pitch(float rate_pitch_target_cds, float dt)
{
	float p, i=0.0f, d;            // used to capture pid values for logging
	float current_rate;     // this iteration's rate
	float rate_error;       // simply target_rate - current_rate
	float integ;

	current_rate = _ahrs.get_gyro().y * ATTITUDE_CONTROL_DEGX100;	//当前速度

	rate_error = rate_pitch_target_cds - current_rate;		//误差

	p = _pid_rate_pitch.get_p(rate_error);

	integ = _pid_rate_pitch.get_integrator();
	if (_motors.armed() &&(!_motors.limit.roll_pitch || (integ > 0 && rate_error < 0) || (integ < 0 && rate_error>0)))
	{
		i = _pid_rate_pitch.get_i(rate_error, dt);
	}

	d = _pid_rate_pitch.get_d(rate_error, dt);

	return constrain_float((p + i + d), -ATTITUDE_RATE_RP_CONTROLLER_OUT_MAX, ATTITUDE_RATE_RP_CONTROLLER_OUT_MAX);
}
float AttitudeControl::_rate_bf_to_motor_yaw(float rate_yaw_target_cds,float dt)
{
	float p, i, d;            // used to capture pid values for logging
	float current_rate;     // this iteration's rate
	float rate_error;       // simply target_rate - current_rate
	float integ;

	current_rate = _ahrs.get_gyro().z * ATTITUDE_CONTROL_DEGX100;	//当前速度

	rate_error = rate_yaw_target_cds - current_rate;		//误差
	p = _pid_rate_yaw.get_p(rate_error);

	integ = _pid_rate_yaw.get_integrator();
	if(_motors.armed() && (!_motors.limit.yaw || (integ>0&&rate_error<0) || (integ<0&&rate_error>0)))
	{
		i = _pid_rate_yaw.get_i(rate_error, dt);
	}

	d = _pid_rate_yaw.get_d(rate_error, dt);

	return constrain_float((p + i + d), -ATTITUDE_RATE_YAW_CONTROLLER_OUT_MAX, ATTITUDE_RATE_YAW_CONTROLLER_OUT_MAX);

}
void AttitudeControl::rate_control_run(float dt)
{
	_motors.set_roll(_rate_bf_to_motor_roll(_rate_bf_target.x, dt));
	_motors.set_pitch(_rate_bf_to_motor_pitch(_rate_bf_target.y, dt));
	_motors.set_yaw(_rate_bf_to_motor_yaw(_rate_bf_target.z, dt));

}







