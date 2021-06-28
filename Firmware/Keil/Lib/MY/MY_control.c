#include "MY_control.h"

int16
		Speed_Min = 20,	// 左右最小速度
		Direct_Para = 0,// 转向系数
		Direct_Last = 0,
		Radius = 0,		// 目标转向半径倒数
		Balance_Para = 0;
float GYROy_Set=0,Angle_Set=0;

Motor_TypeDef Motor={0};

uint8 Init_All(void)
{
	board_init();		//初始化内部寄存器，勿删除此句代码
	gpio_pull_set(P4_0,PULLUP);
	gpio_pull_set(P4_1,PULLUP);
	gpio_pull_set(P4_2,PULLUP);
	gpio_mode(P1_5,GPO_PP);
	P15 =1;		//高电平还在充电
//	iap_init();
//	eeprom_init();
	OLED_Init();
	OLED_ClearScreen(0x00);
	ctimer_count_init(CTIM3_P04);
	ctimer_count_init(CTIM4_P06);

	

	uart_init(UART_1, UART1_RX_P30, UART1_TX_P31, 115200, TIM_1);
//	LED = 0;
	pit_timer_ms(TIM_0, 1);
//	MPU6050_ADDDMPInit();
//	Menu_Set()；
	
	return SUCCESS;
}

void UI_Send(void)
{
	int16 UI_Data[8] = {0};
  
	UI_Data[0] = (int16)pitch*100;
	UI_Data[1] = (int16)roll*100;
	UI_Data[2] = (int16)yaw*100;
	UI_Data[3] = 0;
	UI_Data[4] = 0;
	UI_Data[5] = 0;
	UI_Data[6] = 0;
	UI_Data[7] = 0;
	vcan_sendware((uint8 *)UI_Data, sizeof(UI_Data));
}

uint8 PWM_Set(void)		//pwm output
{
//	旧驱动，能耗较大
//	pwm_duty(PWM_CH_L_Z,((Motor.PWM_L>0)?Motor.PWM_L:0));	
//	pwm_duty(PWM_CH_L_F,((Motor.PWM_L<0)?(-Motor.PWM_L):0)); 
//	pwm_duty(PWM_CH_R_Z,((Motor.PWM_R>0)?Motor.PWM_R:0));
//	pwm_duty(PWM_CH_R_F,((Motor.PWM_R<0)?(-Motor.PWM_R):0));
	
//	新驱动方式
	pwm_duty(PWM_CH_L_Z, ((Motor.PWM_L>0)?(PWM_DUTY_MAX-Motor.PWM_L):PWM_DUTY_MAX));
	pwm_duty(PWM_CH_L_F, ((Motor.PWM_L<0)?(PWM_DUTY_MAX+Motor.PWM_L):PWM_DUTY_MAX));
	pwm_duty(PWM_CH_R_Z, ((Motor.PWM_R>0)?(PWM_DUTY_MAX-Motor.PWM_R):PWM_DUTY_MAX));
	pwm_duty(PWM_CH_R_F, ((Motor.PWM_R<0)?(PWM_DUTY_MAX+Motor.PWM_R):PWM_DUTY_MAX));

	
	return SUCCESS;
}

uint8 Balance(void)
{
	if(FLAG_2MS)
	{
		FLAG_2MS=0;
		DataRead();
		Balance_Para = PID_Incremental(&PID_Ang_Velo,gyroy,GYROy_Set);

//		Motor.PWM_L  = Balance_Para - Direct_Last;
//        Motor.PWM_R  = Balance_Para + Direct_Last;

//		Motor.PWM_L --;
        LED =~ LED;
//        Refresh_MPUTeam(GYRO);
//        Data_Filter();
        
//		Balance_Para = PID_Incremental(&PID_Ang_Velo,MPU6050.GYRO_Real.y,GYROy_Set);
		
		
//		Motor.PWM_L = PID_Incremental(&PID_Ang_Velo,MPU6050.GYRO_Real.y,GYROy_Set);
		
        PWM_Set();	//根据计算的PWM给电机赋值
        
	}
	
	if(FLAG_10MS)
	{
		FLAG_10MS=0;
        
		Speed_Get();
		GYROy_Set = PID_Positional(&PID_Ang,pitch+40,0);

//		
//        ADC_Calcu();
//        
//        Euler_Get();


////		Radius = PID_Positional(&PID_Elec, AD_Data.Error_Out, 0);
////		Direct_Para = PID_Positional(&PID_Direct,gyroz,(Radius*Motor.Speed_Ave_Now)/10.0);
////		Direct_Last  = (int32)(Direct_Last*0.3 + Direct_Para*0.7);

	}
//	
//	if(FLAG_100MS)
//	{
//		FLAG_100MS=0;
//		Angle_Set = (-(int16)PID_Positional(&PID_Speed,Motor.Speed_Ave_Now*0.5,Speed_Set));

//	}
	
	
	return SUCCESS;
}

uint8 Speed_Get(void)	//得到电机速度（编码器）
{
	Motor.Speed_L = (CODER_DIR_L == 1) ? ctimer_count_read(CODER_L) : -ctimer_count_read(CODER_L);
	Motor.Speed_R = (CODER_DIR_R == 0) ? ctimer_count_read(CODER_R) : -ctimer_count_read(CODER_R);
	ctimer_count_clean(CODER_L);
	ctimer_count_clean(CODER_R);

	Motor.Speed_R = Motor.Speed_R*0.9 + Motor.Speed_R_Last*0.1;
	Motor.Speed_R_Last = Motor.Speed_R;	// 更新右轮速度
	
	Motor.Speed_L = 0.9*Motor.Speed_L + 0.1*Motor.Speed_L_Last;	// 低通滤波
	Motor.Speed_L_Last = Motor.Speed_L;	// 更新左轮速度

	Motor.Speed_Ave_Now = (Motor.Speed_L + Motor.Speed_R) / 2;
	
	Motor.Speed_Ave_Now = Motor.Speed_Ave_Now *0.9 + Motor.Speed_Ave_Last * 0.1;
	Motor.Speed_Ave_Last = Motor.Speed_Ave_Now;
	
	return SUCCESS;
}
