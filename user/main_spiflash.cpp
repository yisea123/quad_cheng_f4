#include "sys.h"
#include "delay.h"
#include "usart.h"



#include "HAL_F4.h"
#include "SPIFLASH_W25X.h"

using namespace std;

const HAL_F4& hal = HAL_F4_Board;

u8 TEXT_Buffer[100]={"Explorer STM32F4 SPI TEST"};

	
	
	
	
SPIFLASH_W25X w25x(hal.SPI_W25X);


int main(void)
{

	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();delay_ms(500);
	usart1_init(500000);
	
	
	cout<<"start"<<endl;

	
	hal.Setup();
	
	w25x.init();
		
	while(w25x.id()!=W25X40)								//¼ì²â²»µ½W25X40B
	{
		delay_ms(1000);
		cout<<"check flash fail!!"<<endl;
	}
	cout<<"check flash!!"<<endl;
	cout<<"size"<<w25x.flash_size()<<endl;
	
    while (1)
    {
		while(!w25x.is_ready()){};	
		uint32_t time = micros();
		w25x.erase(0);
		time = micros()-time;
		cout<<"erase sector time="<<time<<endl;
		while(!w25x.is_ready()){};	
		time = micros();
		w25x.write_in_sector(0,TEXT_Buffer,sizeof(TEXT_Buffer));
		time = micros()-time;
		printf("write secrtor time=%d\r\n",time);	
		
			
		w25x.read(0,TEXT_Buffer,sizeof(TEXT_Buffer));
		puts((char*)TEXT_Buffer);
		break;
		
	}
	
	while(1);
	
	
}
