#ifndef _POS_CONTROL_H_
#define _POS_CONTROL_H_

#include "sys.h"
#include "AHRS.h"
#include "InertialNav.h"
#include "Motors.h"
#include "AttitudeControl.h"
#include "PID.h"
#include "LowPassFilter.h"


// position controller default definitions
#define POSCONTROL_THROTTLE_HOVER               450.0f  // default throttle required to maintain hover
#define POSCONTROL_ACCELERATION_MIN             50.0f   // minimum horizontal acceleration in cm/s/s - used for sanity checking acceleration in leash length calculation
#define POSCONTROL_ACCEL_XY                     100.0f  // default horizontal acceleration in cm/s/s.  This is overwritten by waypoint and loiter controllers
#define POSCONTROL_ACCEL_XY_MAX                 980.0f  // max horizontal acceleration in cm/s/s that the position velocity controller will ask from the lower accel controller
#define POSCONTROL_STOPPING_DIST_Z_MAX          200.0f  // max stopping distance vertically   


#define POSCONTROL_TAKEOFF_JUMP_CM 0.0f  // during take-off altitude target is set to current altitude + this value

#define POSCONTROL_SPEED                        500.0f  // default horizontal speed in cm/s
#define POSCONTROL_SPEED_DOWN                  -150.0f  // default descent rate in cm/s
#define POSCONTROL_SPEED_UP                     250.0f  // default climb rate in cm/s

#define POSCONTROL_VEL_XY_MAX_FROM_POS_ERR      200.0f  // max speed output from pos_to_vel controller when feed forward is used

#define POSCONTROL_ACCEL_Z                      250.0f  // default vertical acceleration in cm/s/s.

#define POSCONTROL_LEASH_LENGTH_MIN             100.0f  // minimum leash lengths in cm


#define POSCONTROL_ACCEL_Z_DTERM_FILTER         20      // Z axis accel controller's D term filter (in hz)

// #define POSCONTROL_VEL_UPDATE_TIME              0.020f  // 50hz update rate on high speed CPUs (Pixhawk, Flymaple)


#define POSCONTROL_VEL_ERROR_CUTOFF_FREQ        4.0     // 4hz low-pass filter on velocity error
#define POSCONTROL_ACCEL_ERROR_CUTOFF_FREQ      2.0     // 2hz low-pass filter on accel error

class PosControl
{
public:
	PosControl(const AHRS& ahrs, const Motors& motors,
		AttitudeControl& attitude_control,
		InertialNav& inav,
		PID& p_alt_pos, PID& p_alt_rate, PID& pid_alt_accel,
		PID& p_pos_xy, PID& pid_rate_x, PID& pid_rate_y);

	void set_dt(float delta_sec);



	//set_alt_target_to_current_alt - set altitude target to current altitude
	void set_alt_target_to_current_alt() { _pos_target.z = _inav.get_altitude(); }
	void set_alt_target_from_climb_rate(float climb_rate_cms, float dt, bool force_descend = false);

	void update_z_controller(float dt);

	void init_takeoff();

	//清除前馈
	void freeze_ff_z() { _flags.freeze_ff_z = true; }
	void freeze_ff_xy() { _flags.freeze_ff_xy = true; }


	void set_alt_max(float alt_cm) { _alt_max = fabs(alt_cm); }
	void set_alt_target(float alt_cm) { _pos_target.z = alt_cm; }
	float get_alt_target() { return _pos_target.z; }

	void set_speed_z(float speed_down, float speed_up);
	void set_accel_z(float accel_cmss);
	void set_target_to_stopping_point_z();
	void get_stopping_point_z(Vector3f & stopping_point) const;



private:


	struct poscontrol_flags{
		uint8_t recalc_leash_z : 1;    // 1 if we should recalculate the z axis leash length
		uint8_t recalc_leash_xy : 1;    // 1 if we should recalculate the xy axis leash length
		uint8_t force_recalc_xy : 1;    // 1 if we want the xy position controller to run at it's next possible time.  set by loiter and wp controllers after they have updated the target position.
		uint8_t slow_cpu : 1;    // 1 if we are running on a slow_cpu machine.  xy position control is broken up into multiple steps
		uint8_t keep_xy_I_terms : 1;    // 1 if we are to keep I terms when the position controller is next run.  Reset automatically back to zero in update_xy_controller.
		uint8_t reset_desired_vel_to_pos : 1;    // 1 if we should reset the rate_to_accel_xy step
		uint8_t reset_rate_to_accel_xy : 1;    // 1 if we should reset the rate_to_accel_xy step
		uint8_t reset_rate_to_accel_z : 1;    // 1 if we should reset the rate_to_accel_z step
		uint8_t reset_accel_to_throttle : 1;    // 1 if we should reset the accel_to_throttle step of the z-axis controller
		uint8_t freeze_ff_xy : 1;    // 1 use to freeze feed forward during step updates
		uint8_t freeze_ff_z : 1;    // 1 use to freeze feed forward during step updates
	}_flags;

	struct poscontrol_limit_flags {
		uint8_t pos_up : 1;    // 1 if we have hit the vertical position leash limit while going up
		uint8_t pos_down : 1;    // 1 if we have hit the vertical position leash limit while going down
		uint8_t vel_up : 1;    // 1 if we have hit the vertical velocity limit going up
		uint8_t vel_down : 1;    // 1 if we have hit the vertical velocity limit going down
		uint8_t accel_xy : 1;    // 1 if we have hit the horizontal accel limit
	}_limit;


	//z轴控制
	void calc_leash_length_z(void);			//更新高度最大最小误差
	float calc_leash_length(float speed_cms, float accel_cms, float kP)const;

	void pos_to_rate_z(float dt);
	void rate_to_accel_z(float dt);
	void accel_to_throttle(float accel_target_z, float dt);




//	void desired_vel_to_pos(float nav_dt);


// 	void pos_to_rate_xy(bool use_desired_rate, float dt);
// 	void rate_to_accel_xy(float dt);
// 	void accel_to_lean_angles();


	const AHRS& _ahrs;
	const Motors& _motors;
	AttitudeControl& _attitude_control;
	InertialNav& _inav;

	//PID
	PID& _p_alt_pos;
	PID& _p_alt_rate;
	PID& _pid_alt_accel;

	PID& _p_pos_xy;
	PID& _pid_rate_x;
	PID& _pid_rate_y;

	

	
	//参数
	Param_Float _throttle_hover;

	//内部变量
	float _dt;
	uint32_t    _last_update_xy_ms;     // system time of last update_xy_controller call
	uint32_t    _last_update_z_ms;      // system time of last update_z_controller call
	uint32_t    _last_update_vel_xyz_ms;// system time of last update_vel_controller_xyz call
	
										//最大值
	float       _speed_down_cms;        // max descent rate in cm/s
	float       _speed_up_cms;          // max climb rate in cm/s
	float       _speed_cms;             // max horizontal speed in cm/s
	float       _accel_z_cms;           // max vertical acceleration in cm/s/s
	float       _accel_cms;             // max horizontal acceleration in cm/s/s

	//水平最大误差
	float       _leash;                 // horizontal leash length in cm.  target will never be further than this distance from the vehicle
	//高度最大误差
	float       _leash_down_z;          // vertical leash down in cm.  target will never be further than this distance below the vehicle
	float       _leash_up_z;            // vertical leash up in cm.  target will never be further than this distance above the vehicle


	//控制器输出
	float _roll_target;
	float _pitch_target;


	Vector3f _pos_target;
	Vector3f _pos_error;
	
	Vector3f _vel_desired;
	Vector3f _vel_target;
	Vector3f _vel_error;
	Vector3f _vel_last;

	Vector3f _accel_target;
	Vector3f _accel_error;
	Vector3f _accel_feedforward;	//前馈

	float _alt_max;
	float _distance_to_target;
	uint8_t _xy_step;

	LowPassFilterFloat _vel_error_filter;   // low-pass-filter on z-axis velocity error
	LowPassFilterFloat _accel_error_filter; // low-pass-filter on z-axis accelerometer error




};


#endif

