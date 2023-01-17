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

#define UART_BAUD_RATE 9600

extern int16_t lightValue;
char buffer[7];
volatile versionControl vc = {sizeof(vc), 1, 23, 12, 1, 0};

int main() {
    lcd_init(LCD_DISP_ON);
    adcSetup();
    buttonSetup();
    timerSetup();
    uartSetup();
    while(1){
        uartWorker();
        adcWorker();
        lcd_gotoxy(0, 0);
        lcd_puts(dtostrf(lightValue, 0, 4, buffer));
        lcd_gotoxy(15,0);
        lcd_puts(dtostrf(vc.TransmitInterval, 0, 4, buffer));
        vc.CRC = crc_8((const unsigned char *)&vc,vc.size-1);
        eeprom_update_block(&vc,0,vc.size);
        buttonWorker();
        timerWorker();
    }
}