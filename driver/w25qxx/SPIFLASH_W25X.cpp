#include "SPIFLASH_W25X.h"



//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


uint8_t SPIFLASH_W25X::write_buffer[4096];	//static uint8_t *write_buffer = new uint8_t[sector_size()];




uint16_t SPIFLASH::write_in_sector(uint32_t address,void *buf,uint16_t len)const
{
	uint16_t secremain;	      
	secremain = sector_size() - address%sector_size();	//扇区剩余空间大小
	
   	if(len<=secremain)secremain=len;		//不大于扇区剩余空间大小
	write_nocheck(address,buf,secremain);	

	return secremain;	//返回实际写入字节

}



void SPIFLASH_W25X::init()
{
	
	_spi.cs = 1;
	_id = read_id();

	
	_sector_size = 4*1024;
	switch(_id)
	{
		case W25X40:
			_flash_size = 512*1024;	//512K Byte
			break;
		case W25Q80:
			_flash_size = 1*1024*1024;	//1M Byte
		
			break;
		case W25Q16:
			_flash_size = 2*1024*1024;	//2M Byte
			break;
		case W25Q32:
			_flash_size = 4*1024*1024;	//4M Byte
			break;
		case W25Q64:
			_flash_size = 8*1024*1024;	//8MByte
			break;		
		case W25Q128:
			_flash_size = 16*1024*1024;	//16MByte
			break;	
		
	}
}



uint16_t SPIFLASH_W25X::read_id()
{
	uint16_t temp = 0;	 
	
	_spi.cs = 0;	
	
	_spi.write(0x90);//发送读取ID命令	    
	_spi.write(0x00); 	    
	_spi.write(0x00); 	    
	_spi.write(0x00); 	 			
	
	temp|=_spi.read()<<8;  
	temp|=_spi.read();	 
	
	_spi.cs = 1;	
	
	return temp;
}

uint8_t SPIFLASH_W25X::status()const
{
	_spi.cs = 0;
	_spi.write(W25X_ReadStatusReg);
	uint8_t sta = _spi.read();
	_spi.cs = 1;
	
	return sta;
}

void SPIFLASH_W25X::status(uint8_t sta)const
{
	_spi.cs = 0;                       //使能器件   
	_spi.write(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	_spi.write(sta);               		//写入一个字节  
	_spi.cs = 1;                       //取消片选 
}


void SPIFLASH_W25X::write_enable()const
{
	_spi.cs = 0;                          //使能器件   
    _spi.write(W25X_WriteEnable);      	//发送写使能  
	_spi.cs = 1;                          //取消片选  
} 



void SPIFLASH_W25X::write_disable()const
{
	_spi.cs = 0;                          //使能器件   
    _spi.write(W25X_WriteDisable);      	//发送写使能  
	_spi.cs = 1;                          //取消片选  
} 



//要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void SPIFLASH_W25X::write_page(uint32_t address,void *buf,uint16_t len)const
{
	while(!is_ready()){};	
	_spi.cs = 0; 
    _spi.write(W25X_ReadData);         //发送读取命令   
    _spi.write((uint8_t)((address)>>16));  //发送24bit地址    
    _spi.write((uint8_t)((address)>>8));   
    _spi.write((uint8_t)address);  	

	_spi.write(buf,len);
	_spi.cs = 1;
}


void SPIFLASH_W25X::write_nocheck(uint32_t address,void *buf,uint32_t len)const
{ 			 		 
	uint16_t pageremain;	
	
	pageremain=256-address%256; 		//单页剩余的字节数		 	    
	if(len<=pageremain)pageremain=len;	//不大于256个字节
	
	uint8_t *p = (uint8_t*)buf;
	while(1)
	{	   
		write_page(address,p,pageremain);
		
		if(len==pageremain)break;//写入结束了
	 	else 
		{
			p+=pageremain;
			address+=pageremain;	
			len-=pageremain;			  //减去已经写入了的字节数
			
			if(len>256)pageremain=256; //一次可以写入256个字节
			else pageremain=len; 	  //不够256个字节了
		}
	};	    
}




void SPIFLASH_W25X::write(uint32_t address,const void *buf,uint32_t len)const	 
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	
	uint8_t *W25QXX_BUF = SPIFLASH_W25X::write_buffer;	  
	uint8_t *p = (uint8_t*)buf;
 	secpos=address/sector_size();	//扇区地址  
	secoff=address%sector_size();	//在扇区内的偏移
	secremain=sector_size()-secoff;	//扇区剩余空间大小   
	
	
 	if(len<=secremain)secremain=len;//不大于扇区剩余空间大小
	while(1) 
	{	
		read(secpos*sector_size(),W25QXX_BUF,sector_size());//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验剩余空间为0xFF
		{
			if(W25QXX_BUF[secoff+i]!=0xFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			erase(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=p[i];	  
			}
			write_nocheck(secpos*sector_size(),W25QXX_BUF,sector_size());//写入整个扇区  
		}
		else 
		{
			write_nocheck(address,p,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		}
		
		if(len==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	p+=secremain;  //指针偏移
			address+=secremain;//写地址偏移	   
		   	len-=secremain;				//字节数递减
			if(len>sector_size())secremain=sector_size();	//下一个扇区还是写不完
			else secremain=len;			//下一个扇区可以写完了
		}	 
	}
}


void SPIFLASH_W25X::erase_chip()const
{
	while(!is_ready());   
	_spi.cs = 0;                           //使能器件   
	_spi.write(W25X_ChipErase);        //发送片擦除命令  
	_spi.cs = 1;    
}
//擦除一个山区的最少时间:150ms
void SPIFLASH_W25X::erase(uint32_t sec_index)const
{
	sec_index *= sector_size();	//字节地址
	
	while(!is_ready());   
	_spi.cs = 0;                           //使能器件   
	_spi.write(W25X_SectorErase);      //发送扇区擦除指令 
	_spi.write((uint8_t)((sec_index)>>16));  //发送24bit地址    
	_spi.write((uint8_t)((sec_index)>>8));   
	_spi.write((uint8_t)sec_index);  
	_spi.cs = 1;  	
	_spi.cs = 1; 	

}


void SPIFLASH_W25X::read(uint32_t address,void *buf,uint32_t len)const
{ 
 	uint16_t i;   										    
	_spi.cs = 0;                           //使能器件   
    _spi.write(W25X_ReadData);         //发送读取命令   
    _spi.write((uint8_t)((address)>>16));  //发送24bit地址    
    _spi.write((uint8_t)((address)>>8));   
    _spi.write((uint8_t)address);   
	
	uint8_t *p = (uint8_t*)buf;
    for(i=0;i<len;i++)
	{ 
        *p++=_spi.write(0XFF);   //循环读数  
    }
	_spi.cs = 1;
}




