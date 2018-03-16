#ifndef _LOWPASSFILTER2P_H_
#define _LOWPASSFILTER2P_H_
#include "sys.h"
#include "QMath.h"


#include "vector3.h"
class LowPassFilter2p
{
public:
	LowPassFilter2p(float sample_freq,float cutoff_freq){
		set_cutoff_frequency(sample_freq, cutoff_freq);
	}

	LowPassFilter2p(float wc) {
		set_cutoff_frequency(wc);
	}


	void set_cutoff_frequency(float sample_freq, float cutoff_freq) {
		set_cutoff_frequency(2.0f * cutoff_freq / sample_freq);
	}

	//wc = 0~1
	void set_cutoff_frequency(float wc);
	float apply(float sample);

	
private:
	float _wc;

	float           _a1;
	float           _a2;
	float           _b0;
	float           _b1;
	float           _b2;
	float           _delay_element_1;        // buffered sample -1
	float          _delay_element_2;        // buffered sample -2
};


#endif



