#include <Connect.h>

const char *apnName = "timbrasil.br";
const char *apnUser = "tim";
const char *apnPswd = "tim";

Connect::Connect(int8_t txPin, int8_t rxPin, int8_t pwrPin)
{
    txPin_ = txPin;
    rxPin_ = rxPin;
    pwrPin_ = pwrPin;
    serialAT_ = new HardwareSerial(1);
    modem_ = new TinyGsm(*serialAT_);

    clientnoSSL_ = new TinyGsmClient(*modem_);

    client_ = new SSLClient(*clientnoSSL_, TAs, (size_t)TAs_NUM, 36, 1, SSLClient::DebugLevel::SSL_ERROR);

    mqtt_ = new PubSubClient();
}

void Connect::begin()
{
    this->getCertificates(); //get base info need to connect to internet and some more

    const char *mqttServer = (*set_)["mqttServer"];
    int mqttPort = (*set_)["mqttPort"];

    mqtt_->setServer(mqttServer, mqttPort); //mqtt client
    mqtt_->setBufferSize(10240);

    serialAT_->begin(9600, SERIAL_8N1, txPin_, rxPin_);
    pinMode(pwrPin_, OUTPUT);
    digitalWrite(pwrPin_, LOW);

    this->initModem(); //init sim868
}

bool Connect::initModem()
{
    bool out = false;

    digitalWrite(pwrPin_, HIGH);
    delay(4000);
    digitalWrite(pwrPin_, LOW);
    delay(4000);

    mqtt_->clearWriteError();
    mqtt_->setClient(*client_);
    if(!modem_->isGprsConnected())
        modem_->gprsConnect(apnName, apnUser, apnPswd);
    if (modem_->waitForNetwork())
    {
        Serial.printf("gprs connected\r\n");
        out = true;
    }
    else
    {
        Serial.printf("gprs fail connection\r\n");
    }
    return out;
}

bool Connect::handle()
{
    this->reconnect();
    if (client_->getWriteError() != 0)
        client_->clearWriteError();
    client_->flush();
    return true;
}
bool Connect::handle(const char *info)
{
    this->handle();
    bool ret = false;
    
    if (mqtt_->connected())
    {
        ret = mqtt_->publish((*set_)["device_id"], info);
        Serial.printf((*set_)["device_id"]);
        delay(100);
    }
    return ret;
}

void Connect::reconnect()
{
    mqtt_->loop();
    if (!mqtt_->connected())
    {
        Serial.printf("Attempting MQTT connection...\r\n");
        if (mqtt_->connect("arduinoClient"))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqtt_->state());
            Serial.printf("\n\n\n");
            delay(1000);
        }
    }
    if (client_->getWriteError() != 0)
        client_->clearWriteError();
    client_->flush();
}

void Connect::getCertificates()
{
    int tamCert = 0, tamKey = 0;
    if (!SPIFFS.begin(true) || !SPIFFS.exists("/certificate.pem.crt") || !SPIFFS.exists("/private.pem.key"))
    {
        Serial.println("Faill in certifications");
        delay(1000);
        ESP.restart();
    }

    File certificate = SPIFFS.open("/certificate.pem.crt", FILE_READ);
    File privateKey = SPIFFS.open("/private.pem.key", FILE_READ);
    File base = SPIFFS.open("/base.json", FILE_READ);
    File set = SPIFFS.open("/set.json", FILE_READ);

    tamCert = certificate.size() + 2;
    tamKey = privateKey.size() + 2;

    cert_ = (char *)malloc((tamCert + 1) * sizeof(char));
    key_ = (char *)malloc((tamKey + 1) * sizeof(char));

    cert_[0] = '\n';
    cert_[tamCert - 1] = '\0';
    key_[0] = '\n';
    key_[tamKey - 1] = '\0';

    privateKey.read((uint8_t *)&(key_[1]), privateKey.size());
    certificate.read((uint8_t *)&(cert_[1]), certificate.size());

    certificate.close();
    privateKey.close();
    base_ = new DynamicJsonDocument(100);
    set_ = new DynamicJsonDocument(200);

    DeserializationError erro = deserializeJson(*set_, set);
    if (erro != DeserializationError::Ok)
    {
        Serial.println("Erro in ./set.json");
        delay(300);
        ESP.restart();
    }
    else
    {
        set.close();
    }
    erro = deserializeJson(*base_, base);
    if (erro != DeserializationError::Ok)
    {
        Serial.println("Erro in ./base.json");
        delay(300);
        ESP.restart();
    }
    else
        base.close();

    static SSLClientParameters mTLS = SSLClientParameters::fromPEM(cert_, tamCert, key_, tamKey);
    client_->setMutualAuthParams(mTLS);
}