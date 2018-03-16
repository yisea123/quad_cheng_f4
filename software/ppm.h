#ifndef _PPM_H_
#define _PPM_H_
#include "sys.h"




typedef struct
{
	uint8_t state;
	uint16_t channel[8];

	uint8_t rx_cnt;
	uint16_t buffer[10];				
}ppm_t;
extern ppm_t ppm;

void ppm_period_capture_event(uint16_t perioid_micros);
bool ppm_available(void);
bool ppm_using(void);

#endif


