/*
 * mixer.h
 *
 *  Created on: 2019. 4. 27.
 *      Author: ²¿µ¿
 */

#ifndef MIXER_H_
#define MIXER_H_

#define MIN_PULSE 3079
#define MAX_PULSE 5431

#define MIN_VALUE 352
#define MAX_VALUE 1696

#define OFFSET 2463
#define GAIN (MAX_PULSE - MIN_PULSE) / (MAX_VALUE - MIN_VALUE)

#define M1 0
#define M2 1
#define M3 2
#define M4 3

#include "controller.h"
#include "tim.h"

void mixer();

#endif /* MIXER_H_ */
