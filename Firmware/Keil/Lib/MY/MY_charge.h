#ifndef __MY_CHARGE_H_
#define __MY_CHARGE_H_

#include "headfile.h"

#define TARGET_CAP_VOL 12.5
extern float 	charge_vol,
				charge_cur,
				cap_vol,
				i_set,
				actual_power;
extern float	PowerControl_Out_New,
				target_power,
				power_p,
				power_i,
				power_d;
extern uint8 FLAG_CHARGE_DOWN,
			FLAG_CHARGE_ERROR,
			FLAG_POWER_PLUS,
			FLAG_POWER_SUBTRACT;

extern float error[3];
extern float ad_value;

extern float power_out;

void get_power(void);
void power_control(void);
void pi_incre(void);
void control(void);









#endif