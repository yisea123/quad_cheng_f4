#ifndef _FILTERWITHBUFFER_H_
#define _FILTERWITHBUFFER_H_

#include "sys.h"
#include "Filter.h"


template<class T,uint8_t FILTER_SIZE>
class FilterWithBuffer:public Filter<T>
{
public:
	FilterWithBuffer() :
		sample_index(0)
	{
		reset();
	}

	// apply - take in a new raw sample, and return the filtered results
	virtual T apply(T sample);
	// reset - clear all samples from the buffer
	virtual void reset();

	uint8_t get_filter_size()const{
		return FILTER_SIZE;
	}
	T get_sample(uint8_t i)const{
		return samples[i];
	}
protected:
	T samples[FILTER_SIZE];
	uint8_t sample_index;
};



// apply - take in a new raw sample, and return the filtered results
template<class T, uint8_t FILTER_SIZE>
T FilterWithBuffer<T, FILTER_SIZE>::apply(T sample)
{
	samples[sample_index++] = sample;
	if (sample_index >= FILTER_SIZE) {
		sample_index = 0;
	}
	return sample;
}

// reset - clear all samples from the buffer
template<class T, uint8_t FILTER_SIZE>
void FilterWithBuffer<T, FILTER_SIZE>::reset()
{
	for (uint8_t i = 0; i < FILTER_SIZE; i++) {
		samples[i] = 0;
	}
	sample_index = 0;
}


#endif

