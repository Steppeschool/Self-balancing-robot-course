/*
 * rc_joystick.h
 *
 *  Created on: Jun 9, 2025
 *      Author: yerke
 */

#ifndef INC_RC_JOYSTICK_H_
#define INC_RC_JOYSTICK_H_
#include "main.h"

typedef struct
{
	TIM_HandleTypeDef *htim_rc_ref1;
	TIM_HandleTypeDef *htim_rc_ref2;
	float reference1;
	float reference2;

}RcJoystickHandler;

typedef enum{
	RC_CHANNEL1,
	RC_CHANNEL2
}rc_channel;

float get_reference(rc_channel channel_num);

#endif /* INC_RC_JOYSTICK_H_ */
