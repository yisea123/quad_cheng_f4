#include "USART.h"
#include "GPIO.h"
#include "RCCX.h"



void USART::Setup(USART_TypeDef* usartx)
{
	if(usartx == USART1)	
	{
		RCCX.USART1_.Enable();
// 		GPIO::ModeAF(PA9,GPIO_AF_USART1);		//TX
// 		GPIO::ModeAF(PA10,GPIO_AF_USART1);		

 		GPIO::ModeAF(PB6,GPIO_AF_USART1);		//TX
 		GPIO::ModeAF(PB7,GPIO_AF_USART1);	
		
	}
	else if(usartx == USART2)
	{
		RCCX.USART2_.Enable();
		GPIO::ModeAF(PA2,GPIO_AF_USART2);		//TX
		GPIO::ModeAF(PA3,GPIO_AF_USART2);		//RX
	}
	else if(usartx == USART3)		
	{
		RCCX.USART3_.Enable();
		GPIO::ModeAF(PB10,GPIO_AF_USART3);		//TX
		GPIO::ModeAF(PB11,GPIO_AF_USART3);		//RX
	}
	else if(usartx == UART4)		
	{
		RCCX.UART4_.Enable();
		//GPIO::ModeAF(PC10,GPIO_AF_UART4);		//TX
		GPIO::ModeAF(PC11,GPIO_AF_UART4);		//RX
	}
	else if(usartx == UART5)	
	{
		RCCX.UART5_.Enable();
		GPIO::ModeAF(PC12,GPIO_AF_UART5);		//TX
		GPIO::ModeAF(PD2,GPIO_AF_UART5);		//RX
	}
	else 	
		return;
}

void USART::begin(uint32_t baudrate)
{
	set_baudrate(baudrate);
	USART_Init(_usartx, &_USART_InitStructure); 		//USART设置
	USART_Cmd(_usartx, ENABLE); 						//使能串口	
	add_interrupt(USART::IT_RXNE);						//接收中断使能
}
void USART::open()
{
	USART_Init(_usartx, &_USART_InitStructure); 		//USART设置
	USART_Cmd(_usartx, ENABLE); 						//使能串口	
}


void USART::close()
{
	USART_Cmd(_usartx, DISABLE); 
}
void USART::putc(uint8_t c)
{
	while(USART_GetFlagStatus(_usartx,USART_FLAG_TC)==RESET);
	USART_SendData(_usartx,c);
}
void USART::putbuf(uint8_t *buffer, uint16_t len)
{
	uint16_t i;
	for ( i = 0; i < len; i++)
	{
		putc(*buffer++);
	}
}
void USART::puts(char *s)
{
	while(*s){
		putc(*s++);
	}
	putc('\r');
	putc('\n');
}



// void USART::Send(void *buf,uint16_t len)
// {
// 	uint16_t i;
// 	uint8_t *p = (uint8_t*)buf;
// 	
// 	for(i=0;i<len;i++)
// 	{
// 		_TxBuffer[_Tx_InIndex] = *p++;
// 		if(++_Tx_InIndex == USART_TxBuffer_Size)
// 		{
// 			_Tx_InIndex = 0;
// 		}
// 	}
// 	if(_Tx_State == 0)
// 	{
// 		_Tx_State = 1;
// 		USART_ITConfig(_usartx, USART_IT_TXE, ENABLE);//打开发送中断
// 	}
// 
// }
//==============================================================
// void USART::_TXE_CallBack()
// {
// 	if(_Tx_OutIndex == _Tx_InIndex)
// 	{
// 		_Tx_State = 0;
// 		USART_ITConfig(_usartx, USART_IT_TXE, DISABLE);//关闭发送中断
// 	}
// 	else
// 	{
// 		USART_SendData(_usartx,_TxBuffer[_Tx_OutIndex]);
// 		if(++_Tx_OutIndex == USART_TxBuffer_Size)
// 		{
// 			_Tx_OutIndex = 0;
// 		}
// 	}
// }
// 		
// void USART::_RXNE_CallBack(uint16_t c)
// {
// 	
// }
//==============================================================

#ifdef INSTANCE_USART1 
extern "C" void USART1_IRQHandler(void) //串口 1 中断服务程序
{	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		INSTANCE_USART1._RXNE_CallBack(USART_ReceiveData(USART1));
		
	}

	if(USART_GetITStatus(USART1,USART_IT_TXE) != RESET)
	{	
		INSTANCE_USART1._TXE_CallBack();
	}
}
#endif

