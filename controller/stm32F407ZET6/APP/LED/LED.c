#include "LED.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	LED14_OFF;

	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	LED5_OFF;
	LED6_OFF;
	LED7_OFF;
	LED8_OFF;
	LED9_OFF;
	LED10_OFF;
	LED11_OFF;
	LED12_OFF;
	LED13_OFF;
	
//	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
//	GPIO_SetBits(GPIOD, GPIO_Pin_2);
}

void LED_Light(void)
{
	static int flag = 0;
	const char timeL = 50;

	flag++;
	switch(flag){
		case 1*timeL:{LED1_ON;LED14_OFF;break;}
		case 2*timeL:{LED2_ON;LED1_OFF;
		//GPIO_SetBits(GPIOC, GPIO_Pin_4);
		break;}
		case 3*timeL:{LED3_ON;LED2_OFF;
		//GPIO_ResetBits(GPIOC, GPIO_Pin_4);
		break;}
		case 4*timeL:{LED4_ON;LED3_OFF;break;}
		case 5*timeL:{LED5_ON;LED4_OFF;break;}
		case 6*timeL:{LED6_ON;LED5_OFF;break;}
		case 7*timeL:{LED7_ON;LED6_OFF;break;}
		case 8*timeL:{LED8_ON;LED7_OFF;break;}
		case 9*timeL:{LED9_ON;LED8_OFF;break;}
		case 10*timeL:{LED10_ON;LED9_OFF;break;}
		case 11*timeL:{LED11_ON;LED10_OFF;break;}
		case 12*timeL:{LED12_ON;LED11_OFF;break;}
		case 13*timeL:{LED13_ON;LED12_OFF;break;}
		case 14*timeL:{LED14_ON;LED13_OFF;break;}
		default:break;
	}

	if(flag == 15*timeL){
		flag = 0;
	}
}

void LED_Dark(void)
{
//	static char flag = 0;
//	if(flag){
//		flag = 0;
//		GPIO_ResetBits(GPIOD, GPIO_Pin_0);
//	}else{
//		flag = 1;
//		GPIO_SetBits(GPIOD, GPIO_Pin_0);
//	}
}
