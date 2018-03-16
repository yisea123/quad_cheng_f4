#ifndef _SPIDRIVER_H_
#define _SPIDRIVER_H_


#include "sys.h"


class SPIDriver
{
public:	
	
	SPIDriver(SPI_TypeDef* spin):_spix(spin)
	{
		_SPI_InitStructure.SPI_Direction 	= SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
		_SPI_InitStructure.SPI_Mode 		= SPI_Mode_Master;					//设置SPI工作模式:设置为主SPI
		_SPI_InitStructure.SPI_DataSize 	= SPI_DataSize_8b;					//设置SPI的数据大小:SPI发送接收8位帧结构
		_SPI_InitStructure.SPI_CPOL 		= SPI_CPOL_High;					//串行同步时钟的空闲状态为高电平
		_SPI_InitStructure.SPI_CPHA 		= SPI_CPHA_2Edge;					//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
		_SPI_InitStructure.SPI_NSS 			= SPI_NSS_Soft;						//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
		_SPI_InitStructure.SPI_BaudRatePrescaler 	= SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
		_SPI_InitStructure.SPI_FirstBit 			= SPI_FirstBit_MSB;			//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
		_SPI_InitStructure.SPI_CRCPolynomial 		= 7;						//CRC值计算的多项式
	}
	
	~SPIDriver(){}	
	static void Setup(SPI_TypeDef* spix);	



	enum 
	{
		Direction_2Lines_FullDuplex = SPI_Direction_2Lines_FullDuplex,
		Direction_2Lines_RxOnly = SPI_Direction_2Lines_RxOnly,
		Direction_1Line_Rx = SPI_Direction_1Line_Rx,
		Direction_1Line_Tx = SPI_Direction_1Line_Tx
	};
	enum 
	{
		Mode_Master = SPI_Mode_Master,
		Mode_Slave = SPI_Mode_Slave
	};
	enum 
	{
		DataSize_16b = SPI_DataSize_16b,
		DataSize_8b = SPI_DataSize_8b
	};
	enum 
	{
		CPOL_Low = SPI_CPOL_Low,
		CPOL_High = SPI_CPOL_High,
	};
	enum
	{
		CPHA_1Edge = SPI_CPHA_1Edge,
		CPHA_2Edge = SPI_CPHA_2Edge
	};
	enum 
	{
		NSS_Soft = SPI_NSS_Soft,
		NSS_Hard = SPI_NSS_Hard
	};
	enum 
	{
		BaudRatePrescaler_2 = SPI_BaudRatePrescaler_2,
		BaudRatePrescaler_4 = SPI_BaudRatePrescaler_4,
		BaudRatePrescaler_8 = SPI_BaudRatePrescaler_8,
		BaudRatePrescaler_16 = SPI_BaudRatePrescaler_16,
		BaudRatePrescaler_32 = SPI_BaudRatePrescaler_32,
		BaudRatePrescaler_64 = SPI_BaudRatePrescaler_64,
		BaudRatePrescaler_128 = SPI_BaudRatePrescaler_128,
		BaudRatePrescaler_256 = SPI_BaudRatePrescaler_256,
	};
	enum 
	{
		FirstBit_MSB = SPI_FirstBit_MSB,
		FirstBit_LSB = SPI_FirstBit_LSB
	};
	
	enum 
	{
		NSSInternalSoft_Set = SPI_NSSInternalSoft_Set,
		NSSInternalSoft_Reset = SPI_NSSInternalSoft_Reset
	};
	enum 
	{
		CRC_Tx = SPI_CRC_Tx,
		CRC_Rx = SPI_CRC_Rx
	};
	enum 
	{
		Direction_Rx = SPI_Direction_Rx,
		Direction_Tx = SPI_Direction_Tx
	};




	//I2S
// 	enum 
// 	{
// 		Mode_SlaveTx = I2S_Mode_SlaveTx,
// 		Mode_SlaveRx = I2S_Mode_SlaveRx,
// 		Mode_MasterTx = I2S_Mode_MasterTx,
// 		Mode_MasterRx = I2S_Mode_MasterRx
// 	};
// 	enum 
// 	{
// 		Standard_Phillips = I2S_Standard_Phillips,
// 		Standard_MSB = I2S_Standard_MSB,
// 		Standard_LSB = I2S_Standard_LSB,
// 		Standard_PCMShort = I2S_Standard_PCMShort,
// 		Standard_PCMLong = I2S_Standard_PCMLong
// 	};
// 	enum 
// 	{
// 		DataFormat_16b = I2S_DataFormat_16b,
// 		DataFormat_16bextended = I2S_DataFormat_16bextended,
// 		DataFormat_24b = I2S_DataFormat_24b,
// 		DataFormat_32b = I2S_DataFormat_32b
// 	};
// 	enum
// 	{
// 		MCLKOutput_Enable = I2S_MCLKOutput_Enable,
// 		MCLKOutput_Disable = I2S_MCLKOutput_Disable
// 	};
// 	enum
// 	{
// 		AudioFreq_192k = I2S_AudioFreq_192k,
// 		AudioFreq_96k = I2S_AudioFreq_96k,
// 		AudioFreq_48k = I2S_AudioFreq_48k,
// 		AudioFreq_44k = I2S_AudioFreq_44k,
// 		AudioFreq_32k = I2S_AudioFreq_32k,
// 		AudioFreq_22k = I2S_AudioFreq_22k,
// 		AudioFreq_16k = I2S_AudioFreq_16k,
// 		AudioFreq_11k = I2S_AudioFreq_11k,
// 		AudioFreq_8k = I2S_AudioFreq_8k
// 	};
// 	enum
// 	{
// 		CPOL_Low = I2S_CPOL_Low,
// 		CPOL_High = I2S_CPOL_High
// 	};
	
	//IT
// 	enum
// 	{
// // 		SPI_I2S_IT_TXE = SPI_I2S_IT_TXE,
// // 		SPI_I2S_IT_RXNE = SPI_I2S_IT_RXNE,
// // 		SPI_I2S_IT_ERR = SPI_I2S_IT_ERR,
// // 		I2S_IT_UDR = I2S_IT_UDR,
// // 		SPI_I2S_IT_TIFRFE = SPI_I2S_IT_TIFRFE,
// // 
// // 		SPI_I2S_IT_OVR = SPI_I2S_IT_OVR,
// // 		SPI_IT_MODF = SPI_IT_MODF,
// // 		SPI_IT_CRCERR = SPI_IT_CRCERR,
// 
// 	};
	enum
	{
		FLAG_RXNE = SPI_I2S_FLAG_RXNE,
		FLAG_TXE = SPI_I2S_FLAG_TXE,
		FLAG_CHSIDE = I2S_FLAG_CHSIDE,
		FLAG_UDR = I2S_FLAG_UDR,
		FLAG_CRCERR = SPI_FLAG_CRCERR,
		FLAG_MODF = SPI_FLAG_MODF,
		FLAG_OVR = SPI_I2S_FLAG_OVR,
		FLAG_BSY = SPI_I2S_FLAG_BSY,
		FLAG_TIFRFE = SPI_I2S_FLAG_TIFRFE
	};
	
	enum
	{
		DMAReq_Tx = SPI_I2S_DMAReq_Tx,
		DMAReq_Rx = SPI_I2S_DMAReq_Rx
	};




	void set_direction(uint16_t direction)			{_SPI_InitStructure.SPI_Direction = direction;	}	
	void set_mode(uint16_t mode)					{_SPI_InitStructure.SPI_Mode = mode;			}	
	void set_datasize(uint16_t datasize)			{_SPI_InitStructure.SPI_DataSize = datasize;	}	
	void set_cpol(uint16_t cpol)					{_SPI_InitStructure.SPI_CPOL = cpol;			}	
	void set_cpha(uint16_t cpha)					{_SPI_InitStructure.SPI_CPHA = cpha;			}	
	void set_nss(uint16_t nss)						{_SPI_InitStructure.SPI_NSS = nss;				}	
	void set_baudrate_prescaler(uint16_t prescaler)	{_SPI_InitStructure.SPI_BaudRatePrescaler = prescaler;	}	
	void set_firstbit(uint16_t fistbit)				{_SPI_InitStructure.SPI_FirstBit = fistbit;		}	
	void set_crc_polynomial(uint16_t crc)			{_SPI_InitStructure.SPI_CRCPolynomial = crc;	}

	void add_interrupt(uint16_t it) {
		SPI_I2S_ITConfig(_spix, it, ENABLE);
	}
	void remove_interrupt(uint16_t it) {
		SPI_I2S_ITConfig(_spix, it, DISABLE);
	}
	bool check_interrupt(uint16_t it) {
		return SPI_I2S_GetITStatus(_spix, it) == SET ? true : false;
	}
	void clear_interrupt(uint16_t it){
		SPI_I2S_ClearITPendingBit(_spix, it);
	}
	bool get_flag(uint16_t flag) {
		return SPI_I2S_GetFlagStatus(_spix, flag) == SET ? true : false;
	}
	void clear_flag(uint16_t flag) {
		SPI_I2S_ClearFlag(_spix, flag);
	}

	void enable_dmaTx(bool en) {
		SPI_I2S_DMACmd(_spix,SPIDriver::DMAReq_Tx  ,en ? ENABLE:DISABLE);
	}
	void enable_dmaRx(bool en) {
		SPI_I2S_DMACmd(_spix, SPIDriver::DMAReq_Rx, en ? ENABLE : DISABLE);
	}

		
	

	void open();
	void close();
	uint16_t write(uint16_t c);
	uint16_t read(){return write(0xFF);}
	
	void write(uint8_t *pbuf,uint32_t len);
	void read(uint8_t *pbuf,uint32_t len);
	
	uint8_t read_reg(uint8_t reg_addr);
	void read_reg(uint8_t reg_addr, uint8_t *pbuf, uint8_t len);

	void write_reg(uint8_t reg_addr,uint8_t val);
	void write_reg(uint8_t reg_addr, uint8_t *pbuf, uint8_t len);


	
	
	
private:
	SPI_TypeDef* _spix;
	SPI_InitTypeDef  _SPI_InitStructure;
};


#endif

