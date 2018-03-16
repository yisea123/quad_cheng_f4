#ifndef _COMPASS_AK8975_H_
#define _COMPASS_AK8975_H_
#include "sys.h"

#include "Compass.h"
#include "HAL_F4.h"
#include "AverageFilter.h"

/**********************************************************************************************************
AK8975 / C has the limitation for measurement range that the sum of absolute values of each axis should be
smaller than 2400¦ÌT.
| X | +| Y | +| Z | < 2400¦ÌT


**********************************************************************************************************/
class Compass_AK8975:public Compass
{
public:
	Compass_AK8975(SPIDriver& spi, GPIO cs) :
		_spi(spi),
		_cs_pin(cs)
	{

	}
	virtual void init(void);
	virtual void update(void);
	void start(void);
	uint8_t product_id(void)const { return _product_id; }
private:
	void _init_sensor(void);


	uint8_t _read_reg(uint8_t reg);
	void _read_reg(uint8_t reg, uint8_t *pbuf, uint8_t len);
	void _write_reg(uint8_t reg, uint8_t val);

	SPIDriver &_spi;
	GPIO _cs_pin;

	uint8_t _product_id;

	Vector3f _mag_raw;
	AverageFilter<Vector3f,20> _mag_filter;




};

#endif


