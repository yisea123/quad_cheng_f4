#include "SPI.h"






void SPI::write(const void *buf,uint16_t len)	const 
{
	uint8_t *p = (uint8_t *)buf;
	for(uint16_t i=0;i<len;i++)
	{
		write(*p++);
	}
}


void SPI::read(void *buf,uint16_t len)	const 
{
	uint8_t *p = (uint8_t *)buf;
	for(uint16_t i=0;i<len;i++)
	{
		*p++ = read();
	}
}








