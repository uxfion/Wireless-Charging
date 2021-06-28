/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		����ʽ������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴LPC546XX_config.h�ļ��ڰ汾�궨��
 * @Software 		IAR 8.3 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-01-13
 * @note		
					���߶��壺
					------------------------------------ 
					ģ��ܽ�            ��Ƭ���ܽ�
					SCK    			    �鿴SEEKFREE_ABSOLUTE_ENCODER.H�ļ�ABS_ENCODER_SPI_SCK_PIN �궨��
					MISO         	    �鿴SEEKFREE_ABSOLUTE_ENCODER.H�ļ�ABS_ENCODER_SPI_MISO_PIN�궨��
					MOSI                �鿴SEEKFREE_ABSOLUTE_ENCODER.H�ļ�ABS_ENCODER_SPI_MOSI_PIN�궨��
					CS                  �鿴SEEKFREE_ABSOLUTE_ENCODER.H�ļ�ABS_ENCODER_SPI_PCS_PIN  �궨��
					------------------------------------  
 ********************************************************************************************************************/

#ifndef _SEEKFREE_ABSOLUTE_ENCODER_H
#define _SEEKFREE_ABSOLUTE_ENCODER_H



#include "common.h"

//------------------------���SPI--------------------------//
#define ABS_ENCODER_SCK_PIN   P77   //��ģ�� SCLK
#define ABS_ENCODER_MISO_PIN  P76  	//��ģ�� MISO
#define ABS_ENCODER_MOSI_PIN  P75   //��ģ�� MOSI
#define ABS_ENCODER_CSN_PIN   P54   //��ģ�� CS
     
#define ABS_ENCODER_SCK(x)  ABS_ENCODER_SCK_PIN  = x
#define ABS_ENCODER_MISO    ABS_ENCODER_MISO_PIN
#define ABS_ENCODER_MOSI(x) ABS_ENCODER_MOSI_PIN = x
#define ABS_ENCODER_CSN(x)  ABS_ENCODER_CSN_PIN  = x



//------------------------Ӳ��SPI--------------------------//
#define ABS_ENCODER_SPI_NUM	SPI_3

#define ABS_ENCODER_SPI_SCK_PIN   	SPI3_SCLK_P77   	//��ģ�� SCLK
#define ABS_ENCODER_SPI_MISO_PIN  	SPI3_MISO_P76  		//��ģ�� MISO
#define ABS_ENCODER_SPI_MOSI_PIN  	SPI3_MOSI_P75   	//��ģ�� MOSI
#define ABS_ENCODER_SPI_CS_PIN   	P54    				//��ģ�� CS

#define ABS_ENCODER_SPI_CSN(x)  	ABS_ENCODER_SPI_CS_PIN = x


//�Ƕȴ���������
#define ABS_ENCODER_SPI_W       0x80
#define ABS_ENCODER_SPI_R       0x40


#define ZERO_L_REG              0x00
#define ZERO_H_REG              0x01
#define DIR_REG                 0X09


void   encoder_init_simspi(void);
uint16 encoder_angle_simspi(void);

void   encoder_init_spi(void);
uint16 encoder_angle_spi(void);


#endif

