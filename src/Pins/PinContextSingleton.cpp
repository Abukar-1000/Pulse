#include "../../include/Pins/PinContextSingleton.h"

std::shared_ptr<PinContext> PinContextSingleton::instance = nullptr;

std::shared_ptr<PinContext> PinContextSingleton::Get(void)
{
    if (PinContextSingleton::instance == nullptr)
    {
        instance = std::make_shared<PinContext>();
    }
    
    return PinContextSingleton::instance;
}