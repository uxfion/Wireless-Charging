/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		apic(�ж����ȼ�����)
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __STC8_NVIC_H
#define __STC8_NVIC_H

#include "common.h"

//��ö�����ֹ�û��޸�
//�ж����ȼ�����ö����
typedef enum
{
	INT0_IRQn = 0x00,
	TIMER0_IRQn,
	INT1_IRQn,
	TIMER1_IRQn,
	UART1_IRQn,
	ADC_IRQn,
	LVD_IRQn,			//��ѹ����ж�
	CCP_PCA_PWM_IRQn,	//CCP/PCA/PWM �ж�
	
	UART2_IRQn = 0x10,
	SPI_IRQn,
	PWM_IRQn,
	PWMFD_IRQn,
	INT4_IRQn,
	CMP_IRQn,
	IIC_IRQn,
	
//	STC8Aû����һЩλ
//	PWM2FD_PTKSU_IRQn,	//��ǿ�� PWM2 �쳣����ж� �� ���������ж�
//	UART3_IRQn = 0x20,
//	UART4_IRQn,
//	PWM1_IRQn,
//	PWM2_IRQn,
//	PWM3_IRQn,
//	PWM4_IRQn,
//	PWM5_IRQn,
//	PWM4FD_IRQn,

}NVIC_IRQn_enum;


void NVIC_SetPriority(NVIC_IRQn_enum irqn,uint8 priority);



#endif