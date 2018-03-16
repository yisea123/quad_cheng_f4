#include "ms5611.h"
#include "ms5611_dev.h"
#include "HAL_F4.h"


static uint16_t  ms5611_prom[8]; //标定值存放


static const I2C_IO& iic_ms5611 = hal.iic_ms5611;




void ms5611_reset()
{	
	iic_ms5611.write(MS561101BA_RESET);
}


void ms5611_readPROM()
{
	uint8_t  i;
	uint8_t buf[16];

	iic_ms5611.read_reg(MS561101BA_PROM_BASE_ADDR ,buf,16);	
	
	uint8_t *p = buf;
	for (i=0;i<8;i++) 
	{	
		
		ms5611_prom[i] = (p[0] << 8) | p[1];
		p +=2;
		printf("C%d=%d\r\n",i,ms5611_prom[i]);
	}	
}


void ms5611_start_T()
{
	iic_ms5611.write(MS561101BA_D2+MS5611Temp_OSR);
}

void ms5611_start_P()
{
	iic_ms5611.write(MS561101BA_D1+MS5611Press_OSR);
}


uint32_t ms5611_read_Adc_T()
{
	uint8_t buf[3];
	uint32_t adct;
	iic_ms5611.read_reg(MS5611BA_READ,buf,3);
	adct = (buf[0] << 16) | (buf[1] << 8) | buf[2];//温度
	return adct;
}
uint32_t ms5611_read_Adc_P()
{
	uint8_t buf[3];
	uint32_t adcp;
	iic_ms5611.read_reg(MS5611BA_READ,buf,3);
	adcp = (buf[0] << 16) | (buf[1] << 8) | buf[2];//温度
	return adcp;
}

int32_t ms5611_convertTemperature(uint32_t adc_T)
{
	int64_t dT = adc_T - ((uint32_t)ms5611_prom[5] << 8);		//与20度的ADC差
	int32_t temperature = 2000 + ((dT * ms5611_prom[6]) >> 23);
	return temperature;
}

//void ms5611_getBaroAlt()
//{
//	
//	static uint8_t baro_start = 0;
//	
//	int32_t off2 = 0, sens2 = 0, delt;
//	int32_t temperature,pressure;
//	int32_t dT;
//	int64_t off;
//	int64_t sens;
//	float alt_3;
//	dT = adc_T - ((uint32_t)ms5611_prom[5] << 8);		//温度差

//	off = ((uint32_t)ms5611_prom[2] << 16) + (((int64_t)dT * ms5611_prom[4]) >> 7);
//	sens = ((uint32_t)ms5611_prom[1] << 15) + (((int64_t)dT * ms5611_prom[3]) >> 8);
//	
//	temperature = 2000 + (((int64_t)dT * ms5611_prom[6]) >> 23);					//温度值*100度

//	if (temperature < 2000)// temperature lower than 20degC 
//	{ 
//			delt = temperature - 2000;
//			delt = delt * delt;
//			off2 = (5 * delt) >> 1;
//			sens2 = (5 * delt) >> 2;
//			if (temperature < -1500) // temperature lower than -15degC
//			{ 
//					delt = temperature + 1500;
//					delt = delt * delt;
//					off2  += 7 * delt;
//					sens2 += (11 * delt) >> 1;
//			}
//	}
//	off  -= off2; 
//	sens -= sens2;
//	pressure = (((adc_P * sens ) >> 21) - off) >> 15;			//气压
//	alt_3 = (101000 - pressure)/1000.0f;
//	pressure = 0.82f *alt_3 * alt_3 *alt_3 + 0.09f *(101000 - pressure)*100.0f ;		//气压->高度
//	
//	ms5611_pressure = pressure;
//	
////	//矫正
////	if(baro_start < 60)
////	{
////		baro_start ++;
////		sensor_baro.height_delta = 0;
////		sensor_baro.height = 0;
////		if(baro_start<10)
////		{
////			baro_Offset = pressure;		//预读取10次
////		}
////		else
////		{
////			baro_Offset +=  FILTER_EASY_COEF(5,0.02)*(pressure - baro_Offset);	//气压低通滤波
////		}
////	}
//	
//	
//	ms5611_temperature += 0.01f *( ( 0.01f *temperature ) - ms5611_temperature );
//		
//}





//void ms5611_update()
//{
//	static uint8_t  step = 1;
//	static uint32_t tpre = 0;
//	uint32_t now;
//	now = micros();
//	if(now - tpre < 10000)
//	{
//		return;
//	}
//	if(!myiic_sem_take_nonblocking())return;
//	tpre = now;
//	if(step)	//读取温度，启动气压转换
//	{
//		adc_T = ms5611_read_Adc_T();		//读取温度
//		//printf("adcT = %d\n",adc_T);
//		ms5611_start_P();				//启动气压转换
//		myiic_sem_give();
//		
//		step = 0;
//	}
//	else		//读取气压，启动温度转换
//	{
//		adc_P = ms5611_read_Adc_P();	//读取气压
//		//printf("adcP = %d\n",adc_P);
//		ms5611_start_T();			//启动温度转换
//		myiic_sem_give();
//		ms5611_getBaroAlt();	//获取气压高度
//		step = 1;
//	}
//	
//	return;
//}




void ms5611_init()
{

	ms5611_reset();
	delay_ms(10);
	ms5611_readPROM();

}

