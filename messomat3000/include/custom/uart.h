//
// Created by Daniel Hochegger on 17.01.2023.
//
#include "global.h"
#include "custom/eeprom.h"
#include "lib/uart.h"

#define UART_BAUD_RATE 9600

void uart_setup(void);
void uart_worker(void);

void uart_buildFrame(void);
void uart_readUartFrame(void);
void uart_sendFrame(char *data);