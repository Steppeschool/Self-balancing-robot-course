#ifndef INC_EULER_ANGLES_H_
#define INC_EULER_ANGLES_H_
#include "imu_data.h"

#define ALPHA  0.999

typedef struct{
	float32_t yaw;
	float32_t pitch;
	float32_t roll;
}euler_angles;

void acc_mag2euler(imu_norm imu_norm_data, euler_angles *euler_data);
void update_euler_gyro(imu_norm imu_norm_data,  euler_angles *euler_data);
void complementary_filter_euler(imu_norm imu_norm_data,  euler_angles *euler_out);


#endif /* INC_EULER_ANGLES_H_ */
