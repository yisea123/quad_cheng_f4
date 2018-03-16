#ifndef _TEA5767_H_
#define _TEA5767_H_

#include "sys.h"



typedef __packed struct
{

	//第一字节
	uint8_t MUTE:1;				//1:静音
	uint8_t SM:1;				//1:搜索模式	0:非搜索模式
	uint8_t PLL13_8:6;
	
	//第二字节
	uint8_t PLL7_0;				
	
	//第三字节
	uint8_t	SUD:1;				//1:向上搜索	0:向下搜索
	uint8_t	SSL:2;				//搜索停止级别：
								//0:允许在搜索模式,
								//1:ADC输出=5
								//2:ADC输出=7
								//3:ADC输出=10

	uint8_t	HLSI:1;				//1:高边LO注入,0:低边LO注入
	uint8_t	MS:1;				//1:强行单声道，0:开立体声
	uint8_t	MR:1;				//1:R通道静音和强行单声道，0:R不静音
	uint8_t	ML:1;				//1:L通道静音和强行单声道，0:L不静音
	uint8_t	SWP1:1;				//软件可编程端口
	
	//第四字节
	uint8_t	SWP2:1;				//软件可编程端口
	uint8_t	STBY:1;				//1:标准模式，0:非标准模式
	uint8_t	BL:1;				//频带限制，1：日本FM频带，0:美国/欧洲频带
	uint8_t	XTAL:1;				//晶振设置，1: 32768Hz,0:13MHz
	uint8_t	SMUTI:1;			//软静音:	1:软静音开 0:软静音关
	uint8_t	HCC:1;				//1:高切控制开,0:高切控制关
	uint8_t	SNC:1;				//1:立体噪声消除开，0:立体噪声消除关
	uint8_t	SI:1;				//搜索指示，1:SWPORT1引脚输出就绪标志，0:SWPORT1引脚时软件可编程端口1
	
	//第5字节
	uint8_t	PLLREF:1;			//1:PLL的6.5MHz参考频率使能，0:PLL的6.5MHz参考频率关闭
	uint8_t	DTC:1;				//1:去加重时间常数75us,0:去加重时间常数50us,
	uint8_t	resv:6;				//保留
	
	
	


}TEA5767_PARM_WRITE_T;


typedef __packed struct
{

	//第一字节
	uint8_t RF:1;				//1:已找到一个电台或到了频率极限	0:没有发现电台
	uint8_t BLF:1;				//1:到了频率极限	0:未到频率极限
	uint8_t PLL13_8:6;
	
	//第二字节
	uint8_t PLL7_0;				
	
	//第三字节
	uint8_t	STEREO:1;			//1:立体声接收	0:单通道接收
	uint8_t	IF6_0:7;			//IF计数结果
	
	//第四字节
	uint8_t	LEV3_0:4;			//ADC输出级别
	uint8_t	CI3_1:3;			//芯片识别
	uint8_t resv:1;
	
	
	//第5字节
	uint8_t	resv5;			//全部保留
	

}TEA5767_PARM_READ_T;
#endif


