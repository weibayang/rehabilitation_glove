#ifndef _UsartDma_H
#define _UsartDma_H
#include "stm32f4xx.h"
//#include "TLC1543.h"
#include "math.h"

// 串口数据接收数组声明
extern u8 UsartData[4];
extern u8 SendData[4];
extern u8 mode;


/****** 相关函数声明 ******/
void Usart_Dma_Init(void);
void Send16bitData(u16 data);
void Send32bitData(u32 data);
void SendFloatData(float data);

#endif
