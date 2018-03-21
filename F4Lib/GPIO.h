#ifndef _F4_GPIO_H_
#define _F4_GPIO_H_

#include "sys.h"

typedef enum
{
	PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,//PAx
	PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,//PBx
	PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15,//PCx
	PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15,//PDx
	PE0,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15,//PEx
	PF0,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15,//PFx
	PG0,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15,//PGx
	PH0,PH1,PH2,PH3,PH4,PH5,PH6,PH7,PH8,PH9,PH10,PH11,PH12,PH13,PH14,PH15,//PHx
	PI0,PI1,PI2,PI3,PI4,PI5,PI6,PI7,PI8,PI9,PI10,PI11,PI12,PI13,PI14,PI15,//PIx
	PJ0,PJ1,PJ2,PJ3,PJ4,PJ5,PJ6,PJ7,PJ8,PJ9,PJ10,PJ11,PJ12,PJ13,PJ14,PJ15,//PJx
	PK0,PK1,PK2,PK3,PK4,PK5,PK6,PK7,PK8,PK9,PK10,PK11,PK12,PK13,PK14,PK15,//PKx
}GPIO_PIN_T;




class GPIO
{
public:
	GPIO(GPIO_PIN_T pin);
	GPIO(GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin):
		_port(GPIOx),_pin(GPIO_Pin){};

	
	//设置为输入模式
	void mode_in(GPIOPuPd_TypeDef pupd		= 	GPIO_PuPd_UP,				//上拉
					GPIOSpeed_TypeDef speed = 	GPIO_Speed_100MHz);
	
	//设置为输出模式
	void mode_out(GPIOOType_TypeDef	otype 	=	GPIO_OType_PP,				//推挽输出
					GPIOSpeed_TypeDef speed = 	GPIO_Speed_100MHz);
	
		
	static void ModeAF(GPIO_PIN_T pin,uint8_t GPIO_AF,
							GPIOOType_TypeDef	otype 	=	GPIO_OType_PP,			//推挽输出
							GPIOPuPd_TypeDef pupd		= 	GPIO_PuPd_UP,			//上拉
							GPIOSpeed_TypeDef speed = 	GPIO_Speed_100MHz);
	static void ModeADC(GPIO_PIN_T pin);
	//写入
	void operator =(uint8_t level)const
	{
		if(level)		GPIO_SetBits(_port,_pin);
		else			GPIO_ResetBits(_port,_pin);
	}
	
	//读取
	operator uint8_t() const
	{
		if(_mode_out)
			return GPIO_ReadOutputDataBit(_port,_pin);
		else
			return GPIO_ReadInputDataBit(_port, _pin);
	}
	
	
private:
	GPIO_TypeDef* const _port;
	const uint32_t 	 _pin;
	bool _mode_out;

	 //禁止取地址
	GPIO* operator&() { return this;};
	const GPIO* operator&() const {return this;};
};


#endif
