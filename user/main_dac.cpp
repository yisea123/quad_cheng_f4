#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Vector3f.h"
#include "GPIO.h"

#include "sdio_sdcard.h"
#include "stdio.h"
#include <iostream>


#include "rtc.h"

using namespace std;

GPIO led(PG15);


void Dac1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能 PA 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能 DAC 时钟
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;		//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化 GPIO
	
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;							//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;			//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;			//输出缓存关闭
	DAC_Init(DAC_Channel_1,&DAC_InitType); 								//初始化 DAC 通道 1
	
	DAC_Cmd(DAC_Channel_1, ENABLE); 			//使能 DAC 通道 1
	DAC_SetChannel1Data(DAC_Align_12b_R, 0); 	//12 位右对齐数据格式
}
//设置通道 1 输出电压
//vol:0~3300,代表 0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp = vol;
	temp /= 1000;
	temp  =temp*4096/3.3f;
	
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12 位右对齐数据格式
}

















int main(void)
{	
	uint32_t time;
	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();delay_ms(500);
	usart1_init(500000);
	led.init();
	Dac1_Init();
	
	Dac1_Set_Vol(2000);
	while (1)
	{
		
		
	}
}

