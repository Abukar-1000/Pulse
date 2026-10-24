#include "../../include/Locks/MutexLockSingleton.h"

namespace Locks::Mutex
{
    
    std::shared_ptr<MutexLock> MutexLockSingleton::instance = nullptr;
    void MutexLockSingleton::Init(void) noexcept
    {
        if (MutexLockSingleton::instance == nullptr)
        {
            instance = std::make_shared<MutexLock>();
        }
    }

    std::shared_ptr<MutexLock> MutexLockSingleton::Get(void)
    {
        MutexLockSingleton::Init();
        return MutexLockSingleton::instance;
    }
}