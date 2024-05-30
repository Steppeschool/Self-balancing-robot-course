#ifndef INC_MOTOR_ENCODER_H_
#define INC_MOTOR_ENCODER_H_

#include "main.h"

#define NUMBER_OF_TICKS_PER_REV       25000
#define ONE_REV_LENGTH_CM             22
#define TWOPI                         6.28318530718  // radians per rotation

typedef struct{
	float velocity; /* velocity of the motor radians/sec*/
	float position; /* position of the motor (radians) */
	int64_t last_counter_value; /* counter value for the last iteration*/
	float timer_period; // period
	TIM_HandleTypeDef *htim_encoder; /* timer instance*/
	uint8_t first_time; /*flag that shows whether it has been run for the first time*/
}encoder_inst;


void get_encoder_speed(encoder_inst *encoder);
void reset_encoder(encoder_inst *encoder);
#endif /* INC_MOTOR_ENCODER_H_ */
