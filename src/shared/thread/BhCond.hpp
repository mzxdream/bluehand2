#ifndef _BHCOND_HPP_
#define _BHCOND_PP_

#include <pthread.h>
#include <thread/BhNonCopy.hpp>
#include <thread/BhMutex.hpp>

class BhCond
    :public BhNonCopy
{
public:
    BhCond()
    {
        ::pthread_cond_init(&m_cond, NULL);
    }
    ~BhCond()
    {
        ::pthread_cond_destroy(&m_cond);
    }
    bool Wait(BhMutex& mutex)
    {
        return !pthread_cond_wait(&m_cond, mutex.GetHandle());
    }
    bool Notify()
    {
        return !pthread_cond_signal(&m_cond);
    }
    bool NotifyAll()
    {
        return !pthread_cond_broadcast(&m_cond);
    }
private:
    pthread_cond_t m_cond;
};

#endif
