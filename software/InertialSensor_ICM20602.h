#ifndef _INERTIALSENSOR_ICM20602_H_
#define _INERTIALSENSOR_ICM20602_H_
#include "sys.h"
#include "HAL_F4.h"
#include "InertialSensor.h"
#include "LowPassFilter2p.h"



//加速度量程
#define ICM20_ACCEL_FS_2G			(0<<3)
#define ICM20_ACCEL_FS_4G			(1<<3)
#define ICM20_ACCEL_FS_8G			(2<<3)
#define ICM20_ACCEL_FS_16G			(3<<3)
//角速度量程
#define ICM20_GYRO_FS_250			(0<<3)
#define ICM20_GYRO_FS_500			(1<<3)
#define ICM20_GYRO_FS_1000			(2<<3)
#define ICM20_GYRO_FS_2000			(3<<3)
//CONFIG DPF
#define DLPF_BW_250         	0x00	//Rate=8k
#define DLPF_BW_176         	0x01
#define DLPF_BW_92          	0x02
#define DLPF_BW_41          	0x03
#define DLPF_BW_20          	0x04
#define DLPF_BW_10          	0x05
#define DLPF_BW_5           	0x06
#define DLPF_BW_328           	0x06	//Rate=8k
//ACCEL_CONFIG2
#define ACCEL_AVER_4         	(0x00<<4)	//Rate=8k
#define ACCEL_AVER_8			(0x01<<4)
#define ACCEL_AVER_16			(0x02<<4)
#define ACCEL_AVER_32			(0x03<<4)
//ACCEL_DLPF
#define ACCEL_DLPF_BW_218         	0x00	
//#define ACCEL_DLPF_BW_218         	0x01
#define ACCEL_DLPF_BW_99          	0x02
#define ACCEL_DLPF_BW_44          	0x03
#define ACCEL_DLPF_BW_21          	0x04
#define ACCEL_DLPF_BW_10          	0x05
#define ACCEL_DLPF_BW_5           	0x06
#define ACCEL_DLPF_BW_420           0x06	



class InertialSensor_ICM20602:public InertialSensor
{
public:

	InertialSensor_ICM20602(SPIDriver& spi, GPIO cs_pin) :
		_spi(spi),
		_cs_pin(cs_pin),
		_accel_filter_x(400, 30),
		_accel_filter_y(400, 30),
		_accel_filter_z(400, 30),
		_gyro_filter_x(400, 30),
		_gyro_filter_y(400, 30),
		_gyro_filter_z(400, 30),
		_accel_sensor_scale(1.0f),
		_gyro_sensor_scale(1.0f)
	{ }

	virtual bool update();
	virtual bool accel_available() {
		return _accel_available;
	}
	virtual bool gyro_available() {
		return _gyro_available;
	}


	void init(void);
	void set_accel_fullscale(uint8_t fs);
	void set_gyro_fullscale(uint8_t fs);
	int16_t product_id(void)const { return _product_id; }
private:
	bool _init_sensor(void);
	void _poll_data(void);

	uint8_t _read_reg(uint8_t reg);
	void _read_reg(uint8_t reg, uint8_t *pbuf, uint8_t len);
	void _write_reg(uint8_t reg, uint8_t val);

	SPIDriver &_spi;
	GPIO _cs_pin;

	LowPassFilter2p _accel_filter_x;
	LowPassFilter2p _accel_filter_y;
	LowPassFilter2p _accel_filter_z;
	LowPassFilter2p _gyro_filter_x;
	LowPassFilter2p _gyro_filter_y;
	LowPassFilter2p _gyro_filter_z;

	Vector3f _accel_raw;
	Vector3f _gyro_raw;

	Vector3f _accel_filtered;
	Vector3f _gyro_filtered;

	float _accel_sensor_scale;
	float _gyro_sensor_scale;

	uint8_t _product_id;
};

#endif

