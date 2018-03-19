#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "sys.h"


template<class T,uint8_t SIZE>
class Buffer
{
public:
	Buffer();

	void clear();
	void push_back(const T &item);
	T pop_front();
	const T& peek(uint8_t pos)const;
	const T& front() const { return this->peek(0); }
	uint8_t size()const { return SIZE; }
	bool is_full()const { return _num_items >= SIZE; }
	bool is_empty()const { return _num_items == 0; }


private:
	uint8_t _num_items;
	uint8_t _head;
	T _buff[SIZE];
};

// Typedef for convenience - add more as needed
typedef Buffer<float, 5> BufferFloat_Size5;
typedef Buffer<float, 15> BufferFloat_Size15;

template <class T, uint8_t SIZE>
Buffer<T, SIZE>::Buffer() :
	_num_items(0), _head(0)
{
}

template<class T, uint8_t SIZE>
void Buffer<T, SIZE>::clear()
{
	_num_items = 0;
	_head = 0;
}

template<class T, uint8_t SIZE>
void Buffer<T, SIZE>::push_back(const T & item)
{
	uint8_t tail = _head + _num_items;
	if (tail >= SIZE)tail -= SIZE;
	_buff[tail] = item;

	if (_num_items < SIZE) _num_items++;
	else{
		_head++;
		if (_head >= SIZE) _head = 0;
	}

}

template<class T, uint8_t SIZE>
T Buffer<T, SIZE>::pop_front()
{
	T result;

	// return zero if buffer is empty
	if (_num_items == 0) {
		return 0;
	}

	// get next value in buffer
	result = _buff[_head];

	// increment to next point
	_head++;
	if (_head >= SIZE)
		_head = 0;

	// reduce number of items
	_num_items--;

	// return item
	return result;
}

template <class T, uint8_t SIZE>
const T& Buffer<T, SIZE>::peek(uint8_t position) const
{
	uint8_t j = _head + position;

	// return zero if position is out of range
	if (position >= _num_items) {
		const static T r = 0;
		return r;
	}

	// wrap around if necessary
	if (j >= SIZE)
		j -= SIZE;

	// return desired value
	return _buff[j];
}

#endif
