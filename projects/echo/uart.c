/*
 * Based on uart.c example by Furkan Cayci
 */

#include "uart.h"

char msg_buffer[BUFFERSIZE] = {0};

// Send content of pointer through uart
void uart_send(void * data, size_t size) {
	for (size_t i=0; i<size; i++){
		// Wait until TXE bit is set
		while(!(UART4->ISR & (1 << 7)));
		// Send character
		UART4->TDR = ((char *)data)[i];
	}
	// Wait for character transmit complete - TC bit
	while(!(UART4->ISR & (1 << 6))) {};
}

// Receive size bytes of content from uart and write it to buffer
void uart_receive(void * buffer, size_t size)  {
	for (size_t i=0; i < size; i++) {
		// Wait until RXNE bit is set
		while(!(UART4->ISR & (1 << 5)));
		// Receive character
		((char *)buffer)[i] = UART4->RDR;
	}

}

// Setup GPIO A2 and A3 pins for UART
static void uart_pin_setup(void) {
    // Enable GPIOA clock, bit 0 on AHB2ENR
    RCC->AHB2ENR |= (1 << 0);

    // Set pin modes as alternate mode 7 (PA0 and PA1)
    // UART4 TX and RX pins are PA0 (D1) and PA1(D0) respectively 
    GPIOA->MODER &= ~(0xFU << 0);
    GPIOA->MODER |=  (0xAU << 0);

    // Set pin modes as high speed
    GPIOA->OSPEEDR |= 0x0000000F;

    // Choose AF8 for UART4 in Alternate Function registers
    GPIOA->AFR[0] |= (0x8U << 0); // for pin A0
    GPIOA->AFR[0] |= (0x8U << 4); // for pin A1
}

// Initialize UART 4
static void uart_enable(void) {
    // enable UART4 clock
    RCC->APB1ENR1 |= (1 << 19);

	// Select Sysclk as UART4 Source
	RCC->CCIPR |= (1U << 6);

    // Disable uart4 - UE, bit 0
    UART4->CR1 &= ~(1 << 0);

	// Disable FIFO mode
	UART4->CR1 &= ~(1<<20);

	// Set word size to 8
	UART4->CR1 &= ~(1U<<12 | 1U<<28);

	// OVER8 = 0
	UART4->CR1 &= ~(1<<15);

	// For STM32L4S5: Sysclk = 120 Mhz (Sysclk/2), Baudrate = 115200, OVER8 = 0
	// USARTDIV = (1+OVER8) * fCK / baud
	// USARTDIV = 120Mhz / 115200 = 1041.67 ~ 1042
    UART4->BRR = 1042U;

	// Set stop bits to 1
	UART4->CR2 &= ~(0xF << 12);

	// Disable parity
	UART4->CR1 &= ~(1<<10);

	// Set Auto Baud detection to 0x55 frame detection
	UART4->CR2 |= (3U<<21);
	
	// Enable Auto Baud detection
	UART4->CR2 |= (1<<20);

    // UART4 TX enable, TE bit 3
    UART4->CR1 |= (1 << 3);

    // UART4 RX enable, RE bit 2
    UART4->CR1 |= (1 << 2);

    // Enable uart4 - UE, bit 0
    UART4->CR1 |= (1 << 0);
}	

void uart_baud_gen(void) {
	char temp[8] = {0};
	volatile uint32_t * brr = &(UART4->BRR);
	uart_receive(temp, 1);
	// Wait for auto baud generation to complete- ABRF bit
	while(!(UART4->ISR & (1 << 15))) {};
}

void uart_init(void)
{
	// Set system clock
	set_sysclk_to_120();

	uart_pin_setup();
	uart_enable();
	uart_baud_gen();
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

