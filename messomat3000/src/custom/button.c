//
// Created by Daniel Hochegger on 17.01.2023.
//
#include "custom/button.h"
#include "lib/lcd.h"
#include <util/delay.h>


volatile bool hasToTurnOn = true;
volatile bool digitalStatus = false;

static bool buttonOne = true;
static bool buttonTwo = false;
static bool buttonThree = false;

extern versionControl vc;

void buttonSetup(void){
    DDRD|= (1 << PD2); //LED data direction
    PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2)); // Pull-up on PB0, PB1, PB2

    PCICR |= (1 << PCIE0); // PCINT0 enable
    PCMSK0 |= ((1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2)); // ports PB0, PB1, PB2 enable for PCINT
}
void buttonStart(void){
    sei(); // enable global interrupts
}
void buttonStop(void){
    cli(); // disable global interrupts
}
void buttonWorker(void){
    if(buttonOne){
        buttonOnePressed();
    }
    if(buttonTwo){
        button2Pressed();
    }
    if(buttonThree){
        button3Pressed();
    }
}

void buttonOnePressed(void){
    if(hasToTurnOn){
        vc.TransmitInterval = 1;
        lcd_init(LCD_DISP_ON);
        lcd_clrscr();
        lcd_home();
        lcd_puts("Hello!");
        _delay_ms(1000);
        hasToTurnOn = false;
        buttonOne = false;
        return;
    }
    if(!hasToTurnOn){
        vc.TransmitInterval = 0;
        lcd_clrscr();
        lcd_home();
        lcd_puts("Goodbye!");
        _delay_ms(1000);
        lcd_init(LCD_DISP_OFF);
        hasToTurnOn = true;
        buttonOne = false;
        return;
    }

    OCR1A = 15624 * vc.TransmitInterval;

}

void button2Pressed(void){
    if(vc.TransmitInterval == 1){
        vc.TransmitInterval= 2;
    }
    else if(vc.TransmitInterval == 2){
        vc.TransmitInterval = 3;
    }
    else if(vc.TransmitInterval == 3){
        vc.TransmitInterval = 4;
    }
    else {
        vc.TransmitInterval = 1;
    }
    OCR1A = 15624 * vc.TransmitInterval;
    buttonTwo = false;
}
void button3Pressed(void){
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