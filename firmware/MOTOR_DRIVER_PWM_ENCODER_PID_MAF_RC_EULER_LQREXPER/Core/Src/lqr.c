#include "lqr.h"

// static float lqr_coeff[4] = {15.96, 1.364, 8.07, 4.79};
static float lqr_coeff[4] = {35.0, 0.4, 40.1, 15.8};

#define RAD2METER  0.04

void apply_lqr(StateTypeDef state, float *out)
{
	*out = state.angle * lqr_coeff[0] + state.angle_vel * lqr_coeff[1] +
			state.position * lqr_coeff[2] * RAD2METER + state.velocity * lqr_coeff[3] * RAD2METER;
}
