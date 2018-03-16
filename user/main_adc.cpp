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


//

void My_ADC_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//开启 ADC 和 GPIO 相关时钟和初始化 GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能 GPIOC 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能 ADC1 时钟

	//先初始化 ADC1 通道 0 IO 口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PC0 通道 10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;			//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;		//不带上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//初始化
	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE); //ADC1 复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE); //复位结束
	
	ADC_TempSensorVrefintCmd(ENABLE);//使能内部温度传感器
	
	//初始化通用配置
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;						//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay =	ADC_TwoSamplingDelay_5Cycles;	//两个采样阶段之间的延迟 5 个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode =	ADC_DMAAccessMode_Disabled; 		//DMA 失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;						//预分频 4 分频,ADCCLK=PCLK2/4=84/4=21Mhz,ADC 时钟最好不要超过 36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);										//初始化
	
	//初始化 ADC1 相关参数
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;							//12 位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;									//非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;								//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;										//1 个转换在规则序列中
	ADC_Init(ADC1, &ADC_InitStructure);												//ADC 初始化
	
	//开启 ADC 转换
	ADC_Cmd(ADC1, ENABLE);//开启 AD 转换器
}	
	





	
uint16_t Get_Adc(uint8_t ch)
{
	uint16_t value = 0;

	//设置指定 ADC 的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );
	ADC_SoftwareStartConv(ADC1); 							//使能指定的 ADC1 的软件转换启动功能
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));			//等待转换结束
	return  ADC_GetConversionValue(ADC1); 					//返回最近一次 ADC1 规则组的转换结果

}



int16_t Get_Temprate(void)
{
	uint32_t adcx; 
	int16_t result;
	float                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     temperate;
	
	adcx=Get_Adc(ADC_Channel_16); 			//读取通道 16
	temperate=(float)adcx*(3.3f/4096); 		//电压值
	temperate=(temperate-0.76)/0.0025+25.0f; 	//转换为温度值
	
	
	return temperate*100;
}


int main(void)
{	
	uint32_t time;
	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();delay_ms(500);
	usart1_init(500000);
	led.init();
	
	My_ADC_Init();
	
	while (1)
	{
		delay_ms(500);	
		
//		time = micros();
//		time = micros() - time;
//		cout<<"time="<<time<<endl;
		
		cout<<"adc_value:"<<Get_Adc(10)<<endl;
		cout<<"temp:"<<Get_Temprate()<<endl;
		
	}
}

