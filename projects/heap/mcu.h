#include "../../include/stm32l4s5xx.h"
#include "../../include/system_stm32l4xx.h"

#include <stdio.h>
#include <string.h>

#define MAXLINE 1024

extern char msg[MAXLINE];
extern char output_str[MAXLINE*2];
extern size_t output_offset;

void loop(void);
void var_print(char * str);
