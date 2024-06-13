#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#include "qutyio.h" 

/***
 * Tutorial 06: Introduction
 *
 * In this week's tutorial you will be writing C code to implement
 * a pseudo-random number generator. You will find this useful when
 * you get to implementing Assessment 2.
 *
 */



// Ex 6.0
// 
// Define below a global variable called "state" with the type of 
// 32-bit unsigned integer. Initialise state with a value equal to
// your student number, where each decimal digit in your student 
// number is written as a hex digit in a hexadecimal literal.
//
// e.g. if your student number was n12345678 then you should
// assign the hex value 0x12345678 to "state".
//
// Once you have defined this variable uncomment the first printf()
// statement in main() such that this value is printed via the
// serial interface.

uint32_t state = 0x11275561;

// Write your code for Ex 6.0 above this line.



// Ex 6.1
// 
// Write a function prototype for a function called "next"
// which takes no parameters and returns no result.

void next(void);

// Write your code for Ex 6.1 above this line. 



// Ex 6.2
// 
// Write a function called "print_rand" which returns no result
// and takes, as a single argument, a pointer to an unsigned 32-bit
// integer.
//
// The function should inspect the least significant two bits
// of the integer pointed to by the argument, and depending on
// the value of the bits, execute one of the following function
// calls:
//
//   0b00: putchar('1');
//   0b01: putchar('2');
//   0b10: putchar('3');
//   0b11: putchar('4');
//
// Once you have written this function uncomment the second printf()
// statement in main() such that its operation can be tested.

void print_rand(uint32_t* input) {
    uint32_t input_value = *input;
    switch (input_value & 0b11) { // or 0b00000011, least significant 2 bits
        case 0b00:
            putchar('1');
            break;
        case 0b01:
            putchar('2');
            break;
        case 0b10:
            putchar('3');
            break;
        case 0b11:
            putchar('4');
            break;
    }
};
// (putchar('1') + (*input & 0b11))

// Write your code for Ex 6.2 above this line.



// Ex 6.3
// 
// Complete below the implementation for the function next().
//
// next() should:
//     1) Right shift the bits in "state" by 1 bit position.
//     2) If the bit shifted out of state was set (1), take the XOR
//        of "state" with 0xA8E831E0, and store the result in "state".
//
// Once you have written this function uncomment the third printf()
// statement in main() such that its operation can be tested.

void next(void) {
    uint8_t lsb = (state & 0b1); // least significant bit
    state >>= 1; // bit shift by one
    if (lsb) {
        state ^= 0xA8E831E0;
    }
}

// Write your code for Ex 6.3 above this line. 



// This is main(). It it the entry point for a C programme.
int main(void) {

    serial_init();

    // Uncomment the line below when Ex 6.0 is complete
    printf("Ex 6.0: state = 0x%08lX\n", state);

    // Uncomment the line below when Ex 6.2 is complete
    uint32_t test = 0b1100; printf("Ex 6.2: output = "); print_rand(&test); test >>= 1; print_rand(&test); test >>= 1; print_rand(&test); test >>= 1; print_rand(&test); printf("\n");

    // Uncomment the line below when Ex 6.3 is complete
    next(); printf("Ex 6.3: state = 0x%08lX", state); next(); next(); next(); next(); printf(", state = 0x%08lX\n", state);



    // Ex 6.4
    //
    // The variable "state" together with the function next()
    // you have written above implement a linear-feedback shift
    // register, which can produce a pseudorandom number sequence.
    // We seeded this sequence (i.e. set the initial value) with 
    // your student number at the start of the programme. The tests
    // above have already made a few calls to next() so you should
    // expect the value currently stored in "state" to be different
    // from the initial seed.
    // 
    // The function print_rand() you wrote above can be used to
    // operate on the variable "state" to print a digit in the
    // range 1-4.
    //
    // Write code below to print the next 32 digits (from 1-4)
    // derived from this pseudorandom number sequence using 
    // next() and print_rand(). All the digits should be printed
    // on a single line without spaces. Note: you should call next()
    // to advance "state" prior to printing the first digit.
    // 
    // Tip: You will likely want to use a loop to complete the required
    //      number of repeated function calls.
    //
    // Tip: Recall that the unary operator "&" can be used to get a
    //      reference (pointer) to some data.

    printf("Ex 6.4: ");
    // Write your code for Ex 6.4 below this line. 

    for (uint16_t i = 0; i < 32; i++) {
        next();
        print_rand(&state);
    }

    // Write your code for Ex 6.4 above this line. 
    printf("\n");

// END OF TUTORIAL06 EXERCISES //
// DO NOT EDIT BELOW THIS LINE //
    
    while(1) {
        // Loop forever
    }

} // end main()

