#include "SPIADS1256.h"

//*	函 数 名: SPIADS1256_GPIO_Init
//*	功能说明: 配置STM32的GPIO和SPI接口，用于连接ADS1256
//*	形    参: 无
//*	返 回 值: 无
void SPIADS1256_GPIO_Init(void)
{
	int i = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_GPIO_SPI, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_GPIO_CS, ENABLE);

	/* 主机MCU输出GPIO配置 */
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	=	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin		=	SPI_DIN|SPI_SCLK;	
	GPIO_Init(GPIO_SPI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		=	SPI_CS[0]|SPI_CS[1]|SPI_CS[2]|SPI_CS[3]|SPI_CS[4];	
	GPIO_Init(GPIO_CS, &GPIO_InitStructure);

	/* 主机MCU输入GPIO配置 */	
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin		=	DRDY|SPI_DOUT;	
	GPIO_Init(GPIO_SPI, &GPIO_InitStructure);
	
	/* 起始全部从设备失能 */
	for(i = 0; i < NumOfADS; i++)
		SPI_CS_H(i);
}
	

//*	函 数 名: ADS1256_DelaySCLK
//*	功能说明: CLK之间的延迟，时序延迟. 用于STM32F407  168M主频
//*	形    参: 无
//*	返 回 值: 无
void ADS1256_DelaySCLK(void)
{
	u16 i;
	// 	取 5 时，实测高电平200ns, 低电平250ns <-- 不稳定 
	//	取 10 以上，可以正常工作， 低电平400ns 高定400ns <--- 稳定
	for (i = 0; i < 12; i++);
}


//*	函 数 名: ADS1256_Send8Bit
//*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
//*	形    参: _data : u8数据
//*	返 回 值: 无
void ADS1256_Send8Bit(u8 data)
{
	int i;

	ADS1256_DelaySCLK();		/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	
	for(i = 0; i < 8; i++)
	{
		if (data & 0x80)
		{
			SPI_DIN_H;
		}
		else
		{
			SPI_DIN_L;
		}
		SPI_SCLK_H;				
		ADS1256_DelaySCLK();		
		data <<= 1;		
		SPI_SCLK_L;			/* ADS1256 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
		ADS1256_DelaySCLK();		
	}
}


//*	函 数 名: ADS1256_Recive8Bit
//*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
//*	形    参: 无
//*	返 回 值: 无
u8 ADS1256_Recive8Bit(void)
{
	int i;
	u8 read = 0;

	ADS1256_DelaySCLK();		/*　ADS1256 要求 SCL高电平和低电平持续时间最小 200ns  */
	
	for (i = 0; i < 8; i++)
	{
		SPI_SCLK_H;
		ADS1256_DelaySCLK();
		read = read << 1;
		SPI_SCLK_L;
		ADS1256_DelaySCLK();
		if(IS_DOUT_HIGH)
		{
			read |= 0x1;
		}		
	}
	return read;
}


//*	函 数 名: ADS1256_GetAdcData
//*	功能说明: 获取指定ADS1256通道的ADC数据
//*	形    参: device：设备号（0-4）；channel：通道号（0-7）
//*	返 回 值: 返回一个u32数据
u32 ADS1256_GetAdcData(u8 device, u8 channel)
{	
	int i;
	u32 temp[3] = {0};
	u32 RawData = 0;
	
	/* 选中指定的ADS1256设备，其余设备失能 */
	for(i = 0; i < NumOfADS; i++)
	{
		if(i == device)
			SPI_CS_L(i);
		else
			SPI_CS_H(i);
	}

	/* 等待 DRDY 置低，跳出循环 */
	while (!IS_DRDY_LOW);

	/* 切换模拟通道（非差分输入） */
	ADS1256_Send8Bit(CmdWREG | 1);					/* 发送写寄存器命令, 并发送寄存器地址 */
	ADS1256_Send8Bit(0x00);							/* 发送寄存器个数 - 1, 此处0表示写1个寄存器 */
	ADS1256_Send8Bit((channel << 4)| 8);			/* 设置状态寄存器 */		
	delay_us(1);
	
	/* 发送同步AD转换命令 */
	ADS1256_Send8Bit(CmdSYNC);
	delay_us(1);
	
	/* 发送完成同步或退出等待命令 */
	ADS1256_Send8Bit(CmdWAKEUP);		
	delay_us(1);
	
	/* 发送读取数据命令 */
	ADS1256_Send8Bit(CmdRDATA);
	delay_us(8);									/* 最小延迟 6.5uS, 此处取10us */

	/* 读采样结果，3个字节，高字节在前 */
	RawData = ADS1256_Recive8Bit() << 16;
	RawData += ADS1256_Recive8Bit() << 8;
	RawData += ADS1256_Recive8Bit() << 0;
	
	/* 取23位数据中的13位*/
	RawData = (RawData >> 10);
	
	/* 失能所有ADS1256设备 */
	for(i = 0; i < NumOfADS; i++)	
		SPI_CS_H(i);

	return RawData;
}

//*	函 数 名: ADS1256_Config
//*	功能说明: 配置若干个ADS1255/6的基本状态、输入增益、采样速率
//*	形    参: 无
//*	返 回 值: 无
void ADS1256_Config(void)
{
	u8 buf[3];
	int i = 0;
	
	//STATUS:[ID3, ID2, ID1, ID0, ORDER, ACAL, BUFEN, DRDY]
	//STATUS:[-, -, -, -, 先发高位，关闭自校验，关闭输入缓存器, -]
	buf[0] = (0 << 3) | (0 << 2) | (0 << 1);
	//ADCON:[0, CLK1, CLK0, SDCS1, SCDS0, PGA2, PGA1, PGA0]
	//ADCON:[-, 关闭时钟输出, 关闭电流源检测, 放大增益（1）]
	buf[1] = (0 << 5) | (0 << 3) | 0;
	//DRATE:[数据输出速率（30000SPS -- 11110000 -- 240）]
	//DRATE:[数据输出速率（15000SPS -- 11100000 -- 224）]
	//DRATE:[数据输出速率（1000SPS -- 10100001 -- 161）]
	//DRATE:[数据输出速率（100SPS -- 10000010 -- 130）]
	//DRATE:[数据输出速率（10SPS -- 00100011 -- 35）]
	buf[2] = 240;
	
	for(i = 0; i < NumOfADS; i++)
	{
		/* SPI片选第i个ADS1256 */
		SPI_CS_L(i);
		
		ADS1256_Send8Bit(CmdWREG | 0);		/* 发送写寄存器命令, 并发送寄存器地址 */
		ADS1256_Send8Bit(0x00);				/* 发送寄存器个数 - 1, 此处0表示写1个寄存器 */
		
		ADS1256_Send8Bit(buf[0]);			/* 设置状态寄存器 */
		
		delay_us(5);
		
		ADS1256_Send8Bit(CmdWREG | 2);		/* 发送写寄存器命令, 并发送寄存器地址 */
		ADS1256_Send8Bit(0x01);				/* 发送寄存器个数 - 1, 此处1表示写2个寄存器 */
		
		ADS1256_Send8Bit(buf[1]);			/* 设置ADCON控制寄存器，增益 */
		ADS1256_Send8Bit(buf[2]);			/* 设置输出数据速率 */	
		
		SPI_CS_H(i);
	}
}

