/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		��ɿƼ�����ת����ģ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.24
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-03-27
 * @note		
					���߶��壺
					------------------------------------ 
					    ����ת����       ��Ƭ��                        
    					RX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_TX�궨��
    					TX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_RX�궨��
    					RTS             �鿴SEEKFREE_WIRELESS.h�ļ��е�RTS_PIN�궨��
    					CMD             �鿴SEEKFREE_WIRELESS.h�ļ��е�CMD_PIN�궨��
					------------------------------------ 
 ********************************************************************************************************************/

#ifndef _SEEKFREE_WIRELESS
#define _SEEKFREE_WIRELESS


#include "common.h"


#define WIRELESS_UART        		UART_4         //����ת����ģ�� ��ʹ�õ��Ĵ���     
#define WIRELESS_UART_TX_PIN    	UART4_TX_P03
#define WIRELESS_UART_RX_PIN    	UART4_RX_P02
#define WIRELESS_UART_BAUD   		115200
#define WIRELESS_UART_TIM			TIM_4

#define RTS_PIN P07 			//��������λ����  ָʾ��ǰģ���Ƿ���Խ�������  0���Լ�������  1�����Լ�������
//#define CMD_PIN P05 			//������������



void seekfree_wireless_init(void);
void wireless_uart_callback(void *userData);

uint32  seekfree_wireless_send_buff(uint8 *buff, uint32 len);

#endif 
