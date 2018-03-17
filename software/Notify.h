#ifndef _NOTIFY_H_
#define _NOTIFY_H_
#include "sys.h"

class Notify
{
public:
	Notify():
		_count(0)
	{}
	~Notify(){}

	void init(void);

	static struct notify_flags
	{
		uint16_t armed : 1;
		uint16_t pre_arm_check : 1;
		uint16_t initialising : 1;
		
	}flags;

	void extern_led_update(void);
	void update(void);
private:
	uint8_t _count;

};



#endif

