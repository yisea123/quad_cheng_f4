#ifndef _SPIFLASH_H_
#define _SPIFLASH_H_


#include "HAL_F4.h"


#define W25X40  0XEF12	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17




//W25Qxx扇区大小为4KB



class SPIFlash
{
public:
	SPIFlash(SPIDriver& spi,GPIO cs):_spi(spi),_cs(cs){};
	virtual ~SPIFlash(){};
	
	uint8_t status();
	void status(uint8_t sta);
		
	void write_enable();
	void write_disable();
	
	void init();	
	bool is_ready(){return !((status()&0x01)==0x01); }  
	void wait_busy(){while((status()&0x01)==0x01); }

	void erase_chip();
	void erase_sector(uint32_t sec_index);		//每个扇区4K
	
	void write_page(uint32_t address,void *buf,uint16_t len);		//一页256字节
	void write_nocheck(uint32_t address,void *buf,uint32_t len);	
	uint16_t write_sector(uint32_t address,void *buf,uint16_t len); //在一个扇区内读写,超过时自动阶段
	
	//void write(uint32_t address,const void *buf,uint32_t len) ;
	
	void read(uint32_t address,void *buf,uint32_t len) ;
	
	
	uint16_t sector_size()const	{		return _sector_size;	}
	uint32_t flash_size()const	{		return _flash_size;		}
	uint16_t id()const			{		return _id;				}
	
	
private:
	SPIDriver& _spi;
	GPIO	  _cs;

	
	uint16_t	_sector_size;	//FLASH一个扇区大小,in byte
	uint32_t 	_flash_size;	//FLASH总大小	    ,in byte
	uint16_t 	_id;


	//static uint8_t write_buffer[4096];	//static uint8_t *write_buffer = new uint8_t[sector_size()];
	uint16_t read_id();
};


extern SPIFlash spiflash;



#endif

