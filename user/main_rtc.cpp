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












uint8_t My_RTC_Init()
{
	RTC_InitTypeDef RTC_InitStructure;
	uint16_t retry = 0x1FFF;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);		//使能 PWR 时钟
	PWR_BackupAccessCmd(ENABLE);					 		//使能后备寄存器访问
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) !=0x5050)		//未初始化
	{
		//打开时钟
		RCC_LSEConfig(RCC_LSE_ON);							//LSE 开启
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//等待时钟就绪
		{
			retry ++;
			delay_ms(10);
		}
		if(retry==0)return 1;								//LSE开启失败
	
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); 			//设选择 LSE 作为 RTC 时钟
		RCC_RTCCLKCmd(ENABLE); 								//使能 RTC 时钟
		
		
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;				//RTC 异步分频系数(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;				//RTC 同步分频系数(0~7FFF)
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24 小时格式
		RTC_Init(&RTC_InitStructure);							//初始化 RTC 参数
		
		
		RTC_Set_Time(23,59,56,RTC_H12_AM); 				//设置时间
		RTC_Set_Date(14,5,5,1);			 				//设置日期
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//标记已经初始化过了
	}
	return 0;
}



GPIO led(PG15);

int main(void)
{	
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
	uint16_t t;
	
	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();delay_ms(500);
	usart1_init(500000);
	
	led.init();
	
	
	
	My_RTC_Init();
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);//WAKE UP 每秒一次中断
	cout<<"RTC Init Finish"<<endl;
	
	
	
	
	
	
	
	while (1)
	{
		++t;
		if((t%10) == 0)
		{
			RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
			RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			
			
			//printf("sec=%d\r\n",RTC_TimeStruct.RTC_Seconds);
			cout<<(uint32_t)RTC_DateStruct.RTC_Year<<"/"
				<<(uint32_t)RTC_DateStruct.RTC_Month<<"/"
				<<(uint32_t)RTC_DateStruct.RTC_Date<<"--"
				<<(uint32_t)RTC_DateStruct.RTC_WeekDay<<"  "
				<<(uint32_t)RTC_TimeStruct.RTC_Hours<<"/"
				<<(uint32_t)RTC_TimeStruct.RTC_Minutes<<"/"
				<<(uint32_t)RTC_TimeStruct.RTC_Seconds<<endl;
			
		}
		
		delay_ms(10);	
	
	}
}

