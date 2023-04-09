#ifndef _I2Csml_H
#define _I2Csml_H
#include "stm32f4xx.h"
#include "delay.h"
// I2C��ģ������
#define I2C_SDA 		GPIO_Pin_6									// PC6
#define I2C_SCL 		GPIO_Pin_5									// PC7
#define GPIO_I2C 		GPIOA
#define RCC_GPIO_I2C 	RCC_AHB1Periph_GPIOE

#define I2C_SDA_H 		GPIO_SetBits(GPIO_I2C,I2C_SDA)				// SDA�ø�
#define I2C_SDA_L 		GPIO_ResetBits(GPIO_I2C, I2C_SDA)			// SDA�õ�
#define I2C_SCL_H 		GPIO_SetBits(GPIO_I2C, I2C_SCL)				// SCL�ø�
#define I2C_SCL_L 		GPIO_ResetBits(GPIO_I2C, I2C_SCL)			// SCL�õ�

// I2Cģ��Ӧ�ú�������
void I2C_INIT(void);		
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
u8   I2C_Wait_Ack(void);
void I2C_Send_Byte(u8 txd);
u8   I2C_Read_Byte(u8 ack);
// ע�⣺Ӧ�ú����е�����delay��������Ҫ��ǰ���г�ʼ��
#endif
