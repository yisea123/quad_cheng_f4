#include "sys.h"
#include "HAL_F4.h"

#include "QMath.h"
#include "InertialSensor_ICM20602.h"
#include "Compass_AK8975.h"
#include "AHRS_DCM.h"
#include "Parameters.h"
#include "Notify.h"
#include "state.h"
#include "Ano_DT.h"
#include "AttitudeControl.h"
#include "Motors.h"
#include "sbus.h"
#include "Attitude.h"
#include "failsafe.h"
#include "AHRS_Q.h"
#include "AverageFilter.h"

#include "SPIFlash.h"

state_t state;
Parameters g;
Motors motors(g.rcout_roll, g.rcout_pitch, g.rcout_throttle, g.rcout_yaw);


int main()
{
	uint16_t t = 0;
//	uint8_t i = 0;
//	uint8_t buf[100];

	hal.Setup();
	sbus_init();




	hal.led1 = 1;
	hal.led2 = 0;

	hal.LEDR = 1;
	hal.LEDG = 0;
	hal.LEDB = 1;


	hal.tim3.set_compare1(1700);
	hal.tim3.set_compare2(1600);
	hal.tim3.set_compare3(1500);
	hal.tim3.set_compare4(1400);

	hal.tim1.set_compare1(1300);
	hal.tim1.set_compare2(1200);
	hal.tim1.set_compare3(1100);

	hal.tim12.set_compare2(1800);




	while (1)
	{
		printf("FLASH ID=%x\r\n", spiflash.id());
		hal.usart2.puts("usart2");
		hal.usart3.puts("usart3");
		hal.uart5.puts("uart5");


		printf("ch1=%d,ch2=%d,ch3=%d,ch4=%d\r\n", sbus.channel.ch1, sbus.channel.ch2, sbus.channel.ch3, sbus.channel.ch4);

		puts("");
		puts("");

		delay_ms(500);




		if(++t == 5)
		{
			t = 0;
			hal.led1 = !hal.led1;
			hal.led2 = !hal.led2;
		
			hal.LEDG = !hal.LEDG;
		}
		


	}

}



