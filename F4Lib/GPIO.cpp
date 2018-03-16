#include "GPIO.h"






GPIO::GPIO(GPIO_PIN_T pin):
	_port(GPIO_PORTS[pin>>4]),
	_pin(GPIO_PINS[pin&0x0F])
{
}




void GPIO::mode_in(GPIOPuPd_TypeDef pupd,GPIOSpeed_TypeDef speed)const
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = _pin;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_InitStructure.GPIO_PuPd = pupd; 
	GPIO_Init(_port, &GPIO_InitStructure);
}


void GPIO::mode_out(GPIOOType_TypeDef otype,GPIOSpeed_TypeDef speed)const
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = _pin;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = otype;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP; 
	GPIO_Init(_port, &GPIO_InitStructure);
	
	
}



//打开时钟，IO初始化，引脚映射
void GPIO::ModeAF(GPIO_PIN_T pin,uint8_t GPIO_AF,GPIOOType_TypeDef otype,GPIOPuPd_TypeDef pupd,GPIOSpeed_TypeDef speed)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	uint16_t portn = pin>>4;
	uint16_t pinn = pin&0x0F;
	
	RCC_AHB1PeriphClockCmd(GPIO_PINS_CLOCK[portn],ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_PINS[pinn];	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = otype;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_InitStructure.GPIO_PuPd = pupd; 
	GPIO_Init(GPIO_PORTS[portn], &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIO_PORTS[portn], GPIO_PINS_SOURCE[pinn],GPIO_AF);
	
}

//打开时钟，IO初始化，引脚映射
void GPIO::ModeADC(GPIO_PIN_T pin)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	uint16_t portn = pin>>4;
	uint16_t pinn = pin&0x0F;
	
	RCC_AHB1PeriphClockCmd(GPIO_PINS_CLOCK[portn],ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_PINS[pinn];	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIO_PORTS[portn], &GPIO_InitStructure);
}


//void GPIO::mode_af(uint8_t GPIO_AF,GPIOOType_TypeDef otype,GPIOPuPd_TypeDef pupd,GPIOSpeed_TypeDef speed)const
//{
//	
//	
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = _pin;	 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = otype;
//	GPIO_InitStructure.GPIO_Speed = speed;
//	GPIO_InitStructure.GPIO_PuPd = pupd; 
//	GPIO_Init(_port, &GPIO_InitStructure);
//	
//	GPIO_PinAFConfig(_port,pin_source,GPIO_AF);
//	
//}


//void GPIO::mode_adc(GPIOPuPd_TypeDef pupd,GPIOSpeed_TypeDef speed)const
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = _pin;	 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_Speed = speed;
//	GPIO_InitStructure.GPIO_PuPd = pupd; 
//	GPIO_Init(_port, &GPIO_InitStructure);
//}


