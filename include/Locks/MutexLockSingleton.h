#ifndef MutexLockSingleton_h
#define MutexLockSingleton_h
#include <memory>
#include "./MutexLock.h"

namespace Locks::Mutex
{
    class MutexLockSingleton 
    {
        private:
            static std::shared_ptr<MutexLock> instance;
            static void Init(void) noexcept;
            MutexLockSingleton(void) {}
        public:
            static std::shared_ptr<MutexLock> Get(void);
    };
}
#endif