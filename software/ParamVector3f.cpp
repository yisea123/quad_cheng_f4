#include "ParamVector3f.h"



uint8_t Param_Vector3f::param_get(uint8_t * p) 
{
	memcpy(&x, p, sizeof(float)); p += sizeof(float);
	memcpy(&y, p, sizeof(float)); p += sizeof(float);
	memcpy(&z, p, sizeof(float));
	return (3 * sizeof(float));
}

uint8_t Param_Vector3f::param_set(uint8_t * p)const
{
	memcpy(p, &x, sizeof(float)); p += sizeof(float);
	memcpy(p, &y, sizeof(float)); p += sizeof(float);
	memcpy(p, &z, sizeof(float));
	return (3 * sizeof(float));
}

