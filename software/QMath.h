#ifndef _QMATH_H_
#define _QMATH_H_

#include "rotations.h"
#include "vector2.h"
#include "vector3.h"
#include "matrix3.h"
#include "Quaternion.h"
#include "math.h"

//#include "Matrix.h"


#ifndef M_PI_F
 #define M_PI_F 3.141592653589793f
#endif


#ifndef PI
 # define PI M_PI_F
#endif


#ifndef M_PI_2
 # define M_PI_2 1.570796326794897f
#endif

#define GRAVITY_MSS 9.80665f

//Single precision conversions
#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f

#define HALF_SQRT_2 0.70710678118654757f

/************************************************************************/

float square(float v);
float pythagorous2(float a, float b);
float pythagorous3(float a, float b, float c);
float pythagorous4(float a, float b, float c, float d);

float radians(float deg);
float degrees(float rad);
float constrain_float(float amt, float min, float max) ;
int16_t constrain_int16(int16_t amt, int16_t min, int16_t max);

float wrap_180_cd_float(float angle);
int32_t wrap_360_cd(int32_t error);
int32_t wrap_180_cd(int32_t error);


float safe_asin(float v);
float safe_sqrt(float v);



template <typename T>
inline T constrain(const T& v,const T& min,const T& max)
{
	return (v < min ? min : (v > max ? max : v));
}



template <typename T>
inline T min(const T& a, const T& b)
{
	return (a < b ? a : b);
}



template <typename T>
inline T max(const T& a, const T& b)
{
	return (a > b ? a : b);
}

inline int16_t max(int16_t a, int16_t b)
{
	return (a > b ? a : b);
}
inline int16_t min(int16_t a, int16_t b)
{
	return (a < b ? a : b);
}

template <typename T>
inline T abs(const T& v)
{
	return (v > 0 ? v : (-v));
}

#endif

