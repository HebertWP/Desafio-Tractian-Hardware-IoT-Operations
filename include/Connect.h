/**
 * @file Connect.h
 * @author Hebert Wandick Parreira (wandick.parreira98@gmail.com)
 * @brief This class is built to facilitend the usage of internet module
 * @version 0.1
 * @date 2021-10-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */
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
#include <SPIFFS.h>//Use to get data from memory

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
    /**
     * @brief Construct a new Connect object
     * 
     * @param txPin 
     * @param rxPin 
     * @param pwrPin 
     */
    Connect(int8_t txPin, int8_t rxPin, int8_t pwrPin);
    /**
     * @brief Call once before use
     * 
     */
    void begin();

    /**
     * @brief Call in loop to keep connection
     * 
     * @return true 
     * @return false 
     */
    bool handle();

    /**
     * @brief send mensage to corret topic
     * 
     * @param info 
     * @param topic 
     * @return true 
     * @return false 
     */
    bool handle(const char * info,const char *topic);

private:
    bool initModem();
    void reconnect();
    /**
     * @brief Get the Certificates object, Load data from SPIFFS memory
     * 
     */
    void getCertificates();
};

#endif