/*
 * alti_kf.c
 *
 *  Created on: 2019. 5. 12.
 *      Author: ²¿µ¿
 */
#include "alti_kf.h"

KF kf_alt;

float dt = 0.03;

void alti_kf()
{
	get_sens();
	predict();
	correct();
}

void get_sens()
{
	kf_alt.acc = get_ned_z_acc();
	kf_alt.z_h = get_alti();
}

void predict()
{
	kf_alt.x_h += dt*kf_alt.x_v + (0.5f*dt*dt*kf_alt.acc);
	kf_alt.x_v += dt*kf_alt.acc;

	kf_alt.P[0][0] += (kf_alt.P[1][0] + kf_alt.P[0][1] + kf_alt.P[1][1]*dt)*dt + kf_alt.Q[0];
	kf_alt.P[0][1] += kf_alt.P[1][1]*dt;
	kf_alt.P[1][0] += kf_alt.P[1][1]*dt;
	kf_alt.P[1][1] += kf_alt.Q[1];
}

void correct()
{
	float y, Sinv;

	if (kf_alt.P[0][0] + kf_alt.R != 0)
	{
		Sinv = 1.0f / (kf_alt.P[0][0] + kf_alt.R);
	}
	else
	{
		Sinv = 1.0f / (kf_alt.P[0][0] + kf_alt.R + 1e-5);
	}

	kf_alt.K[0] = kf_alt.P[0][0] * Sinv;
	kf_alt.K[1] = kf_alt.P[1][0] * Sinv;

	y = kf_alt.z_h - kf_alt.x_h;

	kf_alt.x_h += kf_alt.K[0] * y;
	kf_alt.x_v += kf_alt.K[1] * y;

	kf_alt.P[0][0] -= kf_alt.K[0]*kf_alt.P[0][0];
	kf_alt.P[0][1] -= kf_alt.K[0]*kf_alt.P[0][1];
	kf_alt.P[1][0] -= kf_alt.K[1]*kf_alt.P[0][0];
	kf_alt.P[1][1] -= kf_alt.K[1]*kf_alt.P[0][1];
}

void init_kf()
{
	kf_alt.x_h = 0.0f;
	kf_alt.x_v = 0.0f;

	kf_alt.P[0][0] = 2.0f;
	kf_alt.P[0][1] = 0.0f;
	kf_alt.P[1][0] = 0.0f;
	kf_alt.P[1][1] = 2.0f;

	kf_alt.Q[0] = 0.3f;
	kf_alt.Q[1] = 0.3f;

	kf_alt.R = 20.0f;
}
