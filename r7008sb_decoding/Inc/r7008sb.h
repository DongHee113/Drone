/*
 * r7008sb.h
 *
 *  Created on: 2019. 4. 15.
 *      Author: ²¿µ¿
 */

#ifndef R7008SB_H_
#define R7008SB_H_

#define R7008SB_DMA_LEN 1024
#define R7008SB_PACKET_LEN 25

#define START_BYTE 0x0F
#define END_BYTE 0x04

#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5
#define CH7 6
#define CH8 7
#define CH9 8
#define CH10 9
#define CH11 10
#define CH12 11

#define CH_SIZE 12

#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"

void read_r7008sb();
void get_r7008sb_packet();
void r7008sb_decode();

extern uint8_t r7008sb_dma_buffer[R7008SB_DMA_LEN];
extern uint8_t r7008sb_decode_flag;

extern int r7008sb_cnt;

extern int r7008sb_ch_data[CH_SIZE];

#endif /* R7008SB_H_ */
