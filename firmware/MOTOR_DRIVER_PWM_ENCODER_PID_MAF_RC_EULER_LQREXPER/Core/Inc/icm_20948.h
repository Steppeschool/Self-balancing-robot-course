#ifndef INC_ICM_20948_H_
#define INC_ICM_20948_H_

#include <stdint.h>
#include <main.h>

typedef struct{
	int16_t x_accel;
	int16_t y_accel;
	int16_t z_accel;
	int16_t x_gyro;
	int16_t y_gyro;
	int16_t z_gyro;
	int16_t x_magnet;
	int16_t y_magnet;
	int16_t z_magnet;
}icm_20948_data;

/* User initialization */
// These are parameters to choose before running the code
extern SPI_HandleTypeDef   hspi2;

#define IMU_SPI   				hspi2
#define IMU_CS_PORT  			ICM_SPI_CS_GPIO_Port
#define IMU_CS_PIN   			ICM_SPI_CS_Pin
#define GYRO_RANGE_VALUE        _gyro_500dps
#define ACCEL_RANGE_VALUE       _accel_8g

// 345
/* User initialization */
#define REG_BANK_SEL     0x7f
#define ODR_ALIGN_EN     0x09
#define WHO_AM_I         0
#define PWR_MGMT_1       0x06
#define USER_CTRL        0x03
#define GYRO_SMPLRT_DIV  0x00
#define GYRO_CONFIG_1    0x01
#define GYRO_CONFIG_2    0x02

#define ACCEL_SMPLRT_DIV_1 0x10
#define ACCEL_SMPLRT_DIV_2 0x10
#define ACCEL_CONFIG       0x14

#define ACCEL_XOUT_H     0x2d
#define ACCEL_XOUT_L     0x2e
#define ACCEL_YOUT_H     0x2f
#define ACCEL_YOUT_L     0x30
#define ACCEL_ZOUT_H     0x31
#define ACCEL_ZOUT_L     0x32

#define GYRO_XOUT_H      0x33
#define GYRO_XOUT_L      0x34
#define GYRO_YOUT_H      0x35
#define GYRO_YOUT_L      0x36
#define GYRO_ZOUT_H      0x37
#define GYRO_ZOUT_L      0x38


#define XG_OFFS_USRH     0x03
#define XG_OFFS_USRL     0x04
#define YG_OFFS_USRH     0x05
#define YG_OFFS_USRL     0x06
#define ZG_OFFS_USRH     0x07
#define ZG_OFFS_USRL     0x08

// I2C0 registers
#define I2C_MST_CTRL       0x01
#define I2C_MST_DELAY_CTRL 0x02
#define I2C_SLV0_ADDR      0x03
#define I2C_SLV0_REG       0x04
#define I2C_SLV0_CTRL      0x05
#define I2C_SLV0_DO        0x06
#define I2C_MST_ODR_CONFIG 0x00
#define LP_CONFIG          0x05

// I2C0 registers
#define I2C_MST_CTRL 0x01
#define I2C_MST_DELAY_CTRL 0x02
#define I2C_SLV0_ADDR 0x03
#define I2C_SLV0_REG 0x04
#define I2C_SLV0_CTRL 0x05
#define I2C_SLV0_DO 0x06
#define I2C_MST_ODR_CONFIG 0x00
#define LP_CONFIG 0x05

// Magnetometer registers
#define MAG_DEVICE_ID 0x01
#define MAG_STATUS1 0x10
#define MAG_DATA_ONSET 0x11
#define MAG_STATUS2 0x18
#define MAG_CTRL2 0x31
#define MAG_CTRL3 0x32

// Magnetometer address
#define AK09916_ADDRESS 0x0c

// define Magnetometer biases
#define MAG_X_BIAS    2400
#define MAG_Y_BIAS    384
#define MAG_Z_BIAS    9434
typedef enum
{
	_gyro_250dps =  0x00,
	_gyro_500dps =  0x01,
	_gyro_1000dps = 0x02,
	_gyro_2000dps = 0x03
} gyro_range;

typedef enum
{
	_accel_2g =  0x00,
	_accel_4g =  0x01,
	_accel_8g =  0x02,
	_accel_16g = 0x03
} accel_range;

typedef enum
{
	_b0 = 0,
	_b1 = 1 << 4,
	_b2 = 2 << 4,
	_b3 = 3 << 4
}user_bank;

void icm_20948_init();
void icm_20948_write_reg(user_bank ub, uint8_t reg, uint8_t data);
void icm_20948_read_reg(user_bank ub, uint8_t address, uint8_t *data);
uint8_t icm_20948_read_data(icm_20948_data* data);

#endif /* INC_ICM_20948_H_ */
