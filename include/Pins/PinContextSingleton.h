#ifndef PinContextSingeleton_h
#define PinContextSingeleton_h
#include "PinContext.h"

class PinContextSingleton
{
    private:
        static std::shared_ptr<PinContext> instance;
        static void Init(void) noexcept;
        PinContextSingleton(void) {}
    public:
        static std::shared_ptr<PinContext> Get(void);
};

#endif