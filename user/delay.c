#include "delay.h"



static volatile uint32_t usTicks = 168;		//在初始化函数赋值
static volatile uint32_t sysTickUptime = 0;

// SysTick Interrupt
void SysTick_Handler(void)
{
    sysTickUptime++;
}


// Return system uptime in microseconds (rollover in 70minutes)
uint32_t micros(void)
{
    register uint32_t ms, cycle_cnt;
    do {
        ms = sysTickUptime;
        cycle_cnt = SysTick->VAL;
    } while (ms != sysTickUptime);
    return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

// Return system uptime in milliseconds (rollover in 49 days)
uint32_t millis(void)
{
    return sysTickUptime;
}



void delay_ms(uint16_t nms)
{
		volatile uint32_t t0=micros();
		while(micros() - t0 < nms * 1000);
			
}
void delay_us(uint32_t nus)
{
		volatile uint32_t t0 = micros();
		while((micros() - t0) < nus);
			
}

void delay_init(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	usTicks = RCC_Clocks.HCLK_Frequency/1000000;		//1us Ticks数
	SysTick_Config(usTicks*1000);										//1ms中断
}



