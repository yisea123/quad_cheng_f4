/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * 作者    ：匿名科创
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595r
 * 描述    ：数据传输
**********************************************************************************/
#include "Ano_DT.h"
#include "HAL_F4.h"

#include "state.h"
#include "Parameters.h"


#define ANO_DT_USE_USART(_buf,_len)		hal.uart5.putbuf(_buf,_len) //hal.console.putbuf(_buf,_len)
//#define ANO_DT_USE_USB_HID


#define HW_TYPE	05
#define HW_VER	1
#define SOFT_VER 32
#define BL_VER	0
#define PT_VER	400


/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp) + 0) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )



dt_flag_t f;			//需要发送数据的标志
uint8_t data_to_send[50];											//发送数据缓存
static uint8_t DT_RxBuffer[50], DT_data_cnt = 0, ano_dt_data_ok;	//接收数据缓存

/////////////////////////////////////////////////////////////////////////////////////
//Send_Data函数是协议中所有发送数据功能使用到的发送函数
//移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
static void ANO_DT_Send_Data(uint8_t *dataToSend , uint8_t length)
{
#ifdef ANO_DT_USE_USB_HID
	ANO_DT_USE_USB_HID(data_to_send,length);
#endif

#ifdef ANO_DT_USE_USART
	ANO_DT_USE_USART(data_to_send, length);
#endif
}


void ANO_DT_Data_Receive_Anl_Task()
{
	if(ano_dt_data_ok)
	{
		ANO_DT_Data_Receive_Anl(DT_RxBuffer,DT_data_cnt+5);
		ano_dt_data_ok = 0;
	}
}
void ANO_DT_Data_Receive_Prepare(uint8_t data)
{
	static uint8_t _data_len = 0;
	static uint8_t state = 0;

	if (state == 4 && _data_len > 0)
	{
		_data_len--;
		DT_RxBuffer[4 + DT_data_cnt++] = data;
		if (_data_len == 0)
			state = 5;
	}
	else if (state == 0 && data == 0xAA)
	{
		state = 1;
		DT_RxBuffer[0] = data;
	}
	else if(state==1)
	{
		if (data == 0xAF)
		{
			state = 2;
			DT_RxBuffer[1] = data;
		}
		else if (data != 0xAA)
		{
			state = 0;
		}
		
	}
	else if(state==2&&data<0XF1)
	{
		state=3;
		DT_RxBuffer[2]=data;
	}
	else if(state==3&&data<50)
	{
		state = 4;
		DT_RxBuffer[3]=data;
		_data_len = data;
		DT_data_cnt = 0;
	}
	else if(state==5)
	{
		state = 0;
		DT_RxBuffer[4+DT_data_cnt]=data;
		ano_dt_data_ok = 1;
		//ANO_DT_Data_Receive_Anl(DT_RxBuffer,DT_data_cnt+5);
	}
	else  state = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
#define pid_val(_off)		1e-3f*(int16_t)((data_buf[4 + (_off << 1)] << 8) | data_buf[5 + (_off << 1)])
void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num)
{
	uint8_t sum = 0;


	for(uint8_t i=0;i<(num-1);i++)	sum += *(data_buf+i);

	if(!(sum==*(data_buf+num-1)))						return;		//判断sum
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		//判断帧头

																	
	//CMD1
	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X01)					//加速度计校准
		{
			//printf("cali acc\r\n");
			state.calibration_accel = true;
		}
		else if (*(data_buf + 4) == 0X02)		//陀螺仪校准
		{
			//printf("cali gyro\r\n");
			state.calibration_gyro = true;
		}
		else if(*(data_buf+4)==0X03)			//ALT校准(仅用于光流模块)
		{
			
		}
		else if(*(data_buf+4)==0X04)			//磁力计校准
		{
			//printf("cali mag\r\n");
			state.compass_mot = true;
		}
		else if((*(data_buf+4)>=0X021)&&(*(data_buf+4)<=0X26))//6面校准1-6
		{
			//printf("3d cali\r\n");
		}
		else if(*(data_buf+4)==0X20)					 //退出6面校准
		{
			//printf("exit 3d cali\r\n");
		}
	}
	//CMD2
	else if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)				//读取PID请求
		{
			//printf("get pid\r\n");
			f.send_pid1 = 1;
			f.send_pid2 = 1;
			f.send_pid3 = 1;
			f.send_pid4 = 1;
			f.send_pid5 = 1;
			f.send_pid6 = 1;

		}
		if(*(data_buf+4)==0X02)				//读取飞行模式
		{
			//printf("flight mode");
		}
		if(*(data_buf+4)==0XA0)		//读取版本信息
		{
			//printf("get version\r\n");
			f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)		//恢复默认PID
		{
			//printf("reset PID\r\n");
		}
		if(*(data_buf+4)==0XA2)		//恢复默认参数
		{
			//printf("reset param\r\n");
		}
	}
	//CMD3,RC数据
	else if(*(data_buf+2)==0X03)
	{
		//printf("set RC Data\r\n");
//		RX_CH[THR] = (int16_t)(*(data_buf+4)<<8)|*(data_buf+5) ;
//		RX_CH[YAW] = (int16_t)(*(data_buf+6)<<8)|*(data_buf+7) ;
//		RX_CH[ROL] = (int16_t)(*(data_buf+8)<<8)|*(data_buf+9) ;
//		RX_CH[PIT] = (int16_t)(*(data_buf+10)<<8)|*(data_buf+11) ;
//		RX_CH[AUX1] = (int16_t)(*(data_buf+12)<<8)|*(data_buf+13) ;
//		RX_CH[AUX2] = (int16_t)(*(data_buf+14)<<8)|*(data_buf+15) ;
//		RX_CH[AUX3] = (int16_t)(*(data_buf+16)<<8)|*(data_buf+17) ;
//		RX_CH[AUX4] = (int16_t)(*(data_buf+18)<<8)|*(data_buf+19) ;
	}
	//读取参数
	else if(*(data_buf+2)==0X08)		
	{
		f.send_parame = (uint16_t)(*(data_buf+4)<<8)|*(data_buf+5) ;
		//printf("get param=%d\r\n", f.send_parame);
	}
	//设置参数
	else if(*(data_buf+2)==0X09)		
	{
		uint16_t num;
		uint32_t dat;
		num = (uint16_t)(*(data_buf+4)<<8)|*(data_buf+5) ;
		dat = (uint32_t)(((*(data_buf+6))<<24) + ((*(data_buf+7))<<16) + ((*(data_buf+8))<<8) + (*(data_buf+9)));
		
		//printf("set param:%d=%d\r\n", num, dat);
		if(f.send_check == 0)
		{
			f.send_check = 1;
			f.checkdata_to_send = *(data_buf+2);
			f.checksum_to_send = sum;
		}
		
		ANO_DT_SetParame(num,dat);
	}
	//////////////////////////////////////////////////////////////////
	//PID
	else if(*(data_buf+2)==0X10)								
    {
		//printf("set PID1\r\n");
		if (f.send_check == 0)
		{
			f.send_check = 1;
			f.checkdata_to_send = *(data_buf + 2);
			f.checksum_to_send = sum;
		}

		g.pid_rate_roll.kP(pid_val(0));
		g.pid_rate_roll.kI(pid_val(1));
		g.pid_rate_roll.kD(pid_val(2));
		g.pid_rate_roll.save();


		g.pid_rate_pitch.kP(pid_val(3));
		g.pid_rate_pitch.kI(pid_val(4));
		g.pid_rate_pitch.kD(pid_val(5));
		g.pid_rate_pitch.save();

		g.pid_rate_yaw.kP(pid_val(6));
		g.pid_rate_yaw.kI(pid_val(7));
		g.pid_rate_yaw.kD(pid_val(8));
		g.pid_rate_yaw.save();



    }
	else if(*(data_buf+2)==0X11)								//PID2
    {
		//printf("set PID2\r\n");
		if (f.send_check == 0)
		{
			f.send_check = 1;
			f.checkdata_to_send = *(data_buf + 2);
			f.checksum_to_send = sum;
		}

		g.p_angle_roll.kP(pid_val(0));
		g.p_angle_roll.kI(0);
		g.p_angle_roll.kD(0);
		g.p_angle_roll.save();


		g.p_angle_pitch.kP(pid_val(3));
		g.p_angle_pitch.kI(0);
		g.p_angle_pitch.kD(0);
		g.p_angle_pitch.save();

		g.p_angle_yaw.kP(pid_val(6));
		g.p_angle_yaw.kI(0);
		g.p_angle_yaw.kD(0);
		g.p_angle_yaw.save();

    }
	else if(*(data_buf+2)==0X12)								//PID3
    {	
		//printf("set PID3\r\n");
		if (f.send_check == 0)
		{
			f.send_check = 1;
			f.checkdata_to_send = *(data_buf + 2);
			f.checksum_to_send = sum;
		}


    }
	else if(*(data_buf+2)==0X13)								//PID4
	{
		//printf("set PID4\r\n");
		if (f.send_check == 0)
		{
			f.send_check = 1;
			f.checkdata_to_send = *(data_buf + 2);
			f.checksum_to_send = sum;
		}


	}
	else if(*(data_buf+2)==0X14)								//PID5
	{
		//printf("set PID5\r\n");
		if (f.send_check == 0)
		{
			f.send_check = 1;
			f.checkdata_to_send = *(data_buf + 2);
			f.checksum_to_send = sum;
		}


	}
	else if(*(data_buf+2)==0X15)								//PID6
	{
		//printf("set PID6\r\n");
		if (f.send_check == 0)
		{
			f.send_check = 1;
			f.checkdata_to_send = *(data_buf + 2);
			f.checksum_to_send = sum;
		}

	}
}

///////////////////////////////////////////////////////////////////////////////////////


void ANO_DT_Send_Check(uint8_t head, uint8_t check_sum)
{
	data_to_send[0] = 0xAA;
	data_to_send[1] = 0xAA;
	data_to_send[2] = 0xEF;
	data_to_send[3] = 2;
	data_to_send[4] = head;
	data_to_send[5] = check_sum;


	uint8_t sum = 0;
	for (uint8_t i = 0; i < 6; i++)
		sum += data_to_send[i];
	data_to_send[6] = sum;

	ANO_DT_Send_Data(data_to_send, 7);
}

void ANO_DT_Send_Msg(uint8_t id, uint8_t data)
{
	data_to_send[0] = 0xAA;
	data_to_send[1] = 0xAA;
	data_to_send[2] = 0xEE;
	data_to_send[3] = 2;
	data_to_send[4] = id;
	data_to_send[5] = data;


	uint8_t sum = 0;
	for (uint8_t i = 0; i < 6; i++)
		sum += data_to_send[i];
	data_to_send[6] = sum;

	ANO_DT_Send_Data(data_to_send, 7);
}

void ANO_DT_Send_VER(void)
{
	uint8_t temp[14];
	temp[0] = 0xAA;
	temp[1] = 0xAA;
	temp[2] = 0x00;
	temp[3] = 9;
	temp[4] = HW_TYPE;
	temp[5] = HW_VER/256;
	temp[6] = HW_VER%256;
	temp[7] = SOFT_VER/256;
	temp[8] = SOFT_VER%256;
	temp[9] = PT_VER/256;
	temp[10] = PT_VER%256;
	temp[11] = BL_VER/256;
	temp[12] = BL_VER%256;
	uint8_t check_sum = 0;
	for(uint8_t i=0;i<13;i++)
		check_sum += temp[i];
	temp[13] = check_sum;
	
	ANO_DT_Send_Data(temp,14);
}

void ANO_DT_Send_Version(uint8_t hardware_type, uint16_t hardware_ver,uint16_t software_ver,uint16_t protocol_ver,uint16_t bootloader_ver)
{
	uint8_t _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x00;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=hardware_type;
	data_to_send[_cnt++]=BYTE1(hardware_ver);
	data_to_send[_cnt++]=BYTE0(hardware_ver);
	data_to_send[_cnt++]=BYTE1(software_ver);
	data_to_send[_cnt++]=BYTE0(software_ver);
	data_to_send[_cnt++]=BYTE1(protocol_ver);
	data_to_send[_cnt++]=BYTE0(protocol_ver);
	data_to_send[_cnt++]=BYTE1(bootloader_ver);
	data_to_send[_cnt++]=BYTE0(bootloader_ver);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}
//in cm
void ANO_DT_Send_Speed(float x_s,float y_s,float z_s)
{
	uint8_t _cnt=0;
	int16_t _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x0B;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(0.1f *x_s);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(0.1f *y_s);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(0.1f *z_s);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);

}

void ANO_DT_Send_Location(uint8_t state,uint8_t sat_num,int32_t lon,int32_t lat,float back_home_angle)
{
	uint8_t _cnt=0;
	int16_t _temp;
	int32_t _temp2;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x04;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=state;
	data_to_send[_cnt++]=sat_num;
	
	_temp2 = lon;//经度
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);	
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	_temp2 = lat;//纬度
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);	
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	
	_temp = (int16_t)(100 *back_home_angle);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);

}
//in degree*100
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, int32_t alt, uint8_t fly_model, uint8_t armed)
{
	uint8_t _cnt=0;
	int16_t _temp;
	int32_t _temp2 = alt;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(angle_rol*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_pit*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(angle_yaw*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	data_to_send[_cnt++] = fly_model;
	
	data_to_send[_cnt++] = armed;
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_Senser(int16_t a_x,int16_t a_y,int16_t a_z,int16_t g_x,int16_t g_y,int16_t g_z,int16_t m_x,int16_t m_y,int16_t m_z)
{
	uint8_t _cnt=0;
	int16_t _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	
	_temp = a_x;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = a_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = g_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = g_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = m_x;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_y;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = m_z;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
/////////////////////////////////////////
	_temp = 0;	
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}
//in cm*100
void ANO_DT_Send_Senser2(int32_t bar_alt,int32_t csb_alt)
{
	uint8_t _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x07;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE3(bar_alt);
	data_to_send[_cnt++]=BYTE2(bar_alt);
	data_to_send[_cnt++]=BYTE1(bar_alt);
	data_to_send[_cnt++]=BYTE0(bar_alt);

	data_to_send[_cnt++]=BYTE3(csb_alt);
	data_to_send[_cnt++]=BYTE2(csb_alt);
	data_to_send[_cnt++]=BYTE1(csb_alt);
	data_to_send[_cnt++]=BYTE0(csb_alt);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_RCData(uint16_t thr,uint16_t yaw,uint16_t rol,uint16_t pit,uint16_t aux1,uint16_t aux2,uint16_t aux3,uint16_t aux4,uint16_t aux5,uint16_t aux6)
{
	uint8_t _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(thr);
	data_to_send[_cnt++]=BYTE0(thr);
	data_to_send[_cnt++]=BYTE1(yaw);
	data_to_send[_cnt++]=BYTE0(yaw);
	data_to_send[_cnt++]=BYTE1(rol);
	data_to_send[_cnt++]=BYTE0(rol);
	data_to_send[_cnt++]=BYTE1(pit);
	data_to_send[_cnt++]=BYTE0(pit);
	data_to_send[_cnt++]=BYTE1(aux1);
	data_to_send[_cnt++]=BYTE0(aux1);
	data_to_send[_cnt++]=BYTE1(aux2);
	data_to_send[_cnt++]=BYTE0(aux2);
	data_to_send[_cnt++]=BYTE1(aux3);
	data_to_send[_cnt++]=BYTE0(aux3);
	data_to_send[_cnt++]=BYTE1(aux4);
	data_to_send[_cnt++]=BYTE0(aux4);
	data_to_send[_cnt++]=BYTE1(aux5);
	data_to_send[_cnt++]=BYTE0(aux5);
	data_to_send[_cnt++]=BYTE1(aux6);
	data_to_send[_cnt++]=BYTE0(aux6);

	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}

// *100
void ANO_DT_Send_Power(uint16_t votage, uint16_t current)
{
	uint8_t _cnt=0;
	uint16_t temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0;
	
	temp = votage;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	temp = current;
	data_to_send[_cnt++]=BYTE1(temp);
	data_to_send[_cnt++]=BYTE0(temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_MotoPWM(uint16_t m_1,uint16_t m_2,uint16_t m_3,uint16_t m_4,uint16_t m_5,uint16_t m_6,uint16_t m_7,uint16_t m_8)
{
	uint8_t _cnt=0;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	
	data_to_send[_cnt++]=BYTE1(m_1);
	data_to_send[_cnt++]=BYTE0(m_1);
	data_to_send[_cnt++]=BYTE1(m_2);
	data_to_send[_cnt++]=BYTE0(m_2);
	data_to_send[_cnt++]=BYTE1(m_3);
	data_to_send[_cnt++]=BYTE0(m_3);
	data_to_send[_cnt++]=BYTE1(m_4);
	data_to_send[_cnt++]=BYTE0(m_4);
	data_to_send[_cnt++]=BYTE1(m_5);
	data_to_send[_cnt++]=BYTE0(m_5);
	data_to_send[_cnt++]=BYTE1(m_6);
	data_to_send[_cnt++]=BYTE0(m_6);
	data_to_send[_cnt++]=BYTE1(m_7);
	data_to_send[_cnt++]=BYTE0(m_7);
	data_to_send[_cnt++]=BYTE1(m_8);
	data_to_send[_cnt++]=BYTE0(m_8);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_PID(uint8_t group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d)
{
	uint8_t _cnt=0;
	int16_t _temp;
	
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10+group-1;
	data_to_send[_cnt++]=0;
	
	
	_temp = p1_p * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p1_d  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p2_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_p  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_i  * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = p3_d * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_SendParame(uint16_t num)
{
	uint8_t _cnt=0;
	int32_t data;
// 	switch(num)
// 	{
// 		case 1:
// 			data = Ano_Parame.set.pwmInMode;
// 			break;
// 		case 10:
// 			data = Ano_Parame.set.warn_power_voltage * 10;
// 			break;
// 		case 11:
// 			data = Ano_Parame.set.return_home_power_voltage * 10;
// 			break;
// 		case 12:
// 			data = Ano_Parame.set.lowest_power_voltage * 10;
// 			break;
// 		case 13:
// 			data = Ano_Parame.set.center_pos_cm[X] * 10;
// 			break;
// 		case 14:
// 			data = Ano_Parame.set.center_pos_cm[Y] * 10;
// 			break;
// 		case 15:
// 			data = Ano_Parame.set.center_pos_cm[Z] * 10;
// 			break;
// 		case 16:
// 			data = Ano_Parame.set.auto_take_off_height;
// 			break;
// 		default:
// 			data = 0;
// 			break;
// 	}
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x09;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(num);
	data_to_send[_cnt++]=BYTE0(num);
	data_to_send[_cnt++]=BYTE3(data);
	data_to_send[_cnt++]=BYTE2(data);
	data_to_send[_cnt++]=BYTE1(data);
	data_to_send[_cnt++]=BYTE0(data);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_SetParame(uint16_t num,uint32_t data)
{
// 	switch(num)
// 	{
// 		case 1:
// 			Ano_Parame.set.pwmInMode = data;
// 			break;
// 		case 10:
// 			Ano_Parame.set.warn_power_voltage = (float)(data) / 10;
// 			break;
// 		case 11:
// 			Ano_Parame.set.return_home_power_voltage = (float)(data) / 10;
// 			break;
// 		case 12:
// 			Ano_Parame.set.lowest_power_voltage = (float)(data) / 10;
// 			break;
// 		case 13:
// 			Ano_Parame.set.center_pos_cm[X] = (float)((int32_t)data) / 10;
// 			break;
// 		case 14:
// 			Ano_Parame.set.center_pos_cm[Y] = (float)((int32_t)data) / 10;
// 			break;
// 		case 15:
// 			Ano_Parame.set.center_pos_cm[Z] = (float)((int32_t)data) / 10;
// 			Center_Pos_Set();
// 			break;
// 		case 16:
// 			Ano_Parame.set.auto_take_off_height = (float)(data);
// 			break;
// 		default:
// 			break;
// 	}
// 	f.send_parame = num;
// 	data_save();
}

void ANO_DT_SendString(char *str, uint8_t len)
{
	uint8_t _cnt=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xE0;
	data_to_send[_cnt++]=0;
	for(uint8_t i=0; i<len; i++)
		data_to_send[_cnt++] = *(str+i);
	
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_User()
{
	uint8_t _cnt=0;
	int16_t _temp;
	
	data_to_send[_cnt++]=0xAA; 
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xf1; //用户数据
	data_to_send[_cnt++]=0;
////////////////////////////////////////	

	//extern int32_t sensor_val_ref[];
	//_temp = (int16_t)(sensor_val_ref[A_Z]);//          //1
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	

	//_temp = (int16_t)(wcz_acc_fus.out);//         //2
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	
	
	//_temp = (int16_t)(wcz_spe_fus.out);//         //3
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	
	
	//_temp = (int16_t)(wcz_hei_fus.out);//         //4
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	
	
	//_temp = (int16_t)(wcz_ref_speed);//         //5
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	

	//_temp = (int16_t)(wcz_ref_height);//         //6
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
//	extern float speed_test[];
//	_temp = (int16_t)(speed_test[0]);//         //7
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
////////////////////////////////////////
	data_to_send[3] = _cnt-4;
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;

	ANO_DT_Send_Data(data_to_send, _cnt);
}


/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
