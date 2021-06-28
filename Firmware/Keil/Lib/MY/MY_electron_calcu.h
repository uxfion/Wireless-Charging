#ifndef __SELFBUILD_ELECTRON_CALCU_H_
#define __SELFBUILD_ELECTRON_CALCU_H_

#include "headfile.h"

#define ADC_Max 3103//五电感最大值
#define ADC_R_R_Min ADC_R_R_Min_8bit*(ADC_Max/193)	//五电感最小值
#define ADC_R_Min   ADC_R_Min_8bit*(ADC_Max/193)
#define ADC_M_Min   ADC_M_Min_8bit*(ADC_Max/193)
#define ADC_L_Min   ADC_L_Min_8bit*(ADC_Max/193)
#define ADC_L_L_Min ADC_L_L_Min_8bit*(ADC_Max/193)

extern struct AD_Data_Typedef AD_Data;
extern int8 Flag_RoundAbout;
extern float Circum_Para;
extern float GYROz_Sum;


struct AD_Data_Typedef	//电磁传感器数据相关结构体
{
	uint8 AD_L_L;	//最左侧电感值
	uint8 AD_L_L_Last;	//最左侧电感值上一次
	
	uint8 AD_L;	//左侧电感值
	uint8 AD_L_Last;	//左侧电感值上一次
	
	uint8 AD_M;	//中间电感值
	uint8 AD_M_Last;	//中间电感值上一次
	
	uint8 AD_R;	//右侧电感值
	uint8 AD_R_Last;	//右侧电感值上一次
	
	uint8 AD_R_R;	//最右侧电感值
	uint8 AD_R_R_Last;	//最右侧电感值上一次
	
	float Differ;
	float Diff_Ratio;
	
	float Error_Out;		//最终输出偏差值

};


uint8 ADC_Calcu(void);
uint8 ADC_DataGet(void);

#endif
