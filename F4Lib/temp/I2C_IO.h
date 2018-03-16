#ifndef _I2C_IO_H_
#define _I2C_IO_H_

#include "sys.h"
#include "I2C.h"
#include "GPIO.h"



class I2C_IO:public I2C
{
public:
	I2C_IO(GPIO_PIN_NAME SCL,GPIO_PIN_NAME SDA,uint8_t slave_addr=0):
			I2C(slave_addr),_scl(SCL),_sda(SDA){};
		
	virtual void init()const;

	
	
	
private:
	void delay()const;

	virtual void start()const;
	virtual void stop()const;
	virtual void ack()const;
	virtual void nack()const;
	virtual uint8_t wait_ack()const;	//应答成功返回0，否则返回1

	virtual void write_byte(uint8_t c)const;
	virtual uint8_t read_byte()const;
	


	const GPIO _scl;
	const GPIO _sda;
	
};







#endif

