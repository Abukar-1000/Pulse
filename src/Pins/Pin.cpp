#include "../../include/Pins/Pin.h"

bool Pin::SetGpioMode(const bool state, const bool direction)
{
    if (this->isPWM)
    {
        pwm_set_enabled(
            pwm_gpio_to_slice_num(this->id),
            false
        );
        gpio_set_function(this->id, GPIO_FUNC_SIO);
    }

    gpio_init(this->id);
    gpio_set_dir(this->id, direction);
    gpio_set_dir(this->id, state);
    
    this->SetInternals(
        false, 
        0, 
        this->pwmFrequency == NULL? 2.0f : this->pwmFrequency,
        state,
        direction
    );    
    return true;
}

bool Pin::SetPwmMode(const uint16_t state, const float frequency)
{
    gpio_set_function(this->id, GPIO_FUNC_PWM);
    uint sliceNumber = pwm_gpio_to_slice_num(this->id);
    pwm_set_clkdiv(sliceNumber, frequency);
    pwm_set_wrap(sliceNumber, 62499);
    pwm_set_gpio_level(this->id, state);
    pwm_set_enabled(sliceNumber, true);
    
    this->SetInternals(
        true, 
        state, 
        frequency,
        false,
        false
    );
    return true;
}

bool Pin::IsGpio(void) const noexcept
{
    return (
        this->id < NUM_BANK0_GPIOS &&
        gpio_get_function(this->id) == GPIO_FUNC_SIO
    );
}

void Pin::TurnOff(void)
{
    if (!this->IsGpio())
    {
        return;
    }
    
    if (this->isPWM)
    {
        this->TrySet(0, this->pwmFrequency);
        return;
    }
    
    this->TrySet(false, this->gpioDirection);
}

bool Pin::TrySet(const bool state = true, const bool direction = true)
{
    // add checks here 
    return this->SetGpioMode(state, direction);
}

bool Pin::TrySet(const uint16_t state = 31250, const float frequency = 2.0f)
{
    // add checks here 
    return this->SetPwmMode(state, frequency);
}

void Pin::SetInternals(
    const bool isPwm,
    const bool pwmState,
    const float pwmFrequency,
    const bool gpioState,
    const bool gpioDirection
)
{
    this->isPWM = isPWM;
    this->pwmState = pwmState;
    this->pwmFrequency = pwmFrequency;
    this->gpioState = gpioState;
    this->gpioDirection = gpioDirection;
}

Pin::Pin()
:   id(0),
    isPWM(false),
    pwmState(0),
    pwmFrequency(2.0f),
    gpioState(false),
    gpioDirection(false)
{}

// PWM Constructor
Pin::Pin(uint8_t id, uint16_t state, float frequency)
:   id(id),
    isPWM(true),
    pwmState(state),
    pwmFrequency(frequency),
    gpioState(false),
    gpioDirection(false)
{
    this->SetPwmMode(state, frequency);
}

// GPIO constructor 
Pin::Pin(uint8_t id, bool state = true, bool direction = true)
:   id(id),
    isPWM(false),
    pwmState(0),
    pwmFrequency(2.0f),
    gpioState(state),
    gpioDirection(direction)
{
    this->SetGpioMode(state, direction);
}


Pin::~Pin()
{}