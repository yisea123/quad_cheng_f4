#include "Vector3f.h"






float Vector3f::length() const
{
	return pythagorous3(x,y,z);
}

float Vector3f::angle(const Vector3f& v)const
{
	return acosf(((*this)*v) / (this->length()*v.length()));
}

////v'M' = (Mv)'
//Vector3f Vector3f::operator *(const Matrix3f &m) const
//{
//	return Vector3f(*this * m.colx(),
//					*this * m.coly(),
//					*this * m.colz());
//}

////列向量与行向量得到一个3*3矩阵
//Matrix3f Vector3f::mul_rowcol(const Vector3f &v2) const
//{
//    const Vector3f v1 = *this;
//    return Matrix3f (v1.x * v2.x, v1.x * v2.y, v1.x * v2.z,
//                     v1.y * v2.x, v1.y * v2.y, v1.y * v2.z,
//                     v1.z * v2.x, v1.z * v2.y, v1.z * v2.z);
//}

