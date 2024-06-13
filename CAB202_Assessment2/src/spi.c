#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Set up 7 seg display
void spi_init(void) {
    cli(); // Clears the global interupt flag
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc; // Sets the SPI0 peripheral to use alternate pin configuration
    PORTA.OUTSET = PIN1_bm; // Sets the output of PIN1 on PORTA high
    PORTA.DIRSET = PIN1_bm; // Sets PIN1 on PORTA as an output pin
    PORTC.DIRSET = PIN0_bm | PIN2_bm; // Sets PIN0 and PIN2 on PORTC as output pins

    SPI0.CTRLB = SPI_SSD_bm; // Sets the SPI Slave Select pin behavior to Software Controlled
    SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm; // Configures SPI as Master mode and enables SPI
    SPI0.INTCTRL = SPI_IE_bm; // Enables SPI interrupt
    sei(); // Sets the global interupt flag
}

// Write to the 7 seg display
void spi_write(uint8_t data) {
    SPI0.DATA = data; // Input in decimal or what segments of the display to light up
}

// Controlls the display of the 7 segment display
ISR(SPI0_INT_vect) {
    PORTA.OUTCLR = PIN1_bm; // Clear the output of PIN1
    PORTA.OUTSET = PIN1_bm; // Set the output of PIN1 high
    SPI0.INTFLAGS = SPI_IF_bm; // Clear the interrupt flag of SPI0 module
}