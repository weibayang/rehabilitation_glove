#ifndef __SPIAD7606_H
#define __SPIAD7606_H
#include "stm32f4xx.h"
/*
	AD7606  <-------->  STM32f407ZET6
	5.0V    ----------  	5.0V		5V供电
	GND     ----------  	GND     	共地
	
	OS0		<---------      PG0			过采样选择：十进制取值范围[0,6]
	OS1		<---------      PG1			000->无；001->2；010->4；011->8；100->16；101->32；110->64；111->无效；
	OS2		<---------      PG2
	Rage    <---------      PG3			测量范围（1：正负10V；0：正负5V）
	CV      <---------      PG4			触发转换开关（短接CVA和CVB，8通道同时进行转换）
	SCLK    <---------      PG5			时钟
	DA(DB7) --------->      PG7			转换完成数据输出A口（通道1、2、3、4）
	DB(DB8) --------->      PG8			转换完成数据输出B口（通道5、6、7、8）	
		 
	CS_0    <---------  	PD0    		CS0（从机选择）
	CS_1    <---------  	PD1     	CS1（从机选择）
	CS_2    <---------  	PD2			CS2（从机选择）
	CS_3    <---------  	PD3			CS3（从机选择）
			  .......
	BUSY_0  <---------  	PF0    		从机0转换完成标志位
	BUSY_1  <---------  	PF1     	从机1转换完成标志位
	BUSY_2  <---------  	PF2			从机2转换完成标志位
	BUSY_3  <---------  	PF3			从机3转换完成标志位
			  .......
*/

#define NumOfADC			4 	// 所有AD7606设备数量

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

#define AD7606_OS_H0 		GPIO_SetBits(GPIOE, AD7606_OS0)				// AD7606_OS置高
#define AD7606_OS_H1 		GPIO_SetBits(GPIOG, AD7606_OS1)				// AD7606_OS置高
#define AD7606_OS_H2 		GPIO_SetBits(GPIOG, AD7606_OS2)				// AD7606_OS置高

#define AD7606_OS_L0		GPIO_ResetBits(GPIOE, AD7606_OS0)			// AD7606_OS置低
#define AD7606_OS_L1		GPIO_ResetBits(GPIOG, AD7606_OS1)			// AD7606_OS置低
#define AD7606_OS_L2		GPIO_ResetBits(GPIOG, AD7606_OS2)			// AD7606_OS置低

#define AD7606_Range_H 		GPIO_SetBits(GPIOF, AD7606_Range)				// AD7606_SCLK置高
#define AD7606_Range_L 		GPIO_ResetBits(GPIOF, AD7606_Range)			// AD7606_SCLK置低

#define AD7606_CV_H 		GPIO_SetBits(GPIOE, AD7606_CV)				// AD7606_CV置高
#define AD7606_CV_L 		GPIO_ResetBits(GPIOE, AD7606_CV)				// AD7606_CV置低
#define AD7606_SCLK_H 		GPIO_SetBits(GPIOE, AD7606_SCLK)				// AD7606_SCLK置高
#define AD7606_SCLK_L 		GPIO_ResetBits(GPIOE, AD7606_SCLK)			// AD7606_SCLK置低

#define CS_H0		 		GPIO_SetBits(GPIOF, AD7606_CS0)					// 第x个片选置高
#define CS_H1		 		GPIO_SetBits(GPIOG, AD7606_CS1)					// 第x个片选置高
#define CS_H2		 		GPIO_SetBits(GPIOE, AD7606_CS2)					// 第x个片选置高
#define CS_H3		 		GPIO_SetBits(GPIOE, AD7606_CS3)					// 第x个片选置高

#define CS_L0				GPIO_ResetBits(GPIOF, AD7606_CS0)				// 第x个片选置低	
#define CS_L1				GPIO_ResetBits(GPIOG, AD7606_CS1)				// 第x个片选置低	
#define CS_L2				GPIO_ResetBits(GPIOE, AD7606_CS2)				// 第x个片选置低	
#define CS_L3				GPIO_ResetBits(GPIOE, AD7606_CS3)				// 第x个片选置低	

#define IS_BUSY_HIGH0		GPIO_ReadInputDataBit(GPIOF, AD7606_BUSY0) 	== Bit_SET		// 判断BUSY电平： 高电平返回1；低电平返回0
#define IS_BUSY_HIGH1		GPIO_ReadInputDataBit(GPIOF, AD7606_BUSY1) 	== Bit_SET		// 判断BUSY电平： 高电平返回1；低电平返回0
#define IS_BUSY_HIGH2		GPIO_ReadInputDataBit(GPIOG, AD7606_BUSY2)  == Bit_SET		// 判断BUSY电平： 高电平返回1；低电平返回0
#define IS_BUSY_HIGH3		GPIO_ReadInputDataBit(GPIOE, AD7606_BUSY3)  == Bit_SET		// 判断BUSY电平： 高电平返回1；低电平返回0


//#define IS_BUSYSHARE_HIGH	GPIO_ReadInputDataBit(GPIO_AD7606, AD7606_BUSY_Share) 	== Bit_SET	// 判断BUSY电平： 高电平返回1；低电平返回0
#define IS_DoutA_HIGH		GPIO_ReadInputDataBit(GPIOE, AD7606_DoutA) 	== Bit_SET		// 判断DoutA电平：高电平返回1；低电平返回0
#define IS_DoutB_HIGH		GPIO_ReadInputDataBit(GPIOE, AD7606_DoutB) 	== Bit_SET		// 判断DoutB电平：高电平返回1；低电平返回0


void AD7606_GPIO_Init(void);
void AD7606_SetRange(u8 Range);
void AD7606_SetOverSample(u8 OverSample);
void AD7606_AllDeviceSample_AB(u16 SampleData[8*NumOfADC]);
void AD7606_DeviceSample_AB(u16 SampleData[8], u8 Device);
#endif
