#include "../../include/Locks/MutexLock.h"

namespace Locks::Mutex
{
    mutex_t MutexLock::lock;
    bool MutexLock::initialized = false;

    void MutexLock::Unlock(void)
    {
        mutex_exit(&MutexLock::lock);
    }

    void MutexLock::Lock(void)
    {
        mutex_enter_blocking(&MutexLock::lock);
    }

    void MutexLock::WaitAndLock(void)
    {
        while(!mutex_try_enter(&MutexLock::lock, nullptr))
        {
            sleep_ms(1);
        }
    }

    MutexLock::MutexLock(void)
    {
        if (MutexLock::initialized)
        {
            return;
        }

        mutex_init(&MutexLock::lock);
        MutexLock::initialized = true;
    }

    MutexLock::~MutexLock(void)
    {}
}