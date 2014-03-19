#ifndef _BHMUTEX_HPP_
#define _BHMUTEX_HPP_

#include <util/BhNonCopy.hpp>
#include <pthread.h>

class BhMutex
    :public BhNonCopy
{
public:
    BhMutex()
    {
        ::pthread_mutex_init(&m_mutex, NULL);
	}
    ~BhMutex()
    {
        ::pthread_mutex_destroy(&m_mutex);
    }
    void Lock()
    {
        ::pthread_mutex_lock(&m_mutex);
    }
    void UnLock()
    {
        ::pthread_mutex_unlock(&m_mutex);
    }
    bool TryLock()
    {
        return !::pthread_mutex_trylock(&m_mutex);
    }
    pthread_mutex_t* GetHandle()
    {
        return &m_mutex;
    }
private:
    pthread_mutex_t m_mutex;
};

#endif
