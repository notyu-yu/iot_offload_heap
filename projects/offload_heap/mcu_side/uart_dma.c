#include "uart_dma.h"

// DMA status indicators
static int receiving=0;
static int transmitting=0;

// Start uart transmission of size bytes of data
void uart_tx_start(void * data, size_t size) {
	uart_tx_wait();
	//uart_tx_setup();
	
	// Reset CCR
	DMA1_Channel1->CCR = 0U;

	// Source memory address
	DMA1_Channel1->CMAR = (uint32_t)data;
	// Destination memory address
	DMA1_Channel1->CPAR = (uint32_t)&(UART4->TDR);
	// Transfer size
	DMA1_Channel1->CNDTR = size;

	// CCRx Configuration
	DMA1_Channel1->CCR |= (3U << 12); // Very high priority
	DMA1_Channel1->CCR |= (1U << 4); // Memory to peripheral
	DMA1_Channel1->CCR |= (1U << 7); // Enable memory increment
	//DMA1_Channel1->CCR |= (0U << 6); // Disable peripheral increment
	DMA1_Channel1->CCR |= (1U << 1); // Full transfer interrupt enable - TCIE

	// Enable transfer Complete interrupt
	NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	// Configure DMAMUX Channel
	DMAMUX1_Channel0->CCR = 0U; // Reset config
	DMAMUX1_Channel0->CCR |= (31U << 0); // Select UART4 TX as request id
	DMAMUX1_Channel0->CCR |= (0U << 19); // Forward 1 request

	// Clear TC bit
	UART4->ICR |= (1U<<6);

	// Enable DMA
	DMA1_Channel1->CCR |= (1U << 0);

	transmitting=1;
}

// Wait for uart transmission to finish
void uart_tx_wait(void) {
	while (transmitting);
}

// Start uart reception of size bytes of data into buffer
void uart_rx_start(void * buffer, size_t size) {
	uart_rx_wait();

	volatile uint32_t * c2ccr = &(DMA1_Channel2->CCR);

	// Reset CCR
	DMA1_Channel2->CCR = 0;

	// Source memory address
	DMA1_Channel2->CPAR = (uint32_t)&(UART4->RDR);
	// Destination memory address
	DMA1_Channel2->CMAR = (uint32_t)buffer;
	// Transfer size
	DMA1_Channel2->CNDTR = size;

	// CCRx Configuration
	DMA1_Channel2->CCR |= (3U << 12); // Very high priority
	//DMA1_Channel2->CCR |= (0U << 4); // Peripheral to memory
	DMA1_Channel2->CCR |= (1U << 7); // Enable memory increment
	//DMA1_Channel2->CCR |= (0U << 6); // No peripheral increment
	DMA1_Channel2->CCR |= (1U << 1); // Full transfer interrupt enable - TCIE

	// Enable transfer Complete interrupt
	NVIC_SetPriority(DMA1_Channel2_IRQn, 2);
	NVIC_EnableIRQ(DMA1_Channel2_IRQn);

	// Configure DMAMUX Channel
	DMAMUX1_Channel1->CCR = 0U; // Reset config
	DMAMUX1_Channel1->CCR |= (30U << 0); // Select UART4 RX as request id
	DMAMUX1_Channel1->CCR |= (0U << 19); // Forward 1 request

	// Enable DMA
	DMA1_Channel2->CCR |= (1U << 0);

	receiving=1;
}

// Wait for uart reception to finish
void uart_rx_wait(void) {
	while (receiving);
}

// UART reception finish interrupt
void DMA1_Channel2_IRQHandler(void)
{
    // clear stream receive complete interrupt - bit11 for stream 5
    if (DMA1->ISR & DMA_ISR_TCIF2) {
        // clear interrupt
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
		receiving = 0;
		// Disable receive DMA
		// USART2->CR3 &= ~USART_CR3_DMAR;
    }
}

// UART transmission finish interrupt
void DMA1_Channel1_IRQHandler(void)
{
	// Wait for character transmit complete - TC bit
	while(!(UART4->ISR & (1 << 6))) {};
    // Clear stream transfer complete interrupt
    if (DMA1->ISR & DMA_ISR_TCIF1) {
        // clear interrupt
        DMA1->IFCR |= DMA_IFCR_CTCIF1;
		transmitting = 0;
    }
}

// Setup UART DMA
void uart_dma_init(void) {
	// Enable transmit DMA
	UART4->CR3 |= (1U<<7);
	// Enable receive DMA
	UART4->CR3 |= (1U<<6);
	// Enable DMA1 clock
	RCC->AHB1ENR |= (1U<<0);
	// Enable DMAMUX clock
	RCC->AHB1ENR |= (1U<<2);
}
