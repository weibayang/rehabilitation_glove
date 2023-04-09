#include "SPIAD7606.h"
//*	函 数 名: AD7606_GPIO_Init
//*	功能说明: 初始化AD7606所需要的GPIO口
//*	形    参: 无
//*	返 回 值: 无
void AD7606_GPIO_Init(void)
{
	int i = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,     ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,   ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,   ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* 主机MCU输出GPIO配置 */
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	=	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	//9
	GPIO_InitStructure.GPIO_Pin		=	AD7606_CS0;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		=	AD7606_CS1;	
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		=	AD7606_CS2|AD7606_CS3|AD7606_CV|AD7606_SCLK;//
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	/* 主机MCU输入GPIO配置 */	
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin		= AD7606_BUSY0 | AD7606_BUSY1;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= AD7606_BUSY2;	
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin		=	AD7606_BUSY3|AD7606_DoutA|AD7606_DoutB;	
	GPIO_Init(GPIOE,   &GPIO_InitStructure);
	
	
//	GPIO_InitStructure.GPIO_Pin		=	AD7606_BUSY0;	//AD7606_BUSY2|
//	GPIO_Init(GPIOF,   &GPIO_InitStructure);
	
	/* 起始停止全部ADC设备的转换和输出 */
	AD7606_CV_H;
	
//	for(i = 0; i < NumOfADC; i++)
//	{
//		CS_H(i);
//	}	
	CS_H0;
	CS_H1;
	CS_H2;
	CS_H3;
	
}


//*	函 数 名: AD7606_SetRange
//*	功能说明: 选择AD7606的测量范围
//*	形    参: u8 Range	[1：正负10V；0：正负5V]
//*	返 回 值: 无
void AD7606_SetRange(u8 Range)
{
	if(Range == 1)
	{
		AD7606_Range_H;
	}
	else
	{
		AD7606_Range_L;
	}
}


//*	函 数 名: AD7606_SetOverSample
//*	功能说明: 选择AD7606的过采样率
//*	形    参: u8 OverSample		取值范围[0,6]
//*	返 回 值: 无
void AD7606_SetOverSample(u8 OverSample)
{
	switch(OverSample)
	{
		case 0:					// 000->NO OS
			AD7606_OS_L2;
			AD7606_OS_L1;
			AD7606_OS_L0;
			break;
		case 1:					// 001->2
			AD7606_OS_L2;
			AD7606_OS_L1;
			AD7606_OS_H0;
			break;
		case 2:					// 010->4
			AD7606_OS_L2;
			AD7606_OS_H1;
			AD7606_OS_L0;
			break;
		case 3:					// 011->8
			AD7606_OS_L2;
			AD7606_OS_H1;
			AD7606_OS_H0;
			break;
		case 4:					// 100->16
			AD7606_OS_H2;
			AD7606_OS_L1;
			AD7606_OS_L0;
			break;
		case 5:					// 101->32
			AD7606_OS_H2;
			AD7606_OS_L1;
			AD7606_OS_H0;
			break;
		case 6:					// 110->64
			AD7606_OS_H2;
			AD7606_OS_H1;
			AD7606_OS_L0;
			break;
		default:
			break;
	}
}


//*	函 数 名: AD7606_AllDeviceSample_AB
//*	功能说明: 所有AD7606设备的所有通道同时采样，通过DoutA和DoutB接收所有数据
//*	形    参: u16 SampleData[8*NumOfADC]
//*	返 回 值: 无
void AD7606_AllDeviceSample_AB(u16 SampleData[8*NumOfADC])
{
	int i = 0;
	int j = 0;
	int k = 0;
	u16 read_A 	= 0;
	u16 read_B 	= 0;
	u16 mask	= 0x8000;
	
	AD7606_CV_L;										// 触发一次采样
	AD7606_CV_H;
	
//	while(IS_BUSYSHARE_HIGH);							// 等待转换完成
	
//	for(i = 0; i < NumOfADC; i++)
//	//CS0
	{
		while(IS_BUSY_HIGH0);							// 等待转换完成
		
		CS_L0;
		for(j = 0; j < 4; j++)
		{
			read_A 	= 0;
			read_B 	= 0;
			mask 	= 0x8000;
			for(k = 0; k < 16; k++)
			{
				AD7606_SCLK_L;
				if(IS_DoutA_HIGH)
				{
					read_A |= mask;
				}
				if(IS_DoutB_HIGH)
				{
					read_B |= mask;
				}
				
				AD7606_SCLK_H;
				mask >>= 1;
			}
			SampleData[j] = read_A;
			SampleData[j+4] = read_B;
		}
		CS_H0;
	}
	//CS1
	{
		while(IS_BUSY_HIGH1);							// 等待转换完成
		
		CS_L1;
		for(j = 0; j < 4; j++)
		{
			read_A 	= 0;
			read_B 	= 0;
			mask 	= 0x8000;
			for(k = 0; k < 16; k++)
			{
				AD7606_SCLK_L;
				if(IS_DoutA_HIGH)
				{
					read_A |= mask;
				}
				if(IS_DoutB_HIGH)
				{
					read_B |= mask;
				}
				
				AD7606_SCLK_H;
				mask >>= 1;
			}
			SampleData[8*1+j] = read_A;
			SampleData[8*1+j+4] = read_B;
		}
		CS_H1;
	}
	
	//CS2
	{
		while(IS_BUSY_HIGH2);								// 等待转换完成
		
		CS_L2;
		for(j = 0; j < 4; j++)
		{
			read_A 	= 0;
			read_B 	= 0;
			mask 	= 0x8000;
			for(k = 0; k < 16; k++)
			{
				AD7606_SCLK_L;
				if(IS_DoutA_HIGH)
				{
					read_A |= mask;
				}
				if(IS_DoutB_HIGH)
				{
					read_B |= mask;
				}
				
				AD7606_SCLK_H;
				mask >>= 1;
			}
			SampleData[8*2+j] = read_A;
			SampleData[8*2+j+4] = read_B;
		}
		CS_H2;
	}
	//CS3
	{
		while(IS_BUSY_HIGH3);								// 等待转换完成
		
		CS_L3;
		for(j = 0; j < 4; j++)
		{
			read_A 	= 0;
			read_B 	= 0;
			mask 	= 0x8000;
			for(k = 0; k < 16; k++)
			{
				AD7606_SCLK_L;
				if(IS_DoutA_HIGH)
				{
					read_A |= mask;
				}
				if(IS_DoutB_HIGH)
				{
					read_B |= mask;
				}
				
				AD7606_SCLK_H;
				mask >>= 1;
			}
			SampleData[8*3+j] = read_A;
			SampleData[8*3+j+4] = read_B;
		}
		CS_H3;
	}
}
	

//*	函 数 名: AD7606_DeviceSample_AB
//*	功能说明: 选择AD7606设备采样，通过DoutA和DoutB接收所有数据
//*	形    参: u16 SampleData[8*NumOfADC]
//*	返 回 值: 无
void AD7606_DeviceSample_AB(u16 SampleData[8], u8 Device)
{
	int i = 0;
	int j = 0;
	u16 read_A 	= 0;
	u16 read_B 	= 0;
	u16 mask	= 0x8000;
	
	AD7606_CV_L;										// 触发一次采样
	AD7606_CV_H;
	
	if(Device == 0){
		while(IS_BUSY_HIGH0);						// 等待转换完成
		CS_L0;		
	}
	if(Device == 1){
		while(IS_BUSY_HIGH1);						// 等待转换完成
		CS_L1;		
	}
	if(Device == 2){
		while(IS_BUSY_HIGH2);						// 等待转换完成
		CS_L2;		
	}	
	
	for(i = 0; i < 4; i++)
	{
		read_A 	= 0;
		read_B 	= 0;
		mask 	= 0x8000;
		
		for(j = 0; j < 16; j++)
		{
			AD7606_SCLK_L;
			if(IS_DoutA_HIGH)
			{
				read_A |= mask;
			}
			if(IS_DoutB_HIGH)
			{
				read_B |= mask;
			}
			
			AD7606_SCLK_H;
			mask >>= 1;
		}
		
		SampleData[i] 	= read_A;
		SampleData[i+4] = read_B;
	}
	if(Device == 0){
		CS_H0;
	}
	if(Device == 1){
		CS_H1;
	}	
	if(Device == 2){
		CS_H2;
	}	
}
