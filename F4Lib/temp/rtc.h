#ifndef	_RTC_H_
#define _RTC_H_

#include "sys.h"


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

ErrorStatus RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);
ErrorStatus RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week);
void RTC_Set_WakeUp(uint32_t wksel,uint16_t cnt);
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);
uint8_t My_RTC_Init(void);
	 

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif




