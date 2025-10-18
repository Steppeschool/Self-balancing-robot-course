#ifndef INC_MOTOR_ENCODER_H_
#define INC_MOTOR_ENCODER_H_

#include "main.h"
#include "moving_average.h"

#define NUMBER_OF_TICKS_PER_REV       300
#define ONE_REV_LENGTH_CM             22
#define TWOPI                         6.28318530718  // radians per rotation
#define MOVING_AVERAGE_ENCODER  70
typedef struct{
	float velocity; /* velocity of the motor radians/sec*/
	float position; /* position of the motor (radians) */
	int64_t last_counter_value; /* counter value for the last iteration*/
	float timer_period; // period
	TIM_HandleTypeDef *htim_encoder; /* timer instance*/
	uint8_t first_time; /*flag that shows whether it has been run for the first time*/
	mov_aver_instance  motor_vel_filter; /* filter to smooth the velocity values*/
}encoder_inst;


void get_encoder_speed(encoder_inst *encoder);
void reset_encoder(encoder_inst *encoder);
#endif /* INC_MOTOR_ENCODER_H_ */
