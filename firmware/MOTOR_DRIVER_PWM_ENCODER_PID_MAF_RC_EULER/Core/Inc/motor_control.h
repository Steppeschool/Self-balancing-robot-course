#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_

#include "main.h"

typedef struct{
	TIM_HandleTypeDef    *htim_motor; /* timer instance for the pwm signal*/
	uint32_t           htim_motor_ch; /* timer channel*/
	GPIO_TypeDef      *mdir_pin_port; /* pin to control the rotation direction*/
	uint16_t         mdir_pin_number; /* pin to control the rotation direction*/
	GPIO_TypeDef      *rst_pin_port;  /* reset pin*/
	uint16_t         rst_pin_number;  /* reset pin*/
}motor_inst;

void enable_motor(motor_inst *motor);
void disable_motor(motor_inst *motor);
void motor_init(motor_inst *motor);
void set_speed_open(motor_inst *motor, float duty_cycle_percent);
void set_speed_zero(motor_inst *motor);
#endif /* INC_MOTOR_CONTROL_H_ */
