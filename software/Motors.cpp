#include "Motors.h"
#include "Notify.h"
#include "QMath.h"
#include "HAL_F4.h"


void Motors::arm()
{
	_flags.armed = true;
	Notify::flags.armed = true;
}
void Motors::disarm()
{
	_flags.armed = false;

	_spin_when_armed_ramped = 0;
	_flags.slow_start_slow_end = true;

	Notify::flags.armed = false;
}
void Motors::add_motor_raw(int8_t motor_num, float roll_fac, float pitch_fac, float yaw_fac)
{
	if (motor_num >= 0 && motor_num < MOTORS_MAX_MOTOR_NUM)
	{
		motor_enabled[motor_num] = true;
		_roll_factor[motor_num] = roll_fac;
		_pitch_factor[motor_num] = pitch_fac;
		_yaw_factor[motor_num] = yaw_fac;
	}
}
void Motors::add_motor(int8_t motor_num, float angle_degree, float yaw_factor)
{
	add_motor(motor_num, angle_degree, angle_degree, yaw_factor);
}
void Motors::add_motor(int8_t motor_num, float roll_factor_in_degrees, float pitch_factor_degrees, float yaw_factor)
{
	add_motor_raw(motor_num,
		-sinf(radians(roll_factor_in_degrees)),
		cosf(radians(pitch_factor_degrees)),
		yaw_factor);

}
void Motors::remove_motor(int8_t motor_num)
{
	if (motor_num >= 0 && motor_num < MOTORS_MAX_MOTOR_NUM) 
	{
		// disable the motor, set all factors to zero
		motor_enabled[motor_num] = false;
		_roll_factor[motor_num] = 0;
		_pitch_factor[motor_num] = 0;
		_yaw_factor[motor_num] = 0;
	}
}
void Motors::remove_all_motors()
{
	for (int8_t i = 0; i < MOTORS_MAX_MOTOR_NUM; i++) {
		remove_motor(i);
	}
}



void Motors::output_min()
{
	limit.roll_pitch = true;
	limit.yaw = true;
	limit.throttle_lower = true;
	limit.throttle_upper = false;

	for (int8_t i = 0; i < MOTORS_MAX_MOTOR_NUM; i++)
	{
		if (motor_enabled[i]) 
		{
			motor_out[i] = _rc_throttle.radio_min;
			hal.pwm_write(i, _rc_throttle.radio_min);
		}
	}
}
void Motors::_output_armed(void)
{
	int8_t i;
	int16_t out_min_pwm = _rc_throttle.radio_min + _min_throttle;      // minimum pwm value we can send to the motors
	int16_t out_max_pwm = _rc_throttle.radio_max;                      // maximum pwm value we can send to the motors
	int16_t out_mid_pwm = (out_min_pwm + out_max_pwm) / 2;                  // mid pwm value we can send to the motors
	
	int16_t out_best_thr_pwm;  // the is the best throttle we can come up which provides good control without climbing
	float rpy_scale = 1.0; // this is used to scale the roll, pitch and yaw to fit within the motor limits

	int16_t rpy_out[MOTORS_MAX_MOTOR_NUM]; // buffer so we don't have to multiply coefficients multiple times.

	int16_t rpy_low = 0;    // lowest motor value
	int16_t rpy_high = 0;   // highest motor value
	int16_t yaw_allowed;    // amount of yaw we can fit in
	int16_t thr_adj;        // the difference between the pilot's desired throttle and out_best_thr_pwm (the throttle that is actually provided)

	// initialize limits flag
	limit.roll_pitch = false;
	limit.yaw = false;
	limit.throttle_lower = false;
	limit.throttle_upper = false;

	// Throttle is 0 to 1000 only
	// To-Do: we should not really be limiting this here because we don't "own" this _rc_throttle object
	if (_rc_throttle.servo_out <= 0)
	{
		_rc_throttle.servo_out = 0;
		limit.throttle_lower = true;
	}
	if (_rc_throttle.servo_out >= _max_throttle) 
	{
		_rc_throttle.servo_out = _max_throttle;
		limit.throttle_upper = true;
	}

	// capture desired roll, pitch, yaw and throttle from receiver
	_rc_roll.calc_pwm();		//pwm_out = (float)servo_out * 0.1f;
	_rc_pitch.calc_pwm();
	_rc_throttle.calc_pwm();	//
	_rc_yaw.calc_pwm();

	// if we are not sending a throttle output, we cut the motors
	if (_rc_throttle.servo_out == 0)	//解锁,0油门,怠速转
	{
		// range check spin_when_armed
		if (_spin_when_armed_ramped < 0) 
		{
			_spin_when_armed_ramped = 0;
		}
		if (_spin_when_armed_ramped > _min_throttle) 
		{
			_spin_when_armed_ramped = _min_throttle;
		}
		for (i = 0; i < MOTORS_MAX_MOTOR_NUM; i++)
		{
			// spin motors at minimum
			if (motor_enabled[i]) {
				motor_out[i] = _rc_throttle.radio_min + _spin_when_armed_ramped;
			}
		}

		// Every thing is limited
		limit.roll_pitch = true;
		limit.yaw = true;
	}
	else 
	{
		// check if throttle is below limit
		if (_rc_throttle.servo_out <= _min_throttle) 
		{  
			limit.throttle_lower = true;
		}

		//计算roll,pitch输出
		rpy_low = 0;	rpy_high = 0;
		for (i = 0; i < MOTORS_MAX_MOTOR_NUM; i++)
		{
			if (motor_enabled[i]) 
			{
				rpy_out[i] = _rc_roll.pwm_out * _roll_factor[i] +_rc_pitch.pwm_out * _pitch_factor[i];
				// record lowest roll pitch command
				if (rpy_out[i] < rpy_low) rpy_low = rpy_out[i];
				if (rpy_out[i] > rpy_high) rpy_high = rpy_out[i];

			}
		}


		// calculate throttle that gives most possible room for yaw (range 1000 ~ 2000) which is the lower of:
		//      1. mid throttle - average of highest and lowest motor (this would give the maximum possible room margin above the highest motor and below the lowest)
		//      2. the higher of:
		//            a) the pilot's throttle input
		//            b) the mid point between the pilot's input throttle and hover-throttle
		//      Situation #2 ensure we never increase the throttle above hover throttle unless the pilot has commanded this.
		//      Situation #2b allows us to raise the throttle above what the pilot commanded but not so far that it would actually cause the copter to rise.
		//      We will choose #1 (the best throttle for yaw control) if that means reducing throttle to the motors (i.e. we favour reducing throttle *because* it provides better yaw control)
		//      We will choose #2 (a mix of pilot and hover throttle) only when the throttle is quite low.  We favour reducing throttle instead of better yaw control because the pilot has commanded it

		
		//第二项:当radio_out<_hover_out时输出radio_out,当radio_out>_hover_out时输出第二项,减小输出幅度
		//当第二项过大时输出第一项,限制best_thr+pwm以提供足够的yaw_allow
		int16_t motor_mid = (rpy_low + rpy_high) / 2;
		out_best_thr_pwm = min(out_mid_pwm - motor_mid, max(_rc_throttle.radio_out, (_rc_throttle.radio_out + _hover_out) / 2));

		// calculate amount of yaw we can fit into the throttle range
		// this is always equal to or less than the requested yaw from the pilot or rate controller
		//油门剩余量减rp输出得yaw_allow,输出不要超过油门输出??
		yaw_allowed = min(out_max_pwm - out_best_thr_pwm, out_best_thr_pwm - out_min_pwm) - (rpy_high - rpy_low) / 2;
		yaw_allowed = max(yaw_allowed, 200);	//运行量不超过200
		//获取yaw输出
		if (_rc_yaw.pwm_out >= 0)
		{
			// if yawing right
			if (yaw_allowed > _rc_yaw.pwm_out) 
			{
				yaw_allowed = _rc_yaw.pwm_out; // to-do: this is bad form for yaw_allows to change meaning to become the amount that we are going to output
			}
			else 
			{
				limit.yaw = true;
			}
		}
		else 
		{
			// if yawing left
			yaw_allowed = -yaw_allowed;
			if (yaw_allowed < _rc_yaw.pwm_out) 
			{
				yaw_allowed = _rc_yaw.pwm_out; // to-do: this is bad form for yaw_allows to change meaning to become the amount that we are going to output
			}
			else 
			{
				limit.yaw = true;
			}
		}



		//添加yaw输出
		rpy_low = 0;	rpy_high = 0;
		for (i = 0; i < MOTORS_MAX_MOTOR_NUM; i++) 
		{
			if (motor_enabled[i])
			{
				rpy_out[i] = rpy_out[i] + yaw_allowed * _yaw_factor[i];
				// record lowest roll+pitch+yaw command
				if (rpy_out[i] < rpy_low)rpy_low = rpy_out[i];
				// record highest roll+pitch+yaw command
				if (rpy_out[i] > rpy_high) rpy_high = rpy_out[i];
			}
		}

		// check everything fits
		thr_adj = _rc_throttle.radio_out - out_best_thr_pwm;	//缺少的输出量

		// 确保非负
		int16_t thr_adj_max = max(out_max_pwm - (out_best_thr_pwm + rpy_high), 0);

		// if we are increasing the throttle (situation #2 above)..
		if (thr_adj > 0) 
		{
			// increase throttle as close as possible to requested throttle
			// without going over out_max_pwm
			if (thr_adj > thr_adj_max)
			{
				thr_adj = thr_adj_max;
				// we haven't even been able to apply full throttle command
				limit.throttle_upper = true;
			}
		}
		else if (thr_adj < 0) //out_best_thr_pwm大于实际输出
		{

			int16_t thr_adj_min = min(out_min_pwm - (out_best_thr_pwm + rpy_low), 0);//最大减小量
			if (thr_adj > thr_adj_max)
			{
				thr_adj = thr_adj_max;
				limit.throttle_upper = true;
			}

			if (thr_adj < thr_adj_min) 
			{
				thr_adj = thr_adj_min;
				limit.throttle_lower = true;
			}
		}

		//油门优先,计算rpy输出等比例缩放因子,以免超过范围
		if ((rpy_low + out_best_thr_pwm) + thr_adj < out_min_pwm)//输出过小
		{
			rpy_scale = (float)(out_min_pwm - thr_adj - out_best_thr_pwm) / rpy_low;
			// we haven't even been able to apply full roll, pitch and minimal yaw without scaling
			limit.roll_pitch = true;
			limit.yaw = true;
		}
		else if ((rpy_high + out_best_thr_pwm) + thr_adj > out_max_pwm)//输出过大
		{
			rpy_scale = (float)(out_max_pwm - thr_adj - out_best_thr_pwm) / rpy_high;
			// we haven't even been able to apply full roll, pitch and minimal yaw without scaling
			limit.roll_pitch = true;
			limit.yaw = true;
		}

		//所有输出
		for (i = 0; i < MOTORS_MAX_MOTOR_NUM; i++)
		{
			if (motor_enabled[i]) 
			{
				motor_out[i] = out_best_thr_pwm + thr_adj + rpy_scale*rpy_out[i];
				motor_out[i] = constrain_int16(motor_out[i], out_min_pwm, out_max_pwm);
			}
		}
	}


	// send output to each motor
	for (i = 0; i < MOTORS_MAX_MOTOR_NUM; i++) {
		if (motor_enabled[i]) {
			hal.pwm_write(i, motor_out[i]);
		}
	}

}
void Motors::_output_disarmed(void)
{
	output_min();
}
void Motors::output(void)
{
	if (_flags.armed) 
	{
		update_max_throttle();
		_output_armed();
	}
	else 
	{
		_output_disarmed();
	}
}
void Motors::update_max_throttle(void)
{

	if (_flags.slow_start_slow_end)//怠速转动油门
	{
		
		_spin_when_armed_ramped += MOTOR_SLOW_START_LOW_END_INCREMENT;
		if(_spin_when_armed_ramped >= _spin_when_armed)
		{
			_spin_when_armed_ramped = _spin_when_armed;
			_flags.slow_start_slow_end = false;
		}
	}


	if (!_flags.slow_start) {
		return;
	}
	_max_throttle += MOTOR_SLOW_START_LOW_END_INCREMENT;
	if(_max_throttle >= _rc_throttle.servo_out)
	{
		_max_throttle = MOTORS_DEFAULT_MAX_THROTTLE;
		_flags.slow_start = false;
	}
}












