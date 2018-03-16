#ifndef _PARAMVECTOR3F_H_
#define _PARAMVECTOR3F_H_
#include "sys.h"

#include "Parameter.h"
#include "vector3.h"


class Param_Vector3f:public ParameterBase,public Vector3f
{
public:
	Param_Vector3f(){ }
	Param_Vector3f(float x0, float y0, float z0) :Vector3f(x0, y0, z0) { }
	virtual ~Param_Vector3f(){ }

	const Vector3f& operator=(const Vector3f& rhs) { //¸³Öµ²Ù×÷
		x = rhs.x; y = rhs.y; z = rhs.z;
		return (*this); }	

	

// 	void set(const Vector3f &ahr){
// 		x = ahr.x;
// 		y = ahr.y;
// 		z = ahr.z;
// 	}
// 	const Vector3f& get(void)const{
// 		return (*this);
// 	}




private:
	virtual uint8_t param_get(uint8_t *p);
	virtual uint8_t param_set(uint8_t *p)const;
};



#endif

