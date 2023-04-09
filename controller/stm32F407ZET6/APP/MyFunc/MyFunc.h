#ifndef _MyFunc_H
#define _MyFunc_H
#include "stm32F4xx.h"
#include "Usart.h"
#include "TIM.h"
#include "PCA9685.h"
#include "SPIAD7606.h"

#define ADC_Factor	5000.0/pow(2,15)
#define SDL 		20							// StandDataLength 标定数据长度
#define MPN    		4							// MeasurePointNumber 测量点个数
#define LSN			19							// LittleSegmentNumber 小分段个数
#define PI			3.141592653f  
#define WATCH_SIZE			10

typedef struct FINGER{
	
	char arrive;
	char enable;
	
	char protect;
	float hotBeginTime;
	float hotTime;
	
	char advanceShou;
	char advanceShen;

	float maxShouSpeed;
	float maxShenSpeed;
	
	char maxShouduty;
	char maxShenduty;
	
	int num; //手指组数
	int DIP;
	int MCP;
	int PIP;
	int TIP;
	
	int changeDIP;
	int changePIP;	
	int changeMCP;
	
	int maxDIP;
	int minDIP;
	int maxPIP;
	int minPIP;	
	int maxMCP;
	int minMCP;	
	
	float DIP_angle;
	float PIP_angle;
	float MCP_angle;
	
	float DIP_angleWatch[WATCH_SIZE];
	float PIP_angleWatch[WATCH_SIZE];
	float MCP_angleWatch[WATCH_SIZE];
	
	
	float angleAct;  //实际运动角度
	float angleExpect;  //预期运动角度
	
	float Error;  //上一个误差值
	float lastError;  //上一个误差值
	float lastlastError;  //上一个误差值

	char Changeflag;
	float ChangeErrorInit;
	

	float intError;
		
	float kpBend;  
	float kiBend;
	float kdBend;
	
	float kpOpen;	
	float kiOpen;	
	float kdOpen;	

	float dutyShen;
	float dutyShou;
	
	float inquryTime;
	float beginTime;

	float cur_move_state;
	float cur_move_time;
	float cur_last_time;
}Finger;

typedef struct MOVE_PARA{
	Finger* finger;
	
	float beginTime;
	float endTime;
	float tar;
	float speed;
	
	char heatEnable;
	
	float lastTime;
	
	float lastAngleExp;
	float lastAngleAct;
	float lastAngle;

	float KpBend;
	float KpOpen;
	float KdBend;
	float KdOpen;
	char MoveFlag;
	
	float dir;		//0为伸直，1为弯曲
	
}MOVE_PARA;

typedef struct CONTROLLER_PARA{
	short time; 
	short target_angle;
	char heat;
	float heat_time;
}CONTROLLER_PARA;

extern float 	intError;
extern float 	lastError;

extern u16 	 	SampleData[8*NumOfADC];
extern float 	Data[32];
extern float 	TipAngle;
extern int 		posDIP;
extern int 		posPIP;
extern int 		posMCP;
extern float 	SetValue;
extern float  Duty;
extern float 	dutyShou;
extern float 	dutyShen;

extern float 	intError;
extern float 	lastError;
extern char 	flagLight;
extern float	T[12];
extern float 	InitAngle;
extern float 	SetSpeed;
extern int 		state;

extern Finger fingerDA;
extern Finger fingerSHI;
extern Finger fingerZHONG;
extern Finger fingerWUMING;
extern Finger fingerXIAO;

extern Finger* da;
extern Finger* shi;
extern Finger* zhong;
extern Finger* wuming;
extern Finger* xiao;	

extern Finger* handFinger_p[5];

void 	PID_Controller(Finger* nowFinger);
void 	UsartDataRec(void);
void controller(Finger* nowFinger);
void 	PWMDriver(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void PIDchange(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void PIDinquiry(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void controlFan(void);

void woquan(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void duizhi(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void sanzhi(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void yuanzhu(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void cenie(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void neikou(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);
void qiehuan(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);

void MoveFinger(MOVE_PARA* fingerPara);

void speedControlTest(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP);

#endif
