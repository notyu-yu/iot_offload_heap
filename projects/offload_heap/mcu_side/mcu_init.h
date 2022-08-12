#include "mcu.h"

typedef enum {
	LD1,
	LD2
} led;

// LED functions
void led_init(void); // Initialize LED GPIO pins
void led_on(led l); // Turn on LED
void led_off(led l); // Turn off LED
void led_toggle(led l); // Toggle LED state

void mcu_init(void); // Initialize LED and fault handers
