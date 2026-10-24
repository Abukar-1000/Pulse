#ifndef MutexLock_h0
#define MutexLock_h
#include <memory>
#include "pico/mutex.h"
#include "pico/stdlib.h"

namespace Locks::Mutex
{
    class MutexLock 
    {
        private:
            static mutex_t lock;
            static bool initialized;
        public:
            ~MutexLock(void);
            MutexLock(void);
            void Lock(void) noexcept;
            void WaitAndLock(void) noexcept;
            void Unlock(void) noexcept;
    };
}
#endif