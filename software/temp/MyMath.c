#include "MyMath.h"



// square
float sq(float v) {
	return v*v;
}

// 2D vector length
float pythagorous2(float a, float b) 
{
	return sqrtf(sq(a)+sq(b));
}

// 3D vector length
float pythagorous3(float a, float b, float c) 
{
	return sqrtf(sq(a)+sq(b)+sq(c));
}
