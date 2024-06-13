#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#include "simon_init.h"

// Defines the Minimum, Maximum Delay amount in milliseconds and Student ID
#define MIN_DELAY_MS 250
#define MAX_DELAY_MS 2000
#define STUDENT_ID 0x11275561

// Calculating 7 seg display 2 hex values
// 10111110(0xBE) S1 lhs
// 11101011(0xEB) S2 lhs
// 00111110(0x3E) S3 rhs
// 01101011(0x6B) S3 rhs
// 10101010(0xAA) LEFT SUCCESS
// 00101010(0x2A) RIGHT SUCcESS

// Storing all possible display states and frequencies into array's
volatile uint8_t simon_display [] = {0xBE, 0xEB, 0x3E, 0x6B, 0xAA, 0x2A};
uint16_t simon_freq [] = {345, 290, 461, 173};

// Setting the MASK and STATE shift value for later use
uint32_t MASK = 0xE2023CAB;
uint32_t STATE_LFSR = STUDENT_ID;
uint32_t STATE_HISTORY; // Stores the history of the state throughout the program
uint8_t STEP; // Stores the individual Steps produced by the sequence generator

uint8_t sequence_length = 1; // Length of sequence

// Sets the current state of the buzzer to off
uint16_t per_value = 0;
uint16_t cmp_value = 0;

uint32_t static counter = 0; // The counter that is increased by one each clock cycle until it reaches the set playback delay
uint8_t stepContinue = 0; // Determines if simon can continue to the playing the next step of the sequence
uint32_t playback_delay; // The delay or duration between each step in the sequence

// The 3 possible states when playing the sequence
enum step_modes{
    GENERATION,
    DETECTION,
    CONTINUE
};

// The 3 possible states when running the buzzer through the ISR
enum buzzer_modes{
    BUZZER_OFF,
    BUZZER_ON,
    BUZZER_SUCCESS
};
enum buzzer_modes buzzer_state = BUZZER_OFF; // Sets buzzer to off

// The 6 possible states the simon can be in in terms of sound and display
enum simon_modes{
    SIMON_OFF,
    SIMON_1,
    SIMON_2,
    SIMON_3,
    SIMON_4,
    SIMON_SUCCESS
};
enum simon_modes simon_state = SIMON_OFF; // Sets simon display and buzzer to off

// The 6 possible states simon in terms of gameplay, can be in
enum game_modes{
    SEQUENCE,
    USER_INPUT,
    WAIT_PLAYBACK,
    CHECK_INPUT,
    NEW_GAME,
    ADD_SEQUENCE
};
enum game_modes game_state = SEQUENCE; // Sets simon to generate the current sequence

// Generates the next step of the by taking in the current state, following the algorithm and setting it to STEP
void stepGeneration(uint32_t* STATE, uint8_t* STEP){
    uint8_t BIT; // Stores the bit of the state
    BIT = *STATE & 0x01; // Extract the least significant bit
    *STATE = *STATE >> 1; // Right shift the value pointed to by STATE by 1 bit
    // If bit is equal to 1, perform a bitwise XOR operation between the value pointed to by STATE and MASK
    if (BIT == 1) {
        *STATE = *STATE ^ MASK;
    }
    *STEP = *STATE & 0b11; // Extract the two least significant bits
}

// Play the current sequence, gets the length and plays X amount of steps depending on the length
void nextStep(uint8_t sequence_length){
    uint32_t STATE = STATE_LFSR; // Sets the state to the current state (on a first game it would be student ID)
    enum step_modes step_state = GENERATION;
    uint8_t sequence_step = 0;
    while (sequence_step < sequence_length){
        switch (step_state)
        {
            case(GENERATION): // Generate the next step in the sequence
                stepGeneration(&STATE, &STEP);
                stepContinue = 1;
                step_state = DETECTION;
                break;
            case(DETECTION): // Detect and play the assigned tune and 7 seg configuration for the step
                if (STEP == 00){
                    simon_state = SIMON_1;
                    gameplayCases();
                }
                else if (STEP == 01){
                    simon_state = SIMON_2;
                    gameplayCases();
                }
                else if (STEP == 02){
                    simon_state = SIMON_3;
                    gameplayCases();
                }
                else if (STEP == 03){
                    simon_state = SIMON_4;
                    gameplayCases();
                }
                step_state = CONTINUE;
                break;
            case(CONTINUE): // Wait for the playback to delay to complete its cycle
                if (stepContinue == 0) {
                    sequence_step += 1;
                    step_state = GENERATION;
                }
                else{
                    step_state = CONTINUE;
                }
                STATE_HISTORY = STATE;
                break;
            default:
                step_state = GENERATION;
                break;
            }
    }
}

// Compare input by regenerating the latest step from a given sequence length
void checkInput(uint8_t length, uint32_t STATE){
    for (uint8_t sequence_step = 0; sequence_step < length; sequence_step++){
        stepGeneration(&STATE, &STEP); // Uses stepGeneration again to generate the Step
    }
}

// Frequency Calculator, calculates the tone (CMP and PER) from a inputed frequency value
void freqCalculate(uint16_t freq, uint16_t* per_value, uint16_t* cmp_value){
    float f_freq = freq;
    float f_time = 300 * 0.000000001f;
    float f_result = f_freq * f_time;
    *per_value = 1.0f / f_result;
    *cmp_value = *per_value;
}

// Generates the playback delay from an inputted potentiometer value
void delayPlayback(uint8_t potValue) {
    float delayFactor = (float)potValue / 255.0;
    playback_delay = (uint16_t)(MIN_DELAY_MS + delayFactor * (MAX_DELAY_MS - MIN_DELAY_MS));
}

// All possible states simon can be in for tone and 7 seg display
void gameplayCases(void){
    switch (simon_state)
    {
    case (SIMON_OFF): // Simon is set to off, buzzer and 7 seg display is off
        spi_write(0xFF);
        counter = 0; // Resset the counter for the playback delay
        buzzer_state = BUZZER_OFF;
        break;
    case (SIMON_1): // Simon plays the first 7 seg display and frequency option in the previously set arrays
        //E(High) = 345
        spi_write(simon_display[0]);
        freqCalculate(simon_freq[0], &per_value, &cmp_value); // PER AND CMP values are changed based on the results of freqCalculate
        counter = 0;
        buzzer_state = BUZZER_ON;
        break;
    case (SIMON_2): // Simon plays the second 7 seg display and frequency option in the previously set arrays
        //C# = 290
        spi_write(simon_display[1]);
        freqCalculate(simon_freq[1], &per_value, &cmp_value);
        counter = 0;
        buzzer_state = BUZZER_ON;
        break;
    case (SIMON_3): // Simon plays the third 7 seg display and frequency option in the previously set arrays
        //A = 461
        spi_write(simon_display[2]);
        freqCalculate(simon_freq[2], &per_value, &cmp_value);
        counter = 0;
        buzzer_state = BUZZER_ON;
        break;
    case (SIMON_4): // Simon plays the fourth 7 seg display and frequency option in the previously set arrays
        //E(Low) = 173
        spi_write(simon_display[3]);
        freqCalculate(simon_freq[3], &per_value, &cmp_value);
        counter = 0;
        buzzer_state = BUZZER_ON;
        break;
    case (SIMON_SUCCESS): // Simon plays no tone but (attempts to) light up succsess pattern
        buzzer_state = BUZZER_SUCCESS;
        counter = 0;
        break;
    default:
        simon_state = SIMON_OFF;
        break;
    }
}

// Main looping function of the whole program, runs all other functions and initiates all ISR's
int main(void) {

    // Setting everything up
    adc_init();
    uart_init();
    spi_init();
    pwm_init();
    buttons_init();

    // Stating variables required for the push buttons
    uint8_t input_step;
    uint8_t pb_sample = 0xFF;
    uint8_t pb_sample_r = 0xFF;
    uint8_t pb_changed, pb_rising, pb_falling;
    
    while (1) {
        timer_init(); // Starts running a timer
        
        pb_sample_r = pb_sample; // Keeps track of the previous state of the input signal
        pb_sample = PORTA.IN; // Retrieves the current sample of the digital input signal
        pb_changed = pb_sample ^ pb_sample_r; // Detects the bits that have changed between the current and previous samples

        pb_falling = pb_changed & pb_sample_r; // identifies the bits that have changed from high to low for when the button is pressed
        pb_rising = pb_changed & pb_sample; // identifies the bits that have changed from high to low for when the button is released

        // All possible states simon can be in gameplay wise
        switch (game_state)
        {
        case(SEQUENCE): // Generates the sequence for the user to follow and adds to the score (sequence_length)
            nextStep(sequence_length);
            sequence_length += 1;
            stepContinue = 1;
            game_state = USER_INPUT;
            uint8_t input_length = 1;
            uint8_t user_sequence = 0;
            break;
        case(USER_INPUT): // awaits for user input and produces the corrisponding tone and 7 seg display based on the user's input
            if (pb_falling & PIN4_bm) { // Button 0
                simon_state = SIMON_1; // Produce correct tone and display
                game_state = WAIT_PLAYBACK; // Next state
                input_step = 00; // Users inputted step is set to the assigned button
                gameplayCases(); // Goes to the gameplayCases function and runs the case statement
            }
            else if (pb_falling & PIN5_bm) { // Button 1
                simon_state = SIMON_2;
                game_state = WAIT_PLAYBACK;
                input_step = 01;
                gameplayCases();
            }
            else if (pb_falling & PIN6_bm) { // Button 2
                simon_state = SIMON_3;
                game_state = WAIT_PLAYBACK;
                input_step = 02;
                gameplayCases();
            }
            else if (pb_falling & PIN7_bm) { // Button 3
                simon_state = SIMON_4;
                game_state = WAIT_PLAYBACK;
                input_step = 03;
                gameplayCases();
            }
            break;
        case(WAIT_PLAYBACK): // Awaits for the user's inputed tone to finish playing for the set playback delay
            if (stepContinue == 0){
                game_state = CHECK_INPUT;
            }
            break;
        case(CHECK_INPUT): // Checks the user's input and compares it to the current STEP of the sequence as the user performs the sequence
            checkInput(input_length, STATE_LFSR); // Set Step to the current Step the user is attempting to guess
            // STEP is detected as incorrect at anypoint in the sequence user_sequence is set to 1 (User is wrong)
            if (input_step != STEP){
                user_sequence = 1;
            }
            game_state = USER_INPUT; // Return the game state to colecting user input
            // If the user has reached the end of the sequence check if the sequence is wrong (is user_sequence 1)
            if (input_length == (sequence_length - 1)){
                if (user_sequence == 1){
                    game_state = NEW_GAME; // Create a new game
                }
                else if(user_sequence == 0){
                    game_state = ADD_SEQUENCE; // Add to the sequence
                    simon_state = SIMON_SUCCESS; // Display succsess pattern (in theory)
                    gameplayCases();
                }
            }
            input_length += 1; //Increase length/ score
            break;
        case(NEW_GAME): // Create a new game, reseting everything expect sequence
            sequence_length = 1; // Reset length/ score
            STATE_LFSR = STATE_HISTORY; // Set the State to the STATE_HISTORY so it continue from the next step in the sequence
            game_state = SEQUENCE; // Play the new sequence
            break;
        case(ADD_SEQUENCE): // Await the success pattern to finishing playing then add to sequence
            if (stepContinue == 0){
                game_state = SEQUENCE;
            }
            break;
        default:
            game_state = SEQUENCE;
            break;
        }
    }
}

// Input simon button via uart and controll buzzer octave
ISR(USART0_RXC_vect) {
    uint8_t freq_range = 0;
    char rx = USART0.RXDATAL; // Read the UART
    switch (rx) {
        // Button 0
        case '1':
        case 'q':
            simon_state = SIMON_1;
            game_state = WAIT_PLAYBACK;
            gameplayCases();
            break;
        // Button 1
        case '2':
        case 'w':
            simon_state = SIMON_2;
            game_state = WAIT_PLAYBACK;
            gameplayCases();
            break;
        // Button 2
        case '3':
        case 'e':
            simon_state = SIMON_3;
            game_state = WAIT_PLAYBACK;
            gameplayCases();
            break;
        // Button 3
        case '4':
        case 'r':
            simon_state = SIMON_4;
            game_state = WAIT_PLAYBACK;
            gameplayCases();
            break;
        // Increase octave
        case ',':
        case 'k':
            // Check if the frequency won't exeed the set limit
            for (int freq = 0; freq < 4; freq++){
                if ((simon_freq[freq] * 2) > 20000){
                    freq_range = 1;
                }
            }
            // Increase by one octave if acceptable
            if (freq_range == 0){
                for (int freq = 0; freq < 4; freq++){
                    simon_freq[freq] <<= 1;
                }
            }
            freq_range = 0;
            break;
        // Decrease octave
        case '.':
        case 'l':
            // Check if the frequency won't go bellow the minimum
            for (int freq = 0; freq < 4; freq++){
                if ((simon_freq[freq] / 2) < 20){
                    freq_range = 1;
                }
            }
            // Decrease by one octave if acceptable
            if (freq_range == 0){
                for (int freq = 0; freq < 4; freq++){
                    simon_freq[freq] >>= 1;
                }
            }
            freq_range = 0;
            break;
        default:
            game_state = SEQUENCE;
            break;
    }
}

// Manages the buzzer for simon and the success state
uint8_t displayFlag = 0;
ISR(TCB0_INT_vect) {
    delayPlayback(ADC0.RESULT); // Input the potentiometer into the delayPlayback function
    counter++; // Each cycle increase the counter
    // Determines the state of the buzzer, off and on
    switch (buzzer_state) {
    case (BUZZER_OFF): // Sets the buzzer to off, no sound is played
        TCA0.SINGLE.CMP0BUF = per_value;
        if (counter > playback_delay){
            counter = 0;
            buzzer_state = BUZZER_OFF;
            stepContinue = 0;
        }
        else{
            stepContinue = 1;
        }
        break;
    case (BUZZER_ON): // Sets the buzzer to on, the set per_value is set to the buzzer's tone
        if (counter > (playback_delay * 0.5)) { // Buzzer will turn off after half of the playback delay
            counter = 0;
            simon_state = SIMON_OFF;
            gameplayCases();
        }
        else{
            TCA0.SINGLE.PERBUF = per_value;
            TCA0.SINGLE.CMP0BUF = per_value >> 1;
        }
        break;
    case (BUZZER_SUCCESS): // Display the success pattern on the 7 seg display
        if (counter > (playback_delay * 0.5)) {
            counter = 0;
            simon_state = SIMON_OFF;
            gameplayCases();
        }
        else{
            // Constantly switches to both sides of the 7 seg to display them on both sides
            switch (displayFlag) {
            case 0:
                spi_write(simon_display[4]);
                displayFlag = 1;
                break;
            case 1:
                spi_write(simon_display[5]);
                displayFlag = 0;
                break;
            default:
                break;
            }
        }
        break;
    default:
        buzzer_state = BUZZER_OFF;
        break;
    }
    TCB0.INTFLAGS = TCB_CAPT_bm;
}