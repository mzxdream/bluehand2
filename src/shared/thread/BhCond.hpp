#ifndef _BHCOND_HPP_
#define _BHCOND_HPP_

#include <pthread.h>
#include <shared/thread/BhMutex.hpp>

class BhCond
{
public:
    BhCond();
    ~BhCond();
private:
    BhCond(const BhCond&);
    BhCond& operator=(const BhCond&);
public:
    int Init();
    void Clear();
    int Wait(const BhMutex& mtx);
    int Notify();
    int NotifyAll();
private:
    pthread_cond_t* m_pCond;
};

#endif //_BHCOND_HPP_
