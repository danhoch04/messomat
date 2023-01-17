#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "custom/uart.h"
#include "custom/adc.h"
#include "custom/timer.h"
#include "custom/button.h"
#include "custom/eeprom.h"
#include "lib/lcd.h"
#include "custom/uart.h"

#define UART_BAUD_RATE 9600

extern int16_t lightValue;
extern bool digitalStatus;
char buffer[7];
volatile versionControl vc = {sizeof(vc), 1, 23, 12, 1, 0};

int main() {
    lcd_init(LCD_DISP_ON);
    adcSetup();
    buttonSetup();
    timerSetup();
    uart_setup();
    eeprom_init();
    while(1){
        uart_worker();
        adcWorker();
        lcd_gotoxy(0, 0);
        lcd_puts(dtostrf(lightValue, 0, 4, buffer));
        lcd_gotoxy(15,0);
        lcd_puts(dtostrf(vc.TransmitInterval, 0, 4, buffer));
        lcd_gotoxy(0,1);
        lcd_puts(digitalStatus ? "0" : "1");
        buttonWorker();
        timerWorker();
        eeprom_save();
    }
}