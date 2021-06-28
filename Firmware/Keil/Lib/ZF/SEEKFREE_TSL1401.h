/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 * @note	
					TSL1401接线定义：
					------------------------------------ 
					    模块管脚       	单片机管脚
					    CCD_AO	   		查看SEEKFREE_TSL1401.h文件中的AD_CHANNEL 宏定义
					    CCD_CLK	   		查看SEEKFREE_TSL1401.h文件中的CCD_CLK_PIN宏定义
					    CCD_SI	   		查看SEEKFREE_TSL1401.h文件中的CCD_SI_PIN 宏定义
					------------------------------------ 
 ********************************************************************************************************************/


#ifndef _TSL1401_c
#define _TSL1401_c
#include "common.h"
#include "STC8_uart.h"




#define CCD_CLK_PIN   	P50               	//CCD的CLK引脚定义
#define CCD_SI_PIN    	P51               	//CCD的SI 引脚定义
#define AD_CHANNEL 		ADC_P13             //定义AD采集通道 ADC_P13
#define EXPOSURE_TIME  	10                  //定义CCD曝光时间 单位MS


#define CCD_CLK(x)     CCD_CLK_PIN = x 
#define CCD_SI(x)      CCD_SI_PIN  = x


extern uint16 ccd_data[128];                   //CCD数据
extern uint8 tsl1401_finish_flag;

void ccd_init(void);
void ccd_collect(void);
void ccd_send_data(UARTN_enum uart_n ,uint16 *dat);

#endif