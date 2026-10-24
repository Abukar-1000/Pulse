#include "../../include/Pins/PinContextSingleton.h"

std::shared_ptr<PinContext> PinContextSingleton::instance = nullptr;
void PinContextSingleton::Init(void) noexcept
{
    if (PinContextSingleton::instance == nullptr)
    {
        instance = std::make_shared<PinContext>();
    }
}

std::shared_ptr<PinContext> PinContextSingleton::Get(void)
{
    PinContextSingleton::Init();
    return PinContextSingleton::instance;
}