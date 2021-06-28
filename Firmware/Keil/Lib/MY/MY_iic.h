/*
 * @文件       my_oled.c
 * @作者       董少俊
 * @功能       STC  IIC
 * @完成时间   2020-3-21
 */

#ifndef _MY_IIC
#define _MY_IIC

#include "STC8Axx.h"
#include "common.h"

sbit SCL = P0^5;
sbit SDA = P0^3;

#define		SCL_LOW		  SCL=0
#define		SCL_HIGH		SCL=1
#define		SDA_LOW		  SDA=0
#define		SDA_HIGH		SDA=1

#define		ACK   0
#define   NACK  1


void IIC_init();
u8 IIC_WriteBiteToSlave(u8 SlaveAddress,u8 RegAddress,u8 dat);
u8 IIC_BurstWriteByteToSlave(u8 SlaveAddress,u8 RegAddress,u8 Len,u8 *datbuf);
u8 IIC_ReceiveSingleByte(u8 SlaveAddress,u8 RegAddress,u8 *dat);
u8 IIC_ReceiveBurstByte(u8 SlaveAddress,u8 RegAddress,u8 Len,u8 *datbuf);



//以下函数不调用
void IIC_START(void);
void IIC_STOP(void);
bit IIC_receiveACK(void);
void IIC_sendACKorNACK(bit signal);
void IIC_sendbyte(u8 dat);
u8 IIC_receivebyte(void);


#endif


