#ifndef __MTI3_H
#define __MTI3_H

#define MTI3_DMA_LEN 1024
#define MTI3_PACKET_LEN 35

#define PREAMBLE 0xFA
#define BID 0xFF
#define MID 0x36

#define PI 3.141592653589

#define ROLL 0
#define PITCH 1
#define YAW 2

#define PQR_X 0
#define PQR_Y 1
#define PQR_Z 2

#include "stm32f4xx_hal.h"
#include "usart.h"

#include <string.h>


typedef union
{
  float data;
  uint8_t byte[4];
}MTI3_UNION;


typedef struct
{
  float euler[3];
  float pqr[3];
}MTI3;

void read_mti3();
void get_mti3_packet();
void decode_mti3();
int checksum();

extern MTI3 mti3;
extern uint8_t mti3_dma_buffer[MTI3_DMA_LEN];
extern uint8_t mti3_decode_flag;


#endif
