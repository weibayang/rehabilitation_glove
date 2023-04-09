#ifndef _Usart_H
#define _Usart_H
#include "stm32f4xx.h"

#define RecBufSize 40			// 接收缓存大小

// 串口数据接收数组声明
extern int 	Tag;				
extern int 	DataLength;
extern int  Mode;				
extern int  Tail;
extern u8 	RecBuf[RecBufSize];
extern u32  BaudRate;

// 相关函数声明 
void UsartInit(u32 BaudRate);
void Send8bitData(u8 data);
void Send16bitData(u16 data, u8 mode);
void Send32bitData(u32 data, u8 mode);
void SendFloatData(float data, u8 mode);

#endif
