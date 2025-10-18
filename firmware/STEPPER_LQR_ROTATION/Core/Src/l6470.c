/*
 * l6470.c
 *
 *  Created on: Dec 1, 2024
 *      Author: yerke
 */
#include <l6470.h>

static void simple_delay(uint32_t delay_count);
static void l6470_receive_spi(MotorSetTypedef* stepper_motor, uint8_t* data, uint8_t data_length);
static void l6470_set_param(MotorSetTypedef* stepper_motor, uint8_t param, uint8_t *data, uint8_t data_length);
static void l6470_transmit_spi(MotorSetTypedef* stepper_motor, uint8_t* data, uint8_t data_length);
/*
 * @brief enable l6470 motor driver
 * @param stepper_motor: stepper motor handler
 */
void l6470_enable(MotorSetTypedef* stepper_motor)
{
//	uint8_t reg_temp;
//	reg_temp = RESET_DEVICE;
//	for(int i = 0; i < NUMBER_OF_MOTORS; i++)
//	{
//		reg_temp[i] = RESET_DEVICE;
//	}
//	l6470_transmit_spi(stepper_motor, &reg_temp, NUMBER_OF_MOTORS);
}
/*
 * @brief disable l6470 motor driver
 * @param stepper_motor: stepper motor handler
 */
void l6470_disable(MotorSetTypedef* stepper_motor)
{
	uint8_t reg_temp[NUMBER_OF_MOTORS];
	for(int i = 0; i < NUMBER_OF_MOTORS; i++)
	{
		reg_temp[i] = SOFT_STOP;
	}
	l6470_transmit_spi(stepper_motor, reg_temp, NUMBER_OF_MOTORS);
}

void l6470_reset_speed_pos(MotorSetTypedef* stepper_motor)
{
	for(int i = 0; i < NUMBER_OF_MOTORS; i++)
	{
		stepper_motor ->motors[i].speed_pos.rad_pos   = 0;
		stepper_motor ->motors[i].speed_pos.rad_speed = 0;

	}
}

/*
 * @brief l6470 motor driver initialization
 * @param stepper_motor: stepper motor handler
 */
void l6470_init(MotorSetTypedef* stepper_motor)
{
	uint8_t reg_temp[4];

	for(int i = 0; i < NUMBER_OF_MOTORS; i++)
	{
		stepper_motor ->motors[i].speed_pos.steps_per_rev = STEPS_PER_REVOLUTION;
	}
	// reset the driver
	HAL_GPIO_WritePin(stepper_motor -> gpio_rst_port, stepper_motor -> gpio_rst_number, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(stepper_motor ->gpio_rst_port, stepper_motor -> gpio_rst_number, GPIO_PIN_SET);
//	HAL_Delay(100);
	l6470_enable(stepper_motor);
//
	// set KVAL values
	reg_temp[0] = (uint8_t)((uint16_t)KVAL_HOLD_PERCENT * 255 / 100);
	l6470_set_param(stepper_motor, KVAL_HOLD, reg_temp, 1);
	reg_temp[0] = (uint8_t)((uint16_t)KVAL_RUN_PERCENT * 255 / 100);
	l6470_set_param(stepper_motor, KVAL_RUN, reg_temp, 1);
	reg_temp[0] = (uint8_t)((uint16_t)KVAL_ACCDEC_PERCENT * 255 / 100);
	l6470_set_param(stepper_motor, KVAL_ACC, reg_temp, 1);
	reg_temp[0] = (uint8_t)((uint16_t)KVAL_ACCDEC_PERCENT * 255 / 100);
	l6470_set_param(stepper_motor, KVAL_DEC, reg_temp, 1);

	// set overcurrent threshold
	reg_temp[0] = (uint8_t)(MAX_CURRENT / 0.375) + 1;
	l6470_set_param(stepper_motor, OCD_TH, reg_temp, 1);

	// set overcurrent threshold
	reg_temp[0] = (uint8_t)(MAX_CURRENT / 0.375) + 1;
	l6470_set_param(stepper_motor, STALL_TH, reg_temp, 1);

	// set overcurrent threshold
	reg_temp[0] = 0x2e;
	reg_temp[1] = 0x08;
	l6470_set_param(stepper_motor, CONFIG, reg_temp, 2);

	// step mode
	reg_temp[0] = 1;
	l6470_set_param(stepper_motor, STEP_MODE, reg_temp, 1);

	// ACC
	reg_temp[0] = 0x02;
	reg_temp[1] = 0xff;
	l6470_set_param(stepper_motor, DEC, reg_temp, 2);

	// DEC
	reg_temp[0] = 0x01;
	reg_temp[1] = 0xff;
	l6470_set_param(stepper_motor, ACC, reg_temp, 2);

	// min speed optmization, 20 ticks/sec
//	reg_temp[0] = 0x80|0x29;
//	l6470_set_param(stepper_motor, MIN_SPEED, reg_temp, 1);

	// initialize the spi buffers
	for(int i = 0; i < NUMBER_OF_MOTORS; i++)
	{
		stepper_motor -> spd_tx_buffer[4 * NUMBER_OF_MOTORS + i] = ABS_POS;
		stepper_motor -> spd_tx_buffer[4 * NUMBER_OF_MOTORS + i + NUMBER_OF_MOTORS] = NOP;
		stepper_motor -> spd_tx_buffer[4 * NUMBER_OF_MOTORS + i + 2 * NUMBER_OF_MOTORS] = NOP;
		stepper_motor -> spd_tx_buffer[4 * NUMBER_OF_MOTORS + i + 3 * NUMBER_OF_MOTORS] = NOP;

		//reset position
		stepper_motor->motors[i].speed_pos.rad_pos = 0.0;
	}
	stepper_motor -> spi_dma_busy = 0;
	stepper_motor -> spi_tx_count = 0;
}

/* @brief This function is to set rotational velocity at radians per angle
 * @param stepper motor Stepper motor handler
 * @param vel Velocity at radians per sec 62.8 - 2pi
 * @retval None
 */
void l6470_set_vel(MotorSetTypedef* stepper_motor, float* vel)
{
	uint32_t speed;
	float temp_vel;
	for(int i = 0; i < NUMBER_OF_MOTORS; i++)
	{
		stepper_motor -> spd_tx_buffer[i] = NOP;
		if(*(vel + i) > MAX_SPEED_RAD)
		{
			*(vel + i) = MAX_SPEED_RAD;
		}
		else if(*(vel + i) < -MAX_SPEED_RAD)
		{
			*(vel + i) = -MAX_SPEED_RAD;
		}
		stepper_motor->motors[i].speed_pos.rad_speed = vel[i];
		stepper_motor->motors[i].speed_pos.rad_pos += vel[i] / stepper_motor->sampling_rate;
		temp_vel = *(vel + i);
		if(temp_vel > 0)
		{
			stepper_motor -> spd_tx_buffer[NUMBER_OF_MOTORS + i] = (0x50);
		}
		else
		{
			temp_vel = -temp_vel;
			stepper_motor -> spd_tx_buffer[NUMBER_OF_MOTORS + i] = (0x51);
		}
		speed = (uint32_t)(temp_vel * STEPS_PER_REVOLUTION * 67.108864f / TWOPI_STEPPER);
		stepper_motor -> spd_tx_buffer[NUMBER_OF_MOTORS * 2 + i] = (uint8_t)(speed >> 16);
		stepper_motor -> spd_tx_buffer[NUMBER_OF_MOTORS * 3 + i] = (uint8_t)(speed >> 8);
		stepper_motor -> spd_tx_buffer[NUMBER_OF_MOTORS * 4 + i] = (uint8_t)(speed);
	}
	l6470_transmit_spi_dma(stepper_motor);
}

/*
 * @brief updates the stepper motor position (radians)
 * @param stepper_motor: stepper motor handler
 */
void l6470_get_speed_pos(MotorSetTypedef* stepper_motor)
{
	int32_t speed_raw;
	uint8_t reg_temp[NUMBER_OF_MOTORS], raw_value[3];
	for(int i =0; i< NUMBER_OF_MOTORS; i++)
	{
		reg_temp[i] = ABS_POS | 0x20;
	}
	l6470_transmit_spi(stepper_motor, reg_temp, NUMBER_OF_MOTORS);

	for(int i =0; i< NUMBER_OF_MOTORS; i++)
	{
		l6470_receive_spi(stepper_motor, raw_value, 3);
		speed_raw = (((int32_t)(raw_value[0] << 26)|(raw_value[1] << 18)
				|(raw_value[2] << 10)) >> 10)% STEPS_PER_REVOLUTION;
		if(speed_raw < 0)
		{
			speed_raw += STEPS_PER_REVOLUTION;
		}
		stepper_motor->motors[NUMBER_OF_MOTORS - i - 1].speed_pos.rad_pos = (TWOPI_STEPPER * (float)speed_raw) / (float)STEPS_PER_REVOLUTION;
	}
}
static void l6470_set_param(MotorSetTypedef* stepper_motor, uint8_t param, uint8_t *data, uint8_t data_length)
{
	uint8_t data_raw[NUMBER_OF_MOTORS];
	for(int i = 0; i < NUMBER_OF_MOTORS; i++)
	{
		data_raw[i] = param;
	}
	l6470_transmit_spi( stepper_motor, data_raw,NUMBER_OF_MOTORS);
	for(int i = 0; i < data_length; i++)
	{
		for(int j = 0; j < NUMBER_OF_MOTORS; j++)
		{
			data_raw[j] = *(data + i);
		}
		l6470_transmit_spi( stepper_motor, data_raw, NUMBER_OF_MOTORS);
	}
}

/*
 * @brief receiving data through spi
 * @param stepper_motor: stepper motor handler
 * @param data: data pointer
 * @param data_length: data length in bytes
 */
static void l6470_receive_spi(MotorSetTypedef* stepper_motor, uint8_t* data, uint8_t data_length)
{
	uint8_t data_raw[data_length];
	HAL_GPIO_WritePin(stepper_motor ->gpio_cs_port, stepper_motor->gpio_cs_number, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(stepper_motor ->hspi_l6470, data_raw, data, data_length, 1000);
	HAL_GPIO_WritePin(stepper_motor ->gpio_cs_port, stepper_motor->gpio_cs_number, GPIO_PIN_SET);
}
/*
 * @brief transmitting data through spi
 * @param stepper_motor: stepper motor handler
 * @param data: data pointer
 * @param data_length: data length in bytes
 */
static void l6470_transmit_spi(MotorSetTypedef* stepper_motor, uint8_t* data, uint8_t data_length)
{
	uint8_t receive_data[40];
	HAL_GPIO_WritePin(stepper_motor ->gpio_cs_port, stepper_motor->gpio_cs_number, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(stepper_motor ->hspi_l6470, data, receive_data, data_length, 1000);
	HAL_GPIO_WritePin(stepper_motor ->gpio_cs_port, stepper_motor->gpio_cs_number, GPIO_PIN_SET);
}

void l6470_transmit_spi_dma(MotorSetTypedef* stepper_motor)
{
	HAL_GPIO_WritePin(stepper_motor ->gpio_cs_port, stepper_motor->gpio_cs_number, GPIO_PIN_SET);
	if(stepper_motor -> spi_tx_count == SPI_TX_BUFFER_LENGTH / 2)
	{
		stepper_motor -> spi_tx_count = 0;
		stepper_motor -> spi_dma_busy = 0;
		HAL_GPIO_WritePin(stepper_motor ->gpio_cs_port, stepper_motor->gpio_cs_number, GPIO_PIN_RESET);
		simple_delay(10);
		HAL_GPIO_WritePin(stepper_motor ->gpio_cs_port, stepper_motor->gpio_cs_number, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(stepper_motor ->gpio_cs_port, stepper_motor->gpio_cs_number, GPIO_PIN_RESET);
		simple_delay(10);
		HAL_SPI_TransmitReceive_DMA(stepper_motor ->hspi_l6470, stepper_motor -> spd_tx_buffer +
				stepper_motor -> spi_tx_count * NUMBER_OF_MOTORS, stepper_motor -> spd_rx_buffer +
				stepper_motor -> spi_tx_count * NUMBER_OF_MOTORS, NUMBER_OF_MOTORS);
		stepper_motor -> spi_tx_count++;
		stepper_motor -> spi_dma_busy = 1;
	}

}

static void simple_delay(uint32_t delay_count)
{
	for(int i = 0; i < delay_count; i++)
	{

	}
}
