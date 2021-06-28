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

#include "SEEKFREE_ABSOLUTE_ENCODER.h"
#include "STC8_spi.h"



//-------------------------����Ϊ���SPI--------------------------//
//-------------------------����Ϊ���SPI--------------------------//
//-------------------------����Ϊ���SPI--------------------------//

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ڲ�ʹ���û��������
//  @param      NULL            	
//  @return     void
//  Sample usage:               �����û����ã��û���ʹ��h�ļ��еĺ궨��
//-------------------------------------------------------------------------------------------------------------------
void encoder_delay_us(uint16 x)
{
    uint8 i;
    while(x--)
    {
		i = 9;
		while (--i);
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ͨ��SPIдһ��byte,ͬʱ��ȡһ��byte
//  @param      byte        ���͵�����    
//  @return     uint8       return ����status״̬
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 simspi_wr_byte(uint8 byte)
{
    uint8 i;
    for(i=0; i<8; i++)
    {
        ABS_ENCODER_MOSI(byte & 0x80);
        byte <<= 1;
        ABS_ENCODER_SCK (0);    
      
        ABS_ENCODER_SCK (1);
		byte |= ABS_ENCODER_MISO;  
    }	
    return(byte);                                      		
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��valд��cmd��Ӧ�ļĴ�����ַ,ͬʱ����status�ֽ�
//  @param      cmd         ������
//  @param      val         ��д��Ĵ�������ֵ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void simspi_w_reg_byte(uint8 cmd, uint8 val)
{
    ABS_ENCODER_CSN (0);
    cmd |= ABS_ENCODER_SPI_W;
    simspi_wr_byte(cmd);                      	
    simspi_wr_byte(val);                               	
    ABS_ENCODER_CSN (1);                                    	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡcmd����Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void simspi_r_reg_byte(uint8 cmd, uint8 *val)
{
    ABS_ENCODER_CSN (0);
    cmd |= ABS_ENCODER_SPI_R;
    simspi_wr_byte(cmd);                               	
    *val = simspi_wr_byte(0);                           	
    ABS_ENCODER_CSN (1);                                    	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      д��һ�����ݵ��������ļĴ���
//  @param      cmd         �Ĵ�����ַ
//  @param      *val        д�����ݵĵ�ַ
//  @return     uint8       0������  1��ʧ��
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 encoder_simspi_w_reg_byte(uint8 cmd, uint8 val)
{
	
    uint8 dat;
    ABS_ENCODER_CSN (0);
    cmd |= ABS_ENCODER_SPI_W;
    simspi_wr_byte(cmd);                      	
    simspi_wr_byte(val);                               	
    ABS_ENCODER_CSN (1);   
//    systick_delay_us(1);
    ABS_ENCODER_CSN (0);
    dat = simspi_wr_byte(0x00);                      	
    simspi_wr_byte(0x00);                               	
    ABS_ENCODER_CSN (1);  
    
    if(val != dat)  return 1;   //д��ʧ��
    return 0;                   //д��ɹ�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ�Ĵ���
//  @param      cmd         �Ĵ�����ַ
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void encoder_simspi_r_reg_byte(uint8 cmd, uint8 *val)
{
    ABS_ENCODER_CSN (0);
    cmd |= ABS_ENCODER_SPI_R;
    simspi_wr_byte(cmd);                      	
    simspi_wr_byte(0x00);                               	
    ABS_ENCODER_CSN (1);   
	encoder_delay_us(1);
    ABS_ENCODER_CSN (0);
    *val = simspi_wr_byte(0x00);                      	
    simspi_wr_byte(0x00);                               	
    ABS_ENCODER_CSN (1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ƫ
//  @param      zero_position  ��Ҫ���õ���ƫ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void set_zero_position_simspi(uint16 zero_position)
{    
    zero_position = (uint16)(4096 - zero_position);
    zero_position = zero_position << 4;
    encoder_simspi_w_reg_byte(ZERO_L_REG,(uint8)zero_position);//������λ
    encoder_simspi_w_reg_byte(ZERO_H_REG,zero_position>>8);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      д��һ�����ݵ��������ļĴ���
//  @param      void
//  @return     uint16       ���ؽǶ�ֵ0-4096 ��Ӧ0-360��
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint16 encoder_angle_simspi(void)
{
    uint16 angle;

    ABS_ENCODER_CSN (0);
    angle = simspi_wr_byte(0x00);      
    angle <<= 8;                    //�洢�߰�λ
    angle |= simspi_wr_byte(0x00);  //�洢�Ͱ�λ
    ABS_ENCODER_CSN (1);   
    
    return (angle>>4);              //12λ���ȣ����������λ                 
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������Լ캯��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void encoder_self4_check(void)
{
    uint8 val;
    do
    {
        encoder_simspi_r_reg_byte(6,&val);
	
        //��������ԭ�������¼���
        //1 ���������ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
    }while(0x1C != val);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ʼ������
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void encoder_init_simspi(void)
{
    encoder_self4_check();					//�Լ�
    
    encoder_simspi_w_reg_byte(DIR_REG,0x00);//������ת���� ��ת��ֵ��С��0x00   ��ת��ֵ���0x80

    set_zero_position_simspi(0);             //������ƫ
}


//-------------------------����Ϊ���SPI--------------------------//
//-------------------------����Ϊ���SPI--------------------------//
//-------------------------����Ϊ���SPI--------------------------//


//-------------------------����ΪӲ��SPI--------------------------//
//-------------------------����ΪӲ��SPI--------------------------//
//-------------------------����ΪӲ��SPI--------------------------//

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ͨ��SPIдһ��byte,ͬʱ��ȡһ��byte
//  @param      byte        ���͵�����    
//  @return     uint8       return ����status״̬
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 spi_wr_byte(uint8 byte)
{  
	byte = spi_mosi(byte);
	
//	spi_mosi(ABS_ENCODER_SPI_NUM, SPI_CS_NOT_USE, &byte,&byte,1,1);
	return(byte);   
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��valд��cmd��Ӧ�ļĴ�����ַ,ͬʱ����status�ֽ�
//  @param      cmd         ������
//  @param      val         ��д��Ĵ�������ֵ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void spi_w_reg_byte(uint8 cmd, uint8 val)
{
    ABS_ENCODER_SPI_CSN (0);
    cmd |= ABS_ENCODER_SPI_W;
    spi_wr_byte(cmd);                      	
    spi_wr_byte(val);                               	
    ABS_ENCODER_SPI_CSN (1);       
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡcmd����Ӧ�ļĴ�����ַ
//  @param      cmd         ������
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void spi_r_reg_byte(uint8 cmd, uint8 *val)
{
    ABS_ENCODER_SPI_CSN (0);
    cmd |= ABS_ENCODER_SPI_R;
    spi_wr_byte(cmd);                               	
    *val = simspi_wr_byte(0);                           	
    ABS_ENCODER_SPI_CSN (1);                                    	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      д��һ�����ݵ��������ļĴ���
//  @param      cmd         �Ĵ�����ַ
//  @param      *val        д�����ݵĵ�ַ
//  @return     uint8       0������  1��ʧ��
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 encoder_spi_w_reg_byte(uint8 cmd, uint8 val)
{
    uint8 dat;
    ABS_ENCODER_SPI_CSN (0);
    cmd |= ABS_ENCODER_SPI_W;
    spi_wr_byte(cmd);                      	
    spi_wr_byte(val);                               	
    ABS_ENCODER_SPI_CSN (1);   
//    systick_delay_us(1);
    ABS_ENCODER_SPI_CSN (0);
    dat = spi_wr_byte(0x00);                      	
    spi_wr_byte(0x00);                               	
    ABS_ENCODER_SPI_CSN (1);  
    
    if(val != dat)  return 1;   //д��ʧ��
    return 0;                   //д��ɹ�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ�Ĵ���
//  @param      cmd         �Ĵ�����ַ
//  @param      *val        �洢��ȡ�����ݵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void encoder_spi_r_reg_byte(uint8 cmd, uint8 *val)
{
    ABS_ENCODER_SPI_CSN (0);
    cmd |= ABS_ENCODER_SPI_R;
    spi_wr_byte(cmd);                      	
    spi_wr_byte(0x00);   

    ABS_ENCODER_SPI_CSN (1);   
//    systick_delay_us(1);
    ABS_ENCODER_SPI_CSN (0);
    *val = spi_wr_byte(0x00);                      	
    spi_wr_byte(0x00);                               	
    ABS_ENCODER_SPI_CSN (1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ƫ
//  @param      zero_position  ��Ҫ���õ���ƫ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void set_zero_position_spi(uint16 zero_position)
{    
    zero_position = (uint16)(4096 - zero_position);
    zero_position = zero_position << 4;
    encoder_spi_w_reg_byte(ZERO_L_REG,(uint8)zero_position);//������λ
    encoder_spi_w_reg_byte(ZERO_H_REG,zero_position>>8);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      д��һ�����ݵ��������ļĴ���
//  @param      void
//  @return     uint16       ���ؽǶ�ֵ0-4096 ��Ӧ0-360��
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint16 encoder_angle_spi(void)
{
    uint16 angle;
	angle = 0;
    ABS_ENCODER_SPI_CSN (0);
    angle = (uint16)spi_wr_byte(0x00);      
    angle <<= 8;                    //�洢�߰�λ
    angle |= (uint16)spi_wr_byte(0x00);  //�洢�Ͱ�λ
    ABS_ENCODER_SPI_CSN (1);   
    
    return (angle>>4);              //12λ���ȣ����������λ                 
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������Լ캯��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void encoder_self5_check(void)
{
	uint8 val;
    do
    {
        encoder_spi_r_reg_byte(6,&val);
        //��������ԭ�������¼���
        //1 ���������ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
    }while(0x1C != val);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������ʼ������
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void encoder_init_spi(void)
{
	spi_init(ABS_ENCODER_SPI_NUM, ABS_ENCODER_SPI_SCK_PIN, ABS_ENCODER_SPI_MOSI_PIN,ABS_ENCODER_SPI_MISO_PIN, 0, MASTER, SPI_SYSclk_DIV_4);

	encoder_self5_check();
	encoder_spi_w_reg_byte(DIR_REG,0x00);//������ת���� ��ת��ֵ��С��0x00   ��ת��ֵ���0x80
	set_zero_position_spi(0);                   //������ƫ
}


//-------------------------����ΪӲ��SPI--------------------------//
//-------------------------����ΪӲ��SPI--------------------------//
//-------------------------����ΪӲ��SPI--------------------------//
