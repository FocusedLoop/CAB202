#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BASE_OCTAVE  127
#define A440  7575

volatile uint16_t octave = BASE_OCTAVE;
volatile uint16_t period = A440;
volatile uint16_t delayms = 2000;
volatile uint8_t currently_playing = 0;

void init_uart(void) {
    cli();
    USART0.BAUD = 1389;
    USART0.CTRLA = USART_RXCIE_bm;
    USART0.CTRLB = USART_RXEN_bm;
    sei();
}

void uart_putc(char c) {
    while (!(USART0.STATUS & USART_DREIF_bm));
    USART0.TXDATAL = c;
}
void uart_puts(char *s) {
    while (*s != '\0') {
        uart_putc(*s);
        s++;
    }
}

void init_tca(void) {
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;
    // 440hz = 7575

    TCA0.SINGLE.CTRLB = TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
    TCA0.SINGLE.PER = period;
    TCA0.SINGLE.CMP0 = period;

    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void init_tcb(void) {
    cli();
    TCB0.CTRLA = TCB_CLKSEL_DIV1_gc;
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CCMP = 3333;
    TCB0.CTRLA |= TCB_ENABLE_bm;
    sei();
}

volatile uint16_t timerticks = 0xFFFF;

ISR(TCB0_INT_vect) {
    if (timerticks < 0xFFFF) timerticks++;
    TCB0.INTFLAGS = TCB_CAPT_bm;
}

void update_cmpbuf(void) {
    if (currently_playing) {
        TCA0.SINGLE.CMP0BUF = period >> 1;
    } else {
        TCA0.SINGLE.CMP0BUF = period;
    }
}

void set_octave(uint16_t new_oct) {
    octave = new_oct;
    period = A440;
    if (new_oct > BASE_OCTAVE) { period >>= (new_oct - BASE_OCTAVE); }
    if (new_oct < BASE_OCTAVE) { period <<= (BASE_OCTAVE - new_oct); }

    TCA0.SINGLE.PERBUF = period;
    update_cmpbuf();
}

/*
For serial:
'p' does what S1 does.
's' immediately turns the BUZZER OFF.
',' does what S3 does.
'.' does what S4 does.
 
'd' can be used to set X using 4 hexadecimal characters
*/

ISR(USART0_RXC_vect) {
    static uint8_t place = 4;
    static uint16_t input = 0;
    char rx = USART0.RXDATAL;
    switch (rx) {
        case 'p':
            timerticks = 0;
            currently_playing = 1;
            update_cmpbuf();
            break;
        case 's':
            currently_playing = 0;
            update_cmpbuf();
            break;
        case ',':
            set_octave(octave - 1);
            break;
        case '.':
            set_octave(octave + 1);
            break;
        case 'd':
            if (place == 4) {
                place = 0;
                input = 0;
                break;
            }
        default:
            if (place < 4) {
                uint8_t hexdigit;
                if (rx >= '0' && rx <= '9') {
                    hexdigit = rx - '0';
                } else if (rx >= 'a' && rx <= 'f') {
                    hexdigit = rx - 'a' + 10;
                } else {
                    break;
                }
                input |= hexdigit << 12 >> (place << 2);
                place++;
                if (place == 4) {
                    delayms = input;
                }
            }
            break;
    }
}

int main(void) {
    PORTB.OUTCLR = PIN0_bm;
    PORTB.DIRSET = PIN0_bm;

    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;

    init_tca();
    init_tcb();
    init_uart();

    uint8_t prev_porta = 0xFF;
    for (;;) {
        uint8_t porta_in = PORTA.IN;
        if ((porta_in & PIN4_bm) == 0) {
            if (prev_porta & PIN4_bm) {
                timerticks = 0;
            }
            currently_playing = 1;
            update_cmpbuf();
        } else {
            if (timerticks >= delayms) {
                currently_playing = 0;
                update_cmpbuf();
            }
        }

        if ((porta_in & PIN6_bm) == 0) {
            if (prev_porta & PIN6_bm) {
                set_octave(octave - 1);
            }
        }
        if ((porta_in & PIN7_bm) == 0) {
            if (prev_porta & PIN7_bm) {
                set_octave(octave + 1);
            }
        }

        prev_porta = porta_in;
    }
    
    for (;;);
}
