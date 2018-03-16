#pragma once


#include "MyMath.h"
#include "sys.h"



class Vector3f
{
public:
	
	Vector3f(){x=y=z=0;};
	Vector3f(float x0,float y0,float z0):x(x0),y(y0),z(z0){}
	Vector3f(float *vec):x(vec[0]),y(vec[1]),z(vec[2]){}		
	~Vector3f()	{}
	
		
	void operator ()(float x0,float y0,float z0){ x = x0; y = y0; z = z0;}
	float operator[](uint8_t i) const 
	{
        return (&x)[i];
    }
	
	bool operator ==(const Vector3f& v)const	{ return (x==v.x && y==v.y && z==v.z);}
	bool operator !=(const Vector3f& v)const	{ return !(*this == v);}
	
	//取负
	const Vector3f operator -(void) const 		{ return Vector3f(-x,-y,-z);	}
	//加法
	const Vector3f operator +(const Vector3f& v) const 
	{
		return Vector3f(x + v.x,y + v.y,z + v.z);
	}
	//减法
	const Vector3f operator -(const Vector3f& v) const 
	{
		return Vector3f(x - v.x,y - v.y,z - v.z);
	}
	
	//标量乘
	const Vector3f operator *(float k) const	
	{
		return Vector3f(k*x,k*y,k*z);
	}
	//标量除
	const Vector3f operator /(float k) const
	{
		k=1.0f/k;
		return Vector3f(x*k,y*k,z*k);
	}
	
	//+=,-=,*=,/=
	Vector3f& operator +=(const Vector3f& vec) 
	{
		x += vec.x;		y += vec.y;		z += vec.z;
		return *this;
	}
	
	Vector3f& operator -=(const Vector3f& vec) 
	{
		x -= vec.x;		y -= vec.y;		z -= vec.z;
		return *this;
	}
	
	Vector3f& operator *=(float k) 
	{
		x *= k;		y *= k;		z *= k;
		return *this;
	}
	
	Vector3f& operator /=(float k) 
	{
		k = 1.0f/k;
		x *= k;		y *= k;		z *= k;
		return *this;
	}
	
	
	//点乘
	float operator *(const Vector3f& v) const 
	{
		return (x * v.x + y * v.y + z * v.z);
	}
	//叉乘
	const Vector3f operator %(const Vector3f& v) const 
	{
		return Vector3f(y * v.z - z*v.y,
						z * v.x - x*v.z,
						x * v.y - y*v.x);
	}
	
	//v'M' = (Mv)'
	//Vector3f operator *(const Matrix3f &m) const;
	//列向量与行向量得到一个3*3矩阵
	//Matrix3f Vector3f::mul_rowcol(const Vector3f &v2) const
	
	//===============计算
	//模长
	float length() const;
	float length_squared()const {	return  *this * (*this);	}
	float angle(const Vector3f& v)const;
	
	
	
    //将向量投影到v
    void project(const Vector3f &v)
    {
        *this = v * (*this * v)/(v*v);
    }	
	//返回投影到v上的向量
    Vector3f projected(const Vector3f &v) const
    {
        return v * (*this * v)/(v*v);
    }

	
	
	//===============处理
	void normalize(){	*this /= length();	 }
	Vector3f normalized(void) const
    {
        return  *this/length();
    }
	
	void zero()		{	x = y = z = 0.0f;}
	void identify()	{	x = y = z = 1.0f;}
	
	
	//===============检查
	bool is_nan(void) const {return (isnan(x) || isnan(y) || isnan(z) );}
	bool is_inf(void) const {return (isinf(x) || isinf(y) || isinf(z) );}
	bool is_zero(void) const {return (x==0.0f && y==0.0f && z==0.0f);}
	

	//===============
	static const Vector3f zeros(){return Vector3f(0.0f,0.0f,0.0f);}
	static const Vector3f ones(){return Vector3f(1.0f,1.0f,1.0f);}
	
	
	
	
	float x;
	float y;
	float z;
};

//标量乘
inline const Vector3f operator*(float k,const Vector3f& vec)
{	
	return vec*k;
}


