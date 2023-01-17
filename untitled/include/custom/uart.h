#include "custom/global.h"
#include "libs/lcd.h"
#include "libs/uart.h"

#define UART_BAUD_RATE 9600

void uartSetup(void);
void uartWorker(void);

void uartBuildFrame(void);
void uartReadFrame(void);
void uartSendFrame(char *data);
