#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Vector3f.h"
#include "GPIO.h"

#include "sdio_sdcard.h"
#include "stdio.h"
#include <iostream>




using namespace std;

GPIO led(PG15);


void My_DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,uint32_t chx,uint32_t par,uint32_t mar,u16 ndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	if((uint32_t)DMA_Streamx>(uint32_t)DMA2)//得到当前 stream 是属于 DMA2 还是 DMA1
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2 时钟使能
	else
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1 时钟使能

	DMA_DeInit(DMA_Streamx);
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待 DMA 可配置
		
		
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = chx; 										//通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;			
		//DMA 外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;								//DMA 存储器 0 地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;						//存储器到外设模式
		
	DMA_InitStructure.DMA_BufferSize = ndtr;									//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//存储器增量模式
		
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据长度:8 位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//存储器数据长度:8 位
		
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								// 使用普通模式 (还有一种循环模式）
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//中等优先级
		
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;						//FIFO 模式禁止	？？
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;				//FIFO 阈值
		
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;					//存储器突发单次传输	？？？
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;			//外设突发单次传输
	
	DMA_Init(DMA_Streamx, &DMA_InitStructure);									//初始化 DMA Stream
}



//开启一次 DMA 传输
//DMA_Streamx:DMA 数据流,DMA1_Stream0~7/DMA2_Stream0~7
//ndtr:数据传输量
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE); 							//关闭 DMA 传输
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){} 		//确保 DMA 可以被设置
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr); 				//数据传输量
	DMA_Cmd(DMA_Streamx, ENABLE);							//开启 DMA 传输
}



#define SEND_BUF_SIZE 8200
u8 SendBuff[SEND_BUF_SIZE]; //发送数据缓冲区
const u8 TEXT_TO_SEND[]="ALIENTEK Explorer STM32F4 DMA 串口实验";



int main(void)
{	
	uint16_t i;
	uint8_t t=0,j,mask=0;
	uint32_t pro,time;
	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();delay_ms(500);
	usart1_init(500000);
	led.mode_out();
	
	
	
	j=sizeof(TEXT_TO_SEND);
	for(i=0;i<SEND_BUF_SIZE;i++)//填充 ASCII 字符集数据
	{
		if(t>=j)//加入换行符
		{
			if(mask)
			{
				SendBuff[i]=0x0a;
				t=0;
			}else
			{
				SendBuff[i]=0x0d;
				mask++;
			}
		}
		else//复制 TEXT_TO_SEND 语句
		{
			mask=0;
			SendBuff[i]=TEXT_TO_SEND[t];
			t++;
		}
	}
	
	
	My_DMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);
	
	
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口 1 的 DMA 发送
	MYDMA_Enable(DMA2_Stream7,SEND_BUF_SIZE); //开始一次 DMA 传输！
	time = micros();
	
	while (1)
	{
		
		if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待 DMA2_Steam7 传输完成
		{
			DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清传输完成标志
			cout<<"time:"<<micros() - time<<endl;
			break;
		}
		else
		{
			pro=DMA_GetCurrDataCounter(DMA2_Stream7);//得到当前剩余数据数
			pro=1-pro/SEND_BUF_SIZE;//得到百分比
			pro*=100; //扩大 100 倍
		}
		
		
	}
	
	cout<<"DMA Finish"<<endl;
	while(1);
}

