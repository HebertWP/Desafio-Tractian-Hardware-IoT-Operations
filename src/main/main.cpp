#define FIRWAREVERSION "0.01"
#define MESSAGE "{\"module_id\": \"module-07\", \"sensor_id\": \"sensor-02\", \"chip_id\": \"chip-05\", \"step_id\": \"Preparation of Hardware Modules\", \"work_pos_id\": \"work-01\", \"position\": \"in\"}"
#define MESSAGEOUT "{\"module_id\": \"module-07\", \"sensor_id\": \"sensor-02\", \"chip_id\": \"chip-05\", \"step_id\": \"Preparation of Hardware Modules\", \"work_pos_id\": \"work-01\", \"position\": \"out\"}"
#define TOPIC "iot/module/pos"

#include <Arduino.h>
#include <Connect.h>
#include <SSLClient.h>
#include <Button.h>
#include "AWS_Root_CA.h" // This file is created using AmazonRootCA1.pem from https://www.amazontrust.com/repository/AmazonRootCA1.pem

const uint8_t ledPin = 17;
const uint8_t pwrPin = 2;
const int8_t txPin = 4;
const int8_t rxPin = 16;

Button start(15, "Star");
Button stop(26, "Stop");

Connect IOT(txPin, rxPin, pwrPin);

void initSetup();

void IRAM_ATTR startHandle();
void startLoop();
bool startOn = false;

void IRAM_ATTR stopHandle();
void stopLoop();
bool stopOn = false;

void setup()
{
    initSetup(); //Initial Setup
    start.begin();
    stop.begin();
    IOT.begin();
}
void loop()
{
    IOT.handle();
    startLoop();
    stopLoop();
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(10);
    uint32_t rand = esp_random();
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
    static bool first = true;
    if (!startOn)
    {
        if (!first)
        {
            IOT.handle(MESSAGE, TOPIC);
            Serial.println("IN");
        }
        else
            first = false;
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
    static bool first = true;
    if (!stopOn)
    {
        if (!first)
            IOT.handle(MESSAGEOUT, TOPIC);
        else
            first = false;
        delay(1000);
        stopOn = true;
        attachInterrupt(stop.pin(), stopHandle, FALLING);
    }
}

void initSetup()
{
    Serial.begin(115200);
    Serial.printf("Initing...\r\n");
    Serial.printf("FirmWare:");
    Serial.println(FIRWAREVERSION);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH); //signal ending of initial setup
}