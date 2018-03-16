#ifndef _INERTIAL_SENSOR_H_
#define _INERTIAL_SENSOR_H_

#include "QMath.h"
#include "ParamVector3f.h"

class InertialSensor
{
public:
	InertialSensor() :
		_healthy(false),

		_accel_scale(1.0f, 1.0f, 1.0f),
		_accel_offset(0.0f, 0.0f, 0.0f),

		_gyro_scale(1.0f, 1.0f, 1.0f),
		_gyro_offset(0.0f, 0.0f, 0.0f),	

		_have_calibration(false)

	{ }

	virtual ~InertialSensor() { };

	virtual bool update() = 0;
	virtual bool accel_available() = 0;
	virtual bool gyro_available() = 0;

	void set_accel_available(bool set) { _accel_available = set; }
	void set_gyro_available(bool set) { _gyro_available = set; }


	virtual float get_gyro_drift_rate(void) const { return radians(0.5f / 60); }	//漂移速率,0.5度/60s ,每秒0.0083
	
	const Vector3f& get_accel()const {
		return _accel;
	}
	const Vector3f& get_gyro()const{
		return _gyro;
	}

	
	void set_board_orientation(Rotation orientation){
		_board_orientation = orientation;
	}
	
	void check_calibration(void);
	bool calibration_accel(void);
	bool calibration_gyro(void);

	bool calibrated(){
		return _have_calibration;
	}
	bool healthy(){
		return _healthy ;
	}

protected:
	void _rotate_and_offset_gyro(const Vector3f &gyro);
	void _rotate_and_offset_accel(const Vector3f &accel);

	bool _healthy;

	bool _accel_available;
	bool _gyro_available;

	Param_Vector3f _accel_scale;
	Param_Vector3f _accel_offset;
	Param_Vector3f _gyro_scale;
	Param_Vector3f _gyro_offset;
private:
	Rotation _board_orientation;

	Vector3f _accel;
	Vector3f _gyro;
	bool _have_calibration;		//传感器校准
};




#endif

