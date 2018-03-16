#include "AHRS_Q.h"


#define SPIN_RATE_LIMIT 20



void AHRS_Q::update(float dt)
{
	_ins.update();	//更新加速度数据

	//四元数更新
	_omega = _ins.get_gyro();
	_omega += _omega_I;									//积分项
	_q.roate((_omega + _omega_P + _omega_yaw_P)*dt);	//加入比例项

	_q.normalize();									//规范化
	_dcm_matrix = _q.to_dcm();						//获取旋转矩阵
	_accel_ef = _dcm_matrix*_ins.get_accel();		//获取世界坐标系加速度

	drift_correction(dt);				//更新矫正 _omega_P,_omega_I
	euler_angles();						//转为欧拉角
	update_trig();						//求欧拉角三角函数
		
}

float AHRS_Q::_P_gain(float spin_rate) const
{
	if (spin_rate < radians(50)) {
		return 1.0f;
	}
	if (spin_rate > radians(500)) {	//限幅
		return 10.0f;
	}

	return spin_rate / radians(50);//对50度归一化
}
void AHRS_Q::drift_correction_yaw(float dt)
{

}
void AHRS_Q::drift_correction(float dt)
{
	drift_correction_yaw(dt);

	Vector3f GA_e(0, 0, -1);					//重力加速度 

	Vector3f GA_b = _accel_ef / GRAVITY_MSS;	//转1g单位
	GA_b.normalize();							

	//误差
	Vector3f error = GA_b%GA_e;						//叉乘误差
	error = _dcm_matrix.mul_transpose(error);		//误差转到机体

	float spin_rate = _omega.length();				//旋转速率大小

	//比例项
	_omega_P = error*_kp*_P_gain(spin_rate);		//1-10,(50-500度)
	if(_flags.fast_ground_gains){					//地面快速修正
		_omega_P *= 8;
	}

	//积分项
	if (spin_rate < radians(SPIN_RATE_LIMIT)) {		//小于20度每秒
		_omega_I_sum += error*_ki*dt;
		_omega_I_sum_time += dt;
	}

	if (_omega_I_sum_time >= 2)	//积分值2s修正一次
	{
		float change_limit = _gyro_drift_limit * _omega_I_sum_time;		//0.5度/60s
		_omega_I_sum.x = constrain_float(_omega_I_sum.x, -change_limit, change_limit);
		_omega_I_sum.y = constrain_float(_omega_I_sum.y, -change_limit, change_limit);
		_omega_I_sum.z = constrain_float(_omega_I_sum.z, -change_limit, change_limit);

		_omega_I += _omega_I_sum;

		_omega_I_sum.zero();
		_omega_I_sum_time = 0;
	}


}



void AHRS_Q::reset(bool recover_euler)
{
	reset_gyro_drift();
	_omega_P.zero();
	_omega_yaw_P.zero();
	_omega.zero();

	if(recover_euler && !isnan(roll) && isnan(pitch) && isnan(yaw))
	{
		_q.from_euler(roll, pitch, yaw);
		_dcm_matrix = _q.to_dcm();
	}
	else
	{
		_q.from_euler(0, 0, 0);
		_dcm_matrix.identity();
	}
	
}
void AHRS_Q::euler_angles(void)
{
	_body_dcm_matrix = _dcm_matrix;
	_body_dcm_matrix.rotateXYinv(_trim);
	_body_dcm_matrix.to_euler(&roll, &pitch, &yaw);
	update_cd_values();
}


