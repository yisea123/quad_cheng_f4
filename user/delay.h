#pragma once

#include "sys.h"



#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
	 
	 

void delay_init(void);

uint32_t micros(void);
uint32_t millis(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);


	 
#ifdef __cplusplus
}
#endif /* __cplusplus */


