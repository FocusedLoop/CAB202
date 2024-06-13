#include <avr/io.h>
#include <stdio.h>

#include "qutyserial.h"
#include "timer.h"
#include "spi.h"


#define INIT_PATTERN 0b01110111  // '-' on 7-seg

    //    ABCDEFG
    // 0: 0000001
    // 1: 1001111
    // 2: 0010010
    // 3: 0000110
    // 4: 1001100
    // 5: 0100100
    // 6: 0100000
    // 7: 0001111
    // 8: 0000000
    // 9: 0000100

    //    xFABGCDE
    // 0: 00001000
    // 1: 01101011
    // 2: 01000100
    // 3: 01000001
    // 4: 00100011
    // 5: 00010001
    // 6: 00010000
    // 7: 01001011
    // 8: 00000000
    // 9: 00000001
volatile uint8_t segs [] = {
    0x08,0x6B,0x44,0x41,0x23,0x11,0x10,0x4B,0x00,0x01
};

volatile uint8_t pattern1 = 0;    
volatile uint8_t pattern2 = 0;   

volatile uint8_t pb_debounced = 0xFF;

void calc_digits (uint8_t count){
    uint8_t num, tens=0;

    num = count;
    while (num> 9){
        num -= 10;
        tens++;
    }
    pattern1 = segs[tens];
    pattern2 = segs[num];
}

void pb_init(void) {
    // already configured as inputs

    //PBs PA4-7, enable pullup resistors
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
}

void pb_debounce(void) {
    static uint8_t vcount0=0, vcount1=0;   //vertical counter bits
     
    uint8_t pb_sample = PORTA.IN;

    uint8_t pb_changed = (pb_sample ^ pb_debounced);

    //vertical counter update
    vcount1 = (vcount1 ^ vcount0) & pb_changed;  //vcount1 (bit 1 of vertical counter)
    vcount0 = ~vcount0 & pb_changed;             //vcount0 (bit 0 of vertical counter)

    pb_debounced ^= (vcount0 & vcount1);         //toggle pb_debounced
}

typedef enum {
  INIT,
  THREE,
  TWO,
  ONE,
  GO
} state_type;

int main (void) {  
    serial_init();
    pb_init();
    timer_init();
    spi_init();

    uint8_t pb_sample = 0xFF;
    uint8_t pb_sample_r = 0xFF;
    uint8_t pb_changed, pb_rising, pb_falling;
    uint8_t count = 0;

    state_type state = INIT;
    pattern1 = pattern2 = INIT_PATTERN;

    while (1) {
        pb_sample_r = pb_sample;
        pb_sample = pb_debounced;
        pb_changed = pb_sample ^ pb_sample_r;

        pb_falling = pb_changed & pb_sample_r;
        pb_rising = pb_changed & pb_sample;

        //if (pb_falling & PIN4_bm) {    
            //S1 pressed
        //    printf("S1 %u\n",count);

        //    count = (count == 99)? 0 : count + 1;
        //    calc_digits (count);
        //}

        switch(state) {
            case INIT:
                if (pb_rising & PIN4_bm) {  //S1 released
                    state = THREE;          //next state is THREE
                    printf("3...");
                    pattern1 = segs[0];
                    pattern2 = segs[3];
                }   
                break;

            case THREE:
                if (pb_rising & PIN4_bm) {  //S1 released
                    state = TWO;            //next state is TWO
                    printf("2...");
                    pattern1 = segs[0];
                    pattern2 = segs[2];                    
                }             
                break;

            case TWO:
                if (pb_rising & PIN4_bm) {  //S1 released
                    state = ONE;            //next state is ONE
                    printf("1...");
                    pattern1 = segs[0];
                    pattern2 = segs[1];                    
                }             
                break;

            case ONE:
                if (pb_rising & PIN4_bm) {  //S1 released
                    state = GO;             //next state is GO
                    printf("GO!\n");
                    pattern1 = segs[0];
                    pattern2 = segs[0];                    
                }             
                break;

            case GO:
                if (pb_rising & PIN7_bm) {  //S1 released
                    state = INIT;            //next state is INIT
                    pattern1 = pattern2 = INIT_PATTERN;
                }              
                break;

            default:
                state = INIT;            //next state is INIT     
                pattern1 = pattern2 = INIT_PATTERN;                       
        }//switch
    }//while
}//main
