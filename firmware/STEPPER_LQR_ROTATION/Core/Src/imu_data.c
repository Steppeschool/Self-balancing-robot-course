#include "imu_data.h"
#include "stdio.h"

void sensor2imu(icm_20948_data icm_data, imu_norm* imu)
{
	float32_t norm_acc, norm_mag;
	arm_status arm_status_temp;

	arm_status_temp = arm_sqrt_f32((float32_t)icm_data.x_accel * icm_data.x_accel + (float32_t)icm_data.y_accel * icm_data.y_accel
			+ icm_data.z_accel * icm_data.z_accel, &norm_acc);

	if(arm_status_temp != ARM_MATH_SUCCESS)
	{
		printf("error sqrt! %d \n", arm_status_temp);
		while(1);
	}
	arm_status_temp = arm_sqrt_f32((float32_t)icm_data.x_magnet * icm_data.x_magnet
			+ (float32_t)icm_data.y_magnet * icm_data.y_magnet
			+ (float32_t)icm_data.z_magnet * icm_data.z_magnet, &norm_mag);

	if(arm_status_temp != ARM_MATH_SUCCESS)
	{
		printf("error sqrt! %d \n", arm_status_temp);
		while(1);
	}
	imu->ax = -((float32_t)icm_data.x_accel) / norm_acc;
	imu->ay = ((float32_t)icm_data.y_accel) / norm_acc;
	imu->az = ((float32_t)icm_data.z_accel) / norm_acc;

	imu->gx = ((float32_t)icm_data.x_gyro) * GYRO_SCALE;
	imu->gy = -((float32_t)icm_data.y_gyro) * GYRO_SCALE;
	imu->gz = -((float32_t)icm_data.z_gyro) * GYRO_SCALE;

	imu->mx = ((float32_t)icm_data.x_magnet) / norm_mag;
	imu->my = ((float32_t)icm_data.y_magnet) / norm_mag;
	imu->mz = ((float32_t)icm_data.z_magnet) / norm_mag;
}
