#include "I2C.h"




I2C &i2cTEA	= 


//读TEA5767状态,并转换成频率
void radio_read(void)
{
    unsigned char i;
    unsigned char temp_l,temp_h;
    pll=0;
    iic_start();
    iic_write8bit(0xc1);        //TEA5767读地址
    if(!iic_testack())
    {
        for(i=0;i<5;i++)
        {
            radio_read_data[i]=iic_read8bit();
            iic_ack();
        }
    }
    iic_stop();
    temp_l=radio_read_data[1];
    temp_h=radio_read_data[0];
    temp_h&=0x3f;
    pll=temp_h*256+temp_l;
    get_frequency();
}


