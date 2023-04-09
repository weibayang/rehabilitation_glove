#include "TLC1543.h"
	
// TLC1543硬件初始化
void TLC1543_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStruct.GPIO_Pin 	= 	IOCLK|ADDR|CS|DOUT|EOC;	
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType 	=	GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin 	= 	DOUT|EOC;	
	GPIO_InitStruct.GPIO_Mode 	= 	GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

// 采样函数	
u16 TLC1543_sampleData(u8 Channel)
{
	unsigned int ConvertValue;
	unsigned char i, Chan;
	unsigned char ConvertValueL, ConvertValueH;
	unsigned char delay;
 
 	ConvertValueL = ConvertValueH = 0; 
 	delay = 0;
	if(GPIO_ReadInputDataBit(GPIOE, EOC))
	{
		GPIO_ResetBits(GPIOE, IOCLK);
		
		GPIO_SetBits(GPIOE, CS);
		delay_us(1);//Wait2us;
		GPIO_ResetBits(GPIOE, CS);
		delay_us(1);//Wait2us;
		
		Channel = Channel << 4;
		
		for (i = 0; i < 4; i ++) 				//第一个10位时钟周期的前4位用来发送通道地址
		{
			Chan = Channel;
			Chan = Chan >> 7;
			if(Chan)
				GPIO_SetBits(GPIOE, ADDR);
			else
				GPIO_ResetBits(GPIOE, ADDR);
			delay_us(2);//Wait2us;
			
			GPIO_SetBits(GPIOE, IOCLK);
			GPIO_ResetBits(GPIOE, IOCLK);
			Channel = Channel << 1;
		}	
		for (i = 0; i < 6;i ++)					//第一个10位时钟周期的后6位无信息 
		{
			GPIO_SetBits(GPIOE, IOCLK);
			GPIO_ResetBits(GPIOE, IOCLK);
		}
				
		GPIO_SetBits(GPIOE, CS);
		
		while ((!GPIO_ReadInputDataBit(GPIOE, EOC)) && (delay < 10))	//跳过转化时间，防止信号出错
		{
			delay_us(10);//Wait10us;
			delay ++;
		}
		if (delay == 10)
		{
			return (0xFFFF);
		}
		else
		{										//开始第二个10位时钟周期，用来读取相应通道的转化值
			delay_us(10);//Wait10us;
			GPIO_ResetBits(GPIOE, IOCLK);
			
			GPIO_SetBits(GPIOE, CS);
			delay_us(1);//Wait1us;
			GPIO_ResetBits(GPIOE, CS);
			delay_us(1);//Wait1us;
			
			for (i = 0; i < 2; i ++) 
			{
				GPIO_SetBits(GPIOE, IOCLK);
				ConvertValueH <<= 1;
				if(GPIO_ReadInputDataBit(GPIOE, DOUT))
					ConvertValueH |= 0x1;
				GPIO_ResetBits(GPIOE, IOCLK);
				delay_us(1);//Wait1us;
			} 
			for (i = 0; i < 8; i ++) 
			{
				GPIO_SetBits(GPIOE, IOCLK);
				ConvertValueL <<= 1;
				if(GPIO_ReadInputDataBit(GPIOE, DOUT))
					ConvertValueL |= 0x1;
				GPIO_ResetBits(GPIOE, IOCLK);
				delay_us(1);//Wait1us;
			} 
			
			GPIO_SetBits(GPIOE, CS);
			ConvertValue = ConvertValueH;
			ConvertValue <<= 8;
			ConvertValue |= ConvertValueL;
			return ConvertValue; 
		}
	}
}
