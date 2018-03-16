#ifndef _SPI_H_
#define _SPI_H_



#include "sys.h"
#include "GPIO.h"


class SPI
{
public:	
	typedef enum
	{
		Rising_Edge,
		Falling_Edge
	}LatchEdge;
	
	typedef enum
	{
		MSB_First,
		LSB_First
	}FirstBit;
	
	SPI(GPIO  pin_cs):cs(pin_cs){}
	virtual ~SPI(){};
	
	//½Ó¿Ú
	virtual void init(LatchEdge = Rising_Edge,FirstBit firstbit = MSB_First)const  = 0;
	virtual void set_speed(uint8_t div)	const = 	0;
	virtual uint8_t write(uint8_t c)	const  = 0 ;

	//
	uint8_t read()	const {return write(0xFF);	}
	void write(const void *buf,uint16_t len)	const ;
	void read(void *buf,uint16_t len)	const ;

	const GPIO cs;
private:
	
};





#endif



