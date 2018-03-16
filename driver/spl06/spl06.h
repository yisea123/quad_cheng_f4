#ifndef _SP06_H_
#define _SP06_H_

#include "sys.h"
#include "HAL_F4.h"




//气压测量速率(sample/sec),Background 模式使用
#define  PM_RATE_1
#define  PM_RATE_2
#define  PM_RATE_4
#define  PM_RATE_8
#define  PM_RATE_16
#define  PM_RATE_32
#define  PM_RATE_64
#define  PM_RATE_128

//气压重采样速率(times),Background 模式使用
#define PM_PRC_1
#define PM_PRC_2
#define PM_PRC_4
#define PM_PRC_8
#define PM_PRC_16
#define PM_PRC_32
#define PM_PRC_64
#define PM_PRC_128




//温度测量速率(sample/sec),Background 模式使用
#define  TMP_RATE_1
#define  TMP_RATE_2
#define  TMP_RATE_4
#define  TMP_RATE_8
#define  TMP_RATE_16
#define  TMP_RATE_32
#define  TMP_RATE_64
#define  TMP_RATE_128

//温度重采样速率(times),Background 模式使用
#define TMP_PRC_1
#define TMP_PRC_2
#define TMP_PRC_4
#define TMP_PRC_8
#define TMP_PRC_16
#define TMP_PRC_32
#define TMP_PRC_64
#define TMP_PRC_128



class SPL06
{
public:
	SPL06(const I2C& iic):
		_iic(iic)
	{}
		
	uint8_t init();		




private:
	const I2C& _iic;
	uint16_t _C0,_C1,_C01,_C11,_C20,_C21,_C30;
	uint32_t _C00,_C10;
	
};
extern SPL06 spl06;


#endif

