/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		apic(中断优先级管理)
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __STC8_NVIC_H
#define __STC8_NVIC_H

#include "common.h"

//该枚举体禁止用户修改
//中断优先级控制枚举体
typedef enum
{
	INT0_IRQn = 0x00,
	TIMER0_IRQn,
	INT1_IRQn,
	TIMER1_IRQn,
	UART1_IRQn,
	ADC_IRQn,
	LVD_IRQn,			//低压检测中断
	CCP_PCA_PWM_IRQn,	//CCP/PCA/PWM 中断
	
	UART2_IRQn = 0x10,
	SPI_IRQn,
	PWM_IRQn,
	PWMFD_IRQn,
	INT4_IRQn,
	CMP_IRQn,
	IIC_IRQn,
	
//	STC8A没有这一些位
//	PWM2FD_PTKSU_IRQn,	//增强型 PWM2 异常检测中断 和 触摸按键中断
//	UART3_IRQn = 0x20,
//	UART4_IRQn,
//	PWM1_IRQn,
//	PWM2_IRQn,
//	PWM3_IRQn,
//	PWM4_IRQn,
//	PWM5_IRQn,
//	PWM4FD_IRQn,

}NVIC_IRQn_enum;


void NVIC_SetPriority(NVIC_IRQn_enum irqn,uint8 priority);



#endif