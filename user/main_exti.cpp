#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Vector3f.h"
#include "GPIO.h"

#include "sdio_sdcard.h"
#include "stdio.h"
#include <iostream>


#include "rtc.h"
#include "w25qxx.h"

using namespace std;




void EXTIX_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	//KEY_Init(); //按键对应的 IO 口初始化
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能 SYSCFG 时钟
	GPIO_InitTypeDef  GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PE0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;			//不带上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//初始化
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能 SYSCFG 时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);//PE0 连接线0
	/* 配置 EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能 LINE0
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置 NVIC
}

GPIO led(PG15);
int main(void)
{	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();delay_ms(500);
	usart1_init(500000);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	led.init();
	led = 1;
	EXTIX_Init();
	
	cout<<"start"<<endl;
	

	while(1)
	{
		//led = !led;
		//delay_ms(1000);
	}
}

//外部中断 0 服务程序
extern "C" void EXTI0_IRQHandler(void)
{

	cout<<"exti0"<<endl;
	led = !led;
	EXTI_ClearITPendingBit(EXTI_Line0); //清除 LINE0 上的中断标志位
}



