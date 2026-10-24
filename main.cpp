#include <vector>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "./include/Pins/PinContextSingleton.h"
#include "./include/Animations/ConnectedToWifi.h"
#include "./include/Animations/RequestReceived.h"
#include "./include/Connections/TCP/Server.h"
#include "pico/multicore.h"

#define SET_ON 1
#define SET_OFF 0
#define on_board_led_pin 0

#define LED_1 2
#define LED_2 4
#define LED_3 7
#define LED_4 8
#define LED_5 10
#define LED_6 12

#define FULL_BRIGHTNESS 65535
#define MIN_BRIGHTNESS 0

void startServer(void)
{
    cyw43_arch_init();
    Animations::RequestReceived animation(5);
    auto server = std::make_unique<Connections::TCP::Server>();
    server->Start();

    while (server->IsListening())
    {
        cyw43_arch_poll(); 
        server->ProcessActiveEvents();
    }

    server->Stop();
}

void setup()
{
    stdio_init_all();
    auto context = PinContextSingleton::Get();
    context->Get(LED_1)->TrySet(0, 2.0f);
    context->Get(LED_2)->TrySet(0, 2.0f);
    context->Get(LED_3)->TrySet(0, 2.0f);
    context->Get(LED_4)->TrySet(0, 2.0f);
    context->Get(LED_5)->TrySet(0, 2.0f);
    context->Get(LED_6)->TrySet(0, 2.0f);
}

void fadeAll(uint16_t& brightness, bool& increasing)
{
    const uint16_t increment = 100;
    auto context = PinContextSingleton::Get()->WithLock();
    for(uint8_t id = LED_1; id <= LED_6; ++id)
    {
        context->Get(id)->TrySet(brightness, 2.0f);
    }
    context->ReleaseLock();
    
    if (increasing)
    {
        if (brightness >= (FULL_BRIGHTNESS - increment))
        {
            increasing = false;
            brightness = FULL_BRIGHTNESS - 1;
            return;
        }
        
        brightness += increment;
        return;
    }
    else
    {
        if (brightness <= (MIN_BRIGHTNESS + increment))
        {
            increasing = true;
            brightness = MIN_BRIGHTNESS + 1;
            return;
        }

        brightness -= increment;
    }
}

int main()
{
    setup();
    multicore_launch_core1(startServer);
    sleep_ms(200);

    bool increasing = true;
    uint16_t brightness = MIN_BRIGHTNESS;
    while (1)
    {
        cyw43_arch_gpio_put(on_board_led_pin, SET_ON);
        sleep_ms(50);
        cyw43_arch_gpio_put(on_board_led_pin, SET_OFF);
        fadeAll(brightness, increasing);
        sleep_ms(30);
    }

    return 0;
}