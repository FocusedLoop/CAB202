// This directive indicates that the subsequent code should be placed at
// the top of programme memory, that is, beginning at offset 0 (where the 
// Programme Counter (PC) points to on microcontroller reset)
.section .init0

// This is a "label". It does not generate any programme code,
// but marks this location in the programme.
entry:       

    // Ex E1.0
    // 
    // The nop (no operation) instruction takes one CPU cycle to execute, 
    // and peforms no operation. Refer to the AVR Instruction Set Manual
    // pp. 102-103. 
    // 
    // QUESTION: What is the opcode that the assembler translates the 
    // nop instruction into and places into programme memory?
    // Enter your answer on the line below, replacing 0b1111111111111111
    // with "0b" followed by the 16 binary digits (0 or 1) that 
    // compose the opcode.
    //
    // Note ".set" is an assembler directive. It does not generate any
    // programme code.

    .set OPCODE_NOP, 0b0000000000000000
    
    ldi r16, lo8(OPCODE_NOP)
    call avremu_r16out
    ldi r16, hi8(OPCODE_NOP)
    call avremu_r16out


    // Ex E1.1
    // 
    // The wdr (watchdog reset) instruction takes one CPU cycle to execute, 
    // and resets the watchdog timer (in this programme this instruction)
    // will have no effect as we have not configured the watchdog timer).
    // 
    // QUESTION: What is the opcode that the assembler translates the 
    // wdr instruction into and places into programme memory?
    // Enter your answer on the line below, replacing 0b1111111111111111
    // with "0b" followed by the 16 binary digits (0 or 1) that 
    // compose the opcode.
    //
    // Note ".set" is an assembler directive. It does not generate any
    // programme code.

    .set OPCODE_WDR, 0b1001010110101000

    ldi r16, lo8(OPCODE_WDR)
    call avremu_r16out
    ldi r16, hi8(OPCODE_WDR)
    call avremu_r16out
 
 
    // Ex E1.2
    // 
    // The ldi (load immediate) instruction loads an immediate value
    // into one of the working registers R16-R31. 
    //
    // The mov (copy register) instruction copies the value from one
    // working register into another.
    //
    // Refer to the AVR Instruction Set Manual for the syntax of these 
    // instructions.
    // 
    // (exercise continues below...)

    ldi r16, 130
    ldi r17, 44
    mov r17, r16
    mov r16, r17
    ldi r17, 145
    ldi r17, 229

    // (Ex E1.2 cont.)
    //
    // QUESTION: After the preceding block of instructions executes, 
    // what is the value contained in R16? Replace "0" in the line 
    // below with your answer.

    ldi r16, 130
	
    call avremu_r16out


    // Ex E1.3
    //
    // QUESTION: What is the value currently stored in R17? Replace "0" 
    // in the line below with your answer.

    ldi r16, 229
	
    call avremu_r16out


    // Ex E1.4
    // 
    // The Programme Counter (PC) points the the location in programme
    // memory of the next instruction to be executed. The PC is set to
    // 0 on microcontroller reset and advances/changes as the programme
    // executes.
    //
    // QUESTION: What is the value of the programme counter at this 
    // point in the programme? Replace "0" in the line below with your 
    // answer.

    ldi r16, 24
	
    call avremu_r16out


    // Ex E1.5
    // 
    // The push instruction stores the value in one of the working
    // registers onto the top of stack.
    //
    // The pop instruction loads the value from the top of the stack
    // into one of the working registers.
    //
    // Refer to the AVR Instruction Set Manual for the syntax of these 
    // instructions.
    //
    // On initialisation the height of the stack is 0, and the stack
    // height grows by one for each pop instruction, and shrinks by one
    // for each pop instruction.
    //
    // (exercise continues below...)

    ldi r16, 1
    ldi r17, 2
    ldi r18, 3
    ldi r19, 4
    ldi r20, 5
    ldi r21, 6
    push r16 
    push r17
    push r18
    push r19
    push r20
    push r21
    push r17
    pop r16
    pop r19
    push r16
    push r17
    pop r16

    // (Ex E1.5 cont.)
    //
    // QUESTION: After the preceding block of instructions executes, 
    // what is the current height of the stack? Replace "0" in the line 
    // below with your answer.

    ldi r16, 6

    call avremu_r16out



    // Ex E1.6
    // 
    // QUESTION: After the final pop instruction, what is the value stored in R16? 
    // Replace "0" in the line below with your answer.

    ldi r16, 2

    call avremu_r16out



    // Ex E1.7
    // 
    // QUESTION: What is the value currently stored on the top of the 
    // stack? Replace "0" in the line below with your answer.

    ldi r16, 2

    call avremu_r16out



    // Ex E1.8
    // 
    // Some instructions take more than one CPU cycle to execute.
    // You can determine this from the AVR Instruction Set Manual
    // (TIP: recall that we are working with a microcontroller that 
    // uses an AVRxt core).
    //
    // (exercise continues below...)
    
    ldi r16, 0  // Clear register r16 
    ldi r17, 1
    sbrc r16, 0
    jmp 0

    // (Ex E1.8 cont.)
    //
    // QUESTION: How many CPU cycles did the first three instructions
    // in the preceding block take to execute (noting that the 4th
    // instruction is skipped by the programme). Replace "0" in the 
    // line below with your answer.

    ldi r16, 5

    call avremu_r16out



// END OF EXTENSION01 EXERCISES //
// DO NOT EDIT BELOW THIS LINE //

    ldi r16, 0x22           // DISP_EN (PB1) | DISP_DP (PB5)
    sts 0x0420, r16         // Set as outputs
    sts 0x0424, r16         // Drive high

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
    sts 0x0427, r16     
    
    // Loop
    rjmp loop

// This procedure outputs the contents of r16 when using the emulator.
// Address 0x1500 is reserved in the memory map so this procedure has 
// no effect when running on the ATtiny1626 hardware.
avremu_r16out:
    sts 0x1500, r16
    ret