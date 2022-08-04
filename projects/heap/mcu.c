#include "mcu.h"
#define MAXLINE 1024

char output_str[MAXLINE*2];
char msg[MAXLINE];
size_t output_offset=0;

void loop() {
	while(1) {}
}

// Append printed output to output_str
void var_print(char * str) {
	if (output_offset + strlen(str) <= MAXLINE*2) {
		strcat(output_str, str);
	} else {
		while(1){}
	}
}

// Hardfault Handler - Send exit signal
void HardFault_Handler(void) {
	// Force reset stack pointer in case of overflow
	void * sp_reset = (void *)0x20005000;
	asm volatile ("mov sp, %0" : "+r" (sp_reset));

	char err[] = "Hard Fault";
	var_print(err);
	loop();
}
