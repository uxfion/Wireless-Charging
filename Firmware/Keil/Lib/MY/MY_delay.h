/*
 * @�ļ�       my_delay.h
 * @����       ���ٿ�
 * @����       �ö�ʱ��4 ʵ�ֵ���ʱ����
 * @���ʱ��   2020-4-11
 */

#ifndef MY_DELAY_H
#define MY_DELAY_H

#include "STC8Axx.h"
#include "common.h"

void Delay_Init(void);		//����Ҫ����
void Delay_us(u32 us);
void Delay_ms(u32 ms);

#endif


