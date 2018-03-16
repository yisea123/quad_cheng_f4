#include "RCIn.h"
#include "QMath.h"

int16_t RCIn::_pwm_to_range()
{
	int16_t r_in = constrain_int16(radio_in, _radio_min, _radio_max);	//遥控器输入限幅
	
	if (_reverse == -1){
		r_in = _radio_max - (r_in - _radio_min);
	}

	int16_t radio_trim_low = _radio_min + _dead_zone;
	if (r_in > radio_trim_low)
		return (_control_min + ((long)(_control_max - _control_min) * (long)(r_in - radio_trim_low)) / (long)(_radio_max - radio_trim_low));
	else if (_dead_zone > 0)
		return 0;
	else//dead_zone = 0,no dead_zone
		return _control_min;
}

int16_t RCIn::_pwm_to_angle()
{
	int16_t radio_trim_high = _radio_mid + _dead_zone;		//
	int16_t radio_trim_low = _radio_mid - _dead_zone;		//
	int16_t r_in;

	// prevent div by 0
	if ((radio_trim_low - _radio_min) == 0 || (_radio_max - radio_trim_high) == 0)
		return 0;

	r_in = constrain_int16(radio_in, _radio_min, _radio_max);

	if (r_in > radio_trim_high) //大于中值
	{
		return _reverse * ((long)_control_max * (long)(r_in - radio_trim_high)) / (long)(_radio_max - radio_trim_high);
	}
	else if (r_in < radio_trim_low) //小于中值
	{
		return _reverse * ((long)_control_max * (long)(r_in - radio_trim_low)) / (long)(radio_trim_low - _radio_min);
	}
	else
		return 0;
}

void RCIn::save_config()
{
	_radio_min.save();
	_radio_max.save();
	_radio_mid.save();
	_reverse.save();
}


void RCIn::set_radio(int16_t pwm)
{
// 	radio_in = pwm;
// 	if (_type == RCIn::TYPE_RANGE)
// 	{
// 		control_in = _pwm_to_range();
// 	}
// 	else //TYPE_ANGLE
// 	{
// 		control_in =  _pwm_to_angle();
// 	}

}

float RCIn::get_percent(void)
{
	static int16_t last_in = 0;
	static float last_scale = 0;

	radio_in = constrain_int16(radio_in, _radio_min, _radio_max);
	if (_reverse == -1) {
		radio_in = _radio_max - (radio_in - _radio_min);
	}

	if (radio_in == last_in)return last_scale;


	last_scale = (float)(radio_in -_radio_min)/(_radio_max - _radio_min);
	last_in = radio_in;

	return last_scale;
}
