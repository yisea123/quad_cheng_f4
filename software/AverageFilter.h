#ifndef _AVERAGEFILTER_H_
#define _AVERAGEFILTER_H_
#include "sys.h"



//均值滤波,滑动窗口滤波
template <typename T, uint8_t _size>
class AverageFilter
{
public:
	AverageFilter()
	{
		reset();
	}

	void reset();
	const T& apply(const T& v);
private:
	uint8_t _count;
	T _last_output;
	T _buffer[_size];
};





template <typename T, uint8_t _size>
void AverageFilter<T,_size>::reset()
{
	_last_output = 0;
	_count = 0;
	for (uint8_t i = 0; i < _size; i++)
	{
		_buffer[i] = 0.0f;
	}
}

template <typename T, uint8_t _size>
const T& AverageFilter<T,_size>::apply(const T& v)
{
	_buffer[_count] = v;		//保存数据

	uint8_t oldest = _count + 1;		//最早的一个数据索引
	if (oldest == _size)oldest = 0;

	if (++_count == _size)_count = 0;		//指向下一个

	_last_output = _last_output + (v - _buffer[oldest]) / _size;
	return _last_output;
}




#endif

