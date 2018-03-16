#ifndef	_HAL_H_
#define _HAL_H_

#include "sys.h"
class HAL
{
public:
	virtual ~HAL(){};
		
	virtual  void Setup() = 0;
	virtual void pwm_write(uint8_t idx, uint16_t pwm) {}
};







#endif

