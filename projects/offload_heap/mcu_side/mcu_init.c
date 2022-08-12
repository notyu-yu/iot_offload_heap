#include "mcu_init.h"
#include "mcu_mm.h"
#include "mcu_timer.h"

// Hardfault Handler - Send exit signal
void HardFault_Handler(void) {
	// Force reset stack pointer in case of overflow
	sp_reset = (void *)0x20005000;
	asm volatile ("mov sp, %0" : "+r" (sp_reset));

	char err[] = "Hard Fault";
	var_print(err);
	mm_finish();
	loop();
}

/*
// Timer interrupt stopped running - Send exit signal and reset
void WWDG_IRQHandler(void) {
	// Force reset stack pointer in case of overflow
	sp_reset = (void *)0x20005000;
	asm volatile ("mov sp, %0" : "+r" (sp_reset));

	char err[] = "WWDG error";
	var_print(err);
	mm_finish();
	loop();
}
*/

// Memory fault handler -  Send exit signal
void MemManage_Handler(void) {
	// Force reset stack pointer in case of overflow
	sp_reset = (void *)0x20005000;
	asm volatile ("mov sp, %0" : "+r" (sp_reset));

	char err[] = "Memory error";
	var_print(err);
	mm_finish();
	loop();
}

/* Usually not needed if HardFault_Handler works correctly
// Initialize WWDG
void wwdg_init(void) {
	// Enable clock
	RCC->APB1ENR |= (1<<11);

	// timeout (ms) = T_PCLK1(with ms) * 4096 * 2 ^(P) * (T + 1)
	// Max and min timeout calculated with T being 0x3F (63) and 0x00
	// APB1 Frequency 25 Mhz - Period: T_PCLK1 = 1/25,000 (in ms)
	// Min timeout: (1/25,000) * 4096 * 2^1 * 1 = 0.328 ms
	// Max timeout: (1/25,000) * 4096 * 2^1 * (63 + 1) = 20.972 ms

	WWDG->CFR |= (0x1 << 7); // Set timer base/prescaler - P
	WWDG->CFR |= (0x70); // Window countdown value - T
	WWDG->CR |= (0xFF); // Enable WWDG

	NVIC_SetPriority(WWDG_IRQn, 1);
	NVIC_EnableIRQ(WWDG_IRQn);
}
*/

// Initialize memory fault handler
void memfault_init(void) {
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk; // Enable memfault, bit 16
	NVIC_SetPriority(MemoryManagement_IRQn, 0);
}

// Turn on LED
void led_on(led l) {
	switch (l) {
		case LD1:
			GPIOA->ODR |= (1U<<5);
			break;
		case LD2:
			GPIOB->ODR |= (1U<<14);
			break;
		default:
			break;
	}
}

// Turn off LED
void led_off(led l) {
	switch (l) {
		case LD1:
			GPIOA->ODR &= ~(1U<<5);
			break;
		case LD2:
			GPIOB->ODR &= ~(1U<<14);
			break;
		default:
			break;
	}
}

// Toggle LED
void led_toggle(led l) {
	switch (l) {
		case LD1:
			GPIOA->ODR ^= (1U<<5);
			break;
		case LD2:
			GPIOB->ODR ^= (1U<<14);
			break;
		default:
			break;
	}
}

// Setup LED GPIO
void led_init(void) {
	// Enable GPIOA and GPIOB clock
	RCC->AHB2ENR |= (1U << 0);
	RCC->AHB2ENR |= (1U << 1);

	// Turn on output mode on A5 and B14
	GPIOA->MODER &= ~(0xFU << 10);
	GPIOA->MODER |= (1U << 10);
	GPIOB->MODER &= ~(0xFU << 28);
	GPIOB->MODER |= (1U << 28);

	// Turn off LEDs
	GPIOA->ODR &= ~(1U<<5);
	GPIOB->ODR &= ~(1U<<14);
}

// Set up LED and fault handlers
void mcu_init(void) {
	set_sysclk_to_120();
	//wwdg_init();
	memfault_init();
	led_init();
	// Make SVC call priority 6
	NVIC_SetPriority(SVCall_IRQn, 6);
}
