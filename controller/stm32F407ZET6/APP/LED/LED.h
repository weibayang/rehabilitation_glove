#ifndef _LED_H
#define _LED_H
#include "stm32f4xx.h"

#define LED1_ON       GPIO_ResetBits(GPIOD, GPIO_Pin_6)
#define LED1_OFF      GPIO_SetBits(GPIOD, GPIO_Pin_6)
#define LED2_ON       GPIO_ResetBits(GPIOD, GPIO_Pin_7)
#define LED2_OFF      GPIO_SetBits(GPIOD, GPIO_Pin_7)

#define LED3_ON       GPIO_ResetBits(GPIOG, GPIO_Pin_9)
#define LED3_OFF      GPIO_SetBits(GPIOG, GPIO_Pin_9)
#define LED4_ON       GPIO_ResetBits(GPIOG, GPIO_Pin_10)
#define LED4_OFF      GPIO_SetBits(GPIOG, GPIO_Pin_10)
#define LED5_ON       GPIO_ResetBits(GPIOG, GPIO_Pin_11)
#define LED5_OFF      GPIO_SetBits(GPIOG, GPIO_Pin_11)
#define LED6_ON       GPIO_ResetBits(GPIOG, GPIO_Pin_12)
#define LED6_OFF      GPIO_SetBits(GPIOG, GPIO_Pin_12)
#define LED7_ON       GPIO_ResetBits(GPIOG, GPIO_Pin_13)
#define LED7_OFF      GPIO_SetBits(GPIOG, GPIO_Pin_13)

#define LED8_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_3)
#define LED8_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_3)
#define LED9_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_4)
#define LED9_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_4)
#define LED10_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define LED10_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED11_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define LED11_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LED12_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define LED12_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define LED13_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define LED13_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LED14_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define LED14_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_9)


void LED_Init(void);
void LED_Light(void);
void LED_Dark(void);

#endif
