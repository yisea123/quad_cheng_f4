#include "InertialSensor.h"
#include "state.h"





void InertialSensor::check_calibration(void)
{
	_have_calibration = false;

	if (_accel_offset.is_zero())return;

// 	if (fabs(_accel_scale.x - 1.0f) < 0.00001f &&
// 		fabs(_accel_scale.y - 1.0f) < 0.00001f &&
// 		fabs(_accel_scale.z - 1.0f) < 0.00001f)
// 	{
// 		return;
// 	}

	if (_gyro_offset.is_zero())return;

// 	if (fabs(_gyro_scale.x - 1.0f) < 0.00001f &&
// 		fabs(_gyro_scale.y - 1.0f) < 0.00001f &&
// 		fabs(_gyro_scale.z - 1.0f) < 0.00001f)
// 	{
// 		return;
// 	}

	_have_calibration = true;
}
bool InertialSensor::calibration_accel(void)
{
	static Vector3f accel_sum;
	static int16_t accel_sum_count = -1;

	if(accel_sum_count == -1)
	{
		//清除,下次得到原始数据
		_accel_scale.one();
		_accel_offset.zero();
		accel_sum.zero();

		accel_sum_count = 0;
		
	}
	else
	{
		accel_sum += get_accel();
		accel_sum.z += GRAVITY_MSS;
			
		if(++ accel_sum_count == 100)
		{
			_accel_offset = accel_sum / accel_sum_count;
			_accel_offset.save();

			accel_sum_count = -1;

			//printf("accel_ofset:x=%f,y=%f,z=%f\r\n", _accel_offset.x, _accel_offset.y, _accel_offset.z);
			check_calibration();
			return true;

		}

	}

	return false;

}
bool InertialSensor::calibration_gyro(void)
{
	static Vector3f gyro_sum;
	static int16_t gyro_sum_count = -1;
	if (gyro_sum_count == -1)
	{
		//清除,下次得到原始数据
		_gyro_scale.one();	//清除,下次得到原始数据
		_gyro_offset.zero();
		gyro_sum.zero();

		gyro_sum_count = 0;
		
		
	}
	else
	{
		gyro_sum += get_gyro();
		if (++gyro_sum_count == 100)
		{
			_gyro_offset = gyro_sum / gyro_sum_count;
			_gyro_offset.save();			//保存数据(标志)

			gyro_sum_count = -1;
			//printf("gyro_offset:x=%f,y=%f,z=%f\r\n", _gyro_offset.x, _gyro_offset.y, _gyro_offset.z);
			check_calibration();
			return true;
		}

	}
	return false;
}

void InertialSensor::_rotate_and_offset_gyro(const Vector3f & gyro)
{
	_gyro = gyro;
	_gyro.rotate(_board_orientation);
	_gyro -= _gyro_offset;
	
	//scale
	_gyro.x *= _gyro_scale.x;
	_gyro.y *= _gyro_scale.y;
	_gyro.z *= _gyro_scale.z;

}
void InertialSensor::_rotate_and_offset_accel(const Vector3f & accel)
{
	_accel = accel;
 	_accel.rotate(_board_orientation);
 	_accel -= _accel_offset;
	//scale
	_accel.x *= _accel_scale.x;	
	_accel.y *= _accel_scale.y;
	_accel.z *= _accel_scale.z;
}
