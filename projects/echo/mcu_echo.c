#include "uart_dma.h"

#define READSIZE(buffer) *(size_t *)buffer

// Returns pointer to msg without padding
static char * msg_offset(char * msg) {
	for (size_t i=0; i<BUFFERSIZE; i++) {
		if (msg[i]) {
			return msg + i;
		}
	}
	return msg;
}

// Send size bytes at data pointer, using method defined by USE_DMA macro
static void send(void * data, size_t size) {
	/*
	if (USE_DMA) {
		uart_tx_start(data, size);
		uart_tx_wait();
	} else {
	*/
		uart_send(data, size);
	//}
}

// Receive size bytes at buffer pointer, using method defined by USE_DMA macro
static void receive(void * buffer, size_t size) {
	/*
	if (USE_DMA) {
		uart_rx_start(buffer, size);
		uart_rx_wait();
	} else {
	*/
		uart_receive(buffer, size);
	//}
}

int main(void) {
	char debug_msg[BUFFERSIZE] = {0};
	char * stripped_msg;
	size_t received_size=0;
	int discard_cali = 0; // Discard baud calibration character after first communication

	uart_init();
	led_init();
	//uart_dma_init();

	while(1) {
		if (discard_cali) {
			// Discard Auto Baud calibration character
			receive(msg_buffer, 1);
			memset(msg_buffer, 0, 1);
		} else {
			discard_cali = 1;
		}

		// Read received buffer size
		led_on(LD1);
		receive(msg_buffer, sizeof(uint32_t));
		received_size = READSIZE(msg_buffer);
		memset(msg_buffer, 0, sizeof(uint32_t));
		led_off(LD1);

		// Send back buffer content
		if (received_size > BUFFERSIZE) {
			// Message too large
			sprintf(debug_msg, "Attempting to read %u bytes larger than buffer size %d.\n", (unsigned int)received_size, BUFFERSIZE);
			send(debug_msg, strlen(debug_msg));
		} else {
			// Read message
			receive(msg_buffer, received_size);

			// Echo message
			stripped_msg = msg_buffer;
			send(stripped_msg, strlen(stripped_msg));
			memset(msg_buffer, 0, received_size);
		}
	}
}
