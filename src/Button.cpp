#include <Button.h>

Button::Button(uint8_t pin, char *name)
{
    name_ = name;
    pin_ = pin;
}

void Button::begin()
{
    pinMode(pin_, INPUT_PULLUP);//Dont need to use external pul up resistor
}