#include <vector>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "./include/Pins/PinContextSingleton.h"
#include "./include/Animations/ConnectedToWifi.h"

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

void setup()
{
    stdio_init_all();
    cyw43_arch_init();
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
    const uint16_t increment = 20000;
    auto context = PinContextSingleton::Get();
    for(uint8_t id = LED_1; id <= LED_6; ++id)
    {
        context->Get(id)->TrySet(brightness, 2.0f);
    }
    
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
    bool increasing = true;
    uint16_t brightness = MIN_BRIGHTNESS;
    Animations::ConnectedToWifi animation;
    while (1)
    {
        cyw43_arch_gpio_put(on_board_led_pin, SET_ON);
        sleep_ms(50);
        cyw43_arch_gpio_put(on_board_led_pin, SET_OFF);
        // fadeAll(brightness, increasing);
        sleep_ms(30);
        animation.Run();
    }

    return 0;
}