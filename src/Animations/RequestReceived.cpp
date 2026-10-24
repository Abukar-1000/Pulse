#include "../../include/Animations/RequestReceived.h"

namespace Animations
{
    RequestReceived::RequestReceived(uint16_t blinkCount)
    :   blinkCount(blinkCount)
    {}

    RequestReceived::RequestReceived(void)
    {}

    void RequestReceived::Run(void)
    {
        const uint16_t Delay = 100;
        const uint16_t PwmOnHigh = 65535;
        const uint8_t GreenLedPin = 2;
        const uint8_t MidBlueLedPin = 8;
        const uint8_t MaxLedPin = 12;
        
        auto context = PinContextSingleton::Get()->WithLock(true);
        this->TurnOffLLeds();

        context->Get(MidBlueLedPin)->TrySet(0, 2.0f);
        for (uint8_t i = 0; i <= this->blinkCount; ++i)
        {
            context->Get(MidBlueLedPin)->TrySet(PwmOnHigh, 2.0f);
            sleep_ms(Delay);
            context->Get(MidBlueLedPin)->TrySet(0, 2.0f);
            sleep_ms(50);
        }

        for (uint8_t i = GreenLedPin; i > 0; ++i)
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

        this->TurnOffLLeds();
        context->ReleaseLock();
    }

    void RequestReceived::TurnOffLLeds(void) noexcept
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

    RequestReceived::~RequestReceived(void)
    {}
}