#include "SELFBUILD_electron_calcu.h"

struct AD_Data_Typedef  AD_Data;

uint8 ADC_DataGet(void)	
{
	uint16 ad_temp[5][5]={0};
	
	for(int8 i=0;i<5;i++)	//取五次电感值
	{
		ad_temp[0][i]=adc_convert(AD_CH_R__, ADC_12BIT);
		ad_temp[1][i]=adc_convert(AD_CH_R_1, ADC_12BIT);
		ad_temp[2][i]=adc_convert(AD_CH_M_1, ADC_12BIT);
		ad_temp[3][i]=adc_convert(AD_CH_L_1, ADC_12BIT);
		ad_temp[4][i]=adc_convert(AD_CH_L__, ADC_12BIT);
	}
	for(int8 i=0;i<5;i++)//冒泡排序升序五次电感值
	{
		for(int8 j=0;j<4;j++)
		{
			for(int8 k=0;k<4-j;k++)
			{
				if(ad_temp[i][k] > ad_temp[i][k+1])        //前面的比后面的大  则进行交换
				{
					uint16 temp;
					temp = ad_temp[i][k+1];
					ad_temp[i][k+1] = ad_temp[i][k];
					ad_temp[i][k] = temp;
				}
			}
		}
	}
	//电感值归一化，排序后中间三个值得平均值作为当前电感值
    AD_Data.AD_R_R = (uint8)RANGE(100-(100*(((ad_temp[0][1] + ad_temp[0][2] + ad_temp[0][3]) / 3.0-ADC_R_R_Min)/ ((ADC_Max-ADC_R_R_Min)*1.0))),155,0);
	AD_Data.AD_R   = (uint8)RANGE(100-(100*(((ad_temp[1][1] + ad_temp[1][2] + ad_temp[1][3]) / 3.0-ADC_R_Min)  / ((ADC_Max-ADC_R_Min)  *1.0))),155,0);
    AD_Data.AD_M   = (uint8)RANGE(100-(100*(((ad_temp[2][1] + ad_temp[2][2] + ad_temp[2][3]) / 3.0-ADC_M_Min)  / ((ADC_Max-ADC_M_Min)  *1.0))),155,0);
    AD_Data.AD_L   = (uint8)RANGE(100-(100*(((ad_temp[3][1] + ad_temp[3][2] + ad_temp[3][3]) / 3.0-ADC_L_Min)  / ((ADC_Max-ADC_L_Min)  *1.0))),155,0);
    AD_Data.AD_L_L = (uint8)RANGE(100-(100*(((ad_temp[4][1] + ad_temp[4][2] + ad_temp[4][3]) / 3.0-ADC_L_L_Min)/ ((ADC_Max-ADC_L_L_Min)*1.0))),155,0);


    return SUCCESS;
}

uint8 ADC_LastDataSave(void)	//保存上次AD值函数
{
    AD_Data.AD_L_Last   =  AD_Data.AD_L;
    AD_Data.AD_R_Last   =  AD_Data.AD_R;
    AD_Data.AD_L_L_Last =  AD_Data.AD_L_L;
    AD_Data.AD_R_R_Last =  AD_Data.AD_R_R;
    AD_Data.AD_M_Last   =  AD_Data.AD_M;
    return SUCCESS;	
}

/****环岛参数******/
float RA_Turn_Para = 0.8;
int16 RA_Anger = 330;//115
int16 Huandao_DIR[3] = {0,0,0};//1左环 -1右环

float Circum_Para = 4.60;
float GYROz_Sum = 0, RA_ErrorOut = 0;
int8 Flag_RA = 0, Flag_RA_DIR = 0;	//RA for Roundabout 环岛

#define RA_LEFT 1
#define RA_RIGHT -1

uint8 ADC_Calcu(void)	//10ms
{
	static float Diff_Ratio = 0, Differ = 0;
	static float Direct_Error = 0;
	static int8 RA_Timer = 0, RA_Rise = 0;
	static int16 Rise_temp[3] = {0};
//	GYROz_Sum += (gyroz *0.935f * 0.01f * Circum_Para*0.3f);

	ADC_DataGet();	//电感数据读取并滤波
	Differ = AD_Data.AD_R_R - AD_Data.AD_L_L;
	Diff_Ratio = 100 * (float)(Differ) / (float)(AD_Data.AD_L_L + AD_Data.AD_R_R + AD_Data.AD_M);

	if (Flag_RA == 0 && (AD_Data.AD_M > 65 && ((AD_Data.AD_R_R> 45&& AD_Data.AD_L_L>25) || (AD_Data.AD_L_L >45&& AD_Data.AD_R_R>25))))
	{
		Flag_RA = 1;
		gpio_set(BUZZER, ON);
		if((AD_Data.AD_L_L >45&& AD_Data.AD_R_R>25))
		{
			Flag_RA_DIR = RA_LEFT;    //向左
		}
		else if((AD_Data.AD_R_R> 45&& AD_Data.AD_L_L>25))
		{
			Flag_RA_DIR = RA_LEFT;    //向右
		}
	
	}	//第一步 识别圆环
	
	if (Flag_RA==1)
	{
		GYROz_Sum += (MPU6050.GYRO_Real.z *0.935f * 0.01f * Circum_Para*0.3f);
		RA_Timer ++;
	}
	
	if(Flag_RA == 1)
	{
		if(RA_Rise == 0 &&(((int)(AD_Data.AD_M ) > Rise_temp[0] && Rise_temp[0] > Rise_temp[1] && Rise_temp[1] > Rise_temp[2])))
		{
			RA_Rise = 3;
		}
		if(RA_Rise == 3 && (int)(AD_Data.AD_M) < Rise_temp[0])//2
		{
			Flag_RA = 3;
			Rise_temp[0] = 0, Rise_temp[1] = 0, Rise_temp[2] = 0;
		}
		Rise_temp[2] = Rise_temp[1], Rise_temp[1] = Rise_temp[0], Rise_temp[0] = (int)(AD_Data.AD_M);
	}		//中电感上升沿
	
	if(Flag_RA == 3)
	{	
		RA_ErrorOut = RA_Turn_Para *  (float) ((AD_Data.AD_R_R -AD_Data.AD_L_L ) * 2 + (AD_Data.AD_R - AD_Data.AD_L) * 3) / 5.0f;//竖直电感差比和;
		if(fabs(GYROz_Sum) > 45)
		{
			Flag_RA = 5;
		}
		if(RA_ErrorOut * Flag_RA_DIR>0) RA_ErrorOut = 0;
		if(RA_ErrorOut<35&&RA_ErrorOut>0)  RA_ErrorOut=35;
		if(RA_ErrorOut>-35&&RA_ErrorOut<0)  RA_ErrorOut=-35;
		if(RA_ErrorOut >250) RA_ErrorOut =250;
		if(RA_ErrorOut < -250) RA_ErrorOut = -250;
	}	
	
	if(Flag_RA == 5 && fabs(GYROz_Sum) > RA_Anger)	//准备出环
	{
		Flag_RA = 7;
		RA_Timer = RA_Timer*0.15;
	}
	if(Flag_RA == 7 && fabs(GYROz_Sum) > 360)
	{
		if(RA_Timer--< 0)
		{		
			Flag_RA = 9;
			RA_Rise = 0;
			GYROz_Sum = 0;	
		}
		Flag_RA = 0;
		RA_Timer =0;
		Flag_RA_DIR=0;
		gpio_set(BUZZER, OFF);
//		Huandao_time++;

	}
	
		

	
/*************误差输出*******/
	if(Flag_RA == 3)
	{
		AD_Data.Error_Out = RA_ErrorOut;
	}
	else if(Flag_RA == 7)
	{
//		if(Flag_RA_DIR==-1)
//			AD_Data.Error_Out =  100 * ((float)(AD_Data.AD_M-(AD_Data.AD_L_L*2+AD_Data.AD_L)) / (float)(AD_Data.AD_M+AD_Data.AD_L_L));
//		if( Flag_RA_DIR==1)
//			AD_Data.Error_Out = 100 * ((float)((AD_Data.AD_R_R*2+AD_Data.AD_R)-AD_Data.AD_M) / (float)(AD_Data.AD_M+AD_Data.AD_R_R));
//		if(Direct_Error * Flag_RA_DIR<0) 
//		{
//			AD_Data.Error_Out *=0.05f;
////			Direct_Error *=0.2f;	
//		}
		AD_Data.Error_Out = Diff_Ratio;
	}
	else
	{
		AD_Data.Error_Out = Diff_Ratio;
	}

	return SUCCESS;
	
}


/******
if(Flag_Huandao == 0 && ((AD_Data.AD_R_R> 90&& AD_Data.AD_L_L>40) || (AD_Data.AD_L_L>90&&AD_Data.AD_R_R>40) )&&AD_Data.AD_M>75&&pitch>15.0f&&(gpio_get (Light) == 0))//准备进环岛
	{
		Flag_Huandao = 1;
		Flag_Huandao_DIR = Huandao_DIR[huandaoh_geshu];
	}	
	if(Flag_Huandao == 1 || Flag_Huandao == 3 || Flag_Huandao == 5 )		//若进入环岛，开始记录旋转角度
	{
		GYROZ_SUM += (g_fGyroZRead * 0.9765 * 0.01 * 1.125);
	  Huandao_Timer++;
	}
  
	if(Flag_Huandao == 1)
	{
		if(Huandao_Rise == 0 &&AD_Data.AD_M >100&&(((int)AD_Data.AD_M > ADC_Prodata_M_1[0] && ADC_Prodata_M_1[0] > ADC_Prodata_M_1[1] && ADC_Prodata_M_1[1] > ADC_Prodata_M_1[2])))
		{
			Huandao_Rise = 3;
		}
	if(Huandao_Rise == 3 && (int)(AD_Data.AD_M) < ADC_Prodata_M_1[0])//2
		{
			Flag_Huandao = 3;
			ADC_Prodata_M_1[0] = 0, ADC_Prodata_M_1[1] = 0, ADC_Prodata_M_1[2] = 0;
		}
		ADC_Prodata_M_1[2] = ADC_Prodata_M_1[1], ADC_Prodata_M_1[1] = ADC_Prodata_M_1[0], ADC_Prodata_M_1[0] = (int)(AD_Data.AD_M);
	}
	if(Flag_Huandao == 3)
	{
		fHuandaoErrorOut = g_fHuandaoTurn*0.8*(float) ((AD_Data.AD_R_R -AD_Data.AD_L_L ) * 3 + (AD_Data.AD_R - AD_Data.AD_L) * 2) /5.0;;//竖直电感差比和;
		if(fabs(GYROZ_SUM) >45)
		{
			Flag_Huandao = 5;
		}
		if(fHuandaoErrorOut * Flag_Huandao_DIR>0) fHuandaoErrorOut = 0;
		if(fHuandaoErrorOut<30&&fHuandaoErrorOut>0)  fHuandaoErrorOut=30;
		if(fHuandaoErrorOut>-30&&fHuandaoErrorOut<0)  fHuandaoErrorOut=-30;
		if(fHuandaoErrorOut >300) fHuandaoErrorOut =300;
		if(fHuandaoErrorOut < -300) fHuandaoErrorOut = -300;
	}
		
	
	if(Flag_Huandao == 5 && fabs(GYROZ_SUM) > g_nHuandaoAnger)	//准备出环
	{
		Flag_Huandao = 7;
		Huandao_Timer = Huandao_Timer*0.20;
		
	}
	if(Flag_Huandao == 7 && ((Huandao_Timer --)) < 0)
	{
		Flag_Huandao = 9;
	}
	if(Flag_Huandao == 9 &&AD_Data.AD_R < 30&&  AD_Data.AD_L < 30&& AD_Data.AD_R_R  < 100 && AD_Data.AD_L_L < 100)//判断是否出环，清零环岛标志位以便下次判断
	{
		Huandao_Rise = 0;
		GYROZ_SUM = 0;	
   	Flag_Huandao = 0;
		Huandao_Timer =0;
		Flag_Huandao_DIR=0;
		huandaoh_geshu++;
		distance_huandao=0;
		
	}
*****/
