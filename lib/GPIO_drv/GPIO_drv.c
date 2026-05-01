/**
 * @file GPIO_drv.c
 * @brief GPIO driver implementation for Atmega328P (Arduino Uno)
 *
 * This file implements the GPIO driver functions for the Atmega328P microcontroller,
 * which is used in the Arduino Uno. The driver provides functions to set pin direction,
 * write to pins, toggle pins, and read pin states.
 *
 * @author Aditya
 */
#include "GPIO_drv.h"
#include <avr/interrupt.h>
#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////
/// Static Data Structures
//////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Mapping of Arduino Uno digital pins to Atmega328P ports and pins
 *
 * Serves as a lookup table to translate Arduino pin numbers (0-13) to the corresponding
 */
static const uint8_t pinMapping[14] = {
    0, 1, 2, 3, 4, 5, 6,
    7, // Port D: 0-7
    0, 1, 2, 3, 4, 5 // Port B: 8-13
};

/**
 * @brief Arrays of pointers to the Data Direction Registers (DDR), PORT registers, and PIN registers
 *
 * These arrays allow for easy access to the appropriate registers based on the Arduino pin number.
 * The index corresponds to the Arduino pin number, and the value is a pointer to the corresponding register.
 */

static volatile uint8_t* const ddr_reg[] = {
    &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, // Port D : 0-7
    &DDRB, &DDRB, &DDRB, &DDRB, &DDRB, &DDRB // Port B : 8-13
};

static volatile uint8_t* const port_reg[] = {
    &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, // Port D : 0-7
    &PORTB, &PORTB, &PORTB, &PORTB, &PORTB, &PORTB // Port B : 8-13
};

static volatile uint8_t* const pin_reg[] = {
    &PIND, &PIND, &PIND, &PIND, &PIND, &PIND, &PIND,
    &PIND, // 0-7
    &PINB, &PINB, &PINB, &PINB, &PINB, &PINB // 8-13
};

//////////////////////////////////////////////////////////////////////////////////
/// Public API Functions
//////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Set the direction of a GPIO pin
 *
 * @param pin The Arduino pin number (0-13)
 * @param direction The desired direction (INPUT, OUTPUT, INPUT_PULLUP)
 *
 * This function configures the specified pin as an input, output, or input with pull-up resistor.
 * It uses the appropriate Data Direction Register (DDR) and PORT register to set the pin configuration.
 */
void GPIO_setPinDirection(uint8_t pin, uint8_t direction)
{
    if (pin > 13)
        return;

    uint8_t portPin = pinMapping[pin];
    volatile uint8_t* ddr = ddr_reg[pin];

    uint8_t oldSREG = SREG; // Save the current state of the global interrupt flag
    cli(); // disable interrupts to ensure atomic operation while modifying registers

    // Block to handle INPUT, OUTPUT, INPUT_PULLUP.
    switch (direction) {
    case GPIO_INPUT:
        *ddr &= ~(1 << portPin); // Clear the bit to set as input
        break;
    case GPIO_OUTPUT:
        *ddr |= (1 << portPin); // Set the bit to set as output
        break;
    case GPIO_INPUT_PULLUP:
        *ddr &= ~(1 << portPin); // Clear the bit to set as input
        *port_reg[pin] |= (1 << portPin); // Enable pull-up resistor
        break;
    }

    SREG = oldSREG; // Restore the previous state of the global interrupt flag
}

/**
 * @brief Write a value to a GPIO pin
 *
 * @param pin The Arduino pin number (0-13)
 * @param value The value to write (HIGH or LOW)
 *
 * This function sets the specified pin to HIGH or LOW by modifying the appropriate PORT register.
 */
void GPIO_writePin(uint8_t pin, uint8_t value)
{
    if (pin > 13)
        return; // Invalid pin

    uint8_t oldSREG = SREG; // Save the current state of the global interrupt flag
    cli(); // disable interrupts to ensure atomic operation while modifying registers

    if (value == 1) {
        *port_reg[pin] |= (1 << pinMapping[pin]); // Set the bit to HIGH
    } else {
        *port_reg[pin] &= ~(1 << pinMapping[pin]); // Clear the bit to LOW
    }

    SREG = oldSREG; // Restore the previous state of the global interrupt flag
}

/**
 * @brief Toggle the state of a GPIO pin
 *
 * @param pin The Arduino pin number (0-13)
 *
 * This function toggles the state of the specified pin by XORing the appropriate PORT register bit.
 */
void GPIO_togglePin(uint8_t pin)
{
    if (pin > 13)
        return; // Invalid pin

    uint8_t oldSREG = SREG; // Save the current state of the global interrupt flag
    cli(); // disable interrupts to ensure atomic operation while modifying registers

    *port_reg[pin] ^= (1 << pinMapping[pin]); // Toggle the bit

    SREG = oldSREG; // Restore the previous state of the global interrupt flag
}

/**
 * @brief Read the state of a GPIO pin
 *
 * @param pin The Arduino pin number (0-13)
 * @return uint8_t The state of the pin (HIGH or LOW)
 *
 * This function reads the current state of the specified pin by checking the appropriate PIN register bit.
 *
 * @warning The function returns LOW for invalid pin numbers, so the caller should ensure that the pin number is within
 * the valid range (0-13) before calling this function.
 */
uint8_t GPIO_readPin(uint8_t pin)
{
    if (pin > 13)
        return 0; // Invalid pin

    uint8_t pinState = (*pin_reg[pin] & (1 << pinMapping[pin])) ? 1 : 0; // Read the bit state
    return pinState;
}
