#ifndef ConnectedToWifi_h
#define ConnectedToWifi_h
#include "IAnimation.h"

namespace Animations
{
    class ConnectedToWifi: public IAnimation
    {
        private:
            void TurnOffLLeds(void) noexcept;
        public:
            ~ConnectedToWifi(void);
            ConnectedToWifi(void);
            virtual void Run(void) override;
    };
}
#endif