#ifndef _AHRS_Q_H_
#define _AHRS_Q_H_
#include "sys.h"
#include "Quaternion.h"
#include "AHRS.h"

class AHRS_Q:public AHRS
{
public:
	AHRS_Q(InertialSensor &ins,Compass& compass):
		AHRS(ins,compass),
		_kp(0.2f),
		_kp_yaw(0.2f),
		_ki(0.0087f),
		_ki_yaw(0.01f)
	{
		reset(false);
	}

	virtual void update(float dt);




	void reset_gyro_drift(){
		_omega_I.zero();
	}




private:

	void drift_correction(float dt);
	void drift_correction_yaw(float dt);
	float _P_gain(float spin_rate) const;
	void euler_angles(void);
	void reset(bool recover_euler);
	Quaternion _q;


	Vector3f _omega_P;
	Vector3f _omega_yaw_P;

	Vector3f _omega_I;
	Vector3f _omega_I_sum;
	float _omega_I_sum_time;

	Matrix3f _dcm_matrix;

	float _kp;
	float _kp_yaw;
	float _ki;
	float _ki_yaw;



};



#endif


