#include "euler_angles.h"
#include <arm_math.h>
#include "stdio.h"
#include "math.h"

void acc_mag2euler(imu_norm imu_norm_data, euler_angles *euler_accmag)
{
	float32_t pitch_sin, pitch_cos, roll_sin, roll_cos;
	float32_t ay_az_sqrt;

	arm_sqrt_f32(imu_norm_data.ay * imu_norm_data.ay +
			imu_norm_data.az * imu_norm_data.az, &ay_az_sqrt);

	// [pi,-pi]
	euler_accmag ->pitch = atan2(-imu_norm_data.ax, ay_az_sqrt);
	euler_accmag ->roll = atan2(imu_norm_data.ay, imu_norm_data.az);

	pitch_sin = -imu_norm_data.ax;
	pitch_cos = ay_az_sqrt;
	roll_sin = imu_norm_data.ay / ay_az_sqrt;
	roll_cos = imu_norm_data.az / ay_az_sqrt;

	euler_accmag ->yaw = atan2(imu_norm_data.mz * roll_sin -
			imu_norm_data.my * roll_cos, imu_norm_data.mx * pitch_cos + pitch_sin *
			(imu_norm_data.mz * roll_cos +
					imu_norm_data.my * roll_sin));
}

void update_euler_gyro(imu_norm imu_norm_data,  euler_angles *euler_accmag)
{
	float32_t pitch_sin, pitch_cos, roll_sin, roll_cos;
	pitch_sin = arm_sin_f32(euler_accmag  -> pitch);
	pitch_cos = arm_cos_f32(euler_accmag  -> pitch);
	roll_sin = arm_sin_f32(euler_accmag  -> roll);
	roll_cos = arm_cos_f32(euler_accmag  -> roll);
	euler_accmag -> roll += ((float32_t)imu_norm_data.gx + pitch_sin / pitch_cos *
			((float32_t)imu_norm_data.gy * roll_sin +
					(float32_t)imu_norm_data.gz * roll_cos));
	euler_accmag -> pitch += ((float32_t)imu_norm_data.gy * roll_cos
			- (float32_t)imu_norm_data.gz * roll_sin);
	euler_accmag -> yaw +=  ((float32_t)imu_norm_data.gy * roll_sin
			+ (float32_t)imu_norm_data.gz *roll_cos) / pitch_cos;

	pitch_cos = arm_cos_f32(euler_accmag  -> pitch);
	// alternative solution
	if(pitch_cos < 0)
	{
		euler_accmag  -> pitch = PI - euler_accmag  -> pitch;
		euler_accmag  -> roll = PI + euler_accmag  -> roll ;
		euler_accmag  -> yaw = PI + euler_accmag  -> yaw;
	}


	// [pi, -pi]
	if(euler_accmag -> roll > PI)
	{
		euler_accmag -> roll = euler_accmag -> roll - 2 * PI;
	}
	if(euler_accmag -> roll < -PI)
	{
		euler_accmag -> roll = euler_accmag -> roll + 2 * PI;
	}

	if(euler_accmag -> pitch > PI)
	{
		euler_accmag -> pitch = euler_accmag -> pitch - 2 * PI;
	}
	if(euler_accmag -> pitch < -PI)
	{
		euler_accmag -> pitch = euler_accmag -> pitch + 2 * PI;
	}

	if(euler_accmag -> yaw > PI)
	{
		euler_accmag -> yaw = euler_accmag -> yaw - 2 * PI;
	}
	if(euler_accmag -> yaw < -PI)
	{
		euler_accmag -> yaw = euler_accmag -> yaw + 2 * PI;
	}

}


void complementary_filter_euler(imu_norm imu_norm_data,  euler_angles *euler_out)
{
	static euler_angles euler_gyro;
	euler_angles euler_accmag;
	float32_t pitch_diff, roll_diff, yaw_diff;
	static uint8_t first_time = 0;
	if(first_time == 0)
	{
		acc_mag2euler(imu_norm_data, &euler_accmag);
		first_time = 1;
		euler_gyro = euler_accmag;
	}
	else
	{
		update_euler_gyro(imu_norm_data, &euler_gyro);
		acc_mag2euler(imu_norm_data, &euler_accmag);

		pitch_diff = euler_accmag.pitch - euler_gyro.pitch;
		yaw_diff = euler_accmag.yaw - euler_gyro.yaw;
		roll_diff = euler_accmag.roll - euler_gyro.roll;
		// [-pi,pi]
		if(pitch_diff > PI)
		{
			pitch_diff = 2*PI - pitch_diff;
		}
		if(pitch_diff < -PI)
		{
			pitch_diff = 2*PI + pitch_diff;
		}
		if(roll_diff > PI)
		{
			roll_diff = 2*PI - roll_diff;
		}
		if(roll_diff < -PI)
		{
			roll_diff = 2*PI + roll_diff;
		}
		if(yaw_diff > PI)
		{
			yaw_diff = 2*PI - yaw_diff;
		}
		if(yaw_diff < -PI)
		{
			yaw_diff = 2*PI + yaw_diff;
		}

		euler_gyro.pitch = euler_gyro.pitch + (1 - ALPHA) * pitch_diff;
		euler_gyro.roll = euler_gyro.roll + (1 - ALPHA) * roll_diff;
		euler_gyro.yaw = euler_gyro.yaw + (1 - ALPHA) * yaw_diff;
	}
	*euler_out = euler_gyro;
}


