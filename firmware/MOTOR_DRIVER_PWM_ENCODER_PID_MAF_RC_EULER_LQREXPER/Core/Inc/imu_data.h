#ifndef INC_IMU_DATA_H_
#define INC_IMU_DATA_H_

#include "main.h"
#include "icm_20948.h"
#include <arm_math.h>

// delta t = 1 / SAMPLE RATE
#define SAMPLE_RATE   100
// (pi / 180) * (250 / 32767) = 0.0001331621
#define GYRO_SCALE    (0.0001331621 * pow(2, GYRO_RANGE_VALUE))/ (SAMPLE_RATE)


typedef struct
{
	float32_t ax;
	float32_t ay;
	float32_t az;
	float32_t mx;
	float32_t my;
	float32_t mz;
	float32_t gx;
	float32_t gy;
	float32_t gz;
}imu_norm;

void sensor2imu(icm_20948_data icm_data, imu_norm* imu);
#endif
