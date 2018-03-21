#include "sys.h"
#include "HAL_F4.h"

#include "QMath.h"

#include "InertialSensor_ICM20602.h"
#include "Compass_AK8975.h"
#include "Barometer_SPL06.h"


#include "Attitude.h"
#include "Parameters.h"

#include "Notify.h"
#include "AttitudeControl.h"
#include "PosControl.h"
#include "Motors.h"
#include "AHRS_Q.h"
#include "InertialNav.h"

#include "Ano_DT.h"
#include "MiniBalance.h"
#include "Ano_OF.h"

#include "sbus.h"
#include "ibus.h"

#include "failsafe.h"
#include "state.h"
#include "flight_mode.h"

#include "config.h"
#include "defines.h"


static Scheduler& scheduler = hal.scheduler;

state_t state;
Parameters g;


InertialSensor_ICM20602 ins(hal.spi3, GPIO(PA15));
Compass_AK8975 compass(hal.spi3, GPIO(PB14));
Barometer_SPL06 barometer(hal.spi3, GPIO(PB8));

static Notify notify;
AHRS_Q ahrs(ins, compass);
Motors motors(g.rcout_roll, g.rcout_pitch, g.rcout_throttle, g.rcout_yaw);
AttitudeControl attitude_control(ahrs, motors,
	g.p_angle_roll, g.p_angle_pitch, g.p_angle_yaw,
	g.pid_rate_roll, g.pid_rate_pitch, g.pid_rate_yaw);
InertialNav inertial_nav(ahrs);

PosControl pos_control(ahrs, motors, attitude_control,inertial_nav,
	g.p_alt_pos, g.p_alt_rate, g.pid_alt_accel,
	g.p_pos_xy, g.pid_rate_x, g.pid_rate_y);



//气压计高度,速率,in cm
float baro_alt, baro_climbrate;
//float sonar_alt;
int16_t target_climb_rate;

//////////////////////////////////////////////////////////////////////////
//loop
//////////////////////////////////////////////////////////////////////////
bool init_arm_motor()
{
	if (motors.armed()) {
		return true;
	}

	ahrs.set_fast_gains(false);
	ahrs.set_armed(true);
	motors.set_mid_throttle(g.throttle_mid);
	motors.arm();

	inertial_nav.ignore_next_error();	//???
	barometer.update_calibration();		//更新地面气压

	return true;
}
void init_disarm_motor()
{
	if (!motors.armed()){
		return;
	}
	motors.disarm();

	set_land_complete_maybe(true);
	set_land_complete(true);

	ahrs.set_fast_gains(true);
	ahrs.set_armed(false);

}
void pre_arm_rc_check()		//检查遥控器是否校准
{
	if(state.pre_arm_rc_check){
		return;
	}

	if (!g.arming_check&ARMING_CHECK_RC){
		set_pre_arm_rc_check(true);
		return;
	}

	//检查所有radio_min,radio_max,radio_mid


	set_pre_arm_rc_check(true);

}
void pre_arm_check(bool display_failure)//检测各传感器是否正常
{
	if (motors.armed())return;					//已解锁
	if (state.pre_arm_check) return;			//已检查

	if(g.arming_check == ARMING_CHECK_NONE)		//不检查
	{
		set_pre_arm_check(true);
		set_pre_arm_rc_check(true);
		return;
	}

	//check RC
	pre_arm_rc_check();				//遥控器校准检查
	if(!state.pre_arm_rc_check)		//遥控器没有校准
	{	
		if(display_failure){

		}
		return;
	}

// 	if(g.arming_check& ARMING_CHECK_VOLTAGE)
// 	{
// 
// 	}

	//check Baro
// 	if(g.arming_check&ARMING_CHECK_BARO)
// 	{
// 		if (!barometer.healthy())
// 		{
// 			if(display_failure){
// 
// 			}
// 			return;
// 		}
// 	}

	//check Compass
// 	if(g.arming_check&ARMING_CHECK_COMPASS)
// 	{
// 		if(!compass.healthy())
// 		{
// 			if (display_failure) {
// 
// 			}
// 			return;
// 		}
// 
// 		if(!compass.calibrated())
// 		{
// 			if (display_failure) {
// 
// 			}
// 			return;
// 		}
// 
// 	}


	//check INS
	if(g.arming_check&ARMING_CHECK_INS)
	{
		if (!ins.healthy())
		{
			if (display_failure) {

			}
			return;
		}

		if(!ins.calibrated())
		{
			if (display_failure) {

			}
			return;
		}
	}
								 //
	set_pre_arm_check(true);
}
bool arm_check(bool display_failure)//检测当前解锁状态
{
	if (motors.armed())return true;

	if (g.arming_check == ARMING_CHECK_NONE)
	{
		return true;
	}


	//check baro
// 	if(g.arming_check&ARMING_CHECK_BARO)
// 	{
// 
// 	}
	
	//check gps
// 	if (g.arming_check&ARMING_CHECK_GPS)
// 	{
// 
// 	}

	//check ins,倾角过大检测
	if(g.arming_check&ARMING_CHECK_INS)
	{
		if(fabs(ahrs.roll_sensor) > g.angle_max || fabs(ahrs.pitch_sensor) > g.angle_max)
		{
			if(display_failure){

			}
			return false;
		}
	}

	return true;
}
//10hz(100ms):解锁检测
void arm_check_loop(void)
{
//油门最小,方向最右->解锁
//油门最小,方向最左->上锁

#define  ARM_DELAY		15			//2s
#define  DISARM_DELAY	15			//2s

	static int16_t arming_count = 0;
	if (g.rcin_throttle.output > 0)//油门非0,退出
	{
		arming_count = 0;
		return;
	}

	float tmp = g.rcin_yaw.output;		//-1~1
	if (tmp > 0.85f)		//解锁
	{
		if (arming_count < ARM_DELAY)
			arming_count++;

		if (arming_count == ARM_DELAY && !motors.armed())
		{
			pre_arm_check(false);							//检测传感器状态
			if (state.pre_arm_check && arm_check(false))	//解锁检测
			{
				if (!init_arm_motor())				//解锁
				{
					arming_count = 0;
				}
			}
			else
			{
				arming_count = 0;
			}

		}
	}
	else if (tmp < -0.85f)		//上锁
	{
		if (arming_count < DISARM_DELAY)
			arming_count++;

		if (arming_count == DISARM_DELAY && motors.armed())
		{
			init_disarm_motor();

			arming_count = 0;
		}
	}
	else
	{
		arming_count = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
//100Hz(10ms),读取遥控信号
void read_radio(void)
{
	static uint32_t last_update_ms = 0;
	uint32_t tnow_ms = millis();

	if (sbus.state == SBUS_AVAILABLE)		//有新的信号
	{
		sbus.state = SBUS_USING;
		g.rcin_roll.set_radio(sbus.channel.ch1);
		g.rcin_pitch.set_radio(sbus.channel.ch2);
		g.rcin_throttle.set_radio(sbus.channel.ch3);
		g.rcin_yaw.set_radio(sbus.channel.ch4);

	
		g.rcin_ch5.set_radio(sbus.channel.ch5);
		g.rcin_ch6.set_radio(sbus.channel.ch6);
		g.rcin_ch7.set_radio(sbus.channel.ch7);

		g.rcin_ch8.set_radio(sbus.channel.ch8);
		g.rcin_ch9.set_radio(sbus.channel.ch9);

		sbus.state = SBUS_EMPTY;

		state.new_radio_frame = true;

		last_update_ms = tnow_ms;
		set_throttle_failsafe(g.rcin_throttle.radio_in);		//失控油门检测,sbus硬件支持
		set_throttle_zero(g.rcin_throttle.output);				//0油门检测

		return;
	}

// 	if (ibus.state == IBUS_AVAILABLE)		//有新的信号
// 	{
// 		sbus.state = IBUS_USING;
// 		g.rcin_roll.set_radio(ibus.channel.ch1);
// 		g.rcin_pitch.set_radio(ibus.channel.ch2);
// 		g.rcin_throttle.set_radio(ibus.channel.ch3);
// 		g.rcin_yaw.set_radio(ibus.channel.ch4);
// 
// 		g.rcin_ch7.set_radio(ibus.channel.ch9);
// 		g.rcin_ch9.set_radio(ibus.channel.ch10);
// 
// 		ibus.state = IBUS_EMPTY;
// 
// 		state.new_radio_frame = true;
// 		last_update_ms = tnow_ms;
// 		set_throttle_failsafe(g.rcin_throttle.radio_in);		//失控油门检测,sbus硬件支持
// 		set_throttle_zero(g.rcin_throttle.control_in);			//0油门检测
// 		return;
// 	}

	uint32_t elapsed = tnow_ms - last_update_ms;
	if (elapsed >= FS_RADIO_TIMEOUT_MS)
	{
		failsafe_set_radio(true);			//失控保护
	}
	
}
uint8_t mode_map(uint8_t sp)
{
	if (sp < 3)
	{
		return STABILIZE;
	}
	else
	{	
		return ALT_HOLD;
	
	}
}
#define CONTROL_SWITCH_COUNTER  20  
void read_control_switch(void)//切换飞行模式
{
	static int8_t old_switch = -1;
	int8_t sp = g.rcin_ch5.output;
	static uint8_t switch_counter = 0;


	if(old_switch != sp && !failsafe.radio && failsafe.radio_counter == 0)	//切换模式
	{
		switch_counter++;
		if(switch_counter >= CONTROL_SWITCH_COUNTER){
			old_switch = sp;
			switch_counter = 0;

			//TODO:不一定成功??
			set_mode(mode_map(sp),motors.armed());	

		}

	}

}
void rc_loop(void)
{
	read_radio();
	read_control_switch();

}

//////////////////////////////////////////////////////////////////////////
//10Hz,100ms:指示更新
void notify_loop() {
	notify.update();
}

//////////////////////////////////////////////////////////////////////////
//100Hz,10ms:气压计读取更新
void barometer_update(void)
{
	barometer.update();
}

//////////////////////////////////////////////////////////////////////////
//10Hz,100ms:更新气压计,超声波高度
void update_altitude(void)
{
	barometer.read();
  	baro_alt = barometer.get_altitude()*100.0f;
  	baro_climbrate = barometer.get_climb_rate()*100.0f;
}

//////////////////////////////////////////////////////////////////////////
//400Hz(2.5ms):
void stabilize_run(float dt)
{
	int16_t target_roll, target_pitch;
	float target_yaw_rate;
	float pilot_throttle_scaled;
	int16_t throttle_control;

	//未解锁或未启动
	if (!motors.armed() || g.rcin_throttle.output <= 0.0f)//未解锁,0油门
	{
		attitude_control.relax_bf_rate_controller();			//期望角速率设置为当前速率
		attitude_control.set_yaw_target_to_current_heading();	//期望航向设置为当前航向
		attitude_control.set_throttle_out(0, false);		//设置油门输出为0
		return;
	}

	//无头模式下需要转换输出

	
	target_roll = g.rcin_roll.output*g.angle_max;
	target_pitch = g.rcin_pitch.output*g.angle_max;
	target_yaw_rate = g.rcin_yaw.output * g.angle_max * g.acro_yaw_p;
	throttle_control = g.rcin_throttle.output * 1000;

	pilot_throttle_scaled = get_pilot_desired_throttle(throttle_control);
	

	//外环
	attitude_control.angle_ef_roll_pitch_rate_ef_yaw_smooth(
		target_roll, target_pitch, target_yaw_rate, dt, get_smoothing_gain());

	attitude_control.set_throttle_out(pilot_throttle_scaled, true);		//设置油门

}
 void althold_run(float dt)
{
	int16_t target_roll, target_pitch;
	float target_yaw_rate;
	int16_t throttle_control;

	//未解锁或未启动
	if (!motors.armed() || g.rcin_throttle.output <= 0.0f)//未解锁,0油门
	{
		attitude_control.relax_bf_rate_controller();			//期望角速率设置为当前速率
		attitude_control.set_yaw_target_to_current_heading();	//期望航向设置为当前航向
		attitude_control.set_throttle_out(0, false);		//设置油门输出为0
		pos_control.set_alt_target_to_current_alt();	//期望高度设置为当前高度
		return;
	}

	//无头模式下需要转换输出

	target_roll = g.rcin_roll.output*g.angle_max;
	target_pitch = g.rcin_pitch.output*g.angle_max;
	target_yaw_rate = g.rcin_yaw.output * g.angle_max * g.acro_yaw_p;
	throttle_control = g.rcin_throttle.output * 1000;

	target_climb_rate = get_pilot_desired_climb_rate(throttle_control);	//获取期望垂直速率


	if (state.land_complete && target_climb_rate > 0)		//准备起飞 
	{
		set_land_complete(false);
		set_throttle_takeoff();								//slow_start,poscontrol.init_take_off
	}

	if(state.land_complete)	//在地上,摇杆未超过中位
	{
		attitude_control.relax_bf_rate_controller();			//期望角速率设置为当前速率
		attitude_control.set_yaw_target_to_current_heading();	//期望航向设置为当前航向
		pos_control.set_alt_target_to_current_alt();			//期望高度设置为当前高度
		attitude_control.set_throttle_out(get_throttle_pre_takeoff(throttle_control), false);//不到中位油门时输出根据油门变换,但不起飞

	}
	else
	{
		
		attitude_control.angle_ef_roll_pitch_rate_ef_yaw_smooth(target_roll, target_pitch, target_yaw_rate,		//外环
															dt, get_smoothing_gain());
		
		pos_control.set_alt_target_from_climb_rate(target_climb_rate, dt);										//更新期望高度
		pos_control.update_z_controller(dt);
	}
//	attitude_control.set_throttle_out(pilot_throttle_scaled, true);		//设置油门
}
void acro_run(float dt)
{
	stabilize_run(dt);
}
void update_flight_mode(float dt)
{
	switch (control_mode)
	{
	case STABILIZE:
		stabilize_run(dt);
		break;
	case ACRO:
		acro_run(dt);
		break;
	case  ALT_HOLD:
		althold_run(dt);
		break;

	default:
		break;
	}

}
void fast_loop(void)
{
	float deltat = 2.5f / 1000.0f;
	ahrs.update(deltat);		//更新欧拉角
	attitude_control.rate_control_run(deltat);
	motors.output();			//output
	inertial_nav.update(deltat);
	update_flight_mode(deltat);

}
//////////////////////////////////////////////////////////////////////////
//1Hz(1ms),一次只发送一个包
void ANO_DT_Data_Exchange(void)
{
	static uint8_t cnt = 0;
	static uint8_t senser_cnt = 10;	//
 	static uint8_t senser2_cnt = 50;	//
// 	static uint8_t user_cnt = 10;	//
	static uint8_t status_cnt = 15;	//
 	static uint8_t rcdata_cnt = 20;	//
 	static uint8_t motopwm_cnt = 20;	//
// 	static uint8_t power_cnt = 50;		//
//	static uint8_t speed_cnt = 50;			//
// 	static uint8_t location_cnt = 200;		//


	if ((cnt % senser_cnt) == (senser_cnt - 1))	
		f.send_senser = 1;
 	if((cnt % senser2_cnt) == (senser2_cnt-1))	//
 		f.send_senser2 = 1;	
	// 	if((cnt % user_cnt) == (user_cnt-2))		//
	// 		f.send_user = 1;
	if ((cnt % status_cnt) == (status_cnt - 1))
		f.send_status = 1;

	if((cnt % rcdata_cnt) == (rcdata_cnt-1))
		f.send_rcdata = 1;	

	if ((cnt % motopwm_cnt) == (motopwm_cnt - 2))
		f.send_motopwm = 1;

	// 	if((cnt % power_cnt) == (power_cnt-2))
	// 		f.send_power = 1;		
	// 	if((cnt % speed_cnt) == (speed_cnt-3))
	// 		f.send_speed = 1;		
	// 	if((cnt % location_cnt) == (location_cnt-3))
	// 	{
	// 		f.send_location += 1;		
	// 	}

	if (++cnt > 200) cnt = 0;

	/////////////////////////////////////////////////////////////////////////////////////
	if (f.msg_id)
	{
		ANO_DT_Send_Msg(f.msg_id, f.msg_data);
		f.msg_id = 0;
	}
	else if (f.send_check)
	{
		f.send_check = 0;
		ANO_DT_Send_Check(f.checkdata_to_send, f.checksum_to_send);
	}
	else if (f.send_version)
	{
		f.send_version = 0;
		ANO_DT_Send_Version(4, 300, 100, 400, 0);
	}
	else if (f.send_status)
	{
		f.send_status = 0;
		uint8_t ano_mode;

		if (control_mode == STABILIZE)ano_mode = 1;
		else if (control_mode == ALT_HOLD)ano_mode = 2;
		else ano_mode = 0;

		ANO_DT_Send_Status(degrees(ahrs.roll), degrees(ahrs.pitch), degrees(ahrs.yaw),inertial_nav.get_altitude(), ano_mode, motors.armed());
	}
	else if (f.send_speed)
	{
		f.send_speed = 0;
		//ANO_DT_Send_Speed(0,0,);//y,x,z
	}
	else if (f.send_user)
	{
		f.send_user = 0;
		//ANO_DT_Send_User();
	}
	else if (f.send_senser)
	{
		f.send_senser = 0;
		ANO_DT_Send_Senser(ins.get_accel().x*100, ins.get_accel().y * 100, ins.get_accel().z * 100, ins.get_gyro().x*100, ins.get_gyro().y * 100, ins.get_gyro().z * 100, 0, 0, 0);
	}
	else if (f.send_senser2)
	{
		f.send_senser2 = 0;
		ANO_DT_Send_Senser2(baro_alt*100,baro_climbrate*100);//原始数据
	}
	else if (f.send_rcdata)
	{
		f.send_rcdata = 0;

// 		ANO_DT_Send_RCData(ibus.channel.ch3, ibus.channel.ch4, ibus.channel.ch1, ibus.channel.ch2,
// 			ibus.channel.ch5, ibus.channel.ch6, ibus.channel.ch7, ibus.channel.ch8,ibus.channel.ch9, ibus.channel.ch10);
		
// 		ANO_DT_Send_RCData(g.rcin_throttle.output*1000+1000, g.rcin_yaw.output * 500 + 1500,g.rcin_roll.output * 500 + 1500, g.rcin_pitch.output * 500 + 1500,
// 			g.rcin_ch5.output * 1000+1000, g.rcin_ch6.output * 1000+1000, g.rcin_ch7.output * 1000 +1000, 
// 			g.rcin_ch8.output * 500 + 1500, g.rcin_ch9.output * 500 + 1500,0);

		ANO_DT_Send_RCData(g.rcin_throttle.output * 1000 + 1000, g.rcin_yaw.output * 500 + 1500, g.rcin_roll.output * 500 + 1500, g.rcin_pitch.output * 500 + 1500,
			g.rcin_ch5.output + 1000, g.rcin_ch6.output + 1000, g.rcin_ch7.output + 1000,
			g.rcin_ch8.output * 500 + 1500, g.rcin_ch9.output * 500 + 1500, 0);


		//ANO_DT_Send_RCData(motors.motor_out[0], motors.motor_out[1], motors.motor_out[2], motors.motor_out[3], 0, 0, 0, 0, 0, 0);

	}
	else if (f.send_motopwm)
	{
		f.send_motopwm = 0;
		ANO_DT_Send_MotoPWM(motors.motor_out[0], motors.motor_out[1], motors.motor_out[2], motors.motor_out[3], 0, 0, 0, 0);
	}
	else if (f.send_power)
	{
		f.send_power = 0;
		//ANO_DT_Send_Power(Plane_Votage*100,0);
	}
	/////////////////////////////////////////////////////////////////////////////////////
	else if (f.send_pid1)
	{
		f.send_pid1 = 0;
		
		ANO_DT_Send_PID(1, g.pid_rate_roll.kP(), g.pid_rate_roll.kI(), g.pid_rate_roll.kD(),
			g.pid_rate_pitch.kP(), g.pid_rate_pitch.kI(), g.pid_rate_pitch.kD(),
			g.pid_rate_yaw.kP(), g.pid_rate_yaw.kI(), g.pid_rate_yaw.kD());

	}
	else if (f.send_pid2)
	{
		f.send_pid2 = 0;
		ANO_DT_Send_PID(2, g.p_angle_roll.kP(), 0, 0,
			g.p_angle_pitch.kP(), 0, 0,
			g.p_angle_yaw.kP(), 0, 0);
	}
	else if (f.send_pid3)
	{
		f.send_pid3 = 0;
		//ANO_DT_Send_PID(3, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
	else if (f.send_pid4)
	{
		f.send_pid4 = 0;
		ANO_DT_Send_PID(4, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
	else if (f.send_pid5)
	{
		f.send_pid5 = 0;
		ANO_DT_Send_PID(5, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
	else if (f.send_pid6)
	{
		f.send_pid6 = 0;
		ANO_DT_Send_PID(6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
	/////////////////////////////////////////////////////////////////////////////////////
	else if (f.send_location == 2)
	{
		f.send_location = 0;
		//ANO_DT_Send_Location(0, 0, 0 * 10000000, 0 * 10000000, 0);
	}
	else if (f.send_parame)
	{
		//ANO_DT_SendParame(f.send_parame);
		f.send_parame = 0;
	}
	else if (f.send_vef)
	{
		ANO_DT_Send_VER();
		f.send_vef = 0;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	ANO_DT_Data_Receive_Anl_Task();
}
void MiniBalance_Data_Exchange()
{
	static uint8_t cnt = 0;
	if (cnt % 99 == 0)
	{
		MiniBalance_Flag.send_wave = 1;
	}


	if (++cnt == 200)cnt = 0;

//////////////////////////////////////////////////////////////////////////
	//自定义按键
	if (MiniBalance_Flag.u1)
	{
		//		puts("u1");
		MiniBalance_Flag.u1 = 0;
	}
	if (MiniBalance_Flag.u2)
	{
		//		puts("u2");
		MiniBalance_Flag.u2 = 0;
	}
	if (MiniBalance_Flag.u3)
	{
		//		puts("u3");
		MiniBalance_Flag.u3 = 0;
	}
	if (MiniBalance_Flag.u4)
	{
		//		puts("u4");
		MiniBalance_Flag.u4 = 0;
	}
	if (MiniBalance_Flag.u5)
	{
		//		puts("u5");
		MiniBalance_Flag.u5 = 0;
	}
	if (MiniBalance_Flag.u6)
	{
		//		puts("u6");
		MiniBalance_Flag.u6 = 0;
	}
	if (MiniBalance_Flag.u7)
	{
		//		puts("u7");
		MiniBalance_Flag.u7 = 0;
	}
	if (MiniBalance_Flag.u8)
	{
		//		puts("u8");
		MiniBalance_Flag.u8 = 0;
	}
	if (MiniBalance_Flag.u9)
	{
		//		puts("u9");
		MiniBalance_Flag.u9 = 0;
	}
	//speed
	if (MiniBalance_Flag.speed_change)
	{
		MiniBalance_Flag.speed_change = 0;
		if (MiniBalance_Flag.speed == MB_SPEED_HIGH)
		{
			//			puts("high speed");
		}
		else
		{
			//			puts("low speed");
		}
	}
	//direction
	if (MiniBalance_Flag.direction_change)
	{
		MiniBalance_Flag.direction_change = 0;
		//		printf("direction=%c\r\n", MiniBalance_Flag.direction);
	}
	//save parameter
	if (MiniBalance_Flag.save_parameter)
	{
		MiniBalance_Flag.save_parameter = 0;
	}
	//get parameter
	if(MiniBalance_Flag.get_param)
	{
		MiniBalance_Flag.get_param = 0;

	}

//////////////////////////////////////////////////////////////////////////
	if (MiniBalance_Flag.send_parameter)
	{
		MiniBalance_Flag.send_parameter = 0;
		MiniBalance_SendParameter();
	}
	else if(MiniBalance_Flag.send_wave)
	{
		MiniBalance_Flag.send_wave = 0;
		MiniBalance_Wave(target_climb_rate,inertial_nav.get_velocity_z(),baro_alt,inertial_nav.get_position().z, ANO_OF.ALT2);
	}

//////////////////////////////////////////////////////////////////////////

	MiniBalance_Recv_Task();
}
void display_loop(void)
{
//	ANO_DT_Data_Exchange();
	MiniBalance_Data_Exchange();
//	printf("in=%d,percent=%.2f\r\n", g.rcin_ch9.radio_in, g.rcin_ch9.get_percent() * 100);

}
//////////////////////////////////////////////////////////////////////////
//1Hz(1000ms):保存参数,预解锁检测
void parameter_loop()
{
	//保存数据较消耗时间,解锁状态下不保存
	if (ParameterBase::SaveRequeset() && !motors.armed())
	{
		ParameterBase::SaveAll();
	}
}
void pre_arm_check_loop()
{
	static uint8_t pre_arm_display_counter = 0;
	pre_arm_display_counter++;
	if (pre_arm_display_counter >= 5)//5s显示一次错误
	{
		pre_arm_check(1);				//
		pre_arm_display_counter = 0;
	}
	else
	{
		pre_arm_check(0);
	}
}
void one_hz_loop(void)
{
	parameter_loop();
	pre_arm_check_loop();
}
//////////////////////////////////////////////////////////////////////////
//Setup
//////////////////////////////////////////////////////////////////////////
void init_rcin(void)
{
	g.rcin_roll(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_roll.set_dead_zone(30);
	g.rcin_roll.set_reverse(1);
	g.rcin_roll.set_type(RCIn::TYPE_ANGLE);


	g.rcin_pitch(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_pitch.set_dead_zone(30);
	g.rcin_pitch.set_reverse(1);
	g.rcin_pitch.set_type(RCIn::TYPE_ANGLE);


	g.rcin_yaw(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_yaw.set_dead_zone(30);
	g.rcin_yaw.set_reverse(1);
	g.rcin_yaw.set_type(RCIn::TYPE_ANGLE);


	g.rcin_throttle(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_throttle.set_dead_zone(50);
	g.rcin_throttle.set_reverse(1);
	g.rcin_throttle.set_type(RCIn::TYPE_RANGE);

//////////////////////////////////////////////////////////////////////////
	g.rcin_ch5(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_ch5.set_dead_zone(0);
	g.rcin_ch5.set_reverse(1);
	g.rcin_ch5.set_type(RCIn::TYPE_SWITCH);

	g.rcin_ch6(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_ch6.set_dead_zone(0);
	g.rcin_ch6.set_reverse(1);
	g.rcin_ch6.set_type(RCIn::TYPE_SWITCH);

	g.rcin_ch7(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_ch7.set_dead_zone(0);
	g.rcin_ch7.set_reverse(1);
	g.rcin_ch7.set_type(RCIn::TYPE_SWITCH);

	//////////////////////////////////////////////////////////////////////////

	g.rcin_ch8(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_ch8.set_dead_zone(50);
	g.rcin_ch8.set_reverse(1);
	g.rcin_ch8.set_type(RCIn::TYPE_ANGLE);

	g.rcin_ch9(SBUS_MIN, SBUS_MID, SBUS_MAX);
	g.rcin_ch9.set_dead_zone(50);
	g.rcin_ch9.set_reverse(-1);
	g.rcin_ch9.set_type(RCIn::TYPE_ANGLE);




// 	g.rcin_roll(1000, 1500, 2000);
// 	g.rcin_roll.set_dead_zone(100);
// 	g.rcin_roll.set_reverse(1);
// 	g.rcin_roll.set_angle(RC_RPY_INPUT_MAX);
// 
// 
// 	g.rcin_pitch(1000, 1500, 2000);
// 	g.rcin_pitch.set_dead_zone(100);
// 	g.rcin_pitch.set_reverse(1);
// 	g.rcin_pitch.set_angle(RC_RPY_INPUT_MAX);
// 
// 	g.rcin_yaw(1000, 1500, 2000);
// 	g.rcin_yaw.set_dead_zone(100);
// 	g.rcin_yaw.set_reverse(1);
// 	g.rcin_yaw.set_angle(RC_RPY_INPUT_MAX);
// 
// 	g.rcin_throttle(1000, 1500, 2000);
// 	g.rcin_throttle.set_dead_zone(50);
// 	g.rcin_throttle.set_reverse(1);
// 	g.rcin_throttle.set_range(g.throttle_min, g.throttle_max);
// 
// 	g.rcin_ch7(1000, 1500, 2000);
// 	g.rcin_ch7.set_dead_zone(50);
// 	g.rcin_ch7.set_reverse(1);
// 	g.rcin_ch7.set_range(0, 1000);
// 
// 	g.rcin_ch9(1000, 1500, 2000);
// 	g.rcin_ch9.set_dead_zone(50);
// 	g.rcin_ch9.set_reverse(-1);
// 	g.rcin_ch9.set_range(0, 1000);

}
void init_rcout(void)
{
	g.rcout_roll(1000, 1500, 2000);
	g.rcout_pitch(1000, 1500, 2000);
	g.rcout_throttle(1000, 1500, 2000);
	g.rcout_yaw(1000, 1500, 2000);

	g.rcout_roll.set_type(RCOut::TYPE_ANGLE_RAW);
	g.rcout_pitch.set_type(RCOut::TYPE_ANGLE_RAW);
	g.rcout_yaw.set_type(RCOut::TYPE_ANGLE_RAW);
	g.rcout_throttle.set_range(g.throttle_min, g.throttle_max);

}
void init_motors() 
{
	motors.set_throttle_max(g.throttle_max);
	motors.set_throttle_min(g.throttle_min);
	motors.set_mid_throttle(g.throttle_mid);
	//x轴，机头方向为0度
	//===================4轴================================
	motors.remove_all_motors();
	motors.add_motor(MOTORS_MOT_1, 45, MOTORS_YAW_FACTOR_CCW);	//右上
	motors.add_motor(MOTORS_MOT_2, -135, MOTORS_YAW_FACTOR_CCW);//左下
	motors.add_motor(MOTORS_MOT_3, -45, MOTORS_YAW_FACTOR_CW);	//左上
	motors.add_motor(MOTORS_MOT_4, 135, MOTORS_YAW_FACTOR_CW);	//右下
}
void init_barometer(bool full_calibration)
{
	if(full_calibration)
	{
		barometer.calibrate();
	}
	else
	{
		barometer.update_calibration();
	}
}
void startup_ground(void)
{
	ahrs.init();
	ahrs.reset_gyro_drift();			//清除陀螺仪积分漂移
	ahrs.set_fast_gains(true);			//快速矫正,P*8,解锁后取消
}
void esc_calibration(void)	//校准电调,死循环,需要重新上电
{
	uint16_t t;
	uint16_t n;
	int16_t pwm_val;

	for (t = 0; t < 3; t++)
	{
		read_radio();
		delay_ms(100);
	}

	while (1)
	{
		read_radio();			//读取遥控参数
		pwm_val = g.rcin_throttle.output*(g.rcout_throttle.radio_max - g.rcout_throttle.radio_min) + g.rcout_throttle.radio_min;

		for (n=0;n<MOTORS_MAX_MOTOR_NUM;n++)
		{
			if (motors.motor_enabled[n])
			{
				hal.pwm_write(n,pwm_val);
			}
				
		}

		delay_ms(10);
		if(++t == 10)	//10Hz
		{
			t = 0;
			notify_loop();
		}
	}
}
void setup(void)
{
	sbus_init();

	notify.init();
	init_rcin();
	init_rcout();
	init_motors();
	//电调校准
	if (g.esc_cali)
	{
		g.esc_cali = false;	//清除电调校准标志
		g.esc_cali.save();	
		parameter_loop();	//保存参数
		esc_calibration();	//电调校准
	}

	ins.init();

	attitude_control.set_dt(2.5f / 1000);
	pos_control.set_dt(2.5f / 1000);
	pos_control.set_alt_max(180);		//最大高度2m

	barometer.init();
	init_barometer(true); //地面气压温度校准
	inertial_nav.init();

	startup_ground();
	pre_arm_check(true);

	state.initialised = true;
}

//////////////////////////////////////////////////////////////////////////
//main
//////////////////////////////////////////////////////////////////////////
const scheduler_tasks_t scheduler_tasks[] = {
 	{ fast_loop,		2500,	1000 },	//400Hz,2.5ms,姿态解算,控制
	{ rc_loop,			10000,	1000 },	//100Hz,10ms,接收机输入检测
 	{ arm_check_loop,	100000,	1000 },	//10Hz,100ms,遥控解锁手势检测
	{ barometer_update,	10000,	1000 }, //100Hz,10ms,气压计读取更新
	{ update_altitude,	100000,	1000 },	//10Hz,100ms,读取气压计,超声波,更新当前高度
// 	{ throttle_loop,	20000,	1000 },	//50Hz,20ms,
 	{ notify_loop,		100000,	100 },//50Hz,20ms
	{ display_loop,		1000,	100},	//1kHz
	{ one_hz_loop,	1000000,	500},	//1s处理一次参数保存请求
};

int main(void)
{
	hal.Setup();
	setup();

	scheduler.init(scheduler_tasks, sizeof(scheduler_tasks) / sizeof(scheduler_tasks[0]));
	scheduler.run();

}







































