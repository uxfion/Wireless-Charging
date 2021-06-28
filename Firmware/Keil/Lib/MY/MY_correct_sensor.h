#ifndef _SELFBUILD_CORRECT_SENSOR_H
#define _SELFBUILD_CORRECT_SENSOR_H

#include "headfile.h"

#define	FLASH_SAVE_MCP41	(EEPROM_PAGE_COUNT - 23)

#define ADC_R_R_Min_8bit 100	//五电感最小值
#define ADC_R_Min_8bit   100
#define ADC_M_Min_8bit   100
#define ADC_L_Min_8bit   100
#define ADC_L_L_Min_8bit 100


typedef enum
{
  L__ = 0,
  L_1,
  M_1,
  R_1,
  R__,
  ADC_NUM,
}ADC_POSITION;

void EEPROM_InitMCP41(void);
void Correct_Sensor(void);
void Normalized_MCP41(void);


#endif
