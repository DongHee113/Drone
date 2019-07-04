/*
 * coordinate_transformation.c
 *
 *  Created on: 2019. 4. 30.
 *      Author: ²¿µ¿
 */

#include "coordinate_transformation.h"
#include "mti3.h"

float dcm[3][3];
float ned_acc[3];

void cal_dcm(float roll, float pitch, float yaw)
{
	roll  *= PI / 180.0f;
	pitch *= PI / 180.0f;
	yaw   *= PI / 180.0f;

	dcm[0][0] = (cos(pitch)*cos(yaw));
	dcm[0][1] = (cos(yaw)*sin(roll)*sin(pitch)) - (cos(roll)*sin(yaw));
	dcm[0][2] = (cos(roll)*sin(pitch)*cos(yaw)) + (sin(roll)*sin(yaw));

	dcm[1][0] = (cos(pitch)*sin(yaw));
	dcm[1][1] = (sin(roll)*sin(pitch)*sin(yaw)) + (cos(roll)*cos(yaw));
	dcm[1][2] = (cos(roll)*sin(pitch)*sin(yaw)) - (sin(roll)*cos(yaw));

	dcm[2][0] = -sin(pitch);
	dcm[2][1] = (sin(roll)*cos(pitch));
	dcm[2][2] = (cos(roll)*cos(pitch));
}

void transform_body_to_ned(float x, float y, float z)
{
	ned_acc[0] = (dcm[0][0]*x + dcm[0][1]*y + dcm[0][2]*z);
	ned_acc[1] = (dcm[1][0]*x + dcm[1][1]*y + dcm[1][2]*z);
	ned_acc[2] = (dcm[2][0]*x + dcm[2][1]*y + dcm[2][2]*z);
}

float get_ned_z_acc()
{
	return -ned_acc[2] - ACC_G;
}
