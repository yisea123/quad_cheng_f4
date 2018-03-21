#ifndef _FLASH_CLASS_H_
#define _FLASH_CLASS_H_


#include "sys.h"



class FlashClass
{
public:
	
	virtual void write_enable(){}
	virtual void write_disable(){}
		
	virtual void erase_chip(){}
		
		
	virtual void erase_sector(uint32_t sector_index) = 0;	
	//在一个扇区内写，超出截断
	virtual uint32_t write_sector(uint32_t address,void *buffer,uint32_t len) = 0;	
	virtual void read(uint32_t address,void *buffer,uint32_t len) = 0;
	virtual uint32_t sector_size()const = 0;
	
	

private:
		
	
	
	
};


#endif



