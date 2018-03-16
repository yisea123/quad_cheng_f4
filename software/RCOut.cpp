#include "RCOut.h"
#include "QMath.h"





void RCOut::calc_pwm(void)
{
	if (_type == RCOut::TYPE_RANGE) {
		pwm_out = _range_to_pwm();
		radio_out = (_reverse >= 0) ? (radio_min + pwm_out) : (radio_max - pwm_out);
	}
	else if (_type == RCOut::TYPE_ANGLE_RAW) {
		pwm_out = (float)servo_out * 0.1f;
		radio_out = (pwm_out * _reverse) + radio_mid;
	}
	else {     // RC_CHANNEL_TYPE_ANGLE
		pwm_out = _angle_to_pwm();
		radio_out = pwm_out + radio_mid;
	}

	radio_out = constrain_int16(radio_out, radio_min, radio_max);
}

int16_t RCOut::_range_to_pwm()
{
	if (_control_max == _control_min){
		return radio_mid;
	}
	constrain_int16(servo_out, _control_min, _control_max);
	return ((long)(servo_out - _control_min) * (long)(radio_max - radio_min)) / (long)(_control_max - _control_min);

}

int16_t RCOut::_angle_to_pwm()
{
	constrain_int16(servo_out, _control_min, _control_max);
	if ((servo_out * _reverse) > 0)
		return _reverse * ((long)servo_out * (long)(radio_max - radio_mid)) / (long)_control_max;
	else
		return _reverse * ((long)servo_out * (long)(radio_mid - radio_min)) / (long)_control_max;
}
