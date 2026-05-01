/**
 * @file GPIO_drv.h
 * @brief GPIO Driver Header File for AVR Microcontrollers
 *
 * UNBOX robotics task 1
 *
 * This header file defines the interface for a GPIO driver that allows
 * users to configure pin directions, write to pins, toggle pins, and read
 * pin states on AVR microcontrollers.
 *
 * The function level comments for documentation are provided in the corresponding source file.
 *
 * @author Aditya
 */
#ifndef GPIO_DRV_H
#define GPIO_DRV_H

// Handles C++ linkage
#ifdef __cplusplus
extern "C" {
#endif

#include <avr/io.h>
#include <stdint.h>

// Pin Direction Enum Macros

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1
#define GPIO_INPUT_PULLUP 2

// Pin State Enum Macros

#define PIN_LOW 0
#define PIN_HIGH 1

// Function Prototypes

void GPIO_setPinDirection(uint8_t pin, uint8_t direction);
void GPIO_writePin(uint8_t pin, uint8_t value);
void GPIO_togglePin(uint8_t pin);
uint8_t GPIO_readPin(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif
