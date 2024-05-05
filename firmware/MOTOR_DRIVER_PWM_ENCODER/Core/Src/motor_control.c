/**
  ******************************************************************************
  * @file    motor_control.c
  * @author  Yerkebuan Massalim, Steppeschool
  * @brief   Library to work with the DC Motor drivers.
  *          This file functions to work with the DC motor drivers. It is specially designed for STSPIN240,
  *          but it can be applied to other types of DC Motor drivers
  *
**/

#include "motor_control.h"

/**
  * @brief Setting the reset pin of the motor to enable it
  * @param  motor is the motor struct.
  * @retval None
  */

void enable_motor(motor_inst *motor)
{

	HAL_GPIO_WritePin(motor -> rst_pin_port, motor -> rst_pin_number,
					 	 	 	 GPIO_PIN_SET);
}

/**
  * @brief Resetting the reset pin of the motor to disable it
  * @param  motor is the motor struct.
  * @retval None
  */

void disable_motor(motor_inst *motor)
{

	HAL_GPIO_WritePin(motor -> rst_pin_port, motor -> rst_pin_number,
					 	 	 	 GPIO_PIN_RESET);
}

/**
  * @brief Set the duty cycle of the pwm in percentage and the odrection of the motor.
  * @param  motor is the motor struct.
  * @param  duty_cycle_percent in percentage ([+100; -100])
  * @retval None
  */

void set_speed_open(motor_inst *motor, float duty_cycle_percent)
{
	 if(duty_cycle_percent > 100.0)
	 {
		 duty_cycle_percent = 100.0;
	 }

	 if(duty_cycle_percent < -100.0)
	 {
		 duty_cycle_percent = -100.0;
	 }

	 if(duty_cycle_percent > 0)
	 {
		 // duty_cycle = 100 * (CCR / (arr + 1))
		 // CCR = (duty_cycle) * (arr + 1) / 100
		 __HAL_TIM_SET_COMPARE(motor ->htim_motor,
				 motor -> htim_motor_ch, duty_cycle_percent *
				 (motor ->htim_motor->Instance ->ARR + 1) / 100);
		 HAL_GPIO_WritePin(motor -> mdir_pin_port, motor -> mdir_pin_number,
							 GPIO_PIN_SET);
	 }
	 else
	 {
		 __HAL_TIM_SET_COMPARE(motor ->htim_motor,
				 motor -> htim_motor_ch, -duty_cycle_percent *
				 (motor ->htim_motor->Instance ->ARR + 1) / 100);
		 HAL_GPIO_WritePin(motor -> mdir_pin_port, motor -> mdir_pin_number,
							 GPIO_PIN_RESET);
	 }

}
/*
 * @brief set the motor speed to zero
 * @param  motor is the motor struct.
 * @retval none
 */
void set_speed_zero(motor_inst *motor)
{
	__HAL_TIM_SET_COMPARE(motor ->htim_motor,
				 motor -> htim_motor_ch, 0);
}

void motor_init(motor_inst *motor)
{

}
