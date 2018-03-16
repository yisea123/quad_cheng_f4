#ifndef _IBUS_H_
#define _IBUS_H_

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*******************************************************
IBUS:一帧数据32字节,每个通道16位,有14个通道,共28字节
最小值		中位值		最大值		行程
1000		1500		2000		1000(500+500)
*******************************************************/


#define IBUS_MID				1500
#define IBUS_MIN				1000	
#define IBUS_MAX				2000	


#pragma pack(1) 
	//位域,先定义在高位
	typedef	struct
	{
		uint16_t ch1;
		uint16_t ch2;
		uint16_t ch3;
		uint16_t ch4;
		uint16_t ch5;
		uint16_t ch6;
		uint16_t ch7;
		uint16_t ch8;
		uint16_t ch9;
		uint16_t ch10;
		uint16_t ch11;
		uint16_t ch12;
		uint16_t ch13;
		uint16_t ch14;
	}ibus_channel_t;
#pragma pack() 

enum
{
	IBUS_EMPTY = 0,
	IBUS_AVAILABLE,
	IBUS_USING,
};


typedef struct
{
	uint8_t state;
	ibus_channel_t channel;

	uint8_t rx_cnt;
	uint8_t buffer[40];				//startbyte,data1,data2,......,data22,flags,endbyte
}ibus_t;




extern ibus_t ibus;
void ibus_init(void);
void ibus_recvbyte_event(uint8_t byte);
void ibus_frame_event(void);





#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif





