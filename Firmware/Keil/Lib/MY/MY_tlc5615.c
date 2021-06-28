#include "MY_tlc5615.h"

void Set_TLC5615(float voltage)
{
	uint8 temp = 12;
	uint16 dat;
	dat = (uint16)(voltage /0.977 * 512 / VREF);
	dat <<= 2;
	
	TLC5615_CS = 0;
	while(temp--)
	{
		TLC5615_SCLK = 0;
		TLC5615_DIN = (dat & 0x800)?1:0;
		TLC5615_SCLK = 1;
		dat <<= 1;
	}
	TLC5615_CS = 1;
	
}
