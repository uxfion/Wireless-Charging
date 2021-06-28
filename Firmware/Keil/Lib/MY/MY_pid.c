#include "MY_pid.h"

PID PID_Elec={
		1.40,	//电磁循迹PID参数，方向外环
		0.0,
		35.0,	
		100,//MAX 
		-100,//MIN
};

PID PID_Direct={
		0.4,	//普通循迹方向环PID，方向内环
		0.0,
		0.2,
	    1000,//MAX
        -1000,//MIN
};

PID PID_Ang_Velo={
		10.0,
		0.02,
		0,
		990,
		-990,
};

PID PID_Ang={
          15.0,
          0.0,
          3.0,
          300,
          -300,  
};

PID PID_Speed={
		0.2,
		0.03,
		0.1,
		40,
		-40,
};

PID pid_power={
	2.0,
	1.0,
	0.0,
	400,
	0,
};


float PID_Incremental(PID* PID_Ptr, float speed_real, float speed_ask)//增量式PID
{
 
  PID_Ptr->error[0] = speed_ask - speed_real;//PID_Ptr->error[0]为当前误差，PID_Ptr->error[1]为上一次误差，PID_Ptr->error[2]为上两次误差
  PID_Ptr->Ad_value = PID_Ptr->P *0.1f*(PID_Ptr->error[0] - PID_Ptr->error[1]) + 
                      PID_Ptr->I *0.1f* PID_Ptr->error[0] +
                      PID_Ptr->D * (PID_Ptr->error[0] - 2 * PID_Ptr->error[1] + PID_Ptr->error[2]);//计算PID增量
  
  PID_Ptr->error[2] = PID_Ptr->error[1];//这两句话顺序对换会出错
  PID_Ptr->error[1] = PID_Ptr->error[0];
  
  PID_Ptr->PID_Out += PID_Ptr->Ad_value;//计算PID
  
  if(PID_Ptr->PID_Out >PID_Ptr->MAX) PID_Ptr->PID_Out =PID_Ptr-> MAX;//PID限幅
  if(PID_Ptr->PID_Out <PID_Ptr->MIN) PID_Ptr->PID_Out =PID_Ptr-> MIN;
  
  return PID_Ptr->PID_Out;
}

float PID_Positional(PID* PID_Ptr, float speed_real, float speed_ask)//位置式PID
{
  float Positional = 0;
  PID_Ptr->error[0] = speed_ask - speed_real;//PID_Ptr->error[0]为当前误差，PID_Ptr->error[1]为上一次误差，PID_Ptr->error[2]为误差积分
  
	PID_Ptr->error[2] += PID_Ptr->error[0];
  if(PID_Ptr->error[2] >  50) PID_Ptr->error[2] = 50;//一般这里还会有积分限幅
  if(PID_Ptr->error[2]  <  -50) PID_Ptr->error[2] = -50;
	
  Positional = PID_Ptr->P * PID_Ptr->error[0] + 
                     PID_Ptr->I * PID_Ptr->error[2] +
                     PID_Ptr->D * (PID_Ptr->error[0] - PID_Ptr->error[1]);//计算PID
  
  PID_Ptr->error[1] = PID_Ptr->error[0];
  if(Positional >PID_Ptr-> MAX) Positional =PID_Ptr-> MAX;//PID限幅
  if(Positional <PID_Ptr-> MIN) Positional =PID_Ptr-> MIN;
  
  return  Positional;
}

// PID参数初始化
void PID_Parameter_Init(PID *sptr)
{
	sptr->error[0]  = 0;
	sptr->error[1]  = 0;
	sptr->error[2]  = 0;
	sptr->Ad_value = 0;	//Error[-2]	
	sptr->PID_Out = 0;
	sptr->SumError= 0;
}  


