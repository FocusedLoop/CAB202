.section .init0 // Places code at the top of programme memory

entry:
    
    // Ex 1.0
    // 
    // Uncomment the line below and replace <Ex 1.0> with the first 
    // (non-zero) digit in your student number.

    ldi r16, 1
    call avremu_r16out

    // Ex 1.1
    // 
    // Uncomment the line below and replace <Ex 1.1> with the second 
    // digit in your student number.

    ldi r16, 1
    call avremu_r16out

    // Ex 1.2
    // 
    // Uncomment the line below and replace <Ex 1.2> with the third 
    // digit in your student number.

    ldi r16, 2
    call avremu_r16out

    // Ex 1.3
    // 
    // Uncomment the line below and replace <Ex 1.3> with the fourth 
    // digit in your student number.

    ldi r16, 7
    call avremu_r16out

    // Ex 1.4
    //
    // Find the base address for the PORTB peripheral in the datasheet (see p. 62).
    // Uncomment the line below and relpace <Ex 1.4> with the corresponding address.

    .set PORTB, 0x0420
    ldi r16, lo8(PORTB) // lo8 takes the lower bits
    call avremu_r16out
    ldi r16, hi8(PORTB) // hi8 takes the higher bits
    call avremu_r16out

    // Ex 1.5
    //
    // Find the offset for the PORTB_DIR register in the datasheet (see p. 153).
    // Uncomment the line below and relpace <Ex 1.5> with the corresponding offset.

    .set DIR, 0x00
    ldi r16, DIR
    call avremu_r16out

    // Ex 1.6
    //
    // Find the offset for the PORTB_OUT register in the datasheet.
    // Uncomment the line below and relpace <Ex 1.6> with the corresponding offset.

    .set OUT, 0x04
    ldi r16, OUT
    call avremu_r16out

    // Ex 1.7
    //
    // Find the offset for the PORTB_OUTTGL register in the datasheet.
    // Uncomment the line below and relpace <Ex 1.7> with the corresponding offset.

    .set OUTTGL, 0x07
    ldi r16, OUTTGL
    call avremu_r16out

// END OF TUTORIAL01 EXERCISES //
// DO NOT EDIT BELOW THIS LINE //

    ldi r16, 0x22           // DISP_EN (PB1) | DISP_DP (PB5)
    sts (PORTB+DIR), r16    // Set as outputs
    sts (PORTB+OUT), r16    // Drive high

    break   // Terminate here for autograder

    ldi r16, 0x20 // DISP_DP (PB5)
    ldi r17, 0x00

loop:
    // Delay
    com r17
    brne loop
    inc r18
    brne loop
    inc r19
    brne loop

    // Toggle DP LED
    sts (PORTB+OUTTGL), r16     
    
    // Loop
    rjmp loop

// This procedure outputs the contents of r16 when using the emulator.
// Address 0x1500 is reserved in the memory map so this procedure has 
// no effect when running on the ATtiny1626 hardware.
avremu_r16out:
    sts 0x1500, r16
    ret