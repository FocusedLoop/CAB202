#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

// Set up for potentiometer, effects the duration of the playback delay
void adc_init(void) {
    ADC0.CTRLA = ADC_ENABLE_bm; // Enable ADC module
    ADC0.CTRLB = ADC_PRESC_DIV2_gc; // Set ADC prescaler division factor to 2
    ADC0.CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc; // Set ADC timebase and reference voltage source
    ADC0.CTRLE = 64; // Set ADC sampling delay
    ADC0.CTRLF = ADC_FREERUN_bm; // Enable ADC free running mode
    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc; // Set ADC positive input to AIN2 channel
    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc; // Configure ADC mode and start conversion
}