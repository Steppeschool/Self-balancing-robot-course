#ifndef INC_MOVING_AVERAGE_H_
#define INC_MOVING_AVERAGE_H_

#include "stdint.h"
#include "main.h"

#define MOVING_AVERAGE_LENGTH  200

typedef float filter_type;
typedef struct{
	filter_type buffer[MOVING_AVERAGE_LENGTH];
	uint16_t counter;
	filter_type out;
	filter_type sum;
	uint16_t length;
}mov_aver_instance;

void reset_average_filter(mov_aver_instance* instance, uint16_t length);
void apply_average_filter(mov_aver_instance* instance,filter_type  input, filter_type *out);

#endif /* INC_MOVING_AVERAGE_H_ */
