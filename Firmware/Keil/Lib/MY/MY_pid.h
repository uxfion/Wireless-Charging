#ifndef _MY_PID_H
#define _MY_PID_H

#include "headfile.h"

typedef struct
{
	float P;
	float I;
	float D;
	int16 MAX;
	int16 MIN;
	float error[3];
	float Ad_value;
	float PID_Out;
	float SumError;//累计误差
}PID;

extern PID PID_Ang_Velo;//角速度环PID参数
extern PID PID_Ang;		//角度环PID参数
extern PID PID_Speed;	//速度环PID参数
extern PID PID_Elec;	//转向外环
extern PID PID_Direct;	//转向内环PID参数
extern PID pid_power;	//恒功率控制PID参数

float PID_Incremental(PID* PID_Ptr, float speed_real, float speed_ask);//不一定要和我这样这么写，只是给你一个参考
float PID_Positional(PID* PID_Ptr, float speed_real, float speed_ask);
void PID_Parameter_Init(PID *sptr);
#endif
