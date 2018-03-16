#ifndef _RDA5807_H_
#define _RDA5807_H_

#include "sys.h"
#include "I2C.h"



typedef __packed struct
{
	uint16_t ENABLE:1;		//使能
	uint16_t SOFT_RESET:1;	//软复位
	uint16_t NEW_METHOD:1;	//提高灵敏度
	uint16_t RDS_EN:1;		//1:使能rds/rbds	
	uint16_t CLK_MODE:3;	//000:32.768kHz,001:12MHz,......
	uint16_t SKMODE:1;	//0:循环搜索

	uint16_t SEEK:1;			//1:启动自动搜索，完成后SEEK清零，STC位置高
	uint16_t SEEKUP:1;			//0:向下搜索，1:向上搜索
	uint16_t RCLK_DIRECT:1;		//1:RCLK时钟直连
	uint16_t RCLK_MODE:1;		//1:FM工作时关闭RCLK,0:RCLK一直工作
	
	uint16_t BASS:1;			//1:开启重低音
	uint16_t MONO:1;			//0:双声道，1:强制单声道
	uint16_t DMUTE:1;			//0:静音
	uint16_t DHIZ:1;			//0:音频高阻输出
}RDA_R02;


#define RDA_BAND_87_108MHz	0
#define RDA_BAND_76_91MHz	1
#define RDA_BAND_96_108MHz	2
#define RDA_BAND_65_76MHz	3


#define RDA_SPACE_100k	0	//0.1M
#define RDA_SPACE_200k	1	//0.2M
#define RDA_SPACE_50k	2	//
#define RDA_SPACE_25k	3
typedef __packed struct
{
	uint16_t SPACE:2;		//设置SPACE
	uint16_t BAND:2;		//设置BAND
	uint16_t TUNE:1;		//1,启动调谐
	uint16_t DIRECT_MODE:1;	//直接控制模式，测试用	
	uint16_t CHAN:10;		//0:频道，fre = channel spacing(kHz)*CHAN + channels_base;

	
}RDA_R03;



typedef __packed struct
{
	uint16_t :8;			//RSVD

	uint16_t AFCD:1;		//0:afc使能
	uint16_t SOFTMUTE_EN:1;	//1:软静音
	uint16_t :1;			//RSVD
	uint16_t DE:1;			//De-emphasis,0=75us,1=50us
	uint16_t :4;			//RSVD
}RDA_R04;


typedef __packed struct
{
	uint16_t VOLUME:4;		//音量
	uint16_t RSVD:4;		//RSVD
	
	uint16_t SEEKTH:4;		//搜索阈值,0-15
	uint16_t :3;			//RSVD
	uint16_t INT_MODE:1;	//中断模式，0:5ms一次中断，1:中断知道读取0C寄存器
}RDA_R05;


//typedef __packed struct
//{
//	uint16_t :13;			//RSVD
//	uint16_t OPEN_MODE:2;	//保留寄存器开放设置，11:...
//	uint16_t :1;			//RSVD
//}RDA_R06;

//typedef __packed struct
//{
//	uint16_t FREQ_MODE:1;		//
//	uint16_t SOFTBLEND_EN:1;	//
//	uint16_t SEEK_TH_OLD:6;		//
//	uint16_t MODE_65M_50M:1;	//	
//	uint16_t TH_SOFRBLEND:6;	
//	uint16_t :1;				//RSVD
//}RDA_R07;



//读
typedef __packed struct
{
	
	uint16_t READCHAN:10;			//读取频率
	uint16_t ST:1;					//0:立体声，1:单声道
	uint16_t BLK_E:1;				//
	uint16_t RDSS:1;				//	
	uint16_t SF:1;					//0:搜索失败，1:搜索成功
	uint16_t STC:1;					//1:调谐或搜索完成
	uint16_t RDSR:1;				//

}RDA_R0A;

typedef __packed struct
{
	uint16_t BLERB:2;				//
	uint16_t BLERA:2;				//
	uint16_t :2;					//
	uint16_t FM_READY:1;			//1:ready
	uint16_t FM_TRUE:1;				//1:当前通道是1个电台
	uint16_t RSSI:7;				//

}RDA_R0B;

//typedef __packed struct
//{
//	uint16_t RDSA;				
//}RDA_R0C;

//typedef __packed struct
//{
//	uint16_t RDSB;				
//}RDA_R0D;
//typedef __packed struct
//{
//	uint16_t RDSC;				
//}RDA_R0E;
//typedef __packed struct
//{
//	uint16_t RDSD;				
//}RDA_R0F;



class RDA5807
{
	
public:
	RDA5807(I2C& i2c);
	uint8_t init();

	uint8_t write(uint8_t reg_addr,const void *reg);
	uint8_t read(uint8_t reg_addr,void *reg);

	uint8_t frequency(uint16_t fre);
	uint16_t frequency();
	uint8_t rssi();
	uint8_t mute(bool mute);

	uint8_t set_volume(uint8_t vol);//0-15
	uint8_t seek_up();
	uint8_t seek_down();
	uint8_t seek_stop();
	uint8_t seek_success();
private:
	I2C& _i2c;

	uint16_t _band_base;
	RDA_R02 _r02;
	RDA_R03 _r03;
	RDA_R04 _r04;
	RDA_R05 _r05;
	RDA_R0A _r0A;
	RDA_R0B _r0B;



	

};





#endif

