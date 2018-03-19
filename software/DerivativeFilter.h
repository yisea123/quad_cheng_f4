#ifndef _DERIVATIVE_FILTER_H_
#define _DERIVATIVE_FILTER_H_

#include "sys.h"
#include "FilterWithBuffer.h"




/// @brief	A class to implement a derivative (slope) filter
//http://www.holoborodko.com/pavel/numerical-methods/numerical-derivative/smooth-low-noise-differentiators/


template <class T,uint8_t FILTER_SIZE>
class DerivativeFilter:public FilterWithBuffer<T,FILTER_SIZE>
{
public:
	DerivativeFilter() :
		FilterWithBuffer<T, FILTER_SIZE>(),
		_new_data(false),
		_last_slope(0){}
		
	// update - Add a new raw value to the filter, but don't recalculate
	void update(T sample, uint32_t timestamp);
	// return the derivative value
	float slope(void);

private:
	bool _new_data;
	float _last_slope;

	uint32_t _timestamps[FILTER_SIZE];

};


typedef DerivativeFilter<float, 5> DerivativeFilterFloat_Size5;
typedef DerivativeFilter<float, 7> DerivativeFilterFloat_Size7;
typedef DerivativeFilter<float, 9> DerivativeFilterFloat_Size9;


#endif

