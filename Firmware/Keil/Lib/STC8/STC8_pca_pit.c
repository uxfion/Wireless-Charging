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
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#include "STC8_pca_pit.h"
#include "board.h"

uint16 ccp_value[4] = {0,0,0,0};
uint16 pca_time_ms[4] = {0,0,0,0};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void pca_init(PCAN_enum pca_n,PCAMODE_enum pca_mode)
{

	CCON = 0;
	CMOD = 0x03<<1;	//ECI 脚的外部输入时钟
	
	if(PCA_1 == pca_n)
	{
		switch(pca_mode)
		{
			case NONE:
			{
				CCAPM0 = 0x00;
				break;
			}
			case PWM_MODE_N0_ISR:
			{
				CCAPM0 = 0x42;
				break;
			}
			case PWM_MODE_RISING_EDGE_ISR:
			{
				CCAPM0 = 0x63;
				break;
			}
			case PWM_MODE_FALLING_EDGE_ISR:
			{
				CCAPM0 = 0x53;
				break;
			}
			
			case PWM_MODE_BOTH_EDGE_ISR:
			{
				CCAPM0 = 0x73;
				break;
			}
			case RISING_EDGE_CAPTURE:
			{
				CCAPM0 = 0x20;
				break;
			}
			case FALLING_EDGE_CAPTURE:
			{
				CCAPM0 = 0x10;
				break;
			}
			case BOTH_EDGE_CAPTURE:
			{
				CCAPM0 = 0x30;
				break;
			}
			case SOFT_TIMER:
			{
				CCAPM0 = 0x49;
				CR = 1;
				break;
			}		
			case HIGH_SPEED_PULSE_OUTPUT:
			{
				CCAPM0 = 0x4C;
				break;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
uint16 pca_get(PCAN_enum pca_n)
{
	uint16 dat;
	if(PCA_1 == pca_n)
	{
		dat = (uint8)CCAP0H << 8;
		dat = ((uint8)CCAP0L) | dat;
	}
	
	return dat;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部使用用户无需关心
//  @param      NULL            	
//  @return     void
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void pca_clean(PCAN_enum pca_n)
{
	if(PCA_1 == pca_n)
	{
		CH = 0;
		CL = 0;
	}
}





//-------------------------------------------------------------------------------------------------------------------
//  @brief      周期中断初始化
//  @param      pca_n       pca通道号
//  @param      time        周期中断时间(1ms-45ms)
//  @return     void
//  Sample usage:     		pca_init_interrupt_ms(PCA_0, 1); 初始化1ms一次中断
//-------------------------------------------------------------------------------------------------------------------
void pca_init_interrupt_ms(PCAN_enum pca_n,uint16 time_ms)
{
	uint16 cr_count;
	CR = 0;				// 关闭 PCA 计时器
	cr_count = (CH<<8) | CL;
	if(PCA_0 == pca_n)
	{
		time_ms = (sys_clk / (12 * (1000 / time_ms)));
		pca_time_ms[0] = time_ms;
		CCAPM0 = 0x49;
		CCAP0L = CL;
		CCAP0H = CH;
		ccp_value[0] = ccp_value[0] + pca_time_ms[0] + cr_count;
	}
	if(PCA_1 == pca_n)
	{
		time_ms = (sys_clk / (12 * (1000 / time_ms)));
		pca_time_ms[1] = time_ms;
		CCAPM1 = 0x49;
		CCAP1L = CL;
		CCAP1H = CH;
		ccp_value[1] = ccp_value[1] + pca_time_ms[1] + cr_count;
	}

	if(PCA_2 == pca_n)	
	{
		time_ms = (sys_clk/(12*(1000/time_ms)));
		pca_time_ms[2] = time_ms;
		CCAPM2 = 0x49;
		CCAP2L = CL;
		CCAP2H = CH;
		ccp_value[2] = ccp_value[2] + pca_time_ms[2] + cr_count;
	}
//	PCA_3已做延时
//	if(PCA_3 == pca_n)	
//	{
//		time_ms = (sys_clk/(12*(1000/time_ms)));
//		pca_time_ms[3] = time_ms;
//		CCAPM3 = 0x49;
//		CCAP3L = ccp_value[3];
//		CCAP3H = ccp_value[3] >> 8;
//		ccp_value[3] += pca_time_ms[3];
//	}
	CR = 1;				// 启动 PCA 计时器
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      PCA重载计数器
//  @param      pca_n       pca通道号
//  @return     void
//  Sample usage:     		pca_init_interrupt_ms(PCA_0, 1); 初始化1ms一次中断
//-------------------------------------------------------------------------------------------------------------------
void pca_reload_counter(PCAN_enum pca_n)
{
	
	if(PCA_0 == pca_n)
	{
		//重载计数器
		CCAP0L = ccp_value[0];
		CCAP0H = ccp_value[0] >> 8;
		ccp_value[0] += pca_time_ms[0];
	}

	if(PCA_1 == pca_n)
	{
		//重载计数器
		CCAP1L = ccp_value[1];
		CCAP1H = ccp_value[1] >> 8;
		ccp_value[1] += pca_time_ms[1];
	}
	
	
	if(PCA_2 == pca_n)
	{
		//重载计数器
		CCAP2L = ccp_value[2];
		CCAP2H = ccp_value[2] >> 8;
		ccp_value[2] += pca_time_ms[2];
	}
	
	
	if(PCA_3 == pca_n)
	{
		//重载计数器
		CCAP3L = ccp_value[3];
		CCAP3H = ccp_value[3] >> 8;
		ccp_value[3] += pca_time_ms[3];
	}

}

