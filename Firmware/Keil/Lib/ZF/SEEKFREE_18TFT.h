/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		TFT
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.26
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note		
					接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SCL                 查看TFT_SCL_PIN		宏定义的引脚     硬件SPI引脚不可随意切换
						SDA                 查看TFT_SDA_PIN		宏定义的引脚     硬件SPI引脚不可随意切换
						RES                 查看REST_PIN		宏定义的引脚    
						DC                  查看DC_PIN_PIN		宏定义的引脚  
						CS                  查看TFT_CS_PIN		宏定义的引脚      
						
						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
						最大分辨率128*160
					------------------------------------ 

 ********************************************************************************************************************/



#ifndef _SEEKFREE_18TFT_h
#define _SEEKFREE_18TFT_h
#include "common.h"

#define TFT_SPIN SPI_2				 	//定义使用的SPI号
#define	TFT_SCL_PIN 	SPI2_SCLK_P25	//定义SPI_SCK引脚
#define	TFT_SDA_PIN		SPI2_MOSI_P23   //定义SPI_MOSI引脚
#define	TFT_SDA_IN_PIN	SPI2_MISO_P24   //定义SPI_MISO引脚  TFT屏幕没有MISO引脚，但是这里任然需要定义，在spi的初始化时需要使用

#define TFT_REST_PIN  	P20
#define TFT_DC_PIN	  	P21
#define TFT_CS_PIN	  	P22			//定义SPI_CS引脚
#define TFT_BL_PIN	  	P24


#define TFT_REST(x) (TFT_REST_PIN= x)	
#define TFT_DC(x)   (TFT_DC_PIN	 = x)
#define TFT_CS(x) 	(TFT_CS_PIN	 = x)
#define TFT_BL(x) 	(TFT_BL_PIN	 = x)




//-------常用颜色在SEEKFREE_FONT.h文件中定义----------
//#define RED          	    0xF800	//红色
//#define BLUE         	    0x001F  //蓝色
//#define YELLOW       	    0xFFE0	//黄色
//#define GREEN        	    0x07E0	//绿色
//#define WHITE        	    0xFFFF	//白色
//#define BLACK        	    0x0000	//黑色 
//#define GRAY  			0X8430 	//灰色
//#define BROWN 			0XBC40 	//棕色
//#define PURPLE    		0XF81F	//紫色
//#define PINK    		    0XFE19	//粉色



//定义写字笔的颜色
#define PENCOLOR    RED

//定义背景颜色
#define BGCOLOR	    WHITE

     
     
//定义显示方向
//0 竖屏模式
//1 竖屏模式  旋转180
//2 横屏模式
//3 横屏模式  旋转180
#define TFT_DISPLAY_DIR 0

#if (0==TFT_DISPLAY_DIR || 1==TFT_DISPLAY_DIR)
#define	TFT_X_MAX	128	//液晶X方宽度
#define TFT_Y_MAX	160 //液晶Y方宽度
     
#elif (2==TFT_DISPLAY_DIR || 3==TFT_DISPLAY_DIR)
#define	TFT_X_MAX	160	//液晶X方宽度
#define TFT_Y_MAX	128 //液晶Y方宽度
     
#else
#error "TFT_DISPLAY_DIR 定义错误"
     
#endif


void lcd_init(void);
void lcd_clear(int color);
void lcd_drawpoint(uint16 x,uint16 y,uint16 color);
void lcd_showchar(uint16 x,uint16 y,uint8 dat);
void lcd_showstr(uint16 x,uint16 y,uint8 dat[]);
void lcd_showint8(uint16 x,uint16 y,int8 dat);
void lcd_showuint8(uint16 x,uint16 y,uint8 dat);
void lcd_showint16(uint16 x,uint16 y,int16 dat);
void lcd_showuint16(uint16 x,uint16 y,uint16 dat);
void lcd_showint32(uint16 x,uint16 y,int32 dat,uint8 num);
void lcd_showfloat(uint16 x,uint16 y,double dat,uint8 num,uint8 pointnum);
void showimage(const unsigned char *p);


#endif
