#include "mti3.h"

uint8_t mti3_flag = 0;
uint8_t mti3_decode_flag = 0;
uint8_t mti3_packet[MTI3_PACKET_LEN];
uint8_t mti3_dma_buffer[MTI3_DMA_LEN];

MTI3 mti3;
MTI3_UNION mti3_union;

void read_mti3()
{
	get_mti3_packet();
	if(mti3_flag)
	{
		decode_mti3();
		mti3_flag = 0;
	}
}

void get_mti3_packet()
{
	static int flag = 0;
	static int last_ndt = 0;
	static int now_ndt = 0;
	static int start = 0;
	static int end = MTI3_PACKET_LEN - 1;
	int i;

	last_ndt = now_ndt;
	now_ndt = MTI3_DMA_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);


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
		if(mti3_dma_buffer[start] == PREAMBLE && mti3_dma_buffer[(start+1) % MTI3_DMA_LEN] == BID && mti3_dma_buffer[(start+2) % MTI3_DMA_LEN] == MID)
		{
			if(start < end)
			{
			  memcpy(mti3_packet, &mti3_dma_buffer[start], MTI3_PACKET_LEN);
			}
			else if(end < start)
			{
			  memcpy(mti3_packet, &mti3_dma_buffer[start], MTI3_DMA_LEN - start);
			  memcpy(&mti3_packet[MTI3_DMA_LEN - start], mti3_dma_buffer, end + 1);
			}
			start = (end+1) % MTI3_DMA_LEN;
			end = (start + (MTI3_PACKET_LEN - 1)) % MTI3_DMA_LEN;
			mti3_flag = 1;
		}

		else
		{
			for(i = 1; i < MTI3_PACKET_LEN - 2; i++)
			{
				if(mti3_dma_buffer[(start+i) % MTI3_DMA_LEN] == PREAMBLE && mti3_dma_buffer[(start+i+1) % MTI3_DMA_LEN] == BID && mti3_dma_buffer[(start+i+2) % MTI3_DMA_LEN] == MID)
				{
					start = (start+i) % MTI3_DMA_LEN;
					end = (start + (MTI3_PACKET_LEN-1)) % MTI3_DMA_LEN;
					break;
				}
			}
			if(i >= (MTI3_PACKET_LEN - 2))
			{
				if(mti3_dma_buffer[(start+i) % MTI3_DMA_LEN] == PREAMBLE)
				{
					start = (start+i) % MTI3_DMA_LEN;
				}
				else if(mti3_dma_buffer[(start+i+1) % MTI3_DMA_LEN] == PREAMBLE)
				{
					start = (start+i+1) % MTI3_DMA_LEN;
				}
				end = (start + (MTI3_PACKET_LEN-1)) % MTI3_DMA_LEN;
			}
		}

		flag = 0;
	}
}

void decode_mti3()
{
	if(checksum() == 1)
	{
		/* euler angle */
		mti3_union.byte[0] = mti3_packet[10];
		mti3_union.byte[1] = mti3_packet[9];
		mti3_union.byte[2] = mti3_packet[8];
		mti3_union.byte[3] = mti3_packet[7];
		mti3.euler[ROLL] = mti3_union.data;

		mti3_union.byte[0] = mti3_packet[14];
		mti3_union.byte[1] = mti3_packet[13];
		mti3_union.byte[2] = mti3_packet[12];
		mti3_union.byte[3] = mti3_packet[11];
		mti3.euler[PITCH] = -mti3_union.data;

		mti3_union.byte[0] = mti3_packet[18];
		mti3_union.byte[1] = mti3_packet[17];
		mti3_union.byte[2] = mti3_packet[16];
		mti3_union.byte[3] = mti3_packet[15];
		mti3.euler[YAW] = -mti3_union.data;


		/* gyro
		mti3_union.byte[0] = mti3_packet[25];
		mti3_union.byte[1] = mti3_packet[24];
		mti3_union.byte[2] = mti3_packet[23];
		mti3_union.byte[3] = mti3_packet[22];

		// multiple "180/PI" for converting Radian to Degree(mti에서는 라디안 단위로 자이로 값 제공하므로 도 단위로 변환) 
		mti3.pqr[PQR_X] = mti3_union.data * 180/PI;

		mti3_union.byte[0] = mti3_packet[29];
		mti3_union.byte[1] = mti3_packet[28];
		mti3_union.byte[2] = mti3_packet[27];
		mti3_union.byte[3] = mti3_packet[26];

		mti3.pqr[PQR_Y] = -mti3_union.data * 180/PI;

		mti3_union.byte[0] = mti3_packet[33];
		mti3_union.byte[1] = mti3_packet[32];
		mti3_union.byte[2] = mti3_packet[31];
		mti3_union.byte[3] = mti3_packet[30];
		mti3.pqr[PQR_Z] = -mti3_union.data * 180/PI;
*/
		mti3_decode_flag = 1;
	}
	else
	{
		return;
	}
}

int checksum()
{
	int i;
	uint16_t temp = 0;
	uint16_t checksum = 0;

	for(i = 1; i < MTI3_PACKET_LEN; i++)
	{
	  temp += mti3_packet[i];
	}

	checksum = temp & 0x00ff;

	if(checksum == 0x0000)
		return 1;

	else
		return 0;
}
