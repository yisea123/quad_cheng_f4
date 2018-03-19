#ifndef _FILTER_H_
#define _FILTER_H_


template<class T>
class Filter
{
public:
	// apply - Add a new raw value to the filter, retrieve the filtered result
	virtual T apply(T sample) = 0;
	// reset - clear the filter
	virtual void reset() = 0;
};


#endif

