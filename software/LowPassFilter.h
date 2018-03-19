#ifndef _LOW_PASS_FILTER_H_
#define _LOW_PASS_FILTER_H_

#include "Filter.h"


template <class T>
class LowPassFilter:public Filter<T>
{
public:
	LowPassFilter();
	virtual ~LowPassFilter(){}

	void set_cutoff_frequency(float time_step, float cutoff_freq);
	void set_time_constant(float time_step, float time_constant);



	virtual T apply(T sample);

	virtual void reset()
	{
		_base_value_set = false;
	}

	virtual void reset(T new_base_value){
		_base_value = new_base_value;
		_base_value_set = true;
	}

private:
	float _alpha;
	bool _base_value_set;
	float _base_value;
};

//////////////////////////////////////////////////////////////////////////

typedef LowPassFilter<int8_t> LowPassFilterInt8;
typedef LowPassFilter<uint8_t> LowPassFilterUInt8;

typedef LowPassFilter<int16_t> LowPassFilterInt16;
typedef LowPassFilter<uint16_t> LowPassFilterUInt16;

typedef LowPassFilter<int32_t> LowPassFilterInt32;
typedef LowPassFilter<uint32_t> LowPassFilterUInt32;

typedef LowPassFilter<float> LowPassFilterFloat;

//////////////////////////////////////////////////////////////////////////

template<class T>
LowPassFilter<T>::LowPassFilter() :
	Filter<T>(),
	_alpha(1),
	_base_value_set(false)
{ }



#endif

template<class T>
void LowPassFilter<T>::set_cutoff_frequency(float time_step, float cutoff_freq)
{
	// avoid divide by zero and allow removing filtering
	if (cutoff_freq <= 0.0f) {
		_alpha = 1.0f;
		return;
	}

	// calculate alpha
	float rc = 1 / (2 * PI*cutoff_freq);
	_alpha = time_step / (time_step + rc);
}

template<class T>
void LowPassFilter<T>::set_time_constant(float time_step, float time_constant)
{
	// avoid divide by zero
	if (time_constant + time_step <= 0.0f) {
		_alpha = 1.0f;
		return;
	}

	// calculate alpha
	_alpha = time_step / (time_constant + time_step);
}

template<class T>
T LowPassFilter<T>::apply(T sample)
{
	// initailise _base_value if required
	if (!_base_value_set)
	{
		_base_value = sample;
		_base_value_set = true;
	}

	// do the filtering
	//_base_value = _alpha * (float)sample + (1.0 - _alpha) * _base_value;
	_base_value = _base_value + _alpha * ((float)sample - _base_value);

	// return the value.  Should be no need to check limits
	return (T)_base_value;
}
