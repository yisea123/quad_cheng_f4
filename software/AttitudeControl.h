#ifndef _ATTITUDECONTROL_H_
#define _ATTITUDECONTROL_H_
#include "sys.h"
#include "AHRS.h"
#include "Motors.h"
#include "PID.h"
#include "config.h"


#define ATTITUDE_CONTROL_DEGX100 5729.57795f		//弧度转为100度

#define ATTITUDE_RATE_RP_CONTROLLER_OUT_MAX          5000.0f // body-frame rate controller maximum output (for roll-pitch axis)
#define ATTITUDE_RATE_YAW_CONTROLLER_OUT_MAX         4500.0f // body-frame rate controller maximum output (for yaw axis)

#define ATTITUDE_ANGLE_YAW_CONTROLLER_OUT_MAX        4500.0f // earth-frame angle controller maximum output (for yaw axis)
#define ATTITUDE_ANGLE_CONTROLLER_ANGLE_MAX          4500.0f // earth-frame angle controller maximum input angle (To-Do: replace with reference to aparm.angle_max)

#define ATTITUDE_RATE_STAB_ROLL_OVERSHOOT_ANGLE_MAX  3000.0f // earth-frame rate stabilize controller's maximum overshoot angle
#define ATTITUDE_RATE_STAB_PITCH_OVERSHOOT_ANGLE_MAX 3000.0f // earth-frame rate stabilize controller's maximum overshoot angle
#define ATTITUDE_RATE_STAB_YAW_OVERSHOOT_ANGLE_MAX   1000.0f // earth-frame rate stabilize controller's maximum overshoot angle
#define ATTITUDE_RATE_STAB_ACRO_OVERSHOOT_ANGLE_MAX  1000.0f // earth-frame rate stabilize controller's maximum overshoot angle



//控制周期的改变会影响PID的作用	


class AttitudeControl
{
public:
	AttitudeControl(AHRS& ahrs, Motors& motors,
		PID& p_angle_roll, PID& p_angle_pitch, PID& p_angle_yaw,
		PID& pid_rate_roll, PID& pid_rate_pitch, PID& pid_rate_yaw) :
		_ahrs(ahrs),
		_motors(motors),

		_p_angle_roll(p_angle_roll),
		_p_angle_pitch(p_angle_pitch),
		_p_angle_yaw(p_angle_yaw),

		_pid_rate_roll(pid_rate_roll),
		_pid_rate_pitch(pid_rate_pitch),
		_pid_rate_yaw(pid_rate_yaw),

		//
		_angle_rate_rp_max(ANGLE_RATE_MAX),
		_angle_rate_y_max(ANGLE_RATE_MAX),
// 		_accel_rp_max(0),
// 		_accel_y_max(0),
		//
		_angle_boost(0)

	{
		_flags.limit_angle_to_rate_request = false;
		
		
	}
		
	void set_dt(float delta_sec);
	int16_t angle_boost() const { return _angle_boost; }

	void relax_bf_rate_controller();
	void set_yaw_target_to_current_heading(){
		_angle_ef_target.z = _ahrs.yaw_sensor;
	}
	void rate_control_run(float dt);

	//void angle_ef_roll_pitch_rate_ef_yaw_smooth(float roll_angle_ef, float pitch_angle_ef, float yaw_rate_ef, float smoothing_gain);

	void update_ef_yaw_angle_and_error(float yaw_rate_ef, Vector3f &angle_ef_error, float overshoot_max, float dt);
	void frame_conversion_ef_to_bf(const Vector3f& ef_vector, Vector3f& bf_vector);
	void update_rate_bf_targets(void);
	void angle_ef_roll_pitch_rate_ef_yaw_smooth(float roll_angle_ef, float pitch_angle_ef, float yaw_rate_ef, float dt, float smoothing_gain);

	void set_throttle_out(int16_t throttle_out, bool apply_angle_boost);
	int16_t get_angle_boost(int16_t throttle_pwm);
	Vector3f _angle_ef_target;		//期望角度ef
	Vector3f _rate_bf_target;
protected:
	struct AttControlFlags {
		uint8_t limit_angle_to_rate_request : 1;  // 1 if the earth frame angle controller is limiting it's maximum rate request
	} _flags;

	float _rate_bf_to_motor_roll(float rate_roll_target,float dt);
	float _rate_bf_to_motor_pitch(float rate_pitch_target, float dt);
	float _rate_bf_to_motor_yaw(float rate_yaw_target, float dt);


	AHRS& _ahrs;
	Motors& _motors;
	//PID
	PID& _p_angle_roll ;
	PID& _p_angle_pitch;
	PID& _p_angle_yaw;
	PID& _pid_rate_roll;
	PID& _pid_rate_pitch;
	PID& _pid_rate_yaw;

	//parameter
	Param_Float _angle_rate_rp_max;
	Param_Float _angle_rate_y_max;
	Param_Float _accel_rp_max;
	Param_Float _accel_y_max;

	//
	float _dt;
	//Vector3f _angle_ef_target;		//期望角度ef
	Vector3f _angle_bf_error;		//期望角度bf
	//Vector3f _rate_bf_target;		//期望角速率bf

	Vector3f _rate_ef_desired;		//前馈角速率,ef
	Vector3f _rate_bf_desired;		//前馈角速率,bf
	int16_t _angle_boost;

};



#endif



