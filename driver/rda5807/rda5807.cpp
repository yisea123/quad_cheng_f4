#include "rda5807.h"



RDA5807::RDA5807(I2C& i2c):_i2c(i2c)
{	
	//R02
	_r02.DHIZ = 1;
	_r02.DMUTE = 1;
	_r02.MONO = 1;	
	_r02.BASS = 1;
	
	_r02.RCLK_MODE = 0;
	_r02.RCLK_DIRECT = 0;	//not directly
	_r02.SEEKUP = 0;
	_r02.SEEK = 0;
	
	_r02.SKMODE = 0;		//循环搜索
	_r02.CLK_MODE = 0;		//32.768kHz
	_r02.RDS_EN = 0;		//disable rds/rbds
	_r02.SOFT_RESET = 1;	//软复位
	_r02.ENABLE = 0;
	//R03
	_r03.BAND = RDA_BAND_87_108MHz;
	_r03.SPACE = RDA_SPACE_100k;
	_r03.DIRECT_MODE = 0;
	_r03.CHAN = 0;
	
	_band_base = 870;
	
	//R04
	_r04.AFCD = 0;			//afc使能
	_r04.SOFTMUTE_EN = 0;	
	_r04.DE = 0;
	
	

	//R05	
	_r05.INT_MODE = 1;	//
	_r05.SEEKTH = 6;	//搜索阈值
	_r05.RSVD = 0xF;	//高两位必须为1？？否则搜不到
	_r05.VOLUME = 0;	//默认声音0
	
}


uint8_t RDA5807::init()
{
	_i2c.init();
	_i2c.slave_address(0x22);
	

	_r02.SOFT_RESET = 1;
	_r02.ENABLE = 0;
	if(write(0x02,&_r02))
	{
		return 1;
	}

	
	
	delay_ms(100);							//延时,等待复位完成
	
	_r02.SOFT_RESET = 0;
	_r02.ENABLE = 1;				//取消复位
	write(0x02,&_r02);
	
	
	write(0x03,&_r03);
	write(0x04,&_r04);
	write(0x04,&_r04);
	write(0x04,&_r05);
	

	;
  	delay_ms(500);
	
	return 0;

}





uint8_t RDA5807::read(uint8_t reg_addr,void *reg)
{
	uint8_t res;
	uint8_t arr[2];
	res =  _i2c.read_reg(reg_addr,arr,2);
	
	if(res)return 1;
	uint8_t *p = (uint8_t*)reg;
	p[0] = arr[1];
	p[1] = arr[0];
	return 0;
	
	
}

uint8_t RDA5807::write(uint8_t reg_addr,const void *reg)
{
	uint8_t *p = (uint8_t*)reg;
	uint8_t arr[] = {p[1],p[0]};	
	return _i2c.write_reg(reg_addr,arr,2);
}


//BAND
//0:87-108MHz
//1:76-91MHz
//2:76-108MHz
//3:65-76MHz
uint8_t RDA5807::frequency(uint16_t fre)
{

	//设置频率
	_r03.CHAN = fre - _band_base;
	if(write(0x03,&_r03))return 1;
	//开启调谐
	_r03.TUNE = 1;
	write(0x03,&_r03);
	_r03.TUNE = 0;
	return 0;
}


uint16_t RDA5807::frequency()
{

	if(read(0x0A,&_r0A))return 0;
	uint16_t chan = _r0A.READCHAN;
	return (chan + _band_base);
	
}

//都是0？？
uint8_t RDA5807::rssi()
{
	if(read(0x0B,&_r0B))return 0;
	return _r0B.RSSI;
}

uint8_t RDA5807::set_volume(uint8_t vol)
{
	
	_r05.VOLUME = vol;
	return write(0x05,&_r05);
}

uint8_t RDA5807::mute(bool mute)
{
	_r02.DMUTE = mute?0:1;
	return write(0x02,&_r02);
}

uint8_t RDA5807::seek_up()
{
	_r02.SEEKUP = 1;
	_r02.SEEK = 1;
	return write(0x02,&_r02);
}
uint8_t RDA5807::seek_down()
{
	_r02.SEEKUP = 0;
	_r02.SEEK = 1;
	return write(0x02,&_r02);
}
uint8_t RDA5807::seek_stop()
{
	_r02.SEEKUP = 0;
	_r02.SEEK = 0;
	return write(0x02,&_r02);
}

//0:搜索成功
//1:搜索失败
//2:搜索未完成
uint8_t RDA5807::seek_success()
{
	
	read(0x0A,&_r0A);
	
//	std::cout<<"STC:"<<(uint32_t)_r0A.STC<<std::endl;
//	std::cout<<"SF:"<<(uint32_t)_r0A.STC<<std::endl;
	if(_r0A.STC)	//搜索完成
	{
		return _r0A.SF;
	}
	return 2;
	
}


//1048
//