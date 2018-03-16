#include "Compass_AK8975.h"



#define AK8975_WIA     0x00		//Device ID = 0x48

#define AK8975_HXL     0x03		//
#define AK8975_HXH     0x04
#define AK8975_HYL     0x05
#define AK8975_HYH     0x06
#define AK8975_HZL     0x07
#define AK8975_HZH     0x08

#define AK8975_CNTL    0x0A


#define AK8975_INFO     0x01
#define AK8975_ST1     	0x02
#define AK8975_ST2     	0x09
#define AK8975_ASTC     0x0C
//#define AK8975_TS1     	0x0D
//#define AK8975_TS2     	0x0E
#define AK8975_I2CDIS   0x0F
#define AK8975_ASAX     0x10
#define AK8975_ASAY     0x11
#define AK8975_ASAZ     0x12


//²âÁ¿Ä£Ê½
#define AK8975_MODE_PowerDown		0x00
#define AK8975_MODE_SignalMeasure	0x01		//
#define AK8975_MODE_SelfTest		0x08
#define AK8975_MODE_POWR_DOWN		0x0F


#define AK8975_ADDRESS	0x18



void Compass_AK8975::_init_sensor(void)
{
	_healthy = false;
	hal.scheduler.suspend_timer_procs();

	_cs_pin.mode_out();
	_product_id = _read_reg(AK8975_WIA);

	//printf("ak8975 id=%x\r\n", _product_id);

	if (_product_id != 0x48) {

		return;
	}

	hal.scheduler.resume_timer_procs();
}
void Compass_AK8975::init(void)
{
	_init_sensor();
	check_calibration();
}



void Compass_AK8975::update(void)
{
	uint8_t buf[6];
	bool ready = false;

	hal.scheduler.suspend_timer_procs();
	if (_read_reg(AK8975_ST1) & 0x01)	//data is ready
	{
		_read_reg(AK8975_HXL, buf, 6);
		ready = true;
	}
	hal.scheduler.resume_timer_procs();

	if (!ready)return;
#define int16_val(v, idx) ((int16_t)(((uint16_t)v[2*idx+1] << 8) | v[2*idx]))

	_mag_raw.x = int16_val(buf, 1);
	_mag_raw.y = int16_val(buf, 0);
	_mag_raw.z = int16_val(buf, 2);
	_mag_raw *= 0.3f;

	_mag_raw = _mag_filter.apply(_mag_raw);
	_rotate_and_offset(_mag_raw);
	set_available(true);
	
}

void Compass_AK8975::start(void)
{
	_write_reg(AK8975_CNTL, AK8975_MODE_SignalMeasure);
}







uint8_t Compass_AK8975::_read_reg(uint8_t reg)
{
	uint8_t res;
	_cs_pin = 0;
	res = _spi.read_reg(reg);
	_cs_pin = 1;
	return res;
}
void Compass_AK8975::_read_reg(uint8_t reg, uint8_t * pbuf, uint8_t len)
{
	_cs_pin = 0;
	_spi.read_reg(reg, pbuf, len);
	_cs_pin = 1;

}
void Compass_AK8975::_write_reg(uint8_t reg, uint8_t val)
{
	_cs_pin = 0;
	_spi.write_reg(reg, val);
	_cs_pin = 1;
}

