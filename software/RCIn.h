#ifndef _RCIN_H_
#define _RCIN_H_
#include "sys.h"
#include "Parameter.h"

class RCIn
{
public:
	RCIn() :
		//control_in(0),
		output(0.0f),
		//参数
		_dead_zone(0),
		_radio_min(1100),
		_radio_max(1900),
		_radio_mid(1500),
		_reverse(1)
	{ }
	
	enum 
	{
		TYPE_RANGE = 0,
		TYPE_ANGLE,
		TYPE_SWITCH,
	};

	void set_dead_zone(int16_t dz){
		_dead_zone = dz;
	}
	void set_reverse(int8_t reverse) {
		_reverse = reverse;
	}
	void operator()(int16_t min, int16_t mid, int16_t max){
		_radio_min = min;
		_radio_max = max;
		_radio_mid = mid;
	}
	void save_config();


	void set_type(uint8_t t){
		_type = t;
	}
	
// 	void set_angle(int16_t angle){
// 		_type = RCIn::TYPE_ANGLE;
// 		_control_max = angle;
// 	}
// 	void set_range(int16_t min,int16_t max){
// 		_type = RCIn::TYPE_RANGE;
// 		_control_min = min;
// 		_control_max = max;
// 	}
	
	void set_radio(int16_t pwm);
	float get_percent(void);	//获取当前输入的比例值

	int16_t radio_in;		//接收输入值
	//int16_t control_in;	//输出值
	float output;

private:
	//int16_t _pwm_to_range();
	//int16_t _pwm_to_angle();
	float _pwm_to_range();
	float _pwm_to_angle();
	int8_t _pwm_to_switches();

	//转换输出参数
	uint8_t _type;

	//接收输入参数
	int16_t _dead_zone;
	Param_Int16 _radio_min;
	Param_Int16 _radio_max;
	Param_Int16 _radio_mid;
	Param_Int8 _reverse;


};


#endif


