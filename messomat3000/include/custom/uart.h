//
// Created by Daniel Hochegger on 17.01.2023.
//
#include "global.h"
#include "lib/lcd.h"
#include "lib/uart.h"

#define UART_BAUD_RATE 9600

void uartSetup(void);
void uartWorker(void);

void uartBuildFrame(void);
void uartReadFrame(void);
void uartSendFrame(char *data);

