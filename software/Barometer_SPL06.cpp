#include "Barometer_SPL06.h"

#include "Ano_DT.h"


#define SP06_PSR_B2		0x00		//气压值
#define SP06_PSR_B1		0x01
#define SP06_PSR_B0		0x02
#define SP06_TMP_B2		0x03		//温度值
#define SP06_TMP_B1		0x04
#define SP06_TMP_B0		0x05




#define SP06_PSR_CFG	0x06		//气压测量配置
#define SP06_TMP_CFG	0x07		//温度测量配置
#define SP06_MEAS_CFG	0x08		//测量模式配置

#define SP06_CFG_REG	0x09
#define SP06_INT_STS	0x0A
#define SP06_FIFO_STS	0x0B

#define SP06_RESET		0x0C
#define SP06_ID			0x0D

#define SP06_COEF		0x10		//0x10-0x21,12个字节
#define SP06_COEF_SRCE	0x28


#define SPL06_ADDRESS	0xEE


/*
SP06_MEAS_CFG：
SENSOR_RDY:		12ms
MEAS_COEF_RDY:	40ms

温度转换速率:	1Hz-128Hz

气压转换速率: 1 -128 Hz
气压转换时间
低功耗:	5ms
标准:	28ms
高精度:	105ms

*/



void Barometer_SPL06::config_temperature(uint8_t rate, uint8_t oversampling)
{
	switch (oversampling)
	{
	case TMP_PRC_1:
		_kT = 524288;
		break;
	case TMP_PRC_2:
		_kT = 1572864;
		break;
	case TMP_PRC_4:
		_kT = 3670016;
		break;
	case TMP_PRC_8:
		_kT = 7864320;
		break;
	case TMP_PRC_16:
		_kT = 253952;
		break;
	case TMP_PRC_32:
		_kT = 516096;
		break;
	case TMP_PRC_64:
		_kT = 1040384;
		break;
	case TMP_PRC_128:
		_kT = 2088960;
		break;
	}
	_kT = 1.0f / _kT;

	_write_reg(SP06_TMP_CFG, rate | oversampling | 0x80);	//温度每秒128次测量一次
	if (oversampling > TMP_PRC_8)
	{
		uint8_t temp = _read_reg(SP06_CFG_REG);
		_write_reg(SP06_CFG_REG, temp | SPL06_CFG_T_SHIFT);
	}

}
void Barometer_SPL06::config_pressure(uint8_t rate, uint8_t oversampling)
{
	switch (oversampling)
	{
	case PM_PRC_1:
		_kP = 524288;
		break;
	case PM_PRC_2:
		_kP = 1572864;
		break;
	case PM_PRC_4:
		_kP = 3670016;
		break;
	case PM_PRC_8:
		_kP = 7864320;
		break;
	case PM_PRC_16:
		_kP = 253952;
		break;
	case PM_PRC_32:
		_kP = 516096;
		break;
	case PM_PRC_64:
		_kP = 1040384;
		break;
	case PM_PRC_128:
		_kP = 2088960;
		break;
	}
	_kP = 1.0f / _kP;
	_write_reg(SP06_PSR_CFG, rate | oversampling);
	if (oversampling > PM_PRC_8)
	{
		uint8_t temp = _read_reg(SP06_CFG_REG);
		_write_reg(SP06_CFG_REG, temp | SPL06_CFG_P_SHIFT);
	}

}
bool Barometer_SPL06::_init_sensor(void)
{
	uint8_t coef[18];
	_cs_pin.mode_out();

	_healthy = false;
	hal.scheduler.suspend_timer_procs();

	_write_reg(SP06_RESET, 0x89);
	delay_ms(100);	//复位后系数准备好需要至少40ms
	//读取id
	_product_id = _read_reg(SP06_ID);
	//读取系数
	_read_reg(SP06_COEF, coef, 18);

	_C0 = ((int16_t)coef[0] << 4) + ((coef[1] & 0xF0) >> 4);
	_C0 = (_C0 & 0x0800) ? (0xF000 | _C0) : _C0;

	_C1 = ((int16_t)(coef[1] & 0x0F) << 8) + coef[2];
	_C1 = (_C1 & 0x0800) ? (0xF000 | _C1) : _C1;

	_C00 = ((int32_t)coef[3] << 12) + ((uint32_t)coef[4] << 4) + (coef[5] >> 4);
	_C00 = (_C00 & 0x080000) ? (0xFFF00000 | _C00) : _C00;

	_C10 = ((int32_t)(coef[5] & 0x0F) << 16) + ((uint32_t)coef[6] << 8) + coef[7];
	_C10 = (_C10 & 0x080000) ? (0xFFF00000 | _C10) : _C10;

	_C01 = ((int16_t)coef[8] << 8) + coef[9];

	_C11 = ((int16_t)coef[10] << 8) + coef[11];
	_C11 = (_C11 & 0x0800) ? (0xF000 | _C11) : _C11;

	_C20 = ((int16_t)coef[12] << 8) + coef[13];
	_C20 = (_C20 & 0x0800) ? (0xF000 | _C20) : _C20;

	_C21 = ((int16_t)coef[14] << 8) + coef[15];
	_C21 = (_C21 & 0x0800) ? (0xF000 | _C21) : _C21;

	_C30 = ((int16_t)coef[16] << 8) + coef[17];
	_C30 = (_C30 & 0x0800) ? (0xF000 | _C30) : _C30;
	

	config_pressure(PM_RATE_1, PM_PRC_64);
	config_temperature(TMP_RATE_1, TMP_PRC_64);


// 	ANO_Printf("id=%d", _product_id);
// 	ANO_Printf("_C0=%d", _C0);
// 	ANO_Printf("_C1=%d", _C1);
// 	ANO_Printf("_C01=%d", _C01);
// 	ANO_Printf("_C11=%d", _C11);
// 	ANO_Printf("_C20=%d", _C20);
// 	ANO_Printf("_C21=%d", _C21);
// 	ANO_Printf("_C30=%d", _C30);
// 	ANO_Printf("_C00=%d", _C00);
// 	ANO_Printf("_C10=%d", _C10);

	hal.scheduler.resume_timer_procs();
	_healthy = true;
	return true;
}
bool Barometer_SPL06::init()
{
	bool res = false;

	//传感器初始化,初始化系数
	res = _init_sensor();

	start_T();		//启动温度转换
	_timer = micros();

	_state = 0;
	Temp = 0;
	Press = 0;

	_tmp_sum = 0;
	_prs_sum = 0;
	_tmp_count = 0;
	_prs_count = 0;


	return res;
}


void Barometer_SPL06::_calculate()
{
	float Traw_src, Praw_src;
	float qua2, qua3;


	Traw_src = _kT * _raw_temp;
	Praw_src = _kP * _raw_press;

	//计算温度
	Temp = 0.5f*_C0 + Traw_src * _C1;

	//计算气压
	qua2 = _C10 + Praw_src * (_C20 + Praw_src* _C30);
	qua3 = Traw_src * Praw_src * (_C11 + Praw_src * _C21);
	Press = _C00 + Praw_src * qua2 + Traw_src * _C01 + qua3;

}
void Barometer_SPL06::update(void)
{
	uint32_t now;
	now = micros();

	if (now - _timer < 10000) {	//10ms更新一次
		return;
	}

	_timer = now;

	if (_state == 0)
	{
		int32_t tmp = _get_temperature_adc();
		if (tmp != 0)
		{
			_tmp_sum += tmp;
			_tmp_count++;
			if (_tmp_count == 32)
			{
				_tmp_sum /= 2;
				_tmp_count = 16;
			}
		}
		_state++;
		start_P();
	}
	else
	{
		int32_t prs = _get_pressure_adc();
		if (prs != 0)
		{
			_prs_sum += prs;
			_prs_count++;
			if (_prs_count == 128) {
				_prs_sum /= 2;
				_prs_count = 64;
			}

			_updated = true;
		}

		_state++;
		if (_state == 5) {
			start_T();
			_state = 0;
		}
		else {
			start_P();
		}
	}

}
bool Barometer_SPL06::read()
{
	bool updated = _updated;
	int64_t tmp, prs;
	uint8_t tmpcount, prscount;
	
	if(updated)
	{	
		//suspen
		tmp = _tmp_sum; _tmp_sum = 0;
		prs = _prs_sum; _prs_sum = 0;
		tmpcount = _tmp_count; _tmp_count = 0;
		prscount = _prs_count; _prs_count = 0;
		//resume
		_updated = false;

		if (tmpcount != 0){
			_raw_temp = (float)tmp / tmpcount;
		}

		if(prscount != 0){
			_raw_press = (float)prs / prscount;
		}
		_pressure_samples = prscount;
		_calculate();
		_last_update = millis();
	}

	return updated;
}



int32_t Barometer_SPL06::_get_pressure_adc(void)
{
	uint8_t buf[3];
	int32_t adc;
	_read_reg(SP06_PSR_B2, buf, 3);
	adc = (int32_t)((buf[0] << 16) + (buf[1] << 8) + buf[2]);
	adc = (adc & 0x800000) ? (0xFF000000 | adc) : adc;
	return adc;
}
int32_t Barometer_SPL06::_get_temperature_adc(void)
{
	uint8_t buf[3];
	int32_t adc;
	_read_reg(SP06_TMP_B2, buf, 3);
	adc = (int32_t)((buf[0] << 16) + (buf[1] << 8) + buf[2]);
	adc = (adc & 0x800000) ? (0xFF000000 | adc) : adc;
	return adc;
}
void Barometer_SPL06::start_T()
{
	_write_reg(SP06_MEAS_CFG, MEAS_CTRL_TempMeasure);
}
void Barometer_SPL06::start_P()
{
	_write_reg(SP06_MEAS_CFG, MEAS_CTRL_PressMeasure);
}



uint8_t Barometer_SPL06::_read_reg(uint8_t reg)
{
	uint8_t res;
	_cs_pin = 0;
	res = _spi.read_reg(reg);
	_cs_pin = 1;
	return res;
}
void Barometer_SPL06::_read_reg(uint8_t reg, uint8_t * pbuf, uint8_t len)
{
	_cs_pin = 0;
	_spi.read_reg(reg, pbuf, len);
	_cs_pin = 1;

}
void Barometer_SPL06::_write_reg(uint8_t reg, uint8_t val)
{
	_cs_pin = 0;
	_spi.write_reg(reg, val);
	_cs_pin = 1;
}

