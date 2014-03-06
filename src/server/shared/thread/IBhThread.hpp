#ifndef _IBHTHREAD_HPP_
#define _IBHTHREAD_HPP_

#include "../BhNonCopy.hpp"
#include <pthread.h>

class IBhThread
    :public BhNonCopy
{
public:
    IBhThread(){}
    virtual ~IBhThread(){}
public:
    bool Start()
    {
        return !::pthread_create(&m_tid,NULL,ThreadMain,this);
    }
    void Join()
    {
        ::pthread_join(m_tid, NULL);
    }
    pthread_t GetID() const
    {
        return m_tid;
    }
protected:
    virtual void Run() = 0;
    static void* ThreadMain(void *pParam)
    {
        IBhThread *pThread = static_cast<IBhThread *>(pParam);
        if (pThread)
        {
            pThread->Run();
            pThread->m_tid = 0;
        }
        return reinterpret_cast<void *>(0);
    }
private:
    pthread_t m_tid;
};

#endif
