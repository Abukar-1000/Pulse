#include "../../include/Animations/ConnectedToWifi.h"

namespace Animations
{
    ConnectedToWifi::ConnectedToWifi(void)
    {
        auto context = PinContextSingleton::Get();
        context->SetAllPinsOff();
    }

    void ConnectedToWifi::Run(void)
    {
        const uint16_t Delay = 100;
        const uint16_t PwmOnHigh = 65535;
        const uint8_t iterations = 4;
        const uint8_t GreenLedPin = 2;
        const uint8_t MaxLedPin = 12;
        
        auto context = PinContextSingleton::Get();
        context->Get(GreenLedPin)->TrySet(0, 2.0f);
        for (uint8_t i = 0; i <= iterations; ++i)
        {
            context->Get(GreenLedPin)->TrySet(PwmOnHigh, 2.0f);
            sleep_ms(Delay);
            context->Get(GreenLedPin)->TrySet(0, 2.0f);
            sleep_ms(50);
        }

        for (uint8_t i = 0; i <= MaxLedPin; ++i)
        {
            bool isValidLedPin = (
                i == 2 || 
                i == 4 || 
                i == 7 || 
                i == 8 || 
                i == 10 || 
                i == 12
            );
            
            if (isValidLedPin)
            {
                context->Get(i)->TrySet(PwmOnHigh, 2.0f);
            }

            sleep_ms(50);
        }

        for (uint8_t i = 0; i <= MaxLedPin; ++i)
        {
            bool isValidLedPin = (
                i == 2 || 
                i == 4 || 
                i == 7 || 
                i == 8 || 
                i == 10 || 
                i == 12
            );
            
            if (isValidLedPin)
            {
                context->Get(i)->TrySet(0, 2.0f);
            }
        }
    }

    ConnectedToWifi::~ConnectedToWifi(void)
    {}
}