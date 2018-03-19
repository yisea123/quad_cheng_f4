#ifndef _ATTITUDE_H_
#define _ATTITUDE_H_
#include "sys.h"



int16_t get_pilot_desired_throttle(int16_t throttle_control);

int16_t get_pilot_desired_climb_rate(int16_t throttle_control);
void set_throttle_takeoff(void);
int16_t get_throttle_pre_takeoff(int16_t throttle_control);

float get_smoothing_gain(void);


#endif



