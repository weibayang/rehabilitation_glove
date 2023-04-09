#include "UsartDma.h"
u8 UsartData[4];
u8 SendData[4];
u8 mode;

//����DMA��ʼ��
void Usart_Dma_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;	
	USART_InitTypeDef 	USART_InitStruct;
	DMA_InitTypeDef 	DMA_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	while(DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);				//�ȴ� DMA �����ã�DMA����������
	while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);				//�ȴ� DMA �����ã�DMA����������
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 		//PA9 ����Ϊ USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 		//PA10����ΪUSART1
	
	GPIO_InitStruct.GPIO_Pin 	= 	GPIO_Pin_9 | GPIO_Pin_10; 		//GPIOA9 �� GPIOA10
	GPIO_InitStruct.GPIO_Mode 	= 	GPIO_Mode_AF;					//���ù���
	GPIO_InitStruct.GPIO_Speed 	= 	GPIO_Speed_50MHz; 				//�ٶ� 50MHz
	GPIO_InitStruct.GPIO_OType 	= 	GPIO_OType_PP; 					//���츴�����
	GPIO_InitStruct.GPIO_PuPd 	= 	GPIO_PuPd_UP; 					//����
	GPIO_Init(GPIOA, &GPIO_InitStruct); 							//��ʼ�� PA9�� PA10	
	
	USART_InitStruct.USART_BaudRate 	=	115200;
	USART_InitStruct.USART_WordLength 	= 	USART_WordLength_8b;
	USART_InitStruct.USART_StopBits 	= 	USART_StopBits_1;
	USART_InitStruct.USART_Parity 		= 	USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode 		= 	USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);
	
	DMA_InitStruct.DMA_Channel 				= 	DMA_Channel_4;				//DMA2��������5��ͨ��ѡ��
	DMA_InitStruct.DMA_PeripheralBaseAddr 	= 	(u32)(&USART1->DR);			//�������ַ������1�Ľ��ܷ��ʹ�����
	DMA_InitStruct.DMA_Memory0BaseAddr		= 	(u32)&UsartData;			//�ڴ����ַ
	DMA_InitStruct.DMA_DIR 					= 	DMA_DIR_PeripheralToMemory;	//���ݴ��䷽������ΪԴ��
	DMA_InitStruct.DMA_BufferSize    		= 	4;							//һ�δ������������С
	DMA_InitStruct.DMA_PeripheralInc 		= 	DMA_PeripheralInc_Disable;	//�����ַ�Ƿ����
	DMA_InitStruct.DMA_MemoryInc 			= 	DMA_MemoryInc_Enable;		//�ڴ��ַ�Ƿ����
	DMA_InitStruct.DMA_PeripheralDataSize 	= 	DMA_PeripheralDataSize_Byte;//������������ݳ��ȣ��ֽڴ���(Byte8,Half-word16,Word32)	
	DMA_InitStruct.DMA_MemoryDataSize 		= 	DMA_MemoryDataSize_Byte;	//�����ڴ�����ݳ��ȣ��ֽڴ���(Byte8,Half-word16,Word32)
	DMA_InitStruct.DMA_Mode 				= 	DMA_Mode_Circular;			//�Ƿ�ѭ���ɼ�
	DMA_InitStruct.DMA_Priority 			= 	DMA_Priority_High;			//���� DMA ͨ�������ȼ�
	DMA_InitStruct.DMA_FIFOMode 			= 	DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold 		= 	DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst 			= 	DMA_MemoryBurst_Single;		//���δ���
	DMA_InitStruct.DMA_PeripheralBurst 		= 	DMA_PeripheralBurst_Single;	//����ͻ�����δ���		
	
	DMA_Init(DMA2_Stream5, &DMA_InitStruct);

	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);		//ʹ�ܴ���1��DMA����

	DMA_Cmd(DMA2_Stream5, ENABLE);


	DMA_InitStruct.DMA_Channel 				= 	DMA_Channel_4;				//DMA2��������7��ͨ��ѡ��
	DMA_InitStruct.DMA_PeripheralBaseAddr 	= 	(u32)(&USART1->DR);			//�������ַ������1�Ľ��ܷ��ʹ�����
	DMA_InitStruct.DMA_Memory0BaseAddr		= 	(u32)&SendData;				//�ڴ����ַ
	DMA_InitStruct.DMA_DIR 					= 	DMA_DIR_MemoryToPeripheral;	//���ݴ��䷽������ΪԴ��
	DMA_InitStruct.DMA_BufferSize    		= 	4;							//һ�δ������������С
	DMA_InitStruct.DMA_PeripheralInc 		= 	DMA_PeripheralInc_Disable;	//�����ַ�Ƿ����
	DMA_InitStruct.DMA_MemoryInc 			= 	DMA_MemoryInc_Enable;		//�ڴ��ַ�Ƿ����
	DMA_InitStruct.DMA_PeripheralDataSize 	= 	DMA_PeripheralDataSize_Byte;//������������ݳ��ȣ��ֽڴ���	
	DMA_InitStruct.DMA_MemoryDataSize 		= 	DMA_MemoryDataSize_Byte;	//�����ڴ�����ݳ��ȣ��ֽڴ���
	DMA_InitStruct.DMA_Mode 				= 	DMA_Mode_Circular;			//�Ƿ�ѭ������
	DMA_InitStruct.DMA_Priority 			= 	DMA_Priority_High;			//���� DMA ͨ�������ȼ�
	DMA_InitStruct.DMA_FIFOMode 			= 	DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold 		= 	DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst 			= 	DMA_MemoryBurst_Single;		//���δ���
	DMA_InitStruct.DMA_PeripheralBurst 		= 	DMA_PeripheralBurst_Single;	//����ͻ�����δ���		
	
	DMA_Init(DMA2_Stream7, &DMA_InitStruct);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);		//ʹ�ܴ���1��DMA����

	DMA_Cmd(DMA2_Stream7, DISABLE);	
}
//16λ���ݷ��ͺ���
void Send16bitData(u16 data)
{
	u8 data_seperated[] = {0, 0};
	u8 i = 0;
	
	data_seperated[0] = (data >> 8) & 0xff;
	data_seperated[1] = data & 0xff;
	
	for(i = 0;i < 2;i++)
	{
		USART_SendData(USART1,data_seperated[i]);  			
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
	}
}
//32λ���ݷ��ͺ���
void Send32bitData(u32 data)
{
	u8 data_seperated[] = {0, 0, 0, 0};
	u8 i = 0;

	data_seperated[0] = (data >> 24) & 0xff;
	data_seperated[1] = (data >> 16) & 0xff;	
	data_seperated[2] = (data >> 8) & 0xff;
	data_seperated[3] = data & 0xff;
	
	for(i = 0;i < 4;i++)
	{
		USART_SendData(USART1,data_seperated[i]);  			
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
	}
}
//32λ���������ͺ���
void SendFloatData(float data)
{
	int i;
	union exchange
	{
		float data;
		u8 dataSeperated[4];			//���ݹ�����
	}SendData;
	
	SendData.data = data;
	
	for(i = 3;i >= 0;i--)				//��ָ�������ע������ʽ
	{
		USART_SendData(USART1, SendData.dataSeperated[i]);  			
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
	}	
}
//// ���ڽ�������ת��
//void ReceiveDATAProcessing(void)
//{
//	u8 i;
//	u8 j = 0;
//	int angle[12];
//	if(UsartData[0] == 0xaa)
//	{
//		if(UsartData[1] == 0xaa)
//		{
//			if(UsartData[51] == 24)
//			{
//				mode = UsartData[2];
//				for(i = 2; i < 14; i++)
//				{
//					angle[j] = (UsartData[2*i-1] << 8) + UsartData[2*i];
//					j++;
//				}
//				for(i = 0; i < 6; i++)
//				{
//					for(j = 0; j < 3; j++)
//					{
//						if(angle[2*i] == 0xeeee && angle[2*i+1] == 0xeeee)
//							SDPM[i][j] = 0xeeee;
//						else
//							SDPM[i][j] = 14 * PI * angle[2*i] * cos(angle[2*i+1]*PI/180.0 - 2*j*PI/180.0)/180.0;
//					}
//				}
//			}
//			UsartData[0] = 0;
//			UsartData[1] = 0;
//		}
//	}
//}
