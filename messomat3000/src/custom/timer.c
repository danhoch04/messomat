//
// Created by Daniel Hochegger on 17.01.2023.
//
#include "custom/timer.h"

volatile uint8_t timer1_value = 0;
volatile uint8_t timer1Val = 0;

void timerSetup(){
    TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
    TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
    OCR1A = 15624 ; // Set CTC compare value to 1Hz at 1MHz AVR clock, with a prescaler of 64
    TCCR1B |= ((1 << CS10) | (1 << CS12)); // Start timer at Fcpu/1024
}

void timerWorker(void){
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        timer1Val = timer1_value;
    }
}

ISR(TIMER1_COMPA_vect) {
    timer1_value += 1;
}