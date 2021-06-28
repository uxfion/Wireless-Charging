/*
 * @文件       my_oled.c
 * @作者       董少俊
 * @功能       STC  IIC
 * @完成时间   2020-3-21
 
起始信号：SCL为高，SDA从高到低
停止信号：SCL为高，SDA从低到高
发送数据：在每个SCL为高电平时相应的SDA为稳定高或低电平且SDA电平宽度大于SCL
只有在SCL为低时，SDA才可变，否则将出现错误时序！
*/

#include "MY_iic.h"


//*****************************************************************************************************
//延时2us
//******************************************************************************************************
static void Delay2us()		//@24.000MHz
{
	u8 i;
	i = 14;
	while (--i);
}

static void Delay1us()		//@24.000MHz
{
	unsigned char i;

	i = 6;
	while (--i);
}


void IIC_init()
{
	P1M1&=0xfd;
	P1M0&=0xfd;
	
	P1M1&=0xfb;
	P1M0&=0xfb;
	
	P1PU|=0x06;
}

//*****************************************************************************************************
//IIC起始信号
//******************************************************************************************************
void IIC_START(void)
{
	SCL_HIGH;
	SDA_HIGH;
	Delay2us();			//稳定SCL和SDA电平
	SDA_LOW;				//产生START信号
	Delay1us();			//稳定SDA电平
	SCL_LOW;				//准备发送数据
	Delay1us();
}
//*****************************************************************************************************
//IIC停止信号
//******************************************************************************************************
void IIC_STOP(void)
{
	SDA_LOW;
	Delay1us();
	SCL_HIGH;
	Delay1us();
	SDA_HIGH;				//产生STOP信号
	Delay1us();			//进入空闲状态
}

//*****************************************************************************************************
//@函数功能：  接收ACK信号
//@返回值：    0 ACK      1 NACK
//******************************************************************************************************
bit IIC_receiveACK(void)
{
	SDA_HIGH; 			//数据线拉高，准备接收
	Delay1us();
	SCL_HIGH;
	Delay2us();
	CY=SDA;        //CY为STC内部状态寄存器PSW最高位   最高位数据溢出为1   
	SCL_LOW;
	Delay1us();
	return CY;
}

//*****************************************************************************************************
//@函数功能：  发送ACK或NACK信号
//@参数     signal    0（ACK） 或  1（NACK）
//******************************************************************************************************
void IIC_sendACKorNACK(bit signal)
{
	if(signal==ACK)       //判断是NACK或ACK
	{
		SDA_LOW;
	}
	else if(signal==NACK)
	{
		SDA_HIGH;
	}
	Delay1us();
	SCL_HIGH;
	Delay2us();
	SCL_LOW;
	Delay1us();
}
//*****************************************************************************************************
//@函数功能：   向IIC总线写8位数据
//@参数：     dat      要写的8位数据
//******************************************************************************************************
void IIC_sendbyte(u8 dat)
{
	u8 i;
	for(i=0;i<=7;i++)
	{
//		SDA_LOW;							//这种方法也ok但是执行指令时间长
//		if((dat<<i)&0x80)
//		{
//			SDA_HIGH;
//		}
		dat <<= 1;              //移出数据的最高位   （若最高位为一则CY=1）PSW
    SDA = CY;               //送数据口
		
		Delay1us();
		SCL_HIGH;
		Delay2us();
		SCL_LOW;
		Delay1us();
	}
}
//*****************************************************************************************************
//@函数功能：   接收IIC总线上八位数据
//@返回值：     接收到的8位数据
//******************************************************************************************************
u8 IIC_receivebyte(void)
{
	u8 dat=0;
	u8 i;
	SDA_HIGH;			//数据线拉高，准备接收
	Delay1us();
	for(i=0;i<=7;i++)  //接收数据
	{
		dat<<=1;
		SCL_HIGH;
		Delay1us();
		if(SDA)
		{
			dat++;
		}
		Delay1us();
		SCL_LOW;
		Delay1us();
	}
	return dat;
}
//*****************************************************************************************************
//@函数功能：   向IIC从机的寄存器写8位数据
//@参数1：  SlaveAddress     从机地址
//@参数2：  RegAddress       从机中寄存器地址
//@参数3：  dat              要发送的数据
//@返回   0 成功     !0失败
//******************************************************************************************************
u8 IIC_WriteBiteToSlave(u8 SlaveAddress,u8 RegAddress,u8 dat)
{
	IIC_START();
	IIC_sendbyte((SlaveAddress<<1)+0);
	if(IIC_receiveACK()==NACK) return 1;
	IIC_sendbyte(RegAddress);
	if(IIC_receiveACK()==NACK) return 2;
	IIC_sendbyte(dat);
	if(IIC_receiveACK()==NACK) return 3;
	IIC_STOP();
	return 0;
}
//*****************************************************************************************************
//@函数功能：   向IIC从机的寄存器写多个8位数据
//@参数1：  SlaveAddress     从机地址
//@参数2：  RegAddress       从机中寄存器地址
//@参数3：  Len              数据长度（单位/byte）
//@参数4：  *datbuf          要发送的数据
//@返回   0 成功     !0失败
//******************************************************************************************************
u8 IIC_BurstWriteByteToSlave(u8 SlaveAddress,u8 RegAddress,u8 Len,u8 *datbuf)
{
	u8 i;
	IIC_START();
	IIC_sendbyte((SlaveAddress<<1)+0);
	if(IIC_receiveACK()==NACK) return 1;
	IIC_sendbyte(RegAddress);
	if(IIC_receiveACK()==NACK) return 2;
	for(i=0;i<Len;i++)
	{
		IIC_sendbyte(datbuf[i]);
	  if(IIC_receiveACK()==NACK) return 3;
	}
	IIC_STOP();
	return 0;
}
//*****************************************************************************************************
//@函数功能：   接收从机的寄存器8位数据
//@参数1：  SlaveAddress     从机地址
//@参数2：  RegAddress       从机中寄存器地址
//@参数3：  *dat             要接收的数据
//@返回   0 成功     !0失败
//******************************************************************************************************
u8 IIC_ReceiveSingleByte(u8 SlaveAddress,u8 RegAddress,u8 *dat)
{
	IIC_START();
	IIC_sendbyte((SlaveAddress<<1)+0);
	if(IIC_receiveACK()==NACK) return 1;
	IIC_sendbyte(RegAddress);
	if(IIC_receiveACK()==NACK) return 2;
	IIC_START();
	IIC_sendbyte((SlaveAddress<<1)+1);
	if(IIC_receiveACK()==NACK) return 3;
	*dat=IIC_receivebyte();
	IIC_sendACKorNACK(NACK);
	IIC_STOP();
	return 0;
}
//*****************************************************************************************************
//@函数功能：   接收从机的寄存器多个8位数据
//@参数1：  SlaveAddress     从机地址
//@参数2：  RegAddress       从机中寄存器地址
//@参数3：  Len              数据长度（单位/byte）
//@参数4：  *datbuf          要接收的数据
//@返回   0 成功     !0失败
//******************************************************************************************************
u8 IIC_ReceiveBurstByte(u8 SlaveAddress,u8 RegAddress,u8 Len,u8 *datbuf)
{
	u8 i;
	IIC_START();
	IIC_sendbyte((SlaveAddress<<1)+0);
	if(IIC_receiveACK()==NACK) return 1;
	IIC_sendbyte(RegAddress);
	if(IIC_receiveACK()==NACK) return 2;
	IIC_START();
	IIC_sendbyte((SlaveAddress<<1)+1);
	if(IIC_receiveACK()==NACK) return 3;
	for(i=0;i<Len-1;i++)
	{
		datbuf[i]=IIC_receivebyte();
		IIC_sendACKorNACK(ACK);
	}
	datbuf[Len-1]=IIC_receivebyte();
	IIC_sendACKorNACK(NACK);
	IIC_STOP();
	return 0;
}

