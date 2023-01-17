//
// Created by Daniel Hochegger on 17.01.2023.
//
#include "custom/uart.h"

#define UART_START "<"
#define UART_END ">"

volatile bool sendAllMeasurements = false;
volatile bool sendLightValue = false;
volatile bool sendDigitalStatus = false;

char storage[128] = "";
volatile int storageIndex = 0;

extern int16_t lightValue;
extern bool digitalStatus;
extern uint8_t timer1Val;
extern versionControl vc;

void uartSetup() {
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
}

void uartWorker(void) {
    uartReadFrame();

    if (timer1Val == vc.TransmitInterval && vc.TransmitInterval != 0) {
        uartBuildFrame();
        timer1Val = 0;
    }
}

void uartBuildFrame(void) {
    char sendData[128] = "";
    if (sendAllMeasurements) {
        strcat(sendData, "lightLevel: ");
        char measurementsString[128];
        strcat(sendData, utoa(lightValue, measurementsString, 10));
        strcat(sendData, ", ");

        strcat(sendData, "button: ");
        strcat(sendData, digitalStatus ? "on" : "off");
    }

    if (sendLightValue) {
        strcat(sendData, "lightLevel: ");
        char measurementsString[128];
        strcat(sendData, utoa(lightValue, measurementsString, 10));
        strcat(sendData, ", ");
    }
    if (sendDigitalStatus) {
        strcat(sendData, "button: ");
        strcat(sendData, digitalStatus ? "on" : "off");
    }
    uartSendFrame(sendData);
}

void uartReadFrame(void) {
    char uartValue = uart_getc();
    uart_putc(uartValue);

    if (uartValue == UART_NO_DATA || uartValue == '\0')
        return;

    if (uartValue == UART_START[0]) {
        storageIndex = 0;
        for (int i = 0; i < 128; i++)
            storage[i] = '\0';
        return;
    }

    if (uartValue == UART_END[0]) {
        if (strcmp(storage, "a") == 0) {
            sendAllMeasurements = true;
            sendLightValue = false;
            sendDigitalStatus = false;
        } else if (strcmp(storage, "t") == 0) {
            sendAllMeasurements = false;
            sendLightValue = true;
            sendDigitalStatus = false;
        } else if (strcmp(storage, "s") == 0) {
            sendAllMeasurements = false;
            sendLightValue = false;
            sendDigitalStatus = true;
        } else if (strcmp(storage, "q") == 0) {
            sendAllMeasurements = false;
            sendLightValue = false;
            sendDigitalStatus = false;
        }
        return;
    }

    storage[storageIndex] = uartValue;
    storageIndex++;
}

void uartSendFrame(char *data) {
    char frame[128] = "";
    strcat(frame, UART_START);
    strcat(frame, data);
    strcat(frame, UART_END);
    uart_puts(frame);
}