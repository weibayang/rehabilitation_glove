#include "PCA9685.h"
// 刷新周期计算(PCA9685的PWM刷新频率为4-1000HZ)
u8 Prescale_Calculate(u16 frequence)
{
	u8 prescale; 
	float prescaleval;
	prescaleval = 25000000.0/(4096*frequence*0.96);
	prescale = (u8)floor(prescaleval+0.5)-1;
	return prescale;
}

// PWM刷新周期设置
void PrescaleConfig_PCA9685(u8 SCALE_Addr, u8 prescale)
{
	I2C_Start();
	I2C_Send_Byte(SCALE_Addr);
	I2C_Wait_Ack();
	I2C_Send_Byte(PCA9685_MODE1);
	I2C_Wait_Ack();
	I2C_Send_Byte(Sleep_ON);
	I2C_Wait_Ack();
	
	I2C_Start();
	I2C_Send_Byte(SCALE_Addr);
	I2C_Wait_Ack();
	I2C_Send_Byte(PCA9685_PRE_SCALE);
	I2C_Wait_Ack();
	I2C_Send_Byte(prescale);
	I2C_Wait_Ack();
	
	I2C_Start();
	I2C_Send_Byte(SCALE_Addr);
	I2C_Wait_Ack();
	I2C_Send_Byte(PCA9685_MODE1);
	I2C_Wait_Ack();
	I2C_Send_Byte(AI_ON);
	I2C_Wait_Ack();
	I2C_Stop();
}

// 设置置高位led_on和置低位led_off(0-4095)
void setPWM(u8 SCALE_Addr, u8 n, u16 led_off)
{
	u8 led_on_h = 0;
	u8 led_on_l = 0;
	u8 led_off_h = led_off >> 8;
	u8 led_off_l = (led_off & 0xFF);
	
	I2C_Start();
	I2C_Send_Byte(SCALE_Addr);
	I2C_Wait_Ack();
	I2C_Send_Byte(LEDn_ON_L(n));
	I2C_Wait_Ack();
	I2C_Send_Byte(led_on_l);
	I2C_Wait_Ack();
	I2C_Send_Byte(led_on_h);
	I2C_Wait_Ack();
	I2C_Send_Byte(led_off_l);
	I2C_Wait_Ack();
	I2C_Send_Byte(led_off_h);
	I2C_Wait_Ack();
	I2C_Stop();
}

