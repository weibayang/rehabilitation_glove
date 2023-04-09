/******************************************************/
/***

���ƣ��������׿��Ƴ���
�汾�ţ�20211015

�޸ļ�¼��
|-----------------------------------------------------|
 20211015
|-----------------------------------------------------|
 1�����ӷ��ȿ��ƺ�����
 2���������λ������PID����������
 3����������Ʒ���
 4�����°��ӵ����Ž��ж�Ӧ�޸�
 5���������޸�Ϊ115200��
|-----------------------------------------------------|
 20211125
|-----------------------------------------------------|
 1�����ӹ�һ����
 
|-----------------------------------------------------|

|-----------------------------------------------------|
 20211210
|-----------------------------------------------------|
 1���޸���Ĵָ�ı궨���ݣ�
 2������ģʽ��
 3��������ð���ж�ģʽ��
|-----------------------------------------------------|

��ע��
1�����ڽ���ģ��ģʽ��
0Ϊֹͣ������
1Ϊ�Զ���ģʽ��
4Ϊ�궨���ƣ�
5Ϊ�������ȣ�6Ϊ�رշ��ȣ�

101Ϊ��ѯPIDģʽ��102Ϊ�޸�PIDģʽ��
201Ϊ��ѯ���м���ֵ��
11Ϊ�޸Ĵ�Ĵָ�������ޣ�10Ϊ�޸Ĵ�Ĵָ��ֱ���ޣ�
21Ϊ�޸�ʳָ�������ޣ�20Ϊ�޸�ʳָ��ֱ���ޣ�
31Ϊ�޸���ָ�������ޣ�30Ϊ�޸���ָ��ֱ���ޣ�
41Ϊ�޸�����ָ�������ޣ�40Ϊ�޸�����ָ��ֱ���ޣ�
51Ϊ�޸�СĴָ�������ޣ�50Ϊ�޸�СĴָ��ֱ���ޣ�
-----
61Ϊ������ȭģʽ��62Ϊ������ָģʽ��
63Ϊ������ָģʽ��64Ϊ����Բ��ץ�գ�
65Ϊ��������ģʽ��66Ϊ�����ڿ�ģʽ��

****/
/******************************************************/





#include "stm32f4xx.h"
#include "LED.h"
#include "delay.h"
#include "Usart.h"
#include "MyFunc.h"
#include "math.h"
#include "TIM.h"
#include "GetAngle.h"




//int ActDIPAngle[5][2] = {
//	 //DIP-min  DIP-max  DIP-size  
// 	{4339,6356},
//	{4254,5305},
//	{4770,6045},
//	{4407,5335},
//	{4745,6607}
//};
//int ActPIPAngle[5][2] = {
//	  //PIP-min  PIP-max  PIP-size  
// 	{4573,5390},
//	{4531,5867},
//	{4241,5971},
//	{4090,5423},
//	{4924,5823}
//};
//int ActMCPAngle[5][2] = {
//	 //MCP-min  MCP-max  MCP-size  
// 	{0,0},
//	{4966,5397},
//	{4949,5283},
//	{5041,5404},
//	{5016,5797},
//};
int ActDIPAngle[5][2] = {  {4497 , 6451} , {4209 , 5144} , {4667 , 5538} , {4519 , 5356} , {3956 , 5730} };
int ActPIPAngle[5][2] = {  {4828 , 5546} , {4683 , 6164} , {4236 , 6084} , {4179 , 6081} , {5086 , 6585} };
int ActMCPAngle[5][2] = {  {0 , 0} , {5179 , 6728} , {5111 , 5990} , {5211 , 6020} , {5101 , 6012} };
void KEY0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void BUZZER_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//��©���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void sendGUInumber (unsigned char val)
{
	if(val == 0) {
		return;
	}
	sendGUInumber(val/10);
	
	USART_SendData(USART1,val%10+48);  
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};	
}

void sendGUI(char num,unsigned char val){
	char temp[] = ".val=";
		
	USART_SendData(USART1,'n');  		
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};
	USART_SendData(USART1,num%10+48);  		
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};	
	if(num/10!=0){
		USART_SendData(USART1,num/10+48);  		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};	
	}
	for(int i=0;i<5;i++){
		USART_SendData(USART1,temp[i]);  		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};	
	}
	
	if (val != 0) {
		sendGUInumber(val);
	}
	else
	{
		USART_SendData(USART1, 48);  		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};	
	}
	
	for(int i=0;i<3;i++){
		USART_SendData(USART1,255);  		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};	
	}	
}

int main(void)
{ 
	int i=0;
	int loopNumPID = 0;
	int len = 0;
	int state = 0;
	
	/****	Ӳ���豸��ʼ��*****/
	delay_init(168);
	LED_Init();
	KEY0_Init();
	//BUZZER_Init();
	UsartInit(115200);
	TIM3_init(100);				//��ʱ��3�жϴ������ڣ�100us
	TIM5_init(100);			//��ʱ��3�жϴ������ڣ�5000us
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM5,DISABLE);
	I2C_INIT();
	PrescaleConfig_PCA9685(PCA9685_Addr_1, Prescale_Calculate(800));
	
	AD7606_GPIO_Init();
	//AD7606_SetRange(0);
	//AD7606_SetOverSample(6);
	
	LED_Dark();

	/*********������ʼ��*******/
	da = &fingerDA;
	shi = &fingerSHI;
	zhong = &fingerZHONG;
	wuming = &fingerWUMING;
	xiao = &fingerXIAO;
	handFinger_p[0] = da;
	handFinger_p[1] = shi;
	handFinger_p[2] = zhong;
	handFinger_p[3] = wuming;
	handFinger_p[4] = xiao;

	da->num = 0;
	shi->num = 1;
	zhong->num = 2;
	wuming->num = 3;
	xiao->num = 4;
	Data[0] = 100;
	Data[10] = 1353;
	
	da->maxDIP = ActDIPAngle[0][1];
	da->maxPIP = ActPIPAngle[0][1];
	da->maxMCP = ActMCPAngle[0][1];
	da->minDIP = ActDIPAngle[0][0];
	da->minPIP = ActPIPAngle[0][0];
	da->minMCP = ActMCPAngle[0][0];
	
	shi->maxDIP = ActDIPAngle[1][1];
	shi->maxPIP = ActPIPAngle[1][1];
	shi->maxMCP = ActMCPAngle[1][1];
	shi->minDIP = ActDIPAngle[1][0];
	shi->minPIP = ActPIPAngle[1][0];
	shi->minMCP = ActMCPAngle[1][0];
	
	zhong->maxDIP = ActDIPAngle[2][1];
	zhong->maxPIP = ActPIPAngle[2][1];
	zhong->maxMCP = ActMCPAngle[2][1];
	zhong->minDIP = ActDIPAngle[2][0];
	zhong->minPIP = ActPIPAngle[2][0];
	zhong->minMCP = ActMCPAngle[2][0];
	
	wuming->maxDIP = ActDIPAngle[3][1];
	wuming->maxPIP = ActPIPAngle[3][1];
	wuming->maxMCP = ActMCPAngle[3][1];
	wuming->minDIP = ActDIPAngle[3][0];
	wuming->minPIP = ActPIPAngle[3][0];
	wuming->minMCP = ActMCPAngle[3][0];
	
	xiao->maxDIP = ActDIPAngle[4][1];
	xiao->maxPIP = ActPIPAngle[4][1];
	xiao->maxMCP = ActMCPAngle[4][1];
	xiao->minDIP = ActDIPAngle[4][0];
	xiao->minPIP = ActPIPAngle[4][0];
	xiao->minMCP = ActMCPAngle[4][0];	


	
	
	da->angleExpect = 0;
	shi->angleExpect = 0;
	zhong->angleExpect = 0;
	wuming->angleExpect = 0;
	xiao->angleExpect = 0;


	
	da->kpBend = -0.8;
	da->kdBend = -10;
	da->kpOpen = 0.8;
	da->kdOpen = 10;

	shi->kpBend = -1.5;
	shi->kdBend = -10;
	shi->kpOpen = 1.5;
	shi->kdOpen = 10;
	
	zhong->kpBend = -1.1;
	zhong->kdBend = -10;
	zhong->kpOpen = 3;
	zhong->kdOpen = 10;


  wuming->kpBend = -0.9;
  wuming->kdBend = -10;
	wuming->kpOpen = 0.9;
	wuming->kdOpen = 10;	

	xiao->kpBend = -1.3;
	xiao->kdBend = -10;
	xiao->kpOpen = 0.8;
	xiao->kdOpen = 10;
	
	
	da->maxShouSpeed = 0.8;
	da->maxShenSpeed = 0.85;
//	shi->maxShouSpeed = 0.7;
//	shi->maxShenSpeed = 0.7;	
	
	shi->maxShouSpeed = 0.7;
	shi->maxShenSpeed = 0.7;	
	zhong->maxShouSpeed = 0.6;
	zhong->maxShenSpeed = 0.6;	
	wuming->maxShouSpeed = 0.6;
	wuming->maxShenSpeed = 0.6;	
	xiao->maxShouSpeed = 0.6;
	xiao->maxShenSpeed = 0.6;	
	
	da->cur_move_state = -1;
	shi->cur_move_state = -1;
	zhong->cur_move_state = -1;
	wuming->cur_move_state = -1;
	xiao->cur_move_state = -1;

//	shi->kpBend = -0.3;
//	shi->kpOpen = 0.3;
//	
//	zhong->kpBend = -0.2;
//	zhong->kpOpen = 0.2;
//	
//	wuming->kpBend = -0.2;
//	wuming->kpOpen = 0.2;

//	xiao->kpBend = -0.2;
//	xiao->kpOpen = 0.2;
//	

//	setPWM(PCA9685_Addr_1, 2, 0);
//	setPWM(PCA9685_Addr_1, 3, 0);
//	setPWM(PCA9685_Addr_1, 4, 0);
//	setPWM(PCA9685_Addr_1, 5, 0);
//	setPWM(PCA9685_Addr_1, 6, 0);
//	setPWM(PCA9685_Addr_1, 7, 0);
//	setPWM(PCA9685_Addr_1, 8, 0);
//	setPWM(PCA9685_Addr_1, 9, 4000);
//	setPWM(PCA9685_Addr_1, 10, 0);
//	setPWM(PCA9685_Addr_1, 11, 4000);

//	setPWM(PCA9685_Addr_1, 10, 0);
//	
//	setPWM(PCA9685_Addr_1, 0, 4000);
//	setPWM(PCA9685_Addr_1, 1, 4000);
//	setPWM(PCA9685_Addr_1, 2, 2000);
//	setPWM(PCA9685_Addr_1, 3, 2000);
//	setPWM(PCA9685_Addr_1, 4, 2000);
//	
//  setPWM(PCA9685_Addr_1, 10, 2000);	
//	setPWM(PCA9685_Addr_1, 11, 4095);
//	setPWM(PCA9685_Addr_1, 12, 4095);
//	setPWM(PCA9685_Addr_1, 13, 4095);
//	setPWM(PCA9685_Addr_1, 14, 4095);
//	setPWM(PCA9685_Addr_1, 15, 2000);

//	setPWM(PCA9685_Addr_1, 3, 4095);
//	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	while(1)
	{
		LED_Light();
//		/********** ���ڽ���ָ�� **********/
		UsartDataRec();
		/************���Ƶ����***********/
		controlFan();
		/*********** ��������ƹ��� ***********/
		SensorSignal(da,shi,zhong,wuming,xiao);
		
		

//		if(duizhi(45,150,5.0,da,shi,zhong,wuming,xiao)){
//			
//			da->enable = 0;
//			shi->enable = 0;
//		}
//		if(duizhiBack(0,0,3.0,da,shi,zhong,wuming,xiao)){
//			da->enable = 0;
//			shi->enable = 0;
//		}
		//duizhiPro(da,shi,zhong,wuming,xiao);
		duizhi(da,shi,zhong,wuming,xiao);
		woquan(da,shi,zhong,wuming,xiao);
		cenie(da,shi,zhong,wuming,xiao);
		yuanzhu(da,shi,zhong,wuming,xiao);
		sanzhi(da,shi,zhong,wuming,xiao);
		neikou(da,shi,zhong,wuming,xiao);
		qiehuan(da,shi,zhong,wuming,xiao);
		speedControlTest (da,shi,zhong,wuming,xiao);
		controller(da);
		controller(shi);
		controller(zhong);
		controller(wuming);
		controller(xiao);

  	PWMDriver(da,shi,zhong,wuming,xiao);
		
		/*********** ���ͼ�� ***********/		
		
//		delay_ms(10);
//		delay_ms(10);
//		delay_ms(10);
		/*********** ��������֡ ***********/
		if(flagUART>300&&RecBuf[3] != 101&&RecBuf[3] != 102){
			//LED_Dark();
			sendGUI(0,da->angleExpect);
			sendGUI(1,shi->angleExpect);
			sendGUI(2,zhong->angleExpect);
			sendGUI(3,wuming->angleExpect);
			sendGUI(4,xiao->angleExpect);

			sendGUI(5,da->angleAct);
			sendGUI(6,shi->angleAct);
			sendGUI(7,zhong->angleAct);
			sendGUI(8,wuming->angleAct);
			sendGUI(9,xiao->angleAct);			
			
			flagUART = 0;				
		}else if(flagUART>300&&RecBuf[3] == 102){
			loopNumPID++;
			PIDchange(da,shi,zhong,wuming,xiao);
			if(loopNumPID == 10){
				loopNumPID = 0;
				RecBuf[3] = 0;
			}
			flagUART = 0;		
		}else if(flagUART>300&&RecBuf[3] == 101){
			loopNumPID++;
			PIDinquiry(da,shi,zhong,wuming,xiao);
			if(loopNumPID == 100){
				loopNumPID = 0;
				RecBuf[3] = 0;
			}
			flagUART = 0;		
		}

//			
//		delay_ms(100);
	}	
}






