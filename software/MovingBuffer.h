#ifndef _MOVINGBUFFER_H_
#define _MOVINGBUFFER_H_
#include "sys.h"





template <typename T, uint16_t size>
class MovingBuffer
{
public:
	MovingBuffer()
	{
		_count = 0;
	}
	


	void push(const T& v) {
		_buffer[_count] = v;
		if (++_count == size)_count = 0;
	}
	//const T& pop(void)const { return _last_output; }

	const T& average()const;
	const T& standard_deviation()const ;		//±ê×¼²î
	const T& variance()const;					//·½²î
	uint16_t count(void)const{
		return _count;
	}

private:
	uint16_t _count;
	T _buffer[size];
};

#endif



template<typename T, uint16_t size>
const T & MovingBuffer<T, size>::average() const
{
	float sum = 0.0f;
	for (uint16_t i = 0; i < _count; i++)
	{
		sum += _buffer[i];
	}
	return sum / _count;

}

template<typename T, uint16_t size>
const T & MovingBuffer<T, size>::variance() const
{
	float avr = average();
	float sum = 0.0f;
	for (uint16_t i = 0; i < _count; i++)
	{
		sum += square(abs(avr - _buffer[i]));
	}
	return sum / _count;

}

template<typename T, uint16_t size>
const T & MovingBuffer<T, size>::standard_deviation()const
{
	float avr = average();
	float sum = 0.0f;
	for (uint16_t i = 0; i < _count; i++)
	{
		sum += abs(avr - _buffer[i]);
	}
	return sum / _count;
}
