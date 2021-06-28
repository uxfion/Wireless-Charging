#include "MY_charge.h"

float 	charge_vol = 0,
		charge_cur = 0,
		cap_vol = 0,
		i_set = 0,
		actual_power = 0;

float PowerControl_Integral = 0, PowerControl_Out_New=0, PowerControl_Out_Old=0;
float target_power = 21,		//23.5
		power_p = 2,
		power_i = 1,
		power_d = 0;
uint8 FLAG_CHARGE_DOWN = 0,
	  FLAG_CHARGE_ERROR = 0,
	  FLAG_POWER_PLUS = 0,
	  FLAG_POWER_SUBTRACT = 0;
void get_power(void)
{
	uint8 i;
	uint16	ad_v_res[10] = {0},
			ad_i_res[10] = {0},
			ad_cap_res[10] = {0};
	uint16	ad_v = 0,
			ad_i = 0,
			ad_cap = 0;
			
	for(i = 0; i < 10; i++)
	{
		ad_v_res[i] = adc_once(ADC_P01, ADC_12BIT);
		ad_i_res[i] = adc_once(ADC_P05, ADC_12BIT);
		ad_cap_res[i] = adc_once(ADC_P06, ADC_12BIT);
	}
	
	for(i = 0; i < 10; i++)
	{
		ad_v += ad_v_res[i];
		ad_i += ad_i_res[i];
		ad_cap += ad_cap_res[i];
	}
	
	ad_v = ad_v / 10;
	ad_i = ad_i / 10;
	ad_cap = ad_cap /10;
	
	charge_vol = (ad_v / 4095.0) * 3.30 * 11.08;
   	charge_cur = (ad_i / 4095.0) * 3.30 * 2.5;
	cap_vol = (ad_cap / 4095.0) * 3.30 * 11.0;
	
	actual_power = charge_vol * charge_cur;
	
}

void power_control(void)
{
	float cal_p = 0, cal_i = 0, delta = 0;
	PowerControl_Out_Old = PowerControl_Out_New;
	
	delta = target_power - actual_power;
	
	cal_p = delta * power_p;
	cal_i = delta * power_i;
	
	PowerControl_Integral += cal_i;
	
	if(PowerControl_Integral > 200)
		PowerControl_Integral = 200;
	
	PowerControl_Out_New = cal_p + PowerControl_Integral;
	
//	if(PowerControl_Out_New-PowerControl_Out_Old > 50)
//		PowerControl_Out_New = PowerControl_Out_Old;
	
	if(PowerControl_Out_New > 400)		//Iset引脚不超过2V
		PowerControl_Out_New = 400;
	if(PowerControl_Out_New < 0)
		PowerControl_Out_New = 0;

	if(FLAG_CHARGE_ERROR == 1)
	{
		FLAG_CHARGE_ERROR = 0;
		PowerControl_Out_New = 10;
		PowerControl_Out_Old = 0;
		PowerControl_Integral = 0;
//		actual_power = 0;
	}

	
//	Set_TLC5615(PowerControl_Out_New / 200);
	
	
}

float error[3] = {0};
float ad_value = 0;
void pi_incre(void)
{
	error[0] = target_power - actual_power;
	ad_value = power_p*0.1f*(error[0]-error[1])
			 + power_i*0.1f*error[0]
			 + power_d*(error[0]-2*error[1]+error[2]);
	error[2] = error[1];
	error[1] = error[0];
	PowerControl_Out_New += ad_value;
	if(PowerControl_Out_New > 300)		//Iset引脚不超过2V
	PowerControl_Out_New = 300;
	if(PowerControl_Out_New < 0)
	PowerControl_Out_New = 0;
	if(FLAG_CHARGE_ERROR == 1)
	{
		FLAG_CHARGE_ERROR = 0;
		PowerControl_Out_New = 10;
		ad_value = 0;
		error[0] = 0;
		error[1] = 0;
		error[2] = 0;
	}
	Set_TLC5615(PowerControl_Out_New / 200);
	
}

float power_out = 0;
void control(void)
{
	power_out = PID_Incremental(&pid_power, actual_power, target_power);
	if(FLAG_CHARGE_ERROR == 5)
	{
		FLAG_CHARGE_ERROR = 0;
		power_out = 10;
		pid_power.PID_Out = 10;
//		pid_power.Ad_value = 0;
//		pid_power.error[0] = 0;
//		pid_power.error[1] = 0;
//		pid_power.error[2] = 0;
		
	}
	Set_TLC5615(power_out / 200.0);
}

