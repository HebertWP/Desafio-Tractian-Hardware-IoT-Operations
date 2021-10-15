#include <Arduino.h>

const uint8_t ledPin = 12;

void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); //signal ending of initial setup
}

void loop()
{
  digitalWrite(ledPin, HIGH); //signal ending of initial setup
  delay(1000);
  digitalWrite(ledPin, LOW); //signal ending of initial setup
  delay(1000);
}