#include "pid_control.h"

/*	@brief setting pid gains
 * 	@param pid: pid instance
 * 	@param p: proportional gain
 * 	@param i: inegral gain
 * 	@param d: derivative gain
 * 	@retval: none
 * */

void set_pid(pid_instance *pid, float p, float i, float d)
{
	pid ->error_integral = 0;
	pid ->p_gain = p;
	pid ->i_gain = i;
	pid ->d_gain = d;
}

/*	@brief resetting the pid
 * 	@param pid: pid instance
 * 	@retval: none
 * */
void reset_pid(pid_instance *pid)
{
	pid -> error_integral = 0;
	pid -> last_error = 0;

}

/*	@brief apply pid
 * 	This function computes the PID output considering the PID gains and limits
 * 	@param pid: pid instance
 * 	@param input_error: input error
 * 	@retval: none
 * */
pid_typedef apply_pid(pid_instance *pid, float input_error)
{
	pid ->error_integral += input_error;
	if(pid->error_integral > pid ->integral_max)
	{
		pid->error_integral = pid ->integral_max;
	}
	if(pid->error_integral < -pid ->integral_max)
	{
		pid->error_integral = -pid ->integral_max;
	}
	if ( pid ->sam_rate == 0)
	{
		return pid_numerical;
	}
	pid ->output = pid ->p_gain * input_error +
			pid ->i_gain * (pid->error_integral) / pid ->sam_rate +
			pid ->d_gain * pid ->sam_rate * (input_error - pid->last_error);

	if(pid->output >= pid ->pid_max)
	{
		pid->output = pid ->pid_max;
	}
	if(pid->output <= -pid ->pid_max)
	{
		pid->output = -pid ->pid_max;
	}
	pid->last_error = input_error;

	return pid_ok;
}
