#ifndef _BHMUTEX_HPP_
#define _BHMUTEX_HPP_

#include <pthread.h>

class BhMutex
{
public:
    BhMutex();
    ~BhMutex();
private:
    BhMutex(const BhMutex&);
    BhMutex& operator=(const BhMutex&)；
public:
    int Init();
    void Clear();
    int Lock();
    int UnLock();
    int TryLock();
    const pthread_mutex_t* GetHandle() const;
private:
    pthread_mutex_t* m_pMutex;
};

#endif //_BHMUTEX_HPP_
