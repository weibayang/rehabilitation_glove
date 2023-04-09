#ifndef _TIM_H
#define _TIM_H
#include "stm32f4xx.h"
#include "MyFunc.h"

void TIM3_init(u16 TicPeriod);
void TIM5_init(u16 TicPeriod);

extern u32 		TIME_Counter;
extern float 	Time;
extern u32  flagUART;

#endif
