#ifndef _TIMER_H_
#define _TIMER_H_
#include "sys.h"




/**************TIM2 通道引脚****************/
//#define TIM2_PIN_CH2	PA1			//TIM5_PIN_CH2
//#define TIM2_PIN_CH2	PB3
#define TIM2_PIN_CH2	PB15


//#define TIM2_PIN_CH3	PA2			//TIM5_PIN_CH3
#define TIM2_PIN_CH3	PB10


//#define TIM2_PIN_CH4	PA3			//TIM5_PIN_CH3
#define TIM2_PIN_CH4	PB11

/**************TIM3 通道引脚****************/
#define TIM3_PIN_CH1	PC6			//TIM8_PIN_CH1
//#define TIM3_PIN_CH1	PB4
//#define TIM3_PIN_CH1	PA6


#define TIM3_PIN_CH2	PC7			//TIM8_PIN_CH2
//#define TIM3_PIN_CH2	PB5
//#define TIM3_PIN_CH2	PA7

#define TIM3_PIN_CH3	PC8			//TIM8_PIN_CH3
//#define TIM3_PIN_CH3	PB0	

#define TIM3_PIN_CH4	PC9			//TIM8_PIN_CH4
//#define TIM3_PIN_CH4	PB1





class TIMER
{
public:
	
	TIMER(TIM_TypeDef* timx):_timx(timx)
	{}

	static void Setup(TIM_TypeDef* timx);	
	

	
	void add_interrupt(uint16_t it) {
		TIM_ITConfig(_timx, it, ENABLE);
	}
	void remove_interrupt(uint16_t it) {
		TIM_ITConfig(_timx,it, DISABLE);
	}
	bool check_interrupt(uint16_t it) {
		return TIM_GetITStatus(_timx,it) == SET ? true : false;
	}
	bool get_flag(uint16_t flag) {
		return TIM_GetFlagStatus(_timx,flag) == SET ? true : false;
	}
	void clear_flag(uint16_t flag) {
		TIM_ClearFlag(_timx,flag);
	}


	

	
	
	
	void set_counter(uint32_t cnt)	{TIM_SetCounter(_timx, cnt);}
	uint32_t get_counter()	{ return TIM_GetCounter(_timx);}
	
	
	
	
	
	void setup_timebase(uint16_t psc,uint16_t arr);	
	void setup_pwm1(uint16_t Mode = TIM_OCMode_PWM1,\
							uint16_t OutputState = TIM_OutputState_Enable,\
							uint16_t Polarity=TIM_OCPolarity_High,\
							//TIM1,TIM8才使用
							uint16_t OutputNState = TIM_OutputNState_Disable,\
							uint16_t NPolarity = TIM_OCNPolarity_High,\
							uint16_t IdleState=TIM_OCIdleState_Reset,\
							uint16_t NIdleState=TIM_OCNIdleState_Reset);	
	void setup_pwm2(uint16_t Mode = TIM_OCMode_PWM1,\
							uint16_t OutputState = TIM_OutputState_Enable,\
							uint16_t Polarity=TIM_OCPolarity_High,\
							//TIM1,TIM8才使用
							uint16_t OutputNState = TIM_OutputNState_Disable,\
							uint16_t NPolarity = TIM_OCNPolarity_High,\
							uint16_t IdleState=TIM_OCIdleState_Reset,\
							uint16_t NIdleState=TIM_OCNIdleState_Reset);	
	
	void setup_pwm3(uint16_t Mode = TIM_OCMode_PWM1,\
							uint16_t OutputState = TIM_OutputState_Enable,\
							uint16_t Polarity=TIM_OCPolarity_High,\
							//TIM1,TIM8才使用
							uint16_t OutputNState = TIM_OutputNState_Disable,\
							uint16_t NPolarity = TIM_OCNPolarity_High,\
							uint16_t IdleState=TIM_OCIdleState_Reset,\
							uint16_t NIdleState=TIM_OCNIdleState_Reset);	
	
	void setup_pwm4(uint16_t Mode = TIM_OCMode_PWM1,\
							uint16_t OutputState = TIM_OutputState_Enable,\
							uint16_t Polarity=TIM_OCPolarity_High,\
							//TIM1,TIM8才使用
							uint16_t OutputNState = TIM_OutputNState_Disable,\
							uint16_t NPolarity = TIM_OCNPolarity_High,\
							uint16_t IdleState=TIM_OCIdleState_Reset,\
							uint16_t NIdleState=TIM_OCNIdleState_Reset);	
	
	
	
		
	void set_compare1(uint32_t compare1)	{TIM_SetCompare1(_timx,compare1);}
	void set_compare2(uint32_t compare2)	{TIM_SetCompare2(_timx,compare2);}
	void set_compare3(uint32_t compare3)	{TIM_SetCompare3(_timx,compare3);}
	void set_compare4(uint32_t compare4)	{TIM_SetCompare4(_timx,compare4);}
	
	
	
	void setup_capture1(uint16_t Polarity = TIM_ICPolarity_Rising,\
							uint16_t Selection = TIM_ICSelection_DirectTI,\
							uint16_t Prescaler	= TIM_ICPSC_DIV1,
							uint16_t Filter	=  0x0);
	void setup_capture2(uint16_t Polarity = TIM_ICPolarity_Rising,\
				uint16_t Selection = TIM_ICSelection_DirectTI,\
				uint16_t Prescaler	= TIM_ICPSC_DIV1,
				uint16_t Filter	=  0x0);
	void setup_capture3(uint16_t Polarity = TIM_ICPolarity_Rising,\
				uint16_t Selection = TIM_ICSelection_DirectTI,\
				uint16_t Prescaler	= TIM_ICPSC_DIV1,
				uint16_t Filter	=  0x0);
	void setup_capture4(uint16_t Polarity = TIM_ICPolarity_Rising,\
				uint16_t Selection = TIM_ICSelection_DirectTI,\
				uint16_t Prescaler	= TIM_ICPSC_DIV1,
				uint16_t Filter	=  0x0);
	
	void set_capture1_polarity(uint16_t Polarity)	{TIM_OC1PolarityConfig(_timx,Polarity);}
	void set_capture2_polarity(uint16_t Polarity)	{TIM_OC2PolarityConfig(_timx,Polarity);}
	void set_capture3_polarity(uint16_t Polarity)	{TIM_OC3PolarityConfig(_timx,Polarity);}
	void set_capture4_polarity(uint16_t Polarity)	{TIM_OC4PolarityConfig(_timx,Polarity);}
	
	
	
	uint32_t get_capture1()	{return TIM_GetCapture1(_timx);}
	uint32_t get_capture2()	{return TIM_GetCapture2(_timx);}
	uint32_t get_capture3()	{return TIM_GetCapture3(_timx);}
	uint32_t get_capture4()	{return TIM_GetCapture4(_timx);}
	
	
	
	
	
	//interval_micros 中断时间
	void start_timer_micros(uint16_t interval_micros);		
	void start_timer(uint16_t psc,uint16_t arr);	
	


	void start();
	void stop();

private:
	//TIM_TimeBaseInitTypeDef _TIM_TimeBaseInitStructure;
	TIM_TypeDef	*_timx;

	bool _setup_pin_ch1();
	bool _setup_pin_ch2();
	bool _setup_pin_ch3();
	bool _setup_pin_ch4();


};



#endif
