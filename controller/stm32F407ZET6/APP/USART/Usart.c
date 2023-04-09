#include "Usart.h"
// ��������
int Tag 				= 0;				//֡���
int DataLength 			= 0;				//����֡�ܳ���
int Mode 				= 0;				//����֡����ģʽ��־λ
int Tail 				= 0;				//����֡��֡β���ж�����֡�Ƿ������ɣ��Լ��ж��Ƿ����µ�����֡
u8 	RecBuf[RecBufSize] 	= {0};				//����֡ԭʼ���ݣ���֡ͷ��


// ���ڳ�ʼ������
// ���룺��
// �������
void UsartInit(u32 BaudRate)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;	
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef	NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 		//PA9 ����Ϊ USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 		//PA10����ΪUSART1

	GPIO_InitStruct.GPIO_Pin 	= 	GPIO_Pin_9 | GPIO_Pin_10; 		//GPIOA9 �� GPIOA10
	GPIO_InitStruct.GPIO_Mode 	= 	GPIO_Mode_AF;					//���ù���
	GPIO_InitStruct.GPIO_Speed 	= 	GPIO_Speed_100MHz; 				//�ٶ� 100MHz
	GPIO_InitStruct.GPIO_OType 	= 	GPIO_OType_PP; 					//���츴�����
	GPIO_InitStruct.GPIO_PuPd 	= 	GPIO_PuPd_UP; 					//����
	GPIO_Init(GPIOA, &GPIO_InitStruct); 							//��ʼ�� PA9�� PA10	
	
	USART_InitStruct.USART_BaudRate 	=	BaudRate;
	USART_InitStruct.USART_WordLength 	= 	USART_WordLength_8b;
	USART_InitStruct.USART_StopBits 	= 	USART_StopBits_1;
	USART_InitStruct.USART_Parity 		= 	USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode 		= 	USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);							//�����־λ
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					//ʹ�ܴ��ڽ����ж�
	
	NVIC_InitStruct.NVIC_IRQChannel 			= USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;			//��ռ���ȼ� 2  
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 	= 2;  				//�����ȼ� 2  
	NVIC_InitStruct.NVIC_IRQChannelCmd 			= ENABLE;  			//IRQ ͨ��ʹ��  
	NVIC_Init(&NVIC_InitStruct);	
}

// ���ڽ����жϴ�������
// С��ģʽ�����ֽڴ��ڵ͵�ַ��
// ���룺��
// �������
void USART1_IRQHandler(void)
{
	u8 temp = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)			//�ж��Ƿ�Ϊ�����ж�;���ǣ���������ݴ���
	{
		temp = USART_ReceiveData(USART1);							//��ȡ��ǰ�����жϵĽ���ֵ
		if(Tag == 0)
		{
			if(temp == 0xaa)	
			{
				RecBuf[Tag] = temp;
				Tag++;					//֡ͷ1��֤�ɹ���
			}
			else
			{
				RecBuf[Tag] = 0;
				Tag = 0;				//֡ͷ1��֤ʧ�ܣ�
			}
		}
		else if(Tag == 1)
		{
			if(temp == 0xaa)	
			{
				RecBuf[Tag] = temp;
				Tag++;					//֡ͷ2��֤�ɹ���
			}
			else
			{
				RecBuf[Tag] = 0;
				Tag = 0;				//֡ͷ2��֤ʧ�ܣ�
			}			
		}
		else if(Tag == 2)
		{
			RecBuf[Tag] = temp;
			DataLength  = temp;			//��ȡ���������ݣ�����֡��ȷ������֡���ȣ�
			Tag++;						//֡��ǵ���	
		}
		else
		{
			RecBuf[Tag] = temp;			//��ǰ������һ��֡����ʱ����ȡ����֡����֡ͷ��֡����
			Tag++;						//֡��ǵ���
			if(Tag >= DataLength)		//�жϵ�ǰ֡�Ƿ�Ϊ���һ֡
			{
				Tail = RecBuf[Tag-1];  	//��¼֡β����
				Tag = 0;				//����֡������ϣ�����֡���
			}			
		}
	}
}

// 16λ���ݴ��ڷ��ͺ���
// ���ģʽ(1)�����ֽڴ��ڸߵ�ַ��
// С��ģʽ(0)�����ֽڴ��ڵ͵�ַ��
// ���룺data����16λ����
// ���룺mode����ģʽ�����ģʽ(1)��С��ģʽ(0)��
// �������
void Send16bitData(u16 data, u8 mode)
{
	u8 data_seperated[] = {0, 0};
	u8 i = 0;
	
	if(mode == 1)
	{
		data_seperated[0] = data & 0xff;
		data_seperated[1] = (data >> 8) & 0xff;
	}
	else
	{
		data_seperated[0] = (data >> 8) & 0xff;
		data_seperated[1] = data & 0xff;
	}
	
	for(i = 0;i < 2;i++)
	{
		USART_SendData(USART1,data_seperated[i]);  			
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
	}
}

// 32λ���ݴ��ڷ��ͺ���
// ���ģʽ(1)�����ֽڴ��ڸߵ�ַ��
// С��ģʽ(0)�����ֽڴ��ڵ͵�ַ��
// ���룺data����32λ����
// ���룺mode����ģʽ�����ģʽ(1)��С��ģʽ(0)��
// �������
void Send32bitData(u32 data, u8 mode)
{
	u8 data_seperated[] = {0, 0, 0, 0};
	u8 i = 0;
	
	if(mode == 1)
	{
		data_seperated[0] = data & 0xff;
		data_seperated[1] = (data >> 8) & 0xff;	
		data_seperated[2] = (data >> 16) & 0xff;
		data_seperated[3] = (data >> 24) & 0xff;
	}
	else
	{
		data_seperated[0] = (data >> 24) & 0xff;
		data_seperated[1] = (data >> 16) & 0xff;	
		data_seperated[2] = (data >> 8) & 0xff;
		data_seperated[3] = data & 0xff;
	}

	for(i = 0;i < 4;i++)
	{
		USART_SendData(USART1,data_seperated[i]);  			
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
	}
}

// 32λ���������ڷ��ͺ���
// ���ģʽ����(1)���ȷ����ֽڣ�
// С��ģʽ����(0)���ȷ����ֽڣ�
// ���룺data����32λ��������
// ���룺mode����ģʽ�����ģʽ����(1)��С��ģʽ����(0)��
// �������
void SendFloatData(float data, u8 mode)
{
	int i;
	union exchange
	{
		float data;
		u8 dataSeperated[4];			//���ݹ�����
	}SendData;
	
	SendData.data = data;				//��˸�ʽ
	
	if(mode == 1)
	{
		for(i = 0;i < 4;i++)				
		{
			USART_SendData(USART1, SendData.dataSeperated[i]);  			
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
		}
	}
	else
	{
		for(i = 3;i >= 0;i--)				
		{
			USART_SendData(USART1, SendData.dataSeperated[i]);  			
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
		}
	}	
}

// 8λ���ݴ��ڷ��ͺ���
// ���룺data����8λ����
// �������
void Send8bitData(u8 data)
{
	USART_SendData(USART1,data);  			
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
}