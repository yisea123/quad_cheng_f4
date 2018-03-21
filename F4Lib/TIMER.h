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



// 
// template<uint16_t Channel,
// 	void (*OCInit)(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct),
// 	void (*SetCompare)(TIM_TypeDef* TIMx, uint32_t Compare),
// 	void (*ForcedOCConfig)(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction),
// 	void (*OCPreloadConfig)(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload),
// 	void (*OCFastConfig)(TIM_TypeDef* TIMx, uint16_t TIM_OCFast),
// 	void (*ClearOCnRef)(TIM_TypeDef* TIMx, uint16_t TIM_OCClear),
// 	void (*OCPolarityConfig)(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity),
// 	void (*OCNPolarityConfig)(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity),
// 	uint32_t (*GetCapture)(TIM_TypeDef* TIMx),
// 	void (*SetIC1Prescaler)(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)>
// class TIMER_CH
// {
// public:
// 	TIMER_CH(TIM_TypeDef	*timx) :
// 		_timx(timx)
// 	{	
// 	
// 	}
// 
// 	void Init(uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection, uint16_t TIM_ICPrescaler, uint16_t TIM_ICFilter);
// 
// 
// private:
// 	TIM_TypeDef	*_timx;
// };
// 
// template<uint16_t Channel, 
// 	void(*OCInit)(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *TIM_OCInitStruct), 
// 	void(*SetCompare)(TIM_TypeDef *TIMx, uint32_t Compare), 
// 	void(*ForcedOCConfig)(TIM_TypeDef *TIMx, uint16_t TIM_ForcedAction), 
// 	void(*OCPreloadConfig)(TIM_TypeDef *TIMx, uint16_t TIM_OCPreload), 
// 	void(*OCFastConfig)(TIM_TypeDef *TIMx, uint16_t TIM_OCFast), 
// 	void(*ClearOCnRef)(TIM_TypeDef *TIMx, uint16_t TIM_OCClear), 
// 	void(*OCPolarityConfig)(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity), 
// 	void(*OCNPolarityConfig)(TIM_TypeDef *TIMx, uint16_t TIM_OCNPolarity), 
// 	uint32_t(*GetCapture)(TIM_TypeDef *TIMx), 
// 	void(*SetIC1Prescaler)(TIM_TypeDef *TIMx, uint16_t TIM_ICPSC)>
// void TIMER_CH<Channel, OCInit, SetCompare, ForcedOCConfig, OCPreloadConfig, OCFastConfig, 
// 				ClearOCnRef, OCPolarityConfig, OCNPolarityConfig, GetCapture, SetIC1Prescaler>
// ::Init(uint16_t ICPolarity, uint16_t ICSelection, uint16_t ICPrescaler, uint16_t ICFilter)
// {
// 	TIM_ICInitTypeDef TIM_ICInitStructure;
// 	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 					//选择输入端 IC1 映射到 TI1 上
// 	TIM_ICInitStructure.TIM_ICPolarity = ICPolarity; 		//上升沿捕获
// 	TIM_ICInitStructure.TIM_ICSelection = ICSelection; 	//映射到 TI1 上
// 	TIM_ICInitStructure.TIM_ICPrescaler = ICPrescaler; 				//配置输入分频,不分频
// 	TIM_ICInitStructure.TIM_ICFilter = ICFilter;							//IC1F=0000 配置输入滤波器 不滤波
// 	TIM_ICInit(_timx, &TIM_ICInitStructure); 							//初始化 TIM5 输入捕获参数
// 
// }


class TIMER
{
public:
	
	TIMER(TIM_TypeDef* timx):_timx(timx)
	{}


	enum
	{
		IT_Update = TIM_IT_Update,
		IT_CC1 = TIM_IT_CC1,
		IT_CC2 = TIM_IT_CC2,
		IT_CC3 = TIM_IT_CC3,
		IT_CC4 = TIM_IT_CC4,
		IT_COM = TIM_IT_COM,
		IT_Trigger = TIM_IT_Trigger,
		IT_Break = TIM_IT_Break,
	};
	enum
	{
		FLAG_Update = TIM_FLAG_Update,
		FLAG_CC1 = TIM_FLAG_CC1,
		FLAG_CC2 = TIM_FLAG_CC2,
		FLAG_CC3 = TIM_FLAG_CC3,
		FLAG_CC4 = TIM_FLAG_CC4,
		FLAG_COM = TIM_FLAG_COM,
		FLAG_Break = TIM_FLAG_Break,
		FLAG_CC1OF = TIM_FLAG_CC1OF,
		FLAG_CC2OF = TIM_FLAG_CC2OF,
		FLAG_CC3OF = TIM_FLAG_CC3OF,
		FLAG_CC4OF = TIM_FLAG_CC4OF,
	};
	
	//////TIMER_Base
	enum 
	{
		CounterMode_Up = TIM_CounterMode_Up,
		CounterMode_Down = TIM_CounterMode_Down,
		CounterMode_CenterAligned1 = TIM_CounterMode_CenterAligned1,
		CounterMode_CenterAligned2 = TIM_CounterMode_CenterAligned2,
		CounterMode_CenterAligned3 = TIM_CounterMode_CenterAligned3
	};
	enum 
	{
		CKD_DIV1 = TIM_CKD_DIV1,
		CKD_DIV2 = TIM_CKD_DIV2,
		CKD_DIV4 = TIM_CKD_DIV4
	};
	enum 
	{
		Channel_1 = TIM_Channel_1,
		Channel_2 = TIM_Channel_2,
		Channel_3 = TIM_Channel_3,
		Channel_4 = TIM_Channel_4
	};

	//////TIMER_OC
	enum
	{
		OCMode_Timing = TIM_OCMode_Timing,
		OCMode_Active = TIM_OCMode_Active,
		OCMode_Inactive = TIM_OCMode_Inactive,
		OCMode_Toggle = TIM_OCMode_Toggle,
		OCMode_PWM1 = TIM_OCMode_PWM1,
		OCMode_PWM2 = TIM_OCMode_PWM2,
	};
	enum
	{
		OCPolarity_High = TIM_OCPolarity_High,
		OCPolarity_Low = TIM_OCPolarity_Low
	};
	enum
	{
		OutputState_Disable = TIM_OutputState_Disable,
		OutputState_Enable = TIM_OutputState_Enable
	};
	//////TIMER_OCN
	enum
	{
		OutputNState_Disable = TIM_OutputNState_Disable,
		OutputNState_Enable = TIM_OutputNState_Enable
	};
	enum
	{
		OCNPolarity_High = TIM_OCNPolarity_High,
		OCNPolarity_Low = TIM_OCNPolarity_Low
	};
	enum
	{
		OCIdleState_Set = TIM_OCIdleState_Set,
		OCIdleState_Reset = TIM_OCIdleState_Reset
	};
	enum
	{
		OCNIdleState_Set = TIM_OCNIdleState_Set,
		OCNIdleState_Reset = TIM_OCNIdleState_Reset
	};
	//////TIMER_IC
	enum 
	{
		ICPolarity_Rising = TIM_ICPolarity_Rising,
		ICPolarity_Falling = TIM_ICPolarity_Falling,
		ICPolarity_BothEdge = TIM_ICPolarity_BothEdge
	};
	enum 
	{
		ICSelection_DirectTI = TIM_ICSelection_DirectTI,
		ICSelection_IndirectTI = TIM_ICSelection_IndirectTI
	};
	enum 
	{
		ICPSC_DIV1 = TIM_ICPSC_DIV1,
		ICPSC_DIV2 = TIM_ICPSC_DIV2,
		ICPSC_DIV4 = TIM_ICPSC_DIV4
	};
	
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
	void setup_pwm1(uint16_t Mode = TIMER::OCMode_PWM1,\
							uint16_t OutputState = TIMER::OutputState_Enable,
							uint16_t Polarity=TIMER::OCPolarity_High,\
							//TIM1,TIM8才使用
							uint16_t OutputNState = TIMER::OutputNState_Disable,\
							uint16_t NPolarity = TIMER::OCNPolarity_High,\
							uint16_t IdleState= TIMER::OCIdleState_Reset,\
							uint16_t NIdleState= TIMER::OCNIdleState_Reset);

	void setup_pwm2(uint16_t Mode = TIMER::OCMode_PWM1,\
							uint16_t OutputState = TIMER::OutputState_Enable,
							uint16_t Polarity= TIMER::OCPolarity_High,\
							//TIM1,TIM8才使用
							uint16_t OutputNState = TIMER::OutputNState_Disable,
							uint16_t NPolarity = TIMER::OCNPolarity_High,\
							uint16_t IdleState=TIMER::OCIdleState_Reset,\
							uint16_t NIdleState=TIMER::OCNIdleState_Reset);	
	
	void setup_pwm3(uint16_t Mode = TIMER::OCMode_PWM1,\
							uint16_t OutputState = TIMER::OutputState_Enable,
							uint16_t Polarity=TIMER::OCPolarity_High,\
							//TIM1,TIM8才使用
							uint16_t OutputNState = TIMER::OutputNState_Disable,\
							uint16_t NPolarity = TIMER::OCNPolarity_High,\
							uint16_t IdleState=TIMER::OCIdleState_Reset,\
							uint16_t NIdleState=TIMER::OCNIdleState_Reset);	
	
	void setup_pwm4(uint16_t Mode = TIMER::OCMode_PWM1,\
							uint16_t OutputState = TIMER::OutputState_Enable,
							uint16_t Polarity=TIMER::OCPolarity_High,\
							//TIM1,TIM8才使用
							uint16_t OutputNState = TIMER::OutputNState_Disable,\
							uint16_t NPolarity = TIMER::OCNPolarity_High,\
							uint16_t IdleState=TIMER::OCIdleState_Reset,\
							uint16_t NIdleState=TIMER::OCNIdleState_Reset);	
	
	
	
		
	void set_compare1(uint32_t compare1)	{ TIM_SetCompare1(_timx,compare1);}
	void set_compare2(uint32_t compare2)	{ TIM_SetCompare2(_timx,compare2);}
	void set_compare3(uint32_t compare3)	{ TIM_SetCompare3(_timx,compare3);}
	void set_compare4(uint32_t compare4)	{ TIM_SetCompare4(_timx,compare4);}
	
	
	void setup_capture1(uint16_t Polarity = TIMER::ICPolarity_Rising,\
							uint16_t Selection = TIMER::ICSelection_DirectTI,\
							uint16_t Prescaler	= TIMER::ICPSC_DIV1,
							uint16_t Filter	=  0x0);
	void setup_capture2(uint16_t Polarity = TIMER::ICPolarity_Rising,\
				uint16_t Selection = TIMER::ICSelection_DirectTI,\
				uint16_t Prescaler	= TIMER::ICPSC_DIV1,
				uint16_t Filter	=  0x0);
	void setup_capture3(uint16_t Polarity = TIMER::ICPolarity_Rising,\
				uint16_t Selection = TIMER::ICSelection_DirectTI,\
				uint16_t Prescaler	= TIMER::ICPSC_DIV1,
				uint16_t Filter	=  0x0);
	void setup_capture4(uint16_t Polarity = TIMER::ICPolarity_Rising,\
				uint16_t Selection = TIMER::ICSelection_DirectTI,\
				uint16_t Prescaler	= TIMER::ICPSC_DIV1,
				uint16_t Filter	=  0x0);
	
	void set_capture1_polarity(uint16_t Polarity)	{ TIM_OC1PolarityConfig(_timx,Polarity);}
	void set_capture2_polarity(uint16_t Polarity)	{ TIM_OC2PolarityConfig(_timx,Polarity);}
	void set_capture3_polarity(uint16_t Polarity)	{ TIM_OC3PolarityConfig(_timx,Polarity);}
	void set_capture4_polarity(uint16_t Polarity)	{ TIM_OC4PolarityConfig(_timx,Polarity);}
	
	
	
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

