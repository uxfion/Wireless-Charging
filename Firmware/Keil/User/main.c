#include "headfile.h"

//board.h文件中FOSC的值设置为0,则程序自动识别系统频率

/*board.h文件中FOSC的值设置不为0，则系统频率为FOSC的值，
在使用stc-isp工具下载程序的时候需要将IRC频率设置为FOSC的值*/


void main()
{
	uint8 a[2] = {0};
	float temp;
	DisableGlobalIRQ();	//关闭总中断
	Init_All();
	pwm_init(PWM0_P60, 7200, 0);
//	pwm_init(PWM2_P62, 7200, 0);
	pwm_init(PWM4_P64, 7200, 0);
//	pwm_init(PWM6_P66, 7200, 0);
	pca_dealy_init();
	adc_init(ADC_P01, ADC_SYSclk_DIV_2);
	adc_init(ADC_P05, ADC_SYSclk_DIV_2);
	adc_init(ADC_P06, ADC_SYSclk_DIV_2);
	iap_init();

	pca_init_interrupt_ms(PCA_0, 1);
	EnableGlobalIRQ();	//开启总中断
	
	
	target_power =((iap_read_byte(0x0000)<<8) | iap_read_byte(0x0001))/100.0f;
	pwm_duty(PWM0_P60, 500);
	pwm_duty(PWM4_P64, 100);
    while(1)
	{
			OLED_P6x8Str(0, 0, "charge_vol = ");
			OLED_P6x8Flo(79, 0, charge_vol, 2);
//			OLED_P6x8Str(0, 1, "charge_cur = ");
//			OLED_P6x8Flo(79, 1, charge_cur, 1);
			OLED_P6x8Int(0,1,FLAG_CHARGE_ERROR,2);
			OLED_P6x8Str(0, 2, "cap_vol = ");
			OLED_P6x8Flo(79, 2, cap_vol, 2);
			OLED_P6x8Str(0, 3, "power = ");
			OLED_P6x8Flo(79, 3, actual_power, 2);
			OLED_P6x8Str(0, 4, "ctrl_out = ");
			OLED_P6x8Flo(79, 4, PowerControl_Out_New, 4);
			OLED_P6x8Str(0, 5, "time = ");
			OLED_P6x8Int(79, 5, time, 2);
			OLED_P6x8Flo(0, 6, target_power, -2);
			OLED_P6x8Str(60, 6, "target");
			
//			OLED_P6x8Int(0,0,FLAG_CHARGE_ERROR,2);
			
			if(FLAG_10MS)
			{
				FLAG_10MS = 0;
				if(BUTTON_LEFT == 0)
				{
					target_power -= 0.1;
				}
				else if(BUTTON_RIGHT == 0)
				{
					target_power += 0.1;
				}
				else if(BUTTON_MID == 0)
				{
					a[0] = (uint8)(((uint16)(target_power*100))>>8);		//高8位
					a[1] = (uint8)((uint16)(target_power*100));			//低8位
					iap_erase_page(0x0000);
					iap_write_bytes(0x0000, a, 2);
					temp = ((iap_read_byte(0x0000)<<8) | iap_read_byte(0x0001))/100.0f;
					OLED_P6x8Flo(0, 7, temp, 2);
					OLED_P6x8Str(60, 7, "eeprom");
				}
			}
					
			//---------------充电判断----------------------//
			//--------不用改---------//
			if(cap_vol <= TARGET_CAP_VOL && actual_power <= 4)	//充电异常，快闪
			{
				if(FLAG_100MS)
				{
					FLAG_100MS = 0;
					LED =~ LED;
					FLAG_CHARGE_ERROR += 1;
				}
				if(FLAG_50MS)
				{
					FLAG_50MS = 0;
					FLAG_POWER_SUBTRACT += 1;
				}
			}
			if(cap_vol <= TARGET_CAP_VOL && actual_power > 4)		//正在充电，常量
			{
				if(actual_power > 10)
				FLAG_CHARGE_ERROR = 0;
				LED = 0;
				if(FLAG_50MS)
				{
					FLAG_50MS = 0;
					FLAG_POWER_PLUS += 1;
				}
			}
			if(cap_vol > TARGET_CAP_VOL)		//充电完成，慢闪
			{
//				PowerControl_Out_New = 0;
				SEND_CHARGE_DOWN = 0;
				if(FLAG_1S)
				{
					FLAG_1S = 0;
					LED =~ LED;
				}
			}
			
			
			
			
			//----自适应功率----//
			if(FLAG_200MS)
			{
				FLAG_200MS = 0;
				if(target_power <= 15) target_power = 15;
				if(target_power >= 50) target_power = 50;
								//-----保存数据------//
				a[0] = (uint8)(((uint16)(target_power*100))>>8);		//高8位
				a[1] = (uint8)((uint16)(target_power*100));			//低8位
				iap_erase_page(0x0000);
				iap_write_bytes(0x0000, a, 2);
				temp = ((iap_read_byte(0x0000)<<8) | iap_read_byte(0x0001))/100.0f;
				OLED_P6x8Flo(0, 7, temp, 2);
				OLED_P6x8Str(60, 7, "eeprom");
				
				
				
				if(FLAG_POWER_SUBTRACT >= 1 || charge_vol < 12)
				{
					FLAG_POWER_SUBTRACT = 0;
					target_power -= 1.5;
				}
				else if(FLAG_POWER_PLUS >= 1 && charge_vol > 14.8)
				{
					FLAG_POWER_PLUS = 0;
					target_power += 0.1;
				}

			
			}
	}
}
