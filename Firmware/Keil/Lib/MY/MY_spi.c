/*!
 * @file		STC8_myspi.c
 * @brief		硬件SPI和软件SPI统一实现
 * @core		STC8A8K64S4A12-48
 * @author		HAOMING CHEN
 * @date		2020/03/14
 */

#include "MY_spi.h"

//----------------------------------------------------------------------
//  @brief		硬件SPI初始化
//  @param
//  @return
//  @usage		SPI_InitHardware();
//  @notice		移植直接更改函数内容以到达实现相同功能即可,
//  @notice		若不打算移植,直接把函数内容清空,工程内不要调用即可
//----------------------------------------------------------------------
void SPI_InitHardware(void)
{
	P_SW1 &= 0xf3;	//SPI引脚选择清零
	P_SW1 |= 0x04;	//选择第二组P25
	SPDAT = 0;
	SPSTAT = 0xc0;		//清除中断标志
	SPCTL = 0xdc;		//使能SPI主机模式
}

//----------------------------------------------------------------------
//  @brief		硬件SPI发送一格字节的数据硬件SPI发送一格字节的数据
//  @param		data1	需要发送的数据
//  @return		
//  @usage	       
//  @notice		移植直接更改函数内容以到达实现相同功能即可,
//  @notice		若不打算移植,直接把函数内容清空,工程内不要调用即可
//----------------------------------------------------------------------
void SPI_SendDataHardware(uint8 data1)
{
	SPSTAT = 0xc0;
	SPDAT = data1;
	while ((SPSTAT & 0x80)==0);
	SPSTAT = 0xc0;
}

//----------------------------------------------------------------------
//  @brief		软件SPI初始化,即初始化两个GPIO引脚
//  @param
//  @return
//  @usage
//  @notice		移植直接更改函数内容以到达实现相同功能即可
//----------------------------------------------------------------------
void SPI_InitSimulated(void)
{
	
}

//----------------------------------------------------------------------
//  @brief		软件SPI发送一格字节的数据,即设置GPIO电平变换来模拟SPI时序
//  @param		data1		需要发送的数据
//  @return
//  @usage
//  @notice		移植直接更改函数内容以到达实现相同功能即可
//----------------------------------------------------------------------
void SPI_SendDataSimulated(uint8 data1)
{
#define MYSPI_GPIO_SET_BITS(x)    x = 1 //为方便移植,可以直接更改宏定义实现相同功能
#define MYSPI_GPIO_RESET_BITS(x)  x = 0
	uint8_t temp = 8;
	MYSPI_GPIO_RESET_BITS(OLED_SCL_Pin);
	while (temp--)
	{
		MYSPI_GPIO_RESET_BITS(OLED_SCL_Pin);
		if (data1 & 0x80) MYSPI_GPIO_SET_BITS(OLED_SDA_Pin);
		else MYSPI_GPIO_RESET_BITS(OLED_SDA_Pin);
		MYSPI_GPIO_SET_BITS(OLED_SCL_Pin);
		data1 <<= 1;
	}
}
