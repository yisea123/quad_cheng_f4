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




//通用定时器 3 中断初始化
//arr：自动重装值。 psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器 3!
void TIM3_Int_Init(u16 psc,u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 			//使能 TIM3 时钟
	
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc-1; 				//定时器分频
	TIM_TimeBaseInitStructure.TIM_Period = arr-1; 					//自动重装载值
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//初始化定时器 TIM3
	
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 						//允许定时器 3 更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 					//定时器 3 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 		//抢占优先级 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 			//响应优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);									// 初始化 NVIC
	
	TIM_Cmd(TIM3,ENABLE); 											//使能定时器 3
}



	


GPIO led(PG15);
uint16_t cnt = 0;

int main(void)
{	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();delay_ms(500);
	usart1_init(500000);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	led.init();
	led = 1;
	
	TIM3_Int_Init(84,1000);

	while(1)
	{

	}
}



//定时器 3 中断服务函数
extern "C" void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		if(++cnt == 1000)
		{
			cnt = 0;
			led = !led;
		}
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //清除中断标志位
	}
	
}	






	


