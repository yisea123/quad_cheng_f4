#include "Barometer.h"
#include "QMath.h"



void Barometer::calibrate()
{
	float ground_pressure = 0.0f;
	float ground_temperature = 0.0f;

	_alt_offset = 0;
	

	//读取地面气压
	while (ground_pressure == 0)
	{
		for (uint8_t t=0;t<10;t++)
		{
			delay_ms(15);
			update();
		}
		
		read();
		ground_pressure = get_pressure();
		ground_temperature = get_temperature();
	}

	for (uint8_t i = 0; i < 5; i++)
	{
		for (uint8_t t = 0; t < 8; t++)
		{
			update();
			delay_ms(15);
		}
		read();
		ground_pressure = (ground_pressure*0.8f) + 0.2f*get_pressure();
		ground_temperature = (ground_temperature*0.8f) + 0.2f*get_temperature();
	}

	_ground_pressure = ground_pressure;
	_ground_temperature = ground_temperature;

}

void Barometer::update_calibration(void)
{
	_ground_pressure = get_pressure();
	_ground_temperature = get_temperature();
}
float Barometer::get_altitude(void)
{
	if (_ground_pressure == 0){
		return 0;
	}

	//没有新信息
	if (_last_altitude_t == _last_update){
		return _altitude + _alt_offset;
	}

	//更新气压和高度
	float pressure = get_pressure();
	float alt = get_altitude_difference(_ground_pressure, pressure);

	_last_altitude_t = _last_update;
	
	if (isnan(alt) || isinf(alt))	{
		_flags.alt_ok = false;
	}else{
		_altitude = alt;
		_flags.alt_ok = true;
	}


	_climb_rate_filter.update(_altitude, _last_update);

	return _altitude + _alt_offset;

}
float Barometer::get_altitude_difference(float base_pressure, float pressure) const
{
	float ret;
	float scaling = pressure / base_pressure;
	float temp = _ground_temperature + 273.15f;

	// This is an exact calculation that is within +-2.5m of the standard atmosphere tables
	// in the troposphere (up to 11,000 m amsl).
	ret = 153.8462f * temp * (1.0f - expf(0.190259f * logf(scaling)));

	return ret;
}


float Barometer::get_climb_rate(void)
{
	return _climb_rate_filter.slope()*1.0e3f;
}
