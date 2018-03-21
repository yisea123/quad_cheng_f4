#include "InertialSensor_ICM20602.h"

#include "state.h"
#include "Ano_DT.h"
#include "Notify.h"

//========ICM20602寄存器地址========================
/********************************************
*复位后所有寄存器地址都为0，除了
*Register 26  CONFIG				= 0x80
*Register 107 Power Management 1 	= 0x41
*Register 117 WHO_AM_I 				= 0x12
*********************************************/
//陀螺仪温度补偿
#define	ICM20_XG_OFFS_TC_H				0x04
#define	ICM20_XG_OFFS_TC_L				0x05
#define	ICM20_YG_OFFS_TC_H				0x07
#define	ICM20_YG_OFFS_TC_L				0x08
#define	ICM20_ZG_OFFS_TC_H				0x0A
#define	ICM20_ZG_OFFS_TC_L				0x0B
//加速度自检输出(出产时设置，用于与用户的自检输出值比较）
#define	ICM20_SELF_TEST_X_ACCEL			0x0D
#define	ICM20_SELF_TEST_Y_ACCEL			0x0E
#define	ICM20_SELF_TEST_Z_ACCEL			0x0F
//陀螺仪静态偏移
#define	ICM20_XG_OFFS_USRH				0x13
#define	ICM20_XG_OFFS_USRL				0x14
#define	ICM20_YG_OFFS_USRH				0x15
#define	ICM20_YG_OFFS_USRL				0x16
#define	ICM20_ZG_OFFS_USRH				0x17
#define	ICM20_ZG_OFFS_USRL				0x18

#define	ICM20_SMPLRT_DIV				0x19
#define	ICM20_CONFIG					0x1A
#define	ICM20_GYRO_CONFIG				0x1B
#define	ICM20_ACCEL_CONFIG				0x1C
#define	ICM20_ACCEL_CONFIG2				0x1D
#define	ICM20_LP_MODE_CFG				0x1E

//运动唤醒加速度阈值
#define	ICM20_ACCEL_WOM_X_THR			0x20
#define	ICM20_ACCEL_WOM_Y_THR			0x21
#define	ICM20_ACCEL_WOM_Z_THR			0x22


#define	ICM20_FIFO_EN					0x23
#define	ICM20_FSYNC_INT					0x36
#define	ICM20_INT_PIN_CFG				0x37
//#define	ICM20_INT_ENABLE				0x38
#define	ICM20_FIFO_WM_INT_STATUS		0x39
#define	ICM20_INT_STATUS				0x3A

//加速度输出
#define	ICM20_ACCEL_XOUT_H				0x3B
#define	ICM20_ACCEL_XOUT_L				0x3C
#define	ICM20_ACCEL_YOUT_H				0x3D
#define	ICM20_ACCEL_YOUT_L				0x3E
#define	ICM20_ACCEL_ZOUT_H				0x3F
#define	ICM20_ACCEL_ZOUT_L				0x40
//温度输出
#define	ICM20_TEMP_OUT_H				0x41
#define	ICM20_TEMP_OUT_L				0x42
//角速度输出
#define	ICM20_GYRO_XOUT_H				0x43
#define	ICM20_GYRO_XOUT_L				0x44
#define	ICM20_GYRO_YOUT_H				0x45
#define	ICM20_GYRO_YOUT_L				0x46
#define	ICM20_GYRO_ZOUT_H				0x47
#define	ICM20_GYRO_ZOUT_L				0x48
//陀螺仪自检输出
#define	ICM20_SELF_TEST_X_GYRO			0x50
#define	ICM20_SELF_TEST_Y_GYRO			0x51
#define	ICM20_SELF_TEST_Z_GYRO			0x52

#define	ICM20_FIFO_WM_TH1				0x60
#define	ICM20_FIFO_WM_TH2				0x61
#define	ICM20_SIGNAL_PATH_RESET			0x68
#define	ICM20_ACCEL_INTEL_CTRL 			0x69
#define	ICM20_USER_CTRL					0x6A
//电源控制
#define	ICM20_PWR_MGMT_1				0x6B
#define	ICM20_PWR_MGMT_2				0x6C

#define	ICM20_I2C_IF					0x70
#define	ICM20_FIFO_COUNTH				0x72
#define	ICM20_FIFO_COUNTL				0x73
#define	ICM20_FIFO_R_W					0x74

#define	ICM20_WHO_AM_I 					0x75
//加速度静态偏移
#define	ICM20_XA_OFFSET_H				0x77
#define	ICM20_XA_OFFSET_L				0x78
#define	ICM20_YA_OFFSET_H				0x7A
#define	ICM20_YA_OFFSET_L				0x7B
#define	ICM20_ZA_OFFSET_H				0x7D
#define	ICM20_ZA_OFFSET_L 				0x7E
//===========================================================




void InertialSensor_ICM20602::set_accel_fullscale(uint8_t fs)
{
	switch (fs)
	{
	case ICM20_ACCEL_FS_2G:
		_accel_sensor_scale = 1.0f / 16348.0f;
		break;
	case ICM20_ACCEL_FS_4G:
		_accel_sensor_scale = 1.0f / 8192.0f;
		break;
	case ICM20_ACCEL_FS_8G:
		_accel_sensor_scale = 1.0f / 4096.0f;
		break;
	case ICM20_ACCEL_FS_16G:
		_accel_sensor_scale = 1.0f / 2048.0f;
		break;
	default:
		fs = ICM20_ACCEL_FS_8G;
		_accel_sensor_scale = 1.0f / 4096.0f;
		break;

	}
	_write_reg(ICM20_ACCEL_CONFIG, fs);
	_accel_sensor_scale *= GRAVITY_MSS;//in m/s2单位
}
void InertialSensor_ICM20602::set_gyro_fullscale(uint8_t fs)
{
	switch (fs)
	{
	case ICM20_GYRO_FS_250:
		_gyro_sensor_scale = 1.0f / 131.068f;	//32767/250
		break;
	case ICM20_GYRO_FS_500:
		_gyro_sensor_scale = 1.0f / 65.534f;
		break;
	case ICM20_GYRO_FS_1000:
		_gyro_sensor_scale = 1.0f / 32.767f;
		break;
	case ICM20_GYRO_FS_2000:
		_gyro_sensor_scale = 1.0f / 16.3835f;
		break;
	default:
		fs = ICM20_GYRO_FS_2000;
		_gyro_sensor_scale = 1.0f / 16.3835f;
		break;
	}
	_write_reg(ICM20_GYRO_CONFIG, fs);
	_gyro_sensor_scale = radians(_gyro_sensor_scale);		//in rad

}
bool InertialSensor_ICM20602::_init_sensor(void)
{

	hal.scheduler.suspend_timer_procs();
	_cs_pin.mode_out();

	_healthy = false;
	
	_write_reg(ICM20_PWR_MGMT_1, 0x80);		//复位
	delay_ms(50);
	
	_write_reg(ICM20_PWR_MGMT_1, 0x01);		//时钟设置为自动
	delay_ms(50);

	_product_id = _read_reg(ICM20_WHO_AM_I);
	//printf("icm_20602 id=%x\r\n", _product_id);

	if (_product_id != 0x12)
	{
		hal.scheduler.resume_timer_procs();
		return false;
	}


	_write_reg(ICM20_SMPLRT_DIV, 0);									//分频数=为0+1，数据输出速率为内部采样速率
	_write_reg(ICM20_CONFIG, DLPF_BW_20);								//GYRO低通滤波设置
	_write_reg(ICM20_ACCEL_CONFIG2, ACCEL_AVER_4 | ACCEL_DLPF_BW_21);	//ACCEL低通滤波设置


	set_accel_fullscale(ICM20_ACCEL_FS_8G);
	set_gyro_fullscale(ICM20_GYRO_FS_2000);

	// printf("icm20602 init pass\r\n\r\n");

	hal.scheduler.resume_timer_procs();

	_healthy = true;
	return true;
}
void InertialSensor_ICM20602::init(void)
{

	_init_sensor();
	check_calibration();

	//Notify::flags.initialising = true;	//ins初始化，保持静止
// 	printf("gyro_offset:x=%f,y=%f,z=%f\r\n", _gyro_offset.x, _gyro_offset.y, _gyro_offset.z);
// 	printf("accel_ofset:x=%f,y=%f,z=%f\r\n", _accel_offset.x, _accel_offset.y, _accel_offset.z);

}


void InertialSensor_ICM20602::_poll_data(void)
{
	uint8_t buf[14];
	//读取传感器数据
	hal.scheduler.suspend_timer_procs();
	_read_reg(ICM20_ACCEL_XOUT_H, buf, 14);
	hal.scheduler.resume_timer_procs();

#define int16_val(v, idx) ((int16_t)(((uint16_t)v[2*idx] << 8) | v[2*idx+1]))

	_accel_raw.x = int16_val(buf, 1);
	_accel_raw.y = int16_val(buf, 0);
	//TODO向下运动为正???,测得重力大小为负????
	_accel_raw.z = -int16_val(buf, 2);		

	_gyro_raw.x = int16_val(buf, 5);
	_gyro_raw.y = int16_val(buf, 4);
	_gyro_raw.z = -int16_val(buf, 6);

	//单位转换
	_accel_raw *= _accel_sensor_scale;
	_gyro_raw *= _gyro_sensor_scale;

	//滤波
	_accel_filtered = Vector3f(_accel_filter_x.apply(_accel_raw.x),
								_accel_filter_y.apply(_accel_raw.y),
								_accel_filter_z.apply(_accel_raw.z));

	_gyro_filtered = Vector3f(_gyro_filter_x.apply(_gyro_raw.x),
								_gyro_filter_y.apply(_gyro_raw.y),
								_gyro_filter_z.apply(_gyro_raw.z));


	_accel_available = true;
	_accel_available = true;

}
bool InertialSensor_ICM20602::update()
{
	_poll_data();		
	_rotate_and_offset_accel(_accel_filtered);
	_rotate_and_offset_gyro(_gyro_filtered);


	if(state.calibration_accel)
	{
		if(calibration_accel())
		{
			state.calibration_accel = false;
			f.msg_id = 1;
			f.msg_data = 0x01;
		}
	}

	if(state.calibration_gyro)
	{
		if (calibration_gyro())
		{
			state.calibration_gyro = false;
			f.msg_id = 2;
			f.msg_data = 0x01;
		}
	}

	//正在校准,提示不要动
	Notify::flags.initialising = state.calibration_accel || state.calibration_gyro;

	return true;
}




uint8_t InertialSensor_ICM20602::_read_reg(uint8_t reg)
{
	uint8_t res;
	_cs_pin = 0;
	res = _spi.read_reg(reg);
	_cs_pin = 1;
	return res;
}
void InertialSensor_ICM20602::_read_reg(uint8_t reg, uint8_t * pbuf, uint8_t len)
{
	_cs_pin = 0;
	_spi.read_reg(reg, pbuf, len);
	_cs_pin = 1;

}
void InertialSensor_ICM20602::_write_reg(uint8_t reg, uint8_t val)
{
	_cs_pin = 0;
	_spi.write_reg(reg,val);
	_cs_pin = 1;
}
