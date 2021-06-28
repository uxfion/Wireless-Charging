#include "process.h"

#define GyroRatio1 	16.4f		//角速度滤波系数
#define GYRO_FILTER_NUM1 5		// 陀螺仪滤波深度

float  gyrox,gyroy,gyroz;
float  pitch,roll,yaw;



float data gyrox_read,gyroy_read,gyroz_read;
int16 data gyrox_temp=0,gyroy_temp=0,gyroz_temp=0;
int32 data gyrox_buf[GYRO_FILTER_NUM1], gyroy_buf[GYRO_FILTER_NUM1], gyroz_buf[GYRO_FILTER_NUM1];

void DataRead(void)
{
	static float data OldRoll = 0, OldPitch = 0, OldYaw = 0;
	int32 data temp4 = 0, temp5 = 0, temp6 = 0;
	u8 data i;
	
	//读取姿态角
	if(MPU6050_DMPGetData(&pitch, &roll, &yaw))		
	{
	   pitch = OldPitch;
		 roll= OldRoll;
		 yaw = OldYaw;
	}
	 OldPitch = pitch;
	 OldRoll = roll;
	 OldYaw = yaw;
	
	//读取三轴加速度
	MPU6050_ReadData(MPU6050_GYRO_X ,&gyrox_temp);
	MPU6050_ReadData(MPU6050_GYRO_Y ,&gyroy_temp);
	MPU6050_ReadData(MPU6050_GYRO_Z ,&gyroz_temp);
	//原始数据的转化
	gyrox_read = (gyrox_temp) / 16.384 - 0.2259;
    gyroy_read = (gyroy_temp) / 16.384 - 0.2259;
	gyroz_read = (gyroz_temp) / 16.384 - 0.2259+1;
	//滑动滤波
	gyrox_buf[0] = gyrox_read;
	gyroy_buf[0] = gyroy_read;
	gyroz_buf[0] = gyroz_read;

	for(i=0;i<GYRO_FILTER_NUM1;i++)
	{
		temp4 += gyrox_buf[i];
		temp5 += gyroy_buf[i];
		temp6 += gyroz_buf[i];
	}

	gyrox = temp4 / GYRO_FILTER_NUM1 / GyroRatio1;
	gyroy = temp5 / GYRO_FILTER_NUM1 / GyroRatio1;
	gyroz = temp6 / GYRO_FILTER_NUM1 / GyroRatio1;
	
	for(i = 0; i < GYRO_FILTER_NUM1 - 1; i++)
	{
		gyrox_buf[GYRO_FILTER_NUM1-1-i] = gyrox_buf[GYRO_FILTER_NUM1-2-i];
		gyroy_buf[GYRO_FILTER_NUM1-1-i] = gyroy_buf[GYRO_FILTER_NUM1-2-i];
		gyroz_buf[GYRO_FILTER_NUM1-1-i] = gyroz_buf[GYRO_FILTER_NUM1-2-i];
	} 
}


