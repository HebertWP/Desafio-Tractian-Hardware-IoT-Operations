#define FIRWAREVERSION "0.01"
#include <Connect.h>
#include <SSLClient.h>
#include "AWS_Root_CA.h" // This file is created using AmazonRootCA1.pem from https://www.amazontrust.com/repository/AmazonRootCA1.pem

const uint8_t ledPin = 17;
const uint8_t pwrPin = 2;
const int8_t txPin = 4;
const int8_t rxPin = 16;

Connect IOT(txPin, rxPin, pwrPin);

void initSetup();
//void readHandle(void *pvParameters);

void setup()
{
    initSetup(); //Initial Setup
    IOT.begin();
    Serial.printf("Setup end...\r\n");
    //xTaskCreatePinnedToCore(readHandle, "READ", 10000, NULL, 1, NULL, 0);
}

void loop()
{
    IOT.handle();
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(60);
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
/*
bool isClearToSaveData(int64_t timeToSave, int64_t *saveT0)
{
    int64_t t = esp_timer_get_time();
    if (t - *saveT0 >= timeToSave)
    {
        return true;
    }
    else if (*saveT0 > t)
        *saveT0 = t;
    return false;
}

void readHandle(void *pvParameters)
{
    while (true)
    {
        vTaskDelay(10);
    }
}
*/