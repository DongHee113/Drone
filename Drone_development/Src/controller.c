/*
 * controller.c
 *
 *  Created on: 2019. 4. 27.
 *      Author: ²¿µ¿
 */
#include "controller.h"


PID_GAIN pid_gain;

float desired_angle[3];
float desired_alt_vel;
float pid_output[4];

void init_pid_gain()
{
	pid_gain.stab_kp[ROLL] = 1;
	pid_gain.rate_kp[ROLL] = 1;
	pid_gain.rate_ki[ROLL] = 0;
	pid_gain.rate_kd[ROLL] = 0;

	pid_gain.stab_kp[PITCH] = 1;
	pid_gain.rate_kp[PITCH] = 1;
	pid_gain.rate_ki[PITCH] = 0;
	pid_gain.rate_kd[PITCH] = 0;

	pid_gain.stab_kp[YAW] = 1;
	pid_gain.rate_kp[YAW] = 1;
	pid_gain.rate_ki[YAW] = 0;
	pid_gain.rate_kd[YAW] = 0;
	
	pid_gain.alt_kp = 1;
	pid_gain.alt_ki = 0;
	pid_gain.alt_kd = 0;
}

void get_atti_cmd()
{
	desired_angle[ROLL]  = (r7008sb_ch_data[CH1] - 352) / ((float)(1344) / 60) - 30;
	desired_angle[PITCH] = (r7008sb_ch_data[CH2] - 352) / ((float)(1344) / 60) - 30;
	desired_angle[YAW]   = (r7008sb_ch_data[CH4] - 352) / ((float)(1344) / 300) - 150;
}

void get_alti_cmd()
{
	desired_alt_vel = (r7008sb_ch_data[CH3] - 352) / ((float)(1344) / 300) - 150;
}

void atti_pid_controller()
{
	float angle_error[2];
	float rate_error[3];
	float Pterm[3];
	float Dterm[3];
	float dt = 0.01;
	
	static float prev_mti3_pqr[3] = { 0 };
	static float Iterm[3] = { 0 };

	get_atti_cmd();

	//ROLL
	angle_error[ROLL] = desired_angle[ROLL] - mti3.euler[ROLL];
	rate_error[ROLL]  = (pid_gain.stab_kp[ROLL] * angle_error[ROLL]) - mti3.pqr[ROLL];

	Pterm[ROLL] =  rate_error[ROLL] * pid_gain.rate_kp[ROLL];
	Iterm[ROLL] += rate_error[ROLL] * dt;
	Dterm[ROLL] = -((mti3.pqr[ROLL] - prev_mti3_pqr[ROLL]) / dt) * pid_gain.rate_kd[ROLL];

	prev_mti3_pqr[ROLL] = mti3.pqr[ROLL];
	pid_output[ROLL] = Pterm[ROLL] + (pid_gain.rate_ki[ROLL] * Iterm[ROLL]) + Dterm[ROLL];

	//PITCH
	angle_error[PITCH] = desired_angle[PITCH] - mti3.euler[PITCH];
	rate_error[PITCH]  = (pid_gain.stab_kp[PITCH] * angle_error[PITCH]) - mti3.pqr[PITCH];

	Pterm[PITCH] =  rate_error[PITCH] * pid_gain.rate_kp[PITCH];
	Iterm[PITCH] += rate_error[PITCH] * dt;
	Dterm[PITCH] = -((mti3.pqr[PITCH] - prev_mti3_pqr[PITCH]) / dt) * pid_gain.rate_kd[PITCH];

	prev_mti3_pqr[PITCH] = mti3.pqr[PITCH];
	pid_output[PITCH] = Pterm[PITCH] + (pid_gain.rate_ki[PITCH] * Iterm[PITCH]) + Dterm[PITCH];

	//YAW
	rate_error[YAW] = desired_angle[YAW] - mti3.pqr[YAW];

	Pterm[YAW] =  rate_error[YAW] * pid_gain.rate_kp[YAW];
	Iterm[YAW] += rate_error[YAW] * dt;
	// Dterm[YAW] =  -mti3.pqr[YAW] * pid_gain.rate_kd[YAW];

	// prev_mti3_pqr[YAW] = mti3.pqr[YAW];
	pid_output[YAW] = Pterm[YAW] + (pid_gain.rate_ki[YAW] * Iterm[YAW]); // + Dterm[YAW];
}

void alti_pid_controller()
{
	float dt = 0.03;
	float error;
	float Pterm, Dterm;
	static float Iterm;
	
	get_alti_cmd();
	
	error = desired_alt_vel - kf_alt.x_v;
	
	Pterm =  error * pid_gain.alt_kp;
	Iterm += error * dt;
	Dterm = -kf_alt.x_v * pid_gain.alt_kd;
	
	pid_output[THROTTLE] = Pterm + (Iterm * pid_gain.alt_ki) + Dterm;
}

