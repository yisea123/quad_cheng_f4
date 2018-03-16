#include "spl06.h"



#define SP06_PSR_B2		0x00		//气压值
#define SP06_PSR_B1		0x01
#define SP06_PSR_B0		0x02
#define SP06_TMP_B2		0x03		//温度值
#define SP06_TMP_B1		0x04
#define SP06_TMP_B0		0x05




#define SP06_PSR_CFG	0x06		//气压测量配置
#define SP06_TMP_CFG	0x07		//温度测量配置
#define SP06_MEAS_CFG	0x08		//

#define SP06_CFG_REG	0x09
#define SP06_INT_STS	0x0A
#define SP06_FIFO_STS	0x0B

#define SP06_RESET		0x0C
#define SP06_ID			0x0D

#define SP06_COEF		0x10		//-0x21,12个字节
#define SP06_COEF_SRCE	0x28



SPL06 spl06(hal.iic_spl06);




uint8_t SPL06::init()
{
	uint8_t coef[18];
	
	if(_iic.write_reg(SP06_RESET,0x89))
	{
		puts("spl06 reset  fail\r\n");
		return 1;
	}
	
	delay_ms(50);
	_iic.read_reg(SP06_COEF,coef,18);
	
	_C0 	= ((uint16_t)coef[0]<<4 ) + (coef[1]>>4);
	_C1 	= ((uint16_t)(coef[1]&0x0F)<<8 ) + coef[2];
	
	_C00 	= ((uint32_t)coef[3]<<12 ) + ((uint32_t)coef[4]<<4 ) + (coef[5]>>4);
	_C10   	= ((uint16_t)(coef[5]&0x0F)<<16 ) + ((uint32_t)coef[6]<<8 ) + coef[7];
	
	_C01   	= ((uint16_t)coef[8]<<8 ) + coef[9];
	_C11   	= ((uint16_t)coef[10]<<8 ) + coef[11];
	_C20   	= ((uint16_t)coef[12]<<8 ) + coef[13];
	_C21   	= ((uint16_t)coef[14]<<8 ) + coef[15];
	_C30   	= ((uint16_t)coef[16]<<8 ) + coef[17];
	
	
//	for (uint8_t i=0;i<18;i++) 
//	{		
//		printf("C%d=%d\r\n",i,coef[i]);
//	}	
//	
	printf("_C0=%d\r\n",_C0);
	printf("_C1=%d\r\n",_C1);
	printf("_C00=%d\r\n",_C00);
	printf("_C10=%d\r\n",_C10);
	printf("_C01=%d\r\n",_C01);
	printf("_C11=%d\r\n",_C11);
	printf("_C20=%d\r\n",_C20);
	printf("_C21=%d\r\n",_C21);
	printf("_C30=%d\r\n",_C30);
	
	puts("spl06 init pass\r\n");
	return 0;
}


