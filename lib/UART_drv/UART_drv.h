/**
 * @file UART_drv.h
 * @brief UART Driver Header File for AVR Microcontrollers
 *
 * UNBOX robotics task 1
 *
 * This header file defines the interface for a UART driver that allows user code to initialize the UART peripheral,
 * transmit and receive data, and check for available data. The driver is designed for AVR microcontrollers, such as the
 * Atmega328P used in Arduino Uno.
 *
 * The function level comments for documentation are provided in the corresponding source file.
 *
 * @author Aditya
 */
#ifndef UART_DRV_H
#define UART_DRV_H

#ifdef __cplusplus // Handles C++ linkage
extern "C" {
#endif

#include <avr/io.h>
#include <stdint.h>

// Function Prototypes

void UART_init(uint32_t baudRate);

uint8_t UART_availableCount(void);
uint8_t UART_availableBool(void);

void UART_transmitByte(uint8_t data);
void UART_transmitString(const char* text);

uint8_t UART_receiveByte(void);

#ifdef __cplusplus
}
#endif // End of C++ linkage

#endif // UART_DRV_H
