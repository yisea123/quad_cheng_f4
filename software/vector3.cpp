#include "QMath.h"

template <typename T>
void Vector3<T>::rotate(enum Rotation rotation)
{
    T tmp;
    switch (rotation) {
    case ROTATION_NONE:
    case ROTATION_MAX:
        return;
    case ROTATION_YAW_45: {
        tmp = HALF_SQRT_2*(x - y);
        y   = HALF_SQRT_2*(x + y);
        x = tmp;
        return;
    }
    case ROTATION_YAW_90: {
        tmp = x; x = -y; y = tmp;
        return;
    }
    case ROTATION_YAW_135: {
        tmp = -HALF_SQRT_2*(x + y);
        y   =  HALF_SQRT_2*(x - y);
        x = tmp;
        return;
    }
    case ROTATION_YAW_180:
        x = -x; y = -y;
        return;
    case ROTATION_YAW_225: {
        tmp = HALF_SQRT_2*(y - x);
        y   = -HALF_SQRT_2*(x + y);
        x = tmp;
        return;
    }
    case ROTATION_YAW_270: {
        tmp = x; x = y; y = -tmp;
        return;
    }
    case ROTATION_YAW_315: {
        tmp = HALF_SQRT_2*(x + y);
        y   = HALF_SQRT_2*(y - x);
        x = tmp;
        return;
    }
    case ROTATION_ROLL_180: {
        y = -y; z = -z;
        return;
    }
    case ROTATION_ROLL_180_YAW_45: {
        tmp = HALF_SQRT_2*(x + y);
        y   = HALF_SQRT_2*(x - y);
        x = tmp; z = -z;
        return;
    }
    case ROTATION_ROLL_180_YAW_90: {
        tmp = x; x = y; y = tmp; z = -z;
        return;
    }
    case ROTATION_ROLL_180_YAW_135: {
        tmp = HALF_SQRT_2*(y - x);
        y   = HALF_SQRT_2*(y + x);
        x = tmp; z = -z;
        return;
    }
    case ROTATION_PITCH_180: {
        x = -x; z = -z;
        return;
    }
    case ROTATION_ROLL_180_YAW_225: {
        tmp = -HALF_SQRT_2*(x + y);
        y   =  HALF_SQRT_2*(y - x);
        x = tmp; z = -z;
        return;
    }
    case ROTATION_ROLL_180_YAW_270: {
        tmp = x; x = -y; y = -tmp; z = -z;
        return;
    }
    case ROTATION_ROLL_180_YAW_315: {
        tmp =  HALF_SQRT_2*(x - y);
        y   = -HALF_SQRT_2*(x + y);
        x = tmp; z = -z;
        return;
    }
    case ROTATION_ROLL_90: {
        tmp = z; z = y; y = -tmp;
        return;
    }
    case ROTATION_ROLL_90_YAW_45: {
        tmp = z; z = y; y = -tmp;
        tmp = HALF_SQRT_2*(x - y);
        y   = HALF_SQRT_2*(x + y);
        x = tmp;
        return;
    }
    case ROTATION_ROLL_90_YAW_90: {
        tmp = z; z = y; y = -tmp;
        tmp = x; x = -y; y = tmp;
        return;
    }
    case ROTATION_ROLL_90_YAW_135: {
        tmp = z; z = y; y = -tmp;
        tmp = -HALF_SQRT_2*(x + y);
        y   =  HALF_SQRT_2*(x - y);
        x = tmp;
        return;
    }
    case ROTATION_ROLL_270: {
        tmp = z; z = -y; y = tmp;
        return;
    }
    case ROTATION_ROLL_270_YAW_45: {
        tmp = z; z = -y; y = tmp;
        tmp = HALF_SQRT_2*(x - y);
        y   = HALF_SQRT_2*(x + y);
        x = tmp;
        return;
    }
    case ROTATION_ROLL_270_YAW_90: {
        tmp = z; z = -y; y = tmp;
        tmp = x; x = -y; y = tmp;
        return;
    }
    case ROTATION_ROLL_270_YAW_135: {
        tmp = z; z = -y; y = tmp;
        tmp = -HALF_SQRT_2*(x + y);
        y   =  HALF_SQRT_2*(x - y);
        x = tmp;
        return;
    }
    case ROTATION_PITCH_90: {
        tmp = z; z = -x; x = tmp;
        return;
    }
    case ROTATION_PITCH_270: {
        tmp = z; z = x; x = -tmp;
        return;
    }
    case ROTATION_PITCH_180_YAW_90: {
        z = -z;
        tmp = -x; x = -y; y = tmp;
        return;
    }
    case ROTATION_PITCH_180_YAW_270: {
        x = -x; z = -z;
        tmp = x; x = y; y = -tmp;
        return;
    }
    case ROTATION_ROLL_90_PITCH_90: {
        tmp = z; z = y; y = -tmp;
        tmp = z; z = -x; x = tmp;
        return;
    }
    case ROTATION_ROLL_180_PITCH_90: {
        y = -y; z = -z;
        tmp = z; z = -x; x = tmp;
        return;
    }
    case ROTATION_ROLL_270_PITCH_90: {
        tmp = z; z = -y; y = tmp;
        tmp = z; z = -x; x = tmp;
        return;
    }
    case ROTATION_ROLL_90_PITCH_180: {
        tmp = z; z = y; y = -tmp;
        x = -x; z = -z;
        return;
    }
    case ROTATION_ROLL_270_PITCH_180: {
        tmp = z; z = -y; y = tmp;
        x = -x; z = -z;
        return;
    }
    case ROTATION_ROLL_90_PITCH_270: {
        tmp = z; z = y; y = -tmp;
        tmp = z; z = x; x = -tmp;
        return;
    }
    case ROTATION_ROLL_180_PITCH_270: {
        y = -y; z = -z;
        tmp = z; z = x; x = -tmp;
        return;
    }
    case ROTATION_ROLL_270_PITCH_270: {
        tmp = z; z = -y; y = tmp;
        tmp = z; z = x; x = -tmp;
        return;
    }
    case ROTATION_ROLL_90_PITCH_180_YAW_90: {
        tmp = z; z = y; y = -tmp;
        x = -x; z = -z;
        tmp = x; x = -y; y = tmp;
        return;
    }
    case ROTATION_ROLL_90_YAW_270: {
        tmp = z; z = y; y = -tmp;
        tmp = x; x = y; y = -tmp;
        return;
    }
    case ROTATION_YAW_293_PITCH_68_ROLL_90: {
        float tmpx = x;
        float tmpy = y;
        float tmpz = z;
        x =  0.143039f * tmpx +  0.368776f * tmpy + -0.918446f * tmpz;
        y = -0.332133f * tmpx + -0.856289f * tmpy + -0.395546f * tmpz;
        z = -0.932324f * tmpx +  0.361625f * tmpy +  0.000000f * tmpz;
        return;
	}
	}
}

// vector cross product
template <typename T>
Vector3<T> Vector3<T>::operator %(const Vector3<T> &v) const
{
    Vector3<T> temp(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    return temp;
}

// dot product
template <typename T>
T Vector3<T>::operator *(const Vector3<T> &v) const
{
    return x*v.x + y*v.y + z*v.z;
}

template <typename T>
float Vector3<T>::length(void) const
{
    return pythagorous3(x, y, z);
}

template <typename T>
Vector3<T> &Vector3<T>::operator *=(const T num)
{
    x*=num; y*=num; z*=num;
    return *this;
}

template <typename T>
Vector3<T> &Vector3<T>::operator /=(const T num)
{
    x /= num; y /= num; z /= num;
    return *this;
}

template <typename T>
Vector3<T> &Vector3<T>::operator -=(const Vector3<T> &v)
{
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

template <typename T>
bool Vector3<T>::is_nan(void) const
{
    return isnan(x) || isnan(y) || isnan(z);
}

template <typename T>
bool Vector3<T>::is_inf(void) const
{
    return isinf(x) || isinf(y) || isinf(z);
}

template <typename T>
Vector3<T> &Vector3<T>::operator +=(const Vector3<T> &v)
{
    x+=v.x; y+=v.y; z+=v.z;
    return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator /(const T num) const
{
    return Vector3<T>(x/num, y/num, z/num);
}

template <typename T>
Vector3<T> Vector3<T>::operator *(const T num) const
{
    return Vector3<T>(x*num, y*num, z*num);
}

template <typename T>
Vector3<T> Vector3<T>::operator -(const Vector3<T> &v) const
{
    return Vector3<T>(x-v.x, y-v.y, z-v.z);
}

template <typename T>
Vector3<T> Vector3<T>::operator +(const Vector3<T> &v) const
{
    return Vector3<T>(x+v.x, y+v.y, z+v.z);
}

template <typename T>
Vector3<T> Vector3<T>::operator -(void) const
{
    return Vector3<T>(-x,-y,-z);
}

template <typename T>
bool Vector3<T>::operator ==(const Vector3<T> &v) const
{
    return (x==v.x && y==v.y && z==v.z);
}

template <typename T>
bool Vector3<T>::operator !=(const Vector3<T> &v) const
{
    return (x!=v.x && y!=v.y && z!=v.z);
}




template <typename T>
float Vector3<T>::angle(const Vector3<T> &v2) const
{
    return acosf(((*this)*v2) / (this->length()*v2.length()));
}

// multiplication of transpose by a vector
template <typename T>
Vector3<T> Vector3<T>::operator *(const Matrix3<T> &m) const
{
    return Vector3<T>(*this * m.colx(),
                      *this * m.coly(),
                      *this * m.colz());
}

// multiply a column vector by a row vector, returning a 3x3 matrix
template <typename T>
Matrix3<T> Vector3<T>::mul_rowcol(const Vector3<T> &v2) const
{
    const Vector3<T> v1 = *this;
    return Matrix3<T>(v1.x * v2.x, v1.x * v2.y, v1.x * v2.z,
                      v1.y * v2.x, v1.y * v2.y, v1.y * v2.z,
                      v1.z * v2.x, v1.z * v2.y, v1.z * v2.z);
}

// only define for float
template void Vector3<float>::rotate(enum Rotation);
template float Vector3<float>::length(void) const;
template Vector3<float> Vector3<float>::operator %(const Vector3<float> &v) const;
template float Vector3<float>::operator *(const Vector3<float> &v) const;
template Vector3<float> Vector3<float>::operator *(const Matrix3<float> &m) const;
template Matrix3<float> Vector3<float>::mul_rowcol(const Vector3<float> &v) const;
template Vector3<float> &Vector3<float>::operator *=(const float num);
template Vector3<float> &Vector3<float>::operator /=(const float num);
template Vector3<float> &Vector3<float>::operator -=(const Vector3<float> &v);
template Vector3<float> &Vector3<float>::operator +=(const Vector3<float> &v);
template Vector3<float> Vector3<float>::operator /(const float num) const;
template Vector3<float> Vector3<float>::operator *(const float num) const;
template Vector3<float> Vector3<float>::operator +(const Vector3<float> &v) const;
template Vector3<float> Vector3<float>::operator -(const Vector3<float> &v) const;
template Vector3<float> Vector3<float>::operator -(void) const;
template bool Vector3<float>::operator ==(const Vector3<float> &v) const;
template bool Vector3<float>::operator !=(const Vector3<float> &v) const;
template bool Vector3<float>::is_nan(void) const;
template bool Vector3<float>::is_inf(void) const;
template float Vector3<float>::angle(const Vector3<float> &v) const;

template void Vector3<double>::rotate(enum Rotation);
template float Vector3<double>::length(void) const;
template Vector3<double> Vector3<double>::operator %(const Vector3<double> &v) const;
template double Vector3<double>::operator *(const Vector3<double> &v) const;
template Vector3<double> Vector3<double>::operator *(const Matrix3<double> &m) const;
template Matrix3<double> Vector3<double>::mul_rowcol(const Vector3<double> &v) const;
template Vector3<double> &Vector3<double>::operator *=(const double num);
template Vector3<double> &Vector3<double>::operator /=(const double num);
template Vector3<double> &Vector3<double>::operator -=(const Vector3<double> &v);
template Vector3<double> &Vector3<double>::operator +=(const Vector3<double> &v);
template Vector3<double> Vector3<double>::operator /(const double num) const;
template Vector3<double> Vector3<double>::operator *(const double num) const;
template Vector3<double> Vector3<double>::operator +(const Vector3<double> &v) const;
template Vector3<double> Vector3<double>::operator -(const Vector3<double> &v) const;
template Vector3<double> Vector3<double>::operator -(void) const;
template bool Vector3<double>::operator ==(const Vector3<double> &v) const;
template bool Vector3<double>::operator !=(const Vector3<double> &v) const;
template bool Vector3<double>::is_nan(void) const;
template bool Vector3<double>::is_inf(void) const;
template float Vector3<double>::angle(const Vector3<double> &v) const;


