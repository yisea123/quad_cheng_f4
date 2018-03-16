#include "ppm.h"

#include "string.h"


enum
{
	PPM_EMPTY = 0,
	PPM_AVAILABLE,
	PPM_USING,
};

ppm_t ppm;



void ppm_period_capture_event(uint16_t perioid_micros)
{
	static bool started = false;
	if (perioid_micros < 1000)		//´íÎó
	{
		ppm.rx_cnt = 0;
		started = false;
	}
	else if( perioid_micros > 2500)
	{
		if(started && ppm.state != PPM_USING)
		{
			memcpy(ppm.channel, ppm.buffer, ppm.rx_cnt * sizeof(ppm.buffer[0]));
			ppm.state = PPM_AVAILABLE;
		}
		started = true;
		ppm.rx_cnt = 0;
	}
	else
	{
		if(ppm.rx_cnt < 8)
		{
			ppm.buffer[ppm.rx_cnt] = perioid_micros;
			ppm.rx_cnt++;
		}
		
	}

}

bool ppm_available(void)
{
	return (ppm.state == PPM_AVAILABLE);
}

bool ppm_using(void)
{
	return (ppm.state == PPM_USING);
}

