#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include "sys.h"
#include <math.h>

template <typename T>
struct Vector2
{
    T x, y;

    // trivial ctor
    Vector2<T>() {
        x = y = 0;
    }

    // setting ctor
    Vector2<T>(const T x0, const T y0) : x(x0), y(y0) {
    }

    // function call operator
    void operator ()(const T x0, const T y0)
    {
        x= x0; y= y0;
    }

    // test for equality
    bool operator ==(const Vector2<T> &v) const;

    // test for inequality
    bool operator !=(const Vector2<T> &v) const;

    // negation
    Vector2<T> operator -(void) const;

    // addition
    Vector2<T> operator +(const Vector2<T> &v) const;

    // subtraction
    Vector2<T> operator -(const Vector2<T> &v) const;

    // uniform scaling
    Vector2<T> operator *(const T num) const;

    // uniform scaling
    Vector2<T> operator  /(const T num) const;

    // addition
    Vector2<T> &operator +=(const Vector2<T> &v);

    // subtraction
    Vector2<T> &operator -=(const Vector2<T> &v);

    // uniform scaling
    Vector2<T> &operator *=(const T num);

    // uniform scaling
    Vector2<T> &operator /=(const T num);

    // dot product
    T operator *(const Vector2<T> &v) const;

    // cross product
    T operator %(const Vector2<T> &v) const;

    // computes the angle between this vector and another vector
    float angle(const Vector2<T> &v2) const;

    // computes the angle in radians between the origin and this vector
    //T angle(void) const;

    // check if any elements are NAN
    bool is_nan(void) const;

    // check if any elements are infinity
    bool is_inf(void) const;

    // zero the vector
    void zero()
    {
        x = y = 0;
    }

    // gets the length of this vector squared
    T   length_squared() const
    {
        return (T)(*this * *this);
    }

    // gets the length of this vector
    float           length(void) const;

    // normalizes this vector
    void    normalize()
    {
        *this/=length();
    }

    // returns the normalized vector
    Vector2<T>  normalized() const
    {
        return *this/length();
    }

    // reflects this vector about n
	//将向量做n方向的镜像
    void    reflect(const Vector2<T> &n)
    {
        Vector2<T>        orig(*this);
        project(n);
        *this= *this*2 - orig;
    }

    // projects this vector onto v
	//将向量投影到v方向上
    void    project(const Vector2<T> &v)
    {
        *this= v * (*this * v)/(v*v);
    }

    // returns this vector projected onto v
	//返回投影到v方向上的向量
    Vector2<T>  projected(const Vector2<T> &v)
    {
        return v * (*this * v)/(v*v);
    }
};

typedef Vector2<int16_t>        Vector2i;
typedef Vector2<uint16_t>       Vector2ui;
typedef Vector2<int32_t>        Vector2l;
typedef Vector2<uint32_t>       Vector2ul;
typedef Vector2<float>          Vector2f;

#endif // VECTOR2_H
