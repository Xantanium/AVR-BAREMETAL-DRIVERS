/**
 * @file UART_drv.c
 * @brief UART driver implementation for Atmega328P (Arduino Uno)
 *
 * This file implements the UART driver functions for the Atmega328P microcontroller, which is used in the Arduino Uno.
 * The driver provides functions to initialize the UART peripheral, transmit and receive data, and check for available
 * data.
 *
 * @author Aditya
 */
#include "UART_drv.h"
#include <avr/interrupt.h>
#include <avr/io.h>

//////////////////////////////////////////////////////////////////////////////////
/// MACRO CONSTANTS
//////////////////////////////////////////////////////////////////////////////////

#define BUFFER_SIZE 64 // Size of the circular buffer for received data
#define CMD_BUFFER_SIZE 32 // Size of the command buffer for processing received data

//////////////////////////////////////////////////////////////////////////////////
/// MACRO FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////

#define BAUD_TO_UBRR(baud) ((F_CPU / (16UL * (baud))) - 1) // Macro to calculate UBRR value for a given baud rate

//////////////////////////////////////////////////////////////////////////////////
/// Static Data Structures
//////////////////////////////////////////////////////////////////////////////////

static volatile uint8_t rxBuffer[BUFFER_SIZE]; // Circular buffer for received data
static volatile uint8_t rxHead = 0; // Head index for the circular buffer
static volatile uint8_t rxTail = 0; // Tail index for the circular buffer

//////////////////////////////////////////////////////////////////////////////////
/// Public API Functions
//////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes the UART peripheral with the specified baud rate.
 *
 * This function configures the UART registers to set the baud rate, frame format, and enables the transmitter and
 * receiver.
 *
 * @param baudRate The desired baud rate for UART communication (e.g., 9600, 115200).
 */
void UART_init(uint32_t baudRate)
{
    // Configure the ubrr (UART Baud Rate Register) value based on the desired baud rate
    // UBRR is a 16 bit value, so we need to set both the high and low bytes
    uint16_t ubrrVal = BAUD_TO_UBRR(baudRate);
    UBRR0H = (ubrrVal >> 8); // Set the high byte of UBRR
    UBRR0L = ubrrVal; // Set the low byte of UBRR

    // Set frame format: 8 data bits, no parity, 1 stop bit (8N1)
    // UCSRnC : UART Control and Status Register C, set bits 1 and 2 for 8 bit data size.
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // Keep tx simply polling, hence no tx interrupt.
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Enable receiver, transmitter and RX complete interrupt
}

/**
 * @brief Transmits a single byte of data over UART.
 *
 * This function waits until the transmit buffer is empty and then sends the specified byte of data.
 *
 * @param data The byte of data to be transmitted.
 */
void UART_transmitByte(uint8_t data)
{
    // Wait for the transmit buffer to be empty
    // UDRE : UART Data Register Empty Flag
    while (!(UCSR0A & (1 << UDRE0)))
        ;
    // Put the data into the buffer, which sends it
    UDR0 = data;
}

/**
 * @brief Sends a null-terminated string over UART.
 * @param text The string to be transmitted.
 */
void UART_transmitString(const char* text)
{
    while (*text)
        UART_transmitByte((uint8_t)(*text++));
}

/**
 * @brief Recieves a single byte of data from the UART receive buffer.
 *
 * This function uses the circular buffer to get the received data. If the buffer is empty, it returns 0.
 *
 * @return Received data byte
 */
uint8_t UART_receiveByte(void)
{
    if (rxHead == rxTail)
        return 0; // No data available in the buffer

    uint8_t data = rxBuffer[rxTail];
    rxTail = (rxTail + 1) % BUFFER_SIZE; // Circular Wraparound
    return data;
}

/**
 * @brief Checks if there is data available in the UART receive buffer.
 *
 * This function compares the head and tail indices of the circular buffer to determine if there is data available.
 *
 * @return 1 if data is available, 0 otherwise
 */
uint8_t UART_availableBool(void) { return (rxHead != rxTail); }

/**
 * @brief Returns the number of bytes available in the UART receive buffer.
 *
 * This function calculates the number of bytes available by comparing the head and tail indices of the circular
 * buffer.
 *
 * @return Number of bytes available in the receive buffer
 */
uint8_t UART_availableCount(void) { return (rxHead + BUFFER_SIZE - rxTail) % BUFFER_SIZE; }

/**
 * @brief UART Receive Complete Interrupt Service Routine (ISR)
 *
 * This ISR is triggered when a byte of data is received over UART. It reads the received byte from the UDR0 register
 * and stores it in the circular buffer. The head index is updated to point to the next position in the buffer.
 */
ISR(USART_RX_vect)
{
    uint8_t data = UDR0; // Read the received byte from the UART Data Register

    uint8_t nextHead = (rxHead + 1) % BUFFER_SIZE;

    if (nextHead != rxTail) {
        rxBuffer[rxHead] = data; // Store the received byte in the circular buffer
        rxHead = nextHead; // Move the head index to the next position
    }
}
