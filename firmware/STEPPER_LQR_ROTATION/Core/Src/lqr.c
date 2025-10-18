#include "lqr.h"

//static float lqr_coeff[4] = {24.2, 2.1248, 16.0408, 8.4059};
static float lqr_coeff[4] = {80.0, 8.0, 50.0, 30.0};

#define RAD2METER  0.05
#define LQR_MAX    7
void apply_lqr(StateTypeDef state, float *out)
{
	*out = state.angle * lqr_coeff[0] + state.angle_vel * lqr_coeff[1] +
			state.position * lqr_coeff[2] * RAD2METER + state.velocity * lqr_coeff[3] * RAD2METER;
	if(*out > LQR_MAX)
	{
		*out = LQR_MAX;
	}
	else if(*out < -LQR_MAX)
	{
		*out = -LQR_MAX;
	}
}

void reset_state_lqr(StateTypeDef *state)
{
	state ->angle = 0.0;
	state ->angle_vel = 0.0;
	state ->position = 0.0;
	state ->velocity = 0.0;

}
