#ifndef Config_h
#define Config_h

#include "Secrets.h"
#include "pico/stdlib.h"

class Config
{
    public:
        static std::string WIFI_SSID;
        static std::string WIFI_PASSWORD;
        uint16_t port;
        ~Config();
        Config();
        Config(uint16_t port);
};
#endif