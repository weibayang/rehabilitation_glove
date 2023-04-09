#include "SPIAD7606.h"
//*	�� �� ��: AD7606_GPIO_Init
//*	����˵��: ��ʼ��AD7606����Ҫ��GPIO��
//*	��    ��: ��
//*	�� �� ֵ: ��
void AD7606_GPIO_Init(void)
{
	int i = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,     ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,   ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,   ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/* ����MCU���GPIO���� */
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
	
	/* ����MCU����GPIO���� */	
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
	
	/* ��ʼֹͣȫ��ADC�豸��ת������� */
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


//*	�� �� ��: AD7606_SetRange
//*	����˵��: ѡ��AD7606�Ĳ�����Χ
//*	��    ��: u8 Range	[1������10V��0������5V]
//*	�� �� ֵ: ��
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


//*	�� �� ��: AD7606_SetOverSample
//*	����˵��: ѡ��AD7606�Ĺ�������
//*	��    ��: u8 OverSample		ȡֵ��Χ[0,6]
//*	�� �� ֵ: ��
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


//*	�� �� ��: AD7606_AllDeviceSample_AB
//*	����˵��: ����AD7606�豸������ͨ��ͬʱ������ͨ��DoutA��DoutB������������
//*	��    ��: u16 SampleData[8*NumOfADC]
//*	�� �� ֵ: ��
void AD7606_AllDeviceSample_AB(u16 SampleData[8*NumOfADC])
{
	int i = 0;
	int j = 0;
	int k = 0;
	u16 read_A 	= 0;
	u16 read_B 	= 0;
	u16 mask	= 0x8000;
	
	AD7606_CV_L;										// ����һ�β���
	AD7606_CV_H;
	
//	while(IS_BUSYSHARE_HIGH);							// �ȴ�ת�����
	
//	for(i = 0; i < NumOfADC; i++)
//	//CS0
	{
		while(IS_BUSY_HIGH0);							// �ȴ�ת�����
		
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
		while(IS_BUSY_HIGH1);							// �ȴ�ת�����
		
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
		while(IS_BUSY_HIGH2);								// �ȴ�ת�����
		
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
		while(IS_BUSY_HIGH3);								// �ȴ�ת�����
		
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
	

//*	�� �� ��: AD7606_DeviceSample_AB
//*	����˵��: ѡ��AD7606�豸������ͨ��DoutA��DoutB������������
//*	��    ��: u16 SampleData[8*NumOfADC]
//*	�� �� ֵ: ��
void AD7606_DeviceSample_AB(u16 SampleData[8], u8 Device)
{
	int i = 0;
	int j = 0;
	u16 read_A 	= 0;
	u16 read_B 	= 0;
	u16 mask	= 0x8000;
	
	AD7606_CV_L;										// ����һ�β���
	AD7606_CV_H;
	
	if(Device == 0){
		while(IS_BUSY_HIGH0);						// �ȴ�ת�����
		CS_L0;		
	}
	if(Device == 1){
		while(IS_BUSY_HIGH1);						// �ȴ�ת�����
		CS_L1;		
	}
	if(Device == 2){
		while(IS_BUSY_HIGH2);						// �ȴ�ת�����
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
