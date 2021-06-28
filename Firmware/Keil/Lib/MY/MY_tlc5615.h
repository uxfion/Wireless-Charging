#ifndef __MY_TLC5615_H
#define __MY_TLC5615_H

#include "headfile.h"

#define TLC5615_DIN  P10
#define TLC5615_SCLK P11
#define TLC5615_CS   P12

#define VREF 3.30

void Set_TLC5615(float voltage);

#endif
