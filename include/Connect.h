#ifndef __Connect_H__
#define __Connect_H__

#include <Arduino.h>

#define TINY_GSM_MODEM_SIM868
#include <TinyGsmClient.h>

#include <HardwareSerial.h>
#include <PubSubClient.h>
#include <SSLClient.h>
#include <ArduinoJson.h>
#include <certificates.h>
#include <SPIFFS.h>

class Connect
{
private:
    HardwareSerial *serialAT_;
    TinyGsm *modem_;
    TinyGsmClient *clientnoSSL_;
    SSLClient *client_;
    PubSubClient *mqtt_;
    int8_t txPin_, rxPin_, pwrPin_;
    DynamicJsonDocument *data_,*base_,*set_;
    char array_[4096];
    SSLClientParameters *mTLS_;
    char *cert_, *key_;
    
public:
    Connect(int8_t txPin, int8_t rxPin, int8_t pwrPin);
    void begin();
    bool handle();

private:
//    static void callback(char *topic, byte *payload, unsigned int length);
    bool initModem();
    void reconnect();
    void getCertificates();
};

#endif