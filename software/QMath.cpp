#include "QMath.h"
#include "math.h"


// degrees -> radians
float radians(float deg) {
	return deg * DEG_TO_RAD;
}
// radians -> degrees
float degrees(float rad) {
	return rad * RAD_TO_DEG;
}
float safe_asin(float v)
{
    if (isnan(v)) {
        return 0.0;
    }
    if (v >= 1.0f) {
        return PI/2;
    }
    if (v <= -1.0f) {
        return -PI/2;
    }
    return asinf(v);
}
float constrain_float(float amt, float min, float max) 
{
	if (isnan(amt)) {
		return (min+max)*0.5f;
	}
	return ((amt)<(min)?(min):((amt)>(max)?(max):(amt)));
}
int16_t constrain_int16(int16_t amt, int16_t min, int16_t max)
{
	return ((amt)<(min) ? (min) : ((amt)>(max) ? (max) : (amt)));
}
float safe_sqrt(float v)
{
	float ret = sqrtf(v);
	if (isnan(ret)) {
		return 0;
	}
	return ret;
}
// square
float square(float v) {
	return v*v;
}
// 2D vector length
float pythagorous2(float a, float b) {
	return sqrtf(square(a)+square(b));
}
// 3D vector length
float pythagorous3(float a, float b, float c) {
	return sqrtf(square(a)+square(b)+square(c));
}
float pythagorous4(float a, float b, float c,float d) {
	return sqrtf(square(a) + square(b) + square(c) + square(d));
}
float wrap_180_float(float angle)
{
	if (angle > 540.0f || angle < -540.0f) {
		// for large numbers use modulus
		angle = fmod(angle, 360.0f);
	}

	if (angle > 180.0f) { angle -= 360.0f; }
	if (angle < -180.0f) { angle += 360.0f; }
	return angle;
}
float wrap_180_cd_float(float angle)
{
	if (angle > 54000.0f || angle < -54000.0f) {
		// for large numbers use modulus
		angle = fmod(angle, 36000.0f);
	}

	if (angle > 18000.0f) { angle -= 36000.0f; }
	if (angle < -18000.0f) { angle += 36000.0f; }
	return angle;
}
int32_t wrap_360_cd(int32_t error)
{
	if (error > 360000 || error < -360000) {
		// for very large numbers use modulus
		error = error % 36000;
	}
	while (error >= 36000) error -= 36000;
	while (error < 0) error += 36000;
	return error;
}
int32_t wrap_180_cd(int32_t error)
{
	if (error > 360000 || error < -360000) {
		// for very large numbers use modulus
		error = error % 36000;
	}
	while (error > 18000) { error -= 36000; }
	while (error < -18000) { error += 36000; }
	return error;
}

