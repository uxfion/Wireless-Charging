#ifndef __MY_CONTROL_H_
#define __MY_CONTROL_H_

#include "headfile.h"

#define RANGE(x, max, min)  ((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) ))
#define SUCCESS 1
#define FAIL    0

typedef struct 	//电机相关数据结构体
{
//	int32 Speed_Ave_Set;	
//  
//	int8 Left_Crazy;	// 电机疯转
//	int8 Right_Crazy;	// 电机疯转
//	
//	int8 Crazy_StopFlag;
//  
	int16 Speed_L;
	int16 Speed_L_Last;
	int16 Left_Acc;
	
	int16 Speed_R;
	int16 Speed_R_Last;
	int16 Right_Acc;
	
	int16 Speed_Ave_Now;
	int16 Speed_Ave_Last;
//	int16 Crazy_Count;
	
	
	int8 Car_Direct;		//车运动方向
	int16 PWM_L;
	int16 PWM_R;
	int16 PWM_L_Last;
	int16 PWM_R_Last;
}Motor_TypeDef;

extern Motor_TypeDef Motor;	//电机相关变量

uint8 Init_All(void);
uint8 Speed_Get(void);
uint8 PWM_Set(void);
uint8 Euler_Get(void);
uint8 Balance(void);
void UI_Send(void);
#endif