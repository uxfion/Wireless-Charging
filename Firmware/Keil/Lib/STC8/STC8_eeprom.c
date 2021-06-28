/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		eeprom
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#include "STC8_eeprom.h"
#include "board.h"
#include "intrins.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化EEPROM
//  @param      NULL
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void iap_init(void)
{
	IAP_CONTR |= 1<<7;	 //使能EEPROM操作
	iap_set_tps();		 //设置等待时间
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      关闭EEPROM
//  @param      NULL
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void iap_idle(void)
{
	IAP_CONTR &= ~(1<<7);//失能EEPROM操作
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取EEPROM操作失败状态位，需要软件清零
//  @param      NULL
//  @return     void
//  Sample usage:           
//								操作失败返回1;
//-------------------------------------------------------------------------------------------------------------------
uint8 iap_get_cmd_state(void)
{
	return ((IAP_CONTR&0x01) == 0x01);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置IAP等待时间
//  @param      NULL
//  @return     void
//  Sample usage:           
//-------------------------------------------------------------------------------------------------------------------
void iap_set_tps(void)
{
	IAP_CONTR &= (~0x07);
	if(sys_clk >= (30*1000*1000))
	{
		IAP_CONTR |= 0x00;
	}
	else if((sys_clk <= (30*1000*1000)) && (sys_clk >= (24*1000*1000)))
	{
		IAP_CONTR |= 0x01;
	}
	else if((sys_clk <= (24*1000*1000)) && (sys_clk >= (20*1000*1000)))
	{
		IAP_CONTR |= 0x02;
	}
	else if((sys_clk <= (20*1000*1000)) && (sys_clk >= (12*1000*1000)))
	{
		IAP_CONTR |= 0x03;
	}
	else if((sys_clk <= (12*1000*1000)) && (sys_clk >= (6*1000*1000)))
	{
		IAP_CONTR |= 0x04;
	}
	else if((sys_clk <= (6*1000*1000)) && (sys_clk >= (3*1000*1000)))
	{
		IAP_CONTR |= 0x05;
	}
	else if((sys_clk <= (3*1000*1000)) && (sys_clk >= (2*1000*1000)))
	{
		IAP_CONTR |= 0x06;
	}
	else if((sys_clk <= (2*1000*1000)) && (sys_clk >= (1*1000*1000)))
	{
		IAP_CONTR |= 0x07;
	}

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      EEPROM读取多个字节
//  @param      addr			需要读取的eeprom地址
//  @param      *buf			需要读取的数据地址
//  @param      len				需要读取的数据长度
//  @return     void
//  Sample usage:               uint8 str[10];
//								iap_read_bytes(0x00,str,10);
//								将0x00-0x0A地址中的数据，读取到str中。
//-------------------------------------------------------------------------------------------------------------------
void iap_read_bytes(uint16 addr, uint8 *buf, uint8 len)
{

	IAP_CMD = 1; 				//设置 IAP 读命令	

	while(len--)
	{
		IAP_ADDRL = addr; 		//设置 IAP 低地址
		IAP_ADDRH = addr >> 8; 	//设置 IAP 高地址
		IAP_TRIG = 0x5a; 		//写触发命令(0x5a)
		IAP_TRIG = 0xa5; 		//写触发命令(0xa5)	
		*buf++ = IAP_DATA; 		//读 IAP 数据
		addr++;
		
		_nop_(); 
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      EEPROM写多个字节
//  @param      addr			需要写的eeprom地址
//  @param      *buf			需要写的数据地址
//  @param      len				需要写的数据长度
//  @return     void
//  Sample usage:       		iap_write_bytes(0x00,(uint8 *)"0123456789";,10);
//								将"0123456789"写入0x00-0x0A地址中;
//-------------------------------------------------------------------------------------------------------------------
void iap_write_bytes(uint16 addr, uint8 *buf, uint8 len)
{

	IAP_CMD = 2; 				//设置 IAP 读命令	
	
	while(len--)
	{
		IAP_ADDRL = addr; 		//设置 IAP 低地址
		IAP_ADDRH = addr >> 8; 	//设置 IAP 高地址
		IAP_DATA = *buf++; 		//写 IAP 数据
		addr++;

		IAP_TRIG = 0x5a; 		//写触发命令(0x5a)
		IAP_TRIG = 0xa5; 		//写触发命令(0xa5)
		_nop_(); 
	}
}

void iap_write_byte(uint16 addr, uint8 buf)
{
	IAP_CMD = 2;
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	IAP_DATA = buf;
	IAP_TRIG = 0x5a; 		//写触发命令(0x5a)
	IAP_TRIG = 0xa5; 		//写触发命令(0xa5)
	_nop_(); 
}

uint8 iap_read_byte(uint16 addr)
{
	IAP_CMD = 1; 				//设置 IAP 读命令	
	IAP_ADDRL = addr; 		//设置 IAP 低地址
	IAP_ADDRH = addr >> 8; 	//设置 IAP 高地址
	IAP_TRIG = 0x5a; 		//写触发命令(0x5a)
	IAP_TRIG = 0xa5; 		//写触发命令(0xa5)	
	_nop_();
	return IAP_DATA;		//读 IAP 数据
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      EEPROM擦除目标地址所在的一页（1扇区/512字节）
//  @param      addr			需要写的eeprom地址
//  @return     void
//  Sample usage:       		iap_erase(0x20);
//								擦除0x00-0x200的数据
//-------------------------------------------------------------------------------------------------------------------
void iap_erase_page(uint16 addr) 
{ 

	IAP_CMD = 3; 				//设置 IAP 擦除命令

	IAP_ADDRL = addr; 			//设置 IAP 低地址
	IAP_ADDRH = addr >> 8;  	//设置 IAP 高地址
	IAP_TRIG = 0x5a; 			//写触发命令(0x5a)
	IAP_TRIG = 0xa5; 			//写触发命令(0xa5)
	_nop_(); 

}




