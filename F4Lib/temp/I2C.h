#ifndef _I2C_H_
#define _I2C_H_


#include "sys.h"




#define I2C_WAIT_ACK_OK			0
#define I2C_WAIT_ACK_TIMEOUT	1





class I2C
{
public:
	I2C(uint8_t slave_addr):_slave_addr(slave_addr),_fast_mode(true){}
		
	virtual ~I2C(){};
	virtual void init()const  = 0;
		
	/**************************/
	//手动输入从机地址
	uint8_t write(uint8_t slave_addr,uint8_t c)const;											//写一个字节
	uint8_t write(uint8_t slave_addr,const void *buf,uint16_t len)const;						//写一个数组
	
	uint8_t write_reg(uint8_t slave_addr,uint8_t reg_addr,uint8_t reg_val)const;					//向寄存器写一个数据
	uint8_t write_reg(uint8_t slave_addr,uint8_t reg_addr,const void *buf,uint16_t len)const;	//向寄存器写一串数据


	//读一个字节的函数不返回错误，错误时返回默认值0
	uint8_t read(uint8_t slave_addr)const;
	uint8_t read(uint8_t slave_addr,void *buf,uint16_t len)const;
	
	uint8_t read_reg(uint8_t slave_addr,uint8_t reg_addr)const;								//从寄存器读一个数据
	uint8_t read_reg(uint8_t slave_addr,uint8_t reg_addr,void *buf,uint16_t len)const;	//从寄存器读一串数据
	/**************************/
	//自动设置从机地址
	//void slave_address(uint8_t addr){_slave_addr = addr;}
	
	uint8_t write(uint8_t c)const{	return write(slave_address(),c);	}										//写一个字节
	uint8_t write(const void *buf,uint16_t len)const{	return write(slave_address(),buf,len);		}			//写一个数组
	uint8_t write_reg(uint8_t reg_addr,uint8_t c)const{	return write_reg(slave_address(),reg_addr,c);	}		//寄存器写一个数据
	uint8_t write_reg(uint8_t reg_addr,const void *buf,uint16_t len)const{	return write_reg(slave_address(),reg_addr,buf,len);	}	//向寄存器写一串数据


	uint8_t read()const{	return read(slave_address());	}
	uint8_t read(void *buf,uint16_t len)const{	return read(slave_address(),buf,len);	}
	uint8_t read_reg(uint8_t reg_addr)const{	return read_reg(slave_address(),reg_addr);}				//从寄存器读一个数据
	uint8_t read_reg(uint8_t reg_addr,void *buf,uint16_t len)const{ return read_reg(slave_address(),reg_addr,buf,len);};	//从寄存器读一串数据

	/*****************************/
	//uint8_t get_error(){uint8_t t =_error_code;_error_code = 0;return t;}
	uint8_t slave_address()const{return _slave_addr;};
	virtual	void fast_mode(bool fast)const{_fast_mode = fast;};
	bool fast_mode()const{	return _fast_mode;}

private:
	const uint8_t _slave_addr;
	mutable bool 	_fast_mode;
	//uint8_t _error_code;
protected:
	//接口
	virtual void start() const = 0;
	virtual void stop()const   = 0;
	virtual void ack() const  = 0;
	virtual void nack() const  = 0;
	virtual uint8_t wait_ack() const  = 0;			//应答成功返回0，否则返回1

	virtual void write_byte(uint8_t c)const   = 0;	
	virtual uint8_t read_byte()const   = 0;

};




#endif

