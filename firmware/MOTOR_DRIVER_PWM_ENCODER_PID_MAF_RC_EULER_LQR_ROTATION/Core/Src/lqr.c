#include "lqr.h"

//static float lqr_coeff[4] = {24.2, 2.1248, 16.0408, 8.4059};
static float lqr_coeff[4] = {25.0, 1.4, 30.0, 11.4};

#define RAD2METER  0.04

void apply_lqr(StateTypeDef state, float *out)
{
	*out = state.angle * lqr_coeff[0] + state.angle_vel * lqr_coeff[1] +
			state.position * lqr_coeff[2] * RAD2METER + state.velocity * lqr_coeff[3] * RAD2METER;
}
