/*
 * controller.h
 *
 *  Created on: 2019. 4. 27.
 *      Author: ²¿µ¿
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#define THROTTLE 3

#include "alti_kf.h"
#include "mti3.h"
#include "r7008sb.h"

typedef struct
{
	float stab_kp[3];
	float rate_kp[3];
	float rate_ki[3];
	float rate_kd[3];
	
	float alt_kp;
	float alt_ki;
	float alt_kd;
}PID_GAIN;

void init_pid_gain();
void get_atti_cmd();
void get_alti_cmd();
void atti_pid_controller();
void alti_pid_controller();

extern PID_GAIN pid_gain;
extern float pid_output[4];

#endif /* CONTROLLER_H_ */
