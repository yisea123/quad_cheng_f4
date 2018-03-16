#ifndef _SBUS_H_
#define _SBUS_H_

#include "sys.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*******************************************************
SBUS:一帧数据25字节,每个通道11位,有16个通道,共22字节
最小值		中位值		最大值		行程
352			1024		1696		1344(672+672)	
*******************************************************/


#define SBUS_MID				1024
#define SBUS_MIN				352	
#define SBUS_MAX				1696	


#pragma pack(1) 
//位域,先定义在高位
typedef	struct
{
	uint8_t startbyte;	//0x0f
	uint32_t ch1:11;	//副翼,ALI/ROLL
	uint32_t ch2:11;	//升降,ELE/PITCH
	uint32_t ch3:11;	//油门,THR
	uint32_t ch4:11;	//尾翼,RUD/YAW

	uint32_t ch5:11;
	uint32_t ch6:11;
	uint32_t ch7:11;
	uint32_t ch8:11;
	uint32_t ch9:11;
	uint32_t ch10:11;
	uint32_t ch11:11;
	uint32_t ch12:11;
	uint32_t ch13:11;
	uint32_t ch14:11;
	uint32_t ch15:11;
	uint32_t ch16:11;	

	uint8_t dg1:1;				//digital channel (0x80)
	uint8_t dg2:1;				//digital channel (0x40)
	uint8_t framelost:1;	// Frame lost, equivalent red LED on receiver (0x20)
	uint8_t failsafe:1;		//failsafe activated (0x10)
	uint8_t reserve:4;
	uint8_t endbyte;			//0x00
}sbus_channel_t;
#pragma pack() 


enum
{
	SBUS_EMPTY = 0,
	SBUS_AVAILABLE,
	SBUS_USING,
};


typedef struct 
{
	uint8_t state;
	sbus_channel_t channel;

	uint8_t rx_cnt;
	uint8_t buffer[30];				//startbyte,data1,data2,......,data22,flags,endbyte
}sbus_t;




extern sbus_t sbus;
void sbus_init(void);
void sbus_recvbyte_event(uint8_t byte);
void sbus_frame_event(void);





#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

