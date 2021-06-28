 /*!
 * @文件       mcp41.h
 * @功能       mcp41数字电位器函数实现
 * @作者       刘力铭
 * @完成时间   2019-12
 */
 
#ifndef __MY_MCP41_H
#define __MY_MCP41_H

#include "headfile.h"

/****************** SPI引脚选择 ******************/
#define MCP41_SCL_Pin    P41
#define MCP41_SDA_Pin    P42


/****************** 片选引脚选择 ******************/
#define MCP41_CS0       SET_MCP41_CS0  
#define MCP41_CS1       SET_MCP41_CS1
#define MCP41_CS2       SET_MCP41_CS2
#define MCP41_CS3       SET_MCP41_CS3
#define MCP41_CS4       SET_MCP41_CS4
#define MCP41_CS5       SET_MCP41_CS5
#define MCP41_CS6       SET_MCP41_CS6
#define MCP41_CS7       SET_MCP41_CS7

/****************** ADC通道选择 ******************/
#define MCP41_P0       SET_MCP41_P0
#define MCP41_P1       SET_MCP41_P1
#define MCP41_P2       SET_MCP41_P2
#define MCP41_P3       SET_MCP41_P3
#define MCP41_P4       SET_MCP41_P4
#define MCP41_P5       SET_MCP41_P5
#define MCP41_P6       SET_MCP41_P6
#define MCP41_P7       SET_MCP41_P7

void MCP41_Init(void);
void MCP41_SetCmd(uint8 Set_Num ,uint8 dat);
uint8 MCP41_SetValue(uint8 Set_Num, uint8 Ask_Value);

#endif
