#include "../lib/UART_drv/UART_drv.h"
#include <Arduino.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void setup()
{
    UART_init(9600); // Initialize UART with a baud rate of 9600
    sei(); // enable global interrupts
    UART_transmitString("Hello, UART!\n"); // Transmit a test string over UART
}

void loop()
{
    if (UART_availableBool()) { // Check if data is available in the UART receive buffer
        uint8_t receivedByte = UART_receiveByte(); // Read a byte from the UART receive buffer
        UART_transmitByte(receivedByte); // Echo the received byte back over UART
    }
}
