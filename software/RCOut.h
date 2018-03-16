#ifndef _RCOUT_H_
#define _RCOUT_H_
#include "sys.h"


class RCOut
{
public:
	RCOut():
		pwm_out(0),
		radio_out(0),

		radio_min(1100),
		radio_max(1900),
		radio_mid(1500),

		_control_max(1000),
		_control_min(0),
		_reverse(1)

	{}

	enum
	{
		TYPE_RANGE = 0,
		TYPE_ANGLE,
		TYPE_ANGLE_RAW,
	};


	void operator()(int16_t min, int16_t mid, int16_t max){
		radio_min = min;
		radio_max = max;
		radio_mid = mid;
	}
	void set_reverse(int8_t reverse) {
		_reverse = reverse;
	}

	void set_type(uint8_t t) {
		_type = t;
	}
	
	void set_angle(int16_t angle) {
		_type = RCOut::TYPE_ANGLE;
		_control_max = angle;
	}
	void set_range(int16_t min, int16_t max) {
		_type = RCOut::TYPE_RANGE;
		_control_min = min;
		_control_max = max;
	}
	
	void calc_pwm(void);

	int16_t servo_out;	// ‰»Î
	int16_t pwm_out;	//◊™ªª ‰≥ˆ
	int16_t radio_out;

	int16_t radio_min;	// ‰≥ˆ∑∂Œß
	int16_t radio_max;
	int16_t radio_mid;

private:
	int16_t _range_to_pwm();
	int16_t _angle_to_pwm();

	uint8_t _type;
	int16_t _control_max;	// ‰»Î∑∂Œß
	int16_t _control_min;
	int8_t _reverse;



};



#endif


