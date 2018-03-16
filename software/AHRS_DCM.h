#ifndef _AHRS_DCM_H_
#define _AHRS_DCM_H_
#include "sys.h"
#include "QMath.h"
#include "AHRS.h"

class AHRS_DCM:public AHRS
{
public:
	AHRS_DCM(InertialSensor &ins,Compass &compass) :
		AHRS(ins,compass),
		_omega_I_sum_time(0.0f),

		_renorm_val_sum(0.0f),
		_renorm_val_count(0),

		_ra_deltat(0.0f),
		_ra_sum_start(0),

		_error_rp_sum(0.0f),
		_error_rp_count(0),
		_error_yaw_sum(0.0f),
		_error_yaw_count(0),
		_error_yaw_last(0.0f),

		_last_failure_ms(0),

		_kp(0.2f),
		_kp_yaw(0.2f),
		_ki(0.0087f),
		_ki_yaw(0.01f)
	{
		_dcm_matrix.identity();	//单位化,欧拉角全设置为0

	}

	virtual void update(float dt);

	void reset_gyro_drift(void);	//清除陀螺仪积分偏移


private:
	void matrix_update(float dt);
	void normalize(void);
	void drift_correction_yaw();
	void drift_correction(float deltat);
	void reset(bool recover_eulers);	//从欧拉角值恢复?
	bool renorm(Vector3f const &a, Vector3f &result);
	void reset_attitude(float roll, float pitch, float yaw);
	void check_matrix(void);
	void euler_angles(void);
	float _P_gain(float spin_rate)const;
	float _yaw_gain(void)const;



	Vector3f _omega_P;		
	Vector3f _omega_yaw_P;	//只有z分量

	Vector3f _omega_I;		
	Vector3f _omega_I_sum;
	float _omega_I_sum_time;

	

	float _renorm_val_sum;			//for debug
	uint16_t _renorm_val_count;

	Vector3f _ra_sum;	//accel_ef 均值滤波
	float _ra_deltat;
	uint32_t _ra_sum_start;

	float _error_rp_sum;
	uint16_t _error_rp_count;

	float _error_yaw_sum;
	uint16_t _error_yaw_count;
	float _error_yaw_last;

	Matrix3f _dcm_matrix;
	
	uint32_t _last_failure_ms;
	float _kp;
	float _kp_yaw;
	float _ki;
	float _ki_yaw;
};



#endif

