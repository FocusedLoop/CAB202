#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

void spi_init(void) {
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;
    PORTA.DIRSET = PIN1_bm;
    PORTC.DIRSET = PIN0_bm | PIN2_bm;

    cli();
    SPI0.CTRLB = SPI_SSD_bm;
    SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm;
    SPI0.INTCTRL = SPI_IE_bm;
    sei();
}

void spi_write(uint8_t data) {
    SPI0.DATA = data;
}

ISR(SPI0_INT_vect) {
    PORTA.OUTCLR = PIN1_bm;
    PORTA.OUTSET = PIN1_bm;
}