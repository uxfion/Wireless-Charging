/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		spi
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __STC8_SPI_H
#define __STC8_SPI_H
#include "common.h"




//该枚举体禁止用户修改
typedef enum
{
	SPI_1,
	SPI_2,
	SPI_3,
	SPI_4,
} SPIN_enum;


//该枚举体禁止用户修改
typedef enum
{
	SPI1_SCLK_P15 = 0x00,SPI1_MOSI_P13,SPI1_MISO_P14, //SPI只能使用同一组引脚，不允许混用引脚
	
	SPI2_SCLK_P25 = 0x10,SPI2_MOSI_P23,SPI2_MISO_P24, //SPI只能使用同一组引脚，不允许混用引脚
	
	SPI3_SCLK_P77 = 0x20,SPI3_MOSI_P75,SPI3_MISO_P76, //SPI只能使用同一组引脚，不允许混用引脚
	
	SPI4_SCLK_P32 = 0x30,SPI4_MOSI_P34,SPI4_MISO_P33, //SPI只能使用同一组引脚，不允许混用引脚
	
	//其中CS引脚由软件控制
} SPI_PIN_enum;



//该枚举体禁止用户修改
typedef enum
{
    MASTER = 0,	 //主机
    SLAVE, //从机
} SPI_MSTR_enum;

//该枚举体禁止用户修改
typedef enum
{
	SPI_SYSclk_DIV_4 = 0,
	SPI_SYSclk_DIV_8,
	SPI_SYSclk_DIV_16,
	SPI_SYSclk_DIV_32,
}SPI_BAUD_enum;




void spi_init(SPIN_enum spi_n,
			  SPI_PIN_enum sck_pin, 
			  SPI_PIN_enum mosi_pin, 
			  SPI_PIN_enum miso_pin, 
			  uint8 mode,
			  SPI_MSTR_enum mstr,
			  SPI_BAUD_enum baud);

void spi_change_pin(SPIN_enum spi_n, 
					SPI_PIN_enum sck_pin, 
					SPI_PIN_enum mosi_pin, 
					SPI_PIN_enum miso_pin);

void spi_change_mode(uint8 mode);

uint8 spi_mosi(uint8 dat);




#endif


