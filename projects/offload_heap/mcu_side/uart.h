#include "mcu.h"
#include "uart_comms.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Message buffer
extern char msg_buffer[BUFFERSIZE];

// UART Functions
void uart_init(void); // Initialize uart registers
void uart_send(void * data, size_t size); // Send size bytes of data starting at data pointer
void uart_receive(void * buffer, size_t size); // Receive size bytes of data and write to buffer
void uart_wait_receive(void * buffer, size_t size); // Same as receive but stall until message is sent
