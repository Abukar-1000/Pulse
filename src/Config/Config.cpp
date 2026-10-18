#include "../../include/Config/Config.h"


Config::Config(uint16_t port)
:    port(port)
{
    Config::WIFI_SSID = Secrets::WIFI_SSID;
    Config::WIFI_PASSWORD = Secrets::WIFI_PASSWORD;
}

Config::Config()
:    port(80)
{
    Config::WIFI_SSID = Secrets::WIFI_SSID;
    Config::WIFI_PASSWORD = Secrets::WIFI_PASSWORD;
}

Config::~Config()
{}