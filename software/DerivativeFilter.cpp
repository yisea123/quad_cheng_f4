#include "DerivativeFilter.h"

#include "math.h"

template<class T, uint8_t FILTER_SIZE>
void DerivativeFilter<T, FILTER_SIZE>::update(T sample, uint32_t timestamp)
{
	uint8_t i = FilterWithBuffer<T, FILTER_SIZE>::sample_index;
	uint8_t i1 = i == 0 ? FILTER_SIZE - 1 : i - 1;	//上一个数据

	if (_timestamps[i1] == timestamp){	
		//时间戳与上一个相同,丢弃
		return;
	}

	_timestamps[i] = timestamp;
	FilterWithBuffer<T, FILTER_SIZE>::apply(sample);

	_new_data = true;

}



template<class T, uint8_t FILTER_SIZE>
float DerivativeFilter<T, FILTER_SIZE>::slope(void)
{
	if (!_new_data)//没有新数据
	{
		return _last_slope;
	}
	float result = 0;


#define f(i) FilterWithBuffer<T, FILTER_SIZE>::samples[(((FilterWithBuffer<T, FILTER_SIZE>::sample_index-1)+i+1)+3*FILTER_SIZE/2) % FILTER_SIZE]
#define x(i) _timestamps[(((FilterWithBuffer<T, FILTER_SIZE>::sample_index-1)+i+1)+3*FILTER_SIZE/2) % FILTER_SIZE]

	if (_timestamps[FILTER_SIZE - 1] == _timestamps[FILTER_SIZE - 2])
	{
		//缓冲未满,返回0
		return 0;
	}


	// N in the paper is FILTER_SIZE
	//5,7,9,11
	switch (FILTER_SIZE) {
	case 5:
		result = 2 * 2 * (f(1) - f(-1)) / (x(1) - x(-1))
			+ 4 * 1 * (f(2) - f(-2)) / (x(2) - x(-2));
		result /= 8;
		break;
	case 7:
		result = 2 * 5 * (f(1) - f(-1)) / (x(1) - x(-1))
			+ 4 * 4 * (f(2) - f(-2)) / (x(2) - x(-2))
			+ 6 * 1 * (f(3) - f(-3)) / (x(3) - x(-3));
		result /= 32;
		break;
	case 9:
		result = 2 * 14 * (f(1) - f(-1)) / (x(1) - x(-1))
			+ 4 * 14 * (f(2) - f(-2)) / (x(2) - x(-2))
			+ 6 * 6 * (f(3) - f(-3)) / (x(3) - x(-3))
			+ 8 * 1 * (f(4) - f(-4)) / (x(4) - x(-4));
		result /= 128;
		break;
	case 11:
		result = 2 * 42 * (f(1) - f(-1)) / (x(1) - x(-1))
			+ 4 * 48 * (f(2) - f(-2)) / (x(2) - x(-2))
			+ 6 * 27 * (f(3) - f(-3)) / (x(3) - x(-3))
			+ 8 * 8 * (f(4) - f(-4)) / (x(4) - x(-4))
			+ 10 * 1 * (f(5) - f(-5)) / (x(5) - x(-5));
		result /= 512;
		break;
	default:
		result = 0;
		break;
	}

	if (isnan(result) || isinf(result)) {
		result = 0;
	}

	_new_data = false;
	_last_slope = result;
	return result;
}


// add new instances as needed here
template void DerivativeFilter<float, 5>::update(float sample, uint32_t timestamp);
template float DerivativeFilter<float, 5>::slope(void);

template void DerivativeFilter<float, 7>::update(float sample, uint32_t timestamp);
template float DerivativeFilter<float, 7>::slope(void);

template void DerivativeFilter<float, 9>::update(float sample, uint32_t timestamp);
template float DerivativeFilter<float, 9>::slope(void);

template void DerivativeFilter<float, 11>::update(float sample, uint32_t timestamp);
template float DerivativeFilter<float, 11>::slope(void);



