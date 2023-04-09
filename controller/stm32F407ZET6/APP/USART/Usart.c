#include "Usart.h"
// 声明变量
int Tag 				= 0;				//帧标记
int DataLength 			= 0;				//数据帧总长度
int Mode 				= 0;				//数据帧功能模式标志位
int Tail 				= 0;				//数据帧的帧尾，判断数据帧是否接收完成，以及判断是否有新的数据帧
u8 	RecBuf[RecBufSize] 	= {0};				//数据帧原始数据（含帧头）


// 串口初始化函数
// 输入：无
// 输出：无
void UsartInit(u32 BaudRate)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;	
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef	NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 		//PA9 复用为 USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 		//PA10复用为USART1

	GPIO_InitStruct.GPIO_Pin 	= 	GPIO_Pin_9 | GPIO_Pin_10; 		//GPIOA9 与 GPIOA10
	GPIO_InitStruct.GPIO_Mode 	= 	GPIO_Mode_AF;					//复用功能
	GPIO_InitStruct.GPIO_Speed 	= 	GPIO_Speed_100MHz; 				//速度 100MHz
	GPIO_InitStruct.GPIO_OType 	= 	GPIO_OType_PP; 					//推挽复用输出
	GPIO_InitStruct.GPIO_PuPd 	= 	GPIO_PuPd_UP; 					//上拉
	GPIO_Init(GPIOA, &GPIO_InitStruct); 							//初始化 PA9， PA10	
	
	USART_InitStruct.USART_BaudRate 	=	BaudRate;
	USART_InitStruct.USART_WordLength 	= 	USART_WordLength_8b;
	USART_InitStruct.USART_StopBits 	= 	USART_StopBits_1;
	USART_InitStruct.USART_Parity 		= 	USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode 		= 	USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);

	USART_Cmd(USART1, ENABLE);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);							//清除标志位
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					//使能串口接收中断
	
	NVIC_InitStruct.NVIC_IRQChannel 			= USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;			//抢占优先级 2  
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 	= 2;  				//子优先级 2  
	NVIC_InitStruct.NVIC_IRQChannelCmd 			= ENABLE;  			//IRQ 通道使能  
	NVIC_Init(&NVIC_InitStruct);	
}

// 串口接收中断触发函数
// 小端模式：高字节存在低地址；
// 输入：无
// 输出：无
void USART1_IRQHandler(void)
{
	u8 temp = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)			//判断是否为接收中断;若是，则进行数据处理
	{
		temp = USART_ReceiveData(USART1);							//读取当前接收中断的接收值
		if(Tag == 0)
		{
			if(temp == 0xaa)	
			{
				RecBuf[Tag] = temp;
				Tag++;					//帧头1验证成功！
			}
			else
			{
				RecBuf[Tag] = 0;
				Tag = 0;				//帧头1验证失败！
			}
		}
		else if(Tag == 1)
		{
			if(temp == 0xaa)	
			{
				RecBuf[Tag] = temp;
				Tag++;					//帧头2验证成功！
			}
			else
			{
				RecBuf[Tag] = 0;
				Tag = 0;				//帧头2验证失败！
			}			
		}
		else if(Tag == 2)
		{
			RecBuf[Tag] = temp;
			DataLength  = temp;			//读取第三个数据：数据帧（确定数据帧长度）
			Tag++;						//帧标记递增	
		}
		else
		{
			RecBuf[Tag] = temp;			//当前数据在一个帧长内时，读取数据帧（含帧头和帧长）
			Tag++;						//帧标记递增
			if(Tag >= DataLength)		//判断当前帧是否为最后一帧
			{
				Tail = RecBuf[Tag-1];  	//记录帧尾数据
				Tag = 0;				//数据帧接收完毕，重置帧标记
			}			
		}
	}
}

// 16位数据串口发送函数
// 大端模式(1)：高字节存在高地址；
// 小端模式(0)：高字节存在低地址；
// 输入：data——16位数据
// 输入：mode——模式：大端模式(1)；小端模式(0)；
// 输出：无
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

// 32位数据串口发送函数
// 大端模式(1)：高字节存在高地址；
// 小端模式(0)：高字节存在低地址；
// 输入：data——32位数据
// 输入：mode——模式：大端模式(1)；小端模式(0)；
// 输出：无
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

// 32位浮点数串口发送函数
// 大端模式发送(1)：先发低字节；
// 小端模式发送(0)：先发高字节；
// 输入：data——32位浮点数据
// 输入：mode——模式：大端模式发送(1)；小端模式发送(0)；
// 输出：无
void SendFloatData(float data, u8 mode)
{
	int i;
	union exchange
	{
		float data;
		u8 dataSeperated[4];			//数据共用体
	}SendData;
	
	SendData.data = data;				//大端格式
	
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

// 8位数据串口发送函数
// 输入：data——8位数据
// 输出：无
void Send8bitData(u8 data)
{
	USART_SendData(USART1,data);  			
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
}