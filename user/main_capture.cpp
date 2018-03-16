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




GPIO led(PG15);
uint16_t cnt = 0;


void TIM4_Capcture_Init()
{


}



//定时器 5 通道 1 输入捕获配置
//arr：自动重装值(TIM5,TIM5 是 32 位的!!) psc：时钟预分频数
void TIM5_CH1_Cap_Init(u16 psc,u32 arr)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef TIM5_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); 	//TIM5 时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能 PORTA 时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 				//GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 		//速度 100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 			//下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 					//初始化 PA0
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); 	//PA0 复用位定时器 5
	
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1; 					//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr-1; 					//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; 					//选择输入端 IC1 映射到 TI1 上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; 		//上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//映射到 TI1 上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; 				//配置输入分频,不分频
	TIM5_ICInitStructure.TIM_ICFilter = 0x0;							//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure); 							//初始化 TIM5 输入捕获参数
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);					//允许更新和捕获中断
	TIM_Cmd(TIM5,ENABLE );												//使能定时器 5
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;				//抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;					//响应优先级 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); 									//根据指定的参数初始化 VIC 寄存器、
}


//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于 32 位定时器来说,1us 计数器加 1,溢出时间:4294 秒)
u8 TIM5CH1_CAPTURE_STA=0; //输入捕获状态
u32 TIM5CH1_CAPTURE_VAL;//输入捕获值(TIM5/TIM5 是 32 位)
//定时器 5 中断服务程序
extern "C" void TIM5_IRQHandler(void)
{	
	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
		{
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)	//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80; 			//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}
				else 
					TIM5CH1_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获 1 发生捕获事件
		{
			if(TIM5CH1_CAPTURE_STA&0X40) //捕获到一个下降沿
			{
				TIM5CH1_CAPTURE_STA|=0X80; 					//标记成功捕获到一次高电平脉宽
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);	//获取当前的捕获值.
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //设置上升沿捕获
			}
			else //还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;//标记捕获到了上升沿
				
				TIM_Cmd(TIM5,DISABLE ); //使能定时器 5
				TIM_SetCounter(TIM5,0); //计数器清空
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);//设置下降沿捕获
				TIM_Cmd(TIM5,ENABLE ); //使能定时器 5
				
			}
		}
	}
	
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}




int main(void)
{	
	long long temp=0;
	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();
	delay_ms(500);
	usart1_init(500000);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	led.init();
	led = 1;
	
	
	TIM5_CH1_Cap_Init(84,0xffffffff);
	cout<<"start"<<endl;
	while(1)
	{
		if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F;
			temp*=0XFFFFFFFF; 				//溢出时间总和
			temp+=TIM5CH1_CAPTURE_VAL; 		//得到总的高电平时间
			printf("HIGH:%lld us\r\n",temp);//打印总的高点平时间
			cout<<"high:"<<temp<<endl;
			TIM5CH1_CAPTURE_STA=0; 			//开启下一次捕获
		}

	}
}






	


