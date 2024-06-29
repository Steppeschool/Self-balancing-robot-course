

#include "motor_encoder.h"
#include "stdio.h"

/* @brief compute velocity of the motor using the timer encoder
 * @param encoder: Encoder instance
 * @retval: none:
 */
void get_encoder_speed(encoder_inst *encoder)
{
	int64_t temp_counter = __HAL_TIM_GET_COUNTER(encoder -> htim_encoder);
	float scaling = TWOPI / (NUMBER_OF_TICKS_PER_REV * (encoder -> timer_period));
	// if running this code for the first time after reset, set velocity to zero
	if(encoder -> first_time)
	{
		encoder ->velocity = 0;
		encoder -> first_time = 0;
	}
	else
	{	// if the counter value is equal to the old value, the velocity is zero

		if(temp_counter == encoder ->last_counter_value)
		{
			encoder ->velocity = 0;
		}
		// if it is higher:
		else if(temp_counter > encoder ->last_counter_value)
		{
			// overflow occurred:
			if (temp_counter - encoder ->last_counter_value >  __HAL_TIM_GET_AUTORELOAD(encoder -> htim_encoder) / 2)
			{
				encoder ->velocity = scaling * ( -(encoder ->last_counter_value) -
						(__HAL_TIM_GET_AUTORELOAD(encoder -> htim_encoder)
								- temp_counter));
			}
			// no overflow:
			else
			{
				encoder ->velocity = scaling * (temp_counter -
						encoder ->last_counter_value);
			}

		}
		else
		{
			// no overflow
			if ((encoder ->last_counter_value - temp_counter) <  __HAL_TIM_GET_AUTORELOAD(encoder -> htim_encoder) / 2)
			{
				encoder ->velocity = scaling * (temp_counter
						- encoder ->last_counter_value);
			}
			// overflow occurred
			else
			{
				encoder ->velocity = scaling * (temp_counter +
						(__HAL_TIM_GET_AUTORELOAD(encoder -> htim_encoder) -
								encoder ->last_counter_value));
			}
		}
	}
	apply_average_filter(&encoder ->motor_vel_filter, encoder ->velocity, &encoder ->velocity);
	encoder ->position += encoder ->velocity * encoder -> timer_period;
	encoder ->last_counter_value = temp_counter;
}

/* @brief resetting the encoder state
 * @param encoder: encoder instance
 * @retval: none
 */
void reset_encoder(encoder_inst *encoder)
{
	encoder -> position = 0;
	encoder -> first_time = 1;
	encoder -> last_counter_value = 0;
	encoder -> velocity = 0;
	reset_average_filter(&encoder -> motor_vel_filter, MOVING_AVERAGE_ENCODER);
}


