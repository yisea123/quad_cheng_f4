#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Vector3f.h"
#include "GPIO.h"

#include "sdio_sdcard.h"
#include "stdio.h"
#include <iostream>


#include "ff.h"






using namespace std;
//通过串口打印SD卡相关信息
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:	printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:			printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//制造商ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//卡相对地址
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//显示容量
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//显示块大小
}


int main(void)
{
	FATFS fs_sd;
	FIL file;
//	TCHAR buffer[150];
	uint8_t buf[1024];
	UINT	sz;
	uint32_t time;
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





