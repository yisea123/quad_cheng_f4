#ifndef _SPIFLASH_W25X_H_
#define _SPIFLASH_W25X_H_

#include "sys.h"
#include "SPI.h"




class SPIFLASH
{
public:
	SPIFLASH(const SPI& spi):
		_sector_size(0),
		_flash_size(0),
		_id(0),
		_spi(spi){}
		
	virtual ~SPIFLASH(){};
		
	virtual void init() = 0;	
	virtual bool is_ready()const = 0;
		
		
	virtual void erase_chip()const  = 0;
	virtual void erase(uint32_t sec_index)const = 0;
	virtual void read(uint32_t address,void *buf,uint32_t len) const= 0;
	virtual void write_nocheck(uint32_t address,void *buf,uint32_t len)const = 0;
	virtual void write(uint32_t address,const void *buf,uint32_t len)const = 0;
		
	uint16_t write_in_sector(uint32_t address,void *buf,uint16_t len)const;
		
		
	
	uint16_t sector_size()const	{		return _sector_size;	}
	uint32_t flash_size()const	{		return _flash_size;		}
	uint16_t id()const			{		return _id;				}

protected:
	uint16_t	_sector_size;	//FLASH一个扇区大小,in byte
	uint32_t 	_flash_size;	//FLASH总大小	    ,in byte
	uint16_t 	_id;
	
	const SPI&	_spi;


};


//W25X系列/Q系列芯片列表
#define W25X40  0XEF12	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17



class SPIFLASH_W25X:public SPIFLASH
{
public:
	SPIFLASH_W25X(const SPI& spi):SPIFLASH(spi){	}
	virtual ~SPIFLASH_W25X(){};
	
	uint8_t status()const;
	void status(uint8_t sta)const;
		
	void write_enable()const;
	void write_disable()const;
	
	virtual void init();	
	virtual bool is_ready()const{return !((status()&0x01)==0x01); }  // 等待BUSY位清空};

	
	virtual void erase_chip()const;
	virtual void erase(uint32_t sec_index)const;
	virtual void read(uint32_t address,void *buf,uint32_t len)const ;
	
	void write_page(uint32_t address,void *buf,uint16_t len)const;
	virtual void write_nocheck(uint32_t address,void *buf,uint32_t len)const;	
	virtual void write(uint32_t address,const void *buf,uint32_t len)const ;
	
	
	
private:
	static uint8_t write_buffer[4096];	//static uint8_t *write_buffer = new uint8_t[sector_size()];
	uint16_t read_id();
};


#endif

