#include "../../include/Config/Config.h"

std::string Config::WIFI_SSID = Secrets::WIFI_SSID;
std::string Config::WIFI_PASSWORD = Secrets::WIFI_PASSWORD;

Config::Config(uint16_t port)
:    port(port)
{}

Config::Config()
:    port(4242)
{}

Config::~Config()
{}