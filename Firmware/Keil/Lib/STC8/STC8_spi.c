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

#include "STC8_gpio.h"
#include "STC8_spi.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      spi初始化函数(ss引脚由软件控制)
//  @param      spi_n			选择SPI模块(SPI_1-SPI_4)
//  @param      sck_pin			选择SCK引脚
//  @param      mosi_pin		选择MOSI引脚
//  @param      miso_pin		选择MISO引脚
//  @param     	mstr			主从选择
//  @param      mode            SPI模式 0：CPOL=0 CPHA=0    1：CPOL=0 CPHA=1   2：CPOL=1 CPHA=0   3：CPOL=1 CPHA=1 //具体细节可自行百度
//  @param     	baud			速率选择
//  @since      v1.0
//  Sample usage:				spi_init(SPI_1,SPI1_SCLK_P15, SPI1_MOSI_P13,SPI1_MISO_P14,MASTER,SYSclk_DIV_4);	
//								//初始化SPI1,主机模式,速率为系统频率/4，SCLK引脚P1.5 MOSI引脚P1.3 MISO引脚P1.4
//-------------------------------------------------------------------------------------------------------------------
void spi_init(SPIN_enum spi_n,
			  SPI_PIN_enum sck_pin, 
			  SPI_PIN_enum mosi_pin, 
			  SPI_PIN_enum miso_pin, 
			  uint8 mode,
			  SPI_MSTR_enum mstr,
			  SPI_BAUD_enum baud)
{

	sck_pin = sck_pin;
	mosi_pin = mosi_pin;
	miso_pin = miso_pin;
	

	switch(spi_n)
    {
		//IO口需要配置为：准双向口（传统8051端口模式，弱上拉）
		case SPI_1:
			gpio_mode(P1_3, GPIO);
			gpio_mode(P1_4, GPIO);
			gpio_mode(P1_5, GPIO);
			break;
		case SPI_2:
			gpio_mode(P2_3, GPIO);
			gpio_mode(P2_4, GPIO);
			gpio_mode(P2_5, GPIO);
			break;
		case SPI_3:
			gpio_mode(P7_7, GPIO);
			gpio_mode(P7_6, GPIO);
			gpio_mode(P7_5, GPIO);
			break;
		case SPI_4:
			gpio_mode(P3_4, GPIO);
			gpio_mode(P3_3, GPIO);
			gpio_mode(P3_2, GPIO);
			break;
    }
	
	P_SW1 &= ~(0x03<<2);  //清除SPI功能脚选择位
    switch(spi_n)
    {
    case SPI_1:
        P_SW1 |= (0x00<<2);	//SS/P1.2, MOSI/P1.3, MISO/P1.4, SCLK/P1.5
        break;
    case SPI_2:
        P_SW1 |= (0x01<<2);	//SS_2/P2.2, MOSI_2/P2.3, MISO_2/P2.4, SCLK_2/P2.5
        break;
    case SPI_3:
        P_SW1 |= (0x02<<2);	//SS_3/P5.4, MOSI_3/P4.0, MISO_3/P4.1, SCLK_3/P4.3
        break;
    case SPI_4:
        P_SW1 |= (0x03<<2); //SS_4/P3.5, MOSI_4/P3.4, MISO_4/P3.3, SCLK_4/P3.2
        break;
    }
	
	SPCTL &= 0xF3;		//清除SPI极性和相位
	switch(mode)
	{
	case 0:
		
		break;
	case 1:
		SPCTL |= 0x01<<2;
		break;
	case 2:
		SPCTL |= 0x02<<2;
		break;
	case 3:
		SPCTL |= 0x03<<2;
		break;
	}
		
	
	SPCTL |= baud;		//速率设定

    if(mstr == MASTER)
    {
        SPCTL |= 1<<7;	//忽略SS引脚功能，使用MSTR确定器件是主机还是从机
        SPCTL |= 1<<4;	//主机模式
    }
    else
    {
        //不做操作
    }
    SPCTL |= 1<<6;		//使能SPI功能
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI发送接收函数
//  @param      dat          	发送的数据
//  @since      v1.0
//  Sample usage:				buf_1 = spi_mosi(SPI_1,1,buf);    //发送buff的内容，并接收到buf_1里，长度为1字节
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_mosi(uint8 dat)
{
    SPDAT = dat;					//DATA寄存器赋值
    while (!(SPSTAT & 0x80));  		//查询完成标志
    SPSTAT = 0xc0;                  //清中断标志
	return SPDAT;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      spi引脚切换函数(ss引脚由软件控制)
//  @param      spi_n			选择SPI模块(SPI_1-SPI_4)
//  @param      sck_pin			选择SCK引脚
//  @param      mosi_pin		选择MOSI引脚
//  @param      miso_pin		选择MISO引脚
//  Sample usage:				spi_change_pin(SPI_1,SPI1_SCLK_P15, SPI1_MOSI_P13,SPI1_MISO_P14);	
//								//切换SPI引脚
//-------------------------------------------------------------------------------------------------------------------
void spi_change_pin(SPIN_enum spi_n, SPI_PIN_enum sck_pin, SPI_PIN_enum mosi_pin, SPI_PIN_enum miso_pin)
{
	sck_pin = sck_pin;
	mosi_pin = mosi_pin;
	miso_pin = miso_pin;
	
	switch(spi_n)
    {
		//IO口需要配置为：准双向口（传统8051端口模式，弱上拉）
		case SPI_1:
			gpio_mode(P1_3, GPIO);
			gpio_mode(P1_4, GPIO);
			gpio_mode(P1_5, GPIO);
			gpio_pull_set(P1_3, PULLUP);
			gpio_pull_set(P1_4, NOPULL);
			gpio_pull_set(P1_5, PULLUP);
			break;
		case SPI_2:
			gpio_mode(P2_3, GPIO);
			gpio_mode(P2_4, GPIO);
			gpio_mode(P2_5, GPIO);
			gpio_pull_set(P2_3, PULLUP);
			gpio_pull_set(P2_4, NOPULL);
			gpio_pull_set(P2_5, PULLUP);
			break;
		case SPI_3:
			gpio_mode(P7_7, GPIO);
			gpio_mode(P7_6, GPIO);
			gpio_mode(P7_5, GPIO);
			gpio_pull_set(P7_7,PULLUP);
			gpio_pull_set(P7_6,NOPULL);
			gpio_pull_set(P7_5,PULLUP);
			break;
		case SPI_4:
			gpio_mode(P3_4, GPIO);
			gpio_mode(P3_3, GPIO);
			gpio_mode(P3_2, GPIO);
			gpio_pull_set(P3_4,PULLUP);
			gpio_pull_set(P3_3,NOPULL);
			gpio_pull_set(P3_2,PULLUP);
			break;
    }
	
	P_SW1 &= ~(0x03<<2);  //清除SPI功能脚选择位
    switch(spi_n)
    {
		case SPI_1:
			P_SW1 |= (0x00<<2);	//MOSI/P1.3, MISO/P1.4, SCLK/P1.5
			break;
		case SPI_2:
			P_SW1 |= (0x01<<2);	//MOSI_2/P2.3, MISO_2/P2.4, SCLK_2/P2.5
			break;
		case SPI_3:
			P_SW1 |= (0x02<<2);	//MOSI_3/P4.0, MISO_3/P4.1, SCLK_3/P4.3
			break;
		case SPI_4:
			P_SW1 |= (0x03<<2); //MOSI_4/P3.4, MISO_4/P3.3, SCLK_4/P3.2
			break;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      spi模式切换函数
//  @param      mode            SPI模式 0：CPOL=0 CPHA=0    1：CPOL=0 CPHA=1   2：CPOL=1 CPHA=0   3：CPOL=1 CPHA=1 //具体细节可自行百度
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void spi_change_mode(uint8 mode)
{
	SPCTL &= 0xF7;		//清除SPI极性和相位
	switch(mode)
	{
	case 0:
		
		break;
	case 1:
		SPCTL |= 0x01<<2;
		break;
	case 2:
		SPCTL |= 0x02<<2;
		break;
	case 3:
		SPCTL |= 0x03<<2;
		break;
	}
}