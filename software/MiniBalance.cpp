#include "MiniBalance.h"
#include "string.h"

static char minibalance_rx_buf[100];
static uint8_t minibalance_rx_cnt = 0;
static uint8_t minibalance_tx_buf[100];

MiniBalanceFlag_T MiniBalance_Flag;
static uint8_t param_ok;


void MiniBalance_Wave(int32_t a,int32_t b,int32_t c,int32_t d,int32_t e)
{
	int len = sprintf((char*)minibalance_tx_buf, "{B%d:%d:%d:%d:%d}$", a, b, c, d, e);
	MiniBalance_Send(minibalance_tx_buf, len);
}

void MiniBalance_SendString(char *s)
{
	int len = sprintf((char*)minibalance_tx_buf,"{#%s}$", s);
	MiniBalance_Send(minibalance_tx_buf, len);
}

void MiniBalance_SendParameter()
{
	uint32_t *value = MiniBalance_Flag.param;
	int len = sprintf((char*)minibalance_tx_buf, "{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$", value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[8]);
 	MiniBalance_Send(minibalance_tx_buf,len);
}



void MiniBalance_Recv_Task()
{
	static uint32_t *value = MiniBalance_Flag.param;
	uint8_t idx;

	//参数解析
	if (param_ok)
	{
		param_ok = 0;
		if (minibalance_rx_buf[1] >= '0' && minibalance_rx_buf[1] <= '8')	//单个参数
		{
			idx = minibalance_rx_buf[1] - '0';
			if (sscanf(minibalance_rx_buf + 3, "%d}", value + idx))
			{
				MiniBalance_Flag.get_param = 1;
			}
		}
		else if (minibalance_rx_buf[1] == '#')//所有参数
		{
			if (sscanf(minibalance_rx_buf, "{#%d:%d:%d:%d:%d:%d:%d:%d:%d}", value, value + 1, value + 2, value + 3, value + 4, value + 5, value + 6, value + 7, value + 8))
			{
				MiniBalance_Flag.get_param = 1;
			}
		}
		else if (minibalance_rx_buf[1] == 'Q')
		{
			if (minibalance_rx_buf[3] == 'P')				//获取设备参数
			{
				MiniBalance_Flag.send_parameter = 1;
			}
			else if (minibalance_rx_buf[3] == 'W')		//保存所有参数
			{
				MiniBalance_Flag.save_parameter = 1;
			}
		}
	}

}

//数据接收调用
void MiniBalance_Data_Prepare(uint8_t c)
{
	static uint8_t step = 0;
	switch(step)
	{
	case 0:
		switch (c)
		{
			//参数
			case '{':
				minibalance_rx_buf[0] = '{';
				minibalance_rx_cnt = 1;
				step = 1;
				break;
			//自定义按键
			case 'a':
				MiniBalance_Flag.u1 = 1;
				break;
			case 'b':
				MiniBalance_Flag.u2 = 1;
				break;
			case 'c':
				MiniBalance_Flag.u3 = 1;
				break;
			case 'd':
				MiniBalance_Flag.u4 = 1;
				break;
			case 'e':
				MiniBalance_Flag.u5 = 1;
				break;
			case 'f':
				MiniBalance_Flag.u6 = 1;
				break;
			case 'g':
				MiniBalance_Flag.u7 = 1;
				break;
			case 'h':
				MiniBalance_Flag.u8 = 1;
				break;
			case 'i':
				MiniBalance_Flag.u9 = 1;
				break;
			//方向
			case 'X':
				MiniBalance_Flag.speed = MB_SPEED_HIGH;
				MiniBalance_Flag.speed_change = 1;

				break;
			case  'Y':
				MiniBalance_Flag.speed = MB_SPEED_LOW;
				MiniBalance_Flag.speed_change = 1;
				break;
			default:
				if((c >= 'A' && c <= 'H') || c == 'Z')		//方向盘
				{
					MiniBalance_Flag.direction = c;
					MiniBalance_Flag.direction_change = 1;
				}
				break;
		}
		break;
	case 1:
		if (c == '{')	//出错
		{
			minibalance_rx_buf[0] = '{';
			minibalance_rx_cnt = 1;
		}
		else
		{
			minibalance_rx_buf[minibalance_rx_cnt] = c;
			minibalance_rx_cnt++;
			if (c == '}')//结束
			{
				minibalance_rx_buf[minibalance_rx_cnt] = '\0';
				param_ok = 1;
				step = 0;
			}
		}
		break;
	default:
		step = 0;
		break;
	}
}

//定时调用
// void MiniBalance_Data_Exchange()
// {
// 	static uint8_t cnt = 0;
// 	if (cnt % 99 == 0)
// 	{
// 		MiniBalance_Flag.send_wave = 1;		//定时发送波形
// 	}
// 
// 
// 	if (++cnt == 200)cnt = 0;
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	//自定义按键
// 	if (MiniBalance_Flag.u1)
// 	{
// 		//		puts("u1");
// 		MiniBalance_Flag.u1 = 0;
// 	}
// 	if (MiniBalance_Flag.u2)
// 	{
// 		//		puts("u2");
// 		MiniBalance_Flag.u2 = 0;
// 	}
// 	if (MiniBalance_Flag.u3)
// 	{
// 		//		puts("u3");
// 		MiniBalance_Flag.u3 = 0;
// 	}
// 	if (MiniBalance_Flag.u4)
// 	{
// 		//		puts("u4");
// 		MiniBalance_Flag.u4 = 0;
// 	}
// 	if (MiniBalance_Flag.u5)
// 	{
// 		//		puts("u5");
// 		MiniBalance_Flag.u5 = 0;
// 	}
// 	if (MiniBalance_Flag.u6)
// 	{
// 		//		puts("u6");
// 		MiniBalance_Flag.u6 = 0;
// 	}
// 	if (MiniBalance_Flag.u7)
// 	{
// 		//		puts("u7");
// 		MiniBalance_Flag.u7 = 0;
// 	}
// 	if (MiniBalance_Flag.u8)
// 	{
// 		//		puts("u8");
// 		MiniBalance_Flag.u8 = 0;
// 	}
// 	if (MiniBalance_Flag.u9)
// 	{
// 		//		puts("u9");
// 		MiniBalance_Flag.u9 = 0;
// 	}
// 	//speed
// 	if (MiniBalance_Flag.speed_change)
// 	{
// 		MiniBalance_Flag.speed_change = 0;
// 		if (MiniBalance_Flag.speed == MB_SPEED_HIGH)
// 		{
// 			//puts("high speed");
// 		}
// 		else
// 		{
// 			//puts("low speed");
// 		}
// 	}
// 	//direction
// 	if (MiniBalance_Flag.direction_change)
// 	{
// 		MiniBalance_Flag.direction_change = 0;
// 		//printf("direction=%c\r\n", MiniBalance_Flag.direction);
// 	}
// 	//save parameter
// 	if (MiniBalance_Flag.save_parameter)
// 	{
// 		MiniBalance_Flag.save_parameter = 0;
// 	}
// 	//get parameter
// 	if (MiniBalance_Flag.get_param)
// 	{
// 
// 		MiniBalance_Flag.get_param = 0;
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	if (MiniBalance_Flag.send_parameter)
// 	{
// 		MiniBalance_Flag.send_parameter = 0;
// 		MiniBalance_SendParameter();
// 	}
// 	else if (MiniBalance_Flag.send_wave)
// 	{
// 		
// 		MiniBalance_Wave(0, 0, 0, 0, 0);
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	MiniBalance_Recv_Task();
// }
