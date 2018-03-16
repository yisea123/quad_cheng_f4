#include "SPIn.h"



//****Type	SCK		MISO	MOSI	
//****SPI1	PA5		PA6		PA7
//****SPI2	PB13	PB14	PB15
//****SPI3	PC10	PC11	PC12

void SPIn::init(LatchEdge latch_edge,FirstBit firstbit) const
{	 
	

	
	//SPI IO引脚,时钟初始化
	if(_spi == SPI1)
	{
		RCCX.SPI1_.Enable();
		GPIO(PA5).mode_af(GPIO_AF_SPI1);
		GPIO(PA6).mode_af(GPIO_AF_SPI1);
		GPIO(PA7).mode_af(GPIO_AF_SPI1);
	}
	else if(_spi == SPI2)
	{
		RCCX.SPI2_.Enable();
		GPIO(PB13).mode_af(GPIO_AF_SPI2);
		GPIO(PB14).mode_af(GPIO_AF_SPI2);
		GPIO(PB15).mode_af(GPIO_AF_SPI2);	
	}
	else if(_spi == SPI3)
	{
		RCCX.SPI3_.Enable();
		GPIO(PC10).mode_af(GPIO_AF_SPI3);
		GPIO(PC11).mode_af(GPIO_AF_SPI3);
		GPIO(PC12).mode_af(GPIO_AF_SPI3);	
	}
		
	//CS初始化
	cs.mode_out();
	cs = 1;
	

	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_CPHA 		= (latch_edge == Rising_Edge) ?                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        SPI_CPHA_2Edge:SPI_CPHA_1Edge;					//串行同步时钟的第二个跳变沿(上升沿）数据被采样
	SPI_InitStructure.SPI_FirstBit 	= (firstbit == MSB_First ) ? SPI_FirstBit_MSB:SPI_FirstBit_LSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  	//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode		= SPI_Mode_Master;						//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize 	= SPI_DataSize_8b;						//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL 		= SPI_CPOL_High;						//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_NSS 		= SPI_NSS_Soft;							//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_CRCPolynomial = 7;								//CRC值计算的多项式
	SPI_Init(_spi, &SPI_InitStructure);  									//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	enable(true);
}



static void spi_clock_div(SPI_TypeDef* SPIx ,uint8_t div)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(div));//判断有效性
	SPIx->CR1&=0XFFC7;	//位3-5清零，用来设置波特率
	SPIx->CR1|=div;		//设置SPI1速度 
}

void SPIn::set_speed(uint8_t div)	const 
{
	spi_clock_div(_spi,div);
}
	
uint8_t SPIn::write(uint8_t c)const 
{
	while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_TXE) == RESET){}	//等待发送区空  
	SPI_I2S_SendData(_spi, c); 											//通过外设SPIx发送一个byte  数据	
	while (SPI_I2S_GetFlagStatus(_spi, SPI_I2S_FLAG_RXNE) == RESET){} 	//等待接收完一个byte  
	return SPI_I2S_ReceiveData(_spi); 									//返回通过SPIx最近接收的数据		
}



