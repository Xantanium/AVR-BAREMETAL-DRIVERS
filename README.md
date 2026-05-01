# UNBOX Robotics Assignment 1 : GPIO_drv

This branch of the repository contains the code and the documentation for the GPIO driver assignment. PlatformIO cli with nvim was used for development. PlatformIO is used as it provides a convenient way to manage, build and debug the project, but no platformio-specific api is used within the code. The code was tested on the Wokwi simulator.

## Code Organization

The code organization is as follows:

```plaintext
.
├── lib
│   └── GPIO_drv
│       ├── GPIO_drv.c
│       └── GPIO_drv.h
├── platformio.ini
├── README.md
└── src
    └── main.cpp
```

The driver header is placed in the `lib/` folder for organization purposes, and the main application code is in the `src/` folder. Below is the public API that the header provides:

```c
void GPIO_setPinDirection(uint8_t pin, uint8_t direction);
void GPIO_writePin(uint8_t pin, uint8_t value);
void GPIO_togglePin(uint8_t pin);
uint8_t GPIO_readPin(uint8_t pin);
```

These functions have function level comments for LSP level help.

## API Algorithm Overview

All of the given functions follow a similar algorithmic structure of setting register values based on the pin number.
Arduino Schematic was used to determine the corresponding registers for each pin. The functions use bit manipulation to set, clear, or toggle the appropriate bits in the registers to achieve the desired functionality.

### Hardware Mapping and Pin Section

Arduino UNO maps the pins to the ports in below manner. Each port is an 8 bit register.

```plaintext
pin 0-7 -> PORTD, bits 0-7
pin 8-13 -> PORTB, bits 0-5
```

### Known Limitations / Assumptions

The `GPIO_readPin()` function returns 0 if the pin number is out of range, which may not be ideal in all cases. It would be better to return an error code or use a different mechanism to indicate an invalid pin number. Since this is a standalone API, it is assumed that the user will handle such cases in their code.

### API Interrupt Safety

The api is made interrupt-safe by below snippet where read-modify-write operations are performed:

```c
{
    uint8_t oldSREG = SREG; // Save the current state of the global interrupt flag
    cli(); // disable interrupts to ensure atomic operation while modifying registers

    // Register manipulation code goes here

    SREG = oldSREG; // Restore the previous state of the global interrupt flag
}
```

## Build and Flash Intructions

PlatformIO cli can be used to build and flash the code to the Arduino UNO. Below are the commands:

```bash
pio run # To build the code
pio run -t upload # To flash the code to the Arduino UNO
pio device monitor # To monitor the serial output from the Arduino UNO
```
