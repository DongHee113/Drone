/*
 * ms5611.h
 *
 *  Created on: 2019. 5. 11.
 *      Author: ²¿µ¿
 */

#ifndef MS5611_H_
#define MS5611_H_

#define MS5611_W        0xEC
#define MS5611_R        0xED

#define RESET_CMD       0x12
#define PROM_READ_CMD   0xA0
#define ADC_START_CMD   0x00
#define PRESS_READ_CMD  0x48
#define TEMP_READ_CMD   0x58

#define PRESSURE    0
#define TEMPERATURE 1

#include "i2c.h"
#include <math.h>

void calc_start_alt();
void read_ms5611();
void reset_ms5611();
void read_prom();
void start_adc(uint8_t type);
void read_adc(uint8_t type);
void calc_alt();
void low_pass_filter(float x);
float get_alti();

extern uint8_t ms5611_decode_flag;
extern float prev_x;

#endif /* MS5611_H_ */
