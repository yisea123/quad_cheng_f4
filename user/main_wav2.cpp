#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Vector3f.h"
#include "GPIO.h"

#include "sdio_sdcard.h"
#include "stdio.h"
#include <iostream>


#include "ff.h"
#include "malloc.h"
#include "Audio_Wav.h"
#include "AudioPlayer.h"
#include "icm20602.h"


using namespace std;








__align(32) uint8_t audio_buf0[WAV_I2S_TX_DMA_BUFSIZE];
__align(32) uint8_t audio_buf1[WAV_I2S_TX_DMA_BUFSIZE];
__align(32) uint8_t audio_tbuf[WAV_I2S_TX_DMA_BUFSIZE];

AudioPlayer player(audio_buf0,audio_buf1,audio_tbuf);



//DMA1_Stream4 中断服务函数
extern "C" void DMA1_Stream4_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4)==SET)//传输完成标志
	{
		
		DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TCIF4);
		player.i2s_tx_callback();
	}
}



int main(void)
{
	


	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();
	usart1_init(500000);
	delay_ms(500);
	
		

	printf("program start\r\n");
	
	while(SD_Init())//检测不到SD卡
	{
		delay_ms(500);
	}
	delay_ms(50);
	
	
		
	FATFS fs_sd;
	if(!f_mount(&fs_sd,"0:",1))
	{
		cout <<"f_mount finish"<<endl;
	}
	else	
	{
		cout <<"f_mount fail"<<endl;
		while(1);
	}
	
	
	
	
	const char *path1 = "0:/old money.wav";
	const char *path2 = "0:/yesterday once more.wav";
	const char *path3 = "0:/bayan.wav";
	uint32_t res=0;
	Audio_Wav wav;
	//==========================================
	res = wav.open(path1);
	if(!res)
	{
		cout <<"open file finish"<<endl;
	}
	else
	{
		cout <<"open file fail:"<<(uint32_t)res<<endl;
		while(1);
	}
	cout << wav<<endl;
	
	player.load(&wav);		//添加解码器
//	play.open();
	player.start();			//开始
	while(1)
	{
		if(!player.run())	//播放结束
		{
			player.seek(0);	//
			player.start();
		
		}
	}
	
	
	cout<<"stop"<<endl;


	while (1)
	{}
}





