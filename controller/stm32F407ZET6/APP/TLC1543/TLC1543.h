#ifndef _TLC1543_H
#define _TLC1543_H
#include "stm32F4xx.h"
#include "delay.h"
/*
STM32F10X<=>TLC1543

PE4=>CS
PE3=>EOC
PE0=>IOCLK
PE2=>DOUT
PE1=>ADDR
*/

//	TLC1543引脚定义
#define 	IOCLK 	GPIO_Pin_0
#define 	ADDR  	GPIO_Pin_1
#define 	DOUT  	GPIO_Pin_2
#define 	EOC   	GPIO_Pin_3
#define 	CS    	GPIO_Pin_4

// TLC1543应用函数声明
void TLC1543_GPIOInit(void);
u16 TLC1543_sampleData(u8 Channel);
// 注意：应用函数中调用了delay函数，需要提前进行初始化
#endif
