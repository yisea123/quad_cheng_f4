#include "TIMER.h"
#include "RCCX.h"
#include "GPIO.h"





	
void TIMER::Setup(TIM_TypeDef* timx)
{

	//高级定时器
	if(timx == TIM1)		RCCX.TIM1_.Enable();
	else if(timx == TIM8)	RCCX.TIM8_.Enable();
	//基本定时器
	else if(timx == TIM6)	RCCX.TIM6_.Enable();
	else if(timx == TIM7)	RCCX.TIM7_.Enable();
	//通用定时器
	else if(timx == TIM2)	RCCX.TIM2_.Enable();
	else if(timx == TIM3)	RCCX.TIM3_.Enable();
	else if(timx == TIM4)	RCCX.TIM4_.Enable();
	else if(timx == TIM5)	RCCX.TIM5_.Enable();
	else if(timx == TIM9)	RCCX.TIM9_.Enable();
	else if(timx == TIM10)	RCCX.TIM10_.Enable();
	else if(timx == TIM11)	RCCX.TIM11_.Enable();
	else if(timx == TIM12)	RCCX.TIM12_.Enable();
	else if(timx == TIM13)	RCCX.TIM13_.Enable();
	else if(timx == TIM14)	RCCX.TIM14_.Enable();
	else
	{
		return;
	}
}


//psc 分频值
//arr 重载值
void TIMER::setup_timebase(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc-1; 					//定时器分频
	TIM_TimeBaseInitStructure.TIM_Period = arr-1; 						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIMER::CounterMode_Up; 		//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision= TIMER::CKD_DIV1;
	TIM_TimeBaseInit(_timx,&TIM_TimeBaseInitStructure);				//初始化定时器
}


bool TIMER::_setup_pin_ch1()
{

	if(_timx == TIM1)		GPIO::ModeAF(PA8,GPIO_AF_TIM1);		//CH1
	else if(_timx == TIM8)	GPIO::ModeAF(PC6,GPIO_AF_TIM8);		//CH1
	else if(_timx == TIM2)	GPIO::ModeAF(PA5,GPIO_AF_TIM2);		//CH1
	else if(_timx == TIM3)	GPIO::ModeAF(TIM3_PIN_CH1,GPIO_AF_TIM3);		//CH1
	else if(_timx == TIM4)	GPIO::ModeAF(PB6,GPIO_AF_TIM4);		//CH1
	else if(_timx == TIM5)	GPIO::ModeAF(PA0,GPIO_AF_TIM5);		//CH1
	else if(_timx == TIM9)	GPIO::ModeAF(PA2,GPIO_AF_TIM9);		//CH1
	else if(_timx == TIM10)	GPIO::ModeAF(PB8,GPIO_AF_TIM10);		//CH1
	else if(_timx == TIM11)	GPIO::ModeAF(PB9,GPIO_AF_TIM11);		//CH1
	else if(_timx == TIM12)	GPIO::ModeAF(PB14,GPIO_AF_TIM12);		//CH1
	else if(_timx == TIM13)	GPIO::ModeAF(PA6,GPIO_AF_TIM13);		//CH1
	else if(_timx == TIM14)	GPIO::ModeAF(PA7,GPIO_AF_TIM14);		//CH1
	else
	{
		return false;
	}	
	return true;
	
}
bool TIMER::_setup_pin_ch2()
{
	if(_timx == TIM1)		GPIO::ModeAF(PA9,GPIO_AF_TIM1);		//CH2
	else if(_timx == TIM8)	GPIO::ModeAF(PC7,GPIO_AF_TIM8);		//CH2
	else if(_timx == TIM2)	GPIO::ModeAF(TIM2_PIN_CH2,GPIO_AF_TIM2);		//CH2
	else if(_timx == TIM3)	GPIO::ModeAF(TIM3_PIN_CH2,GPIO_AF_TIM3);		//CH2
	else if(_timx == TIM4)	GPIO::ModeAF(PB7,GPIO_AF_TIM4);			//CH2
	else if(_timx == TIM5)	GPIO::ModeAF(PA1,GPIO_AF_TIM5);			//CH2
	else if(_timx == TIM9)	GPIO::ModeAF(PA3,GPIO_AF_TIM9);			//CH2
	else if(_timx == TIM12)	GPIO::ModeAF(PB15,GPIO_AF_TIM12);		//CH2
	else
	{
		return false;
	}		
	return true;
}
bool TIMER::_setup_pin_ch3()
{
	if(_timx == TIM1)			GPIO::ModeAF(PA10,GPIO_AF_TIM1);	//CH3
	else if(_timx == TIM8)		GPIO::ModeAF(PC8,GPIO_AF_TIM8);		//CH3
	else if(_timx == TIM2)		GPIO::ModeAF(TIM2_PIN_CH3,GPIO_AF_TIM2);	//CH3
	else if(_timx == TIM3)		GPIO::ModeAF(TIM3_PIN_CH3,GPIO_AF_TIM3);		//CH3
	else if(_timx == TIM4)		GPIO::ModeAF(PB8,GPIO_AF_TIM4);		//CH3
	else if(_timx == TIM5)		GPIO::ModeAF(PA2,GPIO_AF_TIM5);		//CH3
	else
	{
		return false;
	}

	return true;
}
bool TIMER::_setup_pin_ch4()
{
	if(_timx == TIM1)			GPIO::ModeAF(PA11,GPIO_AF_TIM1);	//CH4
	else if(_timx == TIM8)		GPIO::ModeAF(PC9,GPIO_AF_TIM8);		//CH4
	else if(_timx == TIM2)		GPIO::ModeAF(TIM2_PIN_CH4,GPIO_AF_TIM2);	//CH4
	else if(_timx == TIM3)		GPIO::ModeAF(TIM3_PIN_CH4,GPIO_AF_TIM3);		//CH4
	else if(_timx == TIM4)		GPIO::ModeAF(PB9,GPIO_AF_TIM4);		//CH4
	else if(_timx == TIM5)		GPIO::ModeAF(PA3,GPIO_AF_TIM5);		//CH4
	else
	{
		return false;
	}	
	
	return true;
}

void TIMER::setup_pwm1(uint16_t Mode , uint16_t OutputState,uint16_t Polarity, uint16_t OutputNState,\
							 uint16_t NPolarity,uint16_t IdleState,uint16_t NIdleState)
{


	if(!_setup_pin_ch1())return;
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = Mode; 				//PWM 调制模式 1
	TIM_OCInitStructure.TIM_OutputState = TIMER::OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 					//初始比较值
	TIM_OCInitStructure.TIM_OCPolarity = Polarity; 		//输出极性
	
	TIM_OCInitStructure.TIM_OutputNState = OutputNState;
	TIM_OCInitStructure.TIM_OCNPolarity = NPolarity;
	TIM_OCInitStructure.TIM_OCIdleState = IdleState;
	TIM_OCInitStructure.TIM_OCNIdleState = NIdleState;
	TIM_OC1Init(_timx, &TIM_OCInitStructure); 			//初始化外设

	if(_timx == TIM1 || _timx == TIM8)	TIM_CtrlPWMOutputs(_timx, ENABLE);

	
	TIM_OC1PreloadConfig(_timx, TIM_OCPreload_Enable); 				//使能CCR1预装载寄存器
	TIM_ARRPreloadConfig(_timx, ENABLE);
}

void TIMER::setup_pwm2(uint16_t Mode , uint16_t OutputState, uint16_t Polarity,\
							uint16_t OutputNState, uint16_t NPolarity,uint16_t IdleState,uint16_t NIdleState)
{

	if(!_setup_pin_ch2())return;
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = Mode; 				//PWM 调制模式 1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 					//初始比较值
	TIM_OCInitStructure.TIM_OCPolarity = Polarity; 		//输出极性
	
	TIM_OCInitStructure.TIM_OutputNState = OutputNState;
	TIM_OCInitStructure.TIM_OCNPolarity = NPolarity;
	TIM_OCInitStructure.TIM_OCIdleState = IdleState;
	TIM_OCInitStructure.TIM_OCNIdleState = NIdleState;
	TIM_OC2Init(_timx, &TIM_OCInitStructure); 			//初始化外设

	if (_timx == TIM1 || _timx == TIM8)	TIM_CtrlPWMOutputs(_timx, ENABLE);

	TIM_OC2PreloadConfig(_timx, TIM_OCPreload_Enable); 				//使能CCR1预装载寄存器
	TIM_ARRPreloadConfig(_timx, ENABLE);
}


void TIMER::setup_pwm3(uint16_t Mode,uint16_t OutputState, uint16_t Polarity,\
							uint16_t OutputNState, uint16_t NPolarity,uint16_t IdleState,uint16_t NIdleState)
{

	if(!_setup_pin_ch3())return;
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = Mode; 				//PWM 调制模式 1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 					//初始比较值
	TIM_OCInitStructure.TIM_OCPolarity = Polarity; 		//输出极性
	
	TIM_OCInitStructure.TIM_OutputNState = OutputNState;
	TIM_OCInitStructure.TIM_OCNPolarity = NPolarity;
	TIM_OCInitStructure.TIM_OCIdleState = IdleState;
	TIM_OCInitStructure.TIM_OCNIdleState = NIdleState;
	TIM_OC3Init(_timx, &TIM_OCInitStructure); 			//初始化外设
	
	if (_timx == TIM1 || _timx == TIM8)	TIM_CtrlPWMOutputs(_timx, ENABLE);

	TIM_OC3PreloadConfig(_timx, TIM_OCPreload_Enable); 				//使能CCR1预装载寄存器
	TIM_ARRPreloadConfig(_timx, ENABLE);
}


void TIMER::setup_pwm4(uint16_t Mode, uint16_t OutputState, uint16_t Polarity,\
							uint16_t OutputNState, uint16_t NPolarity,uint16_t IdleState,uint16_t NIdleState)
{

	if(!_setup_pin_ch4())return;
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = Mode; 				//PWM 调制模式 1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 					//初始比较值
	TIM_OCInitStructure.TIM_OCPolarity = Polarity; 		//输出极性
	
	TIM_OCInitStructure.TIM_OutputNState = OutputNState;
	TIM_OCInitStructure.TIM_OCNPolarity = NPolarity;
	TIM_OCInitStructure.TIM_OCIdleState = IdleState;
	TIM_OCInitStructure.TIM_OCNIdleState = NIdleState;
	TIM_OC4Init(_timx, &TIM_OCInitStructure); 			//初始化外设
	
	if (_timx == TIM1 || _timx == TIM8)	TIM_CtrlPWMOutputs(_timx, ENABLE);

	TIM_OC4PreloadConfig(_timx, TIM_OCPreload_Enable); 				//使能CCR1预装载寄存器
	TIM_ARRPreloadConfig(_timx, ENABLE);
}




void TIMER::setup_capture1(uint16_t Polarity,uint16_t Selection,uint16_t Prescale,uint16_t Filter)
{
	if(!_setup_pin_ch1())return;
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 					//选择输入端 IC1 映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPolarity = Polarity; 		//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = Selection; 	//映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPrescaler = Prescale; 				//配置输入分频,不分频
	TIM_ICInitStructure.TIM_ICFilter = Filter;							//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(_timx, &TIM_ICInitStructure); 							//初始化 TIM5 输入捕获参数
	
	TIM_ITConfig(_timx, TIMER::IT_Update| TIMER::IT_CC1,ENABLE);					//允许更新和捕获中断
}	


void TIMER::setup_capture2(uint16_t Polarity,uint16_t Selection,uint16_t Prescale,uint16_t Filter)
{
	if(!_setup_pin_ch2())return;
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; 					//选择输入端 IC1 映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPolarity = Polarity; 		//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = Selection; 	//映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPrescaler = Prescale; 				//配置输入分频,不分频
	TIM_ICInitStructure.TIM_ICFilter = Filter;							//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(_timx, &TIM_ICInitStructure); 							//初始化 TIM5 输入捕获参数
	
	TIM_ITConfig(_timx, TIMER::IT_Update| TIMER::IT_CC2,ENABLE);					//允许更新和捕获中断
}	


void TIMER::setup_capture3(uint16_t Polarity,uint16_t Selection,uint16_t Prescale,uint16_t Filter)
{
	if(!_setup_pin_ch3())return;
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; 					//选择输入端 IC1 映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPolarity = Polarity; 		//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = Selection; 	//映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPrescaler = Prescale; 				//配置输入分频,不分频
	TIM_ICInitStructure.TIM_ICFilter = Filter;							//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(_timx, &TIM_ICInitStructure); 							//初始化 TIM5 输入捕获参数
	
	TIM_ITConfig(_timx,TIMER::IT_Update| TIMER::IT_CC3,ENABLE);					//允许更新和捕获中断
}	


void TIMER::setup_capture4(uint16_t Polarity,uint16_t Selection,uint16_t Prescale,uint16_t Filter)
{
	if(!_setup_pin_ch4())return;
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; 					//选择输入端 IC1 映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPolarity = Polarity; 		//上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = Selection; 	//映射到 TI1 上
	TIM_ICInitStructure.TIM_ICPrescaler = Prescale; 				//配置输入分频,不分频
	TIM_ICInitStructure.TIM_ICFilter = Filter;							//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(_timx, &TIM_ICInitStructure); 							//初始化 TIM5 输入捕获参数
	
	TIM_ITConfig(_timx, TIMER::IT_Update| TIMER::IT_CC4,ENABLE);					//允许更新和捕获中断
}	



void TIMER::start()
{
	TIM_Cmd(_timx,ENABLE); 	
}

void TIMER::stop()
{
	TIM_Cmd(_timx,DISABLE); 		
}
	

void TIMER::start_timer_micros(uint16_t interval_micros)
{
	uint16_t psc;
	if(_timx == TIM1 || _timx == TIM8 || _timx == TIM9 || _timx == TIM10 || _timx==TIM11)
	{
		psc = 168;
	}
	else
	{
		psc = 84;
	}
	start_timer(psc,interval_micros);
	


}


void TIMER::start_timer(uint16_t psc,uint16_t arr)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc-1; 					//定时器分频
	TIM_TimeBaseInitStructure.TIM_Period = arr-1; 						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 		//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(_timx,&TIM_TimeBaseInitStructure);				//初始化定时器 TIM3
	TIM_ITConfig(_timx,TIM_IT_Update,ENABLE); 						//允许定时器更新中断
	start();	

}





















