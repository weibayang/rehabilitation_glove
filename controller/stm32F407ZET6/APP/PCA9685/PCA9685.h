#ifndef _PCA9685_H
#define _PCA9685_H
#include "stm32f4xx.h"
#include "I2Csml.h"
#include "math.h"
/****** Ӳ����ַ ******/
#define PCA9685_Addr_1 		0x80					//Ӳ����ַ1000 000 0
#define PCA9685_Addr_2 		0xc0					//Ӳ����ַ1100 000 0

/****** �Ĵ�����ַ ******/
#define LEDn_ON_L(n)		(0x06 + (n)*4)					//����n�ߵ�λ�Ͱ�λ�Ĵ�����ַ
#define LEDn_ON_H(n)		(0x07 + (n)*4)					//����n�ߵ�λ�߰�λ�Ĵ�����ַ
#define LEDn_OFF_L(n)		(0x08 + (n)*4)					//����n�͵�λ�Ͱ�λ�Ĵ�����ַ
#define LEDn_OFF_H(n)		(0x09 + (n)*4)					//����n�͵�λ�Ͱ�λ�Ĵ�����ַ
// nȡֵ��0~15
#define PCA9685_MODE1            0x00
#define PCA9685_PRE_SCALE      	 0xfe						//PRE_SCALE���ڼĴ���

/****** MODE1ģʽѡ��(ʹ��ʱ��Ҫ���С��롯����) ******/
#define Restart					 0x80						//д1��λ
#define ExtClock_ON				 0x40						//д0ʹ���ڲ�25MHzʱ�ӣ�д1ʹ���ⲿʱ��
#define AI_ON					 0x20						//д0�ڲ���ַ��д���Զ����ӣ�д1�ڲ���ַ�Զ�����
#define Sleep_ON 				 0x10						//д0�˳�SLEEPģʽ��д1����SLEEPģʽ���޸����ڼĴ�����ʱ�ӼĴ�����
#define AllCall_ON               0x01						//д0����Ӧ0x07IICͨ�õ�ַ��д1��Ӧ0x07ͨ�õ�ַ
#define INIT                     0x00

// PCA9685Ӧ�ú�������
void PrescaleConfig_PCA9685(u8 SCALE_Addr, u8 prescale);
void setPWM(u8 SCALE_Addr, u8 n, u16 led_off);				//Ĭ�ϴ�0��ʼ��ͨ������led_off������ռ�ձ�
u8 Prescale_Calculate(u16 frequence);
#endif






















