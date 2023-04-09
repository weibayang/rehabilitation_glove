#ifndef _SPIADS1256_H
#define _SPIADS1256_H
#include "stm32f4xx.h"
#include "delay.h"
/*
	ADS1256ģ��  <-------->  STM32f407ZET6
		 5.0V    ----------  	5.0V		5V����
		 GND     ----------  	GND     	����
		 DRDY    --------->  	PB0     	ADת������׼������
      
		 DIN   	 <---------  	PB1     	SPI_MOSI
		 DOUT    --------->  	PB2     	SPI_MISO
		 SCLK    <---------		PB3    		SPIʱ��
		 
		 CS_0    <---------  	PD0    		SPI_CS0���ӻ�ѡ��
		 CS_1    <---------  	PD1     	SPI_CS1���ӻ�ѡ��
		 CS_2    <---------  	PD2			SPI_CS2���ӻ�ѡ��
		 CS_3    <---------  	PD3			SPI_CS3���ӻ�ѡ��
		 CS_4    <---------  	PD4			SPI_CS4���ӻ�ѡ��
				   .......
		 GND   	 ----------   	GND       	SPI���߹���
		 
		 PDWN  	 <---------  	PB4       	�������
		 RST   	 <---------  	***       	��λ�źţ������ǣ�
*/
 
/*
	ADS1256��������:
	1��ģ�ⲿ�ֹ���5V;
	2��SPI���ֽӿڵ�ƽ��3.3V
	3��PGA���÷�Χ�� 1��2��4��8��16��32��64��
	4���ο���ѹ2.5V (�Ƽ�ȱʡ�ģ����õģ�
	5�������ѹ��Χ��PGA = 1 ʱ, ����������5V
	6. �Զ�У׼ ����������PGA,BUFʹ�ܡ����ݲ�����ʱ����������У׼)
	7. ����Ļ��������������ú͹رգ�һ��ѡ���ã�


	�ⲿ����Ƶ�� = 7.68MHz, 
		ʱ��Ƶ�� tCLK = 1/7.68M = 0.13uS
		����������� tDATA =  1 / 30K = 0.033mS  (��30Ksps����)
	
	��SPI��ʱ���ٶ�Ҫ��: (ads1256.pdf page 6)
		��� 4��tCLK = 0.52uS
		���� 10��tDATA = 0.3mS (�� 30Ksps ����)
		
		SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns
	
	RREG, WREG, RDATA ����֮����Ҫ�ӳ� 4 * tCLK = 0.52uS;
	RDATAC, RESET, SYNC ����֮����Ҫ�ӳ� 24 * tCLK = 3.12uS;
	
	ʵ�ʲ��ԣ���3.3V�ϵ��, ��ʱ�����κ����ã�ADS125��DRDY ���߼���ʼ��������źţ�2.6us��,33.4�ͣ�Ƶ��30KHz��
*/


#define NumOfADS		2

static u16 P[16] = {GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_6,GPIO_Pin_7,GPIO_Pin_8,\
	GPIO_Pin_9,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_14,GPIO_Pin_15};

static u8 SPI_CS[5] = {GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2,GPIO_Pin_3,GPIO_Pin_4};



#define GPIO_SPI		GPIOB
#define GPIO_CS			GPIOD
#define RCC_GPIO_SPI 	RCC_AHB1Periph_GPIOB
#define RCC_GPIO_CS 	RCC_AHB1Periph_GPIOD

#define DRDY			P[0]
#define	SPI_DIN			P[1]
#define	SPI_DOUT		P[2]
#define SPI_SCLK		P[3]
#define PDWN			P[4]

#define SPI_SCLK_H 		GPIO_SetBits(GPIO_SPI, SPI_SCLK)			// SPI_SCLK�ø�
#define SPI_SCLK_L 		GPIO_ResetBits(GPIO_SPI, SPI_SCLK)			// SPI_SCLK�õ�
#define	SPI_DIN_H		GPIO_SetBits(GPIO_SPI, SPI_DIN)
#define	SPI_DIN_L		GPIO_ResetBits(GPIO_SPI, SPI_DIN)
#define PDWN_H			GPIO_SetBits(GPIO_SPI, PDWN)
#define PDWN_L			GPIO_ResetBits(GPIO_SPI, PDWN)
#define SPI_CS_H(x)		GPIO_SetBits(GPIO_CS, SPI_CS[x])
#define SPI_CS_L(x)		GPIO_ResetBits(GPIO_CS, SPI_CS[x])

#define IS_DOUT_HIGH	(GPIO_ReadInputDataBit(GPIO_SPI, SPI_DOUT) 	== 	Bit_SET)	//�͵�ƽ��Ч
#define IS_DRDY_LOW		(GPIO_ReadInputDataBit(GPIO_SPI, DRDY) 		== 	Bit_RESET)	//�͵�ƽ��Ч

#define CmdWAKEUP  	 	0x00	// Completes SYNC and Exits Standby Mode 0000  0000 (00h)
#define CmdRDATA   	 	0x01	// Read Data 0000  0001 (01h)
#define	CmdRDATAC  		0x03	// Read Data Continuously 0000   0011 (03h)
#define	CmdSDATAC  		0x0F 	// Stop Read Data Continuously 0000   1111 (0Fh)
#define	CmdRREG    		0x10	// Read from REG rrr 0001 rrrr (1xh)
#define	CmdWREG    		0x50	// Write to REG rrr 0101 rrrr (5xh)
#define	CmdSELFCAL 		0xF0	// Offset and Gain Self-Calibration 1111    0000 (F0h)
#define	CmdSELFOCAL		0xF1	// Offset Self-Calibration 1111    0001 (F1h)
#define	CmdSELFGCAL		0xF2	// Gain Self-Calibration 1111    0010 (F2h)
#define	CmdSYSOCAL 		0xF3	// System Offset Calibration 1111   0011 (F3h)
#define	CmdSYSGCAL 		0xF4	// System Gain Calibration 1111    0100 (F4h)
#define	CmdSYNC    		0xFC	// Synchronize the A/D Conversion 1111   1100 (FCh)
#define	CmdSTANDBY 		0xFD	// Begin Standby Mode 1111   1101 (FDh)
#define	CmdRESET   		0xFE	// Reset to Power-Up Values 1111   1110 (FEh)
	
void SPIADS1256_GPIO_Init(void);
void ADS1256_DelaySCLK(void);
void ADS1256_Send8Bit(u8 data);
u8 ADS1256_Recive8Bit(void);
u32 ADS1256_GetAdcData(u8 device, u8 channel);
void ADS1256_Config(void);

#endif
