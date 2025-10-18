/*
 * lqr.h
 *
 *  Created on: Oct 1, 2024
 *      Author: massa
 */

#ifndef INC_LQR_H_
#define INC_LQR_H_

typedef struct
{
	float angle;
	float angle_vel;
	float position;
	float velocity;
} StateTypeDef;

void apply_lqr(StateTypeDef state, float *out);

#endif /* INC_LQR_H_ */
