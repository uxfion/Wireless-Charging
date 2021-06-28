/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		TFT
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.26
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note		
					���߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SCL                 �鿴TFT_SCL_PIN		�궨�������     Ӳ��SPI���Ų��������л�
						SDA                 �鿴TFT_SDA_PIN		�궨�������     Ӳ��SPI���Ų��������л�
						RES                 �鿴REST_PIN		�궨�������    
						DC                  �鿴DC_PIN_PIN		�궨�������  
						CS                  �鿴TFT_CS_PIN		�궨�������      
						
						��Դ����
						BL  3.3V��Դ������������ţ�Ҳ���Խ�PWM���������ȣ�
						VCC 3.3V��Դ
						GND ��Դ��
						���ֱ���128*160
					------------------------------------ 

 ********************************************************************************************************************/




#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_PRINTF.h"
#include "STC8_spi.h"
#include "STC8_pca_delay.h"



void lcd_writeIndex(uint8 dat)			//д����
{	
    TFT_CS(0);
	TFT_DC(0);
	spi_mosi(dat);
    TFT_CS(1);
}

void lcd_writeData(uint8 dat)			//д����
{

	
    TFT_CS(0);
	TFT_DC(1);
	spi_mosi(dat);
    TFT_CS(1);

}

void lcd_writedata_16bit(uint16 dat)	//��Һ����дһ��16λ����
{

    uint8 dat1[2];
    dat1[0] = dat >> 8;
    dat1[1] = (uint8)dat;
	
    TFT_CS(0);
    TFT_DC(1);
	spi_mosi(dat1[0]);
	spi_mosi(dat1[1]);
    TFT_CS(1);

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����������
//  @param      x_start     	����x��������
//  @param      y_start     	����y��������
//  @param      x_end       	����x������յ�
//  @param      y_end       	����y������յ�
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_set_region(0,0,10,10);// x��y����㶼��0���յ㶼��10
//-------------------------------------------------------------------------------------------------------------------
void lcd_set_region(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{	

    
#if (0 == TFT_DISPLAY_DIR || 1 == TFT_DISPLAY_DIR)
    lcd_writeIndex(0x2a);
	lcd_writeData(0x00);
	lcd_writeData(x_start+2);
	lcd_writeData(0x00);
	lcd_writeData(x_end+2);

	lcd_writeIndex(0x2b);
	lcd_writeData(0x00);
	lcd_writeData(y_start+1);
	lcd_writeData(0x00);
	lcd_writeData(y_end+1);	

#elif(2 == TFT_DISPLAY_DIR || 3 == TFT_DISPLAY_DIR)
    lcd_writeIndex(0x2a);
	lcd_writeData(0x00);
	lcd_writeData(x_start+1);
	lcd_writeData(0x0);
	lcd_writeData(x_end+1);

	lcd_writeIndex(0x2b);
	lcd_writeData(0x00);
	lcd_writeData(y_start+2);
	lcd_writeData(0x00);
	lcd_writeData(y_end+2);	

#endif
    
    lcd_writeIndex(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����������
//  @param      color     		��ɫ����
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_clear(YELLOW);// ȫ������Ϊ��ɫ
//-------------------------------------------------------------------------------------------------------------------
void lcd_clear(int color)
{
 	uint8 i,j;
	lcd_set_region(0,0,TFT_X_MAX-1,TFT_Y_MAX-1);
 	for (i=0;i<TFT_Y_MAX;i++)
    	for (j=0;j<TFT_X_MAX;j++)
        	lcd_writedata_16bit(color);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʼ��
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void lcd_init(void)
{	
	spi_init(TFT_SPIN, TFT_SCL_PIN, TFT_SDA_PIN, TFT_SDA_IN_PIN, 0, MASTER, SPI_SYSclk_DIV_4);


    TFT_REST(0);
    pca_delay_ms(10);
    TFT_REST(1);
    pca_delay_ms(120);
    lcd_writeIndex(0x11);
    pca_delay_ms(120);

    lcd_writeIndex(0xB1); 
    lcd_writeData(0x01); 
    lcd_writeData(0x2C); 
    lcd_writeData(0x2D); 
    
    lcd_writeIndex(0xB2); 
    lcd_writeData(0x01); 
    lcd_writeData(0x2C); 
    lcd_writeData(0x2D); 
    
    lcd_writeIndex(0xB3); 
    lcd_writeData(0x01); 
    lcd_writeData(0x2C); 
    lcd_writeData(0x2D); 
    lcd_writeData(0x01); 
    lcd_writeData(0x2C); 
    lcd_writeData(0x2D); 
    
    lcd_writeIndex(0xB4);
    lcd_writeData(0x07); 

    lcd_writeIndex(0xC0); 
    lcd_writeData(0xA2); 
    lcd_writeData(0x02); 
    lcd_writeData(0x84); 
    lcd_writeIndex(0xC1); 
    lcd_writeData(0xC5); 
    
    lcd_writeIndex(0xC2); 
    lcd_writeData(0x0A); 
    lcd_writeData(0x00); 
    
    lcd_writeIndex(0xC3); 
    lcd_writeData(0x8A); 
    lcd_writeData(0x2A); 
    lcd_writeIndex(0xC4); 
    lcd_writeData(0x8A); 
    lcd_writeData(0xEE); 
    
    lcd_writeIndex(0xC5);
    lcd_writeData(0x0E); 
    
    lcd_writeIndex(0x36);
    switch(TFT_DISPLAY_DIR)//y x v
    {
        case 0: lcd_writeData(1<<7 | 1<<6 | 0<<5);  break;  //����ģʽ
        case 1: lcd_writeData(0<<7 | 0<<6 | 0<<5);  break;  //����ģʽ  ��ת180
        case 2: lcd_writeData(1<<7 | 0<<6 | 1<<5);  break;  //����ģʽ
        case 3: lcd_writeData(0<<7 | 1<<6 | 1<<5);  break;  //����ģʽ  ��ת180
    }

    lcd_writeIndex(0xe0); 
    lcd_writeData(0x0f); 
    lcd_writeData(0x1a); 
    lcd_writeData(0x0f); 
    lcd_writeData(0x18); 
    lcd_writeData(0x2f); 
    lcd_writeData(0x28); 
    lcd_writeData(0x20); 
    lcd_writeData(0x22); 
    lcd_writeData(0x1f); 
    lcd_writeData(0x1b); 
    lcd_writeData(0x23); 
    lcd_writeData(0x37); 
    lcd_writeData(0x00); 	
    lcd_writeData(0x07); 
    lcd_writeData(0x02); 
    lcd_writeData(0x10); 
    
    lcd_writeIndex(0xe1); 
    lcd_writeData(0x0f); 
    lcd_writeData(0x1b); 
    lcd_writeData(0x0f); 
    lcd_writeData(0x17); 
    lcd_writeData(0x33); 
    lcd_writeData(0x2c); 
    lcd_writeData(0x29); 
    lcd_writeData(0x2e); 
    lcd_writeData(0x30); 
    lcd_writeData(0x30); 
    lcd_writeData(0x39); 
    lcd_writeData(0x3f); 
    lcd_writeData(0x00); 
    lcd_writeData(0x07); 
    lcd_writeData(0x03); 
    lcd_writeData(0x10);  
    
    lcd_writeIndex(0x2a);
    lcd_writeData(0x00);
    lcd_writeData(0x00+2);
    lcd_writeData(0x00);
    lcd_writeData(0x80+2);
    
    lcd_writeIndex(0x2b);
    lcd_writeData(0x00);
    lcd_writeData(0x00+3);
    lcd_writeData(0x00);
    lcd_writeData(0x80+3);
    
    lcd_writeIndex(0xF0); 
    lcd_writeData(0x01); 
    lcd_writeIndex(0xF6);
    lcd_writeData(0x00); 
    
    lcd_writeIndex(0x3A);
    lcd_writeData(0x05); 
    lcd_writeIndex(0x29);

    lcd_clear(WHITE);//ȫ��
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ������
//  @param      x     	        ����x��������
//  @param      y     	        ����y��������
//  @param      dat       	    ��Ҫ��ʾ����ɫ
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_drawpoint(0,0,RED);  //����0,0��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void lcd_drawpoint(uint16 x,uint16 y,uint16 color)
{
    lcd_set_region(x,y,x,y);
    lcd_writedata_16bit(color);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ�ַ�
//  @param      x     	        ����x�������� ������Χ 0 -��TFT_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��TFT_Y_MAX-1��
//  @param      dat       	    ��Ҫ��ʾ���ַ�
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showchar(0,0,'x');//����0,0дһ���ַ�x
//-------------------------------------------------------------------------------------------------------------------
void lcd_showchar(uint16 x,uint16 y,uint8 dat)
{
	uint8 i,j;
	uint8 temp;
    
	for(i=0; i<16; i++)
	{
		lcd_set_region(x,y+i,x+7,y+i);
		temp = tft_ascii[dat-32][i];//��32��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո���ascii�������32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)	lcd_writedata_16bit(PENCOLOR);
			else			lcd_writedata_16bit(BGCOLOR);
			temp>>=1;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ�ַ���
//  @param      x     	        ����x��������  ������Χ 0 -��TFT_X_MAX-1��
//  @param      y     	        ����y��������  ������Χ 0 -��TFT_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ���ַ���
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void lcd_showstr(uint16 x,uint16 y,uint8 dat[])
{
	uint16 j;
	
	j = 0;
	while(dat[j] != '\0')
	{
		lcd_showchar(x+8*j,y*16,dat[j]);
		j++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�з���
//  @param      x     	        ����x��������  ������Χ 0 -��TFT_X_MAX-1��
//  @param      y     	        ����y��������  ������Χ 0 -��TFT_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������int8
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showint8(0,0,x);//xΪint8����
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint8(uint16 x,uint16 y,int8 dat)
{
	uint8 a[3];
	uint8 i;
	if(dat >= 0)
	{
		lcd_showchar(x,y*16,' ');
	}
	else	
	{
		lcd_showchar(x,y*16,'-');
		dat = -dat;
	}
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		lcd_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ8λ�޷���
//  @param      x     	        ����x�������� ������Χ 0 -��TFT_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������uint8
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint8(0,0,x);//xΪuint8����
//-------------------------------------------------------------------------------------------------------------------
void lcd_showuint8(uint16 x,uint16 y,uint8 dat)
{
	uint8 a[3];
	uint8 i;
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		lcd_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ16λ�з���
//  @param      x     	        ����x�������� ������Χ 0 -��TFT_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������int16
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showint16(0,0,x);//xΪint16����
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint16(uint16 x,uint16 y,int16 dat)
{
	uint8 a[5];
	uint8 i;
	if(dat<0)
	{
		lcd_showchar(x,y*16,'-');
		dat = -dat;
	}
	else	lcd_showchar(x,y*16,' ');

	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		lcd_showchar(x+(8*(i+1)),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ16λ�޷���
//  @param      x     	        ����x�������� ������Χ 0 -��TFT_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������uint16
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint16(0,0,x);//xΪuint16����
//-------------------------------------------------------------------------------------------------------------------
void lcd_showuint16(uint16 x,uint16 y,uint16 dat)
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
		lcd_showchar(x+(8*i),y*16,'0' + a[i]);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ32λ�з���(ȥ������������Ч��0)
//  @param      x     	        ����x�������� ������Χ 0 -��TFT_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������uint32
//  @param      num       	    ��Ҫ��ʾ��λ�� ���10λ  ������������
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showuint32(0,0,x,3);//x����Ϊint32 uint16 int16 uint8 int8����
//  Sample usage:               ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void lcd_showint32(uint16 x,uint16 y,int32 dat,uint8 num)
{
    int8    buff[34];
    uint8   length;
    
    if(10<num)      num = 10;
    
    num++;
    if(0>dat)   length = zf_sprintf( &buff[0],"%d",dat);//����
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

    lcd_showstr(x, y, buff);	//��ʾ����
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����ʾ������(ȥ������������Ч��0)
//  @param      x     	        ����x�������� ������Χ 0 -��TFT_X_MAX-1��
//  @param      y     	        ����y�������� ������Χ 0 -��TFT_Y_MAX/16-1��
//  @param      dat       	    ��Ҫ��ʾ�ı�������������float��double
//  @param      num       	    ����λ��ʾ����   ���10λ  
//  @param      pointnum        С��λ��ʾ����   ���6λ
//  @return     void
//  @since      v1.0
//  Sample usage:               lcd_showfloat(0,0,x,2,3);//��ʾ������   ������ʾ2λ   С����ʾ��λ
//  @note                       �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//                              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//                              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//                              ��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void lcd_showfloat(uint16 x,uint16 y,double dat,uint8 num,uint8 pointnum)
{
    uint8   length;
	int8    buff[34];
	int8    start,end,point;

	if(6<pointnum)  pointnum = 6;
    if(10<num)      num = 10;
        
    if(0>dat)   length = zf_sprintf( &buff[0],"%f",dat);//����
    else
    {
        length = zf_sprintf( &buff[1],"%f",dat);
        length++;
    }
    point = length - 7;         //����С����λ��
    start = point - num - 1;    //������ʼλ
    end = point + pointnum + 1; //�������λ
    while(0>start)//����λ����  ĩβӦ�����ո�
    {
        buff[end] = ' ';
        end++;
        start++;
    }
    
    if(0>dat)   buff[start] = '-';
    else        buff[start] = ' ';
    
    buff[end] = '\0';

    lcd_showstr(x, y, buff);	//��ʾ����
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʾ40*40 QQͼƬ
//  @param      *p     			ͼ�������ַ
//  @return     void
//  @since      v1.0
//  Sample usage:          
//-------------------------------------------------------------------------------------------------------------------
void showimage(const unsigned char *p) 
{
  	int i,j,k; 
	unsigned char picH,picL;
	lcd_clear(WHITE); //����  
	
	for(k=0;k<4;k++)
	{
	   	for(j=0;j<3;j++)
		{	
			lcd_set_region(40*j,40*k,40*j+39,40*k+39);		//��������
		    for(i=0;i<40*40;i++)
			 {	
			 	picL=*(p+i*2);	//���ݵ�λ��ǰ
				picH=*(p+i*2+1);				
				lcd_writedata_16bit(picH<<8|picL);  						
			 }	
		 }
	}		
}

