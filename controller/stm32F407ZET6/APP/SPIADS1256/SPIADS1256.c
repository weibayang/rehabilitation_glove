#include "SPIADS1256.h"

//*	�� �� ��: SPIADS1256_GPIO_Init
//*	����˵��: ����STM32��GPIO��SPI�ӿڣ���������ADS1256
//*	��    ��: ��
//*	�� �� ֵ: ��
void SPIADS1256_GPIO_Init(void)
{
	int i = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_GPIO_SPI, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_GPIO_CS, ENABLE);

	/* ����MCU���GPIO���� */
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	=	GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin		=	SPI_DIN|SPI_SCLK;	
	GPIO_Init(GPIO_SPI, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		=	SPI_CS[0]|SPI_CS[1]|SPI_CS[2]|SPI_CS[3]|SPI_CS[4];	
	GPIO_Init(GPIO_CS, &GPIO_InitStructure);

	/* ����MCU����GPIO���� */	
	GPIO_InitStructure.GPIO_Speed	=	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode 	= 	GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd 	= 	GPIO_PuPd_UP;
	
	GPIO_InitStructure.GPIO_Pin		=	DRDY|SPI_DOUT;	
	GPIO_Init(GPIO_SPI, &GPIO_InitStructure);
	
	/* ��ʼȫ�����豸ʧ�� */
	for(i = 0; i < NumOfADS; i++)
		SPI_CS_H(i);
}
	

//*	�� �� ��: ADS1256_DelaySCLK
//*	����˵��: CLK֮����ӳ٣�ʱ���ӳ�. ����STM32F407  168M��Ƶ
//*	��    ��: ��
//*	�� �� ֵ: ��
void ADS1256_DelaySCLK(void)
{
	u16 i;
	// 	ȡ 5 ʱ��ʵ��ߵ�ƽ200ns, �͵�ƽ250ns <-- ���ȶ� 
	//	ȡ 10 ���ϣ��������������� �͵�ƽ400ns �߶�400ns <--- �ȶ�
	for (i = 0; i < 12; i++);
}


//*	�� �� ��: ADS1256_Send8Bit
//*	����˵��: ��SPI���߷���8��bit���ݡ� ����CS���ơ�
//*	��    ��: _data : u8����
//*	�� �� ֵ: ��
void ADS1256_Send8Bit(u8 data)
{
	int i;

	ADS1256_DelaySCLK();		/*��ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
	
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
		SPI_SCLK_L;			/* ADS1256 ����SCK�½��ز���DIN����, ���ݱ���ά�� 50nS */
		ADS1256_DelaySCLK();		
	}
}


//*	�� �� ��: ADS1256_Recive8Bit
//*	����˵��: ��SPI���߽���8��bit���ݡ� ����CS���ơ�
//*	��    ��: ��
//*	�� �� ֵ: ��
u8 ADS1256_Recive8Bit(void)
{
	int i;
	u8 read = 0;

	ADS1256_DelaySCLK();		/*��ADS1256 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
	
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


//*	�� �� ��: ADS1256_GetAdcData
//*	����˵��: ��ȡָ��ADS1256ͨ����ADC����
//*	��    ��: device���豸�ţ�0-4����channel��ͨ���ţ�0-7��
//*	�� �� ֵ: ����һ��u32����
u32 ADS1256_GetAdcData(u8 device, u8 channel)
{	
	int i;
	u32 temp[3] = {0};
	u32 RawData = 0;
	
	/* ѡ��ָ����ADS1256�豸�������豸ʧ�� */
	for(i = 0; i < NumOfADS; i++)
	{
		if(i == device)
			SPI_CS_L(i);
		else
			SPI_CS_H(i);
	}

	/* �ȴ� DRDY �õͣ�����ѭ�� */
	while (!IS_DRDY_LOW);

	/* �л�ģ��ͨ�����ǲ�����룩 */
	ADS1256_Send8Bit(CmdWREG | 1);					/* ����д�Ĵ�������, �����ͼĴ�����ַ */
	ADS1256_Send8Bit(0x00);							/* ���ͼĴ������� - 1, �˴�0��ʾд1���Ĵ��� */
	ADS1256_Send8Bit((channel << 4)| 8);			/* ����״̬�Ĵ��� */		
	delay_us(1);
	
	/* ����ͬ��ADת������ */
	ADS1256_Send8Bit(CmdSYNC);
	delay_us(1);
	
	/* �������ͬ�����˳��ȴ����� */
	ADS1256_Send8Bit(CmdWAKEUP);		
	delay_us(1);
	
	/* ���Ͷ�ȡ�������� */
	ADS1256_Send8Bit(CmdRDATA);
	delay_us(8);									/* ��С�ӳ� 6.5uS, �˴�ȡ10us */

	/* �����������3���ֽڣ����ֽ���ǰ */
	RawData = ADS1256_Recive8Bit() << 16;
	RawData += ADS1256_Recive8Bit() << 8;
	RawData += ADS1256_Recive8Bit() << 0;
	
	/* ȡ23λ�����е�13λ*/
	RawData = (RawData >> 10);
	
	/* ʧ������ADS1256�豸 */
	for(i = 0; i < NumOfADS; i++)	
		SPI_CS_H(i);

	return RawData;
}

//*	�� �� ��: ADS1256_Config
//*	����˵��: �������ɸ�ADS1255/6�Ļ���״̬���������桢��������
//*	��    ��: ��
//*	�� �� ֵ: ��
void ADS1256_Config(void)
{
	u8 buf[3];
	int i = 0;
	
	//STATUS:[ID3, ID2, ID1, ID0, ORDER, ACAL, BUFEN, DRDY]
	//STATUS:[-, -, -, -, �ȷ���λ���ر���У�飬�ر����뻺����, -]
	buf[0] = (0 << 3) | (0 << 2) | (0 << 1);
	//ADCON:[0, CLK1, CLK0, SDCS1, SCDS0, PGA2, PGA1, PGA0]
	//ADCON:[-, �ر�ʱ�����, �رյ���Դ���, �Ŵ����棨1��]
	buf[1] = (0 << 5) | (0 << 3) | 0;
	//DRATE:[����������ʣ�30000SPS -- 11110000 -- 240��]
	//DRATE:[����������ʣ�15000SPS -- 11100000 -- 224��]
	//DRATE:[����������ʣ�1000SPS -- 10100001 -- 161��]
	//DRATE:[����������ʣ�100SPS -- 10000010 -- 130��]
	//DRATE:[����������ʣ�10SPS -- 00100011 -- 35��]
	buf[2] = 240;
	
	for(i = 0; i < NumOfADS; i++)
	{
		/* SPIƬѡ��i��ADS1256 */
		SPI_CS_L(i);
		
		ADS1256_Send8Bit(CmdWREG | 0);		/* ����д�Ĵ�������, �����ͼĴ�����ַ */
		ADS1256_Send8Bit(0x00);				/* ���ͼĴ������� - 1, �˴�0��ʾд1���Ĵ��� */
		
		ADS1256_Send8Bit(buf[0]);			/* ����״̬�Ĵ��� */
		
		delay_us(5);
		
		ADS1256_Send8Bit(CmdWREG | 2);		/* ����д�Ĵ�������, �����ͼĴ�����ַ */
		ADS1256_Send8Bit(0x01);				/* ���ͼĴ������� - 1, �˴�1��ʾд2���Ĵ��� */
		
		ADS1256_Send8Bit(buf[1]);			/* ����ADCON���ƼĴ��������� */
		ADS1256_Send8Bit(buf[2]);			/* ��������������� */	
		
		SPI_CS_H(i);
	}
}

