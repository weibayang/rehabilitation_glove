#include "TIM.h"
u32 	TIME_Counter 	= 0;
float 	Time 			= 0;
u32  flagUART = 0;
//系统时钟定时器Tim3初始化函数
void TIM3_init(u16 TicPeriod)
{
//  函数输入，定时器中断触发周期 TicPeriod：单位us
//	定时器默认时钟为：84MHz
//	TIM_Period：每个周期的自动重装值
//	TIM_Prescaler：时钟预分频数
//	定时器周期计算：
//	Tout= ((TIM_Period+1)*(TIM_Prescaler+1))/Tclk；
//	Tclk为定时器时钟

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);						//挂接在APB1上的基本定时器的时钟频率为72MHz
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);									// 清除中断标志位
	
	TIM_TimeBaseInitStructure.TIM_Period 		= 	TicPeriod;//TicPeriod*84;			// 每个周期的自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler 	= 	83;						// 时钟预分频数：计数周期10us
	TIM_TimeBaseInitStructure.TIM_CounterMode 	= 	TIM_CounterMode_Up;			// 向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision	=	TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  /*重复计数寄存器*/

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 									// 启动更新触发的计时器3中断
	
	// 计时器中断初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel						= TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


//系统时钟定时器Tim5初始化函数
void TIM5_init(u16 TicPeriod)
{
//  函数输入，定时器中断触发周期 TicPeriod：单位us
//	定时器默认时钟为：84MHz
//	TIM_Period：每个周期的自动重装值
//	TIM_Prescaler：时钟预分频数
//	定时器周期计算：
//	Tout= ((TIM_Period+1)*(TIM_Prescaler+1))/Tclk；
//	Tclk为定时器时钟

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);						//挂接在APB1上的基本定时器的时钟频率为72MHz
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);									// 清除中断标志位
	
	TIM_TimeBaseInitStructure.TIM_Period 		= 	TicPeriod*84;			// 每个周期的自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler 	= 	0;						// 时钟预分频数：计数周期10us
	TIM_TimeBaseInitStructure.TIM_CounterMode 	= 	TIM_CounterMode_Up;			// 向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision	=	TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  /*重复计数寄存器*/
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); 									// 启动更新触发的计时器3中断
	
	// 计时器中断初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel						= TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


// 定时器Tim3中断函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		TIME_Counter++;
		flagUART++;
	}
//	if(TIME_Counter%30 == 0){
//		//		/********** 串口接收指令 **********/
//		UsartDataRec();

//		/*********** 传感与控制过程 ***********/
////		SensorSignal(da,shi,zhong,wuming,xiao);

////		controller(shi);
////		controller(zhong);
////		controller(wuming);
////		controller(xiao);

////		PWMDriver(da,shi,zhong,wuming,xiao);
//	}
}

// 定时器Tim4中断函数
void TIM5_IRQHandler(void)
{
	static long a=0;
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		a++;
//		controller();
	}
	if(a/30 == 1){
		a=0;
	}
}
