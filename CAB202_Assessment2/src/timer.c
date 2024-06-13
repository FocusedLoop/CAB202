#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Set up timer
void timer_init() {
    cli(); // Clears the global interupt flag
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;    // Configure TCB0 in periodic interrupt mode
    TCB0.CCMP = 3333;                   // Set interval for 1ms (3333 clocks @ 3.3 MHz)
    TCB0.INTCTRL = TCB_CAPT_bm;         // CAPT interrupt enable
    TCB0.CTRLA = TCB_ENABLE_bm;         // Enable
    sei(); // Sets the global interupt flag
}