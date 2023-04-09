#include "TIM.h"
u32 	TIME_Counter 	= 0;
float 	Time 			= 0;
u32  flagUART = 0;
//ϵͳʱ�Ӷ�ʱ��Tim3��ʼ������
void TIM3_init(u16 TicPeriod)
{
//  �������룬��ʱ���жϴ������� TicPeriod����λus
//	��ʱ��Ĭ��ʱ��Ϊ��84MHz
//	TIM_Period��ÿ�����ڵ��Զ���װֵ
//	TIM_Prescaler��ʱ��Ԥ��Ƶ��
//	��ʱ�����ڼ��㣺
//	Tout= ((TIM_Period+1)*(TIM_Prescaler+1))/Tclk��
//	TclkΪ��ʱ��ʱ��

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);						//�ҽ���APB1�ϵĻ�����ʱ����ʱ��Ƶ��Ϊ72MHz
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);									// ����жϱ�־λ
	
	TIM_TimeBaseInitStructure.TIM_Period 		= 	TicPeriod;//TicPeriod*84;			// ÿ�����ڵ��Զ���װֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler 	= 	83;						// ʱ��Ԥ��Ƶ������������10us
	TIM_TimeBaseInitStructure.TIM_CounterMode 	= 	TIM_CounterMode_Up;			// ���ϼ���
	TIM_TimeBaseInitStructure.TIM_ClockDivision	=	TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  /*�ظ������Ĵ���*/

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 									// �������´����ļ�ʱ��3�ж�
	
	// ��ʱ���жϳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel						= TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


//ϵͳʱ�Ӷ�ʱ��Tim5��ʼ������
void TIM5_init(u16 TicPeriod)
{
//  �������룬��ʱ���жϴ������� TicPeriod����λus
//	��ʱ��Ĭ��ʱ��Ϊ��84MHz
//	TIM_Period��ÿ�����ڵ��Զ���װֵ
//	TIM_Prescaler��ʱ��Ԥ��Ƶ��
//	��ʱ�����ڼ��㣺
//	Tout= ((TIM_Period+1)*(TIM_Prescaler+1))/Tclk��
//	TclkΪ��ʱ��ʱ��

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);						//�ҽ���APB1�ϵĻ�����ʱ����ʱ��Ƶ��Ϊ72MHz
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);									// ����жϱ�־λ
	
	TIM_TimeBaseInitStructure.TIM_Period 		= 	TicPeriod*84;			// ÿ�����ڵ��Զ���װֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler 	= 	0;						// ʱ��Ԥ��Ƶ������������10us
	TIM_TimeBaseInitStructure.TIM_CounterMode 	= 	TIM_CounterMode_Up;			// ���ϼ���
	TIM_TimeBaseInitStructure.TIM_ClockDivision	=	TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  /*�ظ������Ĵ���*/
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); 									// �������´����ļ�ʱ��3�ж�
	
	// ��ʱ���жϳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel						= TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}


// ��ʱ��Tim3�жϺ���
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		TIME_Counter++;
		flagUART++;
	}
//	if(TIME_Counter%30 == 0){
//		//		/********** ���ڽ���ָ�� **********/
//		UsartDataRec();

//		/*********** ��������ƹ��� ***********/
////		SensorSignal(da,shi,zhong,wuming,xiao);

////		controller(shi);
////		controller(zhong);
////		controller(wuming);
////		controller(xiao);

////		PWMDriver(da,shi,zhong,wuming,xiao);
//	}
}

// ��ʱ��Tim4�жϺ���
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
