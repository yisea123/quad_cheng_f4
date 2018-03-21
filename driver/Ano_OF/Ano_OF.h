#ifndef __ANO_OF_H_
#define __ANO_OF_H_

#include "sys.h"

//以下为全局变量，在其他文件中，引用本h文件，即可在其他文件中访问到以下变量
//光流信息质量：QUA
//光照强度：LIGHT
// extern uint8_t 	OF_QUA,OF_LIGHT;
// //原始光流信息，具体意义见光流模块手册
// extern int8_t	OF_DX,OF_DY;
// //融合后的光流信息，具体意义见光流模块手册
// extern int16_t	OF_DX2,OF_DY2,OF_DX2FIX,OF_DY2FIX;
// //原始高度信息和融合后高度信息
// extern uint16_t	OF_ALT,OF_ALT2;
// //原始陀螺仪数据
// extern int16_t	OF_GYR_X,OF_GYR_Y,OF_GYR_Z;
// //滤波后陀螺仪数据
// extern int16_t	OF_GYR_X2,OF_GYR_Y2,OF_GYR_Z2;
// //原始加速度数据
// extern int16_t	OF_ACC_X,OF_ACC_Y,OF_ACC_Z;
// //滤波后加速度数据
// extern int16_t	OF_ACC_X2,OF_ACC_Y2,OF_ACC_Z2;
// //欧拉角格式的姿态数据
// extern float	OF_ATT_ROL,OF_ATT_PIT,OF_ATT_YAW;
// //四元数格式的姿态数据
// extern float	OF_ATT_S1,OF_ATT_S2,OF_ATT_S3,OF_ATT_S4;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct  
{
	//原始光流信息
	uint8_t		QUA, LIGHT;
	int8_t		DX, DY;
	//融合后光流信息
	int16_t		DX2, DY2, DX2FIX, DY2FIX;

	uint16_t	ALT, ALT2;		//原始高度信息，融合后高度信息

	int16_t		GYR_X, GYR_Y, GYR_Z;		//原始陀螺仪数据
	int16_t		GYR_X2, GYR_Y2, GYR_Z2;	//滤波后陀螺仪数据

	int16_t		ACC_X, ACC_Y, ACC_Z;		//原始加速度数据
	int16_t		ACC_X2, ACC_Y2, ACC_Z2;	//滤波后加速度数据

	float		ATT_ROL, ATT_PIT, ATT_YAW;				//姿态欧拉角
	float		ATT_S1, ATT_S2, ATT_S3, ATT_S4;		//姿态四元数

	uint32_t update_time;
}ANO_OF_T;
extern ANO_OF_T ANO_OF;



//本函数是唯一一个需要外部调用的函数，因为光流模块是串口输出数据
//所以本函数需要在串口接收中断中调用，每接收一字节数据，调用本函数一次
void AnoOF_GetOneByte(uint8_t data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
