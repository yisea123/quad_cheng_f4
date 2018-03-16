#ifndef _ATTITUDE_H_
#define _ATTITUDE_H_
#include "sys.h"



void get_pilot_desired_lean_angles(int16_t roll_in, int16_t pitch_in, int16_t &roll_out, int16_t &pitch_out);
float get_pilot_desired_yaw_rate(int16_t stick_angle);
int16_t get_pilot_desired_throttle(int16_t throttle_control);
float get_smoothing_gain(void);


#endif



