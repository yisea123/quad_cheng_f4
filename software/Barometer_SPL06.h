#ifndef _BAROMETER_SPL06_H_
#define _BAROMETER_SPL06_H_

#include "HAL_F4.h"
#include "Barometer.h"

//气压测量速率(sample/sec),Background 模式使用
#define  PM_RATE_1			(0<<4)		//1 measurements pr. sec.
#define  PM_RATE_2			(1<<4)		//2 measurements pr. sec.
#define  PM_RATE_4			(2<<4)		//4 measurements pr. sec.			
#define  PM_RATE_8			(3<<4)		//8 measurements pr. sec.
#define  PM_RATE_16			(4<<4)		//16 measurements pr. sec.
#define  PM_RATE_32			(5<<4)		//32 measurements pr. sec.
#define  PM_RATE_64			(6<<4)		//64 measurements pr. sec.
#define  PM_RATE_128		(7<<4)		//128 measurements pr. sec.

//气压重采样速率(times),Background 模式使用
#define PM_PRC_1			0		//Sigle			kP=524288	,3.6ms
#define PM_PRC_2			1		//2 times		kP=1572864	,5.2ms
#define PM_PRC_4			2		//4 times		kP=3670016	,8.4ms
#define PM_PRC_8			3		//8 times		kP=7864320	,14.8ms
#define PM_PRC_16			4		//16 times		kP=253952	,27.6ms
#define PM_PRC_32			5		//32 times		kP=516096	,53.2ms
#define PM_PRC_64			6		//64 times		kP=1040384	,104.4ms
#define PM_PRC_128			7		//128 times		kP=2088960	,206.8ms

//温度测量速率(sample/sec),Background 模式使用
#define  TMP_RATE_1			(0<<4)		//1 measurements pr. sec.
#define  TMP_RATE_2			(1<<4)		//2 measurements pr. sec.
#define  TMP_RATE_4			(2<<4)		//4 measurements pr. sec.			
#define  TMP_RATE_8			(3<<4)		//8 measurements pr. sec.
#define  TMP_RATE_16		(4<<4)		//16 measurements pr. sec.
#define  TMP_RATE_32		(5<<4)		//32 measurements pr. sec.
#define  TMP_RATE_64		(6<<4)		//64 measurements pr. sec.
#define  TMP_RATE_128		(7<<4)		//128 measurements pr. sec.

//温度重采样速率(times),Background 模式使用
#define TMP_PRC_1			0		//Sigle
#define TMP_PRC_2			1		//2 times
#define TMP_PRC_4			2		//4 times
#define TMP_PRC_8			3		//8 times
#define TMP_PRC_16			4		//16 times
#define TMP_PRC_32			5		//32 times
#define TMP_PRC_64			6		//64 times
#define TMP_PRC_128			7		//128 times

//SPL06_MEAS_CFG
#define MEAS_COEF_RDY		0x80
#define MEAS_SENSOR_RDY		0x40		//传感器初始化完成
#define MEAS_TMP_RDY		0x20		//有新的温度数据
#define MEAS_PRS_RDY		0x10		//有新的气压数据

#define MEAS_CTRL_Standby				0x00		//空闲模式
#define MEAS_CTRL_PressMeasure			0x01	//单次气压测量
#define MEAS_CTRL_TempMeasure			0x02	//单次温度测量
#define MEAS_CTRL_ContinuousPress		0x05	//连续气压测量
#define MEAS_CTRL_ContinuousTemp		0x06	//连续温度测量
#define MEAS_CTRL_ContinuousPressTemp	0x07	//连续气压温度测量
//FIFO_STS
#define SPL06_FIFO_FULL		0x02
#define SPL06_FIFO_EMPTY	0x01
//INT_STS
#define SPL06_INT_FIFO_FULL		0x04
#define SPL06_INT_TMP			0x02
#define SPL06_INT_PRS			0x01
//CFG_REG
#define SPL06_CFG_T_SHIFT	0x08	//oversampling times>8时必须使用
#define SPL06_CFG_P_SHIFT	0x04

class Barometer_SPL06:public Barometer
{
public:
	Barometer_SPL06(SPIDriver& spi, GPIO cs_pin) :
		_spi(spi),
		_cs_pin(cs_pin),
		_updated(false)
	{

	}

	virtual bool init();
	virtual bool read();
	virtual float get_pressure() { return Press; } //in mbar*100 units
	virtual float get_temperature() { return Temp; }

	//100Hz更新
	virtual void update(void);
	uint8_t product_id()const {
		return _product_id;
	}
private:
	void _calculate();
	void start_T(void);
	void start_P(void);

	bool _init_sensor(void);
	void config_temperature(uint8_t rate, uint8_t oversampling);
	void config_pressure(uint8_t rate, uint8_t oversampling);

	int32_t _get_pressure_adc(void);
	int32_t _get_temperature_adc(void);

	uint8_t _read_reg(uint8_t reg);
	void _read_reg(uint8_t reg, uint8_t *pbuf, uint8_t len);
	void _write_reg(uint8_t reg, uint8_t val);

	SPIDriver &_spi;
	GPIO _cs_pin;


	bool _updated;

	uint8_t _tmp_count, _prs_count;
	int64_t _tmp_sum,_prs_sum;

	uint8_t _state;
	uint32_t _timer;

	float Temp;
	float Press;

	float _raw_press;	//气压adc值
	float _raw_temp;	//温度adc值

	uint8_t _product_id;
	int16_t _C0, _C1, _C01, _C11, _C20, _C21, _C30;
	int32_t _C00, _C10;
	float _kT, _kP;

};

#endif

