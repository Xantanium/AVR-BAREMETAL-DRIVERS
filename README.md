# UNBOX Robotics Assignment 2 : UART_drv

This branch of the repository contains the code and the documentation for the UART driver assignment. PlatformIO cli with nvim was used for development. PlatformIO is used as it provides a convenient way to manage, build and debug the project, but no platformio-specific api is used within the code. The code was tested on the Wokwi simulator.

## Code Organization

The code organization is as follows:

```plaintext
.
├── lib
│   └── UART_drv
│       ├── UART_drv.c
│       └── UART_drv.h
├── platformio.ini
├── README.md
└── src
    └── main.cpp
```

The driver header is placed in the `lib/` folder for organization purposes, and the main application code is in the `src/` folder. Below is the public API that the header provides:

```c
void UART_init(uint32_t baudRate);

uint8_t UART_availableCount(void);
uint8_t UART_availableBool(void);

void UART_transmitByte(uint8_t data);
void UART_transmitString(const char* text);

uint8_t UART_receiveByte(void);
```

These functions have function level comments for LSP level help.

## API Algorithm Overview

UART on Atmega328P is implemented using the USART registers. Each function implements exactly what its name suggests. Implementation level details are documented in the code itself through function level comments.

## Important Notes

- The `UART_init()` function must be called before any other UART functions to initialize the USART registers with the correct baud rate. It does not enable global interrupts itself, it is to be done in the application code by the user.
- `UART_receiveByte()` returns 0 when the buffer is empty. Always call `UART_availableBool()` before calling `UART_receiveByte()` to avoid ambiguity with a valid `0x00` byte.
- The RX circular buffer is 64 bytes. Bytes received beyond this capacity are silently dropped. Ensure the application reads from the buffer frequently enough to avoid overflow.

## Build and Flash Instructions

PlatformIO cli can be used to build and flash the code to the Arduino UNO. Below are the commands:

```bash
pio run # To build the code
pio run -t upload # To flash the code to the Arduino UNO
pio device monitor # To monitor the serial output from the Arduino UNO
```
