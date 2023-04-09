#include "GetAngle.h"
#include "CalibrateData.h"


// 传感器信号采样

void SensorSignal(Finger* daP,Finger* shiP,Finger* zhongP,Finger* wumingP,Finger* xiaoP)
{
	int i = 0;

	AD7606_AllDeviceSample_AB(SampleData);
	float adc_factor = 5000.0/SampleData[29];
	for(i = 0; i < 8*4; i++)
	{
		Data[i] = adc_factor * SampleData[i];
	}

	Data2Angle(daP);
	Data2Angle(shiP);
	Data2Angle(zhongP);
	Data2Angle(wumingP);
	Data2Angle(xiaoP);
}

void Data2Angle(Finger* NowFinger){
	int num = NowFinger->num;
	int DIP = Data[30-(6*num+6)]+Data[30-(6*num+5)];
	int PIP = Data[30-(6*num+4)]+Data[30-(6*num+3)];
	int MCP = Data[30-(6*num+2)]+Data[30-(6*num+1)];	

	NowFinger->DIP = DIP;
	NowFinger->PIP = PIP;
	NowFinger->MCP = MCP;
	
//	switch(num){
//		case 0:{
//			DIP = (DIP-3757)*1580/(5895-3757)+2950;
//			PIP = (PIP-4203)*770/(5987-4203)+3500;
//			break;
//		}
//		case 1:{
//			DIP = (DIP-3405)*1400/(5269-3405)+3210;
//			PIP = (PIP-3807)*1190/(5182-3807)+3200;
//			MCP = (MCP-4251)*670/(4872-4251)+3390;
//			break;
//		}
//		case 2:{
//			DIP = (DIP-4322)*1090/(5434-4322)+3460;
//			PIP = (PIP-4533)*1420/(6365-4533)+3790;
//			MCP = (MCP-4774)*1320/(5676-4774)+3980;
//			break;
//		}
//		case 3:{
//			DIP = (DIP-4368)*890/(5584-4368)+3820;
//			PIP = (PIP-4428)*1590/(6690-4428)+3300;
//			MCP = (MCP-4857)*490/(5557-4857)+3740;
//			break;
//		}
//		case 4:{
//			DIP = (DIP-3650)*1850/(5796-3650)+3280;
//			PIP = (PIP-4790)*1090/(6094-4790)+4100;
//			MCP = (MCP-4972)*690/(5924-4972)+4030;
//			break;
//		}		
//		default :{
//			break;
//		}		
//	}
	if(NowFinger->maxDIP!=0){
			DIP = (DIP-NowFinger->minDIP)*(mesDIPAngle[num][1]-mesDIPAngle[num][0])/(NowFinger->maxDIP - NowFinger->minDIP)+mesDIPAngle[num][0];
			PIP = (PIP-NowFinger->minPIP)*(mesPIPAngle[num][1]-mesPIPAngle[num][0])/(NowFinger->maxPIP  -NowFinger->minPIP)+mesPIPAngle[num][0];
			MCP = (MCP-NowFinger->minMCP)*(mesMCPAngle[num][1]-mesMCPAngle[num][0])/(NowFinger->maxMCP - NowFinger->minMCP)+mesMCPAngle[num][0];
		}
//	if(NowFinger->maxDIP!=0){
//			DIP = DIP+mesDIPAngle[num][1]-NowFinger->maxDIP;
//			PIP = PIP+mesPIPAngle[num][1]-NowFinger->maxPIP;
//			MCP = MCP+mesMCPAngle[num][1]-NowFinger->maxMCP;
//		}	
	
	NowFinger->changeDIP = DIP;
	NowFinger->changePIP = PIP;
	NowFinger->changeMCP = MCP;
	
	float DIPAngle;
	float PIPAngle;
	float MCPAngle;
	//calculate DIP angle
	if(DIP<mesDIPAngle[num][0]){
		DIP = mesDIPAngle[num][0];
	}else if(DIP>=mesDIPAngle[num][1]){
		DIP = mesDIPAngle[num][1];
	}

	DIPAngle = DIPSesAngle[num][(DIP-mesDIPAngle[num][0])/10];
	if(DIP<mesDIPAngle[num][1]){
		char pos = (DIP-mesDIPAngle[num][0])/10;
		float deta = (-DIPSesAngle[num][pos] + DIPSesAngle[num][pos+1])/10.0f;
		DIPAngle = DIPAngle + deta*((DIP-mesDIPAngle[num][0])%10);		
	}
	
	
	//calculate PIP angle
	if(PIP<mesPIPAngle[num][0]){
		PIP = mesPIPAngle[num][0];
	}else if(PIP>=mesPIPAngle[num][1]){
		PIP = mesPIPAngle[num][1];
	}
	PIPAngle = PIPSesAngle[num][(PIP-mesPIPAngle[num][0])/10];
	if(PIP<mesPIPAngle[num][1]){
		char pos = (PIP-mesPIPAngle[num][0])/10;
		float deta = (-PIPSesAngle[num][pos] + PIPSesAngle[num][pos+1])/10.0f;
		PIPAngle = PIPAngle + deta*((PIP-mesPIPAngle[num][0])%10);		
	}
	
	//calculate MCP angle
	if(MCP<mesMCPAngle[num][0]){
		MCP = mesMCPAngle[num][0];
	}else if(MCP>=mesMCPAngle[num][1]){
		MCP = mesMCPAngle[num][1];
	}
	MCPAngle = MCPSesAngle[num][(MCP-mesMCPAngle[num][0])/10];
	if(MCP<mesMCPAngle[num][1]){
		char pos = (MCP-mesMCPAngle[num][0])/10;
		float deta = (-MCPSesAngle[num][pos] + MCPSesAngle[num][pos+1])/10.0f;
		MCPAngle = MCPAngle + deta*((MCP-mesMCPAngle[num][0])%10);		
	}
	
	DIPAngle = DIPAngle - DIPSesAngle[num][mesDIPAngle[num][2]/10];
	PIPAngle = PIPAngle - PIPSesAngle[num][mesPIPAngle[num][2]/10];
	MCPAngle = MCPAngle - MCPSesAngle[num][mesMCPAngle[num][2]/10];

	
	NowFinger->DIP_angle = Filter(NowFinger->DIP_angleWatch,DIPAngle);
	NowFinger->PIP_angle = Filter(NowFinger->PIP_angleWatch,PIPAngle);
	NowFinger->MCP_angle = Filter(NowFinger->MCP_angleWatch,MCPAngle);
//	
//	if(NowFinger->num == 4){
//		if(DIPAngle+PIPAngle+MCPAngle <45&&DIPAngle+PIPAngle+MCPAngle >=20){
//			NowFinger->angleAct =  (DIPAngle+PIPAngle+MCPAngle-20)*2.6+20;
//		}else if(DIPAngle+PIPAngle+MCPAngle <20){
//			NowFinger->angleAct = DIPAngle+PIPAngle+MCPAngle;
//		}
//		else{
//			NowFinger->angleAct =  (DIPAngle+PIPAngle+MCPAngle-45)*0.9+85;
//		}
//	}else{
//		NowFinger->angleAct = DIPAngle+PIPAngle+MCPAngle;
//	}
	
	//calculate TIP angle
//	NowFinger->angleAct = 1.067*(DIPAngle+PIPAngle+MCPAngle)+28.82;
//	NowFinger->angleAct = DIPAngle+PIPAngle+MCPAngle;
	if(NowFinger->num == 0){
		NowFinger->angleAct = (DIPAngle+PIPAngle);//+9.923;
	}else if(NowFinger->num == 1){
		NowFinger->angleAct =1.0*(DIPAngle+PIPAngle+MCPAngle);//+9.923;
	}else if(NowFinger->num == 2){
		NowFinger->angleAct = (DIPAngle+PIPAngle+MCPAngle);//0.85*+9.923;
	}else if(NowFinger->num == 3){
		NowFinger->angleAct = (DIPAngle+PIPAngle+MCPAngle);//1*+28.82;
	}else{
		NowFinger->angleAct = (DIPAngle+PIPAngle+MCPAngle);//0.7*+28.82;
	}

}

float Filter(float* nums,float newNum){
	int i=0;
	float sum = 0;
	
	for(i=WATCH_SIZE-1;i>0;--i){
		nums[i] = nums[i-1];
	}
	nums[0] = newNum;
	for(int i=0;i<WATCH_SIZE;i++){
		sum += nums[i];
	}
	return sum/WATCH_SIZE;
}
