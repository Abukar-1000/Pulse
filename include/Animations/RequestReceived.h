#ifndef RequestReceived_h
#define RequestReceived_h
#include "IAnimation.h"

namespace Animations
{
    class RequestReceived: public IAnimation
    {
        private:
            void TurnOffLLeds(void) noexcept;
            uint16_t blinkCount;
        public:
            ~RequestReceived(void);
            RequestReceived(void);
            RequestReceived(uint16_t blinkCount = 15);
            virtual void Run(void) override;
    };
}
#endif