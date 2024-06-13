#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

/***
 * Ex E5.0
 * 
 * Your task is to write code to implement a two-tone siren 
 * using the QUTy. You must use one of the Waveform Outputs 
 * of TCA0 to drive the buzzer. The buzzer should always
 * be driven with a 50% duty cycle signal.
 * 
 * Your siren must use the following two frequencies:
 *     f1 = 2AB0 Hz
 *     f2 = 4CD0 Hz
 * The siren should alternate between f1 and f2, with 
 * f1 active for T1 = 3E0 ms, and f2 active for T2 = 6F0 ms, 
 * where A-F are the 2nd through 7th digits of your student
 * number, e.g. nXABCDEFX.
 * 
 * EXAMPLE: If your student number were n12345678, then
 *   A = 2, B = 3, C = 4, D = 5, E = 6, F = 7
 *   f1 = 2230 Hz, f2 = 4450 Hz, T1 = 360 ms, T2 = 670 ms
 * 
 * Your programme must be interrupt driven. The code you 
 * write in this file (extension05.c) will be compiled 
 * alongside a main.c which we provide. main() will call 
 * the init() function you write below and then drop into 
 * an infinite loop. The init() function you write MUST 
 * return (to demonstrate your code is interrupt driven).
 * 
 * In addition to init(), you may write any code you wish
 * in this file, including ISRs.
 * 
 * We have provided a copy of the main.c code we will use
 * to test your code, so that you can build and test your
 * code locally. You should not write any code in main.c
 * as it will be replaced when you upload your programme.
 */ 
// n11275561
// f1 = 2120 Hz, t1 = 350 ms
// f2 = 4750 Hz, t2 = 660 ms

//n = 10^-9
// TOP = 1 / (2120 * 300n)
// TOP = 1572
// TOP = 1 / (4750 * 300n)
// TOP = 701
// f1 = 1572 * 0.5 = 786
// f2 = 701 * 0.5 = 350

void pwm_init(void) {
    PORTB.DIRSET = PIN0_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
    // f1
    TCA0.SINGLE.PERBUF = 1572;
    TCA0.SINGLE.CMP0BUF = 786;
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}
void timer_init(void) {
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    TCB0.CCMP = 3333;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_ENABLE_bm;
}

void init(void) {
    cli();
    pwm_init();
    timer_init();
    sei();
}

// Unsure where I could use this. Mainly for learning purposes
// fclk = 20000000 / 6
// desiredT = (X)ms * 10^-3
// desiredf = 1/desiredT
// CCMP = fclk/desiredf

// desiredT = 350 * 10^-3
// desiredf = 1/0.35
// CCMP = fclk/2.85714
// CCMP = 1166667

// desiredT = 660 * 10^-3
// desiredf = 1/0.66
// CCMP = fclk/1.51515
// CCMP = 2200002

ISR(TCB0_INT_vect) {
    uint8_t static state = 0;
    uint16_t static counter = 0;
    uint16_t duration1 = 350;
    uint16_t duration2 = 660;
    counter++;
    // t1
    if (state == 0 && counter == duration1) {
        // f2
        TCA0.SINGLE.PERBUF = 701;
        TCA0.SINGLE.CMP0BUF = 350;
        state = 1;
    }
    // 2
    else if (state == 1 && counter == duration1 + duration2) {
        // f1
        TCA0.SINGLE.PERBUF = 1572;
        TCA0.SINGLE.CMP0BUF = 786;
        counter = 0;
        state = 0;
    }
    TCB0.INTFLAGS = TCB_CAPT_bm;
}

// Write your code for Ex E5.0 above this line.

