#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

// Setup for buzzer
void pwm_init(void) {
    cli(); // Clears the global interupt flag
    PORTB.DIRSET = PIN0_bm; // Sets PIN0 on PORTB as an output pin for PWM signal
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc; // Sets the clock source of TCA0 to be divided by 1
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm; // Configures TCA0 in Single Slope PWM mode and enables CMP0 output
    
    // Sets the current state of the buzzer to off
    TCA0.SINGLE.PERBUF = 0;
    TCA0.SINGLE.CMP0BUF = 0;

    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm; // Enables TCA0 operation
    sei(); // Sets the global interupt flag
}
