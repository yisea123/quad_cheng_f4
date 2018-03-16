#include "AHRS.h"
#include "QMath.h"
#include "Parameters.h"



void AHRS::init(void)
{
	set_board_orientation();

}
void AHRS::set_board_orientation()
{
	_ins.set_board_orientation((enum Rotation)_board_orientation.get());

	if(g.use_compass)
	{
		_compass.set_board_orientation((enum Rotation)_board_orientation.get());
	}
}
void AHRS::update_cd_values(void)
{
	roll_sensor = degrees(roll) * 100;
	pitch_sensor = degrees(pitch) * 100;
	yaw_sensor = degrees(yaw) * 100;
// 	if (yaw_sensor < 0)
// 		yaw_sensor += 36000;
//	wrap_180_cd(yaw_sensor);
	if (yaw_sensor < -18000)yaw_sensor += 36000;
	else if (yaw_sensor > 18000)yaw_sensor -= 36000;

}
void AHRS::update_trig(void)
{
	Vector2f yaw_vector;
	const Matrix3f &temp = get_dcm_matrix();
	// sin_yaw, cos_yaw
	yaw_vector.x = temp.a.x;
	yaw_vector.y = temp.b.x;
	yaw_vector.normalize();
	_sin_yaw = constrain_float(yaw_vector.y, -1.0, 1.0);
	_cos_yaw = constrain_float(yaw_vector.x, -1.0, 1.0);

	// cos_roll, cos_pitch
	_cos_pitch = safe_sqrt(1 - (temp.c.x * temp.c.x));
	_cos_roll = temp.c.z / _cos_pitch;
	_cos_pitch = constrain_float(_cos_pitch, 0, 1.0);
	_cos_roll = constrain_float(_cos_roll, -1.0, 1.0); // this relies on constrain_float() of infinity doing the right thing,which it does do in avr-libc

	// sin_roll, sin_pitch
	_sin_pitch = -temp.c.x;
	_sin_roll = temp.c.y / _cos_pitch;
}


