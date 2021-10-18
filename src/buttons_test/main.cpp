#include <Arduino.h>
#include <Button.h>

Button start(15, "Star");
Button stop(26, "Stop");

void IRAM_ATTR startHandle();
void startLoop();
bool startOn = false;

void IRAM_ATTR stopHandle();
void stopLoop();
bool stopOn = false;

void setup()
{
    Serial.begin(115200);
    start.begin();
    stop.begin();
}
void loop()
{
    startLoop();
    stopLoop();
}

void IRAM_ATTR startHandle()
{
    detachInterrupt(start.pin());
    startOn = false;
    static int cont = 0;
    Serial.println(cont);
    cont++;
}

void startLoop()
{
    if (!startOn)
    {
        delay(1000);
        startOn = true;
        attachInterrupt(start.pin(), startHandle, FALLING);
    }
}

void IRAM_ATTR stopHandle()
{
    detachInterrupt(stop.pin());
    stopOn = false;
    static int cont = 0;
    Serial.println(cont);
    cont++;
}

void stopLoop()
{
    if (!stopOn)
    {
        delay(1000);
        stopOn = true;
        attachInterrupt(stop.pin(), stopHandle, FALLING);
    }
}