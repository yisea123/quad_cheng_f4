#include "I2C.h"



//写一个字节
uint8_t I2C::write(uint8_t slave_addr,uint8_t c)const
{
	start();
	
	write_byte(slave_addr&0xfe);
	if(wait_ack())return 1;
	
	write_byte(c);
	if(wait_ack())return 1;
	
	stop();
	return 0;
}


//写一个数组
uint8_t I2C::write(uint8_t slave_addr,const void *buf,uint16_t len)const
{
	start();
	
	write_byte(slave_addr&0xfe);
	if(wait_ack())return 1;
	
	const uint8_t *p = (uint8_t*)buf;
	for(uint16_t i=0;i<len;i++)
	{
		write_byte(*p++);
		if(wait_ack())return 1;
	}
	stop();
	return 0;

}


//向寄存器写一个字节
uint8_t I2C::write_reg(uint8_t slave_addr,uint8_t reg_addr,uint8_t reg_val)const
{
	start();
	
	write_byte(slave_addr&0xfe);
	if(wait_ack())return 1;
	
	write_byte(reg_addr);
	if(wait_ack())return 1;
	
	write_byte(reg_val);
	if(wait_ack())return 1;
	
	stop();
	return 0;
}




//向寄存器写一个数组
uint8_t I2C::write_reg(uint8_t slave_addr,uint8_t reg_addr,const void *buf,uint16_t len)const
{
	start();
	
	write_byte(slave_addr&0xfe);
	if(wait_ack())return 1;
	
	write_byte(reg_addr);
	if(wait_ack())return 1;
	
	const uint8_t *p = (uint8_t*)buf;
	for(uint16_t i=0;i<len;i++)
	{
		write_byte(*p++);
		if(wait_ack())return 1;
	}
	
	stop();
	return 0;
}




//读一个字节
uint8_t I2C::read(uint8_t slave_addr)const
{
	start();
	
	write_byte(slave_addr|0x01);
	if(wait_ack())
	{
//		_error_code = I2C_WAIT_ACK_TIMEOUT;
		return 0;
	}
	
	
	
	uint8_t reg_val = read_byte();
	
	
	stop();
	
	return reg_val;
}

//读一个数组
uint8_t I2C::read(uint8_t slave_addr,void *buf,uint16_t len)const
{
	start();
	
	write_byte(slave_addr|0x01);
	if(wait_ack())return 1;
	
	uint8_t *p = (uint8_t*)buf;
	for(uint16_t i=0;i<len;i++)
	{
		*p++ = read_byte();
		ack();
	}
	
	stop();
	return 0;
}


//从寄存器读一个字节
uint8_t I2C::read_reg(uint8_t slave_addr,uint8_t reg_addr)const
{
	
	start();
	
	
	write_byte(slave_addr&0xfe);
	if(wait_ack())
	{
//		_error_code = I2C_WAIT_ACK_TIMEOUT;
		return 0;
	}
	
	
	write_byte(reg_addr);
	if(wait_ack())	
	{
//		_error_code = I2C_WAIT_ACK_TIMEOUT;
		return 0;
	}
	
	start();
	write_byte(slave_addr|0x01);
	if(wait_ack())
	{
//		_error_code = I2C_WAIT_ACK_TIMEOUT;
		return 0;
	}
	
	uint8_t reg_val = read_byte();
	stop();
	
	return reg_val;
}



//从寄存器读一个数组
uint8_t I2C::read_reg(uint8_t slave_addr,uint8_t reg_addr,void *buf,uint16_t len)const
{
	
	if(len == 0)return 0;
	
	start();
	write_byte(slave_addr&0xfe);
	if(wait_ack())
	{
		stop();
		return 1;
	}
	
	write_byte(reg_addr);
	if(wait_ack())return 1;
	
	start();
	write_byte(slave_addr|0x01);
	if(wait_ack())return 1;
	

	uint8_t *p = (uint8_t*)buf;
	for(uint16_t i=0;i<len-1;i++)
	{
		*p++ = read_byte();
		ack();
	}
	
	*p = read_byte();
	nack();
	
	stop();
	return 0;
}




















