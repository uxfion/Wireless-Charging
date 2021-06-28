/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		pit
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C51 V9.60MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __STC8_CPA_PIT_H
#define __STC8_CPA_PIT_H
#include "common.h"

//清除PCA标志位
#define PCA_CLEAR_FLAG(x)	(CCON &= ~(1<<x))

//获取PCA中断标志位
#define PCA0_GET_FLAG		CCF0
#define PCA1_GET_FLAG		CCF1
#define PCA2_GET_FLAG		CCF2
#define PCA3_GET_FLAG		CCF3

//此枚举定义不允许用户修改
typedef enum    // 枚举PCA通道
{
	PCA_0 = 0,
	PCA_1,
	PCA_2, 	
	PCA_3, 	//已被延时占用

}PCAN_enum;


//此枚举定义不允许用户修改
typedef enum    // 枚举PCA 模式
{
	NONE,							//无操作
	PWM_MODE_N0_ISR,				//6/7/8/10 位 PWM 模式，无中断
	PWM_MODE_RISING_EDGE_ISR,		//6/7/8/10 位 PWM 模式，产生上升沿中断
	PWM_MODE_FALLING_EDGE_ISR,		//6/7/8/10 位 PWM 模式，产生下降沿中断
	PWM_MODE_BOTH_EDGE_ISR,			//6/7/8/10 位 PWM 模式，产生边沿中断
	RISING_EDGE_CAPTURE,			//16 位上升沿捕获
	FALLING_EDGE_CAPTURE,			//16 位下降沿捕获
	BOTH_EDGE_CAPTURE,				//16 位边沿捕获
	SOFT_TIMER,						//16 位软件定时器
	HIGH_SPEED_PULSE_OUTPUT,		//16 为高速脉冲输出
}PCAMODE_enum;



extern uint16 ccp_value[4];
extern uint16 pca_time_ms[4];



//-------------------周期中断----------------------//
void pca_reload_counter(PCAN_enum pca_n);
void pca_init_interrupt_ms(PCAN_enum pca_n,uint16 time_ms);




#endif
