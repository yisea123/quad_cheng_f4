#ifndef _MOTORS_H_
#define _MOTORS_H_
#include "sys.h"

#include "RCOut.h"
#include "Parameter.h"


#define MOTORS_MAX_MOTOR_NUM	8

#define MOTORS_MOT_1	0
#define MOTORS_MOT_2	1
#define MOTORS_MOT_3	2
#define MOTORS_MOT_4	3
#define MOTORS_MOT_5	4
#define MOTORS_MOT_6	5
#define MOTORS_MOT_7	6
#define MOTORS_MOT_8	7

#define MOTORS_YAW_FACTOR_CW   -1	//顺时针，ClockWise
#define MOTORS_YAW_FACTOR_CCW   1	//逆时针,CounterClockWise


#define MOTORS_DEFAULT_MIN_THROTTLE  130
#define MOTORS_DEFAULT_MID_THROTTLE  500
#define MOTORS_DEFAULT_MAX_THROTTLE  1000

#define MOTOR_SLOW_START_INCREMENT           3       // max throttle ramp speed (i.e. motors can reach full throttle in 0.8 seconds)
#define MOTOR_SLOW_START_LOW_END_INCREMENT   1       // min throttle ramp speed (i.e. motors will speed up from zero to _spin_when_armed speed in about 0.3 second)



class Motors
{
public:
	Motors(RCOut& rc_roll, RCOut& rc_pitch, RCOut& rc_throttle, RCOut& rc_yaw) :
		_rc_roll(rc_roll),
		_rc_pitch(rc_pitch),
		_rc_throttle(rc_throttle),
		_rc_yaw(rc_yaw),

		_min_throttle(MOTORS_DEFAULT_MIN_THROTTLE),
		_max_throttle(MOTORS_DEFAULT_MAX_THROTTLE),
		_hover_out(MOTORS_DEFAULT_MID_THROTTLE)
		
	{
		_flags.armed = false;
		_flags.slow_start = false;

		_flags.slow_start_slow_end = true;	//
		_spin_when_armed_ramped = 0;		//计数器
		_spin_when_armed = 50;				//解锁时的转速

		limit.roll_pitch = false;
		limit.yaw = false;
		limit.throttle_lower = false;
		limit.throttle_upper = false;

	}

	bool armed()const {
		return _flags.armed;
	}
	void arm(void);
	void disarm(void);

	void set_throttle_min(int16_t min){
		_min_throttle = min;
	}
	void set_throttle_max(int16_t max) {
		_max_throttle = max;
	}
	void set_throttle_hover(int16_t hover){
		_hover_out = hover;
	}
	void set_mid_throttle(int16_t mt){
		_hover_out = _rc_throttle.radio_min + (float)(_rc_throttle.radio_max - _rc_throttle.radio_min)*mt / 1000;
	}
	int16_t throttle_min(void)const {
		return _min_throttle;
	}
	int16_t throttle_max(void)const {
		return _max_throttle;
	}

	//设置4个通道输出
	void set_roll(int16_t roll_in)		{ _rc_roll.servo_out = roll_in; }
	void set_pitch(int16_t pitch_in)	{ _rc_pitch.servo_out = pitch_in; }
	void set_yaw(int16_t yaw_in)		{ _rc_yaw.servo_out = yaw_in; }
	void set_throttle(int16_t throttle_in)	{ _rc_throttle.servo_out = throttle_in; }
	int16_t get_throttle_out()const			{ return _rc_throttle.servo_out; }

	//输出
	void output_min();
	void output();
	//
	void slow_start(void){ _flags.slow_start = true; }

	void add_motor(int8_t motor_num, float angle_degree, float yaw_factor);
	void add_motor(int8_t motor_num, float roll_factor_in_degrees, float pitch_factor_degrees, float yaw_factor);
	void remove_motor(int8_t motor_num);
	void remove_all_motors();

	struct 
	{
		uint8_t roll_pitch : 1;		// we have reached roll or pitch limit
		uint8_t yaw : 1;			// we have reached yaw limit
		uint8_t throttle_lower : 1; // we have reached throttle's lower limit
		uint8_t throttle_upper : 1; // we have reached throttle's upper limit
	} limit;

	int16_t motor_out[MOTORS_MAX_MOTOR_NUM];
	bool motor_enabled[MOTORS_MAX_MOTOR_NUM];
protected:
	void _output_armed(void);
	void _output_disarmed(void);
	void update_max_throttle(void);

	struct
	{
		uint8_t armed : 1;
		uint8_t frame_orientation : 1;
		uint8_t slow_start : 1;
		uint8_t slow_start_slow_end : 1;
	}_flags;

	RCOut& _rc_roll;
	RCOut& _rc_pitch;
	RCOut& _rc_throttle;
	RCOut& _rc_yaw;


	int16_t _min_throttle;	//0-1000
	int16_t _max_throttle;	//0-1000
	int16_t _hover_out;		//悬停油门？

	int16_t _spin_when_armed_ramped;
	Param_Int16 _spin_when_armed;


	
private:
	void add_motor_raw(int8_t motor_num, float roll_fac, float pitch_fac, float yaw_fac);

	float _roll_factor[MOTORS_MAX_MOTOR_NUM];
	float _pitch_factor[MOTORS_MAX_MOTOR_NUM];
	float _yaw_factor[MOTORS_MAX_MOTOR_NUM];
	
	


};

#endif



