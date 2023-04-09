#ifndef _Usart_H
#define _Usart_H
#include "stm32f4xx.h"

#define RecBufSize 40			// ���ջ����С

// �������ݽ�����������
extern int 	Tag;				
extern int 	DataLength;
extern int  Mode;				
extern int  Tail;
extern u8 	RecBuf[RecBufSize];
extern u32  BaudRate;

// ��غ������� 
void UsartInit(u32 BaudRate);
void Send8bitData(u8 data);
void Send16bitData(u16 data, u8 mode);
void Send32bitData(u32 data, u8 mode);
void SendFloatData(float data, u8 mode);

#endif
