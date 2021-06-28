/*
 * @文件       my_delay.h
 * @作者       董少俊
 * @功能       用定时器4 实现的延时函数
 * @完成时间   2020-4-11
 */

#ifndef MY_DELAY_H
#define MY_DELAY_H

#include "STC8Axx.h"
#include "common.h"

void Delay_Init(void);		//必须要调用
void Delay_us(u32 us);
void Delay_ms(u32 ms);

#endif


