/*
 * Modified from Furkan Cayci's STM32F407 timer.c example
 */
#include "mcu_timer.h"
#include "memlib.h"
#include "mcu_mm.h"
#include "uart.h"
#include "mcu_init.h"

#define MAXLINE 1024

// Current systime runtime in ms
static size_t t2_time = 0;
static size_t t3_time = 0;

// Whether or not timer testing is on
// static int t2_test = 0;
// static int t3_test = 0;

// TIM2 interrupt handler - Update system time
void TIM2_IRQHandler(void)
{
	/*
	// Toggle every 5000 interrupts = 5 seconds
	if (t2_test) {
		static size_t i=0;
		i++;
		if (!(i%5000)) {
			led_toggle(LD1);
		}
	}
	*/

	t2_time++;

	// Reset watchdog bits
	// WWDG->CR |= 0x7F;

    // clear interrupt status
    if (TIM2->DIER & 0x01) {
        if (TIM2->SR & 0x01) {
            TIM2->SR &= ~(1U << 0);
        }
    }
}

// TIM3 interrupt handler - Check for stack overflow
void TIM3_IRQHandler(void)
{
	// Check for stack overflow
	register size_t * stack_top asm("sp");

	/*
	// Toggle every 500 interrupts = 5 seconds
	if (t3_test) {
		static size_t i=0;
		i++;
		if (!(i%500)) {
			led_toggle(LD1);
		}
	}
	*/

	// Stall if stack is overflowing to heap
	if (mem_heap_hi() > (void *)(stack_top)) {
		char err[] = "Stack overflow detected";
		var_print(err);
		mm_finish();
		loop();
	}	

    // clear interrupt status
    if (TIM3->DIER & 0x01) {
        if (TIM3->SR & 0x01) {
            TIM3->SR &= ~(1U << 0);
        }
    }

}

// Returns system time in 0.1 ms
size_t get_time(void) {
	return t2_time;
}

// Initialize timers
void timer_init(void)
{
	// TIM2 - Keeps track of system time
    // Enable TIM2 clock (bit0)
    RCC->APB1ENR1 |= (1 << 0);
	// Formula: Clock speed = (sysclk/AHB_prescaler/APB1_prescaler)/(PSC+1)
	// Multiply clock speed by 2 if APB_prescaler is not 1
	// For STM32L4S5: 120M/1/2*2 = 120M, 120M/(11999+1) = 10 khz clock speed
    TIM2->PSC = 11999;
	// Set auto reload value to 10 to give 1 ms timer interrupts
    TIM2->ARR = 10;
    // Update Interrupt Enable
    TIM2->DIER |= (1 << 0);
    NVIC_SetPriority(TIM2_IRQn, 2); // Priority level 2
    // enable TIM2 IRQ from NVIC
    NVIC_EnableIRQ(TIM2_IRQn);
	// Enable TIM2 Module
	TIM2->CR1 |= (1<<0);

	// TIM3 - Checks for stack overflow
    
	// Enable TIM3 clock (bit1)
    RCC->APB1ENR1 |= (1 << 1);
	// For STM32L4S5: 120M/1/2*2 = 120M, 120M/(11999+1) = 10 khz clock speed
    TIM3->PSC = 11999;
	// Set auto reload value to 100 to give 10 ms timer interrupts
    TIM3->ARR = 100;
    // Update Interrupt Enable
    TIM3->DIER |= (1 << 0);
    NVIC_SetPriority(TIM3_IRQn, 3);
    // enable TIM3 IRQ from NVIC
    NVIC_EnableIRQ(TIM3_IRQn);
    // Enable Timer 3 module (CEN, bit0)
    TIM3->CR1 |= (1 << 0);
}

// Toggle LD1 Every 5 seconds
void TIM2_test(void) {
	// Uncomment test variable declarations and toggle blocks in timer interrupts
	// t2_test=1;
	while (1) {
	}
}

// Toggle LD1 Every 5 seconds
void TIM3_test(void) {
	// t3_test=1;
	// Uncomment test variable declarations and toggle blocks in timer interrupts
	while (1) {
	}
}
