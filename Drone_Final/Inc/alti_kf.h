/*
 * alti_kf.h
 *
 *  Created on: 2019. 5. 12.
 *      Author: ²¿µ¿
 */

#ifndef ALTI_KF_H_
#define ALTI_KF_H_

#include "coordinate_transformation.h"
#include "ms5611.h"

typedef struct
{
  float x_h;
  float x_v;
  float P[2][2];

  float z_h;
  float acc;

  float Q[2];
  float R;

  float K[2];
}KF;

extern KF kf_alt;

void alti_kf();
void predict();
void correct();
void init_kf();
void get_sens();

#endif /* ALTI_KF_H_ */
