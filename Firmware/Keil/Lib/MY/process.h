#ifndef PROCESS_H
#define PROCESS_H

#include "STC8Axx.h"
#include "MY_mpu6050.h"
#include "common.h"

extern float gyrox,gyroy,gyroz;
extern float pitch,roll,yaw;

void DataRead(void);

#endif