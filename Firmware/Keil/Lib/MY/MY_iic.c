/*
 * @�ļ�       my_oled.c
 * @����       ���ٿ�
 * @����       STC  IIC
 * @���ʱ��   2020-3-21
 
��ʼ�źţ�SCLΪ�ߣ�SDA�Ӹߵ���
ֹͣ�źţ�SCLΪ�ߣ�SDA�ӵ͵���
�������ݣ���ÿ��SCLΪ�ߵ�ƽʱ��Ӧ��SDAΪ�ȶ��߻�͵�ƽ��SDA��ƽ��ȴ���SCL
ֻ����SCLΪ��ʱ��SDA�ſɱ䣬���򽫳��ִ���ʱ��
*/

#include "MY_iic.h"


//*****************************************************************************************************
//��ʱ2us
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
//IIC��ʼ�ź�
//******************************************************************************************************
void IIC_START(void)
{
	SCL_HIGH;
	SDA_HIGH;
	Delay2us();			//�ȶ�SCL��SDA��ƽ
	SDA_LOW;				//����START�ź�
	Delay1us();			//�ȶ�SDA��ƽ
	SCL_LOW;				//׼����������
	Delay1us();
}
//*****************************************************************************************************
//IICֹͣ�ź�
//******************************************************************************************************
void IIC_STOP(void)
{
	SDA_LOW;
	Delay1us();
	SCL_HIGH;
	Delay1us();
	SDA_HIGH;				//����STOP�ź�
	Delay1us();			//�������״̬
}

//*****************************************************************************************************
//@�������ܣ�  ����ACK�ź�
//@����ֵ��    0 ACK      1 NACK
//******************************************************************************************************
bit IIC_receiveACK(void)
{
	SDA_HIGH; 			//���������ߣ�׼������
	Delay1us();
	SCL_HIGH;
	Delay2us();
	CY=SDA;        //CYΪSTC�ڲ�״̬�Ĵ���PSW���λ   ���λ�������Ϊ1   
	SCL_LOW;
	Delay1us();
	return CY;
}

//*****************************************************************************************************
//@�������ܣ�  ����ACK��NACK�ź�
//@����     signal    0��ACK�� ��  1��NACK��
//******************************************************************************************************
void IIC_sendACKorNACK(bit signal)
{
	if(signal==ACK)       //�ж���NACK��ACK
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
//@�������ܣ�   ��IIC����д8λ����
//@������     dat      Ҫд��8λ����
//******************************************************************************************************
void IIC_sendbyte(u8 dat)
{
	u8 i;
	for(i=0;i<=7;i++)
	{
//		SDA_LOW;							//���ַ���Ҳok����ִ��ָ��ʱ�䳤
//		if((dat<<i)&0x80)
//		{
//			SDA_HIGH;
//		}
		dat <<= 1;              //�Ƴ����ݵ����λ   �������λΪһ��CY=1��PSW
    SDA = CY;               //�����ݿ�
		
		Delay1us();
		SCL_HIGH;
		Delay2us();
		SCL_LOW;
		Delay1us();
	}
}
//*****************************************************************************************************
//@�������ܣ�   ����IIC�����ϰ�λ����
//@����ֵ��     ���յ���8λ����
//******************************************************************************************************
u8 IIC_receivebyte(void)
{
	u8 dat=0;
	u8 i;
	SDA_HIGH;			//���������ߣ�׼������
	Delay1us();
	for(i=0;i<=7;i++)  //��������
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
//@�������ܣ�   ��IIC�ӻ��ļĴ���д8λ����
//@����1��  SlaveAddress     �ӻ���ַ
//@����2��  RegAddress       �ӻ��мĴ�����ַ
//@����3��  dat              Ҫ���͵�����
//@����   0 �ɹ�     !0ʧ��
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
//@�������ܣ�   ��IIC�ӻ��ļĴ���д���8λ����
//@����1��  SlaveAddress     �ӻ���ַ
//@����2��  RegAddress       �ӻ��мĴ�����ַ
//@����3��  Len              ���ݳ��ȣ���λ/byte��
//@����4��  *datbuf          Ҫ���͵�����
//@����   0 �ɹ�     !0ʧ��
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
//@�������ܣ�   ���մӻ��ļĴ���8λ����
//@����1��  SlaveAddress     �ӻ���ַ
//@����2��  RegAddress       �ӻ��мĴ�����ַ
//@����3��  *dat             Ҫ���յ�����
//@����   0 �ɹ�     !0ʧ��
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
//@�������ܣ�   ���մӻ��ļĴ������8λ����
//@����1��  SlaveAddress     �ӻ���ַ
//@����2��  RegAddress       �ӻ��мĴ�����ַ
//@����3��  Len              ���ݳ��ȣ���λ/byte��
//@����4��  *datbuf          Ҫ���յ�����
//@����   0 �ɹ�     !0ʧ��
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

