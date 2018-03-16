#include "QMath.h"



void matrix_vector_error()
{

}





template <typename T>
Vector<T>::Vector(const Vector<T>& ahs)
{
	_dem = ahs.dem();
	for(uint8_t d=0;d<ahs.dem();d++)
	{
		_array[d] = ahs[d];
	}	
}

template <typename T>
void Vector<T>::operator=(const Vector<T>& ahs)
{
	_dem = ahs.dem();
	for(uint8_t d=0;d<ahs.dem();d++)
	{
		_array[d] = ahs[d];
	}		
}


template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& ahs)
{
	if(_dem != ahs.dem())
	{
		matrix_vector_error();
	}
	
	Vector<T> ve(this);
	for(uint8_t i=0;i<_dem;i++)
		ve += ahs[i];
	return ve;
}


template <typename T>
void Vector<T>::operator+=(const Vector<T>& ahs)
{
	if(_dem != ahs.dem())
	{
		matrix_vector_error();
	}
	for(uint8_t i=0;i<_dem;i++)
		_array += ahs[i];
}

template <typename T>
Vector<T> Vector<T>::operator*(const Vector<T>& ahs)
{
	if(_dem != ahs.dem())
	{
		matrix_vector_error();
	}
	
	Vector<T> ve(this);
	for(uint8_t i=0;i<_dem;i++)
		ve *= ahs[i];
	return ve;
}


template <typename T>
void Vector<T>::operator*=(const Vector<T>& ahs)
{
	if(_dem != ahs.dem())
	{
		matrix_vector_error();
	}
	for(uint8_t i=0;i<_dem;i++)
		_array *= ahs[i];
}

template <typename T>
T Vector<T>::dot(const Vector<T>& ahs)
{
	if(_dem != ahs.dem())
	{
		matrix_vector_error();
	}
	
	T res = _array[0] * ahs[0];
	for(uint8_t i=1;i<ahs.dem();i++)
	{
		res += _array[i]* ahs[i];
	}
	return res;
}
/*********************************************************/

template <typename T>
Matrix<T>::Matrix(uint8_t row,uint8_t col)
{	
	_row = row;
	for(uint8_t i=0;i<row;i++)
	{
		_varray[i].dem(col);
	}
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& ahs)	
{
	_row = ahs.row();
	for(uint8_t r=0;r<_row;r++)
	{
		_varray[r] = ahs[r];
	}	
}

template <typename T>
void Matrix<T>::operator=(const Matrix<T>& ahs)
{
	_row = ahs.row();
	for(uint8_t r=0;r<_row;r++)
	{
		_varray[r] = ahs[r];
	}	
}

template <typename T>
void Matrix<T>::operator+(const Matrix<T>& ahs)
{
	if(ahs.row() != _row || ahs.col() != this->col())
	{
		matrix_vector_error();
	}
	
	Matrix<T> ma(this);
	for(uint8_t i=0;i<ma.row();i++)
	{
		ma[i] += ahs[i];
	}
}



template <typename T>
void Matrix<T>::operator+=(const Matrix<T>& ahs)
{
	if(ahs.row() != _row || ahs.col() != this->col())
	{
		matrix_vector_error();
	}
	
	for(uint8_t i=0;i<_row;i++)
	{
		_varray[i] += ahs[i];
	}
}


template <typename T>
Vector<T> Matrix<T>::operator*(const Vector<T>& vec)
{
	if(this->col() != vec.dem())
	{
		matrix_vector_error();
	}
	
	Vector<T> res(vec.dem());
	for(uint8_t i=0;i<vec.dem();i++)
	{
		res[i] = _varray[i].dot(vec);
	}
	return res;
}



template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& ahs)
{

	if(this->col() != ahs.row() || this->row() != ahs.col())
	{
		matrix_vector_error();
	}		
	
	Matrix<T> res(this->row(),ahs.col());
	Matrix<T>	ma = ahs.transported();
	
	for(uint8_t i=0;i<ma.row();i++)
	{
		for(uint8_t j=0;j<ma.col();j++)
		{
			res[i][j] = _varray[i]*ma[j];
		}
	}
	return res;
}


template <typename T>
void Matrix<T>::transport()
{
	

}

template <typename T>
Matrix<T> Matrix<T>::transported()const
{
	Matrix<T> ma(this->col(),this->row());
	
	for(uint8_t i=0;i<ma.row();i++)
	{
		for(uint8_t j=0;j<ma.col();j++)
		{
			ma[i][j] = _varray[j][i]; 
		}
	}
	return ma;

}





