#include "MyFunc.h"
#include "math.h"
#include "LED.h"



// 定义全局变量
u16 	SampleData[8*NumOfADC] 	= {0};
float 	Data[32]				= {0};
int 		posDIP					= 0;
int 		posPIP					= 0;
int 		posMCP					= 0;
float 	TipAngle				= 0;
float 	SetValue				= 0;
float   Duty 						= 0;
float 	dutyShou				= 0;
float 	dutyShen				= 0;

float 	intError 				= 0;
float 	lastError 			= 0;
char 		flagLight 			= 0;
float		T[12]						= {0};
float 	InitAngle 			= 0;
float 	SetSpeed 				= 0;
int 		state 					= 0;

Finger fingerDA;
Finger fingerSHI;
Finger fingerZHONG;
Finger fingerWUMING;
Finger fingerXIAO;

Finger* da;
Finger* shi;
Finger* zhong;
Finger* wuming;
Finger* xiao;	

Finger* handFinger_p[5];


//// PID控制器
//// 输入SetValue:	手指总弯曲角设定值；（°）
//// 输入MeaValue:	手指总弯曲角测量值；（°）
//// 输入intError:	手指弯曲角的积分误差；
//// 输入lastError:	上一时刻的角度误差；
//// 输入duty:		手指弯曲PID控制器的输出；范围：[-1, 1]
void PID_Controller(Finger* nowFinger)
{ 
	
	nowFinger->Error = nowFinger->angleAct-nowFinger->angleExpect;
	nowFinger->intError = nowFinger->intError + nowFinger->Error;
	
	nowFinger->dutyShou = nowFinger->kpBend/10 *nowFinger->Error + nowFinger->kiBend/1000*nowFinger->intError + nowFinger->kdBend/1000*(nowFinger->lastError - nowFinger->Error);
	
	nowFinger->dutyShen = nowFinger->kpOpen/10 *nowFinger->Error + nowFinger->kiOpen/1000*nowFinger->intError + nowFinger->kdOpen/1000*(nowFinger->lastError - nowFinger->Error);
	
	nowFinger->lastError = nowFinger->Error;
	
	if(nowFinger->dutyShou >= 1)
	{
		nowFinger->intError = nowFinger->intError - nowFinger->Error;
		nowFinger->dutyShou = 1;
	}
	else if(nowFinger->dutyShou <= 0)
	{
		nowFinger->dutyShou = 0;
	}
	
	if(nowFinger->dutyShen >= 1)
	{
		nowFinger->intError = nowFinger->intError - nowFinger->Error;
		nowFinger->dutyShen = 1;
	}
	else if(nowFinger->dutyShen <= 0)
	{
		nowFinger->dutyShen = 0;
	}
}

// 增量型PID控制器

//void PID_Controller(Finger* nowFinger)
//{
//	
//	nowFinger->Error = nowFinger->angleAct-nowFinger->angleExpect;
//	nowFinger->intError = nowFinger->intError + nowFinger->Error;
//	
//	nowFinger->dutyShou += nowFinger->kpBend/10 *(nowFinger->Error-nowFinger->lastError) + nowFinger->kiBend/1000*nowFinger->Error + nowFinger->kdBend/1000*(nowFinger->Error - 2*nowFinger->lastError+nowFinger->lastlastError);
//	
//	nowFinger->dutyShen += nowFinger->kpOpen/10 *(nowFinger->Error-nowFinger->lastError) + nowFinger->kiOpen/1000*nowFinger->Error + nowFinger->kdOpen/1000*(nowFinger->Error - 2*nowFinger->lastError+nowFinger->lastlastError);
//	
//	
//	nowFinger->lastlastError = nowFinger->lastError;
//	nowFinger->lastError = nowFinger->Error;
//	
//	if(nowFinger->dutyShou >= 1)
//	{
//		nowFinger->intError = nowFinger->intError - nowFinger->Error;
//		nowFinger->dutyShou = 1;
//	}
//	else if(nowFinger->dutyShou <= 0)
//	{
//		nowFinger->dutyShou = 0;
//	}
//	
//	if(nowFinger->dutyShen >= 1)
//	{
//		nowFinger->intError = nowFinger->intError - nowFinger->Error;
//		nowFinger->dutyShen = 1;
//	}
//	else if(nowFinger->dutyShen <= 0)
//	{
//		nowFinger->dutyShen = 0;
//	}
//	
//}



// 串口数据帧接收判断
// 数据帧格式：[0xaa 0xaa (u8)SerialDataSize (u8)Mode (u8)-- (u8)-- (u8)-- (u8)-- (u8)Tail]
// Mode： 0   系统终止
// Mode： 1   系统启动
void UsartDataRec(void)
{
	static char state = 0;
	
	if(RecBuf[3] == 4)
	{
//		if(flagLight == 0)
//		{
//			TIME_Counter = 0;
//			Time = TIME_Counter/10000.0;
//			TIM_Cmd(TIM3,ENABLE);
//		}
		flagLight 	= 1;
	}
	else if(RecBuf[3] == 0)
	{
//		if(flagLight == 1)
//		{
//			TIM_Cmd(TIM3,DISABLE);
//			TIME_Counter = 0;
//			Time = TIME_Counter/10000.0;
//		}
		flagLight 	= 0;
		RecBuf[3] 	= 0;
		RecBuf[4] 	= 0;
	}
	if(RecBuf[3] != 0&&state == 0){
		state = 1;
		TIME_Counter = 0;
	//	TIM_Cmd(TIM3,ENABLE);
	}else if(RecBuf[3] == 0){
		state = 0;
	//	TIM_Cmd(TIM3,DISABLE);
		TIME_Counter = 0;
	}
	if(flagLight == 1){
		GPIO_SetBits(GPIOD, GPIO_Pin_5);
	}else{
		GPIO_ResetBits(GPIOD, GPIO_Pin_5);
	}
}

//控制风扇
void controlFan(){
	if(RecBuf[3] == 5){
		setPWM(PCA9685_Addr_1, 15, 4095);
		RecBuf[3] = 0;
	}else if(RecBuf[3] == 6){
		setPWM(PCA9685_Addr_1, 15, 0);
		RecBuf[3] = 0;
	}
}


//改变PID 
void PIDchange(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
	if(RecBuf[3] == 102)
	{
		daP->kpBend = -1*(float)RecBuf[4]/100;
		daP->kiBend = -1*(float)RecBuf[5]/100;
		daP->kdBend = -1*(float)RecBuf[6]/100;
		daP->kpOpen = (float)RecBuf[7]/100;
		daP->kiOpen = (float)RecBuf[8]/100;
		daP->kdOpen = (float)RecBuf[9]/100;
		
		shiP->kpBend = -1*(float)RecBuf[10]/100;
		shiP->kiBend = -1*(float)RecBuf[11]/100;
		shiP->kdBend = -1*(float)RecBuf[12]/100;
		shiP->kpOpen = (float)RecBuf[13]/100;
		shiP->kiOpen = (float)RecBuf[14]/100;
		shiP->kdOpen = (float)RecBuf[15]/100;
		
		zhongP->kpBend = -1*(float)RecBuf[16]/100;
		zhongP->kiBend = -1*(float)RecBuf[17]/100;
		zhongP->kdBend = -1*(float)RecBuf[18]/100;
		zhongP->kpOpen = (float)RecBuf[19]/100;
		zhongP->kiOpen = (float)RecBuf[20]/100;
		zhongP->kdOpen = (float)RecBuf[21]/100;		

		wumingP->kpBend = -1*(float)RecBuf[22]/100;
		wumingP->kiBend = -1*(float)RecBuf[23]/100;
		wumingP->kdBend = -1*(float)RecBuf[24]/100;
		wumingP->kpOpen = (float)RecBuf[25]/100;
		wumingP->kiOpen = (float)RecBuf[26]/100;
		wumingP->kdOpen = (float)RecBuf[27]/100;
		
		xiaoP->kpBend = -1*(float)RecBuf[28]/100;
		xiaoP->kiBend = -1*(float)RecBuf[29]/100;
		xiaoP->kdBend = -1*(float)RecBuf[30]/100;
		xiaoP->kpOpen = (float)RecBuf[31]/100;
		xiaoP->kiOpen = (float)RecBuf[32]/100;
		xiaoP->kdOpen = (float)RecBuf[33]/100;
		PIDinquiry(daP,shiP,zhongP,wumingP,xiaoP);
	}
}
//查询PID
void PIDinquiry(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
//	if(RecBuf[3] == 101){
	
		int i=0;
	
		Send16bitData(0xaaaa,1);									// 发送数据帧帧头
		Send16bitData(134,1);				//length of data
		//发送大拇指数据
		Send16bitData(-1*daP->kpBend*100,1);
		Send16bitData(-1*daP->kiBend*100,1);
		Send16bitData(-1*daP->kdBend*100,1);
		Send16bitData(daP->kpOpen*100,1);
		Send16bitData(daP->kiOpen*100,1);
		Send16bitData(daP->kdOpen*100,1);

		//发送食指数据
		Send16bitData(-1*shiP->kpBend*100,1);
		Send16bitData(-1*shiP->kiBend*100,1);
		Send16bitData(-1*shiP->kdBend*100,1);
		Send16bitData(shiP->kpOpen*100,1);
		Send16bitData(shiP->kiOpen*100,1);
		Send16bitData(shiP->kdOpen*100,1);

		//发送中指数据
		Send16bitData(-1*zhongP->kpBend*100,1);
		Send16bitData(-1*zhongP->kiBend*100,1);
		Send16bitData(-1*zhongP->kdBend*100,1);
		Send16bitData(zhongP->kpOpen*100,1);
		Send16bitData(zhongP->kiOpen*100,1);
		Send16bitData(zhongP->kdOpen*100,1);

		//发送无名指数据
		Send16bitData(-1*wumingP->kpBend*100,1);
		Send16bitData(-1*wumingP->kiBend*100,1);
		Send16bitData(-1*wumingP->kdBend*100,1);
		Send16bitData(wumingP->kpOpen*100,1);
		Send16bitData(wumingP->kiOpen*100,1);
		Send16bitData(wumingP->kdOpen*100,1);

		//发送小拇指数据
		Send16bitData(-1*xiaoP->kpBend*100,1);
		Send16bitData(-1*xiaoP->kiBend*100,1);
		Send16bitData(-1*xiaoP->kdBend*100,1);
		Send16bitData(xiaoP->kpOpen*100,1);
		Send16bitData(xiaoP->kiOpen*100,1);
		Send16bitData(xiaoP->kdOpen*100,1);
		//补齐数据包
		for(i=0;i<128-60;i++){
			Send8bitData(0);
		}
		
		//发送包尾
		USART_SendData(USART1,13);  		// end signal_CR
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};
		USART_SendData(USART1,10);  		// end signal_LF
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET){};					
//	}
}

// 控制过程
void controller(Finger* nowFinger)
{
	float IntervalTime = 16; //s
	int num = nowFinger->num;
	Time = TIME_Counter/10000.0;
	//设定好角度
	if(RecBuf[3] == 1){
		nowFinger->angleExpect = RecBuf[4+num];
	}else if(RecBuf[3] != 2){
		//nowFinger->angleExpect = 0;
		nowFinger->Changeflag = 0;
	}
	
	//修改角度
	if(RecBuf[3]/10-1 == nowFinger->num){
		if(RecBuf[3]%10 == 0){
			nowFinger->maxDIP = RecBuf[4]*100+RecBuf[5];
			nowFinger->maxPIP = RecBuf[6]*100+RecBuf[7];
			nowFinger->maxMCP = RecBuf[8]*100+RecBuf[9];
		}else if(RecBuf[3]%10 == 1){
			nowFinger->minDIP = RecBuf[4]*100+RecBuf[5];
			nowFinger->minPIP = RecBuf[6]*100+RecBuf[7];
			nowFinger->minMCP = RecBuf[8]*100+RecBuf[9];
		}
	}
	
	
	/********** PID控制手指 ***********///自己设角度
	if((RecBuf[3] == 1&&nowFinger->angleExpect!=255)||(RecBuf[3] == 61&&nowFinger->enable == 1)||(RecBuf[3] == 62&&nowFinger->enable == 1)||(RecBuf[3] == 63&&nowFinger->enable == 1)||
		(RecBuf[3] == 64&&nowFinger->enable == 1)||(RecBuf[3] == 65&&nowFinger->enable == 1)||(RecBuf[3] == 66&&nowFinger->enable == 1)||(RecBuf[3] == 67&&nowFinger->enable == 1)||(RecBuf[3] == 70&&nowFinger->enable == 1))
	{

		PID_Controller(nowFinger);  				//angleSpeed = 0;
		if(nowFinger->Changeflag == 0){
			nowFinger->Changeflag = 1;
			nowFinger->ChangeErrorInit = nowFinger->Error;
		}		
//		if(nowFinger->Changeflag == 1){
//			float multi = 30*nowFinger->Error/nowFinger->ChangeErrorInit;
//			if(multi>22){
//				nowFinger->dutyShou 	= multi*nowFinger->dutyShou;
//				nowFinger->dutyShen 	= multi*nowFinger->dutyShen;
//			}
//			if(nowFinger->dutyShou>nowFinger->maxShouSpeed) nowFinger->dutyShou = nowFinger->maxShouSpeed;
//			if(nowFinger->dutyShen>nowFinger->maxShenSpeed) nowFinger->dutyShen = nowFinger->maxShenSpeed;
//		}
//		if(nowFinger->Error<2&&nowFinger->Error>-3){
//			nowFinger->arrive = 1;
//		}
	}
	
//	if(nowFinger->protect == 1 && (nowFinger->dutyShou>0.8||nowFinger->dutyShen>0.8)){
//		if(nowFinger->hotTime == 0){
//			nowFinger->hotBeginTime = Time;
//			nowFinger->hotTime = 0.0001;
//		}else{
//			nowFinger->hotTime = Time - nowFinger->hotBeginTime; 
//		}
//		if(nowFinger->hotTime > 3){
//			nowFinger->dutyShou = 0.5*nowFinger->dutyShou;
//			nowFinger->dutyShen = 0.5*nowFinger->dutyShen;

//		}
//		
//	}else (nowFinger->protect == 0){
//		nowFinger->hotTime = 0;
//		nowFinger->hotBeginTime = 0;
//	}
	
	if(nowFinger->enable == 0){
		nowFinger->dutyShou 	= 0;
		nowFinger->dutyShen 	= 0;
	}
	if(nowFinger->enable == 1&&nowFinger->maxShouduty == 1){
		nowFinger->dutyShou 	= 1; 
		nowFinger->dutyShen 	= 0;
	}else if(nowFinger->enable == 1&&nowFinger->maxShenduty == 1){
		nowFinger->dutyShou 	= 0;
		nowFinger->dutyShen 	= 1;
	}
	
	if(nowFinger->enable == 1&&nowFinger->advanceShou == 1){
		nowFinger->dutyShou 	= 0.9; 
		nowFinger->dutyShen 	= 0;
	}else if(nowFinger->enable == 1&&nowFinger->advanceShen == 1){
		nowFinger->dutyShou 	= 0;
		nowFinger->dutyShen 	= 0.9;
	}
	
	
//	/********** 过热保护 ***********///
//	if(Time-nowFinger->inquryTime>0.5f){
//			nowFinger->inquryTime = Time;
//		
//			if(fabs(nowFinger->Error - nowFinger->lastError)>0.5f){
//				nowFinger->beginTime = Time;
//			}
//			
//			if(Time>3&&Time - nowFinger->beginTime>1&&(nowFinger->dutyShou>0.6f||nowFinger->dutyShen>0.6f)){
//				GPIO_SetBits(GPIOB, GPIO_Pin_8);
//			}	
//	}

	/********** 弯曲结束 ***********///
	if(RecBuf[3] == 0 && RecBuf[4] == 0)
	{
		nowFinger->inquryTime = 0;
		nowFinger->beginTime = 0;
		nowFinger->dutyShou 	= 0;
		nowFinger->dutyShen 	= 0;
		nowFinger->arrive = 0;
		nowFinger->enable = 1;
		
		nowFinger->cur_last_time = 0;
		nowFinger->cur_move_state = -1;
		nowFinger->cur_move_time = 0;
	}			
}

void MoveFinger(MOVE_PARA* fingerPara){
	
	const float recordTime = 0.2;
	float timeTemp = Time - fingerPara->beginTime;
	
	if(fingerPara->tar == -1) return;

	if(timeTemp<recordTime){
		
//		zhongP->enable = 0;
//		wumingP->enable = 0;
//		xiaoP->enable = 0;

		fingerPara->MoveFlag = 0;
		fingerPara->lastTime = timeTemp;
		
		fingerPara->KpBend = fingerPara->finger->kpBend;
		fingerPara->KpOpen = fingerPara->finger->kpOpen;
		fingerPara->KdBend = fingerPara->finger->kdBend;
		fingerPara->KdOpen = fingerPara->finger->kdOpen;

		fingerPara->lastAngleAct = fingerPara->finger->angleAct;
		fingerPara->lastAngleExp = fingerPara->finger->angleExpect;
		
		if(fingerPara->finger->angleAct>fingerPara->tar)				{
			fingerPara->lastAngle = fingerPara->finger->angleExpect;
			fingerPara->dir = 0;
		}
		else 																								   	{
			fingerPara->lastAngle = fingerPara->finger->angleExpect;
			fingerPara->dir = 1;		
		}
		
	}else if(fabs(fingerPara->finger->angleExpect - fingerPara->tar)>0.2f){
		fingerPara->finger->angleExpect = fingerPara->lastAngle+(fingerPara->tar - fingerPara->lastAngle)/fingerPara->speed*(timeTemp-fingerPara->lastTime);
	}
	
	//指调节
	
	if(timeTemp>recordTime && fingerPara->MoveFlag == 0 && fingerPara->tar!=0  && fingerPara->heatEnable == 1){
		fingerPara->finger->enable = 1;	//开启控制器
		if(fingerPara->dir == 0){
			fingerPara->finger->maxShouduty = 0;
			fingerPara->finger->maxShenduty = 1;			
		}else{
			fingerPara->finger->maxShouduty = 1;
			fingerPara->finger->maxShenduty = 0;			
		}
		if(fabs(fingerPara->finger->angleAct - fingerPara->lastAngleAct)>3&&fabs(fingerPara->finger->Error)<10){
			fingerPara->MoveFlag = 1;
		}
	}else if(timeTemp>recordTime){
		fingerPara->finger->enable = 1;	//开启控制器
		fingerPara->finger->maxShouduty = 0;
		fingerPara->finger->maxShenduty = 0;			

		fingerPara->finger->kpBend = fingerPara->KpBend;
		fingerPara->finger->kpOpen = fingerPara->KpOpen;
	}	
}

void duizhi(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
	char speed = 17;
	float timeArr[] = {5,30,55};

	static MOVE_PARA daMove;
	static MOVE_PARA shiMove;
	static MOVE_PARA zhongMove;
	static MOVE_PARA wumingMove;
	static MOVE_PARA xiaoMove;
	if(RecBuf[3] == 62){
		daMove.finger = daP;
		shiMove.finger = shiP;
		zhongMove.finger = zhongP;
		wumingMove.finger = wumingP;
		xiaoMove.finger = xiaoP;

		
		//按顺序控制
		if(Time<timeArr[0]){
			daMove.beginTime = 0;
			daMove.speed = 1;
			daMove.tar = 0;
			daMove.heatEnable = 0;
			
			
			shiMove.beginTime = 0;
			shiMove.speed = 1;
			shiMove.tar = 0;
			shiMove.heatEnable = 0;
		}
		else if(Time<timeArr[1]){
			daMove.beginTime = timeArr[0];
			daMove.speed = speed;
			daMove.tar = 60;	
			daMove.heatEnable = 1;
			
			shiMove.beginTime = timeArr[0];
			shiMove.speed = speed;
			shiMove.tar = 90;
			shiMove.heatEnable = 1;
		
//			zhong.beginTime = 0;
//			zhong.speed = speed;
//			zhong.tar = 155;	
			
//			wuming.beginTime = 0;
//			wuming.speed = speed;
//			wuming.tar = 170;

//			xiao.beginTime = 0;
//			xiao.speed = speed;
//			xiao.tar = 170;		

			if(Time<timeArr[1]&&Time>(timeArr[1]-2.0)){
				daMove.finger->advanceShen = 1;
				shiMove.finger->advanceShen = 1;
			}
			
		}else if(Time<timeArr[2]){
				daMove.finger->advanceShen = 0;
				shiMove.finger->advanceShen = 0;
			
			daMove.beginTime = timeArr[1];
			daMove.speed = speed;
			daMove.tar = 0;	
//			daMove.endTime = da.speed+da.beginTime;
			daMove.heatEnable = 1;
			
			shiMove.beginTime = timeArr[1];
			shiMove.speed = speed;
			shiMove.tar = 0;
//			shi.endTime = shi.speed+shi.beginTime;
			shiMove.heatEnable = 1;

//		zhong.beginTime = 20;
//		zhong.speed = speed;
//		zhong.tar = 15;	
//		zhong.endTime = zhong.speed+zhong.beginTime;

//		wuming.beginTime = 20;
//		wuming.speed = speed;
//		wuming.tar = 15;

//		xiao.beginTime = 20;
//		xiao.speed = speed;
//		xiao.tar = 15;		
		}
		
	//放入控制器

		daP->enable = 0;
		shiP->enable = 0;
		zhongP->enable = 0;
		wumingP->enable = 0;
		xiaoP->enable = 0;
		
		MoveFinger(&daMove);
		MoveFinger(&shiMove);
//		MoveFinger(&zhong);
//		MoveFinger(&wuming);
//		MoveFinger(&xiao);
	}
}



void cenie(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
	char speed = 20;
	float timeArr[] = {5,35,60};

	static MOVE_PARA daMove;
	static MOVE_PARA shiMove;
	static MOVE_PARA zhongMove;
	static MOVE_PARA wumingMove;
	static MOVE_PARA xiaoMove;
	if(RecBuf[3] == 65){
		daMove.finger = daP;
		shiMove.finger = shiP;
		zhongMove.finger = zhongP;
		wumingMove.finger = wumingP;
		xiaoMove.finger = xiaoP;

		
		//按顺序控制
		if(Time<timeArr[0]){
			
			if(Time<timeArr[0]&&Time>(timeArr[0]-0.5)){
				daMove.finger->advanceShou = 1;
				shiMove.finger->advanceShou = 1;
				zhongMove.finger->advanceShou = 1;
				wumingMove.finger->advanceShou = 1;
				xiaoMove.finger->advanceShou = 1;
			}
//			
			daMove.beginTime = 0;
			daMove.speed = 2;
			daMove.tar = 0;	
			daMove.heatEnable = 0;
			
			shiMove.beginTime = 0;
			shiMove.speed = speed;
			shiMove.tar = 0;
			shiMove.heatEnable = 0;
		
			zhongMove.beginTime = 0;
			zhongMove.speed = speed;
			zhongMove.tar = 0;	
			shiMove.heatEnable = 0;

			wumingMove.beginTime = 0;
			wumingMove.speed = speed;
			wumingMove.tar = 0;
			shiMove.heatEnable = 0;

			xiaoMove.beginTime = 0;
			xiaoMove.speed = speed;
			xiaoMove.tar = 0;		
			shiMove.heatEnable = 0;
			
		}
		else if(Time<timeArr[1]){
			
				daMove.finger->advanceShou = 0;
				shiMove.finger->advanceShou = 0;
				zhongMove.finger->advanceShou = 0;
				wumingMove.finger->advanceShou = 0;
				xiaoMove.finger->advanceShou = 0;
			
			daMove.beginTime = timeArr[0];
			daMove.speed = speed;
			daMove.tar = 50;	
			daMove.heatEnable = 0;
			daMove.finger->kpBend = -1.4;
			daMove.finger->kdBend = -10;
			daMove.finger->kpOpen = 1.4;
			daMove.finger->kdOpen = 10;			
			
			
			shiMove.beginTime = timeArr[0];
			shiMove.speed = speed;
			shiMove.tar = -1;//90;
			shiMove.heatEnable = 1;
			shiMove.finger->kpBend = -1.5;
			shiMove.finger->kdBend = -10;
			shiMove.finger->kpOpen = 0.8;
			shiMove.finger->kdOpen = 10;
			

			zhongMove.beginTime = timeArr[0];
			zhongMove.speed = speed;
			zhongMove.tar = -1;//90;	
			zhongMove.heatEnable = 1;
			zhongMove.finger->kpBend = -1.1;
			zhongMove.finger->kdBend = -10;
			zhongMove.finger->kpOpen = 3;
			zhongMove.finger->kdOpen = 10;
			
			
			wumingMove.beginTime = timeArr[0];
			wumingMove.speed = speed;
			wumingMove.tar = -1;//80;
			wumingMove.heatEnable = 0;
			wumingMove.finger->kpBend = -1.1;
			wumingMove.finger->kdBend = -10;
			wumingMove.finger->kpOpen = 1.1;
			wumingMove.finger->kdOpen = 10;
			

			xiaoMove.beginTime = timeArr[0];
			xiaoMove.speed = speed;
			xiaoMove.tar = -1;//90;		
			xiaoMove.heatEnable = 1;
			xiaoMove.finger->kpBend = -0.8;
			xiaoMove.finger->kdBend = -20;
			xiaoMove.finger->kpOpen = 0.8;
			xiaoMove.finger->kdOpen = 10;			
			
			
			
			if(Time<timeArr[1]&&Time>(timeArr[1]-1.5)){
				daMove.finger->advanceShen = 1;
				shiMove.finger->advanceShen = 1;
				zhongMove.finger->advanceShen = 1;
				wumingMove.finger->advanceShen = 1;
				xiaoMove.finger->advanceShen = 1;
			}

		}else if(Time<timeArr[2]){
			
			daMove.finger->advanceShen = 0;
			shiMove.finger->advanceShen = 0;
			zhongMove.finger->advanceShen = 0;
			wumingMove.finger->advanceShen = 0;
			xiaoMove.finger->advanceShen = 0;
			
			daMove.beginTime = timeArr[1];
			daMove.speed = speed;
			daMove.tar =0;	
			daMove.heatEnable = 0;
			daMove.finger->kpOpen = 1.1;
			daMove.finger->kdOpen = 10;
			
			shiMove.beginTime = timeArr[1];
			shiMove.speed = speed;
			shiMove.tar = -1;//0;
			shiMove.heatEnable = 1;
			shiMove.finger->kpOpen = 1.5;
			shiMove.finger->kdOpen = 10;


			zhongMove.beginTime = timeArr[1];
			zhongMove.speed = speed;
			zhongMove.tar = -1;//0;	
			zhongMove.heatEnable = 1;
			zhongMove.finger->kpOpen = 1.5;
			zhongMove.finger->kdOpen = 10;
			

			wumingMove.beginTime = timeArr[1];
			wumingMove.speed = speed;
			wumingMove.tar =-1;//0;
			wumingMove.heatEnable = 1;
			wumingMove.finger->kpOpen = 1.1;
			wumingMove.finger->kdOpen = 10;			

			xiaoMove.beginTime = timeArr[1];
			xiaoMove.speed = speed;
			xiaoMove.tar = -1;//0;		
			xiaoMove.heatEnable = 1;
			xiaoMove.finger->kpOpen = 1.3;
			xiaoMove.finger->kdOpen = 10;
			
		}
		
	//放入控制器

		daP->enable = 0;
		shiP->enable = 0;
		zhongP->enable = 0;
		wumingP->enable = 0;
		xiaoP->enable = 0;
		
		MoveFinger(&daMove);
		MoveFinger(&shiMove);
		MoveFinger(&zhongMove);
		MoveFinger(&wumingMove);
		MoveFinger(&xiaoMove);
	}
	
}



void sanzhi(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
	char speed = 5;
	float timeArr[] = {5,25,45};

	static MOVE_PARA daMove;
	static MOVE_PARA shiMove;
	static MOVE_PARA zhongMove;
	static MOVE_PARA wumingMove;
	static MOVE_PARA xiaoMove;
	if(RecBuf[3] == 63){
		daMove.finger = daP;
		shiMove.finger = shiP;
		zhongMove.finger = zhongP;
		wumingMove.finger = wumingP;
		xiaoMove.finger = xiaoP;

		
		//按顺序控制
		if(Time<timeArr[0]){
			daMove.beginTime = 0;
			daMove.speed = 1;
			daMove.tar = 5;
			daMove.heatEnable = 0;
			
			
			shiMove.beginTime = 0;
			shiMove.speed = 1;
			shiMove.tar = 15;
			shiMove.heatEnable = 0;
			
			zhongMove.beginTime = 0;
			zhongMove.speed = 1;
			zhongMove.tar = 15;	
			zhongMove.heatEnable = 0;
		}
		else if(Time<timeArr[1]){
			daMove.beginTime = timeArr[0];
			daMove.speed = speed;
			daMove.tar = 70;	
			daMove.heatEnable = 1;
			
			shiMove.beginTime = timeArr[0];
			shiMove.speed = speed;
			shiMove.tar = 90;
			shiMove.heatEnable = 1;
		
			zhongMove.beginTime = timeArr[0];
			zhongMove.speed = speed;
			zhongMove.tar = 105;	
			zhongMove.heatEnable = 1;
//			wuming.beginTime = 0;
//			wuming.speed = speed;
//			wuming.tar = 170;

//			xiao.beginTime = 0;
//			xiao.speed = speed;
//			xiao.tar = 170;		
		}else if(Time<timeArr[2]){
			daMove.beginTime = timeArr[1];
			daMove.speed = speed;
			daMove.tar = 5;	
//			daMove.endTime = da.speed+da.beginTime;
			daMove.heatEnable = 1;
			
			shiMove.beginTime = timeArr[1];
			shiMove.speed = speed;
			shiMove.tar = 15;
//			shi.endTime = shi.speed+shi.beginTime;
			shiMove.heatEnable = 1;

			zhongMove.beginTime = timeArr[1];
			zhongMove.speed = speed;
			zhongMove.tar = 15;
//			zhongMove.endTime = shi.speed+shi.beginTime;
			zhongMove.heatEnable = 1;

//		wuming.beginTime = 20;
//		wuming.speed = speed;
//		wuming.tar = 15;

//		xiao.beginTime = 20;
//		xiao.speed = speed;
//		xiao.tar = 15;		
		}
		
	//放入控制器

		daP->enable = 0;
		shiP->enable = 0;
		zhongP->enable = 0;
		wumingP->enable = 0;
		xiaoP->enable = 0;
		
		MoveFinger(&daMove);
		MoveFinger(&shiMove);
		MoveFinger(&zhongMove);
//		MoveFinger(&wuming);
//		MoveFinger(&xiao);
	}
}




void woquan(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
	char speed = 5;
	
	static MOVE_PARA da;
	static MOVE_PARA shi;
	static MOVE_PARA zhong;
	static MOVE_PARA wuming;
	static MOVE_PARA xiao;
	if(RecBuf[3] == 61){
		da.finger = daP;
		shi.finger = shiP;
		zhong.finger = zhongP;
		wuming.finger = wumingP;
		xiao.finger = xiaoP;

		
		//按顺序控制
		if(Time<20){
			da.beginTime = 4.5;
			da.speed = speed;
			da.tar = 50;	
			
			
			shi.beginTime = 0;
			shi.speed = speed;
			shi.tar = 170;
			
		
			zhong.beginTime = 0;
			zhong.speed = speed;
			zhong.tar = 170;	
			
			wuming.beginTime = 0;
			wuming.speed = speed;
			wuming.tar = 170;

			xiao.beginTime = 0;
			xiao.speed = speed;
			xiao.tar = 170;		
		}else if(Time<40){
			da.beginTime = 20;
			da.speed = speed;
			da.tar = 5;	
			da.endTime = da.speed+da.beginTime;
			
			shi.beginTime = 24.5;
			shi.speed = speed;
			shi.tar = 15;
			shi.endTime = shi.speed+shi.beginTime;

			zhong.beginTime = 24.5;
			zhong.speed = speed;
			zhong.tar = 15;	
			zhong.endTime = zhong.speed+zhong.beginTime;

			wuming.beginTime = 24.5;
			wuming.speed = speed;
			wuming.tar = 15;

			xiao.beginTime = 24.5;
			xiao.speed = speed;
			xiao.tar = 15;		
		}
		
	//放入控制器

		daP->enable = 0;
		shiP->enable = 0;
		zhongP->enable = 0;
		wumingP->enable = 0;
		xiaoP->enable = 0;
		
		MoveFinger(&da);
		MoveFinger(&shi);
		MoveFinger(&zhong);
		MoveFinger(&wuming);
		MoveFinger(&xiao);
	}
}


void yuanzhu(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
	char speed = 15;
	float timeArr[] = {5,35,55};

	static MOVE_PARA daMove;
	static MOVE_PARA shiMove;
	static MOVE_PARA zhongMove;
	static MOVE_PARA wumingMove;
	static MOVE_PARA xiaoMove;
	
	
	if(RecBuf[3] == 64){
		daMove.finger = daP;
		shiMove.finger = shiP;
		zhongMove.finger = zhongP;
		wumingMove.finger = wumingP;
		xiaoMove.finger = xiaoP;

//		daMove.finger->protect = 1;
//		shiMove.finger->protect = 1;
//		zhongMove.finger->protect = 1;
//		wumingMove.finger->protect = 1;
//		xiaoMove.finger->protect = 1;

		
		//按顺序控制
		if(Time<timeArr[0]){
			daMove.beginTime = 0;
			daMove.speed = speed;
			daMove.tar = -1;	
			daMove.heatEnable = 0;
			
			shiMove.beginTime = 0;
			shiMove.speed = speed;
			shiMove.tar = 0;
			shiMove.heatEnable = 0;
		
			zhongMove.beginTime = 0;
			zhongMove.speed = speed;
			zhongMove.tar = 0;	
			shiMove.heatEnable = 0;

			wumingMove.beginTime = 0;
			wumingMove.speed = speed;
			wumingMove.tar = 0;
			shiMove.heatEnable = 0;

			xiaoMove.beginTime = 0;
			xiaoMove.speed = speed;
			xiaoMove.tar = 0;		
			shiMove.heatEnable = 0;
			
		}
		else if(Time<timeArr[1]){
			daMove.beginTime = timeArr[0];
			daMove.speed = speed-3;
			daMove.tar = 80;	
			daMove.heatEnable = 0;
			
			
			shiMove.beginTime = timeArr[0]+3;
			shiMove.speed = speed+3;
			shiMove.tar = 60;
			shiMove.heatEnable = 1;
		
			zhongMove.beginTime = timeArr[0]+3;
			zhongMove.speed = speed+3;
			zhongMove.tar = 65;	
			zhongMove.heatEnable = 1;

			wumingMove.beginTime = timeArr[0]+3;
			wumingMove.speed = speed+3;
			wumingMove.tar = 75;
			wumingMove.heatEnable = 1;

			xiaoMove.beginTime = timeArr[0]+3;
			xiaoMove.speed = speed+3;
			xiaoMove.tar = 85;		
			xiaoMove.heatEnable = 0;

			if(Time<timeArr[1]&&Time>(timeArr[1]-1.5)){
				daMove.finger->advanceShen = 1;
				shiMove.finger->advanceShen = 1;
				zhongMove.finger->advanceShen = 1;
				wumingMove.finger->advanceShen = 1;
				xiaoMove.finger->advanceShen = 1;
			}

		}else if(Time<timeArr[2]){
				daMove.finger->advanceShen = 0;
				shiMove.finger->advanceShen = 0;
				zhongMove.finger->advanceShen = 0;
				wumingMove.finger->advanceShen = 0;
				xiaoMove.finger->advanceShen = 0;
			
			daMove.beginTime = timeArr[1];
			daMove.speed = speed;
			daMove.tar =0;	
			daMove.heatEnable = 1;
			daMove.finger->kpOpen = 2.1;
			daMove.finger->kdOpen = 10;
			
			shiMove.beginTime = timeArr[1];
			shiMove.speed = speed;
			shiMove.tar = 0;
			shiMove.heatEnable = 1;
			shiMove.finger->kpOpen = 2;
			shiMove.finger->kdOpen = 20;
			
			
		
			zhongMove.beginTime = timeArr[1];
			zhongMove.speed = speed;
			zhongMove.tar = 0;	
			zhongMove.heatEnable = 1;
			zhongMove.finger->kpOpen = 2;
			zhongMove.finger->kdOpen = 20;
			
			

			wumingMove.beginTime = timeArr[1];
			wumingMove.speed = speed;
			wumingMove.tar = 0;
			wumingMove.heatEnable = 1;
			wumingMove.finger->kpOpen = 3;
			wumingMove.finger->kdOpen = 10;
			
			

			xiaoMove.beginTime = timeArr[1];
			xiaoMove.speed = speed;
			xiaoMove.tar = 0;		
			xiaoMove.heatEnable = 1;
			xiaoMove.finger->kpOpen = 3.5;
			xiaoMove.finger->kdOpen = 10;
			
			
		}
		
	//放入控制器

		daP->enable = 0;
		shiP->enable = 0;
		zhongP->enable = 0;
		wumingP->enable = 0;
		xiaoP->enable = 0;
		
		MoveFinger(&daMove);
		MoveFinger(&shiMove);
		MoveFinger(&zhongMove);
		MoveFinger(&wumingMove);
		MoveFinger(&xiaoMove);
	}
	
}





void neikou(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
	char speed = 20;
	
	static MOVE_PARA daMovePara;
	static MOVE_PARA shiMovePara;
	static MOVE_PARA zhongMovePara;
	static MOVE_PARA wumingMovePara;
	static MOVE_PARA xiaoMovePara;
	if(RecBuf[3] == 66){
		daMovePara.finger = daP;
		shiMovePara.finger = shiP;
		zhongMovePara.finger = zhongP;
		wumingMovePara.finger = wumingP;
		xiaoMovePara.finger = xiaoP;

		
		//按顺序控制
		if(Time<15){
			daMovePara.beginTime = 0;
			daMovePara.speed = 1;
			daMovePara.tar = -1;	
			
		}		
		else if(Time<50){
			daMovePara.beginTime = 15;
			daMovePara.speed = speed;
			daMovePara.tar = 30;	
			daMovePara.heatEnable = 1;
//			
//			shi.beginTime = 0;
//			shi.speed = speed;
//			shi.tar = 170;
//			
//		
//			zhong.beginTime = 0;
//			zhong.speed = speed;
//			zhong.tar = 170;	
//			
//			wuming.beginTime = 0;
//			wuming.speed = speed;
//			wuming.tar = 170;

//			xiao.beginTime = 0;
//			xiao.speed = speed;
//			xiao.tar = 170;		

			if(Time<50&&Time>48){
				daMovePara.finger->advanceShen = 1;
			}
			
		}else if(Time<70){
			daMovePara.finger->advanceShen = 0;
			daMovePara.beginTime = 50;
			daMovePara.speed = speed;
			daMovePara.tar = 0;	
			daMovePara.endTime = daMovePara.speed+daMovePara.beginTime;
			daMovePara.finger->kpOpen = 1.5;
			daMovePara.finger->kdOpen = 10;
			
			
//			
//			shi.beginTime = 20;
//			shi.speed = speed;
//			shi.tar = 15;
//			shi.endTime = shi.speed+shi.beginTime;

//			zhong.beginTime = 20;
//			zhong.speed = speed;
//			zhong.tar = 15;	
//			zhong.endTime = zhong.speed+zhong.beginTime;

//			wuming.beginTime = 20;
//			wuming.speed = speed;
//			wuming.tar = 15;

//			xiao.beginTime = 20;
//			xiao.speed = speed;
//			xiao.tar = 15;		
		}
		
	//放入控制器

		daP->enable = 0;
		shiP->enable = 0;
		zhongP->enable = 0;
		wumingP->enable = 0;
		xiaoP->enable = 0;
		
		MoveFinger(&daMovePara);
//		MoveFinger(&shi);
//		MoveFinger(&zhong);
//		MoveFinger(&wuming);
//		MoveFinger(&xiao);
	}
}

void qiehuan(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){
	char speed = 5;
	float timeArr[] = {5,15,25,35,45,55,65};
	
	static MOVE_PARA daMovePara;
	static MOVE_PARA shiMovePara;
	static MOVE_PARA zhongMovePara;
	static MOVE_PARA wumingMovePara;
	static MOVE_PARA xiaoMovePara;
	if(RecBuf[3] == 67){
		daMovePara.finger = daP;
		shiMovePara.finger = shiP;
		zhongMovePara.finger = zhongP;
		wumingMovePara.finger = wumingP;
		xiaoMovePara.finger = xiaoP;

		
		//按顺序控制
		if(Time<timeArr[0]){
			daMovePara.beginTime = 0;
			daMovePara.speed = 1;
			daMovePara.tar = 5;	
			daMovePara.heatEnable = 0;

			shiMovePara.beginTime = 0;
			shiMovePara.speed = 1;
			shiMovePara.tar = 15;
			shiMovePara.heatEnable = 0;

			zhongMovePara.beginTime = 0;
			zhongMovePara.speed = 1;
			zhongMovePara.tar = 15;
			zhongMovePara.heatEnable = 0;

		}		
		else if(Time<timeArr[1]){
			daMovePara.beginTime = timeArr[0];
			daMovePara.speed = speed;
			daMovePara.tar = 110;	
			daMovePara.heatEnable = 1;

			shiMovePara.beginTime = timeArr[0];
			shiMovePara.speed = speed;
			shiMovePara.tar = 15;
			shiMovePara.heatEnable = 0;

			zhongMovePara.beginTime = timeArr[0];
			zhongMovePara.speed = speed;
			zhongMovePara.tar = 15;
			zhongMovePara.heatEnable = 0;

		}else if(Time<timeArr[2]){
			daMovePara.beginTime = timeArr[1];
			daMovePara.speed = speed;
			daMovePara.tar = 30;	
			daMovePara.heatEnable = 1;

			shiMovePara.beginTime = timeArr[1];
			shiMovePara.speed = speed;
			shiMovePara.tar = 15;
			shiMovePara.heatEnable = 0;

			zhongMovePara.beginTime = timeArr[1];
			zhongMovePara.speed = speed;
			zhongMovePara.tar = 15;
			zhongMovePara.heatEnable = 0;
		}else if(Time<timeArr[3]){
			daMovePara.beginTime = timeArr[2];
			daMovePara.speed = speed;
			daMovePara.tar = 60;	
			daMovePara.heatEnable = 1;
			//da->kpBend = -1.4;
			
			shiMovePara.beginTime = timeArr[2];
			shiMovePara.speed = speed;
			shiMovePara.tar = 90;
			shiMovePara.heatEnable = 1;

			zhongMovePara.beginTime = timeArr[2];
			zhongMovePara.speed = speed;
			zhongMovePara.tar = 15;
			zhongMovePara.heatEnable = 0;
		}else if(Time<timeArr[4]){
			daMovePara.beginTime = timeArr[3];
			daMovePara.speed = speed;
			daMovePara.tar = 30;	
			daMovePara.heatEnable = 1;
			

			shiMovePara.beginTime = timeArr[3];
			shiMovePara.speed = speed;
			shiMovePara.tar = 40;
			shiMovePara.heatEnable = 1;

			zhongMovePara.beginTime = timeArr[3];
			zhongMovePara.speed = speed;
			zhongMovePara.tar = 15;
			zhongMovePara.heatEnable = 0;
		}else if(Time<timeArr[5]){
			daMovePara.beginTime = timeArr[4];
			daMovePara.speed = speed;
			daMovePara.tar = 70;	
			daMovePara.heatEnable = 1;
			

			shiMovePara.beginTime = timeArr[4];
			shiMovePara.speed = speed;
			shiMovePara.tar = 90;
			shiMovePara.heatEnable = 1;

			zhongMovePara.beginTime = timeArr[4];
			zhongMovePara.speed = speed;
			zhongMovePara.tar = 105;
			zhongMovePara.heatEnable = 1;
			
		}else if(Time<timeArr[6]){
			daMovePara.beginTime = timeArr[5];
			daMovePara.speed = speed;
			daMovePara.tar = 0;	
			daMovePara.heatEnable = 1;
			

			shiMovePara.beginTime = timeArr[5];
			shiMovePara.speed = speed;
			shiMovePara.tar = 0;
			shiMovePara.heatEnable = 1;

			zhongMovePara.beginTime = timeArr[5];
			zhongMovePara.speed = speed;
			zhongMovePara.tar = 0;
			zhongMovePara.heatEnable = 1;
		}
		
	//放入控制器

		daP->enable = 0;
		shiP->enable = 0;
		zhongP->enable = 0;
		wumingP->enable = 0;
		xiaoP->enable = 0;
		
		MoveFinger(&daMovePara);
		MoveFinger(&shiMovePara);
		MoveFinger(&zhongMovePara);
//		MoveFinger(&wuming);
//		MoveFinger(&xiao);
	}
}






static void preHeat_init(void)
{
	da->advanceShou = 0;
	da->advanceShen = 0;
	
	shi->advanceShou = 0;
	shi->advanceShen = 0;	
	
	zhong->advanceShou = 0;
	zhong->advanceShen = 0;	
	
	wuming->advanceShou = 0;
	wuming->advanceShen = 0;	
	
	xiao->advanceShou = 0;
	xiao->advanceShen = 0;	
}



void speedControlTest(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){	

	CONTROLLER_PARA controller_table[5][20] = {
		{{5, -1, 0, 0}, {15, 50, 0, 0}, {10, -1, 0, 0}, {15, 20, 0, 0}, {10, -1, 0, 0}, {15, 80, 0, 0}, {10, -1, 0, 0}, {15, 0, 0, 0}, {-1, -1, 0, 0}},
		{{5, -1, 0, 0}, {15, 100, 0, 0}, {10, -1, 0, 0}, {15, 40, 0, 0}, {10, -1, 0, 0}, {15, 90, 0, 0}, {10, -1, 0, 0}, {15, 0, 0, 0}, {-1, -1, 0, 0}},
		{{5, -1, 0, 0}, {15, 100, 0, 0}, {10, -1, 0, 0}, {15, 40, 0, 0}, {10, -1, 0, 0}, {15, 90, 0, 0}, {10, -1, 0, 0}, {15, 0, 0, 0}, {-1, -1, 0, 0}},
		{{5, -1, 0, 0}, {15, 100, 0, 0}, {10, -1, 0, 0}, {15, 40, 0, 0}, {10, -1, 0, 0}, {15, 90, 0, 0}, {10, -1, 0, 0}, {15, 0, 0, 0}, {-1, -1, 0, 0}},
		{{5, -1, 0, 0}, {15, 100, 0, 0}, {10, -1, 0, 0}, {15, 40, 0, 0}, {10, -1, 0, 0}, {15, 90, 0, 0}, {10, -1, 0, 0}, {15, 0, 0, 0}, {-1, -1, 0, 0}},
	};
	
	static MOVE_PARA move_para;
	char cur_state_move;
	float deta_time;
	
	if(RecBuf[3] == 70){
		for(int i = 0; i < 5; i++) {
			move_para.finger = handFinger_p[i];
			deta_time = Time - handFinger_p[i]->cur_last_time;
			move_para.finger->cur_last_time = Time;
			
			if (handFinger_p[i]->cur_move_time <= 0 || handFinger_p[i]->cur_move_time > deta_time) {
				handFinger_p[i]->cur_move_state++;
				cur_state_move = handFinger_p[i]->cur_move_state;
				handFinger_p[i]->cur_move_time = controller_table[i][cur_state_move].time;
				preHeat_init();
				continue;
			}
			
			cur_state_move = handFinger_p[i]->cur_move_state;
			if(cur_state_move == -1)      continue;
			
			handFinger_p[i]->cur_move_time -= deta_time;
			move_para.speed = controller_table[i][cur_state_move].time;
			move_para.tar = controller_table[i][cur_state_move].target_angle;
			move_para.heatEnable = controller_table[i][cur_state_move].heat;
			
			if (deta_time < fabs(controller_table[i][cur_state_move].heat_time)) {
				if (controller_table[i][cur_state_move].heat_time > 0)
						handFinger_p[i]->advanceShou = 1;
				else
						handFinger_p[i]->advanceShen = 1;
			}
			MoveFinger(&move_para);
		}
		
	}

}

void  PWMDriver(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP){	
	
	setPWM(PCA9685_Addr_1, 13, 4095*daP->dutyShou);
	setPWM(PCA9685_Addr_1, 14, 4095*daP->dutyShen);
	
	setPWM(PCA9685_Addr_1, 11, 4095*shiP->dutyShou);
	setPWM(PCA9685_Addr_1, 12, 4095*shiP->dutyShen);		
	
	setPWM(PCA9685_Addr_1, 4, 4095*zhongP->dutyShou);
	setPWM(PCA9685_Addr_1, 10, 4095*zhongP->dutyShen);
	
	setPWM(PCA9685_Addr_1, 2, 4095*wumingP->dutyShou);
	setPWM(PCA9685_Addr_1, 3, 4095*wumingP->dutyShen);
	
	setPWM(PCA9685_Addr_1, 0, 4095*xiaoP->dutyShou);
	setPWM(PCA9685_Addr_1, 1, 4095*xiaoP->dutyShen);
	
}
