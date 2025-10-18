#include <moving_average.h>


void reset_average_filter(mov_aver_instance* instance, uint16_t length)
{
	if(length < MOVING_AVERAGE_LENGTH)
	{
		instance->length = length;
	}
	else
	{
		instance->length = MOVING_AVERAGE_LENGTH;
	}
	instance ->counter = 0;
	instance ->sum = 0;
	instance ->out = 0;
	for (int i = 0; i< MOVING_AVERAGE_LENGTH; i++)
	{
		instance->buffer[i] = 0;
	}
}
void apply_average_filter(mov_aver_instance* instance, filter_type input, filter_type *out)
{

	instance ->sum +=input - instance->buffer[instance ->counter];
	instance->buffer[instance ->counter] = input;
	instance ->counter++;
	if(instance->length == 0)
	{
		instance->length = MOVING_AVERAGE_LENGTH;
	}
	if(instance ->counter == instance->length)
	{
		instance ->counter = 0;
	}
	instance ->out = instance ->sum / instance->length;
	// normalization
	*out = instance ->out;
}
