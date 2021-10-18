#ifndef __Buttons_H__
#define __Buttons_H__

#include <Arduino.h>
#include <functional>

class Button
{
private:
    uint8_t pin_;
    char* name_;

public:
    Button(uint8_t pin, char* name);
    void begin();
    uint8_t pin(){return pin_;};
};

#endif