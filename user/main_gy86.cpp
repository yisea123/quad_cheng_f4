#include "sys.h"
#include "usart.h"

#include "HAL_F4.h"
#include "SPIFLASH_W25X.h"

#include "mpu6050.h"
#include "ms5611.h"
#include "hmc5883.h"





using namespace std;



//usart.c,retargeting.c	
//RAM 8368 = 4kheap + 4kstack + 176
//ROM	1876

//stdio.h printf
//RAM 8368 = 4kheap + 4kstack + 176
//ROM 2136


//F4Lib
//ROM 	3024 =	
//RAM	8692



//iostream	 7k RAM + 25k ROM
//RAM 15 928 = 8368 + 7560		
//ROM 27 788 = 2136 + 25 652




int main(void)
{
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init();delay_ms(500);
	usart1_init(500000);	
	printf("program start\r\n");
	
	hal.Setup();
	
	mpu6050_init();
	ms5611_init();
	hmc5883_init();
	
	printf("end\r\n");

	
		
	
	while(1)
	{
//		ms5611_start_T();
//		delay_ms(500);
//		
//		int16_t temp = ms5611_convertTemperature(ms5611_read_Adc_T());
//		printf("temp = %d\r\n",temp);
		
	}

}





