#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

#include "headfile.h"
#define ON	1
#define OFF	0

#define LED P55
#define STAT P34
#define PG P35

#define SEND_CHARGE_DOWN P15
#define RECE_CHARGE_SUSP P14

//----OLED引脚----	 
#define SET_OLED_SCL_PIN	P25
#define SET_OLED_SDA_PIN	P23
#define SET_OLED_RST_PIN	P20
#define SET_OLED_DC_PIN		P21
#define SET_OLED_CS_PIN		P22

//#define SET_OLED_SCL_PIN	P15
//#define SET_OLED_SDA_PIN	P13
//#define SET_OLED_RST_PIN	P14
//#define SET_OLED_DC_PIN	P11
//#define SET_OLED_CS_PIN	P12


//#define SET_OLED_SDA_IN    SPI4_MISO_B20
//#define SET_OLED_SPI       SPI_4

////----MPU6050模拟IIC引脚-----
//#define SET_MPU6050_SCL_PIN     A14
//#define SET_MPU6050_SDA_PIN     A13

////-----------电感--------------
#define AD_CH_L__  ADC_P17
#define AD_CH_L_1  ADC_P00
#define AD_CH_M_1  ADC_P01
#define AD_CH_R_1  ADC_P02
#define AD_CH_R__  ADC_P03
#define AD_CH_L_2  0xff
#define AD_CH_R_2  0xff

#define SET_MCP41_SDA_PIN  P41
#define SET_MCP41_SCL_PIN  P42

#define SET_MCP41_CS0  P4_0
#define SET_MCP41_CS1  P2_6
#define SET_MCP41_CS2  P2_4
#define SET_MCP41_CS3  P2_2
#define SET_MCP41_CS4  P2_0
#define SET_MCP41_CS5  0xff
#define SET_MCP41_CS6  0xff
#define SET_MCP41_CS7  0xff

#define SET_MCP41_P0  AD_CH_L__
#define SET_MCP41_P1  AD_CH_L_1
#define SET_MCP41_P2  AD_CH_M_1
#define SET_MCP41_P3  AD_CH_R_1
#define SET_MCP41_P4  AD_CH_R__
#define SET_MCP41_P5  0xff
#define SET_MCP41_P6  0xff
#define SET_MCP41_P7  0xff

////----按键----
///******************
//#define BUTTON_UP    B26
//#define BUTTON_DOWN  B13
//#define BUTTON_LEFT  P41  
//#define BUTTON_RIGHT P40
//#define BUTTON_MID   P42
//*******************/
///********板子翻转**********/
//#define BUTTON_UP    B13
//#define BUTTON_DOWN  B26
#define BUTTON_LEFT  P40
#define BUTTON_RIGHT P42
#define BUTTON_MID   P41


////----拨码开关----
//#define SWITCH1_1 A1
//#define SWITCH1_2 B4	
//#define SWITCH1_3 B5 
//#define SWITCH1_4 B6

//#define SWITCH2_1 B23
//#define SWITCH2_2 B24	
//#define SWITCH2_3 B2 
//#define SWITCH2_4 B25

////----蜂鸣器----
#define BUZZER   P44
#define Buzzer_On   BUZZER = 1
#define Buzzer_Off  BUZZER = 0

////舵机
////#define Servo  TIMER4_PWMCH0_A6

//电机PWM
#define PWM_CH_L_Z  PWM0_P60
#define PWM_CH_L_F  PWM1_P61
#define PWM_CH_R_Z  PWM2_P62
#define PWM_CH_R_F  PWM3_P63

//编码器
#define CODER_L         CTIM3_P04
#define CODER_R         CTIM4_P06
#define CODER_DIR_L     P00
#define CODER_DIR_R     P01

////超声波
////#define SONIC_Trig	B3
////#define SONIC_Echo	B1

//////----------串口---------
//#define Bluetooth_UART     USART_8
//#define Bluetooth_UART_TXD UART8_TX_B18
//#define Bluetooth_UART_RXD UART8_RX_B17

////----------外设------------
//#define LASER   A26
//#define Laser_Get   gpio_get(LASER)

//#define	Laser_Hengduan		ADC_CH11_A23//(ADC)				
//#define	Laser_Podao				A24//激光
//#define	Laser_Duanlu			A9
//#define Reed_Switch  			A25
#endif
