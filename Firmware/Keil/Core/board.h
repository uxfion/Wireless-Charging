/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		board
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/



#ifndef __BOARD_H
#define __BOARD_H
#include "common.h"

#define FOSC          24000000	//FOSC的值设置为24Mhz
//#define FOSC           0			//FOSC的值设置为0，则自动读取系统频率
									//FOSC的值设置不为0，则系统频率为FOSC的值

#define EXTERNAL_CRYSTA_ENABLE  0	//使用外部晶振
#define PRINTF_ENABLE		    1	//printf使能
#define ENABLE_IAP 			    1	//使能软件一键下载功能


#define DEBUG_UART 			  	UART_1
#define DEBUG_UART_BAUD 	  	115200
#define DEBUG_UART_RX_PIN  		UART1_RX_P30
#define DEBUG_UART_TX_PIN  		UART1_TX_P31
#define DEBUG_UART_TIM			TIM_2

#if (1==PRINTF_ENABLE)
	char putchar(char c);
#endif

extern uint32 sys_clk;

uint32 get_clk(void);

void board_init(void);
void DisableGlobalIRQ(void);
void EnableGlobalIRQ(void);

#endif

