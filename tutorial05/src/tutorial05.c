
// This header file provides register definitions and constants
// for AVR microcontroller. In this case it will provide the
// relevant definitions for the ATtiny1626 microcontroller which
// is used on the QUTy.
#include <avr/io.h>

// This header file provides utility functions for working with
// standard input and ouput, such as the printf() function used
// throughout this programme.
#include <stdio.h>

// This header file provides access to library functions to 
// control the 7-segment display and serial output of the QUTy.
// You can use these functions to help debug your programme.
#include "qutyio.h" 

#include "preamble.h"

// This is main(). It it the entry point for a C programme.
int main(void) {

    // These functions initialise the 7-segment display and
    // serial output on the QUTy for use later in the programme.
    serial_init();
    display_init();

    uint8_t digits;

    // Ex 5.0
    // 
    // We have defined immediately above this exercise a variable
    // called "digits" representing an 8-bit unsigned integer. Write
    // C code below to assign a value equal to the last two digits 
    // of your student number to this variable.
    //
    // e.g. if your student number was n12345678 then you should
    // assign the decimal value 78 to "digits".


    // Write your code for Ex 5.0 above this line.
    // n11275561
    digits = 61;
    printf("Ex 5.0: digits = %02u\n", digits);



    // Ex 5.1
    //
    // With the help of the definitions included by the "#define <avr/io.h"
    // preprocessor directive above, we can also assign values directly
    // to peripheral registers. The register definitions for the ATtiny1626
    // are accessed using a dot notation, e.g. we could write to the PORTB OUT
    // register using the expression: PORTB.OUT = 0;
    //
    // Write C code below to turn on the DS1-DP LED on the QUTy.

    PORTB.OUTSET = PIN5_bm; // Set LED off
    PORTB.DIRSET = PIN5_bm; // Set output
    PORTB.OUTCLR = PIN5_bm; // Set LED on
    // Write your code for Ex 5.1 above this line.
    printf("Ex 5.1: expected = 0, found = %u\n", PORTB_IN & 040);



    // Ex 5.2
    //
    // We have written a block of code below this exercise that is intended
    // to execute the statement "loop += 1" 24 times. Complete the #define 
    // preprocessor directive "CONDITION" below so that the for loop executes
    // the correct number of times.
    
    #define CONDITION i < 24 // Loop for 24 times
    
    // Write your code for Ex 5.2 above this line.
    uint8_t loop = 0;
    for (uint8_t i = 0; CONDITION; i++) {
        loop += 1;
    }
    printf("Ex 5.2: iterations = %u\n", loop);



    // Ex 5.3
    //
    // Write C code below to correctly configure pushbutton S1 by
    // enabling the internal pull-up resistor for the corresponding pin.

    PORTA.PIN4CTRL |= PORT_PULLUPEN_bm; // Configure pin
    // Write your code for Ex 5.2 above this line.
    printf("Ex 5.3: expected = 0xFF, found = 0x%02X\n", PB ^ 0xF7);



    // Ex 5.4
    //
    // Below we haw written a "while" loop that, as written, will execute 
    // forever. Add code to the body of this while loop so that the 
    // programme will exit the loop as soon as pushbutton S1 is pressed.
    //
    // Hint: You might like to review the "if", "continue" and "break" statements.
    
    printf("Ex 5.4: PORTA.IN = 0x%02X. Waiting for pushbutton press...", VPORTA.IN);
    
    while (1) { // Will always evaluate true so will loop forever      
        // Write your code for Ex 5.4 below this line.

        if ((PORTA.IN & PIN4_bm) == 0){
            break;
        }
        
        // Write your code for Ex 5.4 above this line.
    } //end while
    
    printf("DONE! PORTA.IN = 0x%02X\n", VPORTA.IN);

    

    // Ex 5.5
    //
    // Elsewhere in the programme we have defined some signed, 8-bit integers:
    // X and Y. Write C code multiply X by Y and store the result in a variable
    // called "alpha". Write some additional C code to determine if the value
    // of "alpha" is positive or negative. If "alpha" is positive, perform a 
    // bitwise left shift by two positions and store the result in a variable
    // "beta". If result is negative perform an arithmetic right shift by four
    // positions and store the result in "beta". Once these calculations are
    // complete, uncomment the printf(..) statement below.

    int16_t alpha = X * Y;
    int16_t beta;
    if (alpha < 0) {
        beta = alpha >> 4; // Negative
    } else {
        beta = alpha << 2; // Posative
    }
    // Write your code for Ex 5.5 above this line. Uncomment the line below when complete.
    printf("Ex 5.5: X = %d, Y = %d, alpha = %d, beta = %d\n", X, Y, alpha, beta);

// END OF TUTORIAL05 EXERCISES //
// DO NOT EDIT BELOW THIS LINE //
    
    while(1) {
        // Loop forever
    }

} // end main()