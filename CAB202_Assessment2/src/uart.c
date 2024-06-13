#include <avr/io.h>
#include <stdint.h>

// Sets up the uart 
void uart_init(void) {
    PORTB.DIRSET = PIN2_bm; // Enable PB2 as output
    USART0.BAUD = 1389;     // Sets the buad rate to 9600 @ 3.3 MHz
    USART0.CTRLA = USART_RXCIE_bm; // Enables the receive complete interrupt for the UART
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;   // Enable Tx/Rx, enables the UART's receiver and transmitter, allowing communication through the serial interface
}

// Recieves a character inputted into the uart
uint8_t uart_getc(void) {
    while (!(USART0.STATUS & USART_RXCIF_bm));  // Wait for data or input
    return USART0.RXDATAL; // Retrieves data from the uart
}

// Displays desired characters to the uart
void uart_putc(uint8_t c) {
    while (!(USART0.STATUS & USART_DREIF_bm));  // Wait for TXDATA empty
    USART0.TXDATAL = c; // Returns the desired character to the uart
}

// Display desired strings to the uart
void uart_puts(char* string) {
    uint8_t *s = (uint8_t*) string; // Create a pointer to the input string, treating it as an array of bytes
    while(*s) { // Iterate through the string until reaching the null terminator
        uart_putc(*(s++)); // Call uart_putc() to transmit each character and move the pointer to the next character
    }
}