#include "I2C_IO.h"



#define I2C_IO_DEBUG	1


void I2C_IO::delay()const
{
	
    uint8_t  i;
	if(fast_mode())	i = 15;		//500k
	else			i = 100;	//150k
	
    while (i)i--;
}





void I2C_IO::init()const
{
   	_scl.mode_out();
	_sda.mode_out();
	
	_scl = 0;
	_sda = 1;
	delay();
}
void I2C_IO::start()const
{
	_sda = 1;	delay();
	_scl = 1;	delay();
	
	_sda = 0;	//下升沿
	
	delay(); 	_scl = 0;	
}


void I2C_IO::stop()const
{
	delay();
	_sda = 0;
	delay();
	_scl = 1;
	delay();
	_sda = 1;
	delay();
	_scl = 0;
	
}
uint8_t I2C_IO::wait_ack()const
{
	uint16_t t=0;
	
	_sda.mode_in();
	delay();delay();
	_scl = 1;
	delay();
	while(_sda.read())
	{
		if(++t>1000)
		{
			stop();
			
#if I2C_IO_DEBUG
			printf("I2C_IO ACK_ERROR,address=0x%2x(%d)\r\n",slave_address(),slave_address());
#endif
			return 1;
		}
	}
	delay();
	_scl = 0;
	_sda.mode_out();
	return 0;	
}

void I2C_IO::ack()const
{
	
	_sda = 0;
	delay();
	_scl = 1;		//SDA 保持低电平
	delay();
	delay();
	_scl = 0;		
}


void I2C_IO::nack()const
{
	delay();
	_sda = 1;	
	delay();	
	
	_scl = 1;			//SDA 高电平
	delay();
	delay();
	_scl = 0;		
}


void I2C_IO::write_byte(uint8_t c)const
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		delay();		
		
		_sda = (c&0x80)>>7;
		
		delay();		
		
		_scl = 1;
		delay();		
		delay();
		
		_scl = 0;
		c <<=1;
	}	
}


uint8_t I2C_IO::read_byte()const
{
	uint8_t val,i;
	_sda.mode_in();
	for(i = 0;i<8;i++)
	{	
		delay();delay();	
		
		val<<=1;	
		_scl = 1;
		delay();	
		if(_sda.read())val++;
		delay();	
		
		_scl=0;
	}
	delay();
	_sda.mode_out();
	return val;	
}



