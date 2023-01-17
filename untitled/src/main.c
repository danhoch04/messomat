#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "libs/lcd.c"
#include "libs/uart.c"
#include <stdbool.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "custom/uart.h"
#include "custom/adc.h"
#include "custom/timer.h"
#include "custom/button.h"


#define UART_BAUD_RATE 9600

typedef struct _EepromData{
    int interval;
} EepromData;


static bool hasToStart = true;
static bool uartEnabled = true;
static int16_t interval = 1;
static int timerCount = 0;
static EepromData EData;

uint16_t getADCVal();
float convertToVolts(uint16_t adcVal);
void initADC();
void initPorts();
void initTimer();
void startup();
void shutdown();
void setInterval();
void toggleLED();

int main() {
    sei();
    initADC();
    initPorts();
    initTimer();
    lcd_init(LCD_DISP_ON);
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    char buffer[7];
    while (1) {
        uartWorker();
        adcWorker();
        lcd_clrscr();
        lcd_home();
        lcd_puts(dtostrf(convertToVolts(getADCVal()), 0, 4, buffer));
        interrupt_worker();
        timer_worker();
    }
}

void startup() {
    lcd_init(LCD_DISP_ON);
    hasToStart = false;
    lcd_clrscr();
    lcd_home();
    lcd_puts("Hello!");
    _delay_ms(500);
}

void shutdown(){
    hasToStart = true;
    lcd_clrscr();
    lcd_home();
    lcd_puts("Goodbye!");
    _delay_ms(500);
}

void initTimer() {

}

float convertToVolts(uint16_t adcVal) {
    //Rechnet den ACD-Wert in Spannung um
    return ((float) adcVal * 5.0f) / 1024.0f;
}