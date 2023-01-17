//
// Created by danie on 11.01.2023.
//
#include "custom/button.h"
#include "libs/lcd.h"
#include <util/delay.h>


volatile bool hasToTurnOn = false;
bool digitalInput = false;
volatile bool switchInput = false;

static bool buttonOne = true;
static bool buttonTwo = false;
static bool buttonThree = false;

extern versionControl myVersionControl;

void interrupt_setup(void){
    DDRD|= (1 << PD2); //LED data direction
    PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2)); // Pull-up on PB0, PB1, PB2

    PCICR |= (1 << PCIE0); // PCINT0 enable
    PCMSK0 |= ((1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2)); // ports PB0, PB1, PB2 enable for PCINT
}
void interrupt_start(void){
    sei(); // enable global interrupts
}
void interrupt_stop(void){
    cli(); // disable global interrupts
}
void interrupt_worker(void){
    if(buttonOne){
        interrupt_button1_pressed();
    }
    if(buttonTwo){
        interrupt_button2_pressed();
    }
    if(buttonThree){
        interrupt_button3_pressed();
    }
}

void interrupt_button1_pressed(void){
    if(hasToTurnOn){
        myVersionControl.TransmitInterval = 1;
        lcd_init(LCD_DISP_ON);
        lcd_clrscr();
        lcd_home();
        lcd_puts("Hello!");
        _delay_ms(1000);
        hasToTurnOn = false;
    }
    if(!hasToTurnOn){
        lcd_clrscr();
        lcd_home();
        lcd_puts("Goodbye!");
        _delay_ms(1000);
        lcd_init(LCD_DISP_OFF);
        hasToTurnOn = true;
    }

    OCR1A = 15624 * myVersionControl.TransmitInterval;
    buttonOne = false;
}
void interrupt_button2_pressed(void){
    if(myVersionControl.TransmitInterval == 1){
        myVersionControl.TransmitInterval= 2;
    }
    else if(myVersionControl.TransmitInterval == 2){
        myVersionControl.TransmitInterval = 3;
    }
    else if(myVersionControl.TransmitInterval == 3){
        myVersionControl.TransmitInterval = 4;
    }
    else {
        myVersionControl.TransmitInterval = 1;
    }
    OCR1A = 15624 * myVersionControl.TransmitInterval;
    buttonTwo = false;
}
void interrupt_button3_pressed(void){
    PORTD ^= (1 << PD2);
}

ISR(PCINT0_vect) {
    if (!(PINB & (1 << PINB0))) {
        buttonOne = !buttonOne;
        return;
    }
    if (!(PINB & (1 << PINB1))) {
        buttonTwo = !buttonTwo;
        return;
    }
    if (!(PINB & (1 << PINB2))) {
        buttonThree = !buttonThree;
        return;
    }
}