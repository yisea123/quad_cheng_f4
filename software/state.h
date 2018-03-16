#ifndef _STATE_H_
#define _STATE_H_
#include "sys.h"


typedef struct 
{
	uint8_t calibration_accel : 1;			//校准加速度计
	uint8_t calibration_gyro : 1;			//校准陀螺仪
	uint8_t calibration_radio : 1;			//校准遥控输入
	//uint8_t calibration_compass : 1;		//校准磁力计，compass_mot



	uint8_t home_is_set : 1; // 0	//supersimple ????,????
	uint8_t simple_mode : 2; // 1,2 // This is the state of simple mode : 0 = disabled ; 1 = SIMPLE ; 2 = SUPERSIMPLE
	uint8_t pre_arm_rc_check : 1; // 3   // true if rc input pre-arm checks have been completed successfully
	uint8_t pre_arm_check : 1; // 4   //????? true if all pre-arm checks (rc, accel calibration, gps lock) have been performed
	uint8_t auto_armed : 1; // 5   // stops auto missions from beginning until throttle is raised
	uint8_t logging_started : 1; // 6   // true if dataflash logging has started
	uint8_t land_complete : 1; // 7   //throttle_loop true if we have detected a landing
	uint8_t new_radio_frame : 1; // 8  // Set true if we have new PWM data to act on from the Radio,???????
	uint8_t CH7_flag : 2; // 9,10   // ch7 aux switch : 0 is low or false, 1 is center or true, 2 is high
	uint8_t CH8_flag : 2; // 11,12   // ch8 aux switch : 0 is low or false, 1 is center or true, 2 is high
	uint8_t usb_connected : 1; // 13      // true if APM is powered from USB connection
	uint8_t rc_receiver_present : 1; // 14  // true if we have an rc receiver present (i.e. if we've ever received an update)
	uint8_t compass_mot : 1; // 15  // true if we are currently performing compassmot calibration
	uint8_t motor_test : 1; // 16  // true if we are currently performing the motors test
	uint8_t initialised : 1; // 17  //??????? true once the init_ardupilot function has completed.  Extended status to GCS is not sent until this completes
	uint8_t land_complete_maybe : 1; // 18  // true if we may have landed (less strict version of land_complete)
	uint8_t throttle_zero : 1; // 19  //0???????? true if the throttle stick is at zero, debounced



}state_t;

extern state_t state;
void set_pre_arm_check(bool set);
void set_pre_arm_rc_check(bool b);
void set_throttle_zero(int16_t throttle_control);

void set_land_complete(bool b);
void set_land_complete_maybe(bool b);

#endif


