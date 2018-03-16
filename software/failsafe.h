#ifndef _FAILSAFE_H_
#define _FAILSAFE_H_
#include "sys.h"


typedef struct
{
	uint8_t radio : 1; 						// 1   // A status flag for the radio failsafe,????????
	int8_t radio_counter;                	// number of iterations with throttle below throttle_fs_value

	uint8_t battery : 1; 						// 2   // A status flag for the battery failsafe
//	uint8_t gps : 1; 								// 3   // A status flag for the gps failsafe
//	uint8_t gcs : 1; 								// 4   // A status flag for the ground station failsafe
//	uint8_t ekf : 1; 								// 5   // true if ekf failsafe has occurred
//	uint32_t last_heartbeat_ms;             // the time when the last HEARTBEAT message arrived from a GCS - used for triggering gcs failsafe

} failsafe_t;
extern failsafe_t failsafe;


void set_throttle_failsafe(uint16_t throttle_pwm);
void failsafe_set_radio(uint8_t b);

#endif

