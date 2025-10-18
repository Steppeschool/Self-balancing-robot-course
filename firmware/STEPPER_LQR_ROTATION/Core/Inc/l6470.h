/*
 * l6470.h
 *
 *  Created on: Dec 1, 2024
 *      Author: yerke
 */

#ifndef INC_L6470_H_
#define INC_L6470_H_

#include "main.h"

#define MAX_ACCELERATION    		10  				/*!< max acceleration [rad/sec^2] 			*/
#define MAX_SPEED_RAD    			25.12f 			/*!< max speed [rad / sec] 					*/
#define MAX_CURRENT         		5.0 				/*!< Max Current of the stepper motor 		*/
#define MICROSTEPPING               4					/*!< Number of microsteps per second	  	*/
#define STEPS_PER_REVOLUTION  		200	                /*!< Steps per revolution 	*/
#define TWOPI_STEPPER               6.283185f			/*!< Two pi value 			*/
#define KVAL_HOLD_PERCENT           20
#define KVAL_RUN_PERCENT            40
#define KVAL_ACCDEC_PERCENT         60

#define RESET_DEVICE                0xc0
#define SOFT_STOP					0xd0


#define ABS_POS			0x01 /*!< Current position    */
#define EL_POS			0x02 /*!< Electrical position */
#define MARK			0x03 /*!< Mark position */
#define SPEED           0x04 /*!< Current speed  */
#define ACC             0x05 /*!< Acceleration  */
#define DEC             0x06 /*!< Deceleration  */
#define MAX_SPEED       0x07 /*!< Deceleration  */
#define MIN_SPEED       0x08 /*!< Deceleration  */
#define KVAL_HOLD       0x09 /*!< Holding Kval  */
#define KVAL_RUN        0x0a /*!< Constant speed Kval  */
#define KVAL_ACC        0x0b /*!< Acceleration Starting Kval  */
#define KVAL_DEC        0x0c /*!< Deceleration Starting Kval  */
#define INT_SPEED       0x0d /*!< Intersect Speed  */
#define ST_SLP          0x0e /*!< Start slope  */
#define FN_SLP_ACC      0x0f /*!< Acceleration final slope  */
#define FN_SLP_DEC      0x10 /*!< Deceleration final slope  */
#define K_THERM		    0x11 /*!< Thermal Compensation Factor  */
#define ADC_OUT		    0x12 /*!< ADC output  */
#define OCD_TH			0x13 /*!< OCD THRESHOLD */
#define STALL_TH		0x14 /*!< STALL THRESHOLD */
#define STEP_MODE		0x16 /*!< Step Mode */
#define ALARM_EN        0x17 /*!< Alarms enables */
#define CONFIG          0x18 /*!< CONFIG Register */
#define STATUS          0x19 /*!< STatus Register */

#define NOP             0x00
#define TIMER_FREQUENCY 1000000
#define SPI_TX_BUFFER_LENGTH   10
#define NUMBER_OF_MOTORS       2
typedef struct
{
	float rad_pos;
	float rad_speed;
	float rad_speed_target;
	float max_accel;
	float max_vel;
	uint16_t steps_per_rev;
	uint16_t steps_per_rad;
}SpeedPosTypeDef;

typedef struct
{
	uint16_t stepper_id;
	SpeedPosTypeDef     speed_pos;
}l6470TypeDef;

typedef struct
{
	GPIO_TypeDef* 		gpio_rst_port;
	uint16_t 			gpio_rst_number;
	GPIO_TypeDef*		gpio_cs_port;
	uint16_t 			gpio_cs_number;
	SPI_HandleTypeDef* 	hspi_l6470;
	l6470TypeDef        motors[NUMBER_OF_MOTORS];
	uint8_t             spi_dma_busy;
	uint8_t             spd_tx_buffer[NUMBER_OF_MOTORS * SPI_TX_BUFFER_LENGTH];
	uint8_t             spd_rx_buffer[NUMBER_OF_MOTORS * SPI_TX_BUFFER_LENGTH];
	uint8_t             spi_tx_count;
	float               sampling_rate;

}MotorSetTypedef;

void l6470_enable(MotorSetTypedef* stepper_motor);
void l6470_disable(MotorSetTypedef* stepper_motor);
void l6470_init(MotorSetTypedef* stepper_motor);
void l6470_set_vel(MotorSetTypedef* stepper_motor, float* vel);
void l6470_set_steppersec(MotorSetTypedef stepper_motor,uint8_t motor_id, uint16_t step);
void l6470_get_speed_pos(MotorSetTypedef* stepper_motor);
void l6470_transmit_spi_dma(MotorSetTypedef* stepper_motor);
void l6470_reset_speed_pos(MotorSetTypedef* stepper_motor);
#endif /* INC_L6470_H_ */
