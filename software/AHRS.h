#ifndef _AHRS_H_
#define _AHRS_H_
#include "sys.h"
#include "InertialSensor.h"
#include "Compass.h"
#include "QMath.h"
#include "Parameter.h"


#define AHRS_TRIM_LIMIT 10.0f        // maximum trim angle in degrees
#define AHRS_RP_P_MIN   0.05f        // minimum value for AHRS_RP_P parameter
#define AHRS_YAW_P_MIN  0.05f        // minimum value for AHRS_YAW_P parameter
class AHRS
{
public:
	AHRS(InertialSensor &ins,Compass &compass):
		roll(0.0f),
		pitch(0.0f),
		yaw(0.0f),

		roll_sensor(0.0f),
		pitch_sensor(0.0f),
		yaw_sensor(0.0f),

		_ins(ins),				//传感器
		_compass(compass),

		_cos_roll(1.0f),		//三角函数
		_cos_pitch(1.0f),
		_cos_yaw(1.0f),
		_sin_roll(0.0f),
		_sin_pitch(0.0f),
		_sin_yaw(0.0f),

		//参数
		_trim(0, 0, 0),
		_board_orientation(ROTATION_NONE)

	{
		_gyro_drift_limit = _ins.get_gyro_drift_rate();
		_flags.armed = false;
		
	}

	virtual ~AHRS() {}

	virtual void update(float dt) = 0;

	void init(void);
	void set_fast_gains(bool setting) {
		_flags.fast_ground_gains = setting;
	}
	void set_armed(bool setting) {				_flags.armed = setting;	 	}	
 	bool get_armed(void)const {				return _flags.armed;	 	}

	const Matrix3f &get_dcm_matrix(void)const { return _body_dcm_matrix; }

	const Vector3f& get_gyro(void)const { return _omega; }
	const Vector3f& get_accel_bf(void)const { return _ins.get_accel(); }
	const Vector3f& get_accel_ef(void)const { return _accel_ef; }

	float cos_roll()const { return _cos_roll; }
	float cos_pitch()const { return _cos_pitch; }
	float cos_yaw()const { return _cos_yaw; }
	float sin_roll()const { return _sin_roll; }
	float sin_pitch()const { return _sin_pitch; }
	float sin_yaw()const { return _sin_yaw; }

	void update_cd_values(void);	//将弧度转为100度
	void update_trig(void);			//计算三角函数
	void set_board_orientation();	

	float roll,pitch,yaw;							// Euler angles (radians)
	float roll_sensor,pitch_sensor, yaw_sensor; // integer Euler angles (Degrees * 100)
protected:
	InertialSensor &_ins;
	Compass &_compass;

	struct ahrs_flags
	{
		uint8_t armed					: 1;
		uint8_t fast_ground_gains		: 1;
	}_flags;

	float _cos_roll, _cos_pitch, _cos_yaw;
	float _sin_roll, _sin_pitch, _sin_yaw;

	Vector3f _omega;			//gyro+omega_I
	Vector3f _accel_ef;			//世界坐标系加速度
	Matrix3f _body_dcm_matrix;	//
		
	float _gyro_drift_limit;	//积分漂移限制


	Param_Vector3f _trim;
	Param_Int8 _board_orientation;


};


#endif




