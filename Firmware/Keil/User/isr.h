#ifndef __ISR_H_
#define __ISR_H_

#include "headfile.h"

extern bit	//时间标志位
	FLAG_2MS,
	FLAG_5MS,
	FLAG_10MS,
	FLAG_50MS,
	FLAG_100MS,
	FLAG_200MS,
	FLAG_500MS,
	FLAG_1S;

extern uint8 time;

#endif