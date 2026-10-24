#ifndef FailedToConnect_h
#define FailedToConnect_h
#include "IAnimation.h"

namespace Animations
{
    class FailedToConnect: public IAnimation
    {
        private:
            void TurnOffLLeds(void) noexcept;
        public:
            ~FailedToConnect(void);
            FailedToConnect(void);
            virtual void Run(void) override;
    };
}
#endif