#ifndef Pin_h
#define Pin_h
#include "pico/stdlib.h"
#include "hardware/pwm.h"

class Pin
{
    protected:
        float pwmFrequency;
        uint16_t pwmState;
        uint8_t id;
        bool isPWM;
        bool gpioState;
        bool gpioDirection;
        bool SetPwmMode(const uint16_t state, const float frequency);
        bool SetGpioMode(const bool state, const bool direction);
        void SetInternals(
            const bool isPwm,
            const bool pwmState,
            const float pwmFrequency,
            const bool gpioState,
            const bool gpioDirection
        );
    public:
        ~Pin();
        Pin();
        Pin(uint8_t id, uint16_t state, float frequency);
        Pin(uint8_t id, bool state, bool direction);
        void TurnOff(void);
        bool IsGpio(void) const noexcept;
        bool TrySet(const uint16_t state, const float frequency);
        bool TrySet(const bool state, const bool direction);
};

#endif