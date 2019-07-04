/*
 * mixer.c
 *
 *  Created on: 2019. 4. 27.
 *      Author: ²¿µ¿
 */
#include "mixer.h"

void mixer()
{
	uint16_t motor_output[4];
	int i;
	
	if(pid_output[THROTTLE] <= 355)
	{
		for(i = 0; i < 4; i++)
		{
			motor_output[i] = 352;
		}
	}
	else
	{
		motor_output[M1] = (int16_t) (pid_output[ROLL] + pid_output[PITCH] - pid_output[YAW] + pid_output[THROTTLE]);
		motor_output[M2] = (int16_t)(-pid_output[ROLL] + pid_output[PITCH] + pid_output[YAW] + pid_output[THROTTLE]);
		motor_output[M3] = (int16_t)(-pid_output[ROLL] - pid_output[PITCH] - pid_output[YAW] + pid_output[THROTTLE]);
		motor_output[M4] = (int16_t) (pid_output[ROLL] - pid_output[PITCH] + pid_output[YAW] + pid_output[THROTTLE]);
	}

	for(i = 0; i < 4; i++)
	{
		if(motor_output[i] <= 352)
		{
			motor_output[i] = 352;
		}
		else if(motor_output[i] >= 1696)
		{
			motor_output[i] = 1696;
		}
	}
	
	printf("%d %d %d %d\r\n", motor_output[0], motor_output[1], motor_output[2], motor_output[3]);
		
	htim1.Instance -> CCR1 = (int16_t)(motor_output[M1] * GAIN) + OFFSET;
	htim1.Instance -> CCR2 = (int16_t)(motor_output[M2] * GAIN) + OFFSET;
	htim1.Instance -> CCR3 = (int16_t)(motor_output[M3] * GAIN) + OFFSET;
	htim1.Instance -> CCR4 = (int16_t)(motor_output[M4] * GAIN) + OFFSET;
}
