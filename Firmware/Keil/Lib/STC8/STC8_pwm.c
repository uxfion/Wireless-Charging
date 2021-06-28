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

#include "STC8_pwm.h"
#include "STC8_gpio.h"
#include "board.h"

#define PWMx_CR_BASE_ADDR 0xff04
#define PWMx_T1_BASE_ADDR 0xff00
#define PWMx_T2_BASE_ADDR 0xff02


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM_gpio初始化（内部使用用户无需关心）
//  @param      pwmch       PWM通道号及引脚
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void pwm_set_gpio(PWMCH_enum pwmch)
{
	uint8 gpio_num;

	if((pwmch & 0x03) == 0) 	 //P2
	{
		gpio_num = 0x20;
	}
	else if((pwmch & 0x03) == 1) //P1
	{
		gpio_num = 0x10;
	}
	else if((pwmch & 0x03) == 2) //P6
	{
		gpio_num = 0x60;
	}
	gpio_num |= (pwmch >> 4);
	
	gpio_mode(gpio_num,GPO_PP);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM初始化
//  @param      pwmch       PWM通道号及引脚
//  @param      freq        PWM频率(64Hz-3MHz)
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           pwm_init(PWM0_P00, 100, 5000);     //初始化PWM0  使用引脚P0.0  输出PWM频率100HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//							PWM_DUTY_MAX在fsl_pwm.h文件中 默认为10000
//	@note					所有的PWM通道只能设置为同一个频率
//							PWMx 同一时间，只能有一个引脚输出PWM
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(PWMCH_enum pwmch,uint32 freq, uint16 duty)
{
	
	uint16 match_temp;
	uint16 period_temp; 
	uint8 freq_div = 0;
	
	pwm_set_gpio(pwmch);					//设置为推挽输出
	P_SW2 |= 0x80;
	
	//清除PWM引脚
	(*(unsigned char volatile xdata *)(PWMx_CR_BASE_ADDR + (pwmch & 0xF0))) &= ~(0x03 << 3);
	//切换PWM引脚
	(*(unsigned char volatile xdata *)(PWMx_CR_BASE_ADDR + (pwmch & 0xF0))) |= (pwmch & 0x03) << 3;
	
	freq_div = (sys_clk / freq) >> 15;
	period_temp = sys_clk / freq / (freq_div + 1);					//周期
	match_temp = period_temp * ((float) duty / PWM_DUTY_MAX);	//占空比

	//设置pwmn频率为 系统时钟/freq_div
	PWMCKS = freq_div;
	//设置PWMn周期为period_temp个 PWM 时钟
	PWMC = (uint16)period_temp; 	

	// 在计数值为 match_temp 地方 pwmch 通道输出低电平
	(*(unsigned int  volatile xdata *)(PWMx_T1_BASE_ADDR + (pwmch & 0xF0))) = (uint16)match_temp;
//	// 在计数值为 match_temp 地方 pwmch 通道输出高电平
//	(*(unsigned int  volatile xdata *)(PWMx_T2_BASE_ADDR + (pwmch & 0xF0))) = (uint16)0;


	//使能 PWMx 输出
	(*(unsigned char volatile xdata *)(PWMx_CR_BASE_ADDR + (pwmch & 0xF0))) |= 1 << 7;

	P_SW2 &= ~0x80;

	PWMCR = 0x80; // 启动 PWM 模块
	

}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM占空比设置
//  @param      pwmch       PWM通道号及引脚
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           pwm_duty(PWM0_P00, 5000);     //初始化PWM0  使用引脚P0.0  输出PWM频率50HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//							PWM_DUTY_MAX在fsl_pwm.h文件中 默认为50000
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty(PWMCH_enum pwmch, uint16 duty)
{
	uint16 match_temp;
	P_SW2 |= 0x80;
	
	match_temp = (uint16)(PWMC) * ((float)duty/PWM_DUTY_MAX);	//占空比
	// 在计数值为 match_temp 地方 pwmch 通道输出低电平
	(*(unsigned int  volatile xdata *)(PWMx_T1_BASE_ADDR + (pwmch & 0xF0))) = (uint16)match_temp;
//	// 在计数值为 match_temp 地方 pwmch 通道输出高电平
//	(*(unsigned int  volatile xdata *)(PWMx_T2_BASE_ADDR + (pwmch & 0xF0))) = (uint16)0;
	
	
	P_SW2 &= ~0x80;
}


void pwm_comple_duty(PWMCH_enum pwmch1, PWMCH_enum pwmch2, uint16 duty)
{
	uint16 match_temp;
	P_SW2 |= 0x80;
	
	match_temp = (uint16)(PWMC) * ((float)duty/PWM_DUTY_MAX);	//占空比
	// 在计数值为 match_temp 地方 pwmch 通道输出低电平
	(*(unsigned int  volatile xdata *)(PWMx_T1_BASE_ADDR + (pwmch1 & 0xF0))) = (uint16)match_temp;
	// 在计数值为 match_temp 地方 pwmch 通道输出高电平
	(*(unsigned int  volatile xdata *)(PWMx_T2_BASE_ADDR + (pwmch1 & 0xF0))) = (uint16)0;
	
	// 在计数值为 match_temp 地方 pwmch 通道输出低电平
	(*(unsigned int  volatile xdata *)(PWMx_T1_BASE_ADDR + (pwmch2 & 0xF0))) = (uint16)PWMC - 1;
	// 在计数值为 match_temp 地方 pwmch 通道输出高电平
	(*(unsigned int  volatile xdata *)(PWMx_T2_BASE_ADDR + (pwmch2 & 0xF0))) = (uint16)match_temp + 1;
	
	P_SW2 &= ~0x80;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM频率设置（所有的通道只能使用一个频率）
//  @param      freq        PWM频率
//  @param      duty        PWM占空比
//  @return     void
//  Sample usage:           pwm_freq(PWM0_P00, 50, 5000);     //修改化PWM0  使用引脚P0.0  输出PWM频率50HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//	@note					所有的PWM通道只能设置为一个频率
//-------------------------------------------------------------------------------------------------------------------
void pwm_freq(uint32 freq)
{
	uint16 period_temp; 
	uint8 freq_div = 0;
	P_SW2 |= 0x80;
	freq_div = (sys_clk / freq) >> 15;
	period_temp = sys_clk / freq / (freq_div + 1);					//周期

	//设置pwmn频率为 系统时钟/freq_div
	PWMCKS = freq_div;
	//设置PWMn周期为period_temp个 PWM 时钟
	PWMC = (uint16)period_temp; 
	
	P_SW2 &= ~0x80;
}


