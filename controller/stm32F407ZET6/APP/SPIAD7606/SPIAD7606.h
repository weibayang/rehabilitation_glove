#ifndef __SPIAD7606_H
#define __SPIAD7606_H
#include "stm32f4xx.h"
/*
	AD7606  <-------->  STM32f407ZET6
	5.0V    ----------  	5.0V		5V����
	GND     ----------  	GND     	����
	
	OS0		<---------      PG0			������ѡ��ʮ����ȡֵ��Χ[0,6]
	OS1		<---------      PG1			000->�ޣ�001->2��010->4��011->8��100->16��101->32��110->64��111->��Ч��
	OS2		<---------      PG2
	Rage    <---------      PG3			������Χ��1������10V��0������5V��
	CV      <---------      PG4			����ת�����أ��̽�CVA��CVB��8ͨ��ͬʱ����ת����
	SCLK    <---------      PG5			ʱ��
	DA(DB7) --------->      PG7			ת������������A�ڣ�ͨ��1��2��3��4��
	DB(DB8) --------->      PG8			ת������������B�ڣ�ͨ��5��6��7��8��	
		 
	CS_0    <---------  	PD0    		CS0���ӻ�ѡ��
	CS_1    <---------  	PD1     	CS1���ӻ�ѡ��
	CS_2    <---------  	PD2			CS2���ӻ�ѡ��
	CS_3    <---------  	PD3			CS3���ӻ�ѡ��
			  .......
	BUSY_0  <---------  	PF0    		�ӻ�0ת����ɱ�־λ
	BUSY_1  <---------  	PF1     	�ӻ�1ת����ɱ�־λ
	BUSY_2  <---------  	PF2			�ӻ�2ת����ɱ�־λ
	BUSY_3  <---------  	PF3			�ӻ�3ת����ɱ�־λ
			  .......
*/

#define NumOfADC			4 	// ����AD7606�豸����

//static u16 P[16] 	= {GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7,GPIO_Pin_8,
//GPIO_Pin_9,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15};

//#define GPIO_AD7606			GPIOA		
//#define GPIO_CS				GPIOD
//#define GPIO_BUSY			GPIOF

//#define RCC_GPIO_AD7606 	RCC_AHB1Periph_GPIOG
//#define RCC_GPIO_CS 		RCC_AHB1Periph_GPIOD
//#define RCC_GPIO_BUSY 		RCC_AHB1Periph_GPIOF

#define AD7606_OS0		GPIO_Pin_7
#define AD7606_OS1		GPIO_Pin_1
#define AD7606_OS2		GPIO_Pin_0

#define AD7606_Range		GPIO_Pin_15
#define AD7606_CV			GPIO_Pin_13					   
#define AD7606_SCLK			GPIO_Pin_12
//#define AD7606_BUSY_Share	GPIO_Pin_11				   
#define AD7606_DoutA		GPIO_Pin_9
#define AD7606_DoutB		GPIO_Pin_8

#define AD7606_CS0		GPIO_Pin_12
#define AD7606_CS1		GPIO_Pin_0
#define AD7606_CS2	  GPIO_Pin_7
#define AD7606_CS3	  GPIO_Pin_11

#define AD7606_BUSY0		GPIO_Pin_11
#define AD7606_BUSY1		GPIO_Pin_15
#define AD7606_BUSY2		GPIO_Pin_1
#define AD7606_BUSY3		GPIO_Pin_10

#define AD7606_OS_H0 		GPIO_SetBits(GPIOE, AD7606_OS0)				// AD7606_OS�ø�
#define AD7606_OS_H1 		GPIO_SetBits(GPIOG, AD7606_OS1)				// AD7606_OS�ø�
#define AD7606_OS_H2 		GPIO_SetBits(GPIOG, AD7606_OS2)				// AD7606_OS�ø�

#define AD7606_OS_L0		GPIO_ResetBits(GPIOE, AD7606_OS0)			// AD7606_OS�õ�
#define AD7606_OS_L1		GPIO_ResetBits(GPIOG, AD7606_OS1)			// AD7606_OS�õ�
#define AD7606_OS_L2		GPIO_ResetBits(GPIOG, AD7606_OS2)			// AD7606_OS�õ�

#define AD7606_Range_H 		GPIO_SetBits(GPIOF, AD7606_Range)				// AD7606_SCLK�ø�
#define AD7606_Range_L 		GPIO_ResetBits(GPIOF, AD7606_Range)			// AD7606_SCLK�õ�

#define AD7606_CV_H 		GPIO_SetBits(GPIOE, AD7606_CV)				// AD7606_CV�ø�
#define AD7606_CV_L 		GPIO_ResetBits(GPIOE, AD7606_CV)				// AD7606_CV�õ�
#define AD7606_SCLK_H 		GPIO_SetBits(GPIOE, AD7606_SCLK)				// AD7606_SCLK�ø�
#define AD7606_SCLK_L 		GPIO_ResetBits(GPIOE, AD7606_SCLK)			// AD7606_SCLK�õ�

#define CS_H0		 		GPIO_SetBits(GPIOF, AD7606_CS0)					// ��x��Ƭѡ�ø�
#define CS_H1		 		GPIO_SetBits(GPIOG, AD7606_CS1)					// ��x��Ƭѡ�ø�
#define CS_H2		 		GPIO_SetBits(GPIOE, AD7606_CS2)					// ��x��Ƭѡ�ø�
#define CS_H3		 		GPIO_SetBits(GPIOE, AD7606_CS3)					// ��x��Ƭѡ�ø�

#define CS_L0				GPIO_ResetBits(GPIOF, AD7606_CS0)				// ��x��Ƭѡ�õ�	
#define CS_L1				GPIO_ResetBits(GPIOG, AD7606_CS1)				// ��x��Ƭѡ�õ�	
#define CS_L2				GPIO_ResetBits(GPIOE, AD7606_CS2)				// ��x��Ƭѡ�õ�	
#define CS_L3				GPIO_ResetBits(GPIOE, AD7606_CS3)				// ��x��Ƭѡ�õ�	

#define IS_BUSY_HIGH0		GPIO_ReadInputDataBit(GPIOF, AD7606_BUSY0) 	== Bit_SET		// �ж�BUSY��ƽ�� �ߵ�ƽ����1���͵�ƽ����0
#define IS_BUSY_HIGH1		GPIO_ReadInputDataBit(GPIOF, AD7606_BUSY1) 	== Bit_SET		// �ж�BUSY��ƽ�� �ߵ�ƽ����1���͵�ƽ����0
#define IS_BUSY_HIGH2		GPIO_ReadInputDataBit(GPIOG, AD7606_BUSY2)  == Bit_SET		// �ж�BUSY��ƽ�� �ߵ�ƽ����1���͵�ƽ����0
#define IS_BUSY_HIGH3		GPIO_ReadInputDataBit(GPIOE, AD7606_BUSY3)  == Bit_SET		// �ж�BUSY��ƽ�� �ߵ�ƽ����1���͵�ƽ����0


//#define IS_BUSYSHARE_HIGH	GPIO_ReadInputDataBit(GPIO_AD7606, AD7606_BUSY_Share) 	== Bit_SET	// �ж�BUSY��ƽ�� �ߵ�ƽ����1���͵�ƽ����0
#define IS_DoutA_HIGH		GPIO_ReadInputDataBit(GPIOE, AD7606_DoutA) 	== Bit_SET		// �ж�DoutA��ƽ���ߵ�ƽ����1���͵�ƽ����0
#define IS_DoutB_HIGH		GPIO_ReadInputDataBit(GPIOE, AD7606_DoutB) 	== Bit_SET		// �ж�DoutB��ƽ���ߵ�ƽ����1���͵�ƽ����0


void AD7606_GPIO_Init(void);
void AD7606_SetRange(u8 Range);
void AD7606_SetOverSample(u8 OverSample);
void AD7606_AllDeviceSample_AB(u16 SampleData[8*NumOfADC]);
void AD7606_DeviceSample_AB(u16 SampleData[8], u8 Device);
#endif
