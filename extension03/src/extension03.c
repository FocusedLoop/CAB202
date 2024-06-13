#include <avr/io.h>
#include <stdio.h>
#include "qutyio.h"

// Ex E3.0
//
// In this week's tutorial we used some functions from a library
// "qutyio.h" and from the <stdio.h> library to output data via 
// the serial interface. This can be a useful tool for debugging
// your programme. To use the serial interface you first need to
// initialise the required hardware by calling "serial_init()";
// this only needs to be done once at the beginning of your 
// programme. After this, you can use the function printf() from
// <stdio.h> to output formatted strings via the serial interface.
//
// You will need to use printf() in few different ways in this
// extension exercise:
//
//   printf("foo") - prints the word 'foo' to the serial output.
//   printf("bar") - prints the word 'bar' to the serial output.
//   printf(" ")   - prints a space to the serial output.
//   printf("%02X", x) - prints the 8-bit integer 'x' to the 
//   serial output, formatted as two hexadecimal digits.
//   printf("\n")  - prints a new line to the serial output.
//
// Your task is to write C code to do the following:
//
// 1) Include stdio.h and qutyio.h so that you can access the 
//    functions required to write to the serial interface.
// 2) Initialise the qutyio serial inteface by calling serial_init(). 
// 3) Create a variable "state" to store your student number. You 
//    should interpret your student number as a decimal number. Use 
//    the smallest standard unsigned integer type in which your student
//    number will fit. (you will need to include the stdint header).
//    e.g. the student number 10000012 would represent the number 
//    ten million and twelve. 
// 4) Iterate through all the numbers from 0 to 255 in sequence.
//    For each number in the sequence perform the following steps:
//    a) Take the bitwise xor of the number with the variable "state",
//       storing the result back into "state".
//    b) Rotate right the bits in "state" at least one time, and until 
//       the LSB of "state" is a zero. If there are no cleared bits in 
//       "state" do nothing. 
//    c) Print the least significant two bytes of "state" to the serial 
//       output as the four hexadecimal digits and a space. No prefix is 
//       required.
//    d) Inspect the bits 11-4 of "state" (where bit 0 is the LSB). If the 
//       most significant nibble of this byte, represented as a hexadecimal 
//       digit, matches the second last digit of your student number, 
//       represented decimal digit, print the word "foo" to the serial
//       output. If the least significant nibble of this byte, 
//       represented as a hexadecimal digit, matches the final digit 
//       of your student number, represented decimal digit, print the 
//       word "bar" to the serial output. If both match, print "foobar".
//    e) Print a newline character to the serial output.
// 5) Based on step 4 above, your programme should have printed 256
//    lines to the serial output. After completion of step 4 programme
//    execution should proceed to the infinite loop without producing
//    any further output.
//
// Examples:
//   Assume for all examples below the student number is n12345678
//  
//   Assume that after step 4b "state" holds the value 0x11223344
//     The programme should print the line: 3344 
//
//   Assume that after step 4b "state" holds the value 0x34567728
//     The programme should print the line: 7728 foo   
//
//   Assume that after step 4b "state" holds the value 0x11111780
//     The programme should print the line: 1780 foobar    

int main(void) {
// Write your code for Ex E3.0 below this line.
// You will also need to add some preprocessor directives;
// these would typically go at the top of the file.

serial_init();
uint32_t student_number = 11275561;
uint32_t state = student_number;

for (uint8_t i = 0; i <= 255; i++) {
    state ^= i; // bitwise xor of state
    
    state = (state >> 1) | (state << 31);
    while ((state & 1) != 0) {
        state = (state >> 1) | (state << 31);
    } // rotate right

    printf("%04X ", (state & 0xffff)); // print result

    uint8_t ms_nibble = (state >> 8) & 0xF; // most significant nibble
    uint8_t ls_nibble = (state >> 4) & 0x0F; // least significant nibble
    uint32_t sls_st = (student_number / 10) % 10; // 6
    uint32_t ls_st = student_number & 1; // 1
    
    // Inspecting the states
    if (ms_nibble == sls_st && ls_nibble == ls_st) {
        printf("foobar");
    }
    else if (ms_nibble == sls_st) {
        printf("foo");
    }
    else if (ls_nibble == ls_st) {
        printf("bar");
    }
    if (i == 255) {
        break;
    }
    printf("\n");
}

// END OF EXTENSION03 EXERCISES //
// DO NOT EDIT BELOW THIS LINE //
    
    while(1) {
        // Loop forever
    }

} 

