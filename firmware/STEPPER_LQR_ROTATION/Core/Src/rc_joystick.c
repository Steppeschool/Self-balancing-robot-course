#include "rc_joystick.h"

#define RC_OFFSET1 11000
#define RC_OFFSET2 11900

#define RC_DIV1 200
#define RC_DIV2 500

extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim15;

static RcJoystickHandler rc_balance_robot =
{
	.htim_rc_ref1 = &htim8,
	.htim_rc_ref2 = &htim15,
	.reference1 = 0,
	.reference2 = 0
};

static int32_t duty_cyclech1, duty_cyclech2;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	//int32_t frequency;
	static uint8_t first_time1 = 1, first_time2 = 1;
	if (htim == rc_balance_robot.htim_rc_ref1)
	{
		if(first_time1 == 0)
		{
			if(htim ->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
			{
				duty_cyclech1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) - RC_OFFSET1;
				if(duty_cyclech1 < 150 && duty_cyclech1 > -150 )
				{
					duty_cyclech1 = 0;
				}
				rc_balance_robot.reference1 = (float)duty_cyclech1 / RC_DIV1;
//				if(rc_balance_robot.reference1 > 0)
//				{
//					rc_balance_robot.reference1 += 0.05;
//				}
//				else if(rc_balance_robot.reference1 < 0)
//				{
//					rc_balance_robot.reference1 -= 0.05;
//				}

			}
		}
		else
		{
			first_time1 = 0;
		}
	}
	if (htim == &htim15)
	{
		if(first_time2 == 0)
		{
			if(htim ->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
			{
				duty_cyclech2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) - RC_OFFSET2;
				if(duty_cyclech2 < 150 && duty_cyclech2 > -150 )
				{
					duty_cyclech2 = 0;
				}
				rc_balance_robot.reference2 = (float)duty_cyclech2 / RC_DIV2;
			}
		}
		else
		{
			first_time2 = 0;
		}
	}
}

float get_reference(rc_channel channel_num)
{
	if(channel_num == RC_CHANNEL1)
		return rc_balance_robot.reference1;
	else if(channel_num == RC_CHANNEL2)
		return rc_balance_robot.reference2;
	else
		return 0;
}
