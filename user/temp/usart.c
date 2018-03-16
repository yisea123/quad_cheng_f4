#include "usart.h"



void myputc(uint8_t c)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	USART_SendData(USART1,c);
}

void myputbuf(uint8_t *buffer, uint16_t len)
{
	uint16_t i;
	for ( i = 0; i < len; i++)
	{
		myputc(*buffer++);
	}
}
void myputs(char *s)
{
	while(*s)
	{
		myputc(*s++);
	}
	myputc('\r');
	myputc('\n');
}

