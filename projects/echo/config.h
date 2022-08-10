#define BUFFERSIZE 2048 // Maximum message size sent through UART
#define BAUDRATE B4000000 // Mantissa and fraction bit in uart.c need to be manually set. Format is B$(baudrate)
#define VERBOSE 0 // Change to 1 to display debug messages
#define SERIALDEV "/dev/ttyUSB0" // USB serial device file
#define USE_DMA 0 // Whether or not to use DMA
#define PADDINGSIZE 4
