/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		common
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C51 V9.60
 * @Target core		STC8A8K64S4
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __COMMON_H_
#define __COMMON_H_
#include "STC8Axx.h"

//数据类型声明
typedef unsigned char   uint8  ;	//  8 bits 
typedef unsigned int  	uint16 ;	// 16 bits 
typedef unsigned long   uint32 ;	// 32 bits 
								
								
typedef signed char     int8   ;	//  8 bits 
typedef signed int      int16  ;	// 16 bits 
typedef signed long     int32  ;	// 32 bits 
								
								
typedef volatile int8   vint8  ;	//  8 bits 
typedef volatile int16  vint16 ;	// 16 bits 
typedef volatile int32  vint32 ;	// 32 bits 
								
								
typedef volatile uint8  vuint8 ;	//  8 bits 
typedef volatile uint16 vuint16;	// 16 bits 
typedef volatile uint32 vuint32;	// 32 bits 


typedef unsigned char	u8;
typedef unsigned int	u16;
typedef unsigned long	u32;

typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int32_t;
//typedef   signed       __INT64 int64_t;

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
//typedef unsigned       __INT64 uint64_t;


#endif
