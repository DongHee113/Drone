#ifndef __RF_H
#define __RF_H

#define RF_DMA_LEN 1024
#define RF_PACKET_LEN 10

#define HEADER 0x02
#define TAIL   0x03

#include "controller.h"
#include "mti3.h"
#include "usart.h"

void read_rf();
void decode_rf();
void get_rf_packet();
void update_pid_gain();

extern uint8_t rf_dma_buffer[RF_DMA_LEN];
extern uint8_t rf_decode_flag;

#endif