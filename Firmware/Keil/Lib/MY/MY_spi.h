/*!
 * @file		MY_spi.h
 * @brief		硬件SPI和软件SPI统一实现
 * @core		STC8A8K64S4A12-48
 * @author		HAOMING CHEN
 * @date		2020/03/14
 */

#ifndef __MY_SPI_H
#define __MY_SPI_H

#include "headfile.h"

void SPI_InitHardware(void);
void SPI_SendDataHardware(uint8 data1);
void SPI_InitSimulated(void);
void SPI_SendDataSimulated(uint8 data1);

#endif
