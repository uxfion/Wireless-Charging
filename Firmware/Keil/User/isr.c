#include "isr.h"

//UART1中断
void UartIsr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
		
//		接收数据寄存器为：SBUF
//        res = SBUF;
		
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
//代码软复位到ISP监控程序区，实现自动下载
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
		
    }
}

//UART2中断
void Uart2Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF

	}
}


//UART3中断
void Uart3Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF

	}
}


//UART4中断

void Uart4Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;
		//接收数据寄存器为：S4BUF;

	}
}


uint8 time = 0;
//PCA周期定时器中断
void PCA_Isr() interrupt 7
{
	static uint16 c = 0;
	if(PCA0_GET_FLAG)
	{
		//清除中断标志位
		PCA_CLEAR_FLAG(PCA_0);	

		//重载计数器
		pca_reload_counter(PCA_0);
		
		if(++c == 1000)
		{
			if(cap_vol <= TARGET_CAP_VOL && actual_power > 4) time++;
			c = 0;
		}
		get_power();
//		power_control();
		pi_incre();
//		control();		//没用
	}
	
	if(PCA1_GET_FLAG)
	{
		//清除中断标志位
		PCA_CLEAR_FLAG(PCA_1);	
		//重载计数器
		pca_reload_counter(PCA_1);

	}
	


	if(PCA2_GET_FLAG)
	{
		//清除中断标志位
		PCA_CLEAR_FLAG(PCA_2);	
		//重载计数器
		pca_reload_counter(PCA_2);

	}
	
//	PCA_3已做延时
	if(PCA3_GET_FLAG)
	{
		//清除中断标志位
		PCA_CLEAR_FLAG(PCA_3);	
		
//		//重载计数器
//		pca_reload_counter(PCA_3);
		 
	}
	
}

void INT0_Isr() interrupt 0
{
	//此标志位，硬件自动清零。
	//INT0_CLEAR_FLAG;

}
void INT1_Isr() interrupt 2
{
	//此标志位，硬件自动清零。
	//INT1_CLEAR_FLAG;

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;
	
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;
	
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;
	
}






bit data
	FLAG_2MS=0,
	FLAG_5MS=0,
	FLAG_10MS=0,
	FLAG_50MS=0,
	FLAG_100MS=0,
	FLAG_200MS=0,
	FLAG_500MS=0,
	FLAG_1S=0;

void TM0_Isr() interrupt 1
{
	static uint8
		add_2ms   = 0,
		add_5ms   = 0,
		add_10ms  = 0,
		add_50ms  = 0,
		add_100ms = 0,
		add_200ms = 0;
	static uint16
		add_500ms = 0,
		add_1000ms= 0;
	
	if(++add_2ms   == 2  ) {add_2ms   = 0; FLAG_2MS   = 1;}
	if(++add_5ms   == 5  ) {add_5ms   = 0; FLAG_5MS   = 1;}
	if(++add_10ms  == 10 ) {add_10ms  = 0; FLAG_10MS  = 1;}
	if(++add_50ms  == 50 ) {add_50ms  = 0; FLAG_50MS  = 1;}
	if(++add_100ms == 100) {add_100ms = 0; FLAG_100MS = 1;}
	if(++add_200ms == 200) {add_200ms = 0; FLAG_200MS = 1;}
	if(++add_500ms == 500) {add_500ms = 0; FLAG_500MS = 1;}
	if(++add_1000ms==1000) {add_1000ms= 0; FLAG_1S = 1;}

	TF0 = 0;	//清除T0中断溢出标志位, 硬件清0，可不加	
}


//void INT0_Routine(void) interrupt 0
//void TM0_Rountine(void) interrupt 1
//void INT1_Routine(void) interrupt 2
//void TM1_Rountine(void) interrupt 3
//void UART1_Routine(void) interrupt 4
//void ADC_Routine(void) interrupt 5
//void LVD_Routine(void) interrupt 6
//void PCA_Routine(void) interrupt 7
//void UART2_Routine(void) interrupt 8
//void SPI_Routine(void) interrupt 9
//void INT2_Routine(void) interrupt 10
//void INT3_Routine(void) interrupt 11
//void TM2_Routine(void) interrupt 12
//void INT4_Routine(void) interrupt 16
//void UART3_Routine(void) interrupt 17
//void UART4_Routine(void) interrupt 18
//void TM3_Routine(void) interrupt 19
//void TM4_Routine(void) interrupt 20
//void CMP_Routine(void) interrupt 21
//void PWM_Routine(void) interrupt 22
//void PWMFD_Routine(void) interrupt 23
//void I2C_Routine(void) interrupt 24