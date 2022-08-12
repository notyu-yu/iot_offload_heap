#include "../../../include/stm32l4s5xx.h"
#include "../../../include/system_stm32l4xx.h"

#include <stdio.h>
#include <string.h>

#define MAXLINE 1024

// For printing debug output
extern size_t output_offset;
extern void * sp_reset;

void loop(void); // Infinite loop for gdb break point
void var_print(char * str); // Print to debug output buffer
