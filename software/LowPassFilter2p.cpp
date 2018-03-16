#include "LowPassFilter2p.h"

void LowPassFilter2p::set_cutoff_frequency(float wc)
{
	_wc = wc;
	float ohm = tanf(2 * PI*_wc);
	float ohm2 = ohm*ohm;
	float theta = PI / 4.0f;

	float c = 1.0f + 2.0f*cosf(theta)*ohm + ohm2;
	_b0 = ohm2 / c;
	_b1 = 2.0f*_b0;
	_b2 = _b0;
	_a1 = 2.0f*(ohm2 - 1.0f) / c;
	_a2 = (1.0f - 2.0f*cosf(theta)*ohm + ohm2) / c;

	_delay_element_1 = _delay_element_2 = 0.0f;
}



float LowPassFilter2p::apply(float sample)
{    // do the filtering
	float delay_element_0 = sample - _delay_element_1 * _a1 - _delay_element_2 * _a2;
	if (isnan(delay_element_0) || isinf(delay_element_0)) {
		// don't allow bad values to propogate via the filter
		delay_element_0 = sample;	//mean: _delay_element_1=_delay_element_2=0?
	}

	float output = delay_element_0 * _b0 + _delay_element_1 * _b1 + _delay_element_2 * _b2;

	_delay_element_2 = _delay_element_1;
	_delay_element_1 = delay_element_0;

	// return the value.  Should be no need to check limits
	return output;
}
