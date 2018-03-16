#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "sys.h"



#define MATRIX_SIZE_MAX	8
#define MATRIX_DEBUG



template <typename T>
class Vector
{
public:
	Vector():_dem(0){}
	Vector(uint8_t dem):_dem(dem){}
	Vector(const T* arr,uint8_t dem):_dem(dem)
	{
		for(uint8_t i=0;i<dem;i++)_array[i] = arr[i];
	}	
	Vector(const Vector<T>& ahs);	
	void operator=(const Vector<T>& ahs);
		
		
	Vector<T> operator+(const Vector<T>& ahs);
	void operator+=(const Vector<T>& ahs);
	
	Vector<T> operator*(const Vector<T>& vec);
	void operator*=(const Vector<T>& ahs);

	
	T dot(const Vector<T>& ahs);
	
	
	void from_array(const T* arr,uint8_t dem)
	{
		_dem = dem;
		for(uint8_t i=0;i<_dem;i++)_array[i] = arr[i];
	}
	
	T operator[](uint8_t index)const
	{
		return _array[index];
	}
	
	uint8_t dem()const{ return _dem; }
	void dem(uint8_t d) {_dem = d;}
	
private:
	T _array[MATRIX_SIZE_MAX];
	uint8_t _dem;
		
};


template <typename T>
class Matrix
{
	Matrix(uint8_t row,uint8_t col);
	Matrix(const T* arr,uint8_t row,uint8_t col)
	{
		
		for(uint8_t i=0;i<row;i++)
		{
			_varray[i].from_array(arr,col);
			arr += col;
			
		}
	}
	
	Matrix(const Matrix<T>& ahs);
	void operator=(const Matrix<T>& ahs);
	

	
	Vector<T>& operator[](uint8_t r)
	{
		return _varray[r];
	}
	
	
	const Vector<T>& operator[](uint8_t r)const
	{
		return _varray[r];
	}
	
	uint8_t row()const	{ return _row;}
	uint8_t col()const	{ return _varray[0].dem();}
	
	void transport();
	Matrix<T> transported()const;
	
	void operator+(const Matrix<T>& ahs);
	void operator+=(const Matrix<T>& ahs);

	Vector<T> operator*(const Vector<T>& vec);
	Matrix<T> operator*(const Matrix<T>& ahs);
	//void operator*=(const Matrix<T>& ahs);
	
	
	
private:
	uint8_t _row;
	Vector<T> _varray[MATRIX_SIZE_MAX];

};




#endif

