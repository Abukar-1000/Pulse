#include "../../include/Pins/PinContext.h"

void PinContext::InitializePins(void)
{
    const uint8_t pinCount = 30;
    if (this->Pins.size() < pinCount)
    {
        this->Pins.clear();
        this->Pins.reserve(pinCount);
    }

    for (uint8_t i = 0; i < pinCount; i++)
    {
        this->Pins.push_back(std::make_shared<Pin>(i, false, true));
    }
}

const std::shared_ptr<Pin> PinContext::Get(const uint8_t id)
{
    if (id > this->Pins.size() - 1)
    {
        return std::make_shared<Pin>(500, false, false);
    }

    return this->Pins.at(id);
}

void PinContext::SetAllPinsOff(void)
{
    for (auto& pin: this->Pins)
    {
        pin->TurnOff();
    }
}

const size_t PinContext::Size(void) noexcept
{
    return this->Pins.size();
}

PinContext::PinContext()
{
    this->InitializePins();
}

PinContext::~PinContext()
{
    this->Pins.clear();
}