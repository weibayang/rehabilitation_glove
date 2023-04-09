#include "I2Csml.h"
// I2C��ʼ������������ģ������
void I2C_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_GPIO_I2C, ENABLE);

	GPIO_InitStructure.GPIO_Pin		=	I2C_SCL|I2C_SDA;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	=	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_Init(GPIO_I2C, &GPIO_InitStructure);

	I2C_SCL_H;
	I2C_SDA_H;
}

// I2C����Ϊ���ģʽ
void I2C_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin		=	I2C_SDA;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	=	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_Init(GPIO_I2C, &GPIO_InitStructure);
}

// I2C����Ϊ����ģʽ
void I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin		=	I2C_SDA;
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_Init(GPIO_I2C, &GPIO_InitStructure);
}

// ������ʼ�ź�
void I2C_Start(void)
{
	I2C_SDA_OUT();
	I2C_SDA_H;
	I2C_SCL_H;
	
	delay_us(5);
	I2C_SDA_L;
	delay_us(6);
	I2C_SCL_L;
}

// ����ֹͣ�ź�
void I2C_Stop(void)
{
	I2C_SDA_OUT();
	I2C_SCL_L;
	I2C_SDA_L;
	
	I2C_SCL_H;
	delay_us(6);
	I2C_SDA_H;
	delay_us(6);
}

// ��������Ӧ���ź�
void I2C_Ack(void)
{
	I2C_SCL_L;
	I2C_SDA_OUT();
	I2C_SDA_L;
	delay_us(2);
	I2C_SCL_H;
	delay_us(5);
	I2C_SCL_L;
}

// ����������Ӧ���ź�
void I2C_NAck(void)
{
	I2C_SCL_L;
	I2C_SDA_OUT();
	I2C_SDA_H;
	delay_us(2);
	I2C_SCL_H;
	delay_us(5);
	I2C_SCL_L;
}

// �ȴ��ӻ���Ӧ���ź�
u8 I2C_Wait_Ack(void)
{
	u8 tempTime=0;
	I2C_SDA_IN();
	I2C_SDA_H;
	delay_us(1);
	I2C_SCL_H;
	delay_us(1);
	while(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
	{
		tempTime++;
		if(tempTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL_L;
	return 0;
}

// ͨ��I2C����1��8λ�ֽ�
void I2C_Send_Byte(u8 txd)
{
	u8 i=0;
	I2C_SDA_OUT();
	I2C_SCL_L;							//����ʱ�ӿ�ʼ���ݴ���
	for(i=0;i<8;i++)
	{
		if((txd&0x80)>0) 				//0x80 1000 0000
			I2C_SDA_H;
		else
			I2C_SDA_L;
		txd<<=1;
		I2C_SCL_H;
		delay_us(2);  					//��������
		I2C_SCL_L;
		delay_us(2);
	}
}

// ͨ��I2C����1���ֽ�
u8 I2C_Read_Byte(u8 ack)
{
	u8 i=0,receive=0;
	I2C_SDA_IN();
	for(i=0;i<8;i++)
	{
		I2C_SCL_L;
		delay_us(2);
		I2C_SCL_H;
		receive<<=1;
		if(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
			receive++;
		delay_us(1);
	}
	if(ack==0)
		I2C_NAck();
	else
		I2C_Ack();
	return receive;
}

