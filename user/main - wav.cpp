#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Vector3f.h"
#include "GPIO.h"

#include "sdio_sdcard.h"
#include "stdio.h"
#include <iostream>


#include "ff.h"
#include "wavplay.h" 
#include "audio.h"



using namespace std;



__audiodev audiodev;


//I2S2 TX DMA 配置
//设置为双缓冲模式,并开启 DMA 传输完成中断
//buf0:M0AR 地址.
//buf1:M1AR 地址.
//num:每次传输数据量
void I2S2_TX_DMA_Init(uint8_t* buf0,uint8_t *buf1,u16 num)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1 时钟使能
	DMA_DeInit(DMA1_Stream4);
	
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){}//等待可配置
		
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0; 								//通道 0 SPI2_TX 通道
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SPI2->DR;					//外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)buf0;							//DMA 存储器 0 地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;						//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = num;										//数据传输量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//外设数据长度:16 位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//存储器数据长度： 16 位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								// 使用循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//高优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; 						//不使用 FIFO 模式
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;					//外设突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;			//存储器突发单次传输
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);									//初始化 DMA Stream
		
	DMA_DoubleBufferModeConfig(DMA1_Stream4,(u32)buf1,DMA_Memory_0);			//双缓冲模式配置
	DMA_DoubleBufferModeCmd(DMA1_Stream4,ENABLE);								//双缓冲模式开启
	
	DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);								//开启传输完成中断
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;				//抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;						//响应优先级 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能中断
	NVIC_Init(&NVIC_InitStructure);//配置
}





void I2S2_Init(uint16_t I2S_DataFormat,uint32_t I2S_AudioFreq)
{
	I2S_InitTypeDef I2S_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	
	//IO初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);					//GPIOB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;		//PB3~5复用功能输出	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;						//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;						//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);									//初始化
	
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_SPI2);  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能 SPI2 时钟
	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE); //复位 SPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//结束复位
	
	
	
	I2S_InitStructure.I2S_Mode=I2S_Mode_MasterTx;						//IIS 模式 主发送
	I2S_InitStructure.I2S_Standard=I2S_Standard_Phillips;				//IIS 标准Phillips
	I2S_InitStructure.I2S_DataFormat=I2S_DataFormat;				//IIS 数据长度	16位
	I2S_InitStructure.I2S_MCLKOutput=I2S_MCLKOutput_Disable;			//主时钟输出禁止
	I2S_InitStructure.I2S_AudioFreq=I2S_AudioFreq;						//IIS 频率设置
	I2S_InitStructure.I2S_CPOL=I2S_CPOL_Low;							//空闲状态时钟电平
	I2S_Init(SPI2,&I2S_InitStructure);									//初始化 IIS
	
	SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);//SPI2 TX DMA 请求使能.
	I2S_Cmd(SPI2,ENABLE);//SPI2 I2S EN 使能.
	
	
	RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
    RCC_PLLI2SCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY) == SET);
} 



//I2S DMA 回调函数指针
void (*i2s_tx_callback)(void); //TX 回调函数

//DMA1_Stream4 中断服务函数
extern "C" void DMA1_Stream4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4)==SET)//传输完成标志
	{
		
		DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TCIF4);
		i2s_tx_callback(); //执行回调函数,读取数据等操作在这里面处理
	}
}



__align(32) uint8_t audio_buf1[WAV_I2S_TX_DMA_BUFSIZE];
__align(32) uint8_t audio_buf2[WAV_I2S_TX_DMA_BUFSIZE];
__align(32) uint8_t audio_tbuf[WAV_I2S_TX_DMA_BUFSIZE];
FIL		audio_file;



int main(void)
{
	FATFS fs_sd;
	uint8_t result=0;
//	FIL file;
//	TCHAR buffer[150];
//	uint8_t buf[1024];
//	UINT	sz;
//	UINT	i;
//	uint32_t time;
	__wavctrl wavOldMoney;
	u32 fillnum; 
	//DWORD vsn;
	
	
	int res;
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();
	usart1_init(500000);
	delay_ms(500);
		
	GPIO led(PG15);
	led.init(GPIO::Mode_In);

	printf("program start\r\n");
	
	while(SD_Init())//检测不到SD卡
	{
		led = !led;
		delay_ms(500);
	}
		
	if(!f_mount(&fs_sd,"0:",1))
	{
		cout <<"f_mount result:"<< res<<endl;
	}
	else	
	{
		cout <<"f_mount fail"<<endl;
		while(1);
	}
	
	
	audiodev.file=&audio_file;
	audiodev.i2sbuf1=audio_buf1;
	audiodev.i2sbuf2=audio_buf2;
	audiodev.tbuf=audio_tbuf;
	
	result = wav_open(audiodev.file,"old money.wav",&wavOldMoney);
	if(!result)
	{
		cout <<"open file finish:"<<endl;
	}
	else
	{
		cout <<"open fail fail:"<<(uint32_t)result<<endl;
		while(1);
	}
	
	
	I2S2_Init(I2S_DataFormat_16b,I2S_AudioFreq_44k);								//初始化I2S,飞利浦16位
	I2S2_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); 	//配置TX DMA
	i2s_tx_callback = wav_i2s_dma_tx_callback;
	
	audio_stop();
	res = f_open(audiodev.file,"old money.wav",FA_READ);
	
	if(res == 0)
	{
		f_lseek(audiodev.file, wavOldMoney.datastart);//跳过文件头
		//printf("start:%x\r\n", f_tell(audiodev.file));
		fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,wavOldMoney.bps);
		fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,wavOldMoney.bps);

		cout<<"start"<<endl;
		audio_start();  
		

		while(res == 0)
		{
			while(wavtransferend==0);//等待wav传输完成; 
			wavtransferend=0;		//清除完成标志
			
			
			if(fillnum!=WAV_I2S_TX_DMA_BUFSIZE)//播放结束?
			{
				cout<<"stop"<<endl;
				audio_stop(); 
				f_close(audiodev.file);
				break;
			} 
						
						
			if(wavwitchbuf)
				fillnum=wav_buffill(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,wavOldMoney.bps);//填充buf2
			else 
				fillnum=wav_buffill(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,wavOldMoney.bps);//填充buf1
			
			//cout<<"load"<<endl;
		}
	
	}
	else
	{
		cout<<"open file fail"<<endl;
	}
	
	
	
	
	
	

	
	
	
	
	
	

	
//	if(FR_OK == f_getlabel("0:",label,&vsn))
//	{
//		cout<<"label:"<<label<<endl;
//		cout<<"vsn:"<<vsn<<endl;
//	}	
//	else
//	{
//		cout<<"get label error"<<endl;
//	}
	
	
//	if(!f_open(&file,"文本文档.txt",FA_OPEN_EXISTING|FA_READ))
//	{	
//		cout << f_gets(buffer,150,&file)<<endl;
//		
//	}
//	else
//	{
//		cout<<"open file error"<<endl;
//	}
	
	
//	if(!f_open(&file,"testfile.dat",FA_CREATE_ALWAYS|FA_WRITE))
//	{	
//		time = micros();
//		f_write(&file,buf,1024,&sz);
//		
//		cout<<"time write="<<micros()-time<<endl;
//		
//		f_close(&file);
//		
//		cout << "writing finish:"<<sz<<endl;
//		
//		
//	}
//	else
//	{
//		cout<<"open file error"<<endl;
//	}
	
	
//	if(!f_mkdir("新建文件夹"))
//	{
//		cout<<"mkdir finish"<<endl;
//	}
//	else
//	{
//		cout<<"mkdir fail"<<endl;
//	}
//	
//	
//	if(!f_open(&file,"//新建文件夹//文本文档.txt",FA_OPEN_ALWAYS|FA_WRITE))
//	{	
//		f_puts("文本文档\r\n",&file);
//		f_close(&file);
//		cout << "writing in dirction finish"<<endl;
//		
//	}
//	else
//	{
//		cout<<"open file error"<<endl;
//	}
	
	
	
	
	
	
	
	
//	f_open(&file,"mytxt.txt",FA_OPEN_ALWAYS|FA_WRITE);
//	f_puts("test\r\n",&file);
//	f_close(&file);
//	cout <<"writing finish\r\n"<<endl;

	
	
	
	
	while (1)
	{}
}





