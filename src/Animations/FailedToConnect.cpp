#include "../../include/Animations/FailedToConnect.h"

namespace Animations
{
    FailedToConnect::FailedToConnect(void)
    {}

    void FailedToConnect::Run(void)
    {
        const uint16_t Delay = 100;
        const uint16_t PwmOnHigh = 65535;
        const uint8_t iterations = 10;
        const uint8_t GreenLedPin = 2;
        const uint8_t RedLedPin = 12;
        const uint8_t MaxLedPin = 12;
        
        auto context = PinContextSingleton::Get()->WithLock(true);
        this->TurnOffLLeds();

        context->Get(RedLedPin)->TrySet(0, 2.0f);
        for (uint8_t i = 0; i <= iterations; ++i)
        {
            context->Get(RedLedPin)->TrySet(PwmOnHigh, 2.0f);
            sleep_ms(Delay);
            context->Get(RedLedPin)->TrySet(0, 2.0f);
            sleep_ms(50);
        }

        for (uint8_t i = MaxLedPin; i > GreenLedPin; --i)
        {
            bool isValidLedPin = (
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

        this->TurnOffLLeds();
        context->ReleaseLock();
    }

    void FailedToConnect::TurnOffLLeds(void) noexcept
    {
        // Assume lock has been aquired before calling
        const uint8_t MaxLedPin = 12;
        auto context = PinContextSingleton::Get();
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

    FailedToConnect::~FailedToConnect(void)
    {}
}