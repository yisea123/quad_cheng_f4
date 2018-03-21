#include "HAL_F4.h"
#include "SPIFlash.h"




NVIC_InitTypeDef NVIC_InitStructureTable[] = 
{
/*NVIC_IRQChannel|PreemptionPriority|SubPriority|***Cmd*********/

//=========USART====================================
{USART1_IRQn,		1,					1,			ENABLE},
{UART4_IRQn,		0,					1,			ENABLE},
{UART5_IRQn,		1,					2,			ENABLE },
{USART2_IRQn,		1,					2,			ENABLE },


};




/****************************************************************************************************************/
void NVIC_Setup()
{
	uint16_t i;
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	for(i=0;i<sizeof(NVIC_InitStructureTable)/sizeof(NVIC_InitStructureTable[0]);i++)
	{
		NVIC_Init(NVIC_InitStructureTable+i);
	}

}





void HAL_F4::Setup()
{
	NVIC_Setup();
	delay_init();
	
	//RCC
	RCCX.GPIOA_.Enable();
	RCCX.GPIOB_.Enable();
	RCCX.GPIOC_.Enable();
	RCCX.GPIOG_.Enable();

	
	SPIDriver::Setup(SPI1);
	SPIDriver::Setup(SPI3);
	

	USART::Setup(USART1);
	USART::Setup(USART2);
	USART::Setup(USART3);
	USART::Setup(UART4);
	USART::Setup(UART5);

	TIMER::Setup(TIM1);
	TIMER::Setup(TIM3);
	TIMER::Setup(TIM12);
	
	//PWM
	tim3.setup_timebase(84, 2500);
	tim3.setup_pwm1();
	tim3.setup_pwm2();
	tim3.setup_pwm3();
	tim3.setup_pwm4();
	tim3.start();

	tim1.setup_timebase(168, 2500);
	tim1.setup_pwm1();
	tim1.setup_pwm2();
	tim1.setup_pwm3();
	tim1.start();


	tim12.setup_timebase(84, 2500);
	tim12.setup_pwm2();
	tim12.start();

	//板上led
	led1.mode_out();
	led2.mode_out();
	//外置led
	LEDR.mode_out();
	LEDG.mode_out();
	LEDB.mode_out();

//=============================================================
	console.begin(500000);		//调试
	usart2.begin(115200);		//蓝牙?
	usart3.begin(500000);		//
	uart5.begin(500000);		//数传

	//flash接口
 	spi1.set_baudrate_prescaler(SPIDriver::BaudRatePrescaler_8);//
 	spi1.open();
	//传感器接口
	spi3.set_baudrate_prescaler(SPIDriver::BaudRatePrescaler_4);// 84/4=21
	spi3.open();

 	spiflash.init();
 	ParameterBase::Setup();

	//for sbus
	uart4.begin_sbus();

	//for ibus
	//usart2.begin_ibus();
}

void HAL_F4::pwm_write(uint8_t idx, uint16_t pwm)
{
	switch (idx)
	{
	case 0:
		tim1.set_compare3(pwm);
		break;
	case 1:
		tim1.set_compare2(pwm);
		break;
	case 2:
		tim1.set_compare1(pwm);
		break;
	case 3:
		tim3.set_compare4(pwm);
		break;
	case 4:
		tim3.set_compare3(pwm);
		break;
	case 5:
		tim3.set_compare2(pwm);
		break;
	case 6:
		tim3.set_compare1(pwm);
		break;
	case 7:
		tim12.set_compare2(pwm);
		break;
	default:
		break;

	}
}


//====================================================
HAL_F4 hal;






