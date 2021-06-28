/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		ICM20602
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴common.h��VERSION�궨��
 * @Software 		IAR 7.8 or MDK 5.24a
 * @Target core		LPC54606J512BD100
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2018-05-24
 * @note		
					���߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SPC                 �鿴SEEKFREE_ICM20602.c�ļ��е�ICM20602_SPI_SCLK_PIN �궨��
						SDI                 �鿴SEEKFREE_ICM20602.c�ļ��е�ICM20602_SPI_MOSI_PIN �궨��
						SDO                 �鿴SEEKFREE_ICM20602.c�ļ��е�ICM20602_SPI_MISO_PIN �궨��
						CS                  �鿴SEEKFREE_ICM20602.c�ļ��е�ICM20602_SPI_CS_PIN   �궨��
					------------------------------------ 
 ********************************************************************************************************************/
#ifndef _SEEKFREE_ICM20602_h
#define _SEEKFREE_ICM20602_h

#include "common.h"


//--------------------���SPI--------------------
#define ICM20602_SCK_PIN   		P25
#define ICM20602_MOSI_PIN  		P23
#define ICM20602_MISO_PIN  		P24
#define ICM20602_CSN_PIN   		P26

#define ICM20602_SCK(x)			ICM20602_SCK_PIN  = x
#define ICM20602_MOSI(x) 		ICM20602_MOSI_PIN = x
#define ICM20602_CSN(x)  		ICM20602_CSN_PIN  = x
#define ICM20602_MISO    		ICM20602_MISO_PIN 

//--------------------Ӳ��SPI--------------------
#define ICM20602_SPI_N 			SPI_2
#define ICM20602_SPI_SCLK_PIN   SPI2_SCLK_P25
#define ICM20602_SPI_MOSI_PIN   SPI2_MOSI_P23
#define ICM20602_SPI_MISO_PIN   SPI2_MISO_P24
#define ICM20602_SPI_CS_PIN     P26

#define ICM20602_CS(x) 			ICM20602_SPI_CS_PIN = x


//--------------------���IIC--------------------
//�鿴SEEKFREE_IIC�ļ�



#define     ICM20602_DEV_ADDR           0x69 //SA0�ӵأ�0x68   SA0������0x69  ģ��Ĭ������


#define     ICM20602_SPI_W              0x00
#define     ICM20602_SPI_R              0x80


#define     ICM20602_XG_OFFS_TC_H       0x04
#define     ICM20602_XG_OFFS_TC_L       0x05
#define     ICM20602_YG_OFFS_TC_H       0x07
#define     ICM20602_YG_OFFS_TC_L       0x08
#define     ICM20602_ZG_OFFS_TC_H       0x0A
#define     ICM20602_ZG_OFFS_TC_L       0x0B
#define     ICM20602_SELF_TEST_X_ACCEL  0x0D
#define     ICM20602_SELF_TEST_Y_ACCEL  0x0E
#define     ICM20602_SELF_TEST_Z_ACCEL  0x0F
#define     ICM20602_XG_OFFS_USRH       0x13
#define     ICM20602_XG_OFFS_USRL       0x14
#define     ICM20602_YG_OFFS_USRH       0x15
#define     ICM20602_YG_OFFS_USRL       0x16
#define     ICM20602_ZG_OFFS_USRH       0x17
#define     ICM20602_ZG_OFFS_USRL       0x18
#define     ICM20602_SMPLRT_DIV         0x19
#define     ICM20602_CONFIG             0x1A
#define     ICM20602_GYRO_CONFIG        0x1B
#define     ICM20602_ACCEL_CONFIG       0x1C
#define     ICM20602_ACCEL_CONFIG_2     0x1D
#define     ICM20602_LP_MODE_CFG        0x1E
#define     ICM20602_ACCEL_WOM_X_THR    0x20
#define     ICM20602_ACCEL_WOM_Y_THR    0x21
#define     ICM20602_ACCEL_WOM_Z_THR    0x22
#define     ICM20602_FIFO_EN            0x23
#define     ICM20602_FSYNC_INT          0x36
#define     ICM20602_INT_PIN_CFG        0x37
#define     ICM20602_INT_ENABLE         0x38
#define     ICM20602_FIFO_WM_INT_STATUS 0x39 
#define     ICM20602_INT_STATUS         0x3A
#define     ICM20602_ACCEL_XOUT_H       0x3B
#define     ICM20602_ACCEL_XOUT_L       0x3C
#define     ICM20602_ACCEL_YOUT_H       0x3D
#define     ICM20602_ACCEL_YOUT_L       0x3E
#define     ICM20602_ACCEL_ZOUT_H       0x3F
#define     ICM20602_ACCEL_ZOUT_L       0x40
#define     ICM20602_TEMP_OUT_H         0x41
#define     ICM20602_TEMP_OUT_L         0x42
#define     ICM20602_GYRO_XOUT_H        0x43
#define     ICM20602_GYRO_XOUT_L        0x44
#define     ICM20602_GYRO_YOUT_H        0x45
#define     ICM20602_GYRO_YOUT_L        0x46
#define     ICM20602_GYRO_ZOUT_H        0x47
#define     ICM20602_GYRO_ZOUT_L        0x48
#define     ICM20602_SELF_TEST_X_GYRO   0x50
#define     ICM20602_SELF_TEST_Y_GYRO   0x51
#define     ICM20602_SELF_TEST_Z_GYRO   0x52
#define     ICM20602_FIFO_WM_TH1        0x60
#define     ICM20602_FIFO_WM_TH2        0x61
#define     ICM20602_SIGNAL_PATH_RESET  0x68
#define     ICM20602_ACCEL_INTEL_CTRL   0x69
#define     ICM20602_USER_CTRL          0x6A
#define     ICM20602_PWR_MGMT_1         0x6B
#define     ICM20602_PWR_MGMT_2         0x6C
#define     ICM20602_I2C_IF             0x70
#define     ICM20602_FIFO_COUNTH        0x72
#define     ICM20602_FIFO_COUNTL        0x73
#define     ICM20602_FIFO_R_W           0x74
#define     ICM20602_WHO_AM_I           0x75
#define     ICM20602_XA_OFFSET_H        0x77
#define     ICM20602_XA_OFFSET_L        0x78
#define     ICM20602_YA_OFFSET_H        0x7A
#define     ICM20602_YA_OFFSET_L        0x7B
#define     ICM20602_ZA_OFFSET_H        0x7D
#define     ICM20602_ZA_OFFSET_L        0x7E

extern int16 icm_gyro_x,icm_gyro_y,icm_gyro_z;
extern int16 icm_acc_x,icm_acc_y,icm_acc_z;


//--------Ӳ��SPI--------------
void icm20602_init_spi(void);
void get_icm20602_gyro_spi(void);
void get_icm20602_accdata_spi(void);

//--------���SPI--------------
void icm20602_init_simspi(void);
void get_icm20602_gyro_simspi(void);
void get_icm20602_accdata_simspi(void);

//--------���IIC--------------
void icm20602_init_simiic(void);
void get_icm20602_accdata_simiic(void);
void get_icm20602_gyro_simiic(void);


#endif
