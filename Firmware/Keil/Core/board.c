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
 
 
#include "board.h"
#include "STC8_pca_delay.h"
#include "STC8_uart.h"
#include "STC8_tim.h"
#include "headfile.h"

//IRC 频率调整寄存器
sfr IRTRIM      =   0x9F;
//24MHz的IRC参数寄存器 0xFB
#define IRC_24M (*((uint8  idata*)0xFB))

//系统频率变量
uint32 sys_clk = 0;
	
	
//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取系统频率
//  @param      NULL          	空值
//  @return     void        	系统频率
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
uint32 get_clk(void)
{
	uint32 temp_count;

	P_SW2 |= 0x80;

	temp_count = 24000000UL + ((IRTRIM - IRC_24M) * 0xD2F0UL);  //频率的偏差,计算出大概数据
	temp_count /= CLKDIV;                              	  		//频率太低需要分频
	
	if 	(temp_count < 5764800UL)
		return 5529600UL;
	else if(temp_count < 8529600UL)
		return 6000000UL;
	else if(temp_count < 11529600UL)
		return 11059200UL;
	else if(temp_count < 15216000UL)
		return  12000000UL;
	else if(temp_count < 19216000UL)
		return  18432000UL;
	else if(temp_count < 21059200UL)
		return  20000000UL;
	else if(temp_count < 23059200UL)
		return  22118400UL;
	else if(temp_count < 25500000UL)
		return  24000000UL;
	else if(temp_count < 28500000UL)
		return  27000000UL;
	else if(temp_count < 31500000UL)
		return  30000000UL;
	else if(temp_count < 33500000UL)
		return  33177600UL;
	else
		return  35000000UL;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化内部寄存器
//  @param      NULL          	空值
//  @return     void        	系统频率
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void board_init(void)
{
//获取系统频率
#if (0 == FOSC)
	sys_clk = get_clk();
#else
	sys_clk = FOSC;
#endif

#if (1 == EXTERNAL_CRYSTA_ENABLE)
	P_SW2 = 0x80;
	XOSCCR = 0xc0; 			// 启动外部晶振
	while (!(XOSCCR & 1)); 	// 等待时钟稳定
	CLKDIV = 0x00; 			// 时钟不分频
	CKSEL = 0x01; 			// 选择外部晶振
	P_SW2 = 0x00;
#endif
	
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M0 = 0x00;
	P1M1 = 0x00;
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	P6M0 = 0x00;
	P6M1 = 0x00;
	P7M0 = 0x00;
	P7M1 = 0x00;
	CCON = 0x00;
	CMOD = 0x00;			//PCA 时钟为系统时钟 /12
	ADCCFG = 0;
	AUXR = 0;
	SCON = 0;
	S2CON = 0;
	S3CON = 0;
	S4CON = 0;
	P_SW1 = 0;
	P_SW2 = 0;
	IE2 = 0;
	TMOD = 0;
	CL = 0;
	CH = 0;
	pca_dealy_init();		//PCA延时函数初始化
	//DEBUG串口初始化
	uart_init(DEBUG_UART, DEBUG_UART_RX_PIN, DEBUG_UART_TX_PIN, DEBUG_UART_BAUD, DEBUG_UART_TIM);
}


#if (1 == PRINTF_ENABLE)      //初始化调试串口
//重定义printf 数字 只能输出uint16
char putchar(char c)
{
	uart_putchar(UART_1,(uint8)c);//把自己实现的串口打印一字节数据的函数替换到这里
	return c;
}
#endif


//-------------------------------------------------------------------------------------------------------------------
//  @brief      失能外部中断
//  @param      NULL          	
//  @return     NULL        	
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void DisableGlobalIRQ(void)
{
	EA = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      使能外部中断
//  @param      NULL          	
//  @return     NULL        	
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void EnableGlobalIRQ(void)
{
	EA = 1;
}

