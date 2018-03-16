#include "ADC.h"
#include "RCCX.h"
#include "GPIO.h"



void ADCx::Setup(ADC_TypeDef* adcx)
{
	if(adcx == ADC1)
	{
		RCCX.ADC1_.Enable();
	}
	else if(adcx == ADC2)
	{
		RCCX.ADC2_.Enable();
	}
	else if(adcx == ADC3)
	{
		RCCX.ADC3_.Enable();
	}
	else
	{
		return ;
	}
	
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;						//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay =	ADC_TwoSamplingDelay_5Cycles;	//两个采样阶段之间的延迟 5 个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode =	ADC_DMAAccessMode_Disabled; 		//DMA 失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;						//预分频 4 分频,ADCCLK=PCLK2/4=84/4=21Mhz,ADC 时钟最好不要超过 36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);										//初始化
	
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;							//12 位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;									//非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;								//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;										//1 个转换在规则序列中
	ADC_Init(adcx, &ADC_InitStructure);												//ADC 初始化
	
	
	ADC_Cmd(adcx, ENABLE);//开启 AD 转换器

}



void ADCx::init()
{
	
	if(_channel <= 7)
	{
		GPIO::ModeADC((GPIO_PIN_T)((uint16_t)PA0+_channel));
	}
	else if(_channel == 8)
	{
		GPIO::ModeADC(PB0);
	}
	else if(_channel == 9)
	{
		GPIO::ModeADC(PB1);
	}
	else if(_channel <= 15)
	{
		GPIO::ModeADC((GPIO_PIN_T)((uint16_t)PC0+_channel-10));
	}
	else if(_channel == 16)
	{
		ADC_TempSensorVrefintCmd(ENABLE);//使能内部温度传感器
	}
	else if(_channel == 17)
	{
		ADC_VBATCmd(ENABLE);				//内部参考电压
	}

}


void ADCx::start_conv()
{
	ADC_RegularChannelConfig(_adcx, _channel, 1, _time );
	ADC_SoftwareStartConv(_adcx); 							//使能指定的 ADC1 的软件转换启动功能

}




