#ifndef _DATA_TRANSFER_H
#define	_DATA_TRANSFER_H

#include "sys.h"


// #ifdef __cplusplus
// extern "C" {
// #endif /* __cplusplus */

typedef struct
{
    uint8_t msg_id;
    uint8_t msg_data;
    uint8_t send_check;
    uint8_t send_version;
    uint8_t send_status;
    uint8_t send_senser;
    uint8_t send_senser2;
    uint8_t send_pid1;
    uint8_t send_pid2;
    uint8_t send_pid3;
    uint8_t send_pid4;
    uint8_t send_pid5;
    uint8_t send_pid6;
    uint8_t send_rcdata;
    uint8_t send_offset;
    uint8_t send_motopwm;
    uint8_t send_power;
    uint8_t send_user;
    uint8_t send_speed;
    uint8_t send_location;
	uint8_t send_vef;
	uint16_t send_parame;

	uint8_t checkdata_to_send;
	uint8_t checksum_to_send;
} dt_flag_t;

extern dt_flag_t f;

extern char Ano_StringBuffer[100];
#define ANO_Printf(fmt, ...) 	do{uint8_t len = sprintf(Ano_StringBuffer,fmt, ##__VA_ARGS__);ANO_DT_SendString(Ano_StringBuffer,len);}while(0)


void ANO_DT_Data_Receive_Anl_Task(void);
void ANO_DT_Send_VER(void);
void ANO_DT_Data_Exchange(void);
void ANO_DT_Data_Receive_Prepare(uint8_t data);
void ANO_DT_Data_Receive_Anl(uint8_t *data_buf,uint8_t num);
void ANO_DT_Send_Version(uint8_t hardware_type, uint16_t hardware_ver,uint16_t software_ver,uint16_t protocol_ver,uint16_t bootloader_ver);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, int32_t alt, uint8_t fly_model, uint8_t armed);
void ANO_DT_Send_Senser(int16_t a_x,int16_t a_y,int16_t a_z,int16_t g_x,int16_t g_y,int16_t g_z,int16_t m_x,int16_t m_y,int16_t m_z);
void ANO_DT_Send_Senser2(int32_t bar_alt,int32_t csb_alt);
void ANO_DT_Send_RCData(uint16_t thr,uint16_t yaw,uint16_t rol,uint16_t pit,uint16_t aux1,uint16_t aux2,uint16_t aux3,uint16_t aux4,uint16_t aux5,uint16_t aux6);
void ANO_DT_Send_Power(uint16_t votage, uint16_t current);
void ANO_DT_Send_MotoPWM(uint16_t m_1,uint16_t m_2,uint16_t m_3,uint16_t m_4,uint16_t m_5,uint16_t m_6,uint16_t m_7,uint16_t m_8);
void ANO_DT_Send_PID(uint8_t group,float p1_p,float p1_i,float p1_d,float p2_p,float p2_i,float p2_d,float p3_p,float p3_i,float p3_d);
void ANO_DT_Send_User(void);
void ANO_DT_Send_Speed(float,float,float);
void ANO_DT_Send_Location(uint8_t state,uint8_t sat_num,int32_t lon,int32_t lat,float back_home_angle);
void ANO_DT_Send_Msg(uint8_t id, uint8_t data);
void ANO_DT_Send_Check(uint8_t head, uint8_t check_sum);
//void ANO_DT_SendCenterPos(float x,float y,float z);
void ANO_DT_SendParame(uint16_t num);
void ANO_DT_SetParame(uint16_t num,uint32_t data);
void ANO_DT_SendString(char *str, uint8_t len);


// #ifdef __cplusplus
// }
// #endif /* __cplusplus */

#endif

