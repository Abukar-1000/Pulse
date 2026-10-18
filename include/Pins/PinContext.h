#ifndef PinContext_h
#define PinContext_h
#include "Pin.h"
#include <memory>
#include <vector>

class PinContext
{
    private:
        std::vector<std::shared_ptr<Pin>> Pins;
        std::unique_ptr<Pin> GetPin(uint8_t id);
        void InitializePins(void);
    public:
        PinContext();
        ~PinContext();
        void SetAllPinsOff(void);
        const size_t Size(void) noexcept;
        const std::shared_ptr<Pin> Get(const uint8_t id);
};

#endif