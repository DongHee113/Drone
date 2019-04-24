/*
 * r7008sb.c
 *
 *  Created on: 2019. 4. 15.
 *      Author: ²¿µ¿
 */
#include "r7008sb.h"

uint8_t r7008sb_packet[R7008SB_PACKET_LEN];
uint8_t r7008sb_dma_buffer[R7008SB_DMA_LEN];
uint8_t r7008sb_decode_flag = 0;
uint8_t r7008sb_flag = 0;

int r7008sb_cnt;

int r7008sb_ch_data[CH_SIZE];

void read_r7008sb()
{
	get_r7008sb_packet();
	if(r7008sb_flag)
	{
		r7008sb_decode();
		r7008sb_flag = 0;
	}
}

void get_r7008sb_packet()
{
	static int flag = 0;
	static int last_ndt = 0;
	static int now_ndt = 0;
	static int start = 0;
	static int end = R7008SB_PACKET_LEN - 1;
	int i = 0;

	last_ndt = now_ndt;
	now_ndt = R7008SB_DMA_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

	if(now_ndt > last_ndt)
	{
		if(end > last_ndt && end <= now_ndt)
		{
			flag = 1;
		}
	}
	else if(now_ndt < last_ndt)
	{
		if(end > last_ndt || end <= now_ndt)
		{
			flag = 1;
		}
	}
	else
		return;

	if(flag)
	{
		if(r7008sb_dma_buffer[start] == START_BYTE && (r7008sb_dma_buffer[end] & 0x04) == END_BYTE)
		{
			if(start < end)
			{
			  memcpy(r7008sb_packet, &r7008sb_dma_buffer[start], R7008SB_PACKET_LEN);
			}
			else if(end < start)
			{
			  memcpy(r7008sb_packet, &r7008sb_dma_buffer[start], R7008SB_DMA_LEN - start);
			  memcpy(&r7008sb_packet[R7008SB_DMA_LEN - start], r7008sb_dma_buffer, end + 1);
			}
			start = (end+1) % R7008SB_DMA_LEN;
			end = (start + (R7008SB_PACKET_LEN-1)) % R7008SB_DMA_LEN;
			r7008sb_flag = 1;
		}
		else
		{
			for(i = 1; i < R7008SB_PACKET_LEN; i++)
			{
				if(r7008sb_dma_buffer[(start + i) % R7008SB_DMA_LEN] == START_BYTE)
				{
					start = (start+i) % R7008SB_DMA_LEN;
					end = (start + (R7008SB_PACKET_LEN-1)) % R7008SB_DMA_LEN;
					break;
				}
			}
		}

		flag = 0;
	}
}

void r7008sb_decode()
{
	int i;

	r7008sb_ch_data[CH1]  = (uint32_t)(r7008sb_packet[1] + ((r7008sb_packet[2] & 0x07) << 8));  //yaw
	r7008sb_ch_data[CH2]  = (uint32_t)(((r7008sb_packet[2]  & 0xf8) >> 3) + ((r7008sb_packet[3] & 0x3f) << 5)); //pitch-¾ÕµÚ
	r7008sb_ch_data[CH3]  = (uint32_t)(((r7008sb_packet[3]  & 0xc0) >> 6) + (r7008sb_packet[4] << 2) + ((r7008sb_packet[5] & 0x01) << 10));  //throttle
	r7008sb_ch_data[CH4]  = (uint32_t)(((r7008sb_packet[5]  & 0xfe) >> 1) + ((r7008sb_packet[6] & 0x0f) << 7));  //roll-ÁÂ¿ì
	r7008sb_ch_data[CH5]  = (uint32_t)(((r7008sb_packet[6]  & 0xf0) >> 4) + ((r7008sb_packet[7] & 0x7f) << 4));
	r7008sb_ch_data[CH6]  = (uint32_t)(((r7008sb_packet[7]  & 0x80) >> 7) + (r7008sb_packet[8] << 1) + ((r7008sb_packet[9] & 0x03) << 9));
	r7008sb_ch_data[CH7]  = (uint32_t)(((r7008sb_packet[9]  & 0xfc) >> 2) + ((r7008sb_packet[10] & 0x1f) << 6));
	r7008sb_ch_data[CH8]  = (uint32_t)(((r7008sb_packet[10] & 0xe0) >> 5) + (r7008sb_packet[11] << 3));
	r7008sb_ch_data[CH9]  = (uint32_t)(r7008sb_packet[12] + ((r7008sb_packet[13] & 0x07) << 8));
	r7008sb_ch_data[CH10] = (uint32_t)(((r7008sb_packet[13] & 0xf8) >> 3) + ((r7008sb_packet[14] & 0x3f) << 5));
	r7008sb_ch_data[CH11] = (uint32_t)(((r7008sb_packet[14] & 0xc0) >> 6) + (r7008sb_packet[15] << 2) + ((r7008sb_packet[16] & 0x01) << 10));
	r7008sb_ch_data[CH12] = (uint32_t)(((r7008sb_packet[16] & 0xfe) >> 1) + ((r7008sb_packet[17] & 0x0f) <<7));

	for(i = 0; i < 12; i++)
	{
		printf("%d\n\r", r7008sb_ch_data[i]);
	}
	printf("\n\r\n\r");
}
