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

//	TLC1543���Ŷ���
#define 	IOCLK 	GPIO_Pin_0
#define 	ADDR  	GPIO_Pin_1
#define 	DOUT  	GPIO_Pin_2
#define 	EOC   	GPIO_Pin_3
#define 	CS    	GPIO_Pin_4

// TLC1543Ӧ�ú�������
void TLC1543_GPIOInit(void);
u16 TLC1543_sampleData(u8 Channel);
// ע�⣺Ӧ�ú����е�����delay��������Ҫ��ǰ���г�ʼ��
#endif
