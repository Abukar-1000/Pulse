#ifndef IConnection_h
#define IConnection_h
#include <memory>

namespace Connections
{
    class IConnection
    {
        public:
            virtual void Start(void) = 0;
            virtual void Stop(void) = 0;
    };
}
#endif