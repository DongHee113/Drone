/*
 * ms5611.c
 *
 *  Created on: 2019. 5. 11.
 *      Author: ²¿µ¿
 */
#include "ms5611.h"
#include <stdio.h>

uint8_t send_cmd;
uint8_t rx_buffer[3];
uint16_t C[7];
uint32_t d[2];

uint8_t ms5611_decode_flag = 0;

float lpf_alt, start_alt, alt;

void calc_start_alt()
{
	int i;
	float temp = 0;

	for(i = 0; i < 100; i++)
	{
		start_adc(PRESSURE);
		HAL_Delay(10);
		read_adc(PRESSURE);

		start_adc(TEMPERATURE);
		HAL_Delay(10);
		read_adc(TEMPERATURE);

		calc_alt();

		temp += alt;

	}

	start_alt = temp/100;
}

void reset_ms5611()
{
	send_cmd = RESET_CMD;
	HAL_I2C_Master_Transmit(&hi2c1, MS5611_W, &(send_cmd), 1, 2);
	HAL_Delay(3);
}

void read_prom()
{
	int i;

	for(i = 1; i <= 6; i++)
	{
	  send_cmd = PROM_READ_CMD | (i << 1);
	  HAL_I2C_Master_Transmit(&hi2c1, MS5611_W, &(send_cmd), 1, 100);
	  HAL_I2C_Master_Receive(&hi2c1, MS5611_R, rx_buffer, 2, 100);

	  C[i] = ((rx_buffer[0]<<8) | rx_buffer[1]);
	}
}

void read_ms5611()
{
	static int adc_wait_ms = 0;
	static int type = 0;

	if(adc_wait_ms == 0)
	{
		start_adc(type);
		adc_wait_ms++;
	}
	else if(adc_wait_ms < 10)
	{
		adc_wait_ms++;
	}
	else if(adc_wait_ms == 10)
	{
		adc_wait_ms = 0;

		read_adc(type);
		if(type == TEMPERATURE)
		{
			calc_alt();
			ms5611_decode_flag = 1;
		}
		type = !type;
	}
}

void start_adc(uint8_t type)
{
	if(type == PRESSURE)
	{
		send_cmd = PRESS_READ_CMD;
		HAL_I2C_Master_Transmit(&hi2c1, MS5611_W, &(send_cmd), 1, 100);
	}
	else if(type == TEMPERATURE)
	{
		send_cmd = TEMP_READ_CMD;
		HAL_I2C_Master_Transmit(&hi2c1, MS5611_W, &(send_cmd), 1, 100);
	}
}

void read_adc(uint8_t type)
{
	send_cmd = ADC_START_CMD;

	HAL_I2C_Master_Transmit(&hi2c1, MS5611_W, &(send_cmd), 1, 100);
	HAL_I2C_Master_Receive(&hi2c1, MS5611_R, rx_buffer, 3, 100);

	d[type] = ((rx_buffer[0] << 16) | (rx_buffer[1] << 8) | rx_buffer[2]);
}

void calc_alt()
{
	int32_t dt;
	int32_t temp, temp2;

	int64_t off, off2;
	int64_t sens, sens2;

	float P;

	dt = d[1] - (uint32_t)C[5] * 256;
	temp = 2000 + ((int64_t)dt * C[6]) / 8388608;

	off  = ((int64_t)C[2] * 65536) + (((int64_t)C[4] * dt) / 128);
	sens = ((int64_t)C[1] * 32768) + (((int64_t)C[3] * dt) / 256);

	if(temp < 2000)
	{
		temp2 = (dt*dt) / 2147483648;
		off2  = 5 * (temp-2000)*(temp-2000) / 2;
		sens2 = 5 * (temp-2000)*(temp-2000) / 4;
		if(temp < -1500)
		{
			off2  = off2 + (7 * (temp+1500)*(temp+1500));
			sens2 = sens2 + (11 * (temp+1500)*(temp+1500) / 2);
		}

		temp = temp - temp2;
		off  = off  - off2;
		sens = sens - sens2;
	}

	P = ((d[0] * sens / 2097152 - off) / 32768) * 0.01f;

	alt = 44330.0f * (1.0f - pow((P/1011.6), 0.190295f));

	low_pass_filter(alt);
        
        //printf("%f\r\n", lpf_alt);
}

void low_pass_filter(float x)
{
	float x_hat;
	float alpha = 0.7;

	static float prev_x = 0;

	x_hat = (alpha*prev_x) + ((1.0f - alpha)*x);
	prev_x = x_hat;

	lpf_alt = x_hat;
}

float get_alti()
{
	return lpf_alt - start_alt;
}
