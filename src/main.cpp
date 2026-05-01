#include "../lib/GPIO_drv/GPIO_drv.h"
#include <Arduino.h>
#include <util/delay.h>

void setup()
{
    // Set pin 13 as OUTPUT
    GPIO_setPinDirection(13, GPIO_OUTPUT);
}

void loop()
{
    // Toggle pin 13 every second
    GPIO_togglePin(13);
    _delay_ms(500); // Delay for 0.5 second as given in the requirements
}
