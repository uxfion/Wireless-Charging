/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-11-15
 * @note		
					接线定义：
					------------------------------------ 
					1.14寸IPS模块管脚       单片机管脚
					SCL                 	查看SEEKFREE_IPS114_SPI.h文件内的IPS114_SCL		宏定义     硬件SPI引脚不可随意切换
					SDA                 	查看SEEKFREE_IPS114_SPI.h文件内的IPS114_SDA		宏定义     硬件SPI引脚不可随意切换
					RES                 	查看SEEKFREE_IPS114_SPI.h文件内的IPS114_REST_PIN宏定义    
					DC                  	查看SEEKFREE_IPS114_SPI.h文件内的IPS114_DC_PIN	宏定义  
					CS                  	查看SEEKFREE_IPS114_SPI.h文件内的IPS114_CS		宏定义     
					BL  					查看SEEKFREE_IPS114_SPI.h文件内的IPS114_BL_PIN	宏定义
					
					电源引脚
					VCC 3.3V电源
					GND 电源地
					最大分辨率135*240
					------------------------------------ 
 ********************************************************************************************************************/
 



#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_PRINTF.h"
#include "STC8_spi.h"
#include "STC8_pca_delay.h"




//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips114_writeIndex(uint8 dat)			//写命令
{	
	IPS114_CS(0);
    IPS114_DC(0);

	spi_mosi(dat);
	IPS114_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips114_writeData(uint8 dat)			//写数据
{
	IPS114_CS(0);
    IPS114_DC(1);

	spi_mosi(dat);
	IPS114_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void 		    
//  @return     				
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips114_writedata_16bit(uint16 dat)	//向液晶屏写一个16位数据
{
    uint8 dat1[2];
    dat1[0] = dat >> 8;
    dat1[1] = (uint8)dat;
	IPS114_CS(0);
    IPS114_DC(1);
	spi_mosi(dat1[0]);	//写入数据  高位在前  低位在后
	spi_mosi(dat1[1]);
	IPS114_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      1.14寸 IPS液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void ips114_init(void)
{
    spi_init(IPS114_SPIN_PIN, IPS114_SCL_PIN, IPS114_SDA_PIN, IPS114_SDA_IN_PIN, 0, MASTER, SPI_SYSclk_DIV_4);

	IPS114_BL(1);
	IPS114_DC(0);
	IPS114_REST(0);
	
    pca_delay_ms(200);
    
    IPS114_REST(1);
    pca_delay_ms(100);
	
    ips114_writeIndex(0x36);
    
    if      (IPS114_DISPLAY_DIR==0)    ips114_writeData(0x00);
    else if (IPS114_DISPLAY_DIR==1)    ips114_writeData(0xC0);
    else if (IPS114_DISPLAY_DIR==2)    ips114_writeData(0x70);
    else                            ips114_writeData(0xA0);
    
    ips114_writeIndex(0x3A); 
    ips114_writeData(0x05);

    ips114_writeIndex(0xB2);
    ips114_writeData(0x0C);
    ips114_writeData(0x0C);
    ips114_writeData(0x00);
    ips114_writeData(0x33);
    ips114_writeData(0x33); 

    ips114_writeIndex(0xB7); 
    ips114_writeData(0x35);  

    ips114_writeIndex(0xBB);
    ips114_writeData(0x37);

    ips114_writeIndex(0xC0);
    ips114_writeData(0x2C);

    ips114_writeIndex(0xC2);
    ips114_writeData(0x01);

    ips114_writeIndex(0xC3);
    ips114_writeData(0x12);   

    ips114_writeIndex(0xC4);
    ips114_writeData(0x20);  

    ips114_writeIndex(0xC6); 
    ips114_writeData(0x0F);    

    ips114_writeIndex(0xD0); 
    ips114_writeData(0xA4);
    ips114_writeData(0xA1);

    ips114_writeIndex(0xE0);
    ips114_writeData(0xD0);
    ips114_writeData(0x04);
    ips114_writeData(0x0D);
    ips114_writeData(0x11);
    ips114_writeData(0x13);
    ips114_writeData(0x2B);
    ips114_writeData(0x3F);
    ips114_writeData(0x54);
    ips114_writeData(0x4C);
    ips114_writeData(0x18);
    ips114_writeData(0x0D);
    ips114_writeData(0x0B);
    ips114_writeData(0x1F);
    ips114_writeData(0x23);

    ips114_writeIndex(0xE1);
    ips114_writeData(0xD0);
    ips114_writeData(0x04);
    ips114_writeData(0x0C);
    ips114_writeData(0x11);
    ips114_writeData(0x13);
    ips114_writeData(0x2C);
    ips114_writeData(0x3F);
    ips114_writeData(0x44);
    ips114_writeData(0x51);
    ips114_writeData(0x2F);
    ips114_writeData(0x1F);
    ips114_writeData(0x1F);
    ips114_writeData(0x20);
    ips114_writeData(0x23);

    ips114_writeIndex(0x21); 

    ips114_writeIndex(0x11); 
    pca_delay_ms(120); 

    ips114_writeIndex(0x29);
    ips114_clear(IPS114_BGCOLOR);
}



void ips114_set_region(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	#if(IPS114_DISPLAY_DIR==0)
	{
		ips114_writeIndex(0x2a);//列地址设置
		ips114_writedata_16bit(x1+52);
		ips114_writedata_16bit(x2+52);
		ips114_writeIndex(0x2b);//行地址设置
		ips114_writedata_16bit(y1+40);
		ips114_writedata_16bit(y2+40);
		ips114_writeIndex(0x2c);//储存器写
	}
	#elif(IPS114_DISPLAY_DIR==1)
	{
		ips114_writeIndex(0x2a);//列地址设置
		ips114_writedata_16bit(x1+53);
		ips114_writedata_16bit(x2+53);
		ips114_writeIndex(0x2b);//行地址设置
		ips114_writedata_16bit(y1+40);
		ips114_writedata_16bit(y2+40);
		ips114_writeIndex(0x2c);//储存器写
	}
	#elif(IPS114_DISPLAY_DIR==2)
	{
		ips114_writeIndex(0x2a);//列地址设置
		ips114_writedata_16bit(x1+40);
		ips114_writedata_16bit(x2+40);
		ips114_writeIndex(0x2b);//行地址设置
		ips114_writedata_16bit(y1+53);
		ips114_writedata_16bit(y2+53);
		ips114_writeIndex(0x2c);//储存器写
	}
	#else
	{
		ips114_writeIndex(0x2a);//列地址设置
		ips114_writedata_16bit(x1+40);
		ips114_writedata_16bit(x2+40);
		ips114_writeIndex(0x2b);//行地址设置
		ips114_writedata_16bit(y1+52);
		ips114_writedata_16bit(y2+52);
		ips114_writeIndex(0x2c);//储存器写
	}
	#endif
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color     		颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_clear(YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void ips114_clear(uint16 color)
{
    uint16 i,j;
	ips114_set_region(0,0,IPS114_X_MAX-1,IPS114_Y_MAX-1);
    for(i=0;i<IPS114_X_MAX;i++)
    {
        for (j=0;j<IPS114_Y_MAX;j++)
	   	{
        	ips114_writedata_16bit(color);	 			 
	    }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶画点
//  @param      x     	        坐标x方向的起点
//  @param      y     	        坐标y方向的起点
//  @param      dat       	    需要显示的颜色
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_drawpoint(0,0,RED);  //坐标0,0画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void ips114_drawpoint(uint16 x,uint16 y,uint16 color)
{
    ips114_set_region(x,y,x,y);
    ips114_writedata_16bit(color);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS114_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS114_Y_MAX/16-1）
//  @param      dat       	    需要显示的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showchar(0,0,'x');//坐标0,0写一个字符x
//-------------------------------------------------------------------------------------------------------------------
void ips114_showchar(uint16 x,uint16 y,const int8 dat)
{
	uint8 i,j;
	uint8 temp;
    
	for(i=0; i<16; i++)
	{
		ips114_set_region(x,y+i,x+7,y+i);
		temp = tft_ascii[dat-32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)	
            {
                ips114_writedata_16bit(IPS114_PENCOLOR);
            }
			else			ips114_writedata_16bit(IPS114_BGCOLOR);
			temp>>=1;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符串
//  @param      x     	        坐标x方向的起点  参数范围 0 -（IPS114_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（IPS114_Y_MAX/16-1）
//  @param      dat       	    需要显示的字符串
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips114_showstr(uint16 x,uint16 y,const int8 dat[])
{
	uint16 j;
	
	j = 0;
	while(dat[j] != '\0')
	{
		ips114_showchar(x+8*j,y*16,dat[j]);
		j++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位有符号
//  @param      x     	        坐标x方向的起点  参数范围 0 -（IPS114_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（IPS114_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型int8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showint8(0,0,x);//x为int8类型
//-------------------------------------------------------------------------------------------------------------------
void ips114_showint8(uint16 x,uint16 y,int8 dat)
{
	uint8 a[3];
	uint8 i;
	if(dat<0)
	{
		ips114_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	ips114_showchar(x,y*16,' ');
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		ips114_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS114_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS114_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showuint8(0,0,x);//x为uint8类型
//-------------------------------------------------------------------------------------------------------------------
void ips114_showuint8(uint16 x,uint16 y,uint8 dat)
{
	uint8 a[3];
	uint8 i;
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		ips114_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位有符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS114_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS114_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型int16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showint16(0,0,x);//x为int16类型
//-------------------------------------------------------------------------------------------------------------------
void ips114_showint16(uint16 x,uint16 y,int16 dat)
{
	uint8 a[5];
	uint8 i;
	if(dat<0)
	{
		ips114_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	ips114_showchar(x,y*16,' ');

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		ips114_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS114_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS114_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showuint16(0,0,x);//x为uint16类型
//-------------------------------------------------------------------------------------------------------------------
void ips114_showuint16(uint16 x,uint16 y,uint16 dat)
{
	uint8 a[5];
	uint8 i;

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		ips114_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示32位有符号(去除整数部分无效的0)
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS114_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS114_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint32
//  @param      num       	    需要显示的位数 最高10位  不包含正负号
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showuint32(0,0,x,3);//x可以为int32 uint16 int16 uint8 int8类型
//  Sample usage:               负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void ips114_showint32(uint16 x,uint16 y,int32 dat,uint8 num)
{
    int8    buff[34];
    uint8   length;
    
    if(10<num)      num = 10;
    
    num++;
    if(0>dat)   length = zf_sprintf( &buff[0],"%d",dat);//负数
    else
    {
        buff[0] = ' ';
        length = zf_sprintf( &buff[1],"%d",dat);
        length++;
    }
    while(length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    ips114_showstr(x, y, buff);	//显示数字
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示浮点数(去除整数部分无效的0)
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS114_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS114_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型float或double
//  @param      num       	    整数位显示长度   最高10位  
//  @param      pointnum        小数位显示长度   最高6位
//  @return     void
//  @since      v1.0
//  Sample usage:               ips114_showfloat(0,0,x,2,3);//显示浮点数   整数显示2位   小数显示三位
//  @note                       特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//                              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//                              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//                              负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void ips114_showfloat(uint16 x,uint16 y,double dat,uint8 num,uint8 pointnum)
{
    uint8   length;
	int8    buff[34];
	int8    start,end,point;

	if(6<pointnum)  pointnum = 6;
    if(10<num)      num = 10;
        
    if(0>dat)   length = zf_sprintf( &buff[0],"%f",dat);//负数
    else
    {
        length = zf_sprintf( &buff[1],"%f",dat);
        length++;
    }
    point = length - 7;         //计算小数点位置
    start = point - num - 1;    //计算起始位
    end = point + pointnum + 1; //计算结束位
    while(0>start)//整数位不够  末尾应该填充空格
    {
        buff[end] = ' ';
        end++;
        start++;
    }
    
    if(0>dat)   buff[start] = '-';
    else        buff[start] = ' ';
    
    buff[end] = '\0';

    ips114_showstr(x, y, buff);	//显示数字
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      汉字显示
//  @param      x       横坐标 0-（IPS114_X_MAX-1）
//  @param      y       纵坐标 0-（IPS114_Y_MAX-1）
//  @param      size    取模的时候设置的汉字字体大小，也就是一个汉字占用的点阵长宽为多少个点，取模的时候需要长宽是一样的。
//  @param      *p      需要显示的汉字数组
//  @param      number  需要显示多少位
//  @param      color   显示颜色
//  @return     void
//  @since      v1.0
//  Sample usage:		ips114_display_chinese(0,0,16,chinese_test[0],4,RED);//显示font文件里面的 示例
//  @Note       使用PCtoLCD2002软件取模		    阴码、逐行式、顺向   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips114_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color)
{
    int i, j, k; 
    uint8 temp, temp1, temp2;
    const uint8 *p_data;
    
    temp2 = size/8;
    
    ips114_set_region(x,y,number*size-1+x,y+size-1);
    
    for(i=0;i<size;i++)
    {
        temp1 = number;
        p_data = p+i*temp2;
        while(temp1--)
        {
            for(k=0;k<temp2;k++)
            {
                for(j=8;j>0;j--)
                {
                    temp = (*p_data>>(j-1)) & 0x01;
                    if(temp)    ips114_writedata_16bit(color);
                    else        ips114_writedata_16bit(IPS114_BGCOLOR);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2*size;
        }   
    }
}


