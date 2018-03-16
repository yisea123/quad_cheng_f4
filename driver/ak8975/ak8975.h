#ifndef _AK8975_H_
#define	_AK8975_H_

#include "sys.h"
#include "HAL_F4.h"



#define AK8975_MODE_PowerDown		0x00
#define AK8975_MODE_SignalMeasure	0x01
#define AK8975_MODE_SelfTest		0x08
#define AK8975_MODE_POWR_DOWN		0x0F

class AK8975
{
public:
	AK8975(const I2C& iic):
		_iic(iic),
		_mag_scale(0.0f)
	{}
		
	uint8_t init();	
		
	uint8_t start();
	uint8_t ready();
	uint8_t get_mag_adc(int16_t *mag);
	uint8_t get_mag(float *mag);
		




private:
	const I2C& _iic;
	float _mag_scale;
	
};
extern AK8975 ak8975;





#endif

