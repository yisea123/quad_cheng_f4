#ifndef _QUATERNION_H_
#define _QUATERNION_H_
#include "sys.h"
#include "math.h"
#include "vector3.h"
#include "matrix3.h"



/***************************************************************

Q(q0,q1,q2,q3) = q0 + q1*i + q2*j + q3*k;
不满足乘法交换律,满足结合律
Q = cos(theta) + uR*sin(theta);





****************************************************************/

class Quaternion
{
public:
	Quaternion()
	{
		q[0] = 0.0f;
		q[1] = q[2] = q[3] = 0.0f;
	}
	Quaternion(const float d[4]){
		q[0] = d[0]; q[1] = d[1]; q[2] = d[2]; q[3] = d[3];
	}
	Quaternion(float q0, float q1, float q2, float q3) {
		q[0] = q0; q[1] = q1; q[2] = q2; q[3] = q3;
	}
	
	void operator()(float q0,float q1,float q2,float q3) {
		q[0] = q0; q[1] = q1; q[2] = q2; q[3] = q3;
	}

	//乘法,不满足交换律,满足结合律
	Quaternion operator*(const Quaternion &rhs)const{
		return Quaternion(
			q[0] * rhs.q[0] - q[1] * rhs.q[1] - q[2] * rhs.q[2] - q[3] * rhs.q[3],
			q[0] * rhs.q[1] + q[1] * rhs.q[0] + q[2] * rhs.q[3] - q[3] * rhs.q[2],
			q[0] * rhs.q[2] - q[1] * rhs.q[3] + q[2] * rhs.q[0] + q[3] * rhs.q[1],
			q[0] * rhs.q[3] + q[1] * rhs.q[2] - q[2] * rhs.q[1] + q[3] * rhs.q[0]);
	}
	//除法,四元数的逆位四元数的共轭归一化
	Quaternion operator /(const Quaternion &rhs) const {
		float norm = rhs.length_squared();
		return Quaternion(
			(q[0] * rhs.q[0] + q[1] * rhs.q[1] + q[2] * rhs.q[2] + q[3] * rhs.q[3]) / norm,
			(-q[0] * rhs.q[1] + q[1] * rhs.q[0] - q[2] * rhs.q[3] + q[3] * rhs.q[2]) / norm,
			(-q[0] * rhs.q[2] + q[1] * rhs.q[3] + q[2] * rhs.q[0] - q[3] * rhs.q[1]) / norm,
			(-q[0] * rhs.q[3] - q[1] * rhs.q[2] + q[2] * rhs.q[1] + q[3] * rhs.q[0]) / norm
		);
	}
	//加法
	Quaternion operator +(const Quaternion &rhs)const {
		return Quaternion(q[0] + rhs.q[0], q[1] + rhs.q[1], q[2] + rhs.q[2], q[3] + rhs.q[3]);
	}
	//减法
	Quaternion operator -(const Quaternion &rhs)const {
		return Quaternion(q[0] - rhs.q[0], q[1] - rhs.q[1], q[2] - rhs.q[2], q[3] - rhs.q[3]);
	}
	//取负
	Quaternion operator -()const {
		return Quaternion(-q[0] , -q[1] ,- q[2], -q[3]);
	}
	//+=
	Quaternion& operator +=(const Quaternion &rhs) {
		q[0] += rhs.q[0]; q[1] += rhs.q[1]; q[2] += rhs.q[2]; q[3] += rhs.q[3];
		return *this;
	}
	//-=
	Quaternion& operator -=(const Quaternion &rhs) {
		q[0] -= rhs.q[0]; q[1] -= rhs.q[1]; q[2] -= rhs.q[2]; q[3] -= rhs.q[3];
		return *this;
	}
	//乘标量
	Quaternion operator*(float a)const{
		return Quaternion(q[0] * a, q[1] * a, q[2] * a, q[3] * a);
	}
	//除标量
	Quaternion operator/(float a)const {

		return operator*(1.0f / a);
	}
	//*=
	Quaternion& operator*=(float a) {
		q[0] *= a; q[1] *= a; q[2] *= a; q[3] *= a;
		return *this;
	}
	// /=
	Quaternion& operator/=(float a) {
		return operator*=(1.0f / a);
	}

	//共轭
	Quaternion conjugated() const {
		return Quaternion(q[0], -q[1], -q[2], -q[3]);
	}
	//虚部
	Vector3f imag(void) {
		return Vector3f(q[1],q[2],q[3]);
	}
	
	//
	
	
	//转换
	void from_euler(float roll, float pitch, float yaw) {
		double cosPhi_2 = cos(double(roll) / 2.0);
		double sinPhi_2 = sin(double(roll) / 2.0);
		double cosTheta_2 = cos(double(pitch) / 2.0);
		double sinTheta_2 = sin(double(pitch) / 2.0);
		double cosPsi_2 = cos(double(yaw) / 2.0);
		double sinPsi_2 = sin(double(yaw) / 2.0);

		/* operations executed in double to avoid loss of precision through
		* consecutive multiplications. Result stored as float.
		*/
		q[0] = static_cast<float>(cosPhi_2 * cosTheta_2 * cosPsi_2 + sinPhi_2 * sinTheta_2 * sinPsi_2);
		q[1] = static_cast<float>(sinPhi_2 * cosTheta_2 * cosPsi_2 - cosPhi_2 * sinTheta_2 * sinPsi_2);
		q[2] = static_cast<float>(cosPhi_2 * sinTheta_2 * cosPsi_2 + sinPhi_2 * cosTheta_2 * sinPsi_2);
		q[3] = static_cast<float>(cosPhi_2 * cosTheta_2 * sinPsi_2 - sinPhi_2 * sinTheta_2 * cosPsi_2);
	}
	void from_yaw(float yaw) {
		q[0] = cosf(yaw / 2.0f);
		q[1] = 0.0f;
		q[2] = 0.0f;
		q[3] = sinf(yaw / 2.0f);
	}
	Vector3f to_euler() const {
		return Vector3f(
			atan2f(2.0f * (q[0] * q[1] + q[2] * q[3]), 1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2])),
			asinf(2.0f * (q[0] * q[2] - q[3] * q[1])),
			atan2f(2.0f * (q[0] * q[3] + q[1] * q[2]), 1.0f - 2.0f * (q[2] * q[2] + q[3] * q[3]))
			);
	}

	void from_dcm(const Matrix3f &dcm) {

	}
	Matrix3f to_dcm(void) const {
		return Matrix3f(
			Vector3f(1.0f - 2.0f*(q[2] * q[2] + q[3] * q[3]), 2.0f*(q[1] * q[2] - q[0] * q[3]), 2.0f*(q[1] * q[3] + q[0] * q[2])),
			Vector3f(2.0f*(q[1] * q[2] + q[0] * q[3]), 1.0f - 2.0f*(q[1] * q[1] + q[3] * q[3]), 2.0f*(q[2] * q[3] - q[0] * q[1])),
			Vector3f(2.0f*(q[1] * q[3] - q[0] * q[2]), 2.0f*(q[2] * q[3] + q[0] * q[1]), 1.0f - 2.0f*(q[1] * q[1] + q[2] * q[2])));
	}

	//变换
	void roate(const Vector3f &g);


	//处理
	bool is_inf() {
		return (isinf(q[0]) || isinf(q[1]) || isinf(q[2]) || isinf(q[3]));
	}
	bool is_nan() {
		return isnan(q[0]) || isnan(q[1]) || isnan(q[2]) || isnan(q[3]);
	}
	bool is_zero(void) const {
		return (q[0] == 0 && q[1] == 0 && q[2] == 0 && q[3] == 0);
	}
	float length_squared() const;
	float length(){
		return sqrtf(length_squared());
	}
	//归一化
	void normalize()
	{
		*this /= length();
	}


	//
	float q0() { return q[0]; }
	float q1() { return q[1]; }
	float q2() { return q[2]; }
	float q3() { return q[3]; }
	float  operator[](uint8_t i) const{
		return q[i];
	}

	float& operator[](uint8_t i) {
		return q[i];
	}
	float q[4];

};

inline Quaternion operator*(float a,const Quaternion& rhs)
{
	return rhs*a;
}

#endif


