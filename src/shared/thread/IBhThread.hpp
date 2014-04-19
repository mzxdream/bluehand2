#ifndef _IBHTHREAD_HPP_
#define _IBHTHREAD_HPP_

#include <pthread.h>

class IBhThread
{
public:
    IBhThread();
    virtual ~IBhThread();
private:
    IBhThread(const IBhThread&);
    IBhThread& operator=(const IBhThread&);
public:
    int Start();
    int Join();
    pthread_t GetThreadID() const;
protected:
    virtual void Run() = 0;
private:
    static void* ThreadMain(void *pParam);
private:
    pthread_t m_tid;
};

#endif
