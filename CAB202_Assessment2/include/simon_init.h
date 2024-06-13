#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void pwm_init();
void timer_init();
void buttons_init();
void adc_init(void);

void spi_init(void);
void spi_write(uint8_t data);

void uart_init(void);
uint8_t uart_getc(void);
void uart_putc(uint8_t c);
void uart_puts(char* string);