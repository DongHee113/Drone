#include "rf.h"

uint8_t rf_decode_flag;
uint8_t rf_dma_buffer[RF_DMA_LEN];
uint8_t rf_flag = 0;
uint8_t rf_packet[RF_PACKET_LEN];
PID_GAIN rx_gain;

void read_rf()
{
	get_rf_packet();
	if(rf_flag)
	{
		decode_rf();
		rf_flag = 0;	
	}
}

void get_rf_packet()
{
	static int flag = 0;
	static int last_ndt = 0;
	static int now_ndt = 0;
	static int start = 0;
	static int end = RF_PACKET_LEN - 1;
	int i;

	last_ndt = now_ndt;
	now_ndt = RF_DMA_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
	
	if(now_ndt == last_ndt)
	{
		return;
	}
	else if(now_ndt > last_ndt)
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
	
	if(flag)
	{
		if(rf_dma_buffer[start] == HEADER && rf_dma_buffer[end] == TAIL)
		{
			if(start < end)
			{
			  memcpy(rf_packet, &rf_dma_buffer[start], RF_PACKET_LEN);
			}
			else if(end < start)
			{
			  memcpy(rf_packet, &rf_dma_buffer[start], RF_DMA_LEN - start);
			  memcpy(&rf_packet[RF_DMA_LEN - start], rf_dma_buffer, end + 1);
			}
			start = (end+1) % RF_DMA_LEN;
			end = (start + (RF_PACKET_LEN-1)) % RF_DMA_LEN;
			rf_flag = 1;
		}
		else
		{
			for(i = 1; i < RF_PACKET_LEN; i++)
			{
				if(rf_dma_buffer[(start+i) % RF_DMA_LEN] == HEADER)
				{
					start = (start+i) % RF_DMA_LEN;
					end = (start + (RF_PACKET_LEN-1)) % RF_DMA_LEN;
					break;
				}
			}
			
			if(i == RF_PACKET_LEN)
			{
				start = (start+i) % RF_DMA_LEN;
				end = (start + (RF_PACKET_LEN-1)) % RF_DMA_LEN;
			}
		}
		flag = 0;
	}
}

void decode_rf()
{
	float temp = 0;
	
	temp += 1e+2 * (rf_packet[2] - '0');
	temp += 1e+1 * (rf_packet[3] - '0');
	temp += 1e+0 * (rf_packet[4] - '0');
	temp += 1e-1 * (rf_packet[6] - '0');
	temp += 1e-2 * (rf_packet[7] - '0');
	temp += 1e-3 * (rf_packet[8] - '0');
	
	switch (rf_packet[1])
	{
		case 'A' :
			rx_gain.stab_kp[ROLL] = temp;
			break;
		case 'B' :
			rx_gain.rate_kp[ROLL] = temp;
			break;
		case 'C' :
			rx_gain.rate_ki[ROLL] = temp;
			break;
		case 'D' :
			rx_gain.rate_kd[ROLL] = temp;
			break;
		case 'E' :
			rx_gain.stab_kp[PITCH] = temp;
			break;
		case 'F' :
			rx_gain.rate_kp[PITCH] = temp;
			break;
		case 'G' :
			rx_gain.rate_ki[PITCH] = temp;
			break;
		case 'H' :
			rx_gain.rate_kd[PITCH] = temp;
			break;
		case 'I' :
			rx_gain.stab_kp[YAW] = temp;
			break;
		case 'J' :
			rx_gain.rate_kp[YAW] = temp;
			break;
		case 'K' :
			rx_gain.rate_ki[YAW] = temp;
			break;
		case 'L' :
			rx_gain.rate_kd[YAW] = temp;
			break;
		case 'M' :
			rx_gain.alt_kp = temp;
			break;
		case 'N' :
			rx_gain.alt_ki = temp;
			break;
		case 'O' :
			rx_gain.alt_kd = temp;
			break;
	}
}

void update_pid_gain()
{
	pid_gain = rx_gain;
}