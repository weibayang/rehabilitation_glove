#ifndef _GetAngle_H
#define _GetAngle_H

#include "stm32F4xx.h"
#include "Usart.h"
#include "TIM.h"
#include "PCA9685.h"
#include "SPIAD7606.h"
#include "MyFunc.h"


void SensorSignal(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void Data2Angle(Finger* NowFinger);

float Filter(float* nums,float newNum);


#endif
