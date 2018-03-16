#include "Compass.h"

void Compass::check_calibration(void)
{

}

void Compass::_rotate_and_offset(const Vector3f & mag)
{
	_mag = mag;
	_mag.rotate(_board_orientation);

}
