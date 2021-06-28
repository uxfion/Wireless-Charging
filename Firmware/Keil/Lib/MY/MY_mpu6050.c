/*!
 * @file       mpu6050.c
 * @brief      MPU6050函数实现
 * @author     刘力铭   //移植：董少俊 2020年4月11日
 */

#include "MY_mpu6050.h"
#include "MY_delay.h"

//struct MPU6050_Typedef MPU6050;


#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include <math.h>


/************************************************************************/
//函数:MPU6050初始化函数
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU6050_NODMPInit(void)
{
	IIC_init();
	Delay_Init();
  if(IIC_WriteBiteToSlave(MPU6050_ADDR, MPU6050_REGISTER_PWR_MGMT_1, 0x01))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteBiteToSlave(MPU6050_ADDR, MPU6050_REGISTER_SMPLRT_DIV, 0x04))return ERR_IIC_CHECKACK_FAIL;   //200Hz
  if(IIC_WriteBiteToSlave(MPU6050_ADDR, MPU6050_REGISTER_CONFIG, DLPF_CFG_BANDWIDTH_94))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteBiteToSlave(MPU6050_ADDR, MPU6050_REGISTER_ACCEL_CONFIG, AFS_SEL_2G))return ERR_IIC_CHECKACK_FAIL;
  if(IIC_WriteBiteToSlave(MPU6050_ADDR, MPU6050_REGISTER_GYRO_CONFIG, FS_SEL_2000))return ERR_IIC_CHECKACK_FAIL;
  return NO_ERR;
}

/************************************************************************/
//函数:MPU6050读取数据
//参数:Data_Type 读取数据的类型，data 为读取数据存放的地址
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU6050_ReadData(MPU6050_DATA_TypeDef Data_Type, int16_t *dat)
{
	uint8_t Register_Arr;
	uint8_t dat_h, dat_l;
	Register_Arr = MPU6050_REGISTER_ACCEL_XOUTH + Data_Type * 2;
	if(IIC_ReceiveSingleByte(MPU6050_ADDR, Register_Arr, &dat_h))return ERR_IIC_CHECKACK_FAIL;
	if(IIC_ReceiveSingleByte(MPU6050_ADDR, Register_Arr + 1, &dat_l))return ERR_IIC_CHECKACK_FAIL;
	*dat = (int16_t)((dat_h << 8) | dat_l);
	return NO_ERR;
}





#if MPU6050_DMP_EN == 1 //以下全为DMP函数

unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx);

static signed char gyro_orientation[9] = { 1, 0, 0,
                                              0, 1, 0,
                                              0, 0, 1};

/************************************************************************/
//函数:MPU6050初始化函数
//返回:0 = 成功,其他 = 失败
//注意:DMP默认配置陀螺仪量程为2000，加速度为2G
/************************************************************************/
uint8_t MPU6050_ADDDMPInit(void)
{
	uint8_t err=0;
	if(MPU6050_NODMPInit()==0)	//初始化MPU6050
	{
		err = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);//设置需要的传感器
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//设置FIFO
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = mpu_set_sample_rate(200);	//设置采样率200HZ
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = dmp_load_motion_driver_firmware();		//加载DMP固件
		if(err)return ERR_DMPINIT_FAIL; 
		err = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//设置陀螺仪方向
		if(err)return ERR_MPU6050INIT_FAIL; 
		err = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|DMP_FEATURE_GYRO_CAL);//设置DMP功能
		if(err)return ERR_DMPINIT_FAIL; 
		err = dmp_set_fifo_rate(100);	//设置DMP输出速率
		if(err)return ERR_DMPINIT_FAIL;   
//		err = MPU6050_SelfTest();		//自检，建议屏蔽否则每次初始化移动陀螺仪会导致自检失败
//		if(err)return ERR_SELTTEST_FAIL;    
		err = mpu_set_dmp_state(1);	//使能DMP
		if(err)return ERR_DMPINIT_FAIL;     
	}else return ERR_MPU6050INIT_FAIL;
	return NO_ERR;
}

/************************************************************************/
//函数:MPU6050自检函数
//返回:0 = 成功,其他 = 失败
/************************************************************************/
uint8_t MPU6050_SelfTest(void)
{
	int result;
	long gyro[3], accel[3]; 
	result = mpu_run_self_test(gyro, accel);
	if (result == 0x3) 
	{
		float sens;
		unsigned short accel_sens;
		mpu_get_gyro_sens(&sens);
		gyro[0] = (long)(gyro[0] * sens);
		gyro[1] = (long)(gyro[1] * sens);
		gyro[2] = (long)(gyro[2] * sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel_sens = 0;
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
		return NO_ERR;
	}else return ERR_SELTTEST_FAIL;
}


/************************************************************************/
//函数:MPU6050读取DMP处理之后的数据
//参数:pitch:俯仰角   精度:0.1°   范围: -90.0° <--->  +90.0°
//  	 roll :横滚角   精度:0.1°   范围:-180.0° <---> +180.0°
//		 yaw  :航向角   精度:0.1°   范围:-180.0° <---> +180.0°
//返回:0 = 成功,其他 = 失败
//备注:若读DMP时间太慢,会导致FIFO溢出,造成dmp_read_fifo()函数调用错误
//		 函数调用错误以后会自动调用mpu_reset_fifo();故解决办法为:1.定时
//		 器定时调用MPU6050_DMPGetData()2.在一次调用失败以后延时再一次调
//		 用
/************************************************************************/
uint8_t MPU6050_DMPGetData(float *pitch,float *roll,float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4];
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))return 1;
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		*pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		*roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return NO_ERR;
}

//方向转换
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

//陀螺仪方向控制
unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar; 
    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;
    return scalar;
}
#endif
/*
u8 Refresh_MPUTeam(MPU_DataTeam_TypeDef MPU_DataTeam)//更新MPU6050数据
{
  	switch(MPU_DataTeam)
	{
	  case ACCn:
		 MPU6050_ReadData(MPU6050_ACCEL_X, &MPU6050.ACC.x);
		 MPU6050_ReadData(MPU6050_ACCEL_Y, &MPU6050.ACC.y);
		 MPU6050_ReadData(MPU6050_ACCEL_Z, &MPU6050.ACC.z);break;
	  case GYRO:
		 MPU6050_ReadData(MPU6050_GYRO_X, &MPU6050.GYRO.x);
		 MPU6050_ReadData(MPU6050_GYRO_Y, &MPU6050.GYRO.y);
		 MPU6050_ReadData(MPU6050_GYRO_Z, &MPU6050.GYRO.z);break;
		case DMP:
#if MPU6050_DMP_EN == 1
		 MPU6050_DMPGetData(&MPU6050.Pitch, &MPU6050.Roll, &MPU6050.Yaw);
#endif
		break;
	  case ACC_X:
		 MPU6050_ReadData(MPU6050_ACCEL_X, &MPU6050.ACC.x);break;
	  case ACC_Y:
		 MPU6050_ReadData(MPU6050_ACCEL_Y, &MPU6050.ACC.y);break;
	  case ACC_Z:
		 MPU6050_ReadData(MPU6050_ACCEL_Z, &MPU6050.ACC.z);break;
	  case GYRO_X:
		 MPU6050_ReadData(MPU6050_GYRO_X,  &MPU6050.GYRO.x);break;
	  case GYRO_Y:
		 MPU6050_ReadData(MPU6050_GYRO_Y,  &MPU6050.GYRO.y);break;
	  case GYRO_Z:
		 MPU6050_ReadData(MPU6050_GYRO_Z,  &MPU6050.GYRO.z);break;
	  case ALL:
		 MPU6050_ReadData(MPU6050_ACCEL_X, &MPU6050.ACC.x);
		 MPU6050_ReadData(MPU6050_ACCEL_Y, &MPU6050.ACC.y);
		 MPU6050_ReadData(MPU6050_ACCEL_Z, &MPU6050.ACC.z);
		 MPU6050_ReadData(MPU6050_GYRO_X, &MPU6050.GYRO.x);
		 MPU6050_ReadData(MPU6050_GYRO_Y, &MPU6050.GYRO.y);
		 MPU6050_ReadData(MPU6050_GYRO_Z, &MPU6050.GYRO.z);
#if MPU6050_DMP_EN == 1
		 MPU6050_DMPGetData(&MPU6050.Pitch, &MPU6050.Roll, &MPU6050.Yaw);
#endif
		 break;
	  default : break;
	}
	return SUCCESS;
}

u8 Save_MPUTeam(MPU_DataTeam_TypeDef MPU_DataTeam)//保存MPU6050上次数据
{
  	switch(MPU_DataTeam)
	{
	case GYRO:
	       MPU6050.GYRO_Last.x=MPU6050.GYRO.x;
	       MPU6050.GYRO_Last.y=MPU6050.GYRO.y;
	       MPU6050.GYRO_Last.z=MPU6050.GYRO.z;break;
	case ACCn:
	       MPU6050.ACC_Last.x=MPU6050.ACC.x;
	       MPU6050.ACC_Last.y=MPU6050.ACC.y;
	       MPU6050.ACC_Last.z=MPU6050.ACC.z;break;
	case DMP:
#if MPU6050_DMP_EN == 1		
	       MPU6050.Pitch_Last=MPU6050.Pitch;
	       MPU6050.Roll_Last=MPU6050.Roll;
	       MPU6050.Yaw_Last=MPU6050.Yaw;
#endif
	       break;
	case ACC_X:
	       MPU6050.ACC_Last.x=MPU6050.ACC.x;break;
	case ACC_Y:
	       MPU6050.ACC_Last.y=MPU6050.ACC.y;break;
	case ACC_Z:
	       MPU6050.ACC_Last.z=MPU6050.ACC.z;break;
	case GYRO_X:
	       MPU6050.GYRO_Last.x=MPU6050.GYRO.x;break;
	case GYRO_Y:
	       MPU6050.GYRO_Last.y=MPU6050.GYRO.y;break;
	case GYRO_Z:
	       MPU6050.GYRO_Last.z=MPU6050.GYRO.z;break;
	case ALL:
	       MPU6050.GYRO_Last.x=MPU6050.GYRO.x;
	       MPU6050.GYRO_Last.y=MPU6050.GYRO.y;
	       MPU6050.GYRO_Last.z=MPU6050.GYRO.z;
	       MPU6050.ACC_Last.x=MPU6050.ACC.x;
	       MPU6050.ACC_Last.y=MPU6050.ACC.y;
	       MPU6050.ACC_Last.z=MPU6050.ACC.z;
#if MPU6050_DMP_EN == 1
	       MPU6050.Pitch_Last=MPU6050.Pitch;
	       MPU6050.Roll_Last=MPU6050.Roll;
	       MPU6050.Yaw_Last=MPU6050.Yaw;
#endif 
				 break;
	default : break;
	}
	return SUCCESS;
}


#define AcceRatio 	16384.0f	//加速度滤波系数
#define GyroRatio 	16.4f		//角速度滤波系数
#define Gyro_Gr		0.0010653	// 角速度变成弧度	此参数对应陀螺2000度每秒
#define ACC_FILTER_NUM 5		// 加速度计滤波深度
#define GYRO_FILTER_NUM 10		// 陀螺仪滤波深度
int32 ACC_X_BUF[ACC_FILTER_NUM], ACC_Y_BUF[ACC_FILTER_NUM], ACC_Z_BUF[ACC_FILTER_NUM];	// 滤波缓存数组
int32 GYRO_X_BUF[GYRO_FILTER_NUM], GYRO_Y_BUF[GYRO_FILTER_NUM], GYRO_Z_BUF[GYRO_FILTER_NUM];
void Data_Filter(void)	// 数据滤波
{
	u8 i;
	int32 temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0;
	
	ACC_X_BUF[0] = MPU6050.ACC.x;	// 更新滑动窗口数组
	ACC_Y_BUF[0] = MPU6050.ACC.y;
	ACC_Z_BUF[0] = MPU6050.ACC.z;
	GYRO_X_BUF[0] = MPU6050.GYRO.x;
	GYRO_Y_BUF[0] = MPU6050.GYRO.y;
	GYRO_Z_BUF[0] = MPU6050.GYRO.z;
	
	for(i=0;i<ACC_FILTER_NUM;i++)
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
		
	}
	for(i=0;i<GYRO_FILTER_NUM;i++)
	{
		temp4 += GYRO_X_BUF[i];
		temp5 += GYRO_Y_BUF[i];
		temp6 += GYRO_Z_BUF[i];
	}
	
	MPU6050.ACC_Real.x = temp1 / ACC_FILTER_NUM / AcceRatio;
	MPU6050.ACC_Real.y = temp2 / ACC_FILTER_NUM / AcceRatio;
	MPU6050.ACC_Real.z = temp3 / ACC_FILTER_NUM / AcceRatio;
	MPU6050.GYRO_Real.x = temp4 / GYRO_FILTER_NUM / GyroRatio;
	MPU6050.GYRO_Real.y = temp5 / GYRO_FILTER_NUM / GyroRatio;
	MPU6050.GYRO_Real.z = temp6 / GYRO_FILTER_NUM / GyroRatio;
	
	for(i = 0; i < ACC_FILTER_NUM - 1; i++)
	{
		ACC_X_BUF[ACC_FILTER_NUM-1-i] = ACC_X_BUF[ACC_FILTER_NUM-2-i];
		ACC_Y_BUF[ACC_FILTER_NUM-1-i] = ACC_Y_BUF[ACC_FILTER_NUM-2-i];
		ACC_Z_BUF[ACC_FILTER_NUM-1-i] = ACC_Z_BUF[ACC_FILTER_NUM-2-i];
		
	}
	for(i = 0; i < GYRO_FILTER_NUM - 1; i++)
	{
		GYRO_X_BUF[GYRO_FILTER_NUM-1-i] = GYRO_X_BUF[GYRO_FILTER_NUM-2-i];
		GYRO_Y_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Y_BUF[GYRO_FILTER_NUM-2-i];
		GYRO_Z_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Z_BUF[GYRO_FILTER_NUM-2-i];
	}
}
*/