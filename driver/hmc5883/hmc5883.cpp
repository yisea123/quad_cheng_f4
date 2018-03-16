#include "hmc5883.h"
#include "hmc5883_dev.h"

#include "HAL_F4.h"



static const I2C_IO& iic_hmc5883 = hal.iic_hmc5883;


void hmc5883_init()
{
	
	//设置平均数和输出速率
	iic_hmc5883.write_reg(HMC_CONFIGA,HMC5883_AVRCOUNTS_8|HMC5883_SPEED_75|HMC5883_MS_NORMAL);
	iic_hmc5883.write_reg(HMC_CONFIGB, HMC5883_GN_1370);//设置量程 
	iic_hmc5883.write_reg(HMC_MODE, HMC5883_MR_ONE);//测量模式
	
	uint8_t id[3];
	hmc5883_getID(id);
}


//获取3个字节的ID: 'H','4','3'
void hmc5883_getID(uint8_t *id)
{
	iic_hmc5883.read_reg(HMC_IDA,id,3);		
	printf("HMC5883 ID:%c%c%c\r\n",id[0],id[1],id[2]);
}
	



void hmc5883_getMagAdc(int16_t *mag)
{
	uint8_t magbuffer[6];
	iic_hmc5883.read_reg(HMC_XMSB,magbuffer,6);		
	mag[0]=(int16_t)((magbuffer[0]<<8) | magbuffer[1]);
	mag[2]=(int16_t)((magbuffer[2]<<8) | magbuffer[3]);
	mag[1]=(int16_t)((magbuffer[4]<<8) | magbuffer[5]);
	iic_hmc5883.write_reg(HMC_MODE, HMC5883_MR_ONE);//测量模式
}

void hmc5883_getMag(float *mag)
{
	uint8_t magbuffer[6];
	iic_hmc5883.read_reg(HMC_XMSB,magbuffer,6);		
	mag[0]= HMC5883_KMAG*(int16_t)((magbuffer[0]<<8) | magbuffer[1]);
	mag[2]= HMC5883_KMAG*(int16_t)((magbuffer[2]<<8) | magbuffer[3]);
	mag[1]= HMC5883_KMAG*(int16_t)((magbuffer[4]<<8) | magbuffer[5]);
	iic_hmc5883.write_reg(HMC_MODE, HMC5883_MR_ONE);//测量模式
}


