#ifndef _MINI_BALANCE_H_
#define  _MINI_BALANCE_H_

#include "sys.h"
#include "HAL_F4.h"



#define MiniBalance_Send(_buf,_len)		hal.usart2.putbuf(_buf,_len) //hal.console.putbuf(_buf,_len)



enum 
{
	MB_DIR_TOP = 'A',
	MB_DIR_TOP_RIGHT = 'B',
	
	MB_DIR_RIGHT = 'C',
	MB_DIR_BOTTOM_RIGHT = 'D',

	MB_DIR_BOTTOM = 'E',
	MB_DIR_BOTTOM_LEFT = 'F',

	MB_DIR_LEFT = 'G',
	MB_DIR_TOP_LEFT = 'H',

	MB_DIR_CENTER = 'Z',
};

enum 
{
	MB_SPEED_LOW = 0,
	MB_SPEED_HIGH
};


typedef struct  
{
	uint8_t direction;
	uint8_t speed;

	uint16_t speed_change : 1;
	uint16_t direction_change : 1;
	uint16_t get_param : 1;


	uint16_t send_parameter : 1;
	uint16_t save_parameter : 1;
	uint16_t send_wave : 1;


	//自定义按键
	uint16_t u1 : 1;
	uint16_t u2 : 1;
	uint16_t u3 : 1;
	uint16_t u4 : 1;
	uint16_t u5 : 1;
	uint16_t u6 : 1;
	uint16_t u7 : 1;
	uint16_t u8 : 1;
	uint16_t u9 : 1;



 	uint32_t param[9];


}MiniBalanceFlag_T;

extern MiniBalanceFlag_T MiniBalance_Flag;


void MiniBalance_Wave(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
void MiniBalance_SendString(char *s);
void MiniBalance_SendParameter(void);

void MiniBalance_Data_Prepare(uint8_t c);
void MiniBalance_Recv_Task(void);





#endif

