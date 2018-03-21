#include "GPIO.h"




GPIO_TypeDef* const GPIO_PORTS[] = { GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH,GPIOI,GPIOJ,GPIOK };

const uint16_t  GPIO_PINS[] = {
	GPIO_Pin_0,	GPIO_Pin_1,	GPIO_Pin_2,		GPIO_Pin_3,		GPIO_Pin_4,		GPIO_Pin_5,		GPIO_Pin_6,		GPIO_Pin_7,
	GPIO_Pin_8,	GPIO_Pin_9,	GPIO_Pin_10,	GPIO_Pin_11,	GPIO_Pin_12,	GPIO_Pin_13,	GPIO_Pin_14,	GPIO_Pin_15 };

const uint16_t  GPIO_PINS_SOURCE[] = {
	GPIO_PinSource0,	GPIO_PinSource1,	GPIO_PinSource2,	GPIO_PinSource3,	GPIO_PinSource4,	GPIO_PinSource5,	GPIO_PinSource6,	GPIO_PinSource7,
	GPIO_PinSource8,	GPIO_PinSource9,	GPIO_PinSource10,	GPIO_PinSource11,	GPIO_PinSource12,	GPIO_PinSource13,	GPIO_PinSource14,	GPIO_PinSource15 };

const uint32_t  GPIO_PINS_CLOCK[] = {
	RCC_AHB1Periph_GPIOA,RCC_AHB1Periph_GPIOB,RCC_AHB1Periph_GPIOC,
	RCC_AHB1Periph_GPIOD,RCC_AHB1Periph_GPIOE,RCC_AHB1Periph_GPIOF,
	RCC_AHB1Periph_GPIOG,RCC_AHB1Periph_GPIOH,RCC_AHB1Periph_GPIOI,
	RCC_AHB1Periph_GPIOJ,RCC_AHB1Periph_GPIOK
};


GPIO::GPIO(GPIO_PIN_T pin):
	_port(GPIO_PORTS[pin>>4]),
	_pin(GPIO_PINS[pin&0x0F])
{
}




void GPIO::mode_in(GPIOPuPd_TypeDef pupd,GPIOSpeed_TypeDef speed)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = _pin;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_InitStructure.GPIO_PuPd = pupd; 
	GPIO_Init(_port, &GPIO_InitStructure);

	_mode_out = false;

}
void GPIO::mode_out(GPIOOType_TypeDef otype,GPIOSpeed_TypeDef speed)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = _pin;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = otype;
	GPIO_InitStructure.GPIO_Speed = speed;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP; 
	GPIO_Init(_port, &GPIO_InitStructure);
	
	_mode_out = true;
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
