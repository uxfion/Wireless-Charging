/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		pwm
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __STC8_PWM_H
#define __STC8_PWM_H
#include "common.h"



#define PWM_DUTY_MAX 1000


typedef enum
{
	PWM0_P20 = 0x00,PWM0_P10,PWM0_P60,
	                                     
	PWM1_P21 = 0x10,PWM1_P11,PWM1_P61,
	                                  
	PWM2_P22 = 0x20,PWM2_P12,PWM2_P62,
	                                  
	PWM3_P23 = 0x30,PWM3_P13,PWM3_P63,
	                                  
	PWM4_P24 = 0x40,PWM4_P14,PWM4_P64,
	                                  
	PWM5_P25 = 0x50,PWM5_P15,PWM5_P65,

	PWM6_P26 = 0x60,PWM6_P16,PWM6_P66,
	
	PWM7_P27 = 0x70,PWM7_P17,PWM7_P67,
}PWMCH_enum;


void pwm_init(PWMCH_enum pwmch,uint32 freq, uint16 duty);
void pwm_duty(PWMCH_enum pwmch, uint16 duty);
void pwm_comple_duty(PWMCH_enum pwmch1, PWMCH_enum pwmch2, uint16 duty);
void pwm_freq(uint32 freq);


#endif
