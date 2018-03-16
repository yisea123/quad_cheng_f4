#ifndef	_HAL_F4_H_
#define _HAL_F4_H_

#include "sys.h"


#include "HAL.h"			//³éÏó½Ó¿Ú
#include "RCCX.h"
#include "GPIO.h"			
#include "USART.h"
#include "TIMER.h"
#include "ADC.h"
#include "SPIDriver.h"
#include "Parameter.h"
#include "Scheduler.h"



#define RCC_GPIOALL 	(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC\
						|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF\
						|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI\
						|RCC_AHB1Periph_GPIOJ|RCC_AHB1Periph_GPIOK)



class HAL_F4:public HAL
{
public:
	HAL_F4() :
		spi1(SPI1),
		spi2(SPI2),
		spi3(SPI3),

		usart1(USART1),
		usart2(USART2),
		usart3(USART3),
		uart4(UART4),
		uart5(UART5),

		tim1(TIM1),
		tim3(TIM3),
		tim12(TIM12),

		led1(PB13),
		led2(PB12),
		LEDR(PC0),
		LEDG(PC1),
		LEDB(PC2),

		console(usart1)
	{

	}

	virtual void Setup();
	virtual void pwm_write(uint8_t idx,uint16_t pwm);

		
	Scheduler scheduler;

	SPIDriver spi1;
	SPIDriver spi2;
	SPIDriver spi3;

	USART usart1;
	USART usart2;
	USART usart3;
	USART uart4;
	USART uart5;

	TIMER tim1;
	TIMER tim3;
	TIMER tim12;



	GPIO led1;
	GPIO led2;

	GPIO LEDR;
	GPIO LEDG;
	GPIO LEDB;


	USART& console;
	
};


extern HAL_F4 hal;

#endif

