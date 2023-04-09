#include "UsartDma.h"
u8 UsartData[4];
u8 SendData[4];
u8 mode;

//串口DMA初始化
void Usart_Dma_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;	
	USART_InitTypeDef 	USART_InitStruct;
	DMA_InitTypeDef 	DMA_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	while(DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);				//等待 DMA 可配置：DMA接收数据流
	while(DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);				//等待 DMA 可配置：DMA发送数据流
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 		//PA9 复用为 USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 		//PA10复用为USART1
	
	GPIO_InitStruct.GPIO_Pin 	= 	GPIO_Pin_9 | GPIO_Pin_10; 		//GPIOA9 与 GPIOA10
	GPIO_InitStruct.GPIO_Mode 	= 	GPIO_Mode_AF;					//复用功能
	GPIO_InitStruct.GPIO_Speed 	= 	GPIO_Speed_50MHz; 				//速度 50MHz
	GPIO_InitStruct.GPIO_OType 	= 	GPIO_OType_PP; 					//推挽复用输出
	GPIO_InitStruct.GPIO_PuPd 	= 	GPIO_PuPd_UP; 					//上拉
	GPIO_Init(GPIOA, &GPIO_InitStruct); 							//初始化 PA9， PA10	
	
	USART_InitStruct.USART_BaudRate 	=	115200;
	USART_InitStruct.USART_WordLength 	= 	USART_WordLength_8b;
	USART_InitStruct.USART_StopBits 	= 	USART_StopBits_1;
	USART_InitStruct.USART_Parity 		= 	USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode 		= 	USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);
	
	DMA_InitStruct.DMA_Channel 				= 	DMA_Channel_4;				//DMA2的数据流5的通道选择
	DMA_InitStruct.DMA_PeripheralBaseAddr 	= 	(u32)(&USART1->DR);			//外设基地址：串口1的接受发送储存器
	DMA_InitStruct.DMA_Memory0BaseAddr		= 	(u32)&UsartData;			//内存基地址
	DMA_InitStruct.DMA_DIR 					= 	DMA_DIR_PeripheralToMemory;	//数据传输方向：外设为源地
	DMA_InitStruct.DMA_BufferSize    		= 	4;							//一次传输的数据量大小
	DMA_InitStruct.DMA_PeripheralInc 		= 	DMA_PeripheralInc_Disable;	//外设地址是否递增
	DMA_InitStruct.DMA_MemoryInc 			= 	DMA_MemoryInc_Enable;		//内存地址是否递增
	DMA_InitStruct.DMA_PeripheralDataSize 	= 	DMA_PeripheralDataSize_Byte;//设置外设的数据长度：字节传输(Byte8,Half-word16,Word32)	
	DMA_InitStruct.DMA_MemoryDataSize 		= 	DMA_MemoryDataSize_Byte;	//设置内存的数据长度：字节传输(Byte8,Half-word16,Word32)
	DMA_InitStruct.DMA_Mode 				= 	DMA_Mode_Circular;			//是否循环采集
	DMA_InitStruct.DMA_Priority 			= 	DMA_Priority_High;			//设置 DMA 通道的优先级
	DMA_InitStruct.DMA_FIFOMode 			= 	DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold 		= 	DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst 			= 	DMA_MemoryBurst_Single;		//单次传输
	DMA_InitStruct.DMA_PeripheralBurst 		= 	DMA_PeripheralBurst_Single;	//外设突发单次传输		
	
	DMA_Init(DMA2_Stream5, &DMA_InitStruct);

	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);		//使能串口1的DMA接收

	DMA_Cmd(DMA2_Stream5, ENABLE);


	DMA_InitStruct.DMA_Channel 				= 	DMA_Channel_4;				//DMA2的数据流7的通道选择
	DMA_InitStruct.DMA_PeripheralBaseAddr 	= 	(u32)(&USART1->DR);			//外设基地址：串口1的接受发送储存器
	DMA_InitStruct.DMA_Memory0BaseAddr		= 	(u32)&SendData;				//内存基地址
	DMA_InitStruct.DMA_DIR 					= 	DMA_DIR_MemoryToPeripheral;	//数据传输方向：外设为源地
	DMA_InitStruct.DMA_BufferSize    		= 	4;							//一次传输的数据量大小
	DMA_InitStruct.DMA_PeripheralInc 		= 	DMA_PeripheralInc_Disable;	//外设地址是否递增
	DMA_InitStruct.DMA_MemoryInc 			= 	DMA_MemoryInc_Enable;		//内存地址是否递增
	DMA_InitStruct.DMA_PeripheralDataSize 	= 	DMA_PeripheralDataSize_Byte;//设置外设的数据长度：字节传输	
	DMA_InitStruct.DMA_MemoryDataSize 		= 	DMA_MemoryDataSize_Byte;	//设置内存的数据长度：字节传输
	DMA_InitStruct.DMA_Mode 				= 	DMA_Mode_Circular;			//是否循环发送
	DMA_InitStruct.DMA_Priority 			= 	DMA_Priority_High;			//设置 DMA 通道的优先级
	DMA_InitStruct.DMA_FIFOMode 			= 	DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold 		= 	DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst 			= 	DMA_MemoryBurst_Single;		//单次传输
	DMA_InitStruct.DMA_PeripheralBurst 		= 	DMA_PeripheralBurst_Single;	//外设突发单次传输		
	
	DMA_Init(DMA2_Stream7, &DMA_InitStruct);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);		//使能串口1的DMA发送

	DMA_Cmd(DMA2_Stream7, DISABLE);	
}
//16位数据发送函数
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
//32位数据发送函数
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
//32位浮点数发送函数
void SendFloatData(float data)
{
	int i;
	union exchange
	{
		float data;
		u8 dataSeperated[4];			//数据共用体
	}SendData;
	
	SendData.data = data;
	
	for(i = 3;i >= 0;i--)				//拆分浮点数，注意对齐格式
	{
		USART_SendData(USART1, SendData.dataSeperated[i]);  			
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
	}	
}
//// 串口接收数据转换
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
