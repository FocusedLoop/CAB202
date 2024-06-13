#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

/***
 * Ex E6.0
 * 
 * Your task is to write code which handles a sequence of input characters
 * from the UART interface, and respond with the output specified below.
 * It is strongly recommended that you design a state machine to complete
 * this task, and practice drawing a state machine diagram before you
 * begin coding.
 * 
 * On receipt of the character sequence: 
 *   "foo" your programme should print '0' to the UART interface.
 *   "bar" your programme should print '1' to the UART interface.
 *   "foobar" your program should not print either '0' nor '1' as
 *   specified above, but should instead print a linefeed ('\n') 
 *   character.
 * 
 * Your solution should use a baud rate of 9600, and 8N1 frame format.
 * Your solution MUST NOT use qutyio.o or qutyserial.o.
 * 
 * Examples:
 * 
 *   Input:  
 *     ...foo.bar.foo.barfoobarfood
 *   Output: 
 *     0101
 *     0
 * 
 *   Input:
 *     barsfoosbarforbarfoobarrforfoobarfoobarfood
 *   Output:
 *     1011
 *     
 *     
 *     0
 */

// foo = 0
// bar = 1
// foobar = \n
void uart_init(void) {
    PORTB.DIRSET = PIN2_bm; // Enable PB2 as output (USART0 TXD)
    USART0.BAUD = 1389;     // 9600 baud @ 3.3 MHz
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;   // Enable Tx/Rx
}

uint8_t uart_getc(void) {
    while (!(USART0.STATUS & USART_RXCIF_bm)); // Wait for data
    return USART0.RXDATAL;
}

void uart_putc(uint8_t c) {
    while (!(USART0.STATUS & USART_DREIF_bm)); // Wait for TXDATA empty
    USART0.TXDATAL = c;
}

void uart_puts(char* string) {
    uint8_t *s = (uint8_t*) string;
    while(*s) {
        uart_putc(*(s++));
    }
}

typedef enum {
    start,
    middle,
    middle_2,
    end,
    end_2,
    foob,
    fooba
    };

int main(void) {
    uart_init();
    uint8_t state = start;
    char input = 0;
    uint8_t foobar = 0;
    while(1){
        switch (state){
        case start:
            input = uart_getc();

            if (input == 'f'){
                //foobar = 0;
                state = middle;
            }
            else if (input == 'b' && foobar == 0){ // issue
                state = middle_2;
            }
            else if (input == 'b' && foobar == 1){ // issue
                state = foob;
            }
            else if (foobar == 1){ // can't print when there is a b
                uart_putc('0');
                foobar = 0;
                state = start;
            }
            break;
        case middle:
            input = uart_getc();
            if (input == 'o'){
                state = end;
            }
            else if (input == 'f'){
                state = middle;
            }
            else if (input == 'b'){
                state = middle_2;
            }
            break;
        case middle_2:
            input = uart_getc();
            if (input == 'a'){
                state = end_2;
            }
            else if (input == 'f'){
                state = middle;
            }
            else if (input == 'b'){
                state = middle_2;
            }
            break;
        case end:
            input = uart_getc();
            if (input == 'o'){
                foobar = 1;
                state = start;
            }
            else if (input == 'f'){
                state = middle;
            }
            else if (input == 'b'){
                state = middle_2;
            }
            break;
        case end_2:
            input = uart_getc();
            if (input == 'r' && foobar == 0){
                uart_putc('1');
                state = start;
            }
            else if (input == 'f'){
                state = middle;
            }
            else if (input == 'b'){
                state = middle_2;
            }
            break;
        case foob:
            foobar = 0;
            input = uart_getc();
            if (input == 'a'){
                state = fooba;
            }
            else if (input == 'b'){
                uart_putc('0'); // added
                state = middle_2;
            }
            else if (input == 'f'){// added
                uart_putc('0');
                state = middle;
            }
            else{
                uart_putc('0'); // added
                state = start;
            }
            break;
        case fooba:
            input = uart_getc();
            if (input == 'r'){
                uart_putc('\n');
                state = start;
            }
            else if (input == 'b'){
                uart_putc('0'); // added
                state = middle_2;
            }
            else if (input == 'f'){// added
                uart_putc('0');
                state = middle;
            }
            else{
                uart_putc('0'); // added
                state = start;
            }
            break;
        default:
            state = start;
            break;
        }
    }
} // end main()
// Create history array
// make prints seperate
// b after foo
// write it out