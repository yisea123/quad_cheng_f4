#ifndef _STMFLASH_H_
#define _STMFLASH_H_
#include "sys.h"
#include "Flash.h"


//扇区大小
const uint32_t STM_SECTOR_SIZE = 1024;		
const uint32_t STM32_FLASH_BASE = 0x08000000;




class StmFlash :public FlashClass
{

public:
	StmFlash(uint32_t address_offset) :_address_offset(address_offset)
	{}


	virtual void erase_sector(uint32_t sector_index);
	//在一个扇区内写，超出截断
	virtual uint32_t write_sector(uint32_t address, void *buffer, uint32_t len);

	virtual void read(uint32_t address, void *buffer, uint32_t len);
	virtual uint32_t sector_size()const { return STM_SECTOR_SIZE; }

	uint32_t get_absolute_address(uint32_t relative_address) {
		return (STM32_FLASH_BASE + _address_offset + relative_address);
	}


private:
	uint32_t _address_offset;

	virtual void erase_chip() {}



};

#endif


