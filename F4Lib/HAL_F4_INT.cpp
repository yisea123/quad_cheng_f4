#include "sys.h"

#include "sbus.h"
#include "ibus.h"

#include "Ano_DT.h"
#include "MiniBalance.h"
#include "Ano_OF.h"



extern "C" void UART4_IRQHandler(void)
{
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		sbus_recvbyte_event(UART4->DR);
	}

	if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)			//¿ÕÏÐÖÐ¶Ï
	{
		uint8_t Clear;

		Clear = UART4->SR;
		Clear = UART4->DR;
		Clear = Clear;
		sbus_frame_event();
	}
}


extern "C" void UART5_IRQHandler(void)
{
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		ANO_DT_Data_Receive_Prepare(UART5->DR);
	}

}

extern "C" void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		AnoOF_GetOneByte(USART3->DR);
	}
}

extern "C" void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		MiniBalance_Data_Prepare(USART2->DR);
	}

// 	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
// 	{
// 		ibus_recvbyte_event(USART2->DR);
// 		//hal.console.putc(USART2->DR);
// 	}
// 
// 	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)			//¿ÕÏÐÖÐ¶Ï
// 	{
// 		uint8_t Clear;
// 
// 		Clear = USART2->SR;
// 		Clear = USART2->DR;
// 		Clear = Clear;
// 		ibus_frame_event();
// 	}

}


