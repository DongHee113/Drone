/*
 * coordinate_transformation.h
 *
 *  Created on: 2019. 4. 30.
 *      Author: ²¿µ¿
 */

#ifndef COORDINATE_TRANSFORMATION_H_
#define COORDINATE_TRANSFORMATION_H_

#define ACC_G 9.80665

#include <math.h>

void cal_dcm(float x, float y, float z);
void transform_body_to_ned(float x, float y, float z);
float get_ned_z_acc();

#endif /* COORDINATE_TRANSFORMATION_H_ */
