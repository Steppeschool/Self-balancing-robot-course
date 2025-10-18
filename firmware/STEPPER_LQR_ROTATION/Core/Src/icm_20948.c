#include "icm_20948.h"
#include "stdio.h"

static void sel_user_bank(user_bank ub);
static void ak09916_mag_init();
static void ak09916_read_reg(uint8_t onset_reg, uint8_t len);
static void ak09916_write_reg(uint8_t reg, uint8_t data);
static void remove_gyro_bias();

static void activate_imu()
{
	HAL_GPIO_WritePin(IMU_CS_PORT, IMU_CS_PIN,
			GPIO_PIN_RESET);
}
static void deactivate_imu()
{
	HAL_GPIO_WritePin(IMU_CS_PORT, IMU_CS_PIN,
				GPIO_PIN_SET);
}
static void sel_user_bank(user_bank ub)
{
	uint8_t data = ub;
	uint8_t reg = REG_BANK_SEL;
	activate_imu();
	HAL_SPI_Transmit(&IMU_SPI, &reg, 1, 100);
	HAL_SPI_Transmit(&IMU_SPI, &data, 1, 100);
	deactivate_imu();
}
void icm_20948_read_reg(user_bank ub, uint8_t address, uint8_t *data)
{
	uint8_t temp_data = 0x80|address;
	sel_user_bank(ub);
	activate_imu();
	HAL_SPI_Transmit(&IMU_SPI, &temp_data , 1, 100);
	HAL_SPI_Receive(&IMU_SPI, data, 1, 100);
	deactivate_imu();
}

void icm_20948_write_reg(user_bank ub, uint8_t reg, uint8_t data)
{
	sel_user_bank(ub);
	activate_imu();
	HAL_SPI_Transmit(&IMU_SPI, &reg, 1, 100);
	HAL_SPI_Transmit(&IMU_SPI, &data, 1, 100);
	deactivate_imu();
}
void icm_20948_init()
{
	uint8_t temp_data;
	// IMU reset-sleep-clock, page 37, 0xc1 = 0b1100 0001
	icm_20948_write_reg(_b0, PWR_MGMT_1, 0xc1);
	HAL_Delay(100);
	// Exit from sleep mode, selecting the clock page 37, 0x01 = 0b0000 0001
	icm_20948_write_reg(_b0, PWR_MGMT_1, 0x01);

	// Accelerometer configuration, sample rate divider = 0, page 63
	icm_20948_write_reg(_b2, ACCEL_SMPLRT_DIV_1, 0x00);
	icm_20948_write_reg(_b2, ACCEL_SMPLRT_DIV_2, 0x00);

	// Accelerometer configuration, accelerometer range set and enable digital filter, page 64
	icm_20948_write_reg(_b2, ACCEL_CONFIG, ((ACCEL_RANGE_VALUE << 1)|0x01)|(0x02 << 3));

	// Put the serial interface in SPI mode only, page 36, 0x10 = 0b0001 0000
	temp_data = 0x10;
	icm_20948_write_reg(_b0, USER_CTRL, temp_data);

	sel_user_bank(_b0);
	remove_gyro_bias();

	// Gyroscope configuration, sample rate divider = 0, page 59

	icm_20948_write_reg(_b2, GYRO_SMPLRT_DIV, 0x00);

	// Gyroscope configuration, gyroscope range set and enable digital filter, page 59
	// enable dlpf,
	icm_20948_write_reg(_b2, GYRO_CONFIG_1, ((GYRO_RANGE_VALUE << 1)|0x01|(0x02 << 3)));

	ak09916_mag_init();
	ak09916_read_reg(MAG_DATA_ONSET, 8);

	sel_user_bank(_b0);

}

uint8_t icm_20948_read_data(icm_20948_data* data)
{
	static uint8_t data_rx[20];
	uint8_t temp_data = 0x80|ACCEL_XOUT_H;

	activate_imu();
	HAL_SPI_Transmit(&IMU_SPI, &temp_data, 1, 1000);
	HAL_SPI_Receive(&IMU_SPI, data_rx, 20, 100);
	deactivate_imu();
	// since we changed the orientation of the board
	// we have to swap the x and z axes
	data ->z_accel = -((int16_t)data_rx[0]<<8)| (int16_t)data_rx[1];
	data ->x_accel = ((int16_t)data_rx[2]<<8)| (int16_t) data_rx[3];
	data ->y_accel = -((int16_t)data_rx[4]<<8)| (int16_t) data_rx[5];

	data ->z_gyro = -((int16_t)data_rx[6]<<8) | (int16_t) data_rx[7];
	data ->x_gyro = ((int16_t)data_rx[8]<<8) | (int16_t)data_rx[9];
	data ->y_gyro = -((int16_t)data_rx[10]<<8)| (int16_t)data_rx[11];

	data ->z_magnet = -(((int16_t)data_rx[15]<<8) | (int16_t)data_rx[14]) - MAG_X_BIAS;
	data ->x_magnet = (((int16_t)data_rx[17]<<8) | (int16_t)data_rx[16]) - MAG_Y_BIAS;
	data ->y_magnet = -(((int16_t)data_rx[19]<<8) | (int16_t)data_rx[18]) - MAG_Z_BIAS;



	return 0;
}

static void ak09916_mag_init()
{
	uint8_t temp_data;
	// I2C master reset, page 36
	icm_20948_read_reg(_b0, USER_CTRL, &temp_data);
	temp_data |= 0x02;
	icm_20948_write_reg(_b0, USER_CTRL, temp_data);
	HAL_Delay(100);
	// I2C Master enable, page 36
	icm_20948_read_reg(_b0, USER_CTRL, &temp_data);
	temp_data |= 0x20;
	icm_20948_write_reg(_b0, USER_CTRL, temp_data);
	HAL_Delay(10);
	// I2C Master clock: 7 (400 kHz), page 68
	temp_data = 0x07;
	icm_20948_write_reg(_b3, I2C_MST_CTRL, temp_data);
	HAL_Delay(10);
	// LP_CONFIG:ODR is determined by I2C_MST_ODR_CONFIG register,page 37
	temp_data = 0x40;
	icm_20948_write_reg(_b0, LP_CONFIG, temp_data);
	HAL_Delay(10);
	// I2C_MST_ODR_CONFIG: 1.1 kHz/(2^3) = 136 Hz, page 68
	temp_data = 0x03;
	icm_20948_write_reg(_b3, I2C_MST_ODR_CONFIG, temp_data);
	HAL_Delay(10);
	// I2C_MST_DELAY_CTRL: delays shadowing of external sensors, page 69
	temp_data = 0x80;
	icm_20948_write_reg(_b3, I2C_MST_DELAY_CTRL, temp_data);
	HAL_Delay(10);
	// Magnetometer reset, page, page 80
	ak09916_write_reg(MAG_CTRL3, 0x01);
	HAL_Delay(100);
	// continuous mode 4: 100 Hz, page 79
	ak09916_write_reg(MAG_CTRL2, 0x08);
}

static void ak09916_write_reg(uint8_t reg, uint8_t data)
{
	icm_20948_write_reg(_b3, I2C_SLV0_ADDR, AK09916_ADDRESS);
	icm_20948_write_reg(_b3, I2C_SLV0_REG , reg);
	icm_20948_write_reg(_b3, I2C_SLV0_DO , data);
	// Enable and single data write
	HAL_Delay(50);
	icm_20948_write_reg(_b3, I2C_SLV0_CTRL, 0x80|0x01);
	HAL_Delay(50);
}

static void ak09916_read_reg(uint8_t onset_reg, uint8_t len)
{
	icm_20948_write_reg(_b3, I2C_SLV0_ADDR, 0x80|AK09916_ADDRESS);
	icm_20948_write_reg(_b3, I2C_SLV0_REG , onset_reg);
	HAL_Delay(50);
	icm_20948_write_reg(_b3, I2C_SLV0_CTRL, 0x80|len);
	HAL_Delay(50);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi == &IMU_SPI)
	{
		deactivate_imu();
	}
}

static void remove_gyro_bias()
{
	int16_t x_gyro_bias, y_gyro_bias, z_gyro_bias;
	icm_20948_data data;
	int32_t x_bias = 0, y_bias = 0, z_bias = 0;
	icm_20948_read_data(&data);
	HAL_Delay(2);
	icm_20948_read_data(&data);
	HAL_Delay(2);
	// since we changed the orientation of the board
	// we have to swap the x and z axes
	for(int i = 0; i < 100; i++)
	{
		icm_20948_read_data(&data);
		x_bias -= (int32_t)data.z_gyro;

		y_bias += (int32_t)data.x_gyro;
		z_bias -= (int32_t)data.y_gyro;
		HAL_Delay(2);
	}

	x_gyro_bias = -(int16_t)(x_bias / 400);
	y_gyro_bias = -(int16_t)(y_bias / 400);
	z_gyro_bias = -(int16_t)(z_bias / 400) + 40;
	printf("x,y,z %d, %d, %d \n", x_gyro_bias, y_gyro_bias, z_gyro_bias);
	HAL_Delay(100);
	icm_20948_write_reg(_b2, XG_OFFS_USRH, (uint8_t)(x_gyro_bias >> 8));
	icm_20948_write_reg(_b2, XG_OFFS_USRL, (uint8_t)(x_gyro_bias));
	icm_20948_write_reg(_b2, YG_OFFS_USRH, (uint8_t)(y_gyro_bias >> 8));
	icm_20948_write_reg(_b2, YG_OFFS_USRL, (uint8_t)(y_gyro_bias));
	icm_20948_write_reg(_b2, ZG_OFFS_USRH, (uint8_t)(z_gyro_bias >> 8));
	icm_20948_write_reg(_b2, ZG_OFFS_USRL, (uint8_t)(z_gyro_bias));
}
