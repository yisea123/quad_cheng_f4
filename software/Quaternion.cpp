#include "QMath.h"



float Quaternion::length_squared() const
{
	return (square(q[0]) + square(q[1]) + square(q[2]) + square(q[3]));
}


void Quaternion::roate(const Vector3f & g)
{
	Quaternion qdot;
	Vector3f half_g = g / 2;

	qdot.q[0] = -q[1] * half_g.x - q[2] * half_g.y - q[3] * half_g.z;
	qdot.q[1] = q[0] * half_g.x + q[2] * half_g.z - q[3] * half_g.y;
	qdot.q[2] = q[0] * half_g.y - q[1] * half_g.z + q[3] * half_g.x;
	qdot.q[3] = q[0] * half_g.z + q[1] * half_g.y - q[2] * half_g.x;

	(*this) += qdot;

}

