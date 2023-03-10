//
// Created by Daniel Hochegger on 17.01.2023.
//
#include "custom/uart.h"

#define UART_START "\02"
#define UART_END "\03"
#define UART_ACK "\06"
#define UART_NACK "\21"


char uart_mode = '\0';

char uart_storage[8] = "";
volatile int uart_storage_index = 0;

extern int16_t lightValue;
extern bool digitalStatus;
extern uint8_t timer1Val;
uint8_t last_timer_val = 0;
extern versionControl vc;

void uart_setup()
{
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
}

void uart_worker(void)
{
    uart_readUartFrame();

    if (((timer1Val - last_timer_val) >= vc.TransmitInterval) && (vc.TransmitInterval != 0))
    {
        uart_buildFrame();

        last_timer_val = timer1Val;
    }
}

uint8_t uart_storage_length()
{
    int length = 0;
    while (uart_storage[length] != '\0')
    {
        length++;
    }
    return length;
}

void uart_buildFrame()
{
    if (uart_mode == 'a')
    {
        char data[128] = "";
        strcat(data, "A:");
        strcat(data, "LV:");
        char uart_a_string[128];
        strcat(data, utoa(lightValue, uart_a_string, 10));
        strcat(data, ";");

        strcat(data, "BV:");
        strcat(data, digitalStatus ? "on" : "off");

        /* INTERVALL SENDEN
         * strcat(data, "intervall:");
        char uart_b_string[128];
        strcat(data, utoa(vc.TransmitInterval, uart_b_string, 10));
         */

        uart_sendFrame(data);
    }
    else if (uart_mode == 'l')
    {
        char data[128] = "";
        strcat(data, "LV:");
        char uart_a_string[128];
        strcat(data, utoa(lightValue, uart_a_string, 10));

        uart_sendFrame(data);
    }
    else if (uart_mode == 'b')
    {
        char data[128] = "";

        strcat(data, "BV:");
        strcat(data, digitalStatus ? "on" : "off");

        uart_sendFrame(data);
    }
}

void uart_readUartFrame()
{
    int uartValue = uart_getc();
    uart_putc(uartValue);

    if (uartValue == UART_NO_DATA)
        return;

    if (uartValue == '<') // Plathalter für STX, weil mit Putty nicht sendbar
    {
        for (uint8_t i = 0; i < sizeof(uart_storage); i++)
        {
            uart_storage[i] = '\0';
            uart_storage_index = 0;
        }
        return;
    }

    if (uartValue == '>') // Plathalter für ETX, weil mit Putty nicht sendbar
    {
        if (uart_storage_length() == 1)
        {
            if (uart_storage[0] == 'a')
            {
                uart_mode = 'a';
                uart_sendFrame(UART_ACK);
            }
            else if (uart_storage[0] == 'l')
            {
                uart_mode = 'l';
                uart_sendFrame(UART_ACK);
            }
            else if (uart_storage[0] == 'b')
            {
                uart_mode = 'b';
                uart_sendFrame(UART_ACK);
            }
            else if (uart_storage[0] == 's')
            {
                uart_mode = 's';
                uart_sendFrame(UART_ACK);
            }
            else{
                uart_mode = 'F';
                uart_sendFrame(UART_NACK);
            }
        }
        else if (uart_storage_length() == 3)
        {
            if (uart_storage[0] == 't' && uart_storage[1] == 'r')
            {
                // if uart_storage[2] is between 0 and 4
                switch (uart_storage[2])
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                        vc.TransmitInterval = uart_storage[2] - '0';
                        eeprom_save();
                        break;
                }
            }
        }
        return;
    }

    uart_storage[uart_storage_index] = uartValue;
    uart_storage_index++;
}

void uart_sendFrame(char *data)
{
    char frame[128] = "";
    strcat(frame, UART_START);
    strcat(frame, data);
    strcat(frame, UART_END);

    uart_puts(frame);
    // uart_putc(13);
}