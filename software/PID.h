#ifndef _PID_H_
#define _PID_H_
#include "sys.h"
#include "Parameter.h"
#include "QMath.h"



class PID
{
public:
	PID(float initial_p = 0.0f,
		float initial_i = 0.0f,
		float initial_d = 0.0f,
		int16_t initial_imax = 0):
		_d_lpf_alpha()
	{
		_kp = initial_p;
		_ki = initial_i;
		_kd = initial_d;
		_imax = abs(initial_imax);

		set_d_lpf_alpha(20, 2.5e-3);
		reset();
	}


	float get_p(float error)const { return _kp.get()*error; }
	float get_i(float error, float dt);
	float get_d(float input, float dt);
	float get_pi(float error, float dt);
	float get_pid(float error, float dt);

	void reset(){
		_integrator = 0;
		_last_derivative = NAN;
		_last_input = NAN;
	}

	
	void set_d_lpf_alpha(int16_t cutoff_freq, float time_step);

	//设置参数
	void operator()(float p,float i,float d,int16_t imax)
	{
		_kp = p; _ki = i; _kd = d; _imax = abs(imax);
	}
	void kP(float v) { _kp.set(v); }
	void kI(float v) { _ki.set(v); }
	void kD(float v) { _kd.set(v); }
	void imax(int16_t v) { _imax.set(abs(v)); }
	//获取参数
	float kP()const { return _kp.get(); }
	float kI()const { return _ki.get(); }
	float kD()const { return _kd.get(); }
	int16_t imax()const { return _imax.get(); }
	//保存参数
	void save()
	{
		_kp.save(); _ki.save(); _kd.save(); _imax.save();
	}
	
	//
	float get_integrator()const { return _integrator; }
	void set_integrator(float i) { _integrator = i; }

protected:
	Param_Float _kp;
	Param_Float _ki;
	Param_Float _kd;
	Param_Int16 _imax;

	float _integrator;
	float _last_input;
	float _last_derivative;
	float _d_lpf_alpha;

};








#endif


