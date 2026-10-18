#ifndef IAnimation_h
#define IAnimation_h
#include "pico/stdlib.h"
#include "../Pins/PinContextSingleton.h"

namespace Animations
{
    class IAnimation 
    {
        public:
            virtual void Run(void) = 0;
    };
}
#endif